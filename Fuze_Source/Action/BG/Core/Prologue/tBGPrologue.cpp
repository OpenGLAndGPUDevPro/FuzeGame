// -------------------------------------------------------------------------- //
//! @file   tBGPrologue.cpp
//! @brief  背景 プロローグ
//! @author Nal
//! @since  13/05/14(火)
//! 
//! COPYRIGHT (C) 2013 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
/* ====< インクルード >====================================================== */
#include "prefix.h"
#include <TGL.h>
#include <Effect/Program/EPrg.inc>
#include "Game/Game.h"
#include "Action/Camera/cBGCamera.h"
#include "tBGPrologue.h"
#include "Action/ActionTypes.h"
#include "Effect/sAppEffect.h"
#include "Sound/sAppSound.h"
#include "../../../../../tgs/scene/Prologue/scn_Prologue.h"

//#define	CITY_OBJ	//!< 市街地のロケータ配置

static const char*	_PARAM_EX = "param/BG/Prologue/Prologue_paramEx.json";	//!< 固有パラメータリソース

#ifdef _WINDOWS
#pragma warning(disable : 4355)
#endif
/* ========================================================================= */
//!		tBGPrologue															 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  コンストラクタ "tBGPrologue"
// ---------------------------------------------
tBGPrologue::tBGPrologue()
:	tBG(),
	mLightningInt(0,10,240), mLightningRel(0,10,30), mpLensflare(NULL),
	mpRain(NULL), mRainSpeedValley(0), mRainSpeedCity(0), 
	mRainColorBaseMin(IRGBA(255,255,255,255)), mRainColorBaseMax(IRGBA(255,255,255,255)),
	mRainColorLightMin(IRGBA(255,255,255,255)), mRainColorLightMax(IRGBA(255,255,255,255)),
	mCityFall_Uv(TGL::Vec2(0,0)), mSortCity(*this),
	mDriveStar_Uv(TGL::Vec2(0,0)), mDrive_UvA(TGL::Vec2(0,0)), mDrive_UvB(TGL::Vec2(0,0)), mDrive_UvC(TGL::Vec2(0,0)), mDrive_UvD(TGL::Vec2(0,0))
{
	SLLINK_Init( &mEftCloudLink );
	reset();
}

// ---------------------------------------------
//! @brief  デストラクタ "tBGPrologue"
// ---------------------------------------------
tBGPrologue::~tBGPrologue()
{
	IAppResource->releaseTexPool( "models/BG/common/St00/St00_Flare01.dds" );
	IAppResource->releaseTexPool( "models/BG/common/St00/St00_Flare02.dds" );
	IAppResource->releaseTexPool( "models/BG/common/St00/St00_Flare03.dds" );
	IAppResource->releaseTexPool( "models/BG/common/St00/St00_Flare04.dds" );
	IAppResource->releaseTexPool( "models/BG/common/St00/St00_Post_ToneCurve_nmip.dds" );
	IAppResource->releaseTexPool( "models/BG/common/St00/St00_Post_ToneCurve_Sky_nmip.dds" );
	IAppResource->releaseTexPool( "models/BG/common/St00/St00_Post_ToneCurve_Space_nmip.dds" );
	IAppResource->releaseTexPool( "models/BG/common/St00R/St00_Drive_envC.dds" );
	IAppResource->releaseTexPool( "models/BG/common/St00R/St00_envC.dds" );
	IAppResource->releaseTexPool( "models/BG/common/St00R/St00_Sharp_envC.dds" );
	foreach( stl::vector<Meteor*>, mMeteor, it )
		SAFE_DELETE( *it );
	SAFE_DELETE( mpLensflare );
	SAFE_DELETE( mpRain );
	EEMIT_End( &mEftCloudLink );
	ITGLSound->stop( 0, TGL::Sound::STOPFS_ONLY_AMBIENT | TGL::Sound::STOPF_N_ONMEMORY );
	ITGLRenderer->getFilterToneCurve()->setTexCurve( NULL );
	ITGLRenderer->getFilterToneCurve()->setEnableValue( false );
}

// ---------------------------------------------
//! @brief  リセット
// ---------------------------------------------
void tBGPrologue::reset()
{
	tBG::reset();
	EEMIT_End( &mEftCloudLink );
	mLightning	= mLightningSE = 0;
	ZeroMemory( mbLightningSE, sizeof(mbLightningSE) );
	mLightningAlpha.clear();
	mMeteorReq	= 0;
	foreach( stl::vector<Meteor*>, mMeteor, it )
		SAFE_DELETE( *it );
	mMeteor.clear();
	mEftCloudMtx	= TGL::Matrix::UNIT;
	ITGLSound->stop( 0, TGL::Sound::STOPFS_ONLY_AMBIENT | TGL::Sound::STOPF_N_ONMEMORY );
}

// ---------------------------------------------
//! @brief  リソースロード
//! @param  uBG	[in]	BGID_????
//! @return true:成功, false:失敗
// ---------------------------------------------
bool tBGPrologue::load( u8 uBG )
{
	//!		モデル		
	cBGModel*	pModel;
	const u32	_SCENE_VALLEY	= (1<<0) | (1<<STGPRSCENE_METEORFAR) | (1<<STGPRSCENE_METEORNEAR) | (1<<STGPRSCENE_METEOR) | (1<<STGPRSCENE_VALLEY) | (1<<STGPRSCENE_CITYENTRANCE);
	const u32	_SCENE_CITY		= (1<<STGPRSCENE_CITYENTRANCE) | (1<<STGPRSCENE_CITY);
	const u32	_SCENE_SKY		= (1<<STGPRSCENE_SKY);
	const u32	_SCENE_DRIVE	= (1<<STGPRSCENE_DRIVE);
	const u32	_SCENE_SPACE	= (1<<STGPRSCENE_SPACE);
	//!	遠景 暗雲
	pModel = addModel( "St00_Far_01.mdl", cBGModel::TYPE_SKYDOME_Y, cBGModel::FLAG_FAROBJ );
	if( pModel )	pModel->setOnScene( _SCENE_VALLEY | _SCENE_CITY );
	//!	遠景 暗雲稲光
	pModel = addModel( "St00_Far_01_Lightning.mdl", cBGModel::TYPE_SKYDOME_Y, cBGModel::FLAG_FAROBJ );
	if( pModel )	pModel->setOnScene( _SCENE_VALLEY | _SCENE_CITY );
	//!	遠景
	pModel = addModel( "St00_Far_02.mdl", cBGModel::TYPE_SKYDOME_Y, cBGModel::FLAG_FAROBJ );
	if( pModel )	pModel->setOnScene( _SCENE_SKY );
	//!	遠景
	pModel = addModel( "St00_Far_03.mdl", cBGModel::TYPE_SKYDOME_Y, cBGModel::FLAG_FAROBJ );
	if( pModel )	pModel->setOnScene( _SCENE_DRIVE );
	//!	ドライブループ
	pModel = addModel( "St00_SLSDLoop.mdl", cBGModel::TYPE_SKYDOME_Y, cBGModel::FLAG_FAROBJ );
	if( pModel )	pModel->setOnScene( _SCENE_DRIVE );
	//!	遠景
	pModel = addModel( "St00_Far_04.mdl", cBGModel::TYPE_SKYDOME_Y, cBGModel::FLAG_FAROBJ );
	if( pModel )	pModel->setOnScene( _SCENE_SPACE );
	//!	崖入口
	pModel = addModel( "St00_ValleyStart.mdl" );
	if( pModel )	pModel->setOnScene( _SCENE_VALLEY );
	//!	崖ループ
	pModel = addModel( "St00_ValleyLoop.mdl" );
	if( pModel )	pModel->setOnScene( _SCENE_VALLEY );
	//!	崖出口
	pModel = addModel( "St00_ValleyEnd.mdl" );
	if( pModel )	pModel->setOnScene( _SCENE_VALLEY );
	//!	市街ロケータ
	pModel = addModel( "St00_CityLoop.mdl" );
	if( pModel )	pModel->setOnScene( _SCENE_CITY );
#ifdef CITY_OBJ
	//!	市街
	pModel = addModel( "Obj_St00_City_A.mdl" );
	if( pModel )	pModel->setOnScene( _SCENE_CITY );
	pModel = addModel( "Obj_St00_City_B.mdl" );
	if( pModel )	pModel->setOnScene( _SCENE_CITY );
	pModel = addModel( "Obj_St00_City_C.mdl" );
	if( pModel )	pModel->setOnScene( _SCENE_CITY );
#endif	/*	CITY_OBJ	*/
	//!	市街ロケータ
	pModel = addModel( "St00_Freet.mdl" );
	if( pModel )	pModel->setOnScene( _SCENE_SPACE );

	//!		テクスチャ		
	IAppResource->loadTexPool( "models/BG/common/St00/St00_Flare01.dds" );
	IAppResource->loadTexPool( "models/BG/common/St00/St00_Flare02.dds" );
	IAppResource->loadTexPool( "models/BG/common/St00/St00_Flare03.dds" );
	IAppResource->loadTexPool( "models/BG/common/St00/St00_Flare04.dds" );
	IAppResource->loadTexPool( "models/BG/common/St00/St00_Post_ToneCurve_nmip.dds" );
	IAppResource->loadTexPool( "models/BG/common/St00/St00_Post_ToneCurve_Sky_nmip.dds" );
	IAppResource->loadTexPool( "models/BG/common/St00/St00_Post_ToneCurve_Space_nmip.dds" );
	IAppResource->loadTexPool( "models/BG/common/St00R/St00_Drive_envC.dds" );
	IAppResource->loadTexPool( "models/BG/common/St00R/St00_envC.dds" );
	IAppResource->loadTexPool( "models/BG/common/St00R/St00_Sharp_envC.dds" );

	//!	崖当たり
	mValleyCol.clear();
	for( u8 i=0 ; i<3 ; i++ ){
		const f32	_POS[][3] = {
			{     0, -4500, 0 },
			{  2800,     0, 0 },
			{ -2800,     0, 0 },
		};
		const f32	_NORMAL[][3] = {
			{  0.0f, 1.0f, 0.0f },
			{ -0.9f, 0.1f, 0.0f },
			{  0.9f, 0.1f, 0.0f },
		};
		TGL::ColPlane	plane;
		plane.setup( TGL::MtxGuide(_POS[i]), _NORMAL[i] );
		mValleyCol.push_back( plane );
	}

	//!		シーンパラメータ		
	mSceneParam.getFromJson( "param/BG/Prologue/Prologue_param.json" );
	{	//!	固有パラメータ
		TGL::JsonUtil	json( _PARAM_EX );
		TGL::Json::array	jsonArray = json.getArray("-");
		foreach( TGL::Json::array, jsonArray, it ){
			TGL::Json::object&	work = it->get<TGL::Json::object>();
			mLightningInt.setMin( (f32)atof( work["Lightning_Interval_Min"].to_str().c_str() ) );
			mLightningInt.setMax( (f32)atof( work["Lightning_Interval_Max"].to_str().c_str() ) );
			mLightningRel.setMin( (f32)atof( work["Lightning_Release_Min"].to_str().c_str() ) );
			mLightningRel.setMax( (f32)atof( work["Lightning_Release_Max"].to_str().c_str() ) );

			mRainSpeedValley	= (f32)atof( work["Rain_Speed_Valley"].to_str().c_str() );
			mRainSpeedCity		= (f32)atof( work["Rain_Speed_City"].to_str().c_str() );
			mRainColorBaseMin	= IRGBA(	atoi( work["Rain_ColorBaseMin_R"].to_str().c_str() ),
											atoi( work["Rain_ColorBaseMin_G"].to_str().c_str() ),
											atoi( work["Rain_ColorBaseMin_B"].to_str().c_str() ),
											atoi( work["Rain_ColorBaseMin_A"].to_str().c_str() ) );
			mRainColorBaseMax	= IRGBA(	atoi( work["Rain_ColorBaseMax_R"].to_str().c_str() ),
											atoi( work["Rain_ColorBaseMax_G"].to_str().c_str() ),
											atoi( work["Rain_ColorBaseMax_B"].to_str().c_str() ),
											atoi( work["Rain_ColorBaseMax_A"].to_str().c_str() ) );
			mRainColorLightMin	= IRGBA(	atoi( work["Rain_ColorLightMin_R"].to_str().c_str() ),
											atoi( work["Rain_ColorLightMin_G"].to_str().c_str() ),
											atoi( work["Rain_ColorLightMin_B"].to_str().c_str() ),
											atoi( work["Rain_ColorLightMin_A"].to_str().c_str() ) );
			mRainColorLightMax	= IRGBA(	atoi( work["Rain_ColorLightMax_R"].to_str().c_str() ),
											atoi( work["Rain_ColorLightMax_G"].to_str().c_str() ),
											atoi( work["Rain_ColorLightMax_B"].to_str().c_str() ),
											atoi( work["Rain_ColorLightMax_A"].to_str().c_str() ) );

			mCityFall_Uv		= TGL::Vec2( (f32)atof( work["CityWaterFall_Speed_U"].to_str().c_str() ),
										(f32)atof( work["CityWaterFall_Speed_V"].to_str().c_str() ) );

			mDriveStar_Uv	= TGL::Vec2( (f32)atof( work["DriveStar_Speed_U"].to_str().c_str() ),
									(f32)atof( work["DriveStar_Speed_V"].to_str().c_str() ) );
			mDrive_UvA		= TGL::Vec2( (f32)atof( work["Drive_A_Speed_U"].to_str().c_str() ),
									(f32)atof( work["Drive_A_Speed_V"].to_str().c_str() ) );
			mDrive_UvB		= TGL::Vec2( (f32)atof( work["Drive_B_Speed_U"].to_str().c_str() ),
									(f32)atof( work["Drive_B_Speed_V"].to_str().c_str() ) );
			mDrive_UvC		= TGL::Vec2( (f32)atof( work["Drive_C_Speed_U"].to_str().c_str() ),
									(f32)atof( work["Drive_C_Speed_V"].to_str().c_str() ) );
			mDrive_UvD		= TGL::Vec2( (f32)atof( work["Drive_D_Speed_U"].to_str().c_str() ),
									(f32)atof( work["Drive_D_Speed_V"].to_str().c_str() ) );
		}
	}
	
	//!		パラメータ		
	//!	モデル配置
	applyModelSetFromJson( "param/BG/Prologue/Prologue_modelset.json" );

#ifdef CITY_OBJ
	//!	都市 ロケータ配置列挙
	mSortCity.setBegin( mModelSet.size() );
	for( u8 j=0 ; j<3 ; j++ ){
		for( u8 i=1 ;; i++ ){
			//!	ロケーター名
			const char*	_LOCATOR[] = {
				"St00_CityA_LOC_",
				"St00_CityB_LOC_",
				"St00_CityC_LOC_",
			};
			//!	モデル
			const char*	_OBJ[] = {
				"Obj_St00_City_A.mdl",
				"Obj_St00_City_B.mdl",
				"Obj_St00_City_C.mdl",
			};
			stl::string	strLoc = stl::FormatString<stl::string>( "%s%02d", _LOCATOR[j], i );
			cModelSet*	pSet = addModelSetAssign( _OBJ[j], "St00_CityLoop.mdl", strLoc, true );
			if( !pSet )
				break;
			pSet->setId( i );
			pSet->setOnScene( _SCENE_CITY );
		}
	}
	//!	ソート用設定
	mSortCity.setEnd( mModelSet.size() );
	mSortCity.setScene( _SCENE_CITY );
	mSortCity.setArray( &mModelSet );
	mSortCity.setSortCB( cModelSetSort::sortCB_ZNear );
#endif	/*	CITY_OBJ	*/
	
	//!	カメラパス追加
	applyCameraPathSetFromJson( "param/BG/Prologue/Prologue_camera.json" );

	//!	UVアニメ設定
	setModelUvAnim();

	//!	トーンカーブ
	ITGLRenderer->getFilterToneCurve()->setEnableValue( true );
	ITGLRenderer->getFilterToneCurve()->setTexCurve( IAppResource->getTexture("St00_Post_ToneCurve_nmip") );

	//!	シーン環境マップ
	mSceneParam.setEnvTex( "St00_envC.dds" );
	ICharacter->overwriteBGEnvTex();	//!< キャラクターへの適用
	overwriteEnvTex();	//!< 背景モデルへの適用

	//!	更新関数登録
	addUpdateEvent( updateCloudParCB,	0, STGPRSCENE_CITY, setupCloudParCB, finalCloudParCB );	//!< 雲パーティクル
	addUpdateEvent( updateRainCB,		0, STGPRSCENE_CITY, setupRainCB, finalRainCB );	//!< 雨
	addUpdateEvent( updateLightningCB,	0, STGPRSCENE_CITY );	//!< 稲光
	addUpdateEvent( NULL,				STGPRSCENE_METEORFAR, STGPRSCENE_METEORFAR, setupFireCB, finalFireCB );	//!< 戦艦周辺破片
	addUpdateEvent( updateMeteorCB,		STGPRSCENE_METEORNEAR, STGPRSCENE_METEOR );	//!< 破片
	addUpdateEvent( NULL,				STGPRSCENE_SKY, STGPRSCENE_SKY, setupSkyCB, finalSkyCB );	//!< 空
	addUpdateEvent( updateCloudParCB,	STGPRSCENE_SKY, STGPRSCENE_SKY, NULL, finalCloudParCB );	//!< 雲パーティクル
	addUpdateEvent( NULL,				STGPRSCENE_SPACE, STGPRSCENE_SPACE, setupSpaceCB, finalSpaceCB );	//!< 宇宙

	return true;
}

// ---------------------------------------------
//! @brief  UVアニメ設定
// ---------------------------------------------
void tBGPrologue::setModelUvAnim()
{
	cBGModel*	pModel;
	
	//!	市街地
#ifdef CITY_OBJ
	pModel = getModel( "Obj_St00_City_A.mdl" );
	if( pModel )	pModel->SetNodeUVAnime( "City_Fall_A_alpha", mCityFall_Uv );
	pModel = getModel( "Obj_St00_City_B.mdl" );
	if( pModel )	pModel->SetNodeUVAnime( "City_Fall_B_alpha", mCityFall_Uv );
	pModel = getModel( "Obj_St00_City_C.mdl" );
	if( pModel )	pModel->SetNodeUVAnime( "City_Fall_C_alpha", mCityFall_Uv );
#else
	pModel = getModel( "St00_CityLoop.mdl" );
	if( pModel )	pModel->SetNodeUVAnime( "City_Fall_alpha", mCityFall_Uv );
#endif	/*	CITY_OBJ	*/

	//!	ドライブ
	pModel = getModel( "St00_Far_03.mdl" );
	if( pModel )	pModel->SetNodeUVAnime( "St02_01_Star_nofog", mDriveStar_Uv );
	pModel = getModel( "St00_SLSDLoop.mdl" );
	if( pModel ){
		pModel->SetNodeUVAnime( "SLSD_A_add", mDrive_UvA );
		pModel->SetNodeUVAnime( "SLSD_B_add", mDrive_UvB );
		pModel->SetNodeUVAnime( "SLSD_C_add", mDrive_UvC );
		pModel->SetNodeUVAnime( "SLSD_D_add", mDrive_UvD );
	}
}

// ---------------------------------------------
//! @brief  エフェクト開始
// ---------------------------------------------
void tBGPrologue::beginEffect()
{
}

// ---------------------------------------------
//! @brief  更新
// ---------------------------------------------
void tBGPrologue::update()
{
	//!	トーンカーブ
	if( mScene != mSceneOld ){
		if( mScene >= STGPRSCENE_SPACE )
			ITGLRenderer->getFilterToneCurve()->setTexCurve( IAppResource->getTexture("St00_Post_ToneCurve_Space_nmip") );
		else if( mScene == STGPRSCENE_SKY )
			ITGLRenderer->getFilterToneCurve()->setTexCurve( IAppResource->getTexture("St00_Post_ToneCurve_Sky_nmip") );
		else
			ITGLRenderer->getFilterToneCurve()->setTexCurve( IAppResource->getTexture("St00_Post_ToneCurve_nmip") );
	}

	//!	親クラス
	tBG::update();
	
	//!	破片
	for( stl::vector<Meteor*>::iterator it=mMeteor.begin() ; it!=mMeteor.end() ; ){
		if( (*it)->mbEnd ){
			SAFE_DELETE( *it );
			it = mMeteor.erase( it );
		}else{
			(*it)->update();
			++it;
		}
	}
}

// ---------------------------------------------
//! @brief  カメラ更新後の処理
// ---------------------------------------------
void tBGPrologue::updateCameraAfter()
{
	tBG::updateCameraAfter();
	
	//!	都市のソート
	mSortCity.sort();
}

// ---------------------------------------------
//! @brief  スクリプトメッセージ受け取り
//! @param  nId		[in]	メッセージID
//! @param  nParam	[in]	パラメータ
// ---------------------------------------------
void tBGPrologue::receiveMessage( s32 nId, s32 nParam )
{
	switch( nId ){
		case STGPRMSG_WINDSTOP:	//!	風音とめる
			ITGLSound->stop( nParam, TGL::Sound::STOPFS_ONLY_AMBIENT | TGL::Sound::STOPF_N_ONMEMORY );
			mLightningSE = -1;
			break;

		case STGPRMSG_CLOUDPAR:	//!	雲パーティクルリクエスト
			//!	パーティクル
			if( !ITGLSystem->isGraphicsLevelLow() && !ITGLSystem->isPostFilterLevelLow() ){
				TGL::MtxGuide	guide( &mEftCloudMtx );
				IAppEffect->req( EFTNO_SCENES0_CLOUDFOG2, TGL::Vec3::ZERO, TGL::Vec3::ZERO, NULL, &guide, NULL, NULL, 0, 0, &mEftCloudLink );
			}
			break;

		case STGPRMSG_SKYROT:	//!	天球回す
		{	cBGModel*	pModel = getModel( "St00_Far_02.mdl" );
			if( !pModel )	return;
			pModel->setRot( TGL::Vec3(0, (f32)nParam, 0) );
		}	break;
	}
}

// ---------------------------------------------
//! @brief  雲パーティクル 初期化
// ---------------------------------------------
void tBGPrologue::setupCloudPar()
{
	//!	パーティクル
	if( !ITGLSystem->isGraphicsLevelLow() && !ITGLSystem->isPostFilterLevelLow() ){
		TGL::MtxGuide	guide( &mEftCloudMtx );
		IAppEffect->req( EFTNO_SCENES0_CLOUDFOG, TGL::Vec3::ZERO, TGL::Vec3::ZERO, NULL, &guide, NULL, NULL, 0, 0, &mEftCloudLink );
	}
}
// ---------------------------------------------
//! @brief  雲パーティクル 終了
// ---------------------------------------------
void tBGPrologue::finalCloudPar()
{
	EEMIT_End( &mEftCloudLink );
}
// ---------------------------------------------
//! @brief  雲パーティクル
//! @param  bSceneChg	[in]	true:シーン変更瞬間
// ---------------------------------------------
void tBGPrologue::updateCloudPar( bool bSceneChg )
{
	//!	パーティクル追従マトリクス更新
	TGL::BaseCamera*	pCam = ICamera->getCamera(0);
	//!	回転
	TGL::Vec3	v0 = -pCam->getMove();
	v0.y = 0;	//!< 水平のみ
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
	v0 += pCam->getMove() * 100.0f * TGL::Math::Clamp<f32>( pCam->getEyeDistance()/500.0f, 0.1f, 1.0f );
	v0 += pCam->getEyeVector() * 4000.0f;
	v0 += pCam->getUpVector() * RANDF2(-4000.0f, 4000.0f);
	mEftCloudMtx.setW( v0 );
	
#if 0
	IPrim->add( TGL::PrimCross(mEftCloudMtx.getW(), 1000.f, IRGBA(255,255,0,255)), 0, TGL::Camera::CAMF_00 );
	
	TGL::Matrix	m0 = mEftCloudMtx;
	m0.setW( pCam->getTar() );
	IPrim->add( TGL::PrimPivot(m0, 1000.0f), 0, TGL::Camera::CAMF_00 );
#endif
}

// ---------------------------------------------
//! @brief  雨 初期化
// ---------------------------------------------
void tBGPrologue::setupRain()
{
	if( mpRain )	return;

	if( !ITGLSystem->isGraphicsLevelLow() && !ITGLSystem->isPostFilterLevelLow() )
		mpRain = APP_NEW tRain( this, GPRI_EFTPRE(0) );
	
	TGL::SoundChannel*	pCh = IAppSound->play( SDID_STREAM, AMBSTR_STORM );
	if( pCh ){
		pCh->setVolume( 0.01f, 1.0f, 60 );
		pCh->setUserId( SNDOWNER_AMBIENT );
	}
}
// ---------------------------------------------
//! @brief  雨 終了
// ---------------------------------------------
void tBGPrologue::finalRain()
{
	SAFE_DELETE( mpRain );
	TGL::SoundChannel*	pCh = ITGLSound->getChFromUserId( SNDOWNER_AMBIENT );
	if( pCh && pCh->getSoundId() == AMBSTR_STORM )
		pCh->stop();
}
// ---------------------------------------------
//! @brief  雨
//! @param  bSceneChg	[in]	true:シーン変更瞬間
// ---------------------------------------------
void tBGPrologue::updateRain( bool bSceneChg )
{
	if( mpRain ){
		//!	ライト
		mpRain->setColorMin( RGBA32_Lerp(mRainColorLightMin, mRainColorBaseMin, mSceneParam.getBlendTime().getValue()) );
		mpRain->setColorMax( RGBA32_Lerp(mRainColorLightMax, mRainColorBaseMax, mSceneParam.getBlendTime().getValue()) );
		//!	速度
		if( mScene < STGPRSCENE_CITYENTRANCE )
			mpRain->setSpd( TGL::Vec3(0, 0, -mRainSpeedValley) );
		else
			mpRain->setSpd( TGL::Vec3(0, 0, -mRainSpeedCity) );
	}
}

// ---------------------------------------------
//! @brief  稲光
//! @param  bSceneChg	[in]	true:シーン変更瞬間
// ---------------------------------------------
void tBGPrologue::updateLightning( bool bSceneChg )
{
	mLightning -= getDeltaTime();
	if( mLightning < DELTA ){
		f32	fRelease = mLightningRel.rand();	//!< 減衰時間
		//!	ライト
		if( !mbSceneApplyOff ){
			mSceneParam.setCurrentId( STGPRSP_UNDERLIGHT );
			mSceneParam.setCurrentId( STGPRSP_UNDER, (s32)fRelease, TGL::TransValue::CURVE_SUB1 );
		}

		//!	遠景影響
		mLightningAlpha.init( TGL::TransValue::MODE_NORMAL, fRelease, TGL::TransValue::CURVE_SUB1, 999, 1.0f, 0.0f );
		cBGModel*	pModel = getModel( "St00_Far_01_Lightning.mdl" );
		if( pModel ){
			pModel->setRot( TGL::Vec3(0, RANDF2(-180,180), 0) );
		}

		//!	次回
		mLightning = mLightningInt.rand();
	}
	
	//!	遠景アルファ
	cBGModel*	pModel = getModel( "St00_Far_01_Lightning.mdl" );
	if( pModel ){
		pModel->SetModelAlpha( mLightningAlpha.getValue() );
	}
	mLightningAlpha.update( getDeltaTime() );
	
	//!	音
	if( mLightningSE >= 0.0f ){
		mLightningSE -= getDeltaTime();
		if( mLightningSE < DELTA ){
			//!	SEテーブル
			u8	i0 = 0;
			for( u8 i=0 ; i<numberof(mbLightningSE) ; i++ ){
				if( mbLightningSE[i] )
					i0++;
			}
			if( i0 >= numberof(mbLightningSE) )
				ZeroMemory( mbLightningSE, sizeof(mbLightningSE) );
			
			u16 uSE = 0;
			for(;;){
				uSE = RANDI2( 0, numberof(mbLightningSE)-1 );
				if( !mbLightningSE[uSE] ){
					mbLightningSE[uSE] = true;
					break;
				}
			}
			
			//!	再生
			TGL::SoundChannel*	pCh = IAppSound->play( SDID_ONMEM, uSE + SE_S0_THUNDER_00 );
			if( pCh ){
				pCh->setVolume( RANDF2(0.3f, 1.0f) );
				pCh->setPan( RANDF2(-1.0f, 1.0f) );
				pCh->setSPan( RANDF2(-1.0f, 1.0f) );
			}
			
			mLightningSE = RANDF2( 60, 240 );
		}
	}
}

// ---------------------------------------------
//! @brief  戦艦周辺破片ジェネレータ 初期化
// ---------------------------------------------
void tBGPrologue::setupFire()
{
	const TGL::Vec3	_CENTER( 0, 11000, 108000 );
	//!	破片
	for( u8 i=0 ; i<8 ; i++ ){
		//!	座標
		TGL::Vec3	v0( TGL::Vec3::rand() );
		v0.y *= 0.25f;
		v0 = _CENTER + v0 * RANDF2(3000.0f,4000.0f);
		TGL::Vec3	vPos( v0 );

		//!	方向
		v0.y -= 5000.0f;
		v0 = _CENTER.wayNL( v0 );
		TGL::Matrix	m0( TGL::Matrix::createVec( TGL_Z, v0 ) );
		m0.setW( vPos );
		
		mMeteor.push_back( APP_NEW Meteor(this, m0, RANDF2(10,30)) );
	}
	//!	レンズフレア
	SAFE_DELETE( mpLensflare );
	mpLensflare = createLensFlare( TGL::MtxGuide(_CENTER), "St00_Flare", _PARAM_EX, "Lensflare_ShipBreak" );
}
// ---------------------------------------------
//! @brief  戦艦周辺破片ジェネレータ 終了
// ---------------------------------------------
void tBGPrologue::finalFire()
{
	foreach( stl::vector<Meteor*>, mMeteor, it )
		SAFE_DELETE( *it );
	mMeteor.clear();
	SAFE_DELETE( mpLensflare );
}

// ---------------------------------------------
//! @brief  更新 破片ジェネレータ
//! @param  bSceneChg	[in]	true:シーン変更瞬間
// ---------------------------------------------
void tBGPrologue::updateMeteor( bool bSceneChg )
{
	mMeteorReq -= getDeltaTime();
	if( mMeteorReq < DELTA ){
		TGL::Matrix	m0;
		TGL::Vec3	v0;
		//!	方向
		v0 = TGL::Vec3::waySphere( TGL_Y, RANDF2(-45,45), RANDF2(-180,180) );
		v0 = -v0;
		m0 = TGL::Matrix::createVec( TGL_Z, v0 );
		
		//!	座標
		TGL::BaseCamera*	pCam = ICamera->getCamera(0);
		f32	fDist = pCam->getEyeDistance();
		v0 = pCam->getTar() + TGL::Vec3(RANDF2(-fDist,fDist), 5000, RANDF2(-fDist,fDist));
		pCam->adjustPosScreen( v0, -100 );
		v0 += pCam->getMove() * 15.0f;
		m0.setW( v0 );
		
		mMeteor.push_back( APP_NEW Meteor(this, m0, RANDF2(80,300)) );
		
		mMeteorReq = RANDF2( 10, 30 );
	}
}

// ---------------------------------------------
//! @brief  コンストラクタ "Meteor"
//! @param  mtx		[in]	マトリクス
//! @param  fSpd	[in]	速度
// ---------------------------------------------
tBGPrologue::Meteor::Meteor( tBGPrologue* pBg, const TGL::Matrix& mtx, f32 fSpd )
:	mpOwner(pBg), mbEnd(false), mMtx(mtx), mSpd(fSpd), mhSE(0)
{
	SLLINK_Init( &mEft );
	
	mCol.setup( TGL::MtxGuide(TGL::Vec3::ZERO), TGL::MtxGuide(TGL::Vec3::ZERO), 30.0f );
	
	//!	エフェクト発生
	TGL::MtxGuide	guide( &mMtx );
	IAppEffect->req( EFTNO_SCENES0_METEOR, TGL::Vec3::ZERO, TGL::Vec3::ZERO, NULL, &guide, NULL, NULL, 0, 0, &mEft );

	EPRGZANBBDSET_T		set;
	ZeroMemory( &set, sizeof(EPRGZANBBDSET_T) );
	set.cmn.uFlag		= EPRGF_ZTEST;
	set.cmn.uBlend		= RSBL_ADD;
	set.cmn.uTexHash	= TGL::Texture::HASH_NULL;
	set.cmn.nDelay		= 0;
	set.cmn.nFadeIn		= 0;
	set.cmn.nLife		= -1;
	set.cmn.nFadeOut	= 8;
	set.cmn.uTot		= GTOT_EFT(0);
	set.cmn.uPri		= GPRI_EFT(0);
	set.cmn.uCamFlag	= TGL::Camera::CAMF_00;
	set.cmn.uDrawCount	= 1;
	set.uFlag			= EPRGZANBF_BFADE | EPRGZANBF_FADESTOP;
	set.uLocus			= 16;
	set.uDiv			= 1;
	set.fWidthS			= 80.0f;
	set.fWidthE			= 80.0f;
	set.uRgbaI			= IRGBA(255,200,128,255);
	set.uRgbaO			= IRGBA(255,128, 64,  0);
	set.vMove			= TGL::Vec3::ZERO;
	EPRGZANBBD_T*	pEft = EPRGZANBBD_Req( &set, TGL::MtxGuide(&mMtx) );
	if( pEft )
		SLLINK_Next( pEft, &pEft->cmn.EmitLink, &mEft );	//!< リンク接続
	
	//!	SE
	TGL::SoundChannel*	pSE = IAppSound->play( SDID_ONMEM, SE_S0_METEOR, TGL::SoundChannel::PLAYF_LOOP );
	if( pSE ){
		pSE->setOwner( this );
		pSE->setField( TGL::MtxGuide(&mMtx), 1500.0f, 3, 1 );
		pSE->setVolume( 0.01f, 1.0f, 15 );
		pSE->setLoopKeep( 4 );
		mhSE = pSE->getUniqueId();
	}
}

// ---------------------------------------------
//! @brief  デストラクタ "Meteor"
// ---------------------------------------------
tBGPrologue::Meteor::~Meteor()
{
	EEMIT_End( &mEft );
	TGL::SoundChannel*	pSE = ITGLSound->getChFromUniqueId(mhSE);
	if( pSE )
		pSE->detach();
}

// ---------------------------------------------
//! @brief  破片 更新
//! @param  Bg	[i/o]	背景
// ---------------------------------------------
void tBGPrologue::Meteor::update()
{
	//!	前の座標
	TGL::Vec3	vOld = mMtx.getW();
	//!	推進
	TGL::Vec3	v0( mMtx.getZ() * mSpd * mpOwner->getDeltaTime() );
	mMtx.addW( v0 );
	
	mCol.getMtxGuideS().set( vOld );
	mCol.getMtxGuideE().set( mMtx.getW() );
	
	//!	音
	TGL::SoundChannel*	pSE = ITGLSound->getChFromUniqueId(mhSE);
	if( pSE )
		pSE->onLoopKeep();
	
	//!	当たり判定
	foreach( stl::vector<TGL::ColPlane>, mpOwner->mValleyCol, it ){
		TGL::ColBase::RET_T	ret = {0};
		if( (*it).check(mCol,&ret) ){
			//!	画面内ならエフェクト発生
			TGL::BaseCamera*	pCam = ICamera->getCamera(0);
			if( pCam && !pCam->isOutScreen(ret.mPos, 300.0f) ){
				IAppEffect->reqWay( EFTNO_SCENES0_METEORGROUND, ret.mPos, -ret.mWay );
				
				//!	カメラ振動
				if( mpOwner->getScene() >= STGPRSCENE_METEOR ){
					f32	fRate;
					f32	fDist = pCam->getPos().distance( ret.mPos );
			  		if( IACtrl->getScene().isDemo() && !IACtrl->getScene().tstDemoF(DEMOF_INPUTENABLE) ){	//!< 通常デモ中
						fDist = TGL::Math::Clamp<f32>( fDist, 5000.0f, 10000.0f );
						fRate = TGL::Math::Lerp<f32>( 1.0f, 0.5f, (fDist-5000.0f)/5000.0f );
					}else{
						fDist = TGL::Math::Clamp<f32>( fDist, 1000.0f, 2000.0f );
						fRate = TGL::Math::Lerp<f32>( 1.0f, 0.5f, (fDist-1000.0f)/1000.0f );
					}
			  		pCam->reqShake( 90, TGL::Vec3(50*fRate,50*fRate,0), TGL::TransValue::CURVE_SUB1 );
		  			ICamera->getMain()->reqShake( 90, TGL::Vec3(5*fRate,5*fRate,0), TGL::TransValue::CURVE_SUB1 );
			  	}
			}
			//!	SE
			TGL::SoundChannel*	pCh = IAppSound->play( SDID_ONMEM, SE_S0_METEORBOMB );
			if( pCh ){
				f32	fDist = 2000.0f;
		  		if( IACtrl->getScene().isDemo() && !IACtrl->getScene().tstDemoF(DEMOF_INPUTENABLE) )	//!< 通常デモ中
		  			fDist = 3000.0f;
	  			pCh->setVolume( 0.75f );
				pCh->setField( ret.mPos, fDist, 1, 1 );
			}
			
			if( pSE )
				pSE->detach();
			mbEnd = true;
			break;
		}
	}
}

// ---------------------------------------------
//! @brief  空 初期化
// ---------------------------------------------
void tBGPrologue::setupSky()
{
	//!	レンズフレア
	cBGModel*	pModel = getModel( "St00_Far_02.mdl" );
	if( pModel ){
		SAFE_DELETE( mpLensflare );
		mpLensflare = createLensFlare( TGL::MtxGuide(pModel->GetWorldMatrix(), TGL::Vec3( 5, 28231, 11090)), "St00_Flare", _PARAM_EX, "Lensflare_Sky" );
	}
	
	//!	ライト
	if( mSceneParam.getCurrentId() < STGPRSP_SKY )
		mSceneParam.setCurrentId( STGPRSP_SKY );

	//!	音
	TGL::SoundChannel*	pCh = IAppSound->play( SDID_STREAM, AMBSTR_WIND );
	if( pCh ){
		pCh->setVolume( 0.01f, 1.0f, 120 );
		pCh->setUserId( SNDOWNER_AMBIENT );
	}
}
// ---------------------------------------------
//! @brief  空 終了
// ---------------------------------------------
void tBGPrologue::finalSky()
{
	SAFE_DELETE( mpLensflare );
}

// ---------------------------------------------
//! @brief  宇宙 初期化
// ---------------------------------------------
void tBGPrologue::setupSpace()
{
	//!	レンズフレア
	cBGModel*	pModel = getModel( "St00_Far_04.mdl" );
	if( pModel ){
		SAFE_DELETE( mpLensflare );
		mpLensflare = createLensFlare( TGL::MtxGuide(pModel->GetWorldMatrix(), TGL::Vec3( 0, 8765, 36889)), "St00_Flare", _PARAM_EX, "Lensflare_Space" );
	}
	
	//!	ライト
	mSceneParam.setCurrentId( STGPRSP_SPACE );
}
// ---------------------------------------------
//! @brief  宇宙 終了
// ---------------------------------------------
void tBGPrologue::finalSpace()
{
	SAFE_DELETE( mpLensflare );
}

/* ========================================================================= */
/*		デバッグ															 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  デバッグ情報表示
// ---------------------------------------------
void tBGPrologue::dispDebugInfo()
{
#ifndef _TGL_FINAL
	if( !mbDebugInfo )	return;
	
	//!	親
	tBG::dispDebugInfo();

	TGL::BaseCamera*	pCam = ICamera->getCamera(0);
	if( !pCam )	return;
	
	//!	当たり判定
	foreach( stl::vector<TGL::ColPlane>, mValleyCol, it )
		(*it).dispDebug( IRGBA(255,255,255,255), TGL::Camera::CAMF_00 );
	
	//!	ロケータ表示
	stl::vector<TGL::Vec2*>	mPosBuf;	//!< 表示2D座標
	dispDebugInfoLocator( pCam, "St00_CityLoop.mdl", "St00_CityA_LOC_", &mPosBuf );
	dispDebugInfoLocator( pCam, "St00_CityLoop.mdl", "St00_CityB_LOC_", &mPosBuf );
	dispDebugInfoLocator( pCam, "St00_CityLoop.mdl", "St00_CityC_LOC_", &mPosBuf );
	foreach( stl::vector<TGL::Vec2*>, mPosBuf, it )
		SAFE_DELETE( *it );
	mPosBuf.clear();
	
#endif	/*	_TGL_FINAL	*/
}
