// -------------------------------------------------------------------------- //
//! @file   tBGStg05.cpp
//! @brief  背景 ５面
//! @author Nal
//! @since  13/08/30(金)
//! 
//! COPYRIGHT (C) 2013 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
/* ====< インクルード >====================================================== */
#include "prefix.h"
#include <TGL.h>
#include "Game/Game.h"
#include "Action/Camera/cBGCamera.h"
#include "tBGStg05.h"
#include "Action/ActionTypes.h"
#include "Effect/sAppEffect.h"
#include "Sound/sAppSound.h"
#include "../../../../../tgs/scene/Stage5/scn_Stage5.h"

static const char*	_PARAM_EX = "param/BG/Stage5/Stage5_paramEx.json";	//!< 固有パラメータリソース

#ifdef _WINDOWS
#pragma warning(disable : 4355)
#endif
/* ========================================================================= */
//!		tBGStg05															 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  コンストラクタ "tBGStg05"
// ---------------------------------------------
tBGStg05::tBGStg05()
:	tBG(),
	mCityFall_Uv(TGL::Vec2(0,0)), mSkyPillarCullDist(0), mpLensflare(NULL), mRoadLightOfsBgSpeed(120), mRoadLightOfsChrSpeed(60),
	mpDust(NULL), mDustColorMin(0), mDustColorMax(0),
	mpSEFire(NULL), mpSEQuake(NULL), mbWindVolDown(false)
{
	SLLINK_Init( &mEftDustLink );
	for( u8 i=0 ; i<numberof(mEftLink) ; i++ )
		SLLINK_Init( &mEftLink[i] );
	reset();
}

// ---------------------------------------------
//! @brief  デストラクタ "tBGStg05"
// ---------------------------------------------
tBGStg05::~tBGStg05()
{
	IAppResource->releaseTexPool( "models/BG/common/St05/St05_Flare01.dds" );
	IAppResource->releaseTexPool( "models/BG/common/St05/St05_Flare02.dds" );
	IAppResource->releaseTexPool( "models/BG/common/St05/St05_Flare03.dds" );
	IAppResource->releaseTexPool( "models/BG/common/St05/St05_Flare04.dds" );
	IAppResource->releaseTexPool( "models/BG/common/St05/St05_Post_ToneCurve_nmip.dds" );
	IAppResource->releaseTexPool( "models/BG/common/St05/St05_Post_ToneCurve_Hall_nmip.dds" );
	IAppResource->releaseTexPool( "models/BG/common/St05/St05_Post_ToneCurve_City_nmip.dds" );
	IAppResource->releaseTexPool( "models/BG/common/St05/St05_Post_ToneCurve_Sky_nmip.dds" );
	IAppResource->releaseTexPool( "models/BG/common/St05R/St05_Hall_envC.dds" );
	IAppResource->releaseTexPool( "models/BG/common/St05R/St05_Prologue_envC.dds" );
	IAppResource->releaseTexPool( "models/BG/common/St05R/St05_Shaft_envC.dds" );
	IAppResource->releaseTexPool( "models/BG/common/St05R/St05_Sky_envC.dds" );
	ITGLSound->stop( 0, TGL::Sound::STOPFS_ONLY_AMBIENT | TGL::Sound::STOPF_N_ONMEMORY );
	ITGLRenderer->getFilterToneCurve()->setTexCurve( NULL );
	ITGLRenderer->getFilterToneCurve()->setEnableValue( false );

	EEMIT_End( &mEftDustLink );
	SAFE_DELETE( mpDust );
	SAFE_DELETE( mpLensflare );
	for( u8 i=0 ; i<numberof(mEftLink) ; i++ )
		EEMIT_End( &mEftLink[i] );
	foreach( stl::vector<BGDoorStg05*>, mDoor, it )
		SAFE_DELETE( *it );
	foreach( stl::vector<DoorPiece*>, mDoorPiece, it )
		SAFE_DELETE( *it );
}

// ---------------------------------------------
//! @brief  原点位置の取得
//! @return 座標
// ---------------------------------------------
TGL::Vec3	tBGStg05::getOrigin() const
{
	if( mScene < S5SCENE_SKY )
		return TGL::Vec3(120000, -190000, 400000);
	else
		return TGL::Vec3::ZERO;
}

// ---------------------------------------------
//! @brief  リセット
// ---------------------------------------------
void tBGStg05::reset()
{
	tBG::reset();
	ITGLSound->stop( 0, TGL::Sound::STOPFS_ONLY_AMBIENT | TGL::Sound::STOPF_N_ONMEMORY );
	EEMIT_End( &mEftDustLink );
	SAFE_DELETE( mpDust );
	for( u8 i=0 ; i<numberof(mEftLink) ; i++ )
		EEMIT_End( &mEftLink[i] );
	mEftCloudMtx	= TGL::Matrix::UNIT;
	foreach( stl::vector<BGDoorStg05*>, mDoor, it )
		(*it)->reset();
	foreach( stl::vector<DoorPiece*>, mDoorPiece, it )
		SAFE_DELETE( *it );
}

// ---------------------------------------------
//! @brief  リソースロード
//! @param  uBG	[in]	BGID_????
//! @return true:成功, false:失敗
// ---------------------------------------------
bool tBGStg05::load( u8 uBG )
{
	//!		モデル		
	cBGModel*	pModel;
	const u32	_SCENE_HALL		= (1<<S5SCENE_HALL) | (1<<S5SCENE_ROAD_DOORBREAK);	//!< 縦穴
	const u32	_SCENE_ROAD		= _SCENE_HALL | (1<<S5SCENE_ROAD_DOORBREAK) | (1<<S5SCENE_ROAD_SIDE);	//!< 通路
	const u32	_SCENE_3AXIS	= (1<<S5SCENE_ROAD_SIDE) | (1<<S5SCENE_ROAD_3AXIS) | (1<<S5SCENE_ROAD_END);	//!< ３軸通路
	const u32	_SCENE_VALLEY	= (1<<S5SCENE_ROAD_END) | (1<<S5SCENE_VALLEY);	//!< 峡谷
	const u32	_SCENE_CITY		= (1<<S5SCENE_CITY);	//!< 都市
	const u32	_SCENE_SKY		= (1<<S5SCENE_SKY);		//!< 空
	//!	
//	pModel = addModel( "St05_Far_01.mdl", cBGModel::TYPE_SKYDOME, cBGModel::FLAG_FAROBJ );
//	if( pModel )	pModel->setOnScene( _SCENE_HALL );
	//!	天球 都市
	pModel = addModel( "St05_Far_02.mdl", cBGModel::TYPE_SKYDOME_Y, cBGModel::FLAG_FAROBJ );
	if( pModel )	pModel->setOnScene( _SCENE_VALLEY | _SCENE_CITY );
	//!	天球 雲海
	pModel = addModel( "St05_Far_03.mdl", cBGModel::TYPE_SKYDOME_Y, cBGModel::FLAG_FAROBJ | cBGModel::FLAG_UPDATE_HIERARCHY );
	if( pModel )	pModel->setOnScene( _SCENE_SKY );
	//!	縦穴
	pModel = addModel( "St05_4Boss.mdl" );
	if( pModel )	pModel->setOnScene( _SCENE_HALL );
	//!	縦穴 ボスユニット
	pModel = addModel( "St05_4Boss_Unit.mdl" );
	if( pModel )	pModel->setOnScene( _SCENE_HALL );
	//!	通路
	pModel = addModel( "St05_RoadLoop_A.mdl" );
	if( pModel )	pModel->setOnScene( _SCENE_ROAD );
	//!	3軸通路
	pModel = addModel( "St05_Road3axis.mdl" );
	if( pModel )	pModel->setOnScene( _SCENE_3AXIS );
	//!	峡谷
	pModel = addModel( "St05_ValleyLoop.mdl" );
	if( pModel )	pModel->setOnScene( _SCENE_VALLEY | _SCENE_CITY );
	//!	都市
	pModel = addModel( "St05_CityLoop.mdl" );
	if( pModel )	pModel->setOnScene( _SCENE_CITY );
	//!	空
	pModel = addModel( "St05_Sky.mdl" );
	if( pModel )	pModel->setOnScene( _SCENE_SKY );
	//!	空 柱
	pModel = addModel( "Obj_St05_Pillar.mdl" );
	if( pModel )	pModel->setOnScene( _SCENE_SKY );
	//!	ドア
	pModel = addModel( "Obj_St05_Door.mdl" );
	if( pModel ){
		//!	モーション
		TGL::AnimationController*	pAnim = APP_NEW TGL::AnimationController( pModel );
		pModel->setAnim( pAnim );
		pModel->addMotion( 0, "Obj_St05_Door.anm" );
		pModel->addMotion( 1, "Obj_St05_Door_Open.anm" );
		pModel->getAnim()->Play( 0, true, true );
	}
	//!	ダメージドア
	pModel = addModel( "Obj_St05_Door_Damage.mdl" );

	//!		テクスチャ		
	IAppResource->loadTexPool( "models/BG/common/St05/St05_Flare01.dds" );
	IAppResource->loadTexPool( "models/BG/common/St05/St05_Flare02.dds" );
	IAppResource->loadTexPool( "models/BG/common/St05/St05_Flare03.dds" );
	IAppResource->loadTexPool( "models/BG/common/St05/St05_Flare04.dds" );
	IAppResource->loadTexPool( "models/BG/common/St05/St05_Post_ToneCurve_nmip.dds" );
	IAppResource->loadTexPool( "models/BG/common/St05/St05_Post_ToneCurve_Hall_nmip.dds" );
	IAppResource->loadTexPool( "models/BG/common/St05/St05_Post_ToneCurve_City_nmip.dds" );
	IAppResource->loadTexPool( "models/BG/common/St05/St05_Post_ToneCurve_Sky_nmip.dds" );
	IAppResource->loadTexPool( "models/BG/common/St05R/St05_Hall_envC.dds" );
	IAppResource->loadTexPool( "models/BG/common/St05R/St05_Prologue_envC.dds" );
	IAppResource->loadTexPool( "models/BG/common/St05R/St05_Shaft_envC.dds" );
	IAppResource->loadTexPool( "models/BG/common/St05R/St05_Sky_envC.dds" );

	//!		シーンパラメータ		
	mSceneParam.getFromJson( "param/BG/Stage5/Stage5_param.json" );
	{	//!	固有パラメータ
		TGL::JsonUtil	json( _PARAM_EX );
		{
			TGL::Json::array	jsonArray = json.getArray("-");
			foreach( TGL::Json::array, jsonArray, it ){
				TGL::Json::object&	work = it->get<TGL::Json::object>();
				
				mDustColorMin		= RGBA32_Set( (f32)atof( work["DustColor_Min_R"].to_str().c_str() ),
												  (f32)atof( work["DustColor_Min_G"].to_str().c_str() ),
												  (f32)atof( work["DustColor_Min_B"].to_str().c_str() ),
												  (f32)atof( work["DustColor_Min_A"].to_str().c_str() ) );
				mDustColorMax		= RGBA32_Set( (f32)atof( work["DustColor_Max_R"].to_str().c_str() ),
												  (f32)atof( work["DustColor_Max_G"].to_str().c_str() ),
												  (f32)atof( work["DustColor_Max_B"].to_str().c_str() ),
												  (f32)atof( work["DustColor_Max_A"].to_str().c_str() ) );
				
				mCityFall_Uv		= TGL::Vec2( (f32)atof( work["CityWaterFall_Speed_U"].to_str().c_str() ),
											(f32)atof( work["CityWaterFall_Speed_V"].to_str().c_str() ) );

				mRoadLightOfsBgSpeed	= atoi( work["RoadLight_Bg_Speed"].to_str().c_str() );
				mRoadLightOfsChrSpeed	= atoi( work["RoadLight_Chr_Speed"].to_str().c_str() );

				mSkyPillarCullDist	= (f32)atof( work["SkyPillar_CullingDist"].to_str().c_str() );
			}
		}
	}
//	mSceneParam.erasePointLight();
	//!	背景色とフォグ色の同期
	mSceneParam.setClearSyncFog( true );
	
	//!		パラメータ		
	//!	モデル配置
	applyModelSetFromJson( "param/BG/Stage5/Stage5_modelset.json" );

	//!	空 柱 ロケータ配置列挙
	for( u8 i=1 ;; i++ ){
		//!	ロケーター名
		const char*	_LOCATOR = { "Pillar_Loc_" };
		//!	モデル
		const char*	_OBJ = { "Obj_St05_Pillar.mdl" };
		stl::string	strLoc = stl::FormatString<stl::string>( "%s%02d", _LOCATOR, i );
		cModelSet*	pSet = addModelSetAssign( _OBJ, "St05_Sky.mdl", strLoc, true );
		if( !pSet )
			break;
		pSet->setId( i );
		pSet->setOnScene( _SCENE_SKY );
		pSet->setCullingDist( mSkyPillarCullDist );
	}

	//!	カメラパス追加
	applyCameraPathSetFromJson( "param/BG/Stage5/Stage5_camera.json" );

	//!	UVアニメ設定
	setModelUvAnim();

	//!	トーンカーブ
	ITGLRenderer->getFilterToneCurve()->setEnableValue( true );
	ITGLRenderer->getFilterToneCurve()->setTexCurve( IAppResource->getTexture("St05_Post_ToneCurve_nmip") );

	//!	シーン環境マップ
	mSceneParam.setEnvTex( "St05_Hall_envC.dds" );
	ICharacter->overwriteBGEnvTex();	//!< キャラクターへの適用
	overwriteEnvTex();	//!< 背景モデルへの適用

	//!	ドア
	mDoor.push_back( APP_NEW BGDoorStg05(getModelSet("Obj_St05_Door.mdl",0), S5SCENE_HALL, S5SCENE_ROAD_DOORBREAK,	0, 480, -180) );	//!< 通路
	mDoor.push_back( APP_NEW BGDoorStg05(getModelSet("Obj_St05_Door.mdl",1), S5SCENE_ROAD_END, S5SCENE_VALLEY,	3, 460, 180) );	//!< 通路

	//!	シーン遷移
	{//!	縦穴～通路破壊	
		BGSceneChange::cSet	set;
		//!	シーン
		set.addScene( BGSceneChange::cSceneTable(  750, S5SCENE_HALL) );
		set.addScene( BGSceneChange::cSceneTable(    0, S5SCENE_ROAD_DOORBREAK) );
		//!	シーンパラメータ
		set.addParam( BGSceneChange::cParamTable(    0,   0, S5SP_HALL_BITROUND) );	//!< ビット周回
		set.addParam( BGSceneChange::cParamTable(  120,   0, S5SP_HALL_BITSHOT) );	//!< ビット射撃
		set.addParam( BGSceneChange::cParamTable(  360, 120, S5SP_HALL) );			//!< 縦穴
		set.addParam( BGSceneChange::cParamTable(  750, 120, S5SP_HALL_DOOR) );		//!< 縦穴～ドア
		set.addParam( BGSceneChange::cParamTable(    0,   0, S5SP_ROAD_DARK) );		//!< 通路暗転
		mSceneChange.getSet()[0] = set;
	}
	{//!	通路横	
		BGSceneChange::cSet	set;
		//!	シーン
		set.addScene( BGSceneChange::cSceneTable(   60, S5SCENE_ROAD_DOORBREAK) );
		set.addScene( BGSceneChange::cSceneTable(    0, S5SCENE_ROAD_SIDE) );
		//!	シーンパラメータ
		set.addParam( BGSceneChange::cParamTable(    0, 120, S5SP_ROAD_RED) );	//!< 通路
		mSceneChange.getSet()[1] = set;
	}
	{//!	３軸通路	
		BGSceneChange::cSet	set;
		//!	シーン
		set.addScene( BGSceneChange::cSceneTable(  480, S5SCENE_ROAD_SIDE) );
		set.addScene( BGSceneChange::cSceneTable(    0, S5SCENE_ROAD_3AXIS) );
		//!	シーンパラメータ
		set.addParam( BGSceneChange::cParamTable(  120,  90, S5SP_ROAD_RED_BACK) );		//!< 通路 後方エスト
		set.addParam( BGSceneChange::cParamTable(  360, 240, S5SP_ROAD_LASERCHARGE) );	//!< 通路レーザー溜め
		set.addParam( BGSceneChange::cParamTable(  560,  60, S5SP_ROAD_RED_FRONT) );	//!< 通路 正面
		set.addParam( BGSceneChange::cParamTable(  700,  40, S5SP_ROAD_LASER) );		//!< 通路レーザー
		set.addParam( BGSceneChange::cParamTable(    0,  60, S5SP_ROAD_RED_FRONT2) );	//!< 通路 正面
		mSceneChange.getSet()[2] = set;
	}
	{//!	峡谷	
		BGSceneChange::cSet	set;
		//!	シーン
		set.addScene( BGSceneChange::cSceneTable(  460, S5SCENE_ROAD_3AXIS) );
		set.addScene( BGSceneChange::cSceneTable(  700, S5SCENE_ROAD_END) );
		set.addScene( BGSceneChange::cSceneTable( 3060, S5SCENE_VALLEY) );
		set.addScene( BGSceneChange::cSceneTable(    0, S5SCENE_CITY) );
		//!	シーンパラメータ
		set.addParam( BGSceneChange::cParamTable(  460,   0, S5SP_ROAD_RED_FRONT2) );	//!< 通路 正面
		set.addParam( BGSceneChange::cParamTable(  700, 160, S5SP_ROAD_EXIT) );	//!< 通路出口
		set.addParam( BGSceneChange::cParamTable( 3060, 120, S5SP_VALLEY) );	//!< 峡谷
		set.addParam( BGSceneChange::cParamTable(    0, 360, S5SP_CITY) );		//!< 都市
		mSceneChange.getSet()[3] = set;
	}
	{//!	都市	
		BGSceneChange::cSet	set;
		//!	シーン
		set.addScene( BGSceneChange::cSceneTable(    0, S5SCENE_CITY) );
		//!	シーンパラメータ
		set.addParam( BGSceneChange::cParamTable(    0, 0, S5SP_CITY) );		//!< 都市
		mSceneChange.getSet()[4] = set;
		mSceneChange.getSet()[5] = set;
	}
	{//!	空 縦		
		BGSceneChange::cSet	set;
		//!	シーン
		set.addScene( BGSceneChange::cSceneTable(    0, S5SCENE_SKY) );
		//!	シーンパラメータ
		set.addParam( BGSceneChange::cParamTable(    0, 0, S5SP_SKY_VERTICAL) );
		mSceneChange.getSet()[6] = set;
	}
	{//!	空 移動		
		BGSceneChange::cSet	set;
		//!	シーン
		set.addScene( BGSceneChange::cSceneTable(    0, S5SCENE_SKY) );
		//!	シーンパラメータ
		set.addParam( BGSceneChange::cParamTable(  120,   0, S5SP_SKY_VERTICAL) );
		set.addParam( BGSceneChange::cParamTable(    0, 120, S5SP_SKY_MOVE) );
		mSceneChange.getSet()[7] = set;
	}
	{//!	空 横		
		BGSceneChange::cSet	set;
		//!	シーン
		set.addScene( BGSceneChange::cSceneTable(    0, S5SCENE_SKY) );
		//!	シーンパラメータ
		set.addParam( BGSceneChange::cParamTable( 2800,   0, S5SP_SKY_SIDE) );
		set.addParam( BGSceneChange::cParamTable(    0, 120, S5SP_SKY_FRONT) );
		mSceneChange.getSet()[8] = set;
	}
	{//!	空 ボス		
		BGSceneChange::cSet	set;
		//!	シーン
		set.addScene( BGSceneChange::cSceneTable(    0, S5SCENE_SKY) );
		//!	シーンパラメータ
		set.addParam( BGSceneChange::cParamTable(  240,   0, S5SP_SKY_FRONT) );
		set.addParam( BGSceneChange::cParamTable(    0, 240, S5SP_BOSS_FRONT) );
		mSceneChange.getSet()[9] = set;
	}

	//!	更新関数登録
	addUpdateEvent( updateDustParCB,	0, -1, setupDustParCB, finalDustParCB );	//!< 塵パーティクル
	addUpdateEvent( NULL,				0, -1, setupDustCB, finalDustCB );	//!< 塵
	addUpdateEvent( NULL,				S5SCENE_HALL, 			S5SCENE_HALL,		setup4BossCB, final4BossCB );	//!< ４ボスエリア
	addUpdateEvent( updateRoadLightCB,	S5SCENE_ROAD_DOORBREAK, S5SCENE_ROAD_END,	setupRoadLightCB );	//!< 通路光源
	addUpdateEvent( updateRoadExitCB,	S5SCENE_ROAD_END,		S5SCENE_ROAD_END,	setupRoadExitCB, finalRoadExitCB );	//!< 通路出口
	addUpdateEvent( updateSkyCB,		S5SCENE_SKY,			S5SCENE_SKY,		setupSkyCB, finalSkyCB );	//!< 空

	return true;
}

// ---------------------------------------------
//! @brief  塵パーティクル 初期化
// ---------------------------------------------
void tBGStg05::setupDustPar()
{
	//!	パーティクル
	if( !ITGLSystem->isGraphicsLevelLow() && !ITGLSystem->isPostFilterLevelLow() ){
		TGL::MtxGuide	guide( &mEftDustMtx );
		IAppEffect->req( EFTNO_SCENES5_DUST, TGL::Vec3::ZERO, TGL::Vec3::ZERO, NULL, &guide, NULL, NULL, 0, 0, &mEftDustLink );
	}
}
// ---------------------------------------------
//! @brief  塵パーティクル 終了
// ---------------------------------------------
void tBGStg05::finalDustPar()
{
	EEMIT_End( &mEftDustLink );
}
// ---------------------------------------------
//! @brief  塵パーティクル
//! @param  bSceneChg	[in]	true:シーン変更瞬間
// ---------------------------------------------
void tBGStg05::updateDustPar( bool bSceneChg )
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
		mEftDustMtx.setY( m0.getY() );

		//!	Z : カメラ移動逆方向
		v0 = v0.normalize();
		mEftDustMtx.setZ( v0 );
		
		//!	X : ZY垂線
		v0 = mEftDustMtx.getZ().cross( mEftDustMtx.getY() );
		mEftDustMtx.setX( v0 );
	}
	
	//!	座標 : カメラ位置から、カメラ移動量を足す
	v0 = pCam->getPos();
	v0 += pCam->getMove() * 120.0f;
	v0 += pCam->getEyeVector() * 3000.0f;
	mEftDustMtx.setW( v0 );
	
#if 0
	IPrim->add( TGL::PrimCross(mEftDustMtx.getW(), 1000.f, IRGBA(255,255,0,255)), 0, TGL::Camera::CAMF_00 );
	
	TGL::Matrix	m0 = mEftDustMtx;
	m0.setW( pCam->getTar() );
	IPrim->add( TGL::PrimPivot(m0, 1000.0f), 0, TGL::Camera::CAMF_00 );
#endif
}

// ---------------------------------------------
//! @brief  塵 初期化
// ---------------------------------------------
void tBGStg05::setupDust()
{
	if( mpDust )	return;
	if( !ITGLSystem->isGraphicsLevelLow() && !ITGLSystem->isPostFilterLevelLow() ){
		mpDust = APP_NEW tDust( this, GPRI_EFTPRE(0) );
		mpDust->setColorMin( mDustColorMin );
		mpDust->setColorMax( mDustColorMax );
		mpDust->setFogColor( IRGBA(0,0,0,255) );
		mpDust->setFogNear(	500 );
		mpDust->setFogFar(	3000 );
		mpDust->setSpd( 0.3f );
	}
}
// ---------------------------------------------
//! @brief  塵 終了
// ---------------------------------------------
void tBGStg05::finalDust()
{
	SAFE_DELETE( mpDust );
}

// ---------------------------------------------
//! @brief  UVアニメ設定
// ---------------------------------------------
void tBGStg05::setModelUvAnim()
{
	cBGModel*	pModel;
	
	//!	市街地
	pModel = getModel( "St05_CityLoop.mdl" );
	if( pModel )	pModel->SetNodeUVAnime( "City_Fall_alpha", mCityFall_Uv );
}

// ---------------------------------------------
//! @brief  エフェクト開始
// ---------------------------------------------
void tBGStg05::beginEffect()
{
}

// ---------------------------------------------
//! @brief  ４ボスエリア 初期化
// ---------------------------------------------
void tBGStg05::setup4Boss()
{
	//!	パーティクル
	const TGL::Vec3	_POS( 120000, -199000, 430000 );
	IAppEffect->req( EFTNO_SCENES5_SMOKEZONE,  _POS, TGL::Vec3::ZERO, NULL, NULL, NULL, NULL, 0, 0, &mEftLink[EFTLINK_SMOKE] );
	IAppEffect->req( EFTNO_SCENES5_SMOKEZONE2, _POS, TGL::Vec3::ZERO, NULL, NULL, NULL, NULL, 0, 0, &mEftLink[EFTLINK_SMOKE2] );
	//!	SE
	mpSEFire = IAppSound->play( SDID_ONMEM, SE_S5_FIRE_LOOP, TGL::SoundChannel::PLAYF_LOOP );
	if( mpSEFire ){
		mpSEFire->setField( _POS, 10000.0f, 0.3f, 1 );
		mpSEFire->setVolume( 0.01f, 1.0f, 15 );
		mpSEFire->setLoopKeep( 60 );
		mLoopKeepSE.push_back( mpSEFire );
	}
	mpSEQuake = IAppSound->play( SDID_STREAM, AMBSTR_QUAKE );
	if( mpSEQuake ){
		mpSEQuake->setVolume( 0.5f );
		mpSEQuake->setSPan( 1.0f );
	}
}
// ---------------------------------------------
//! @brief  ４ボスエリア 終了
// ---------------------------------------------
void tBGStg05::final4Boss()
{
	EEMIT_End( &mEftLink[EFTLINK_SMOKE] );
	EEMIT_End( &mEftLink[EFTLINK_SMOKE2] );
	eraseLoopKeepSE( mpSEFire );
	mpSEFire = NULL;
	if( mpSEQuake ){
		mpSEQuake->stop( 120 );
		mpSEQuake = NULL;
	}
}

// ---------------------------------------------
//! @brief  通路光源 初期化
// ---------------------------------------------
void tBGStg05::setupRoadLight()
{
	mRoadLightOfsBg.init(  TGL::TransValue::MODE_REPEAT, mRoadLightOfsBgSpeed, TGL::TransValue::CURVE_LINEAR, 999, 0.0f, -5000.0f );
	mRoadLightOfsChr.init( TGL::TransValue::MODE_REPEAT, mRoadLightOfsChrSpeed, TGL::TransValue::CURVE_LINEAR, 999, 2000.0f, -2000.0f );
}
// ---------------------------------------------
//! @brief  通路光源
//! @param  bSceneChg	[in]	true:シーン変更瞬間
// ---------------------------------------------
void tBGStg05::updateRoadLight( bool bSceneChg )
{
	mRoadLightOfsBg.update( getDeltaTime() );
	mRoadLightOfsChr.update( getDeltaTime() );
	
	//!	背景
	for( u8 i=S5PL_ROADZ_BG_A ; i<=S5PL_ROADZ_BG_B ; i++ ){
		cSceneParam::LightPoint*	pLight = mSceneParam.getPointLight(i);
		if( pLight )
			pLight->mPosOfs.z = mRoadLightOfsBg.getValue();
	}
	//!	背景
	for( u8 i=S5PL_ROADX_BG_A ; i<=S5PL_ROADX_BG_B ; i++ ){
		cSceneParam::LightPoint*	pLight = mSceneParam.getPointLight(i);
		if( pLight )
			pLight->mPosOfs.x = -mRoadLightOfsBg.getValue();
	}
	//!	キャラ
	cSceneParam::LightPoint*	pLight = mSceneParam.getPointLight(S5PL_ROADZ_CHR);
	if( pLight ){
		TGL::Vec3	vOfs = ICamera->getMain()->getEyeVector() * -250.0f;
		vOfs += ICamera->getMain()->getRightVector() * mRoadLightOfsChr.getValue();
		
		pLight->mPosOfs = vOfs;
	}
}

// ---------------------------------------------
//! @brief  通路出口 初期化
// ---------------------------------------------
void tBGStg05::setupRoadExit()
{
	//!	レンズフレア
	cModelSet*	pSet = mDoor[DOOR_EXIT]->getModelSet();
	if( pSet ){
		SAFE_DELETE( mpLensflare );
		mpLensflare = createLensFlare( TGL::MtxGuide(pSet->getPos()+getOrigin()), "St05_Flare", _PARAM_EX, "Lensflare_RoadExit" );
	}
}
// ---------------------------------------------
//! @brief  通路出口 終了
// ---------------------------------------------
void tBGStg05::finalRoadExit()
{
	SAFE_DELETE( mpLensflare );
}
// ---------------------------------------------
//! @brief  通路出口
//! @param  bSceneChg	[in]	true:シーン変更瞬間
// ---------------------------------------------
void tBGStg05::updateRoadExit( bool bSceneChg )
{
	if( mpLensflare ){
		f32	fDistance = ICamera->getCamera(0)->getPos().distance( mpLensflare->getAttach().getPos() );
		fDistance = TGL::Math::Clamp<f32>( fDistance/6000.0f, 0.0f, 1.0f );
		f32	fRate = 1.0f - fDistance;
		
		mpLensflare->setStreakSize( TGL::Math::Lerp<f32>(0.0f, 2048.0f, fRate) );
		mpLensflare->applyParam();
		
		fRate = 1.0f - pow( fDistance, 4.0f );
		mpLensflare->setAlpha( fRate );
	}
}

// ---------------------------------------------
//! @brief  空 初期化
// ---------------------------------------------
void tBGStg05::setupSky()
{
	//!	レンズフレア
	cBGModel*	pModel = getModel( "St05_Far_03.mdl" );
	if( pModel ){
		SAFE_DELETE( mpLensflare );
		mpLensflare = createLensFlare( TGL::MtxGuide(pModel, "St05_Far_03_Sun_Loc"), "St05_Flare", _PARAM_EX, "Lensflare_Sky" );
	}
	//!	パーティクル
	if( !ITGLSystem->isGraphicsLevelLow() && !ITGLSystem->isPostFilterLevelLow() ){
		TGL::MtxGuide	guide( &mEftCloudMtx );
		IAppEffect->req( EFTNO_SCENES5_SKY_CLOUDFOG, TGL::Vec3::ZERO, TGL::Vec3::ZERO, NULL, &guide, NULL, NULL, 0, 0, &mEftLink[EFTLINK_CLOUD] );
	}
	//!	SE
	TGL::SoundChannel*	pCh = IAppSound->play( SDID_STREAM, AMBSTR_WIND );
	if( pCh ){
		pCh->setVolume( 0.01f, mbWindVolDown ? 0.05f : 1.0f, 120 );
		pCh->setUserId( SNDOWNER_AMBIENT );
	}
}
// ---------------------------------------------
//! @brief  空 終了
// ---------------------------------------------
void tBGStg05::finalSky()
{
	SAFE_DELETE( mpLensflare );
	EEMIT_End( &mEftLink[EFTLINK_CLOUD] );
}
// ---------------------------------------------
//! @brief  空
//! @param  bSceneChg	[in]	true:シーン変更瞬間
// ---------------------------------------------
void tBGStg05::updateSky( bool bSceneChg )
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
}

// ---------------------------------------------
//! @brief  更新
// ---------------------------------------------
void tBGStg05::update()
{
	//!	トーンカーブ
	if( mScene != mSceneOld ){
		if( mScene >= S5SCENE_SKY )
			ITGLRenderer->getFilterToneCurve()->setTexCurve( IAppResource->getTexture("St05_Post_ToneCurve_Sky_nmip") );
		else if( mScene >= S5SCENE_VALLEY )
			ITGLRenderer->getFilterToneCurve()->setTexCurve( IAppResource->getTexture("St05_Post_ToneCurve_City_nmip") );
		else if( mScene >= S5SCENE_ROAD_DOORBREAK )
			ITGLRenderer->getFilterToneCurve()->setTexCurve( IAppResource->getTexture("St05_Post_ToneCurve_nmip") );
		else
			ITGLRenderer->getFilterToneCurve()->setTexCurve( IAppResource->getTexture("St05_Post_ToneCurve_Hall_nmip") );
	}

	//!	親クラス
	tBG::update();

	//!	シーン遷移
	mSceneChange.update( *this );

	//!	ドア
	foreach( stl::vector<BGDoorStg05*>, mDoor, it )
		(*it)->update( *this );

	//!	ドア破片
	for( stl::vector<DoorPiece*>::iterator it=mDoorPiece.begin() ; it!=mDoorPiece.end() ; ){
		if( (*it)->mbEnd ){
			SAFE_DELETE( *it );
			it = mDoorPiece.erase( it );
		}else{
			(*it)->update();
			++it;
		}
	}
}

// ---------------------------------------------
//! @brief  カメラ更新後の処理
// ---------------------------------------------
void tBGStg05::updateCameraAfter()
{
	tBG::updateCameraAfter();
}

// ---------------------------------------------
//! @brief  スクリプトメッセージ受け取り
//! @param  nId		[in]	メッセージID
//! @param  nParam	[in]	パラメータ
// ---------------------------------------------
void tBGStg05::receiveMessage( s32 nId, s32 nParam )
{
	switch( nId ){
		case STGPRMSG_SMOKE2END:	//!	煙２終了
			EEMIT_End( &mEftLink[EFTLINK_SMOKE2] );
			break;

		case STGPRMSG_FIRESEEND:	//!	火事音終了
			eraseLoopKeepSE( mpSEFire );
			mpSEFire = NULL;
			if( mpSEQuake ){
				mpSEQuake->stop( 60 );
				mpSEQuake = NULL;
			}
			break;
			
		case STGPRMSG_DOORPIECE:	//!	ドア破片生成
		{
			cModelSet*	pModel = getModelSet( "Obj_St05_Door_Damage.mdl" );
			if( !pModel )	break;
			
			TGL::Matrix	mtx = pModel->getWorldMtx();
			mtx.addW( getOrigin() );
			
			{	//!	１枚だけ固定
				TGL::Vec3	v0( 0, 0, 0 );
				v0 = v0.apply( mtx );
				mDoorPiece.push_back( APP_NEW DoorPiece(this, "Obj_St05_Door_Piece.mdl", v0, TGL::Vec3(0, 0, 80.0f)) );
			}
			
			for( u8 i=0 ; i<4 ; i++ ){
				f32	fDeg = -60.0f + (270.0f/4.0f*i);
				TGL::Vec3	v0( 100.0f*SIN(fDeg), 100.0f*COS(fDeg), 0 );
				v0.x += 150.0f;
				v0 = v0.apply( mtx );

				mDoorPiece.push_back( APP_NEW DoorPiece(this, "Obj_St05_Door_Piece.mdl", v0) );
			}
		}	break;

		case STGPRMSG_WINDVOLDOWN:	//!	風音量下げ
			mbWindVolDown = true;
			foreach( stl::vector<TGL::SoundChannel*>, ITGLSound->getChannel(), it ){
				if( (*it)->getUserId() == SNDOWNER_AMBIENT )
					(*it)->setVolume( 0.05f, nParam );
			}
			break;
	}
}

// ---------------------------------------------
//! @brief  コンストラクタ "DoorPiece"
//! @param  strModel	[in]	モデル名
//! @param  vPos		[in]	座標
//! @param  fSpeed		[in]	速度
// ---------------------------------------------
tBGStg05::DoorPiece::DoorPiece( tBG* pBg, const stl::string& strModel, const TGL::Vec3& vPos, const TGL::Vec3& vMove/*=TGL::Vec3::ZERO*/ )
:	mpOwner(pBg), mbEnd(false), mTime(0),
	mPos(vPos),
	mRot( TGL::Vec3(RANDF2(0,360), RANDF2(0,360), RANDF2(0,360)) ),
	mMove( TGL::Vec3(RANDF2(0,3), RANDF2(-3,3), RANDF2(45,65)) ),
	mRoll( TGL::Vec3(RANDF2(-5,5), RANDF2(-5,5), RANDF2(-5,5)) )
{
	mpModel = APP_NEW TGL::Model( IResourceManager->getResourceBinary<TGL::ModelResource>(strModel, TGL::Resource::TYPE_MODEL)->Get() );
	mpModel->SetScale( 0.8f );
	//!	座標
	if( !vMove.isZero() )
		mMove = vMove;

	//!	音
	mpSE = IAppSound->play( SDID_ONMEM, SE_S5_DOORPIECE_FLY, TGL::SoundChannel::PLAYF_LOOP | TGL::SoundChannel::PLAYF_RANDSTART | TGL::SoundChannel::PLAYF_NOHISTORY );
	if( mpSE ){
		mpSE->setPitch( 0.5f );
		mpSE->setField( TGL::MtxGuide(mpModel->GetWorldMatrix()), 300.0f, 5.0f, 1 );
		mpSE->setLoopKeep();
		mpOwner->addLoopKeepSE( mpSE );
	}
}

// ---------------------------------------------
//! @brief  デストラクタ "DoorPiece"
// ---------------------------------------------
tBGStg05::DoorPiece::~DoorPiece()
{
	SAFE_DELETE( mpModel );
	mpOwner->eraseLoopKeepSE( mpSE );
}

// ---------------------------------------------
//! @brief  更新
// ---------------------------------------------
void tBGStg05::DoorPiece::update()
{
	mPos += mMove * mpOwner->getDeltaTime();
	mRot += mRoll * mpOwner->getDeltaTime();
	if( mTime > 90.0f ){
		mMove.z = TGL::Math::Clamp<f32>( mMove.z, -0.2f, 0.0f, 100.0f );
		mMove.y -= 0.20f;
	}
	//!	簡易衝突判定
	cModelSet*	pModel = mpOwner->getModelSet( "Obj_St05_Door_Damage.mdl" );
	if( pModel ){
		TGL::Vec3	v0 = mPos;
		TGL::Vec3	v1 = pModel->getWorldMtx().getW() + mpOwner->getOrigin();
		v0.z	= v1.z = 0;
		TGL::Vec3	vWay = v0 - v1;
		
		const f32	_RADIUS = 300.0f;
		if( vWay.length() > _RADIUS ){
			if( mMove.length() >= 1.0f ){
				TGL::Vec3	v0;
				vWay = vWay.normalize();
				v0 = v1 + vWay * 450.0f;
				v0.z = mPos.z;
				IAppEffect->reqWay( EFTNO_SCENES5_DOORPIECE_HIT, v0, -vWay+TGL::Vec3(0, 0, 0.5f) );
				
				TGL::SoundChannel*	pCh = IAppSound->play( SDID_ONMEM, SE_S5_DOORPIECE_HIT );
				if( pCh ){
					pCh->setField( v0, 2000.0f, 1.0f, 1 );
					pCh->setPitch( RANDF2(0.9f,1.1f) );
				}
			}else{
				mRoll *= 0.5f;
				if( mpSE ){
					mpOwner->eraseLoopKeepSE( mpSE );
					mpSE = NULL;
				}
			}
			mMove.x *= -0.5f;
			mMove.y *= -0.5f;
			mMove.z *= 0.5f;
		}
		mPos.x = TGL::Math::Clamp<f32>( mPos.x, v1.x-_RADIUS, v1.x+_RADIUS );
		mPos.y = TGL::Math::Clamp<f32>( mPos.y, v1.y-_RADIUS, v1.y+_RADIUS );
	}
	
	//!	モデル更新
	TGL::Matrix	mtxWorld = TGL::Matrix::UNIT;
	mtxWorld.rollYPR( mRot );	//!< 回転
	mtxWorld.setW( mPos );		//!< 座標
	
	mpModel->Update( mtxWorld );
	
	//!	描画リスト登録
	if( !IACtrl->getScene().tstSceneF(SCENEF_NORENDER) ){
		ITGLRenderer->addDrawList( TGL::Renderer::DPASS_MAIN, GPRI_BG(3), mpOwner->getTskHead().mTot, TGL::Camera::CAMF_00, drawlistExec, (void*)this );
	}
	
	//!	終了
	mTime += mpOwner->getDeltaTime();
	if( mTime > 300.0f ){
		if( ICamera->getCamera(0)->isOutScreen(mPos, 300.0f) )
			mbEnd = true;
	}
}

/* ========================================================================= */
/*		デバッグ															 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  デバッグ情報表示
// ---------------------------------------------
void tBGStg05::dispDebugInfo()
{
#ifndef _TGL_FINAL
	if( !mbDebugInfo )	return;
	
	//!	親
	tBG::dispDebugInfo();

	TGL::BaseCamera*	pCam = ICamera->getCamera(0);
	if( !pCam )	return;
	
	//!	ロケータ表示
	stl::vector<TGL::Vec2*>	mPosBuf;	//!< 表示2D座標
	dispDebugInfoLocator( pCam, "St05_Sky.mdl", "Pillar_Loc_" );
	
#endif	/*	_TGL_FINAL	*/
}
