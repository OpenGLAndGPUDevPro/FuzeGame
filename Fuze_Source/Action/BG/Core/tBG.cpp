// -------------------------------------------------------------------------- //
//! @file   tBG.cpp
//! @brief  背景タスク
//! @author Nal
//! @since  11/09/05(月)
//!
//! COPYRIGHT (C) 2011 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
/* ====< インクルード >====================================================== */
#include "prefix.h"
#include <TGL.h>
#include <Graphics/Model/cTGLModelData.h>
#include "sTGLMain.h"
#include "sTGLPause.h"
#include "Game/Game.h"
#include "tBG.h"
#include "Effect/Lensflare/tLensFlareSF.h"
#include "Action/ActionTypes.h"
#include "Action/Camera/cBGCamera.h"

//#define	ENABLE_INSTANCING	//!< インスタンシング有効

#ifdef _WINDOWS
#pragma warning(disable : 4355)
#endif
/* ========================================================================= */
/*		tBG																	 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  コンストラクタ "tBG"
// ---------------------------------------------
tBG::tBG()
:	mDeltaTimeLocal(1), mSceneParam(this), mbDebugInfo(false)
{
	ZeroMemory( &mTskHead, sizeof(GLTSKHEAD_T) );
	reset();
}

// ---------------------------------------------
//! @brief  デストラクタ "tBG"
// ---------------------------------------------
tBG::~tBG()
{
	//!	リセット
	reset();
	ITGLRenderer->setClearColor( IRGBA(0,0,0,255) );
}

// ---------------------------------------------
//! @brief  リセット
// ---------------------------------------------
void tBG::reset()
{
	//!	更新イベント解放
	foreach( stl::vector<cUpdateEvent*>, mUpdateEvent, it )
		SAFE_DELETE( *it );
	mUpdateEvent.clear();
	
	//!	モデル解放
	foreach( stl::vector<cBGModel*>, mModelArray, it )
		SAFE_DELETE( *it );
	mModelArray.clear();
	
	//!	モデル配置解放
	foreach( stl::vector<cModelSet*>, mModelSet, it )
		SAFE_DELETE( *it );
	mModelSet.clear();
	
	//!	SE停止
	stopSE();

	//!	タスク切断
	GLTSK_Remove( &mTskHead );
	ZeroMemory( &mTskHead, sizeof(GLTSKHEAD_T) );

	//!	変数
	mDeltaTimeLocal	= 1;
	mBGId			= BGID_NONE;
	mScene			= 0;
	mSceneOld		= 0;
	mTimePass		= 0;
	mbEnable		= false;
	mbViewer		= false;
	mbSceneApplyOff	= false;
	mbAreaWait		= IACtrl->getStep() > ACtrl::STEP_NONE && IACtrl->getStep() <= ACtrl::STEP_GAME;	//!< ゲームから来てる場合はウェイト状態で始まる（スクリプトから消す）
	mbAllScene		= false;
	mModelSetPath	= stl::nullstr();
	mCameraPath		= stl::nullstr();
	mSkyNear		= 1000.0f;
	mSkyFar			= 100000.0f;
	mSkyFov			= 0.0f;
	mSceneParam.reset();
}

// ---------------------------------------------
//! @brief  リスタート
// ---------------------------------------------
void tBG::restart()
{
	//!	SE停止
	foreach( stl::vector<TGL::SoundChannel*>, mSE, it )
		(*it)->stop();
	mSE.clear();
}

// ---------------------------------------------
//! @brief  セットアップ
//! @param  uBG	[in]	BGID_????
//! @return true:成功, false:失敗
// ---------------------------------------------
bool tBG::set( u8 uBG )
{
	if( isEnable() ){	T_ERROR("既にセットアップ済み");	return true;	}
	//!	リソースロード
	if( !load(uBG) ){	T_WARNING("リソースロード失敗");	return false;	}

	//!	初期化
	mDeltaTime	= 1.0f;
	mBGId		= uBG;
	mbEnable	= true;
	
	//!	タスク接続
	GLTSK_SetNext( GLTSK_GetBase(GTOT_BG(0)), &mTskHead, this, (s32 (*)(void*,u32))tBG::updateTask );
	
	return true;
}

// ---------------------------------------------
//! @brief  モデル追加
//! @param  strFName	[in]	ファイル名
//! @param  uType		[in]	cBGModel::TYPE_????
//! @param  uFlag		[in]	cBGModel::FLAG_????
//! @return 追加されたモデル
// ---------------------------------------------
cBGModel* tBG::addModel( const stl::string& strFName, u8 uType/*=cBGModel::TYPE_NORMAL*/, u32 uFlag/*=0*/ )
{
	cBGModel*	pModel = APP_NEW cBGModel( IResourceManager->getResourceBinary<TGL::ModelResource>(strFName, TGL::Resource::TYPE_MODEL)->Get(), uType, uFlag );
	if( !pModel ){	T_ERROR("モデル生成に失敗");	return NULL;	}

	mModelArray.push_back( pModel );
	return pModel;
}

// ---------------------------------------------
//! @brief  指定Jsonからモデル配置
//! @param  strPath	[in]	Jsonパス
//! @param  strObj	[in]	オブジェクト名
// ---------------------------------------------
void tBG::applyModelSetFromJson( const stl::string& strPath, const stl::string& strObj/*="ModelSet"*/ )
{
	TGL::JsonUtil	json( strPath );
	TGL::Json::array	jsonArray = json.getArray( strObj );
	foreach( TGL::Json::array, jsonArray, it ){
		TGL::Json::object&	work = it->get<TGL::Json::object>();
		
		for( u8 i=0 ; ; i++ ){
			stl::string	str = stl::FormatString<stl::string>( "%d", i );
			if( !work[str.c_str()].is<TGL::Json::array>() )	break;
			TGL::Json::array	jsonArray2 = work[str.c_str()].get<TGL::Json::array>();
			foreach( TGL::Json::array, jsonArray2, it2 ){
				TGL::Json::object&	work2 = it2->get<TGL::Json::object>();
				
				cModelSet*	pSet = APP_NEW cModelSet;
				pSet->setName( work2["Model"].to_str() );
				pSet->setPos( TGL::Vec3(	(f32)atof( work2["PosX"].to_str().c_str() ),
											(f32)atof( work2["PosY"].to_str().c_str() ),
											(f32)atof( work2["PosZ"].to_str().c_str() ) ) );
				pSet->setRot( TGL::Vec3(	(f32)atof( work2["RotX"].to_str().c_str() ),
											(f32)atof( work2["RotY"].to_str().c_str() ),
											(f32)atof( work2["RotZ"].to_str().c_str() ) ) );
				pSet->setLoop( atoi( work2["Loop"].to_str().c_str() ) );
				pSet->setLoopPos( TGL::Vec3(	(f32)atof( work2["LoopPosX"].to_str().c_str() ),
												(f32)atof( work2["LoopPosY"].to_str().c_str() ),
												(f32)atof( work2["LoopPosZ"].to_str().c_str() ) ) );
				pSet->setLoopRot( TGL::Vec3(	(f32)atof( work2["LoopRotX"].to_str().c_str() ),
												(f32)atof( work2["LoopRotY"].to_str().c_str() ),
												(f32)atof( work2["LoopRotZ"].to_str().c_str() ) ) );
				if( work2["OnScene"] )
					pSet->setOnScene( strtoul( work2["OnScene"].to_str().c_str(), NULL, 16 ) );
				pSet->setModelId( atoi( work2["ModelId"].to_str().c_str() ) );
				pSet->setId( atoi( work2["Id"].to_str().c_str() ) );
				
				pSet->setResAdd( true );
				mModelSet.push_back( pSet );
			}
		}
	}
	mModelSetPath = strPath;
}

// ---------------------------------------------
//! @brief  現在のモデル配置をファイル出力
// ---------------------------------------------
void tBG::saveModelSet()
{
#ifdef _WINDOWS
	if( mModelSetPath.empty() )	return;
	
	//!	ファイルハンドル
	HANDLE	hFile = TGL::nFile::openHandle( mModelSetPath.c_str() );
	if( hFile == INVALID_HANDLE_VALUE )	return;

	//!	モデル配置の総数計算
	u8	uModelTotal = 0;
	foreach( stl::vector<cModelSet*>, mModelSet, it ){
		if( !(*it)->isResAdd() )	continue;
		uModelTotal++;
	}
	if( uModelTotal <= 0 ){
		T_WARNING("tBG::saveModelSet: モデルが配置されてない");
		return;
	}

	stl::string	str(stl::nullstr());
	//!	ヘッダ
	str += "{\n";
	str += "	\"ModelSet\":[\n";
	str += "		{\n";
	
	//!	中身
	u8	i0 = 0;
	foreach( stl::vector<cModelSet*>, mModelSet, it ){
		if( !(*it)->isResAdd() )	continue;
		
		str	+= stl::FormatString<stl::string>("			\"%d\":[\n", i0 );
		str += "				{\n";

		str += stl::FormatString<stl::string>("					\"Model\"		: \"%s\",\n", (*it)->getName().c_str() );
		str += stl::FormatString<stl::string>("					\"PosX\"		: \"%7.0f\",  \"PosY\" : \"%7.0f\",  \"PosZ\" : \"%7.0f\",\n", (*it)->getPos().x, (*it)->getPos().y, (*it)->getPos().z );
		str += stl::FormatString<stl::string>("					\"RotX\"		: \"%7.0f\",  \"RotY\" : \"%7.0f\",  \"RotZ\" : \"%7.0f\",\n", (*it)->getRot().x, (*it)->getRot().y, (*it)->getRot().z );
		str += stl::FormatString<stl::string>("					\"Loop\"		: \"%d\",\n", (*it)->getLoop() );
		str += stl::FormatString<stl::string>("					\"LoopPosX\"	: \"%7.0f\",  \"LoopPosY\" : \"%7.0f\",  \"LoopPosZ\" : \"%7.0f\",\n", (*it)->getLoopPos().x, (*it)->getLoopPos().y, (*it)->getLoopPos().z );
		str += stl::FormatString<stl::string>("					\"LoopRotX\"	: \"%7.0f\",  \"LoopRotY\" : \"%7.0f\",  \"LoopRotZ\" : \"%7.0f\",\n", (*it)->getLoopRot().x, (*it)->getLoopRot().y, (*it)->getLoopRot().z );
		str += stl::FormatString<stl::string>("					\"OnScene\"	: \"%x\",\n", (*it)->getOnScene() );
		str += stl::FormatString<stl::string>("					\"ModelId\"	: \"%d\",\n", (*it)->getModelId() );
		str += stl::FormatString<stl::string>("					\"Id\"		: \"%d\"\n", (*it)->getId() );
		str += "				}\n";
		str += "			]";

		if( i0 >= uModelTotal-1 )	str += "\n";
		else						str += ",\n";

		i0++;
	}

	//!	フッター
	str += "		}\n";
	str += "	]\n";
	str += "}\n";

	//!	書き込み
	TGL::nFile::writeHandle( hFile, (void*)str.c_str() );
	TGL::nFile::closeHandle( hFile );
	//!	リロード
	IFileManager->reload( mModelSetPath );
	
	PRINT("Num:%d, Size:%u, \"%s\"\n", uModelTotal, str.length(), mModelSetPath.c_str() );
#endif	/*	_WINDOWS	*/
}

// ---------------------------------------------
//! @brief  指定モデルにアサインさせてモデルセット追加（モデルセットへのアサイン）
//! @param  strAddModel	[in]	追加するモデル名
//! @param  strModel	[in]	アサインモデル
//! @param  strBone		[in]	アサインジョイント
//! @param  bScale		[in]	true:ロケータスケールを適用
//! @return cModelSet*:成功, NULL:失敗
// ---------------------------------------------
cModelSet* tBG::addModelSetAssign( const stl::string& strAddModel, const stl::string& strModel, const stl::string& strBone, bool bScale/*=false*/ )
{
	//!	アサインするモデルセットを取得
	cModelSet*	pSetSrc = NULL;
	foreach( stl::vector<cModelSet*>, mModelSet, it ){
		if( (*it)->getName() == strModel ){
			pSetSrc = *it;
			break;
		}
	}
	if( !pSetSrc )	return NULL;
	
	return addModelSetAssignExec( strAddModel, strModel, strBone, bScale, pSetSrc->getWorldMtx() );
}

// ---------------------------------------------
//! @brief  指定モデルにアサインさせてモデルセット追加
//! @param  strAddModel	[in]	追加するモデル名
//! @param  strModel	[in]	アサインモデル
//! @param  strBone		[in]	アサインジョイント
//! @param  bScale		[in]	true:ロケータスケールを適用
//! @param  mtxOfs		[in]	オフセットマトリクス
//! @return cModelSet*:成功, NULL:失敗
// ---------------------------------------------
cModelSet* tBG::addModelSetAssignExec( const stl::string& strAddModel, const stl::string& strModel, const stl::string& strBone, bool bScale/*=false*/, const TGL::Matrix& mtxOfs/*=TGL::Matrix::UNIT*/ )
{
	cBGModel*	pModel = getModel( strModel );
	if( !pModel )	return NULL;

	//!	ロケータ取得
	if( !pModel->IsExistBone(strBone.c_str()) )	return NULL;
	TGL::Matrix	mtxLocate = *pModel->GetBoneLocalMatrix( strBone.c_str() );

	TGL::Matrix	mtx = mtxOfs;
	mtx.setW( mtxLocate.getW().apply( mtxOfs ) );

	TGL::Vec3	vRot;
	TGL::Vec3	vScale;
	mtxLocate.getTransRotScale( NULL, &vRot, &vScale );
	vRot = TGL::Vec3( RAD2DEG(vRot.x), RAD2DEG(vRot.y), RAD2DEG(vRot.z) );

	cModelSet*	pSet = APP_NEW cModelSet;
	pSet->setName( strAddModel );
	pSet->setPos( mtx.getW() );
	pSet->setSortPos( pSet->getPos() + getOrigin() );	//!< ソート比較用に、背景原点を事前に加算
	pSet->setRot( vRot );
	if( bScale )
		pSet->setScale( vScale );
//	pSet->getAttach().set( pModel, strBone );	※動く背景の場合は必要
	mModelSet.push_back( pSet );
	
	return pSet;
}

// ---------------------------------------------
//! @brief  シーンの環境マップをモデルへ適用する
//! @note   シーンの環境マップが取得できない場合は初期化される
// ---------------------------------------------
void tBG::overwriteEnvTex()
{
	if( mSceneParam.getEnvTex().empty() ){
		foreach( stl::vector<cBGModel*>, mModelArray, it )
			(*it)->OverwriteTexture();
	}else{
		foreach( stl::vector<cBGModel*>, mModelArray, it ){
			for( u8 i=0 ; i<sBG::ENVTEXNODE_MAX ; i++ ){
				if( !(*it)->IsExistTextureNode(sBG::ENV_TEXNODE[i]) )	continue;
				(*it)->OverwriteTexture( sBG::ENV_TEXNODE[i], mSceneParam.getEnvTex().c_str() );
				break;
			}
		}
	}
}

// ---------------------------------------------
//! @brief  指定Jsonからカメラパス追加
//! @param  strPath	[in]	Jsonパス
//! @param  strObj	[in]	オブジェクト名
// ---------------------------------------------
void tBG::applyCameraPathSetFromJson( const stl::string& strPath, const stl::string& strObj/*="Group"*/ )
{
	if( strPath.empty() )	return;
	TGL::JsonUtil	json( strPath );
	
	//!	カメラ取得
	BGCamera*	pCam = (BGCamera*)( ICamera->getCamera(0) );
	pCam->eraseGroup();	//!< グループ全消し

	for( u8 j=0 ; ; j++ ){
		stl::string	str = strObj + stl::FormatString<stl::string>( "%02d", j );
		if( !json.isObj(str) )	break;

		//!	グループの追加
		TGL::PathCamera::Group*	pGroup = APP_NEW TGL::PathCamera::Group;
		pCam->addGroup( pGroup );

		TGL::Json::array	jsonArray = json.getArray( str );
		foreach( TGL::Json::array, jsonArray, it ){
			TGL::Json::object&	work = it->get<TGL::Json::object>();
			
			//!	キーの追加
			for( u8 i=0 ; ; i++ ){
				stl::string	str = stl::FormatString<stl::string>( "%d", i );
				if( !work[str.c_str()].is<TGL::Json::array>() )	break;
				TGL::Json::array	jsonArray2 = work[str.c_str()].get<TGL::Json::array>();
				foreach( TGL::Json::array, jsonArray2, it2 ){
					TGL::Json::object&	work2 = it2->get<TGL::Json::object>();
					
					TGL::PathCamera::Key*	pKey = APP_NEW TGL::PathCamera::Key;
					pKey->mFrame	= (f32)atof( work2["Frame"].to_str().c_str() );
					pKey->mPos.x	= (f32)atof( work2["PosX"].to_str().c_str() );
					pKey->mPos.y	= (f32)atof( work2["PosY"].to_str().c_str() );
					pKey->mPos.z	= (f32)atof( work2["PosZ"].to_str().c_str() );
					pKey->mTar.x	= (f32)atof( work2["TarX"].to_str().c_str() );
					pKey->mTar.y	= (f32)atof( work2["TarY"].to_str().c_str() );
					pKey->mTar.z	= (f32)atof( work2["TarZ"].to_str().c_str() );
					pKey->mTwist	= (f32)atof( work2["Twist"].to_str().c_str() );
					pKey->mFov		= (f32)atof( work2["Fov"].to_str().c_str() );
					pGroup->addKey( pKey );
				}
			}
		}
	}
	mCameraPath = strPath;
}

// ---------------------------------------------
//! @brief  現在のカメラパスをファイル出力
// ---------------------------------------------
void tBG::saveCameraPath()
{
#ifdef _WINDOWS
	if( mCameraPath.empty() )	return;
	
	BGCamera*	pCam = (BGCamera*)( ICamera->getCamera(0) );
	if( pCam->getGroupNum() <= 0 )	return;

	//!	ファイルハンドル
	HANDLE	hFile = TGL::nFile::openHandle( mCameraPath.c_str() );
	if( hFile == INVALID_HANDLE_VALUE )	return;
	
	stl::string	str(stl::nullstr());
	//!	ヘッダ
	str += "{\n";
	
	//!	グループ
	for( u16 j=0 ; j<pCam->getGroupNum() ; j++ ){
		TGL::PathCamera::Group*	pGroup = pCam->getGroup(j);
	
		str	+= stl::FormatString<stl::string>("	\"Group%02d\":[\n", j );
		str += "		{\n";
		
		//!	キー
		for( u16 i=0 ; i<pGroup->getKeyNum() ; i++ ){
			TGL::PathCamera::Key*	pKey = pGroup->getKey(i);
			
			#if 0	//!	変換
				pKey->mPos += TGL::Vec3( 120000, -40000, 400000 );
				pKey->mTar += TGL::Vec3( 120000, -40000, 400000 );
			#endif
			
			str += stl::FormatString<stl::string>("			\"%d\":[\n", i );
			str += stl::FormatString<stl::string>("				{\n");
			str += stl::FormatString<stl::string>("					\"Frame\"	: \"%0.0f\",\n", pKey->mFrame );
			str += stl::FormatString<stl::string>("					\"PosX\"	: \"%6.0f\",  \"PosY\" : \"%6.0f\",  \"PosZ\" : \"%6.0f\",\n", pKey->mPos.x, pKey->mPos.y, pKey->mPos.z );
			str += stl::FormatString<stl::string>("					\"TarX\"	: \"%6.0f\",  \"TarY\" : \"%6.0f\",  \"TarZ\" : \"%6.0f\",\n", pKey->mTar.x, pKey->mTar.y, pKey->mTar.z );
			str += stl::FormatString<stl::string>("					\"Twist\"	: \"%0.1f\",\n", pKey->mTwist );
			str += stl::FormatString<stl::string>("					\"Fov\"	: \"%0.1f\"\n", pKey->mFov );
			str += "				}\n";
			str += "			]";
			
			if( i >= pGroup->getKeyNum()-1 )	str += "\n";
			else								str += ",\n";
		}

		str += "		}\n";
		if( j >= pCam->getGroupNum()-1 )	str += "	]\n";
		else								str += "	],\n";
		PRINT("  Group:%d, Key:%d\n", j, pGroup->getKeyNum() );
	}

	//!	フッター
	str += "}\n";

	//!	書き込み
	TGL::nFile::writeHandle( hFile, (void*)str.c_str() );
	TGL::nFile::closeHandle( hFile );
	//!	リロード
	IFileManager->reload( mCameraPath );
	
	PRINT("Group:%d, Size:%u, \"%s\"\n", pCam->getGroupNum(), str.length(), mCameraPath.c_str() );
#endif	/*	_WINDOWS	*/
}

// ---------------------------------------------
//! @brief  レンズフレア生成
//! @param  guide			[in]	アタッチ
//! @param  strTexPrefix	[in]	テクスチャ接頭名
//! @param  strParamRes		[in]	パラメータリソース名
//! @param  strParam		[in]	パラメータ名
//! @return 生成されたレンズフレア
// ---------------------------------------------
tLensFlareSF* tBG::createLensFlare( const TGL::MtxGuide& guide, const stl::string& strTexPrefix, const stl::string& strParamRes, const stl::string& strParam )
{
	tLensFlareSF*	pLensflare = APP_NEW tLensFlareSF(this);
	pLensflare->setAttach( guide );
	pLensflare->setCamF( TGL::Camera::CAMF_00 );
	pLensflare->addTex( IAppResource->getTexPool(strTexPrefix + "01") );
	pLensflare->addTex( IAppResource->getTexPool(strTexPrefix + "02") );
	pLensflare->addTex( IAppResource->getTexPool(strTexPrefix + "03") );
	pLensflare->addTex( IAppResource->getTexPool(strTexPrefix + "04") );
	pLensflare->generateSphere();
	//!	レンズフレアパラメータ
	TGL::JsonUtil	json( strParamRes );
	TGL::Json::array	jsonArray = json.getArray( strParam );
	foreach( TGL::Json::array, jsonArray, it ){
		TGL::Json::object&	work = it->get<TGL::Json::object>();
		pLensflare->setupFromJson( work );
	}
	return pLensflare;
}

/* ========================================================================= */
/*		更新																 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  定期更新
// ---------------------------------------------
s32 tBG::updateTask( tBG* pBG, u32 uMsg )
{
	// ---------------------------------
	//		タスクメッセージ受取		
	if( uMsg ){
		if( tstFLG(uMsg,SMSG_END) ){	// 終了
			return true;
		}
		return 0;
	}
	
	// ---------------------------------
	//		更新						
	pBG->mPerfCpu.begin();
	
	pBG->update();

	if( !IACtrl->getScene().tstSceneF(SCENEF_NORENDER) ){
		//!	デバッグ情報表示
		pBG->dispDebugInfo();

		//!	描画リスト登録
		//!	モデル描画
		ITGLRenderer->addDrawList( TGL::Renderer::DPASS_MAIN, GPRI_BG(6), pBG->mTskHead.mTot, TGL::Camera::CAMF_00, tBG::drawlistFarExec, (void*)pBG );
		ITGLRenderer->addDrawList( TGL::Renderer::DPASS_MAIN, GPRI_BG(3), pBG->mTskHead.mTot, TGL::Camera::CAMF_00, tBG::drawlistNearExec, (void*)pBG );
		//ITGLRenderer->addDrawList( TGL::Renderer::DPASS_SHADOW,  GPRI_BG(3), mTskHead.mTot, TGL::Camera::CAMF_00, tBG::drawlistNearShadowExec, (void*)this );
	}
	
	pBG->mPerfCpu.end();
	return 0;
}

// ---------------------------------------------
//! @brief  更新
// ---------------------------------------------
void tBG::update()
{
#ifdef PS4
	//!	キャラクター描画前にZバッファをクリア
//	ITGLRenderer->addDrawList( TGL::Renderer::DPASS_MAIN, GPRI_EFTPRE(-5), mTskHead.mTot, ICamera->getMainDrawFlag(), tBG::clearZBuffer, (void*)this );
#endif

	//!	ループサウンド更新
	for( stl::vector<TGL::SoundChannel*>::iterator it=mLoopKeepSE.begin() ; it!=mLoopKeepSE.end() ; ){
		if( !(*it)->onLoopKeep() )
			it = mLoopKeepSE.erase( it );
		else
			++it;
	}

	if( IMain->isPause() || IPause->isActive() )	return;

	//!	デルタタイム更新
	mDeltaTime = mDeltaTimeLocal * IACtrl->getTime().getRateStd();

	if( IACtrl->getTime().isStop() )	return;

	//!	時間経過
	mTimePass += getDeltaTime();
	
	//!	スクロール更新
	if( !isViewer() )
		updateScroll();
	
	//!	更新イベント
	if( !mbAreaWait ){
		foreach( stl::vector<cUpdateEvent*>, mUpdateEvent, it )
			(*it)->exec();
	}

	//!	モデル更新
	foreach( stl::vector<cBGModel*>, mModelArray, it ){
		cBGModel*	pModel = *it;
		if( !pModel->isActive() )							continue;	//!< 非動作設定
		if( !isTstOnScene(*pModel) )						continue;	//!< シーンチェック
		if(    pModel->getType() == cBGModel::TYPE_SKYDOME
			|| pModel->getType() == cBGModel::TYPE_SKYDOME_Y )	continue;	//!< 天球

		pModel->update( getDeltaTime() );
	}
	
	//!	シーンパラメータ更新
	mSceneParam.update( getDeltaTime() );
	if( !mbSceneApplyOff )
		mSceneParam.apply();

	mSceneOld = mScene;
}

// ---------------------------------------------
//! @brief  カメラ更新後の処理
// ---------------------------------------------
void tBG::updateCameraAfter()
{
	if( IMain->isPause() || IPause->isActive() )	return;
	if( IACtrl->getTime().isStop() )	return;
	
	//!	遠景モデルのカメラ追従更新
	foreach( stl::vector<cBGModel*>, mModelArray, it ){
		cBGModel*	pModel = *it;
		if( !pModel->isActive() )							continue;	//!< 非動作設定
		if( !isTstOnScene(*pModel) )						continue;	//!< シーンチェック
		if(    pModel->getType() != cBGModel::TYPE_SKYDOME
			&& pModel->getType() != cBGModel::TYPE_SKYDOME_Y )	continue;	//!< 天球

		pModel->update( getDeltaTime() );
	}
	
	//!	移動量をエフェクトへ通知
	IEffect->setFieldMove( -ICamera->getCamera(0)->getMove() );
}

// ---------------------------------------------
//! @brief  指定名のモデル数を取得する
//! @param  strName	[in]	名前
//! @return u8
// ---------------------------------------------
u8 tBG::getModelNum( const stl::string& strName )
{
	u8	uRet = 0;
	foreach( stl::vector<cBGModel*>, mModelArray, it ){
		if( (*it)->getModelData()->cmpResName(strName) )
			uRet++;
	}
	return uRet;
}

// ---------------------------------------------
//! @brief  指定名のモデルを取得する
//! @param  strName	[in]	名前
//! @param  uIdx	[in]	番号。同じモデルが複数ある場合に指定
//! @return モデル
// ---------------------------------------------
cBGModel* tBG::getModel( const stl::string& strName, u8 uIdx/*=0*/ )
{
	u8	i0 = 0;
	foreach( stl::vector<cBGModel*>, mModelArray, it ){
		if( (*it)->getModelData()->cmpResName(strName) ){
			if( i0 < uIdx ){	//!< 番号チェック
				i0++;
				continue;
			}
			return (*it);
		}
	}
	return NULL;
}

// ---------------------------------------------
//! @brief  指定名のモデルを取得する
//! @param  strName	[in]	名前
//! @return u8
// ---------------------------------------------
u8 tBG::getModelId( const stl::string& strName )
{
	u8	i0 = 0;
	foreach( stl::vector<cBGModel*>, mModelArray, it ){
		cBGModel*	pModel = *it;
		if( pModel->getModelData()->cmpResName(strName) )	return i0;
		i0++;
	}
	return 0;
}

// ---------------------------------------------
//! @brief  モデルのシーン有効チェック
//! @param  Model	[in]	モデル
//! @return true:有効
// ---------------------------------------------
template <typename T> bool tBG::isTstOnScene( const T& Model ) const
{
	if( mbAllScene )	return true;
	return Model.tstOnScene( (1<<mScene) ) != false;
}

// ---------------------------------------------
//! @brief  更新イベント追加
//! @return 追加されたイベント
// ---------------------------------------------
cUpdateEvent* tBG::addUpdateEvent( void (*pUpdateCB)(tBG*,bool), s8 nSceneMin, s8 nSceneMax, void (*pSetupCB)(tBG*), void (*pFinalCB)(tBG*) )
{
	cUpdateEvent*	pEvent = APP_NEW cUpdateEvent( *this, pUpdateCB, nSceneMin, nSceneMax, pSetupCB, pFinalCB );
	mUpdateEvent.push_back( pEvent );
	return pEvent;
}

/* ========================================================================= */
/*		描画																 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  遠景 描画
// ---------------------------------------------
void tBG::drawFar()
{
	mPerfGpu.begin();

	//!	天球用のカメラ設定変更
	TGL::BaseCamera*	pCam = ICamera->getRenderCamera();
	const f32	_NEAR	= mSkyNear;	//!< 遠景用ニア
	const f32	_FAR	= mSkyFar;	//!< 遠景用ファー 天球の半径500m
	const f32	_FOV	= (mSkyFov > DELTA) ? mSkyFov : pCam->getFov();	//!< 遠景用FOV
	f32	fNear	= pCam->getNear();
	f32	fFar	= pCam->getFar();
	f32	fFov	= pCam->getFov();
	pCam->setNear( _NEAR );
	pCam->setFar( _FAR );
	pCam->setFov( _FOV );
	pCam->updateViewMatrix();
	pCam->setRender( false );
	IGraphics->setWorldTransform();

	//!	モデル描画
	foreach( stl::vector<cBGModel*>, mModelArray, it ){
		cBGModel*	pModel = *it;
		if( !pModel->isActive() )						continue;	//!< 非動作設定
		if( !isTstOnScene(*pModel) )					continue;	//!< シーンチェック
		if( !pModel->tstFlag(cBGModel::FLAG_FAROBJ) )	continue;	//!< 遠景チェック
		pModel->Render( TGL::nModel::PASS_COLOR, false );
	}

	//!	固有遠景描画
	if( drawFarEx() ){
		//!	ビューマトリクスの再更新
		pCam->setNear( _NEAR );
		pCam->setFar( _FAR );
		pCam->setFov( _FOV );
		pCam->updateViewMatrix();
		pCam->setRender( false );
	}

	//!	半透明モデル描画
	foreach( stl::vector<cBGModel*>, mModelArray, it ){
		cBGModel*	pModel = *it;
		if( !pModel->isActive() )						continue;	//!< 非動作設定
		if( !isTstOnScene(*pModel) )					continue;	//!< シーンチェック
		if( !pModel->tstFlag(cBGModel::FLAG_FAROBJ) )	continue;	//!< 遠景チェック
		pModel->RenderAlpha();
	}

	//!	Zバッファを消す
	IGraphics->clearScreen( TGL::Graphics::CLEARF_ZBUFFER, TGL::Color(0,0,0,0) );

	//!	カメラ設定戻す
	pCam->setNear( fNear );
	pCam->setFar( fFar );
	pCam->setFov( fFov );

	pCam->updateViewMatrix();
	pCam->setRender( false );
	IGraphics->setWorldTransform();

	mPerfGpu.end();
}

// ---------------------------------------------
//! @brief  近景 描画
// ---------------------------------------------
void tBG::drawNear(bool bDrawShadow)
{
	mPerfGpu.begin();
	
	TGL::BaseCamera*	pCam = ICamera->getRenderCamera();
	if( !pCam )	return;

	// @todo 仮)低スペック向けにファークリップ調整
	f32	fFarBuf = pCam->getFar();
	if( ITGLSystem->getGraphicsLevel() == SYSINFOGRPLV_LOW ){
		if( fFarBuf > 50000.0f )
			pCam->setFar( 50000.0f );
	}

	u32	uDrawCount = 0;

	//!	モデル配置描画
	foreach( stl::vector<cModelSet*>, mModelSet, it ){
		cModelSet*	pSet = *it;
		cBGModel*	pModel = getModel( pSet->getName(), pSet->getModelId() );
		if( !pModel || !pModel->isActive() )					continue;	//!< 非動作設定
		if( !isTstOnScene(*pModel) || !isTstOnScene(*pSet) )	continue;	//!< シーンチェック
		if( pModel->tstFlag(cBGModel::FLAG_FAROBJ) )			continue;	//!< 遠景チェック

#ifdef ENABLE_INSTANCING
		//!	インスタンシングマトリクス生成
		TGL::Model::MODEL_MATRIX_ARRAY	mtxArray;
		if( pSet->getLoop() > 1 ){
			for( u8 i=0 ; i<pSet->getLoop() ; i++ ){
				TGL::Matrix	mtxWorld = pSet->getWorldMtx( i, pCam );
				//!	スケーリング
				TGL::Matrix	mtxScale = TGL::Matrix::UNIT;
				mtxScale = TGL::Matrix::createScale( pSet->getScale() );
				mtxWorld *= mtxScale;
				//!	ステージ原点を加算
				mtxWorld.addW( getOrigin() );
				
				mtxArray.push_back( mtxWorld );
			}
			pModel->Update( mtxArray );
		}

		if( pSet->getLoop() <= 1 ){	//!< 通常
			//!	マトリクス生成
			TGL::Matrix	mtxWorld = pSet->getWorldMtx( 0, pCam );
			mtxWorld.addW( getOrigin() );	//!< ステージ原点を加算
			//!	スケール
			f32	fScaleBuf = pModel->GetScale();
			pModel->SetScale( pSet->getScale().x );

			//!	クリップ数チェック
			if( ITGLRenderer->isDrawList() ){
				TGL::Vec3	vPos = mtxWorld.getW() + pModel->GetBoundingVolume().getCenter();
				f32	fSize = pModel->GetBoundingVolume().getDiameter() * 0.5f * pSet->getScale().x;
				IPrim->add( TGL::PrimCircle(vPos, fSize, TGL::COLOR32::RED), 0, TGL::Camera::CAMF_00 );
				
				// @todo 仮)画面外クリッピング
				u32	uRet = pCam->isOutScreen( vPos, fSize );
				
				stl::string	str( stl::nullstr() );
				if( tstFLG(uRet,TGL::BaseCamera::CLIP_UP) )		str += "U";
				if( tstFLG(uRet,TGL::BaseCamera::CLIP_DOWN) )	str += "D";
				if( tstFLG(uRet,TGL::BaseCamera::CLIP_LEFT) )	str += "L";
				if( tstFLG(uRet,TGL::BaseCamera::CLIP_RIGHT) )	str += "R";
				if( tstFLG(uRet,TGL::BaseCamera::CLIP_FAR) )	str += "F";
				if( tstFLG(uRet,TGL::BaseCamera::CLIP_NEAR) )	str += "N";
				
				IDPrint->add( "%s %s\n", pSet->getName().c_str(), str.c_str() );
				if( uRet )
					continue;
				uDrawCount++;
			}

			pModel->Render( mtxWorld, bDrawShadow==true ? TGL::nModel::PASS_SHADOW : TGL::nModel::PASS_COLOR, false );
			pModel->SetScale( fScaleBuf );
			
		}else{	//!< インスタンシング
			pModel->Render( TGL::Matrix::UNIT, bDrawShadow==true ? TGL::nModel::PASS_SHADOW : TGL::nModel::PASS_COLOR, false );
		}

#else	/*	ENABLE_INSTANCING	*/
		for( u8 i=0 ; i<pSet->getLoop() ; i++ ){
			//!	マトリクス生成
			TGL::Matrix	mtxWorld = pSet->getWorldMtx( i, pCam );
			mtxWorld.addW( getOrigin() );	//!< ステージ原点を加算
			//!	カリング判定
			if( pSet->isCulling(pCam, mtxWorld.getW()) )	continue;
			//!	スケール
			f32	fScaleBuf = pModel->GetScale();
			pModel->SetScale( pSet->getScale().x );

			//!	クリップ数チェック
			if( ITGLRenderer->isDrawList() ){
				TGL::Vec3	vPos = mtxWorld.getW() + pModel->GetBoundingVolume().getCenter();
				f32	fSize = pModel->GetBoundingVolume().getDiameter() * 0.5f * pSet->getScale().x;
				IPrim->add( TGL::PrimCircle(vPos, fSize, TGL::COLOR32::RED), 0, TGL::Camera::CAMF_00 );
				
				// @todo 仮)画面外クリッピング
				u32	uRet = pCam->isOutScreen( vPos, fSize );
				
				stl::string	str( stl::nullstr() );
				if( tstFLG(uRet,TGL::BaseCamera::CLIP_UP) )		str += "U";
				if( tstFLG(uRet,TGL::BaseCamera::CLIP_DOWN) )	str += "D";
				if( tstFLG(uRet,TGL::BaseCamera::CLIP_LEFT) )	str += "L";
				if( tstFLG(uRet,TGL::BaseCamera::CLIP_RIGHT) )	str += "R";
				if( tstFLG(uRet,TGL::BaseCamera::CLIP_FAR) )	str += "F";
				if( tstFLG(uRet,TGL::BaseCamera::CLIP_NEAR) )	str += "N";
				
				IDPrint->add( "%s %s\n", pSet->getName().c_str(), str.c_str() );
				if( uRet )
					continue;
				uDrawCount++;
			}

			pModel->Render( mtxWorld, bDrawShadow==true ? TGL::nModel::PASS_SHADOW : TGL::nModel::PASS_COLOR, false );
			pModel->SetScale( fScaleBuf );
		}
#endif	/*	ENABLE_INSTANCING	*/
	}
	if( ITGLRenderer->isDrawList() )
		IDPrint->add( "背景表示数 : %d", uDrawCount );

	if( !bDrawShadow ){
		//!	半透明モデル配置描画
		foreach( stl::vector<cModelSet*>, mModelSet, it ){
			cModelSet*	pSet = *it;
			cBGModel*	pModel = getModel( pSet->getName(), pSet->getModelId() );
			if( !pModel || !pModel->isActive() )					continue;	//!< 非動作設定
			if( !isTstOnScene(*pModel) || !isTstOnScene(*pSet) )	continue;	//!< シーンチェック
			if( pModel->tstFlag(cBGModel::FLAG_FAROBJ) )			continue;	//!< 遠景チェック

#ifdef ENABLE_INSTANCING
			if( pSet->getLoop() <= 1 ){	//!< 通常
				//!	マトリクス生成
				TGL::Matrix	mtxWorld = pSet->getWorldMtx( 0, pCam );
				mtxWorld.addW( getOrigin() );	//!< ステージ原点を加算
				//!	スケール
				f32	fScaleBuf = pModel->GetScale();
				pModel->SetScale( pSet->getScale().x );

				pModel->RenderAlpha( mtxWorld );
				pModel->SetScale( fScaleBuf );
				
			}else{	//!< インスタンシング
				pModel->RenderAlpha( TGL::Matrix::UNIT );
			}

#else	/*	ENABLE_INSTANCING	*/
			for( u8 i=0 ; i<pSet->getLoop() ; i++ ){
				//!	マトリクス生成
				TGL::Matrix	mtxWorld = pSet->getWorldMtx( i, pCam );
				mtxWorld.addW( getOrigin() );	//!< ステージ原点を加算
				//!	スケール
				f32	fScaleBuf = pModel->GetScale();
				pModel->SetScale( pSet->getScale().x );

				pModel->RenderAlpha( mtxWorld );
				pModel->SetScale( fScaleBuf );
			}
#endif	/*	ENABLE_INSTANCING	*/
		}
	}

	//!	低スペック向けにいじったファーを戻す
	if( ITGLSystem->getGraphicsLevel() == SYSINFOGRPLV_LOW )
		pCam->setFar( fFarBuf );

	mPerfGpu.end();
}

/* ========================================================================= */
/*		デバッグ															 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  デバッグ情報表示
// ---------------------------------------------
void tBG::dispDebugInfo()
{
#ifndef _TGL_FINAL
	if( !mbDebugInfo )	return;
	
	s32	nX, nY;
	IDPrint->getPos( nX, nY );
	IPrim->add( TGL::Prim2Rect((f32)(nX-4), (f32)(nY-4), 320.0f, (f32)IDPrint->getLineHeight( 6 ), IRGBA(0,0,64,128)) );
	IDPrint->addSh( IRGBA(192,255,192,255), "■ 背景情報 [%s]\n", getName() );
	IDPrint->addSh( "  シーン : %d, 経過:%0.1f\n", mScene, mTimePass );
	
	//!	シーンパラメータ
	IDPrint->addSh( "  シーンパラメータ : %d", mSceneParam.getPrevId() );
	for( u8 i=0 ; i<10 ; i++ ){	//!< ブレンド率
		u8	uBlend = TGL::Math::Clamp<s32>( (s32)(mSceneParam.getBlendTime().getValue() * 10.0f), 0, 9 );
		if( i == uBlend )
			IDPrint->addSh( "|" );
		else
			IDPrint->addSh( "-" );
	}
	IDPrint->addSh( "%d\n", mSceneParam.getCurrentId() );
	//IDPrint->addSh( "  環境マップ : %s\n", mSceneParam.getEnvTex().c_str() );
	
	//!	処理負荷
	mPerfCpu.dispDebug("CPU");
	mPerfGpu.dispDebug("GPU");
	

#endif	/*	_TGL_FINAL	*/
}

// ---------------------------------------------
//! @brief  指定モデルのロケータを画面表示（モデル配置使用）
//! @param  pCam		[in]	描画カメラ
//! @param  strModel	[in]	モデル名
//! @param  strLocator	[in]	ロケータ名。末尾にIDが付く。IDの開始値は0
//! @param  pPosBuf		[i/o]	比較用表示2D座標バッファ。NULL:内部で確保
// ---------------------------------------------
void tBG::dispDebugInfoLocator( const TGL::BaseCamera* pCam, const stl::string& strModel, const stl::string& strLocator, stl::vector<TGL::Vec2*>* pPosBuf/*=NULL*/ )
{
#ifndef _TGL_FINAL
	//!	比較用表示2D座標バッファ
	bool	bPosBufNew = (pPosBuf == NULL);
	if( bPosBufNew )
		pPosBuf = APP_NEW stl::vector<TGL::Vec2*>;
	
	foreach( stl::vector<cModelSet*>, mModelSet, it ){
		cModelSet*	pSet = *it;
		if( pSet->getName() != strModel )	continue;
		if( !isTstOnScene(*pSet) )			continue;

		cBGModel*	pModel = getModel( pSet->getName(), pSet->getModelId() );
		if( !pModel || !isTstOnScene(*pModel) )	continue;

		dispDebugInfoLocatorExec( pCam, pSet, pModel, strLocator, pPosBuf );
	}

	//!	解放
	if( bPosBufNew ){
		foreach( stl::vector<TGL::Vec2*>, *pPosBuf, it )
			SAFE_DELETE( *it );
		SAFE_DELETE( pPosBuf );
	}
#endif	/*	_TGL_FINAL	*/
}

// ---------------------------------------------
//! @brief  指定モデルのロケータを画面表示（モデル直接指定）
//! @param  pCam		[in]	描画カメラ
//! @param  pModel		[in]	モデル
//! @param  strLocator	[in]	ロケータ名。末尾にIDが付く。IDの開始値は0
//! @param  pPosBuf		[i/o]	比較用表示2D座標バッファ。NULL:内部で確保
// ---------------------------------------------
void tBG::dispDebugInfoLocator( const TGL::BaseCamera* pCam, cBGModel* pModel, const stl::string& strLocator, stl::vector<TGL::Vec2*>* pPosBuf/*=NULL*/ )
{
#ifndef _TGL_FINAL
	//!	比較用表示2D座標バッファ
	bool	bPosBufNew = (pPosBuf == NULL);
	if( bPosBufNew )
		pPosBuf = APP_NEW stl::vector<TGL::Vec2*>;
	
	if( !isTstOnScene(*pModel) )	return;
	dispDebugInfoLocatorExec( pCam, NULL, pModel, strLocator, pPosBuf );

	//!	解放
	if( bPosBufNew ){
		foreach( stl::vector<TGL::Vec2*>, *pPosBuf, it )
			SAFE_DELETE( *it );
		SAFE_DELETE( pPosBuf );
	}
#endif	/*	_TGL_FINAL	*/
}

// ---------------------------------------------
//! @brief  指定モデルのロケータを画面表示
//! @param  pCam		[in]	描画カメラ
//! @param  pSet		[in]	モデルセット
//! @param  pModel		[in]	モデル
//! @param  strLocator	[in]	ロケータ名。末尾にIDが付く。IDの開始値は0
//! @param  pPosBuf		[i/o]	比較用表示2D座標バッファ。NULL:内部で確保
// ---------------------------------------------
void tBG::dispDebugInfoLocatorExec( const TGL::BaseCamera* pCam, cModelSet* pSet, cBGModel* pModel, const stl::string& strLocator, stl::vector<TGL::Vec2*>* pPosBuf )
{
#ifndef _TGL_FINAL
	const f32		_RATE = FABS(SIN(360.0f / 120.0f * (f32)((ITimer->getTotalFrame()+1) % 120)));
	const RGBA32	_BASE_COLOR = IRGBA(255.0f*_RATE, 255.0f*_RATE, 0, 255);
	
	for( u8 i=1 ;; i++ ){
		stl::string	strLoc = stl::FormatString<stl::string>( "%s%02d", strLocator.c_str(), i );
		if( !pModel->IsExistBone(strLoc.c_str()) )	break;
		//!	モデルセットとロケータのマトリクス取得
		TGL::Matrix	mtxModelSet = pSet ? pSet->getWorldMtx() : TGL::Matrix::UNIT;	//!< モデルセットのマトリクス
		const TGL::Matrix*	pMtxModel = pModel->GetBoneWorldMatrix( strLoc.c_str() );	//!< モデルローカルのロケータマトリクス

		//!	マトリクス掛け合わせてワールド系にする
		TGL::Matrix	mtx = *pMtxModel * mtxModelSet;
		mtx.addW( getOrigin() );	//!< ステージ原点加算
		TGL::Vec3	vPos( mtx.getW() );	//!< 座標
		
		//!	裏側は表示しない
		TGL::Vec3	v0 = vPos - pCam->getPos();
		if( v0.isZero() )	continue;
		v0 = v0.normalize();
		if( pCam->getEyeVector().dot(v0) <= 0.0f )	continue;

		//!	表示
		IPrim->add( TGL::PrimCross(vPos, 500.f, _BASE_COLOR), 0, TGL::Camera::CAMF_00 );
		TGL::Vec2	vScreenPos = pCam->getScreenPos( vPos );
		
		//!	それまでの表示と近過ぎないか
		if( pPosBuf ){
			bool	bFlag = true;
			foreach( stl::vector<TGL::Vec2*>, *pPosBuf, it ){
				if( FABS((*it)->x - vScreenPos.x) < 96
					&& FABS((*it)->y - vScreenPos.y) < 16 ){
					bFlag = false;
					break;
				}
			}
			if( !bFlag )	continue;
			pPosBuf->push_back( APP_NEW TGL::Vec2(vScreenPos) );
		}

		IDPrint->setPosBufStrCenter( strLoc, vScreenPos.x, vScreenPos.y-TGL::DPrint::FONT_H/2.0f );
		IDPrint->addSh( _BASE_COLOR, "%s", strLoc.c_str() );
		IDPrint->resumePos();
	}
#endif	/*	_TGL_FINAL	*/
}

// ---------------------------------------------
//! @brief  Zバッファクリア
// ---------------------------------------------
void tBG::clearZBuffer( void *pData )
{
	IGraphics->clearScreen( TGL::Graphics::CLEARF_ZBUFFER, TGL::Color(0,0,0,0) );
}

// ---------------------------------------------
//! @brief  背景名を取得
//! @return 文字列
// ---------------------------------------------
const char* tBG::getName() const
{
	return sBG::getName( mBGId );
}

/* ========================================================================= */
/*		EOF																	 */
/* ========================================================================= */
