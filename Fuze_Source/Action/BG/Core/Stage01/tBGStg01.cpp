// -------------------------------------------------------------------------- //
//! @file   tBGStg01.cpp
//! @brief  背景 １面
//! @author Nal
//! @since  12/01/16(月)
//! 
//! COPYRIGHT (C) 2012 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
/* ====< インクルード >====================================================== */
#include "prefix.h"
#include <TGL.h>
#include "Game/Game.h"
#include "Action/Camera/cBGCamera.h"
#include "tBGStg01.h"
#include "Action/ActionTypes.h"
#include "Effect/sAppEffect.h"
#include "Sound/sAppSound.h"
#include "../../../../../tgs/scene/Stage1/scn_Stage1.h"

//!	シェーダー定数バッファ
#include "CbSystem.h"	//!< シェーダー定数
#include "BG/BG_Sea.h"

static const char*	_PARAM_EX = "param/BG/Stage1/Stage1_paramEx.json";	//!< 固有パラメータリソース

/* ========================================================================= */
//!		tBGStg01															 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  コンストラクタ "tBGStg01"
// ---------------------------------------------
tBGStg01::tBGStg01()
:	tBG(),
	mpSEHatchMove(NULL),
	mSeaNml1Repeat(20), mSeaNml1SpdU(1), mSeaNml1SpdV(1), mSeaNml2Repeat(10), mSeaNml2SpdU(-1), mSeaNml2SpdV(1),
	mSeaAmbient(0.2f), mSeaReflection(1), mSeaSpecular(1.0f), mSeaSpecularPower(3.0f), mSeaFresnelScale(3),
	mSeaFlickerSpeed(1), mSeaFlickerFrequency(300), mSeaFlickerAmplitude(0.02f), mSeaNmlUV(0,0), mSeaNml2UV(0,0), mSeaNml2Alpha(1), mSeaFlicker(0),
	mFoamA_Uv(TGL::Vec2(0,0)), mFoamB_Uv(TGL::Vec2(0,0)), mRiver06_Uv(TGL::Vec2(0,0)), mRiver07_Uv(TGL::Vec2(0,0)),
	mpSEGateMoveL(NULL), mpSEGateMoveR(NULL), mDriveAnmOld(0), mSLSD_UvA(TGL::Vec2(0,0)), mSLSD_UvB(TGL::Vec2(0,0)), mSLSD_UvC(TGL::Vec2(0,0)), mSLSD_UvD(TGL::Vec2(0,0)),
	mpLensflare(NULL), mpLensflareSpace(NULL)
{
	for( u8 i=0 ; i<numberof(mEftLink) ; i++ )
		SLLINK_Init( &mEftLink[i] );
	SLLINK_Init( &mEftCloudLink );
	FRGB_Set( &mSeaDiffuse, 1.0f, 1.0f, 1.0f );
	reset();
}

// ---------------------------------------------
//! @brief  デストラクタ "tBGStg01"
// ---------------------------------------------
tBGStg01::~tBGStg01()
{
	IAppResource->releaseTexPool( "models/BG/St01_Sea/St01_Sea_envS.dds" );
	IAppResource->releaseTexPool( "models/BG/St01_Sea/St01_Sea_nml.dds" );
	IAppResource->releaseTexPool( "models/BG/St01_Sea/St01_Sea2_nml.dds" );
	IAppResource->releaseTexPool( "models/BG/common/St01/St01_Flare01.dds" );
	IAppResource->releaseTexPool( "models/BG/common/St01/St01_Flare02.dds" );
	IAppResource->releaseTexPool( "models/BG/common/St01/St01_Flare03.dds" );
	IAppResource->releaseTexPool( "models/BG/common/St01/St01_Flare04.dds" );
	IAppResource->releaseTexPool( "models/BG/common/St01/St01_Post_ToneCurve_nmip.dds" );
	IAppResource->releaseTexPool( "models/BG/common/St01/St01_Post_ToneCurve_Departure_nmip.dds" );
	//IAppResource->releaseTexPool( "models/BG/common/St01R/St01_envC.dds" );
	ITGLRenderer->getFilterToneCurve()->setTexCurve( NULL );
	ITGLRenderer->getFilterToneCurve()->setEnableValue( false );

	for( u8 i=0 ; i<numberof(mEftLink) ; i++ )
		EEMIT_End( &mEftLink[i] );
	EEMIT_End( &mEftCloudLink );
	
	foreach( stl::vector<SpaceLaser*>, mSpaceLaser, it )
		delete *it;

	foreach( stl::vector<Cannon*>, mCannon, it )
		delete *it;

	SAFE_DELETE( mpLensflare );
	SAFE_DELETE( mpLensflareSpace );
}

// ---------------------------------------------
//! @brief  リセット
// ---------------------------------------------
void tBGStg01::reset()
{
	tBG::reset();
	if( mpSEHatchMove ){
		mpSEHatchMove->stop();
		mpSEHatchMove = NULL;
	}
	mbCatapultFix	= true;
	mCatapultMove.clear();
	
	mbSpaceLaser	= false;
	mSpaceLaserTime	= 0;
	foreach( stl::vector<SpaceLaser*>, mSpaceLaser, it )
		delete *it;
	mSpaceLaser.clear();
	
	for( u8 i=0 ; i<numberof(mEftLink) ; i++ )
		EEMIT_End( &mEftLink[i] );
	EEMIT_End( &mEftCloudLink );
	mEftCloudMtx	= TGL::Matrix::UNIT;

	if( mpSEGateMoveL ){
		mpSEGateMoveL->stop();
		mpSEGateMoveL = NULL;
	}
	if( mpSEGateMoveR ){
		mpSEGateMoveR->stop();
		mpSEGateMoveR = NULL;
	}
	
	foreach( stl::vector<Cannon*>, mCannon, it )
		delete *it;
	mCannon.clear();
	mbCannonEffect	= true;

	mDriveAnmOld = 0;
}

// ---------------------------------------------
//! @brief  リソースロード
//! @param  uBG	[in]	BGID_????
//! @return true:成功, false:失敗
// ---------------------------------------------
bool tBGStg01::load( u8 uBG )
{
	//!		テクスチャ	
	IAppResource->loadTexPool( "models/BG/St01_Sea/St01_Sea_envS.dds" );
	IAppResource->loadTexPool( "models/BG/St01_Sea/St01_Sea_nml.dds" );
	IAppResource->loadTexPool( "models/BG/St01_Sea/St01_Sea2_nml.dds" );
	
	IAppResource->loadTexPool( "models/BG/common/St01/St01_Flare01.dds" );
	IAppResource->loadTexPool( "models/BG/common/St01/St01_Flare02.dds" );
	IAppResource->loadTexPool( "models/BG/common/St01/St01_Flare03.dds" );
	IAppResource->loadTexPool( "models/BG/common/St01/St01_Flare04.dds" );
	IAppResource->loadTexPool( "models/BG/common/St01/St01_Post_ToneCurve_nmip.dds" );
	IAppResource->loadTexPool( "models/BG/common/St01/St01_Post_ToneCurve_Departure_nmip.dds" );
	//IAppResource->loadTexPool( "models/BG/common/St01R/St01_envC.dds" );

	//!		モデル		
	const u32	SCENEFS_BOSS	= (1<<S1SCENE_VALLEY_FRONT) | (1<<S1SCENE_VALLEY_SIDE) | (1<<S1SCENE_BASE_FRONT) | (1<<S1SCENE_BASE_SIDE) | (1<<S1SCENE_BOSS);
	cBGModel*	pModel;
	//!	0 : 遠景 火星
	pModel = addModel( "St01_Far_03.mdl", cBGModel::TYPE_SKYDOME, cBGModel::FLAG_FAROBJ );
	if( pModel ){
		pModel->setOnScene( (1<<S1SCENE_SEA_SIDE) | (1<<S1SCENE_SEA_FRONT) | (1<<S1SCENE_SEA_VERTICAL) | (1<<S1SCENE_VALLEY_VERTICAL) | SCENEFS_BOSS );
		SAFE_DELETE( mpLensflare );
		mpLensflare = createLensFlare( TGL::MtxGuide(pModel->GetWorldMatrix(), TGL::Vec3(26877, 21014, -14973)), "St01_Flare", _PARAM_EX, "Lensflare_Sea" );
	}
	//!	1 : 海 → 崖つなぎ
	pModel = addModel( "St01_06_Connect.mdl" );
	if( pModel )	pModel->setOnScene( (1<<S1SCENE_SEA_SIDE) | (1<<S1SCENE_SEA_FRONT) | (1<<S1SCENE_SEA_VERTICAL) );
	//!	2 : 崖ループ
	pModel = addModel( "St01_07_Loop.mdl" );
	if( pModel )	pModel->setOnScene( (1<<S1SCENE_SEA_SIDE) | (1<<S1SCENE_SEA_FRONT) | (1<<S1SCENE_SEA_VERTICAL) | (1<<S1SCENE_VALLEY_VERTICAL) | (1<<S1SCENE_VALLEY_FRONT) );
	//!	3 : ボス部屋
	pModel = addModel( "St01_11.mdl" );
	if( pModel ){
		pModel->setOnScene( SCENEFS_BOSS );
		pModel->SetNodeVisible( "St01_SLSD_A_add", false );
		pModel->SetNodeVisible( "St01_SLSD_B_add", false );
		pModel->SetNodeVisible( "St01_SLSD_C_add", false );
		pModel->SetNodeVisible( "St01_SLSD_D_add", false );
		pModel->SetNodeVisible( "St01_SLSD_Flare_add", false );
	}
	//!	4 : ゲート
	pModel = addModel( "Obj_St01_Gate.mdl" );
	if( pModel ){
		pModel->setOnScene( SCENEFS_BOSS );
		//!	モーション
		TGL::AnimationController*	pAnim = APP_NEW TGL::AnimationController( pModel );
		pModel->setAnim( pAnim );
		pModel->addMotion( 0, "Obj_St01_Gate_Close.anm" );
	}
	//!	5 : 砲台
	for( u8 i=0 ; i<7 ; ++i ){
		pModel = addModel( "Obj_St01_Cannon.mdl" );
		if( pModel ){
			pModel->setOnScene( SCENEFS_BOSS );
			//!	モーション
			TGL::AnimationController*	pAnim = APP_NEW TGL::AnimationController( pModel );
			pModel->setAnim( pAnim );
			pModel->addMotion( 0, "Obj_St01_Cannon_Stand.anm" );
			pModel->addMotion( 1, "Obj_St01_Cannon_Stop_A.anm" );
			pModel->addMotion( 2, "Obj_St01_Cannon_Stop_B.anm" );
		}
	}
	//!	6 : 柱
	pModel = addModel( "Obj_St01_SLSDrive.mdl" );
	if( pModel ){
		pModel->setOnScene( SCENEFS_BOSS );
		//!	モーション
		TGL::AnimationController*	pAnim = APP_NEW TGL::AnimationController( pModel );
		pModel->setAnim( pAnim );
		pModel->addMotion( 0, "Obj_St01_SLSDrive_Open.anm" );
	}
	
	//		宇宙(発進)		
	const u32	SCENEFS_SPACE	= (1<<S1SCENE_SPACE_FAR) | (1<<S1SCENE_SPACE_NEAR) | (1<<S1SCENE_SPACE_CATAPULT);
	//!	遠景 宇宙
	pModel = addModel( "St01_Far_Space.mdl", cBGModel::TYPE_SKYDOME, cBGModel::FLAG_FAROBJ );
	if( pModel ){
		pModel->setOnScene( SCENEFS_SPACE );
		SAFE_DELETE( mpLensflareSpace );
		mpLensflareSpace = createLensFlare( TGL::MtxGuide(pModel->GetWorldMatrix(), TGL::Vec3(27356,14287,-8235)), "St01_Flare", _PARAM_EX, "Lensflare_Space" );
	}
	//!	艦隊
	pModel = addModel( "St01_Ship.mdl" );
	if( pModel ){
		pModel->setOnScene( SCENEFS_SPACE );

		TGL::AnimationController*	pAnim = APP_NEW TGL::AnimationController( pModel );
		pModel->setAnim( pAnim );
		pModel->addMotion( 0, "St01_Ship.anm" );
		pModel->getAnim()->Play( 0, true, true );
	}
	//!	旗艦 カタパルト
	pModel = addModel( "Obj_St01_Catapult.mdl" );
	if( pModel )	pModel->setOnScene( (1<<S1SCENE_SPACE_NEAR) | (1<<S1SCENE_SPACE_CATAPULT) );
	//!	旗艦 カタパルト光
	pModel = addModel( "Obj_St01_CatapultLight.mdl" );
	if( pModel ){
		pModel->setOnScene( (1<<S1SCENE_SPACE_NEAR) | (1<<S1SCENE_SPACE_CATAPULT) );

		TGL::AnimationController*	pAnim = APP_NEW TGL::AnimationController( pModel );
		pModel->setAnim( pAnim );
		pModel->addMotion( 0, "Obj_St01_CatapultLight.anm" );
		pModel->getAnim()->Play( 0, false, true, DELTA );
		pModel->getAnim()->SetAnimationSpeed( -1.0f );
	}
	//!	旗艦 ハッチ
	pModel = addModel( "Obj_St01_Hatch.mdl" );
	if( pModel ){
		pModel->setOnScene( (1<<S1SCENE_SPACE_NEAR) | (1<<S1SCENE_SPACE_CATAPULT) );

		TGL::AnimationController*	pAnim = APP_NEW TGL::AnimationController( pModel );
		pModel->setAnim( pAnim );
		pModel->addMotion( 0, "Obj_St01_Hatch.anm" );
		pModel->getAnim()->Play( 0, false, true, DELTA );
		pModel->getAnim()->SetAnimationSpeed( -1.0f );
	}

	//!		シーンパラメータ		
	mSceneParam.getFromJson( "param/BG/Stage1/Stage1_param.json" );

	//!		パラメータ		
	{	//!	海
		TGL::JsonUtil	json( "models/BG/St01_Sea/St01_Sea_param.json" );
		TGL::Json::array	jsonArray = json.getArray("Sea");
		foreach( TGL::Json::array, jsonArray, it ){
			TGL::Json::object&	work = it->get<TGL::Json::object>();
			mSeaNml1Repeat		= (f32)atof( work["Normal1_Repeat"].to_str().c_str() );
			mSeaNml1SpdU		= (f32)atof( work["Normal1_Speed_U"].to_str().c_str() );
			mSeaNml1SpdV		= (f32)atof( work["Normal1_Speed_V"].to_str().c_str() );
			mSeaNml2Repeat		= (f32)atof( work["Normal2_Repeat"].to_str().c_str() );
			mSeaNml2SpdU		= (f32)atof( work["Normal2_Speed_U"].to_str().c_str() );
			mSeaNml2SpdV		= (f32)atof( work["Normal2_Speed_V"].to_str().c_str() );
			mSeaNml2Alpha		= (f32)atof( work["Normal2_Alpha"].to_str().c_str() );
			
			mSeaAmbient			= (f32)atof( work["Ambient"].to_str().c_str() );
			mSeaReflection		= (f32)atof( work["Reflection"].to_str().c_str() );
			mSeaDiffuse.fR		= (f32)atof( work["DiffuseR"].to_str().c_str() );
			mSeaDiffuse.fG		= (f32)atof( work["DiffuseG"].to_str().c_str() );
			mSeaDiffuse.fB		= (f32)atof( work["DiffuseB"].to_str().c_str() );
			mSeaSpecular		= (f32)atof( work["Specular"].to_str().c_str() );
			mSeaSpecularPower	= (f32)atof( work["SpecularPower"].to_str().c_str() );
			
			mSeaFresnelScale	= (f32)atof( work["FresnelScale"].to_str().c_str() );
			
			mSeaFlickerSpeed	= (f32)atof( work["Flicker_Speed"].to_str().c_str() );
			mSeaFlickerFrequency= (f32)atof( work["Flicker_Frequency"].to_str().c_str() );
			mSeaFlickerAmplitude= (f32)atof( work["Flicker_Amplitude"].to_str().c_str() );
		}
	}
	{	//!	固有パラメータ
		TGL::JsonUtil	json( _PARAM_EX );
		TGL::Json::array	jsonArray = json.getArray("-");
		foreach( TGL::Json::array, jsonArray, it ){
			TGL::Json::object&	work = it->get<TGL::Json::object>();
			mFoamA_Uv		= TGL::Vec2( (f32)atof( work["FoamA_Speed_U"].to_str().c_str() ),
									(f32)atof( work["FoamA_Speed_V"].to_str().c_str() ) );
			mFoamB_Uv		= TGL::Vec2( (f32)atof( work["FoamB_Speed_U"].to_str().c_str() ),
									(f32)atof( work["FoamB_Speed_V"].to_str().c_str() ) );
			mRiver06_Uv		= TGL::Vec2( (f32)atof( work["River06_Speed_U"].to_str().c_str() ),
									(f32)atof( work["River06_Speed_V"].to_str().c_str() ) );
			mRiver07_Uv		= TGL::Vec2( (f32)atof( work["River07_Speed_U"].to_str().c_str() ),
									(f32)atof( work["River07_Speed_V"].to_str().c_str() ) );
			mSLSD_UvA		= TGL::Vec2( (f32)atof( work["SLSD_A_Speed_U"].to_str().c_str() ),
									(f32)atof( work["SLSD_A_Speed_V"].to_str().c_str() ) );
			mSLSD_UvB		= TGL::Vec2( (f32)atof( work["SLSD_B_Speed_U"].to_str().c_str() ),
									(f32)atof( work["SLSD_B_Speed_V"].to_str().c_str() ) );
			mSLSD_UvC		= TGL::Vec2( (f32)atof( work["SLSD_C_Speed_U"].to_str().c_str() ),
									(f32)atof( work["SLSD_C_Speed_V"].to_str().c_str() ) );
			mSLSD_UvD		= TGL::Vec2( (f32)atof( work["SLSD_D_Speed_U"].to_str().c_str() ),
									(f32)atof( work["SLSD_D_Speed_V"].to_str().c_str() ) );
		}
	}

	//!	モデル配置
	applyModelSetFromJson( "param/BG/Stage1/Stage1_modelset.json" );
	//!	ゲート
	addModelSetAssign( "Obj_St01_Gate.mdl", "St01_11.mdl", "St01_Gate_Loc" );
	//!	砲台
	for( u8 i=1 ; i<=7 ; i++ ){
		pModel = getModel( "Obj_St01_Cannon.mdl", i-1 );
		if( !pModel )	continue;

		cModelSet*	pSet = addModelSetAssign( "Obj_St01_Cannon.mdl", "St01_11.mdl", stl::FormatString<stl::string>("St01_Cannon_01_Loc_%d",i) );
		pSet->setModelId( i-1 );
		
		mCannon.push_back( APP_NEW Cannon(*this, pModel, pSet) );
	}
	//!	柱
	addModelSetAssign( "Obj_St01_SLSDrive.mdl", "St01_11.mdl", "Obj_St01_SLSDrive_Loc" );
	
	//!	カメラパス追加
	applyCameraPathSetFromJson( "param/BG/Stage1/Stage1_camera.json" );

	//!	UVアニメ設定
	setModelUvAnim();

	//!	トーンカーブ
	ITGLRenderer->getFilterToneCurve()->setEnableValue( true );
	ITGLRenderer->getFilterToneCurve()->setTexCurve( IAppResource->getTexture("St01_Post_ToneCurve_nmip") );

	//!	シーン環境マップ
	mSceneParam.setEnvTex( "St01_envC.dds" );
	ICharacter->overwriteBGEnvTex();	//!< キャラクターへの適用
	overwriteEnvTex();	//!< 背景モデルへの適用

	//!	頂点宣言
	const TGL::Vertex::SlotTable	slottable[] = {
		{ TGL::Vertex::SLOT_POSITION,	TGL::Vertex::TYPE_FLOAT3 },
		{ TGL::Vertex::SLOT_TEXCOORD,	TGL::Vertex::TYPE_FLOAT4 },
		{ TGL::Vertex::SLOT_TEXCOORD,	TGL::Vertex::TYPE_FLOAT2 },
	};
	mInputLayout.init( slottable, numberof(slottable) );

	//!	シーン遷移
	{
		BGSceneChange::cSet	set;
		//!	シーンパラメータ
		set.addParam( BGSceneChange::cParamTable( 1950,   0, S1SCENE_SEA_SIDE) );			//!< 海・右側
		set.addParam( BGSceneChange::cParamTable( 2550, 440, S1SCENE_SEA_FRONT) );			//!< 海・背後
		set.addParam( BGSceneChange::cParamTable( 3050, 310, S1SCENE_SEA_VERTICAL) );		//!< 海・俯瞰
		set.addParam( BGSceneChange::cParamTable( 4600,  50, S1SCENE_VALLEY_VERTICAL) );	//!< 崖・俯瞰
		set.addParam( BGSceneChange::cParamTable( 5800, 390, S1SCENE_VALLEY_FRONT) );		//!< 崖・背後
		set.addParam( BGSceneChange::cParamTable( 6950, 200, S1SCENE_VALLEY_SIDE) );		//!< 崖・左側
		set.addParam( BGSceneChange::cParamTable( 7850, 300, S1SCENE_BASE_FRONT) );			//!< 基地・背後～俯瞰
		set.addParam( BGSceneChange::cParamTable( 8200, 240, S1SCENE_BASE_SIDE) );			//!< 基地・右側
		set.addParam( BGSceneChange::cParamTable(    0, 250, S1SCENE_BOSS) );				//!< ボス・戦闘開始
		mSceneChange.getSet()[1] = set;
	}

	//!	更新関数登録
	const u32	SCENE_MAX = S1SCENE_SPACE_FAR-1;
	addUpdateEvent( NULL,					S1SCENE_SPACE_FAR,			S1SCENE_SPACE_FAR, setupSpaceFarCB );	//!< 宇宙 遠景
	addUpdateEvent( NULL,					S1SCENE_SPACE_NEAR,			S1SCENE_SPACE_NEAR, setupSpaceNearCB );	//!< 宇宙 戦艦内
	addUpdateEvent( NULL,					S1SCENE_SPACE_CATAPULT,		S1SCENE_SPACE_CATAPULT, setupSpaceCatapultCB );	//!< 宇宙 カタパルト
	addUpdateEvent( updateSpaceCB,			S1SCENE_SPACE_FAR,			S1SCENE_SPACE_CATAPULT, setupSpaceCB, finalSpaceCB );	//!< 宇宙
//	addUpdateEvent( updateCloudParCB,		S1SCENE_SEA_SIDE,			S1SCENE_SEA_FRONT, setupCloudParCB, finalCloudParCB );	//!< 雲パーティクル
	addUpdateEvent( updateSeaCB,			S1SCENE_SEA_SIDE,			S1SCENE_SEA_FRONT, setupSeaCB, finalSeaCB );	//!< 海
	addUpdateEvent( updateWaterfallEndCB,	S1SCENE_VALLEY_VERTICAL,	SCENE_MAX );	//!< 滝終了
//	addUpdateEvent( NULL,					S1SCENE_VALLEY_VERTICAL,	S1SCENE_VALLEY_VERTICAL, setupFarMountCB );	//!< 遠景 山表示
	addUpdateEvent( updateGateCloseCB,		S1SCENE_VALLEY_FRONT,		SCENE_MAX );	//!< ゲート閉まる
	addUpdateEvent( updateCannonOffCB,		S1SCENE_BASE_FRONT,			SCENE_MAX );	//!< 砲台無力化
	addUpdateEvent( updateDriveOpenCB,		S1SCENE_BASE_SIDE,			SCENE_MAX );	//!< ドライブ起動
	addUpdateEvent( updateDriveStartCB,		S1SCENE_BOSS,				SCENE_MAX );	//!< ドライブ開始

	return true;
}

// ---------------------------------------------
//! @brief  UVアニメ設定
// ---------------------------------------------
void tBGStg01::setModelUvAnim()
{
	cBGModel*	pModel;
	
	pModel = getModel( "St01_06_Connect.mdl" );
	if( pModel ){
		pModel->SetNodeUVAnime( "St01_06Ct_Foam_A_alpha", mFoamA_Uv );
		pModel->SetNodeUVAnime( "St01_06Ct_Foam_B_alpha", mFoamB_Uv );
		pModel->SetNodeUVAnime( "St01_06Cn_River_alpha", mRiver06_Uv );
		pModel->SetNodeUVAnime( "St01_06Cn_Fall_alpha", mRiver06_Uv );
	}
	pModel = getModel( "St01_07_Loop.mdl" );
	if( pModel )
		pModel->SetNodeUVAnime( "River_alpha", mRiver07_Uv );

	pModel = getModel( "St01_11.mdl" );
	if( pModel ){
		pModel->SetNodeUVAnime( "St01_SLSD_A_add", mSLSD_UvA );
		pModel->SetNodeUVAnime( "St01_SLSD_B_add", mSLSD_UvB );
		pModel->SetNodeUVAnime( "St01_SLSD_C_add", mSLSD_UvC );
		pModel->SetNodeUVAnime( "St01_SLSD_D_add", mSLSD_UvD );
		
	}
}

/* ========================================================================= */
//!		更新																 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  更新
// ---------------------------------------------
void tBGStg01::update()
{
	//!	トーンカーブ
	if( mScene != mSceneOld ){
		if( mScene >= S1SCENE_SPACE_FAR )
			ITGLRenderer->getFilterToneCurve()->setTexCurve( IAppResource->getTexture("St01_Post_ToneCurve_Departure_nmip") );
		else
			ITGLRenderer->getFilterToneCurve()->setTexCurve( IAppResource->getTexture("St01_Post_ToneCurve_nmip") );
	}

	//!	親クラス
	tBG::update();
	
	//!	シーン遷移
	mSceneChange.update( *this );

	//!	海
	if( mScene >= S1SCENE_SEA_SIDE && mScene <= S1SCENE_SEA_VERTICAL )
		ITGLRenderer->addDrawList( TGL::Renderer::DPASS_MAIN, GPRI_BG(4), mTskHead.mTot, TGL::Camera::CAMF_00, tBGStg01::drawlistExecSea, (void*)this );

	//!	レンズフレア
	if( mpLensflare )
		mpLensflare->setEnable( (mScene < S1SCENE_BOSS) );
	if( mpLensflareSpace )
		mpLensflareSpace->setEnable( (mScene == S1SCENE_SPACE_FAR) | (mScene == S1SCENE_SPACE_CATAPULT) );

	//!	レーザー
	for( stl::vector<SpaceLaser*>::iterator it=mSpaceLaser.begin() ; it!=mSpaceLaser.end() ; ){
		if( !(*it)->update(getDeltaTime()) ){
			delete *it;
			it = mSpaceLaser.erase( it );
		}else{
			++it;
		}
	}

	//!	砲台
	foreach( stl::vector<Cannon*>, mCannon, it )
		(*it)->update( getDeltaTime() );
}

// ---------------------------------------------
//! @brief  スクリプトメッセージ受け取り
//! @param  nId		[in]	メッセージID
//! @param  nParam	[in]	パラメータ
// ---------------------------------------------
void tBGStg01::receiveMessage( s32 nId, s32 nParam )
{
	switch( nId ){
		case STG01MSG_HATCH_OPEN:	//!	ハッチ開ける
		{
			cBGModel*	pModel = getModel( "Obj_St01_Hatch.mdl" );
			if( pModel ){
				pModel->getAnim()->Play( 0, false, true );
				pModel->getAnim()->SetAnimationSpeed( 1.0f );
				//!	SE
				mpSEHatchMove = IAppSound->play( SDID_ONMEM, SE_S1_GATEMOVE, TGL::SoundChannel::PLAYF_LOOP );
				if( mpSEHatchMove ){
					mpSEHatchMove->setField( TGL::Vec3(0,20,350), 500, 0.3f, 1 );
					mpSEHatchMove->setVolume( 0.01f, 1.0f, 60 );
					mpSEHatchMove->setLoopKeep( 15 );
				}
			}

			pModel = getModel( "Obj_St01_CatapultLight.mdl" );
			if( pModel ){
				pModel->getAnim()->Play( 0, false, true );
				pModel->getAnim()->SetAnimationSpeed( 1.0f );
			}
		}	break;

		case STG01MSG_CATAPULTMOVE:	//!	発進 カタパルト移動
			mCatapultMove.init( TGL::TransValue::MODE_NORMAL, (f32)nParam, TGL::TransValue::CURVE_ADD0, 999, 0.0f, 1.0f );
			break;

		case STG01MSG_CATAPULTFIX_OFF:	//!	発進 自機とカタパルトの固定を外す
			mbCatapultFix = false;
			break;

		case STG01MSG_SPACE_LASER_ON:	//!	発進 レーザーオン
			mbSpaceLaser = true;
			break;

		case STG01MSG_SPACE_LASER_OFF:	//!	発進 レーザーオフ
			mbSpaceLaser = false;
			break;

		case STG01MSG_WINDSTOP:	//!	風とめる
			EEMIT_End( &mEftCloudLink );
			break;

		case STG01MSG_CANNON_FIRST:	//!	対空砲
			if( (u32)nParam < mCannon.size() )
				mCannon[nParam]->shot( true );
			break;

		case STG01MSG_CANNON_RANDOM:	//!	対空砲 ランダム
			foreach( stl::vector<Cannon*>, mCannon, it )
				(*it)->mbRand = nParam != 0;
			break;

		case STG01MSG_CANNON_EFFECT:	//!	対空砲 エフェクト
			mbCannonEffect = nParam != 0;
			break;
	}
}

// ---------------------------------------------
//! @brief  宇宙(遠景) 初期化
// ---------------------------------------------
void tBGStg01::setupSpaceFar()
{
	TGL::Skelton::Joint*	pJoint = getModelJoint( "St01_Ship.mdl", "SK_NEAR_ROOT" );
	if( pJoint )	pJoint->setManualScale( DELTA );
	pJoint = getModelJoint( "St01_Ship.mdl", "SK_FAR_FLAG_SHIP" );
	if( pJoint )	pJoint->setManualScale( 1.0f );
	pJoint = getModelJoint( "St01_Ship.mdl", "SK_FAR_OTHER_SHIP" );
	if( pJoint )	pJoint->setManualScale( 1.0f );
}
// ---------------------------------------------
//! @brief  宇宙(戦艦内) 初期化
// ---------------------------------------------
void tBGStg01::setupSpaceNear()
{
	TGL::Skelton::Joint*	pJoint = getModelJoint( "St01_Ship.mdl", "SK_NEAR_ROOT" );
	if( pJoint )	pJoint->setManualScale( 1.0f );
	pJoint = getModelJoint( "St01_Ship.mdl", "SK_FAR_FLAG_SHIP" );
	if( pJoint )	pJoint->setManualScale( DELTA );
	pJoint = getModelJoint( "St01_Ship.mdl", "SK_FAR_OTHER_SHIP" );
	if( pJoint )	pJoint->setManualScale( DELTA );
}
// ---------------------------------------------
//! @brief  宇宙(カタパルト) 初期化
// ---------------------------------------------
void tBGStg01::setupSpaceCatapult()
{
	TGL::Skelton::Joint*	pJoint = getModelJoint( "St01_Ship.mdl", "SK_NEAR_ROOT" );
	if( pJoint )	pJoint->setManualScale( 1.0f );
	pJoint = getModelJoint( "St01_Ship.mdl", "SK_FAR_FLAG_SHIP" );
	if( pJoint )	pJoint->setManualScale( DELTA );
	pJoint = getModelJoint( "St01_Ship.mdl", "SK_FAR_OTHER_SHIP" );
	if( pJoint )	pJoint->setManualScale( 1.0f );
}
// ---------------------------------------------
//! @brief  宇宙 初期化
// ---------------------------------------------
void tBGStg01::setupSpace()
{
	mbCatapultFix	= true;
	mCatapultMove.clear();
	mbSpaceLaser	= false;
}
// ---------------------------------------------
//! @brief  宇宙 終了
// ---------------------------------------------
void tBGStg01::finalSpace()
{
	mbSpaceLaser = false;
}
// ---------------------------------------------
//! @brief  宇宙
//! @param  bSceneChg	[in]	true:シーン変更瞬間
// ---------------------------------------------
void tBGStg01::updateSpace( bool bSceneChg )
{
	//!	ハッチ開閉音
	if( mpSEHatchMove ){
		cBGModel*	pModel = getModel( "Obj_St01_Hatch.mdl" );
		if( pModel ){
			if( pModel->getAnim()->IsPlay() ){
				SAFE_LOOPKEEP_ON( mpSEHatchMove );
			}else{
				TGL::SoundChannel*	pCh = IAppSound->play( SDID_ONMEM, SE_S1_GATECLOSE );
				if( pCh )
					pCh->setField( mpSEHatchMove->getPos(), 3000, 0.3f, 1 );
				mpSEHatchMove = NULL;
			}
		}
	}

	//!	カタパルト
	if( mbCatapultFix ){
		//!	カタパルト動かす
		cBGModel*	pModel = getModel( "Obj_St01_Catapult.mdl" );
		cModelSet*	pSet = getModelSet( "Obj_St01_Catapult.mdl" );
		if( !pModel || !pSet )	return;

		mCatapultMove.update( getDeltaTime() );
		pSet->setPosOfs( TGL::Vec3::ZERO.lerp(TGL::Vec3(0,0,2700), mCatapultMove.getValue()) );

		//!	自機をくっつける
		tCharacter*	pPlayer = ICharacter->getPlayer();
		if( !pPlayer )	return;
		
		TGL::Matrix	m0 = *pModel->GetBoneWorldMatrix( "Catapult" );
		m0.setW( TGL::Vec3(0,0,0).apply(m0) );
		m0 *= pSet->getWorldMtx();

		pPlayer->setModelPos( m0.getW() );
	}
	
	//!	レーザー
	if( mbSpaceLaser ){
		mSpaceLaserTime -= getDeltaTime();
		if( mSpaceLaserTime <= 0.0f ){
			mSpaceLaser.push_back( APP_NEW SpaceLaser );
			mSpaceLaserTime = RANDF2( 8, 30 );
		}
	}
}

// ---------------------------------------------
//! @brief  雲パーティクル 初期化
// ---------------------------------------------
void tBGStg01::setupCloudPar()
{
	//!	パーティクル
	if( !ITGLSystem->isGraphicsLevelLow() && !ITGLSystem->isPostFilterLevelLow() ){
		TGL::MtxGuide	guide( &mEftCloudMtx );
		IAppEffect->req( EFTNO_SCENES1_CLOUDFOG, TGL::Vec3::ZERO, TGL::Vec3::ZERO, NULL, &guide, NULL, NULL, 0, 0, &mEftCloudLink );
	}
}
// ---------------------------------------------
//! @brief  雲パーティクル 終了
// ---------------------------------------------
void tBGStg01::finalCloudPar()
{
	EEMIT_End( &mEftCloudLink );
}
// ---------------------------------------------
//! @brief  雲パーティクル
//! @param  bSceneChg	[in]	true:シーン変更瞬間
// ---------------------------------------------
void tBGStg01::updateCloudPar( bool bSceneChg )
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

#if 0
		//!	Z : カメラ移動逆方向
		v0 = v0.normalize();
		mEftCloudMtx.setZ( v0 );
		
		//!	X : ZY垂線
		v0 = mEftCloudMtx.getZ().cross( mEftCloudMtx.getY() );
		mEftCloudMtx.setX( (-v0) );
#else
		//!	XZ : 定数
		mEftCloudMtx.setX( TGL::Vec3(-TGL::Vec3::X) );
		mEftCloudMtx.setZ( TGL::Vec3(-TGL::Vec3::Z) );
#endif
	}
	
	//!	座標 : カメラ位置から、カメラ移動量を足す
	v0 = pCam->getPos();
	v0 += pCam->getMove() * 150.0f;
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
//! @brief  海 初期化
// ---------------------------------------------
void tBGStg01::setupSea()
{
	EEMIT_End( &mEftLink[0] );
	//!	波打ち
	IAppEffect->req( EFTNO_SCENES1_SEASWELL_L, TGL::Vec3( 2300, -1000, 26750), TGL::Vec3::ZERO, &mEftLink[0] );
	IAppEffect->req( EFTNO_SCENES1_SEASWELL_L, TGL::Vec3( 7184, -1000, 22588), TGL::Vec3::ZERO, &mEftLink[0] );
	IAppEffect->req( EFTNO_SCENES1_SEASWELL_M, TGL::Vec3(15500, -1000, 31250), TGL::Vec3::ZERO, &mEftLink[0] );
	IAppEffect->req( EFTNO_SCENES1_SEASWELL_L, TGL::Vec3( 3143, -1000, 34146), TGL::Vec3::ZERO, &mEftLink[0] );
	IAppEffect->req( EFTNO_SCENES1_SEASWELL_M, TGL::Vec3( 1850, -1000, 37750), TGL::Vec3(0, -90, 0),    &mEftLink[0] );
	IAppEffect->req( EFTNO_SCENES1_SEASWELL_S, TGL::Vec3(-1780, -1000, 40692), TGL::Vec3(0, -90, 0),    &mEftLink[0] );
	IAppEffect->req( EFTNO_SCENES1_SEASWELL_M, TGL::Vec3(-1000, -1000, 42550), TGL::Vec3(0, -90, 0),    &mEftLink[0] );
	IAppEffect->req( EFTNO_SCENES1_SEASWELL_L, TGL::Vec3( 3607, -1000, 41826), TGL::Vec3(0, -90, 0),    &mEftLink[0] );
	IAppEffect->req( EFTNO_SCENES1_SEASWELL_L, TGL::Vec3( 2150, -1000, 46000), TGL::Vec3(0, -90, 0),    &mEftLink[0] );
	IAppEffect->req( EFTNO_SCENES1_SEASWELL_M, TGL::Vec3(   42, -1000, 46550), TGL::Vec3(0, -90, 0),    &mEftLink[0] );
	IAppEffect->req( EFTNO_SCENES1_SEASWELL_L, TGL::Vec3(-3050, -1000, 47076), TGL::Vec3(0, -90, 0),    &mEftLink[0] );
	IAppEffect->req( EFTNO_SCENES1_SEASWELL_S, TGL::Vec3( 2930, -1000, 48740), TGL::Vec3(0, -90, 0),    &mEftLink[0] );
	IAppEffect->req( EFTNO_SCENES1_SEASWELL_BL, TGL::Vec3(-1490, -1000, 50173), TGL::Vec3(0, -90, 0), &mEftLink[0] );

	IAppEffect->req( EFTNO_SCENES1_SEASWELL_2A, TGL::Vec3(  305, -1000, 52600), TGL::Vec3(0,  90, 0), &mEftLink[0] );
	IAppEffect->req( EFTNO_SCENES1_SEASWELL_2B, TGL::Vec3( 7400, -1000, 49600), TGL::Vec3(0, -50, 0), &mEftLink[0] );
	IAppEffect->req( EFTNO_SCENES1_SEASWELL_2B, TGL::Vec3(11500, -1000, 46500), TGL::Vec3(0, -75, 0), &mEftLink[0] );
	IAppEffect->req( EFTNO_SCENES1_SEASWELL_2B, TGL::Vec3(-6000, -1000, 50000), TGL::Vec3(0,-135, 0), &mEftLink[0] );

	if( mSE.size() <= 0 ){
		TGL::SoundChannel*	pCh;
		for( u8 i=0 ; i<4 ; i++ ){	//!< 音
			pCh = IAppSound->play( SDID_ONMEM, SE_S1_SEAWAVEGURGLE, TGL::SoundChannel::PLAYF_LOOP | TGL::SoundChannel::PLAYF_RANDSTART | TGL::SoundChannel::PLAYF_NOHISTORY );
			if( pCh ){
				const TGL::Vec3	_POS[4] = {	//!< 座標
					TGL::Vec3( 2300, -1000, 26750 ),
					TGL::Vec3(-2500, -1000, 32000 ),
					TGL::Vec3( 2500, -1000, 38000 ),
					TGL::Vec3(-2000, -1000, 42000 ),
				};
				pCh->setField( _POS[i], 3000.0f, 1, 1 );
				pCh->setVolume( 0.01f, 1.0f, 60 );
				mSE.push_back( pCh );
			}
		}

		//!	滝
		IAppEffect->req( EFTNO_SCENES1_FALLSPLASH, TGL::Vec3( -156, -1000, 56100), TGL::Vec3(0, 0, 0), &mEftLink[0] );
		pCh = IAppSound->play( SDID_ONMEM, SE_S1_WATERFALL, TGL::SoundChannel::PLAYF_LOOP );
		if( pCh ){
			pCh->setField( TGL::Vec3(-156, -1000, 56100), 4000.0f, 1, 1 );
			pCh->setVolume( 0.01f, 1.0f, 60 );
			mSE.push_back( pCh );
		}
	}
}
// ---------------------------------------------
//! @brief  海 終了
// ---------------------------------------------
void tBGStg01::finalSea()
{
	EEMIT_End( &mEftLink[0] );
}
// ---------------------------------------------
//! @brief  更新 海
//! @param  bSceneChg	[in]	true:シーン変更瞬間
// ---------------------------------------------
void tBGStg01::updateSea( bool bSceneChg )
{
	//!	海UV
	mSeaNmlUV.x		+= mSeaNml1SpdU * getDeltaTime() * 0.0001f;
	mSeaNmlUV.y		+= mSeaNml1SpdV * getDeltaTime() * 0.0001f;
	mSeaNml2UV.x	+= mSeaNml2SpdU * getDeltaTime() * 0.0001f;
	mSeaNml2UV.y	+= mSeaNml2SpdV * getDeltaTime() * 0.0001f;
	
	//!	海 揺らぎ
	mSeaFlicker	+= mSeaFlickerSpeed * getDeltaTime() * 0.0001f;
	
/*
	//!	遠景 山非表示
	if( mScene < 1 ){
		cBGModel*	pModel = getModel( "St01_Far_03.mdl" );
		if( pModel )
			pModel->SetNodeVisible( "St01_03_Mount_nofog_alpha", false );
	}
*/
}

// ---------------------------------------------
//! @brief  更新 遠景 山表示
// ---------------------------------------------
void tBGStg01::setupFarMount()
{
/*
	//!	遠景 山表示
	cBGModel*	pModel = getModel( "St01_Far_03.mdl" );
	if( pModel )
		pModel->SetNodeVisible( "St01_03_Mount_nofog_alpha", true );
*/
}

// ---------------------------------------------
//! @brief  更新 滝 終了
//! @param  bSceneChg	[in]	true:シーン変更瞬間
// ---------------------------------------------
void tBGStg01::updateWaterfallEnd( bool bSceneChg )
{
	if( !bSceneChg )	return;
	//!	※仮 フェードアウト中にステージ解放されるとまずい
	foreach( stl::vector<TGL::SoundChannel*>, mSE, it )
		(*it)->stop( 60 );
	mSE.clear();
}

// ---------------------------------------------
//! @brief  更新 ゲート閉まる
//! @param  bSceneChg	[in]	true:シーン変更瞬間
// ---------------------------------------------
void tBGStg01::updateGateClose( bool bSceneChg )
{
	cBGModel*	pModel = getModel( "Obj_St01_Gate.mdl" );
	if( !pModel || !pModel->getAnim() )	return;

	//!	初期化
	if( mSceneOld < S1SCENE_VALLEY_FRONT ){
		pModel->getAnim()->Play( 0, false, true );
		pModel->getAnim()->SetAnimationSpeed( 0.017f );
		
		mpSEGateMoveL = IAppSound->play( SDID_ONMEM, SE_S1_GATEMOVE, TGL::SoundChannel::PLAYF_LOOP | TGL::SoundChannel::PLAYF_RANDSTART | TGL::SoundChannel::PLAYF_NOHISTORY );
		if( mpSEGateMoveL ){	//!< 左
			mpSEGateMoveL->setField( TGL::Vec3(1130, 1100, 300000), 6000.0f, 0.1f, 1 );
			mpSEGateMoveL->setVolume( 0.01f, 1.0f, 60 );
			mpSEGateMoveL->setLoopKeep( 15 );
			mLoopKeepSE.push_back( mpSEGateMoveL );
		}
		mpSEGateMoveR = IAppSound->play( SDID_ONMEM, SE_S1_GATEMOVE, TGL::SoundChannel::PLAYF_LOOP | TGL::SoundChannel::PLAYF_RANDSTART | TGL::SoundChannel::PLAYF_NOHISTORY );
		if( mpSEGateMoveR ){	//!< 右
			mpSEGateMoveR->setField( TGL::Vec3(-1130, 1100, 300000), 6000.0f, 0.1f, 1 );
			mpSEGateMoveR->setVolume( 0.01f, 1.0f, 60 );
			mpSEGateMoveR->setLoopKeep( 15 );
			mLoopKeepSE.push_back( mpSEGateMoveR );
		}
		
		//!	砲台モーション
		foreach( stl::vector<Cannon*>, mCannon, it )
			(*it)->mpModel->getAnim()->Play( 0, false, true );
	}
	
	//!	SE停止
	if( mpSEGateMoveL || mpSEGateMoveR ){
		if( !pModel->getAnim()->IsPlay() ){
			eraseLoopKeepSE( mpSEGateMoveL );
			eraseLoopKeepSE( mpSEGateMoveR );
			mpSEGateMoveL = mpSEGateMoveR = NULL;
			
			TGL::SoundChannel*	pCh = IAppSound->play( SDID_ONMEM, SE_S1_GATECLOSE );
			if( pCh )
				pCh->setField( TGL::Vec3(0, 1100, 300000), 6000.0f, 0.0f, 1 );
		}
	}
}

// ---------------------------------------------
//! @brief  更新 砲台無力化
//! @param  bSceneChg	[in]	true:シーン変更瞬間
// ---------------------------------------------
void tBGStg01::updateCannonOff( bool bSceneChg )
{
	//!	初期化
	if( mSceneOld < S1SCENE_BASE_FRONT ){
		//!	砲台モーション
		foreach( stl::vector<Cannon*>, mCannon, it )
			(*it)->mpModel->getAnim()->Play( 1, false, true );
	}
}

// ---------------------------------------------
//! @brief  更新 ドライブ起動
//! @param  bSceneChg	[in]	true:シーン変更瞬間
// ---------------------------------------------
void tBGStg01::updateDriveOpen( bool bSceneChg )
{
	cBGModel*	pModel = getModel( "Obj_St01_SLSDrive.mdl" );
	if( !pModel || !pModel->getAnim() )	return;

	//!	初期化
	if( mSceneOld < S1SCENE_BASE_SIDE ){
		pModel->getAnim()->Play( 0, false, true );
		pModel->getAnim()->SetAnimationSpeed( 1.0f );
		mDriveAnmOld = 0;
		
		TGL::SoundChannel*	pCh = IAppSound->play( SDID_ONMEM, SE_S1R_DRIVEBEGIN );
		if( pCh )
			pCh->setField( TGL::Vec3(0, 0, 310000), 4000.0f, 0.0f, 1 );
	}
	
	//!	SE
	if( mDriveAnmOld < 1.0f && pModel->getAnim()->GetCurrentTime() >= 1.0f ){
		TGL::SoundChannel*	pCh = IAppSound->play( SDID_ONMEM, SE_S1R_DRIVEOPEN_S );
		if( pCh )
			pCh->setField( TGL::Vec3(0, -1680, 310000), 4000.0f, 0.0f, 1 );
	}
	if( mDriveAnmOld < 1.75f && pModel->getAnim()->GetCurrentTime() >= 1.75f ){
		TGL::SoundChannel*	pCh = IAppSound->play( SDID_ONMEM, SE_S1R_DRIVEOPEN_L );
		if( pCh )
			pCh->setField( TGL::Vec3(0, 880, 310000), 4000.0f, 0.0f, 1 );
	}
	
	mDriveAnmOld = pModel->getAnim()->GetCurrentTime();
}

// ---------------------------------------------
//! @brief  更新 ドライブ開始
//! @param  bSceneChg	[in]	true:シーン変更瞬間
// ---------------------------------------------
void tBGStg01::updateDriveStart( bool bSceneChg )
{
	//!	初期化
	if( bSceneChg ){
		cBGModel*	pModel = getModel( "St01_11.mdl" );
		if( pModel ){
			pModel->SetNodeVisible( "St01_SLSD_A_add", true );
			pModel->SetNodeVisible( "St01_SLSD_B_add", true );
			pModel->SetNodeVisible( "St01_SLSD_C_add", true );
			pModel->SetNodeVisible( "St01_SLSD_D_add", true );
			pModel->SetNodeVisible( "St01_SLSD_Flare_add", true );
		}
	}
}

/* ========================================================================= */
//!		SpaceLaser															 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  コンストラクタ "SpaceLaser"
// ---------------------------------------------
tBGStg01::SpaceLaser::SpaceLaser()
:	mTime(0)
{
	//!		エフェクト発生		
	TGL::Vec3	v0, v1;
	//!	座標
	TGL::Vec3	vPos( RANDF2(-50000, 50000), -25000, RANDF2(40000, 60000) );
	v0	= ICamera->getCamera(0)->getPos();
	v1	= v0.wayNL( vPos );
	vPos = v0 + v1 * 50000.0f;
	
	//!	方向
	f32	fAngle	= RANDF2( 10, 70 );		//!< 仰角
	f32	fRot	= RANDF2( 180, 360 );	//!< 回転量
	TGL::Vec3	vWay( TGL::Vec3::waySphere(TGL_Y, fAngle, fRot) );
	
	//!	わっかの回転計算おかしくてちゃんと出せないのでマトリクス追従で出す
	mMtx = TGL::Matrix::createVec( TGL_Z, vWay );
	mMtx.setW( vPos );

	TGL::MtxGuide	guide( &mMtx );
	IAppEffect->req( EFTNO_SCENES1_SPACE_LASER, TGL::Vec3::ZERO, TGL::Vec3::ZERO, NULL, &guide, NULL, NULL, 0, 0 );
}

// ---------------------------------------------
//! @brief  更新
//! @param  fDelta	[in]	デルタタイム
//! @return true:継続, false:終了
// ---------------------------------------------
bool tBGStg01::SpaceLaser::update( f32 fDelta )
{
	f32	fOld = mTime;
	mTime += fDelta;
	if( mTime >= 300.0f )	return false;
	
	//!	SE
	if( fOld < 30.0f && mTime >= 30.0f ){
		TGL::SoundChannel*	pCh = IAppSound->play( SDID_ONMEM, SE_SHOT_ELECT01 );
		if( pCh ){
			pCh->setField( mMtx.getW(), 25000.0f, 0.0f, 1 );
			pCh->setVolume( RANDF2(0.5f, 1.0f) );
			pCh->setPitch( RANDF2(0.80f, 1.0f) );
			pCh->setUserId( SNDOWNER_AMBIENT );
		}
	}
	
	return true;
}

/* ========================================================================= */
//!		Cannon																 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  砲台更新
//! @param  fDelta	[in]	デルタタイム
// ---------------------------------------------
void tBGStg01::Cannon::update( f32 fDelta )
{
	if( !mbRand )	return;

	TGL::Skelton::Joint*	pJoint = mpModel->GetSkelton()->GetJoint( "X_Rot" );

	if( IACtrl->getScene().getStep() == SCENESTEP_RESULT ){
		if( pJoint )
			pJoint->setManualRot( TGL::Vec3::ZERO );	//!< リザルト入ったら初期化
		return;
	}
	
	//!	首振り
	f32	fOld = mShotAfter;
	mShotAfter += fDelta;
	if( fOld < 60.0f && mShotAfter >= 60.0f ){
		f32	fTime = mShotTime - 30.0f;
		if( fTime > 0.0f ){
			TGL::Vec3	v0( RANDF2(-12, 4), RANDF2(-45.0f, 45.0f), 0 );
			mSwing.init( TGL::TransValue::MODE_NORMAL, fTime, TGL::TransValue::CURVE_PARABOLIC, 999, v0 );
		}
	}else
		mSwing.update( fDelta );
	if( pJoint )
		pJoint->setManualRot( mSwing.getValue() );
	
	//!	ショット発射
	mShotTime -= fDelta;
	if( mShotTime <= 0.0f ){
		mShotTime	= RANDF2(SHOTTIME_MIN, SHOTTIME_MAX);
		mShotAfter	= 0;
		shot();
	}
}

// ---------------------------------------------
//! @brief  砲台発射
//! @param  bFirst	[in]	true:一発目
// ---------------------------------------------
void tBGStg01::Cannon::shot( bool bFirst/*=false*/ )
{
	//!	位置計算
	TGL::Matrix	m0 = *mpModel->GetBoneWorldMatrix( "X_Rot" );
	m0.setW( TGL::Vec3(0,0,2280).apply(m0) );
	m0 *= mpSet->getWorldMtx();
	
	//!	エフェクト
	if( mOwner.getScene() != STG01AREA_BOSS_VERTICAL ){	//!< 縦視点時は出さない
		TGL::Vec3	v0 = ICamera->getCamera(0)->getPos().wayNL( m0.getW() );
		if( v0.dot(ICamera->getCamera(0)->getEyeVector()) > 0.0f )	//!< カメラ背面なら出さない
			IAppEffect->reqWay( EFTNO_SCENES1_CANNON, m0.getW(), m0.getZ() );
	}
	
	//!	音
	TGL::SoundChannel*	pCh;
	if( bFirst ){
		pCh = IAppSound->play( SDID_ONMEM, SE_SPREADBURST );
		if( pCh )	pCh->setField( m0.getW(), 12000.0f, 0.0f, 1 );
		pCh = IAppSound->play( SDID_ONMEM, SE_SHOT_ELECT01 );
		if( pCh )	pCh->setField( m0.getW(), 18000.0f, 0.0f, 1 );
		pCh = IAppSound->play( SDID_ONMEM, SE_SHOT_LASER_LOOP01, TGL::SoundChannel::PLAYF_LOOP );
		if( pCh ){
			pCh->setField( m0.getW(), 18000.0f, 0.0f, 1 );
			pCh->stop( 240 );
		}
	}else{
		pCh = IAppSound->play( SDID_ONMEM, SE_SPREADBURST );
		if( pCh )	pCh->setField( m0.getW(), 3000.0f, 0.0f, 1 );
		pCh = IAppSound->play( SDID_ONMEM, SE_SHOT_ELECT01 );
		if( pCh )	pCh->setField( m0.getW(), 6000.0f, 0.0f, 1 );
		pCh = IAppSound->play( SDID_ONMEM, SE_SHOT_LASER_LOOP01, TGL::SoundChannel::PLAYF_LOOP );
		if( pCh ){
			pCh->setField( m0.getW(), 6000.0f, 0.0f, 1 );
			pCh->stop( 120 );
		}
	}
}

/* ========================================================================= */
//!		描画																 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  海描画
// ---------------------------------------------
void tBGStg01::drawSea()
{
	TGL::BaseCamera*	pCam = ICamera->getRenderCamera();
	if( !pCam )	return;

	mPerfGpu.begin();

	PIX_BEGIN_EVENT(IRGBA(0xff, 0,0,0xff), "tBGStg01::drawSea");
PERF_BEGIN("tBGStg01::drawSea",IRGBA( 200,0,0,128 ));

	//!	テクスチャ
	TGL::BaseTex*	pObjNml = IAppResource->getTexPool( "St01_Sea_nml" );	//!< ノーマルマップ
	TGL::BaseTex*	pObjNml2 = IAppResource->getTexPool( "St01_Sea2_nml" );	//!< ノーマルマップ
	TGL::BaseTex*	pObjEnv = IAppResource->getTexPool( "St01_Sea_envS" );	//!< 環境マップ

	//!	シェーダー
	TGL::ShaderEffect*	pEffect = IAppResource->getShaderEffect( "BG_Sea" );
	
#ifdef DX9
	// シェーダーテクニック
	stl::string shaderTechniqueName = "Main";
	// 低スペック向け
	if( ITGLSystem->getGraphicsLevel() == SYSINFOGRPLV_LOW )
		shaderTechniqueName += "_low";
	pEffect->setTechnique( shaderTechniqueName.c_str() );
#endif

	//!	デバイス設定
	IGraphics->setWorldTransform();
	IGraphics->getRS().setZ( TGL::RenderState::Z_TEST | TGL::RenderState::Z_WRITE );
	IGraphics->getRS().setCull( TGL::RenderState::CULL_NONE );
	IGraphics->getRS().setBlend( RSBL_HALF, false );
	IGraphics->getRS().onSamplerFilter( 0, true );	//!< テクスチャフィルタON
	
	//!	サイズ定義
	const u8	_LINE = 10;
	const f32	_SIZE = 100000.0f;
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

	//!	頂点レイアウト
	typedef struct {
		TGL::Vec3	mPos;
		TGL::Vec2	mTex;
		TGL::Vec2	mTex2;
		TGL::Vec2	mRate;
	} VERTEX_T;

	//!		シェーダーパラメータ	
	TGL::cbSystem	cb;
	cb.gCameraToProjectionMatrix = ICamera->getDrawMtx();

	TGL::cbLocal	cbL;
	cbL.gCameraPosition	= pCam->getPos();
	//!	ライト
	TGL::LightGroup*	pGroup = ILight->getGroup(0);	//!< ライトグループ
	if( pGroup ){
		cbL.gLightAmbient	= TGL::Vec3( &pGroup->getAmbient()->getColor().fR );
		
		TGL::DirectionalLight*	pDirectional = pGroup->getDirectional(0);	//!< 平行光
		if( pDirectional ){
			FRGB	Col = pDirectional->getColor();
			TGL::Vec4	vDirColor( Col.fR, Col.fG, Col.fB, pDirectional->getPower() );
			cbL.gLightDirColor	= vDirColor;					//!< ライト 平行光
			cbL.gLightDirection	= pDirectional->getDirection();	//!< ライト 方向
		}
	}
	//!	フォグ
	TGL::BaseFog*	pFog = IFog->getFog(1);	//!< フォグ
	if( pFog ){
		cbL.gFogCoord	= pFog->getFogCoord();
		cbL.gFogColor	= pFog->getFogColorRgb();
	}else{
		cbL.gFogCoord	= TGL::Vec2::X;
		cbL.gFogColor	= TGL::Vec3::ONE;
	}
	cbL.gAmbient		= mSeaAmbient;			//!< アンビエント
	cbL.gNormal2Alpha	= mSeaNml2Alpha;		//!< 法線マップ透過度
	cbL.gReflection		= mSeaReflection;		//!< 環境マップ強度
	cbL.gDiffuse		= TGL::Vec3( &mSeaDiffuse.fR );	//!< ディフューズ
	cbL.gSpecular		= mSeaSpecular;			//!< スペキュラ
	cbL.gSpecularPower	= mSeaSpecularPower;	//!< スペキュラ範囲
	cbL.gFresnelScale	= mSeaFresnelScale;		//!< フレネル項スケール
	cbL.gFlickerFrequency	= mSeaFlickerFrequency;	//!< 海 揺らぎ 周期
	cbL.gFlickerAmplitude	= mSeaFlickerAmplitude;	//!< 海 揺らぎ 振幅

	//!	テクスチャ
	pEffect->setTexture( "gTextureNormal",	pObjNml,	0 );	//!< ノーマルマップ
	pEffect->setTexture( "gTextureNormal2",	pObjNml2,	1 );	//!< ノーマルマップ
	pEffect->setTexture( "gTextureEnvMap",	pObjEnv,	2 );	//!< 環境マップ
	//!	サンプラー
	pEffect->setTextureFilterPreset();

#ifdef PS4
	pEffect->setConstantBuffer( "cbSystem", &cb, sizeof(TGL::cbSystem) );
	pEffect->setConstantBuffer( "cbLocal", &cbL, sizeof(TGL::cbLocal) );
#else
	pEffect->setMatrix( "gCameraToProjectionMatrix", cb.gCameraToProjectionMatrix );
	pEffect->setValue(	"gCameraPosition",	&cbL.gCameraPosition,	sizeof(TGL::Vec3) );
	pEffect->setValue(	"gLightAmbient",	&cbL.gLightAmbient,		sizeof(FRGB) );
	pEffect->setValue(	"gLightDirColor",	&cbL.gLightDirColor,	sizeof(TGL::Vec4) );
	pEffect->setValue(	"gLightDirection",	&cbL.gLightDirection,	sizeof(TGL::Vec3) );
	pEffect->setFloat(	"gAmbient",			cbL.gAmbient );
	pEffect->setFloat(	"gNormal2Alpha",	cbL.gNormal2Alpha );
	pEffect->setFloat(	"gReflection",		cbL.gReflection );
	pEffect->setValue(	"gDiffuse",			&cbL.gDiffuse,			sizeof(FRGB) );
	pEffect->setFloat(	"gSpecular",		cbL.gSpecular );
	pEffect->setFloat(	"gSpecularPower",	cbL.gSpecularPower );
	pEffect->setFloat(	"gFresnelScale",	cbL.gFresnelScale );
	pEffect->setValue(	"gFogCoord",		&cbL.gFogCoord,			sizeof(TGL::Vec2) );
	pEffect->setValue(	"gFogColor",		&cbL.gFogColor,			sizeof(FRGB) );
	pEffect->setFloat(	"gFlickerFrequency", cbL.gFlickerFrequency );
	pEffect->setFloat(	"gFlickerAmplitude", cbL.gFlickerAmplitude );
#endif

	//!	描画
	pEffect->begin();

	for( u8 v=0 ; v<_LINE ; v++ ){
		TGL::Vec3	vBase = vOfs;
		vBase.x += (f32)( -(_SIZEHALF-_SIZECELL/2)+(_SIZECELL)*v );
		vBase.y = -1000.0f;	//!< 海抜
		
		//!	頂点設定
		VERTEX_T	vtx[_LINE*2];

		u8	vnum = 0;
		for( u8 u=0 ; u<_LINE ; u++ ){
			//!	頂点設定
			vBase.z = vOfs.z + (f32)( -(_SIZEHALF-_SIZECELL/2)+(_SIZECELL)*u );
			vtx[vnum].mPos		= vBase;
			vtx[vnum].mTex.x	= mSeaNmlUV.x + (mSeaNml1Repeat / (f32)_LINE * (f32)(u+nOfsZ));
			vtx[vnum].mTex.y	= mSeaNmlUV.y + (mSeaNml1Repeat / (f32)_LINE * (f32)(v+0));
			vtx[vnum].mTex2.x	= mSeaNml2UV.x + (mSeaNml2Repeat / (f32)_LINE * (f32)(u+nOfsZ));
			vtx[vnum].mTex2.y	= mSeaNml2UV.y + (mSeaNml2Repeat / (f32)_LINE * (f32)(v+0));
			vtx[vnum].mRate.x	= mSeaFlicker + 1.0f / (f32)_LINE * (f32)(u+nOfsZ);
			vtx[vnum].mRate.y	= mSeaFlicker + 1.0f / (f32)_LINE * (f32)(v+0);
			vnum++;
			vtx[vnum].mPos		= vBase;
			vtx[vnum].mPos.x	+= _SIZECELL;
			vtx[vnum].mTex.x	= vtx[vnum-1].mTex.x;
			vtx[vnum].mTex.y	= mSeaNmlUV.y + (mSeaNml1Repeat / (f32)_LINE * (f32)(v+1));
			vtx[vnum].mTex2.x	= vtx[vnum-1].mTex2.x;
			vtx[vnum].mTex2.y	= mSeaNml2UV.y + (mSeaNml2Repeat / (f32)_LINE * (f32)(v+1));
			vtx[vnum].mRate.x	= vtx[vnum-1].mRate.x;
			vtx[vnum].mRate.y	= mSeaFlicker + 1.0f / (f32)_LINE * (f32)(v+1);
			vnum++;
		}

		//!	描画
		TGL::VertexBuffer	vbuf( vtx, vnum );
		mInputLayout.setDevice( vbuf, pEffect );
		IGraphics->draw( TGL::Graphics::PRIM_TRIANGLESTRIP );
	}
	pEffect->end();

PERF_END("tBGStg01::drawSea");
	PIX_END_EVENT();
	mPerfGpu.end();
}

