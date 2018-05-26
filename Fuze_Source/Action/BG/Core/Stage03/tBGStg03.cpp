// -------------------------------------------------------------------------- //
//! @file   tBGStg03.cpp
//! @brief  背景 ３面
//! @author Nal
//! @since  12/08/17(金)
//! 
//! COPYRIGHT (C) 2012 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
/* ====< インクルード >====================================================== */
#include "prefix.h"
#include <TGL.h>
#include "Game/Game.h"
#include "Action/Camera/cBGCamera.h"
#include "tBGStg03.h"
#include "Action/ActionTypes.h"
#include "Effect/sAppEffect.h"
#include "../../../../../tgs/scene/Stage3/scn_Stage3.h"

#include <CbSystem.h>
#include <BG/BG_Cloud.h>

static const char*	_PARAM_EX = "param/BG/Stage3/Stage3_paramEx.json";	//!< 固有パラメータリソース

//!	動的環境マップ生成
#define	EXENV

#ifdef _WINDOWS
#pragma warning(disable : 4355)
#endif
/* ========================================================================= */
//!		tBGStg03															 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  コンストラクタ "tBGStg03"
// ---------------------------------------------
tBGStg03::tBGStg03()
:	tBG(),
	mpLensflare(NULL), mSortCloud(*this), mSortCloudBoss(*this), mpDust(NULL), mExEnvSize(32)
{
	SLLINK_Init( &mEftCloudLink );
	reset();
}

// ---------------------------------------------
//! @brief  デストラクタ "tBGStg03"
// ---------------------------------------------
tBGStg03::~tBGStg03()
{
	IAppResource->releaseTexPool( "models/BG/St03_Cloud/St03_Cloud_Albedo.dds" );
	IAppResource->releaseTexPool( "models/BG/St03_Cloud/St03_Cloud_Trp1_Albedo.dds" );
	IAppResource->releaseTexPool( "models/BG/St03_Cloud/St03_Cloud_Trp2_Albedo.dds" );
	IAppResource->releaseTexPool( "models/BG/common/St03/St03_Flare01.dds" );
	IAppResource->releaseTexPool( "models/BG/common/St03/St03_Flare02.dds" );
	IAppResource->releaseTexPool( "models/BG/common/St03/St03_Flare03.dds" );
	IAppResource->releaseTexPool( "models/BG/common/St03/St03_Flare04.dds" );
	IAppResource->releaseTexPool( "models/BG/common/St03/St03_Post_ToneCurve_nmip.dds" );
	IAppResource->releaseTexPool( "models/BG/common/St03/St03_Post_ToneCurve2_nmip.dds" );
	IAppResource->releaseTexPool( "models/BG/common/St03R/St03_Cloud_envC.dds" );
	IAppResource->releaseTexPool( "models/BG/common/St03R/St03_Space_envC.dds" );
	ITGLRenderer->getFilterToneCurve()->setTexCurve( NULL );
	ITGLRenderer->getFilterToneCurve()->setEnableValue( false );
	SAFE_DELETE( mpLensflare );
	SAFE_DELETE( mpDust );

	EEMIT_End( &mEftCloudLink );

	IAppEffect->releaseExEnvironment();
	ITGLRenderer->setFBCTexEnable( false );
}

// ---------------------------------------------
//! @brief  リセット
// ---------------------------------------------
void tBGStg03::reset()
{
	tBG::reset();
	EEMIT_End( &mEftCloudLink );
	mMeteorAlphaRef	= 1;
	mCloudSize		= 150000.0f;
	mCloudRot		= 0;
	mEftCloudMtx	= TGL::Matrix::UNIT;
	m06_UvA			= TGL::Vec2(0,0);
	m06_UvB			= TGL::Vec2(0,0);
	m06_UvC			= TGL::Vec2(0,0);
	m06_UvD			= TGL::Vec2(0,0);
	mSpiral_UvA		= TGL::Vec2(0,0);
	mSpiral_UvB		= TGL::Vec2(0,0);
	mSpiral_UvC		= TGL::Vec2(0,0);
	mCloud_UvA		= TGL::Vec2(0,0);
	mCloud_UvB		= TGL::Vec2(0,0);
	mCloud_UvC		= TGL::Vec2(0,0);
	mBossSkyScaleEnd	= 1.5f;
	mBossSkyScale.clear( 1.0f );
}

// ---------------------------------------------
//! @brief  リソースロード
//! @param  uBG	[in]	BGID_????
//! @return true:成功, false:失敗
// ---------------------------------------------
bool tBGStg03::load( u8 uBG )
{
	//!		モデル		
	cBGModel*	pModel;
	//!	シーン
	const u32	_SCENE_SPACE		= cBGModel::SCENE_00 | cBGModel::SCENE_01;	//!< 道中 宇宙
	const u32	_SCENE_CLOUD		= cBGModel::SCENE_02;	//!< 道中 雲海
	const u32	_SCENE_BOSSENTRY	= cBGModel::SCENE_03;	//!< ボス戦 開始
	const u32	_SCENE_BOSS			= cBGModel::SCENE_04;	//!< ボス戦 雲海
	//!	遠景 宇宙
	pModel = addModel( "St03_Far_01.mdl", cBGModel::TYPE_SKYDOME, cBGModel::FLAG_FAROBJ );
	if( pModel )	pModel->setOnScene( _SCENE_SPACE );
	//!	隕石ロケータ
	pModel = addModel( "St03_01_Loop.mdl" );
	if( pModel )	pModel->setOnScene( _SCENE_SPACE );
	//!	隕石
	pModel = addModel( "Obj_St03_Dust_BB.mdl" );
	if( pModel )	pModel->setOnScene( _SCENE_SPACE );
	//!	雲配置ロケータ
	pModel = addModel( "St03_02_Loop.mdl" );
	if( pModel )	pModel->setOnScene( _SCENE_CLOUD );
	//!	ボスエリア
	pModel = addModel( "St03_06.mdl", cBGModel::TYPE_NORMAL, cBGModel::FLAG_FAROBJ );
	if( pModel ){
		pModel->setOnScene( _SCENE_BOSS );
		pModel->setPos( TGL::Vec3(40000, 0, 228000) );
	}
	//!	ボスエリア
	pModel = addModel( "Obj_St03_ENP_Spiral.mdl", cBGModel::TYPE_NORMAL, cBGModel::FLAG_FAROBJ );
	if( pModel ){
		pModel->setOnScene( _SCENE_BOSS );
		pModel->setPos( TGL::Vec3(40000, 0, 228000) );
		//!	モーション
		TGL::AnimationController*	pAnim = APP_NEW TGL::AnimationController( pModel );
		pModel->setAnim( pAnim );
		pModel->addMotion( 0, "Obj_St03_ENP_Spiral.anm" );
		pModel->getAnim()->Play( 0, true, true );
	}
	//!	遠景 雲海
	pModel = addModel( "St03_Far_02.mdl", cBGModel::TYPE_SKYDOME, cBGModel::FLAG_FAROBJ );
	if( pModel )	pModel->setOnScene( _SCENE_CLOUD | _SCENE_BOSSENTRY | _SCENE_BOSS );
	//!	雲
	pModel = addModel( "Obj_St03_Cloud_01A_BB.mdl" );
	if( pModel )	pModel->setOnScene( _SCENE_CLOUD | _SCENE_BOSS );
	//!	雲
	pModel = addModel( "Obj_St03_Cloud_01B_BB.mdl" );
	if( pModel )	pModel->setOnScene( _SCENE_CLOUD | _SCENE_BOSS );
	//!	雲
	pModel = addModel( "Obj_St03_Cloud_01C_BB.mdl" );
	if( pModel )	pModel->setOnScene( _SCENE_CLOUD | _SCENE_BOSS );

	//!	テクスチャ
	IAppResource->loadTexPool( "models/BG/St03_Cloud/St03_Cloud_Albedo.dds" );
	IAppResource->loadTexPool( "models/BG/St03_Cloud/St03_Cloud_Trp1_Albedo.dds" );
	IAppResource->loadTexPool( "models/BG/St03_Cloud/St03_Cloud_Trp2_Albedo.dds" );
	IAppResource->loadTexPool( "models/BG/common/St03/St03_Flare01.dds" );
	IAppResource->loadTexPool( "models/BG/common/St03/St03_Flare02.dds" );
	IAppResource->loadTexPool( "models/BG/common/St03/St03_Flare03.dds" );
	IAppResource->loadTexPool( "models/BG/common/St03/St03_Flare04.dds" );
	IAppResource->loadTexPool( "models/BG/common/St03/St03_Post_ToneCurve_nmip.dds" );
	IAppResource->loadTexPool( "models/BG/common/St03/St03_Post_ToneCurve2_nmip.dds" );
	IAppResource->loadTexPool( "models/BG/common/St03R/St03_Cloud_envC.dds" );
	IAppResource->loadTexPool( "models/BG/common/St03R/St03_Space_envC.dds" );

	//!	レンズフレア
	pModel = getModel( "St03_Far_01.mdl" );
	if( pModel ){
		SAFE_DELETE( mpLensflare );
		mpLensflare = createLensFlare( TGL::MtxGuide(pModel->GetWorldMatrix(), TGL::Vec3(-21, 8836, 36833)), "St03_Flare", _PARAM_EX, "Lensflare" );
	}

	//!		シーンパラメータ		
	mSceneParam.getFromJson( "param/BG/Stage3/Stage3_param.json" );

	{	//!	固有パラメータ
		TGL::JsonUtil	json( _PARAM_EX );
		TGL::Json::array	jsonArray = json.getArray("-");
		foreach( TGL::Json::array, jsonArray, it ){
			TGL::Json::object&	work = it->get<TGL::Json::object>();
			mMeteorAlphaRef		= atoi( work["Meteor_AlphaRef"].to_str().c_str() );
			mBossSkyScaleEnd	= (f32)atof( work["BossSkyScale"].to_str().c_str() );
			mCloudSize			= (f32)atof( work["Cloud_Size"].to_str().c_str() );
			mCloudRot			= (f32)atof( work["Cloud_Rot"].to_str().c_str() );
			
			for( u8 i=0 ; i<numberof(mCloudParam) ; i++ ){
				CloudParam&	param = mCloudParam[i];
				stl::string	str( stl::FormatString<stl::string>("Cloud%d_", i+1) );
				
				param.mBlend	= atoi( work[str+ "Blend"].to_str().c_str() );
				param.mHeight	= (f32)atof( work[str+ "Height"].to_str().c_str() );
				param.mColor	= IRGBA(	atoi( work[str+ "ColorR"].to_str().c_str() ),
				                          	atoi( work[str+ "ColorG"].to_str().c_str() ),
				                          	atoi( work[str+ "ColorB"].to_str().c_str() ),
				                          	atoi( work[str+ "ColorA"].to_str().c_str() ) );
				param.mUVOfs	= (f32)atof( work[str+ "UVOfs"].to_str().c_str() );
				param.mUVRepeat	= (f32)atof( work[str+ "UVRepeat"].to_str().c_str() );
			}

			m06_UvA		= TGL::Vec2( (f32)atof( work["06_A_Speed_U"].to_str().c_str() ),
								(f32)atof( work["06_A_Speed_V"].to_str().c_str() ) );
			m06_UvB		= TGL::Vec2( (f32)atof( work["06_B_Speed_U"].to_str().c_str() ),
								(f32)atof( work["06_B_Speed_V"].to_str().c_str() ) );
			m06_UvC		= TGL::Vec2( (f32)atof( work["06_C_Speed_U"].to_str().c_str() ),
								(f32)atof( work["06_C_Speed_V"].to_str().c_str() ) );
			m06_UvD		= TGL::Vec2( (f32)atof( work["06_D_Speed_U"].to_str().c_str() ),
								(f32)atof( work["06_D_Speed_V"].to_str().c_str() ) );

			mSpiral_UvA	= TGL::Vec2( (f32)atof( work["Spiral_A_Speed_U"].to_str().c_str() ),
								(f32)atof( work["Spiral_A_Speed_V"].to_str().c_str() ) );
			mSpiral_UvB	= TGL::Vec2( (f32)atof( work["Spiral_B_Speed_U"].to_str().c_str() ),
								(f32)atof( work["Spiral_B_Speed_V"].to_str().c_str() ) );
			mSpiral_UvC	= TGL::Vec2( (f32)atof( work["Spiral_C_Speed_U"].to_str().c_str() ),
								(f32)atof( work["Spiral_C_Speed_V"].to_str().c_str() ) );

			mCloud_UvA	= TGL::Vec2( (f32)atof( work["Cloud_A_Speed_U"].to_str().c_str() ),
								(f32)atof( work["Cloud_A_Speed_V"].to_str().c_str() ) );
			mCloud_UvB	= TGL::Vec2( (f32)atof( work["Cloud_B_Speed_U"].to_str().c_str() ),
								(f32)atof( work["Cloud_B_Speed_V"].to_str().c_str() ) );
			mCloud_UvC	= TGL::Vec2( (f32)atof( work["Cloud_C_Speed_U"].to_str().c_str() ),
								(f32)atof( work["Cloud_C_Speed_V"].to_str().c_str() ) );

			mExEnvSize		= atoi( work["ExEnvironmentSize"].to_str().c_str() );

			mSceneParam.getRimLight().setupFromJson( work );
		}
	}

	//!	モデル配置
	//!	リソースからのモデル配置
	applyModelSetFromJson( "param/BG/Stage3/Stage3_modelset.json" );

	//!	道中雲 ロケータ配置列挙
	mSortCloud.setBegin( mModelSet.size() );
	for( u8 j=0 ; j<3 ; j++ ){
		for( u8 i=1 ;; i++ ){
			//!	ロケーター名
			const char*	_LOCATOR[] = {
				"St03_Cloud01A_LOC_",
				"St03_Cloud01B_LOC_",
				"St03_Cloud01C_LOC_",
			};
			//!	モデル
			const char*	_OBJ[] = {
				"Obj_St03_Cloud_01A_BB.mdl",
				"Obj_St03_Cloud_01B_BB.mdl",
				"Obj_St03_Cloud_01C_BB.mdl",
			};
			stl::string	strLoc = stl::FormatString<stl::string>( "%s%02d", _LOCATOR[j], i );
			cModelSet*	pSet = addModelSetAssign( _OBJ[j], "St03_02_Loop.mdl", strLoc, true );
			if( !pSet )
				break;
			pSet->setId( i );
			pSet->setOnScene( _SCENE_CLOUD );
			pSet->setRotType( cModelSet::ROTTYPE_BBD_Y );	//!< Y軸ビルボード
		}
	}
	//!	ソート用設定
	mSortCloud.setEnd( mModelSet.size() );
	mSortCloud.setScene( _SCENE_CLOUD );
	mSortCloud.setArray( &mModelSet );
	mSortCloud.setSortCB( cModelSetSort::sortCB_ZFar );
#if 0
	//!	ボス雲 ロケータ配置列挙
	{
		mSortCloudBoss.setBegin( mModelSet.size() );

		TGL::Matrix	mtx = TGL::Matrix::UNIT;
		pModel = getModel( "St03_06.mdl" );
		mtx.setW( pModel->getPos() );

		for( u8 j=0 ; j<3 ; j++ ){
			for( u8 i=1 ;; i++ ){
				//!	ロケーター名
				const char*	_LOCATOR[] = {
					"St03_06_BB_A_LOC_",
					"St03_06_BB_B_LOC_",
					"St03_06_BB_C_LOC_",
				};
				//!	モデル
				const char*	_OBJ[] = {
					"Obj_St03_Cloud_01A_BB.mdl",
					"Obj_St03_Cloud_01B_BB.mdl",
					"Obj_St03_Cloud_01C_BB.mdl",
				};
				stl::string	strLoc = stl::FormatString<stl::string>( "%s%02d", _LOCATOR[j], i );

				cModelSet*	pSet = addModelSetAssignExec( _OBJ[j], "St03_06.mdl", strLoc, true, mtx );
				if( !pSet )
					break;
				pSet->setId( i );
				pSet->setOnScene( _SCENE_BOSS );
				pSet->setRotType( cModelSet::ROTTYPE_BBD_Y );	//!< Y軸ビルボード
			}
		}
		//!	ソート用設定
		mSortCloudBoss.setEnd( mModelSet.size() );
		mSortCloudBoss.setScene( _SCENE_BOSS );
		mSortCloudBoss.setArray( &mModelSet );
		mSortCloudBoss.setSortCB( cModelSetSort::sortCB_ZFar );
	}
#endif

	//!	カメラパス追加
	applyCameraPathSetFromJson( "param/BG/Stage3/Stage3_camera.json" );

	//!	UVアニメ設定
	setModelUvAnim();

	//!	トーンカーブ
	ITGLRenderer->getFilterToneCurve()->setEnableValue( true );
	ITGLRenderer->getFilterToneCurve()->setTexCurve( IAppResource->getTexture("St03_Post_ToneCurve_nmip") );

#ifdef EXENV
	//!	特殊環境マップ
	ITGLRenderer->setFBCTexEnable( true );
	IAppEffect->createExEnvironment( mExEnvSize );
#endif
	//!	シーン環境マップ
	mSceneParam.setEnvTex( "St03_Space_envC.dds" );
	ICharacter->overwriteBGEnvTex();	//!< キャラクターへの適用
	overwriteEnvTex();	//!< 背景モデルへの適用

	//!	更新関数登録
	addUpdateEvent( updateDustParCB,	1, 1, setupDustParCB, finalDustParCB );	//!< 塵パーティクル
	addUpdateEvent( NULL,				1, 1, setupDustCB, finalDustCB );	//!< 塵
	addUpdateEvent( updateFarCB,		1, 1 );		//!< 遠景回す
	addUpdateEvent( updateCloudBeginCB,	2, 2 );		//!< 雲海開始
	addUpdateEvent( updateCloudParCB,	2, -1 );	//!< 雲海パーティクル
	addUpdateEvent( updateFarScaleCB,	3, -1 );	//!< 天球スケール

	return true;
}

// ---------------------------------------------
//! @brief  UVアニメ設定
// ---------------------------------------------
void tBGStg03::setModelUvAnim()
{
	cBGModel*	pModel;
	
	//!	AlphaRef設定
	pModel = getModel( "St03_Far_01.mdl" );
	if( pModel ){
		pModel->SetNodeAlphaRef( "Meteor2_nofog_alpha", mMeteorAlphaRef );
		pModel->SetNodeAlphaRef( "Meteor_nofog_alpha", mMeteorAlphaRef );
		pModel->SetNodeAlphaRef( "MeteorFar_nofog_alpha", mMeteorAlphaRef );
	}
	pModel = getModel( "St03_Far_02.mdl" );
	if( pModel ){
		pModel->SetNodeAlphaRef( "Meteor2_nofog_alpha", mMeteorAlphaRef );
		pModel->SetNodeAlphaRef( "MeteorFar_nofog_alpha", mMeteorAlphaRef );
		pModel->SetNodeAlphaRef( "Meteor_nofog_alpha", mMeteorAlphaRef );
	}

	//!	UVアニメ
	pModel = getModel( "St03_06.mdl" );
	if( pModel ){
		pModel->SetNodeUVAnime( "ENP_EFFCenterA_add", m06_UvA );
		pModel->SetNodeUVAnime( "ENP_EFFCenterB_add", m06_UvB );
		pModel->SetNodeUVAnime( "ENP_EFFCenterC_add", m06_UvC );
		pModel->SetNodeUVAnime( "ENP_EFFCenterD_add", m06_UvD );

		pModel->SetNodeUVAnime( "Cloud_Spiral_1_alpha", mSpiral_UvA );
		pModel->SetNodeUVAnime( "Cloud_Spiral_2_alpha", mSpiral_UvB );
		pModel->SetNodeUVAnime( "Cloud_Spiral_3_alpha", mSpiral_UvC );

		pModel->SetNodeUVAnime( "St03_06_Cloud_alpha", mCloud_UvA );
		pModel->SetNodeUVAnime( "St03_06_Cloud_Trp1_alpha", mCloud_UvB );
		pModel->SetNodeUVAnime( "St03_06_Cloud_Trp2_alpha", mCloud_UvC );
	}
}

// ---------------------------------------------
//! @brief  エフェクト開始
// ---------------------------------------------
void tBGStg03::beginEffect()
{
}

// ---------------------------------------------
//! @brief  更新
// ---------------------------------------------
void tBGStg03::update()
{
	//!	親クラス
	tBG::update();

	//!	レンズフレア
	if( mpLensflare )
		mpLensflare->setEnable( true );

#ifdef EXENV
	//!	特殊環境マップの更新
	if( mScene <= 1 && IAppEffect->getExEnvTex() )
		IAppEffect->getExEnvTex()->setTrigger( true );
#endif
}

// ---------------------------------------------
//! @brief  カメラ更新後の処理
// ---------------------------------------------
void tBGStg03::updateCameraAfter()
{
	tBG::updateCameraAfter();
	
	//!	雲のソート
	mSortCloud.sort();
	//!	ボス雲のソート
	mSortCloudBoss.sort();
}

// ---------------------------------------------
//! @brief  スクリプトメッセージ受け取り
//! @param  nId		[in]	メッセージID
//! @param  nParam	[in]	パラメータ
// ---------------------------------------------
void tBGStg03::receiveMessage( s32 nId, s32 nParam )
{
	switch( nId ){
		case STG3MSG_DUST_CRUISER:	//!	塵 巡洋艦
			if( mpDust ){
				mpDust->setCameraMove( 0.10f );
			}
			break;

		case STG3MSG_DUST_FLAGSHIP:	//!	塵 旗艦接近
			break;
			if( mpDust ){
				mpDust->setCameraMove( 0.50f );
			}
			break;

		case STG3MSG_DUSTEND:	//!	塵消す
			finalDustPar();
			break;
			
		case STG3MSG_FARSCALE_MIN:	//! 天球スケール 最小
		{	//!	スケール更新
			mSkyNear	= 1000.0f;
			mSkyFar		= 100000.0f;
			mBossSkyScale.clear( 1.0f );
			//!	遠景にスケール適用
			cBGModel*	pModel = getModel( "St03_Far_02.mdl" );
			if( pModel )
				pModel->SetScale( mBossSkyScale.getValue() );
		}	break;

		case STG3MSG_FARSCALE_MAX:	//! 天球スケール 最大
		{	//!	スケール更新
			mSkyNear	= 1500.0f;
			mSkyFar		= 150000.0f;
			mBossSkyScale.clear( mBossSkyScaleEnd );
			//!	遠景にスケール適用
			cBGModel*	pModel = getModel( "St03_Far_02.mdl" );
			if( pModel )
				pModel->SetScale( mBossSkyScale.getValue() );
		}	break;
	}
}

// ---------------------------------------------
//! @brief  塵パーティクル 初期化
// ---------------------------------------------
void tBGStg03::setupDustPar()
{
	//!	パーティクル
	if( !ITGLSystem->isGraphicsLevelLow() && !ITGLSystem->isPostFilterLevelLow() ){
		TGL::MtxGuide	guide( &mEftCloudMtx );
		IAppEffect->req( EFTNO_SCENES3_DUST, TGL::Vec3::ZERO, TGL::Vec3::ZERO, NULL, &guide, NULL, NULL, 0, 0, &mEftCloudLink );
	}
}
// ---------------------------------------------
//! @brief  塵パーティクル 終了
// ---------------------------------------------
void tBGStg03::finalDustPar()
{
	EEMIT_End( &mEftCloudLink );
}
// ---------------------------------------------
//! @brief  塵パーティクル
//! @param  bSceneChg	[in]	true:シーン変更瞬間
// ---------------------------------------------
void tBGStg03::updateDustPar( bool bSceneChg )
{
	//!	パーティクル追従マトリクス更新
	TGL::BaseCamera*	pCam = ICamera->getCamera(0);
	//!	回転
	TGL::Vec3	v0 = -pCam->getMove();
	if( !v0.isZero() ){
		//!	Y : ツイスト回転済みカメラUP
		TGL::Matrix	m0 = TGL::Matrix::UNIT;
		m0.setX( pCam->getRightVector() );
		m0.setY( pCam->getUpVector() );
		m0.setZ( pCam->getEyeVector() );
		m0.rollVec( pCam->getEyeVector(), -pCam->getTwist() );
		mEftCloudMtx.setY( m0.getY() );

		//!	Z : カメラ移動逆方向
		v0 = v0.normalize();
		mEftCloudMtx.setZ( v0 );
		
		//!	X : ZY垂線
		v0 = mEftCloudMtx.getZ().cross( mEftCloudMtx.getY() );
		mEftCloudMtx.setX( v0 );
	}
	
	//!	座標 : カメラ位置から、カメラ移動量を足す
	v0 = pCam->getPos();
	v0 += pCam->getMove() * 120.0f;
	v0 += pCam->getEyeVector() * 3000.0f;
	mEftCloudMtx.setW( v0 );
	
#if 0
	IPrim->add( TGL::PrimCross(mEftCloudMtx.getW(), 1000.f, IRGBA(255,255,0,255)), 0, TGL::Camera::CAMF_00 );
	
	TGL::Matrix	m0 = mEftCloudMtx;
	m0.setW( pCam->getTar() );
	IPrim->add( TGL::PrimPivot(m0, 1000.0f), 0, TGL::Camera::CAMF_00 );
#endif
}

// ---------------------------------------------
//! @brief  塵 初期化
// ---------------------------------------------
void tBGStg03::setupDust()
{
	if( mpDust )	return;
	if( !ITGLSystem->isGraphicsLevelLow() && !ITGLSystem->isPostFilterLevelLow() ){
		mpDust = APP_NEW tDust( this, GPRI_EFTPRE(0) );
		mpDust->setColorMin( IRGBA( 96, 96, 96,255) );
		mpDust->setColorMax( IRGBA(255,255,255,255) );
		mpDust->setFogColor( IRGBA(0,0,0,255) );
		mpDust->setFogNear(	200 );
		mpDust->setFogFar(	2000 );
		mpDust->setSpd( 0.2f );
	}
}
// ---------------------------------------------
//! @brief  塵 終了
// ---------------------------------------------
void tBGStg03::finalDust()
{
	SAFE_DELETE( mpDust );
}

// ---------------------------------------------
//! @brief  更新 遠景回す
//! @param  bSceneChg	[in]	true:シーン変更瞬間
// ---------------------------------------------
void tBGStg03::updateFar( bool bSceneChg )
{
	if( bSceneChg ){
		cBGModel*	pModel = getModel( "St03_Far_01.mdl" );
		if( pModel ){
			pModel->setRot( TGL::Vec3(0, 60, 0) );
			//!	レンズフレア接続
			if( mpLensflare )
				mpLensflare->setAttach( TGL::MtxGuide( pModel->GetWorldMatrix(), TGL::Vec3(-21, 8836, 36833) ) );
		}

		//!	トーンカーブ変更
		ITGLRenderer->getFilterToneCurve()->setTexCurve( IAppResource->getTexture("St03_Post_ToneCurve_nmip") );
	}
}

// ---------------------------------------------
//! @brief  雲海開始
//! @param  bSceneChg	[in]	true:シーン変更瞬間
// ---------------------------------------------
void tBGStg03::updateCloudBegin( bool bSceneChg )
{
	if( bSceneChg ){
		//!	遠景回す
		cBGModel*	pModel = getModel( "St03_Far_02.mdl" );
		if( pModel ){
			//pModel->setRot( TGL::Vec3(0, 60, 0) );
			//!	レンズフレア接続
			if( mpLensflare ){
				TGL::MtxGuide	guide( pModel->GetWorldMatrix(), TGL::Vec3(-21, 8836, 36833) );
				guide.setScaleModel( pModel );
				mpLensflare->setAttach( guide );
			}
		}
		
		//!	トーンカーブ変更
		ITGLRenderer->getFilterToneCurve()->setTexCurve( IAppResource->getTexture("St03_Post_ToneCurve2_nmip") );

		//!	シーン環境マップ
		mSceneParam.setEnvTex( "St03_Cloud_envC.dds" );
		ICharacter->overwriteBGEnvTex();	//!< キャラクターへの適用
		overwriteEnvTex();	//!< 背景モデルへの適用
	}
}

// ---------------------------------------------
//! @brief  雲海パーティクル
//! @param  bSceneChg	[in]	true:シーン変更瞬間
// ---------------------------------------------
void tBGStg03::updateCloudPar( bool bSceneChg )
{
	if( mSceneOld < 2 ){
		//!	パーティクル
		if( ITGLSystem->getGraphicsLevel() != SYSINFOGRPLV_LOW ){
			TGL::MtxGuide	guide( &mEftCloudMtx );
			IAppEffect->req( EFTNO_SCENES3_CLOUDFOG, TGL::Vec3::ZERO, TGL::Vec3::ZERO, NULL, &guide, NULL, NULL, 0, 0, &mEftCloudLink );
		}
	}
	
	//!	パーティクル追従マトリクス更新
	TGL::BaseCamera*	pCam = ICamera->getCamera(0);
	//!	回転
	TGL::Vec3	v0 = -pCam->getMove();
	if( !v0.isZero() ){
		//!	Y : ツイスト回転済みカメラUP
		TGL::Matrix	m0 = TGL::Matrix::UNIT;
		m0.setX( pCam->getRightVector() );
		m0.setY( pCam->getUpVector() );
		m0.setZ( pCam->getEyeVector() );
		m0.rollVec( pCam->getEyeVector(), -pCam->getTwist() );
		mEftCloudMtx.setY( m0.getY() );

		//!	Z : カメラ移動逆方向
		v0 = v0.normalize();
		mEftCloudMtx.setZ( v0 );
		
		//!	X : ZY垂線
		v0 = mEftCloudMtx.getZ().cross( mEftCloudMtx.getY() );
		mEftCloudMtx.setX( v0 );
	}
	
	//!	座標 : カメラ位置から、カメラ移動量を足す
	v0 = pCam->getPos();
	v0 += pCam->getMove() * 120.0f;
	v0 += pCam->getEyeVector() * 4000.0f;
	mEftCloudMtx.setW( v0 );
	
#if 0
	IPrim->add( TGL::PrimCross(mEftCloudMtx.getW(), 1000.f, IRGBA(255,255,0,255)), 0, TGL::Camera::CAMF_00 );
	
	TGL::Matrix	m0 = mEftCloudMtx;
	m0.setW( pCam->getTar() );
	IPrim->add( TGL::PrimPivot(m0, 1000.0f), 0, TGL::Camera::CAMF_00 );
#endif
}

// ---------------------------------------------
//! @brief  天球スケール
//! @param  bSceneChg	[in]	true:シーン変更瞬間
// ---------------------------------------------
void tBGStg03::updateFarScale( bool bSceneChg )
{
	//!	スケール更新
	if( mSceneOld < 3 ){
		mSkyNear	= 1500.0f;
		mSkyFar		= 150000.0f;
		mBossSkyScale.init( TGL::TransValue::MODE_NORMAL, 190, TGL::TransValue::CURVE_PARABOLIC, 999, 1.0f, mBossSkyScaleEnd );
	}
	mBossSkyScale.update( getDeltaTime() );
	
	//!	遠景にスケール適用
	cBGModel*	pModel = getModel( "St03_Far_02.mdl" );
	if( pModel )
		pModel->SetScale( mBossSkyScale.getValue() );
}

// ---------------------------------------------
//! @brief  固有遠景描画
//! @return true:ビューマトリクスの再更新が必要
// ---------------------------------------------
bool tBGStg03::drawFarEx()
{
	//!	雲海
	if( mScene == 2 ){
		drawCloud();
		return true;
	}
	return false;
}

// ---------------------------------------------
//! @brief  描画 雲海
// ---------------------------------------------
void tBGStg03::drawCloud()
{
	TGL::BaseCamera*	pCam = ICamera->getRenderCamera();
	if( !pCam )	return;

	//!	カメラの高さでNear, Far設定
	f32	fRate = TGL::Math::Clamp<f32>( pCam->getPos().y, -9000.0f, -8000.0f ) + 8000.0f;
	fRate /= -1000.0f;
	pCam->setNear( TGL::Math::Lerp<f32>(1000.0f, 10.0f, fRate) );
	pCam->setFar( TGL::Math::Lerp<f32>(40000.0f, 40000.0f, fRate) );
	pCam->updateViewMatrix();
	pCam->setRender( false );
	IGraphics->setWorldTransform();

	mPerfGpu.begin();

	PIX_BEGIN_EVENT(IRGBA(0xff, 0,0,0xff), "tBGStg03::drawCloud");
	PERF_BEGIN("tBGStg03::drawCloud",IRGBA( 200,0,0,128 ));

	//!	シェーダー取得
	TGL::ShaderEffect*	pEffect = IAppResource->getShaderEffect( "BG_Cloud" );

	//!	デバイス設定
	IGraphics->setWorldTransform();
	IGraphics->getRS().setZ( TGL::RenderState::Z_TEST | TGL::RenderState::Z_WRITE );
	IGraphics->getRS().setCull( TGL::RenderState::CULL_NONE );

	//!	定数バッファ
	TGL::cbSystem	cb;
	cb.gCameraToProjectionMatrix	= ICamera->getDrawMtx();
	
	TGL::cbLocal	cbL;
	TGL::BaseFog*	pFog = IFog->getFog(1);	//!< フォグ
	if( pFog ){
		cbL.gFogCoord	= pFog->getFogCoord();
		cbL.gFogColor	= pFog->getFogColorRgb();
	}else{
		cbL.gFogCoord	= TGL::Vec2::X;
		cbL.gFogColor	= TGL::Vec3::ONE;
	}
#ifdef PS4
	pEffect->setConstantBuffer( "cbSystem", &cb, sizeof(cb) );
	pEffect->setConstantBuffer( "cbLocal", &cbL, sizeof(cbL) );
#else
	pEffect->setMatrix(	"gCameraToProjectionMatrix", cb.gCameraToProjectionMatrix );
	pEffect->setValue(	"gFogCoord",		&cbL.gFogCoord, sizeof(TGL::Vec2) );
	pEffect->setValue(	"gFogColor",		&cbL.gFogColor, sizeof(FRGB) );
#endif

	//!	サンプラー
	pEffect->setTextureFilterPreset();

	//!	サイズ定義
	const u8	_LINE = 10;
	const f32	_SIZE = mCloudSize;
	const f32	_SIZEHALF = _SIZE / 2.0f;
	const f32	_SIZECELL = _SIZE / _LINE;
	
	//!	カメラ相対の中心座標
	s32	nOfsZ=0;
	TGL::Vec3	vOfs = pCam->getTar();
	while( FABS(vOfs.z) > _SIZECELL+DELTA ){	//!< Z
		if( vOfs.z > 0.0f ){
			vOfs.z -= _SIZECELL;
			nOfsZ++;
		}else{
			vOfs.z += _SIZECELL;
			nOfsZ--;
		}
	}
	vOfs = pCam->getTar() - vOfs;

	//!	UV回転
	TGL::Matrix	mtxUV = TGL::Matrix::UNIT;
	mtxUV.rollZ( mCloudRot );

	//!	描画
	pEffect->begin();

	for( s8 nC=numberof(mCloudParam)-1 ; nC>=0 ; nC-- ){
		const CloudParam&	Param = mCloudParam[nC];

		//!	合成モード
		IGraphics->getRS().setBlend( Param.mBlend, true );
		//!	テクスチャ
		const char*	_TEX[] = {
			"St03_Cloud_Trp1_Albedo",
			"St03_Cloud_Trp2_Albedo",
			"St03_Cloud_Albedo",
		};
		TGL::BaseTex*	pTex = IAppResource->getTexPool( _TEX[nC] );
		pEffect->setTexture( "gTexture", pTex );
		pEffect->commitChange();
		
		for( u8 v=0 ; v<_LINE ; v++ ){
			TGL::Vec3	vBase = vOfs;
			vBase.x += (f32)( -(_SIZEHALF-_SIZECELL/2)+(_SIZECELL)*v );
			vBase.y = mCloudParam[0].mHeight;	//!< 高さ
			if( nC != 0 )	vBase.y += Param.mHeight;
			
			//!	頂点設定
			TGL::Vertex::POS_COLOR_TEX_T	vtx[_LINE*2];
			u8	vnum = 0;
			for( u8 u=0 ; u<_LINE ; u++ ){
				//!	頂点設定
				vBase.z = vOfs.z + (f32)( -(_SIZEHALF-_SIZECELL/2)+(_SIZECELL)*u );
				vtx[vnum].mPos		= vBase;
				vtx[vnum].mTex.x	= Param.mUVOfs + (Param.mUVRepeat / (f32)_LINE * (f32)(u+nOfsZ));
				vtx[vnum].mTex.y	= Param.mUVOfs + (Param.mUVRepeat / (f32)_LINE * (f32)(v+0));
				vtx[vnum].mColor	= Param.mColor;
				vnum++;
				vtx[vnum].mPos		= vBase;
				vtx[vnum].mPos.x	+= _SIZECELL;
				vtx[vnum].mTex.x	= vtx[vnum-1].mTex.x;
				vtx[vnum].mTex.y	= Param.mUVOfs + (Param.mUVRepeat / (f32)_LINE * (f32)(v+1));
				vtx[vnum].mColor	= Param.mColor;
				vnum++;
				//!	UV回転
				for( u8 i=vnum-2 ; i<vnum ; i++ ){
					TGL::Vec3	v0;
					v0 = TGL::Vec3(vtx[i].mTex.x, vtx[i].mTex.y, 0).applySR( mtxUV );
					vtx[i].mTex = TGL::Vec2( v0.x, v0.y );
				}
			}

			//!	描画
			IVertex->setVertexLayout( TGL::Vertex::VTXDEC_COLOR_TEX, vtx, vnum, pEffect );
			IGraphics->draw( TGL::Graphics::PRIM_TRIANGLESTRIP );
		}
	}
	pEffect->end();

	//!	Zバッファを消す
	IGraphics->clearScreen( TGL::Graphics::CLEARF_ZBUFFER, TGL::Color(0,0,0,0) );

	PERF_END("tBGStg03::drawCloud");
	PIX_END_EVENT();
	mPerfGpu.end();
}

/* ========================================================================= */
/*		デバッグ															 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  デバッグ情報表示
// ---------------------------------------------
void tBGStg03::dispDebugInfo()
{
#ifndef _TGL_FINAL
	if( !mbDebugInfo )	return;
	
	//!	親
	tBG::dispDebugInfo();
	
	TGL::BaseCamera*	pCam = ICamera->getCamera(0);
	if( !pCam )	return;
	
	//!	ロケータ表示
	stl::vector<TGL::Vec2*>	mPosBuf;	//!< 表示2D座標
	dispDebugInfoLocator( pCam, "St03_01_Loop.mdl", "Dust_BB_LOC_" );
	
	dispDebugInfoLocator( pCam, "St03_02_Loop.mdl", "St03_Cloud01A_LOC_", &mPosBuf );
	dispDebugInfoLocator( pCam, "St03_02_Loop.mdl", "St03_Cloud01B_LOC_", &mPosBuf );
	dispDebugInfoLocator( pCam, "St03_02_Loop.mdl", "St03_Cloud01C_LOC_", &mPosBuf );
	foreach( stl::vector<TGL::Vec2*>, mPosBuf, it )
		SAFE_DELETE( *it );
	mPosBuf.clear();

	cBGModel*	pModel = getModel( "St03_06.mdl" );
	dispDebugInfoLocator( pCam, pModel, "St03_06_BB_A_LOC_", &mPosBuf );
	dispDebugInfoLocator( pCam, pModel, "St03_06_BB_B_LOC_", &mPosBuf );
	dispDebugInfoLocator( pCam, pModel, "St03_06_BB_C_LOC_", &mPosBuf );
	foreach( stl::vector<TGL::Vec2*>, mPosBuf, it )
		SAFE_DELETE( *it );

#endif	/*	_TGL_FINAL	*/
}
