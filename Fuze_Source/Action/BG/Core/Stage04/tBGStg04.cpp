// -------------------------------------------------------------------------- //
//! @file   tBGStg04.cpp
//! @brief  背景 ４面
//! @author Nal
//! @since  12/12/04(火)
//! 
//! COPYRIGHT (C) 2012 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
/* ====< インクルード >====================================================== */
#include "prefix.h"
#include <TGL.h>
#include "Game/Game.h"
#include "Action/Camera/cBGCamera.h"
#include "tBGStg04.h"
#include "Action/ActionTypes.h"
#include "Effect/sAppEffect.h"
#include "Sound/sAppSound.h"
#include "../../../../../tgs/scene/Stage4/scn_Stage4.h"

static const char*	_PARAM_EX = "param/BG/Stage4/Stage4_paramEx.json";	//!< 固有パラメータリソース

/* ========================================================================= */
//!		tBGStg04															 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  コンストラクタ "tBGStg04"
// ---------------------------------------------
tBGStg04::tBGStg04()
:	tBG(),
	m01_UvA(TGL::Vec2(0,0)), m01_UvB(TGL::Vec2(0,0)), m01_UvC(TGL::Vec2(0,0)), m01_UvD(TGL::Vec2(0,0)), mCubeSpd(1), mBossUnitRollSpd(1)
{
	for( u8 i=0 ; i<numberof(mEftLink) ; i++ )
		SLLINK_Init( &mEftLink[i] );
	ZeroMemory( mpSEENP, sizeof(mpSEENP) );
	ZeroMemory( mpSEENPSub, sizeof(mpSEENPSub) );
	mpSEBossLight = NULL;
	reset();
}

// ---------------------------------------------
//! @brief  デストラクタ "tBGStg04"
// ---------------------------------------------
tBGStg04::~tBGStg04()
{
	IAppResource->releaseTexPool( "models/BG/common/St04/St04_Flare01.dds" );
	IAppResource->releaseTexPool( "models/BG/common/St04/St04_Flare02.dds" );
	IAppResource->releaseTexPool( "models/BG/common/St04/St04_Flare03.dds" );
	IAppResource->releaseTexPool( "models/BG/common/St04/St04_Flare04.dds" );
	IAppResource->releaseTexPool( "models/BG/common/St04/St04_Post_ToneCurve_nmip.dds" );
	//IAppResource->releaseTexPool( "models/BG/common/St04R/St04_envC.dds" );
	ITGLRenderer->getFilterToneCurve()->setTexCurve( NULL );
	ITGLRenderer->getFilterToneCurve()->setEnableValue( false );

	for( u8 i=0 ; i<numberof(mEftLink) ; i++ )
		EEMIT_End( &mEftLink[i] );

	foreach( stl::vector<Elect*>, mElect, it )
		SAFE_DELETE( *it );

	foreach( stl::vector<BGDoorStg04*>, mDoor, it )
		SAFE_DELETE( *it );

	foreach( stl::vector<BossPiece*>, mBossPiece, it )
		SAFE_DELETE( *it );

	foreach( stl::vector<tLensFlareSF*>, mLensflare, it )
		SAFE_DELETE( *it );
}

// ---------------------------------------------
//! @brief  リセット
// ---------------------------------------------
void tBGStg04::reset()
{
	tBG::reset();
	for( u8 i=0 ; i<numberof(mEftLink) ; i++ )
		EEMIT_End( &mEftLink[i] );

	ZeroMemory( mpSEENP, sizeof(mpSEENP) );
	ZeroMemory( mpSEENPSub, sizeof(mpSEENPSub) );

	foreach( stl::vector<Elect*>, mElect, it )
		(*it)->reset();

	foreach( stl::vector<BGDoorStg04*>, mDoor, it )
		(*it)->reset();

	foreach( stl::vector<BossPiece*>, mBossPiece, it )
		SAFE_DELETE( *it );
	mBossPiece.clear();

	foreach( stl::vector<tLensFlareSF*>, mLensflare, it )
		(*it)->setEnable( false );
}

// ---------------------------------------------
//! @brief  リソースロード
//! @param  uBG	[in]	BGID_????
//! @return true:成功, false:失敗
// ---------------------------------------------
bool tBGStg04::load( u8 uBG )
{
	//!		モデル		
	cBGModel*	pModel;
	//!	遠景 
	//pModel = addModel( "St04_Far_01.mdl", cBGModel::TYPE_SKYDOME, cBGModel::FLAG_FAROBJ );
	//if( pModel )	pModel->setOnScene( cBGModel::SCENE_00 );
	//!	遠景 
	//pModel = addModel( "St04_Far_02.mdl", cBGModel::TYPE_SKYDOME, cBGModel::FLAG_FAROBJ );
	//if( pModel )	pModel->setOnScene( cBGModel::SCENE_01 );
	//!	 入り口
	pModel = addModel( "St04_Entrance.mdl" );
	//!	 通路
	pModel = addModel( "St04_RoadLoop_A.mdl" );
	//!	 T字路
	pModel = addModel( "St04_RoadT.mdl" );
	//!	 L字路
	pModel = addModel( "St04_RoadL.mdl" );
	//!	 ３軸通路
	pModel = addModel( "St04_Road3axis.mdl" );
	//!	 縦穴入口
	pModel = addModel( "St04_BossEntrance.mdl" );
	//!	 縦穴
	pModel = addModel( "St04_BossLoop.mdl" );
	//!	 ボス
	pModel = addModel( "St04_Boss.mdl" );
	pModel = addModel( "St04_Boss_Unit.mdl" );
	pModel = addModel( "St04_Boss_Unit_Dmg.mdl" );
	pModel = addModel( "St04_Boss_Unit_Roll.mdl" );
	if( pModel ){
		//!	モーション
		TGL::AnimationController*	pAnim = APP_NEW TGL::AnimationController( pModel );
		pModel->setAnim( pAnim );
		pModel->addMotion( 0, "St04_Boss_Unit_Roll_Anim.anm" );
		pModel->getAnim()->Play( 0, true, true );
	}
	//!	 ドア
	for( u8 i=0 ; i<3 ; i++ ){
		pModel = addModel( "Obj_St04_Door.mdl" );
		if( !pModel )	break;
		//!	モーション
		TGL::AnimationController*	pAnim = APP_NEW TGL::AnimationController( pModel );
		pModel->setAnim( pAnim );
		pModel->addMotion( 0, "Obj_St04_Door.anm" );
		//pModel->addMotion( 1, "Obj_St04_Door_openA.anm" );
		//pModel->addMotion( 2, "Obj_St04_Door_openB.anm" );
		//pModel->addMotion( 3, "Obj_St04_Door_openC.anm" );
		//pModel->addMotion( 4, "Obj_St04_Door_openD.anm" );
		pModel->addMotion( 1, "Obj_St04_Door_openABCD.anm" );
		pModel->getAnim()->Play( 0, true, true );
	}
	//!	 螺旋オブジェクト
	{
		const char*	_NAME[] = {
			"Obj_St04_ENP_Spiral",
			"Obj_St04_ENP_Spiral_3axis",
			"Obj_St04_ENPBoss_Spiral",
		};
		for( u8 i=0 ; i<numberof(_NAME) ; i++ ){
			pModel = addModel( stl::FormatString<stl::string>("%s.mdl", _NAME[i]) );
			if( pModel ){
				//!	モーション
				TGL::AnimationController*	pAnim = APP_NEW TGL::AnimationController( pModel );
				pModel->setAnim( pAnim );
				pModel->addMotion( 0, stl::FormatString<stl::string>("%s.anm", _NAME[i]) );
				pModel->getAnim()->Play( 0, true, true );
			}
		}
	}
	//!	 箱
	pModel = addModel( "Obj_St04_ENPBoss_Cube.mdl" );
	if( pModel ){
		//!	モーション
		TGL::AnimationController*	pAnim = APP_NEW TGL::AnimationController( pModel );
		pModel->setAnim( pAnim );
		pModel->addMotion( 0, "Obj_St04_ENPBoss_Cube.anm" );
		pModel->getAnim()->Play( 0, true, true );
	}

	//!	テクスチャ
	IAppResource->loadTexPool( "models/BG/common/St04/St04_Flare01.dds" );
	IAppResource->loadTexPool( "models/BG/common/St04/St04_Flare02.dds" );
	IAppResource->loadTexPool( "models/BG/common/St04/St04_Flare03.dds" );
	IAppResource->loadTexPool( "models/BG/common/St04/St04_Flare04.dds" );
	IAppResource->loadTexPool( "models/BG/common/St04/St04_Post_ToneCurve_nmip.dds" );
	//IAppResource->loadTexPool( "models/BG/common/St04R/St04_envC.dds" );

	//!	モデル配置
	applyModelSetFromJson( "param/BG/Stage4/Stage4_modelset.json" );
	
	//!	レンズフレア
	for( u8 i=0 ; i<2 ; i++ ){
		tLensFlareSF*	pLensflare = APP_NEW tLensFlareSF(this);
		pLensflare->setCamF( TGL::Camera::CAMF_00 );
		pLensflare->addTex( IAppResource->getTexPool("St04_Flare01") );
		pLensflare->addTex( IAppResource->getTexPool("St04_Flare02") );
		pLensflare->addTex( IAppResource->getTexPool("St04_Flare03") );
		pLensflare->addTex( IAppResource->getTexPool("St04_Flare04") );
		pLensflare->generateSphere();
		pLensflare->setEnable( false );
		mLensflare.push_back( pLensflare );
	}

	//!		シーンパラメータ		
	mSceneParam.getFromJson( "param/BG/Stage4/Stage4_param.json" );
	{	//!	固有パラメータ
		TGL::JsonUtil	json( _PARAM_EX );
		TGL::Json::array	jsonArray = json.getArray("-");
		foreach( TGL::Json::array, jsonArray, it ){
			TGL::Json::object&	work = it->get<TGL::Json::object>();
			m01_UvA		= TGL::Vec2( (f32)atof( work["01_A_Speed_U"].to_str().c_str() ),
								(f32)atof( work["01_A_Speed_V"].to_str().c_str() ) );
			m01_UvB		= TGL::Vec2( (f32)atof( work["01_B_Speed_U"].to_str().c_str() ),
								(f32)atof( work["01_B_Speed_V"].to_str().c_str() ) );
			m01_UvC		= TGL::Vec2( (f32)atof( work["01_C_Speed_U"].to_str().c_str() ),
								(f32)atof( work["01_C_Speed_V"].to_str().c_str() ) );
			m01_UvD		= TGL::Vec2( (f32)atof( work["01_D_Speed_U"].to_str().c_str() ),
								(f32)atof( work["01_D_Speed_V"].to_str().c_str() ) );
			mCubeSpd			= (f32)atof( work["Cube_AnmSpeed"].to_str().c_str() );
			mBossUnitRollSpd	= (f32)atof( work["BossUnitRoll_AnmSpeed"].to_str().c_str() );
		}
	}
	//!	背景色とフォグ色の同期
	mSceneParam.setClearSyncFog( true );

	//!	ボス箱
	for( u8 i=1 ;; i++ ){
		stl::string	strLoc = stl::FormatString<stl::string>( "%s%02d", "BossCube_LOC_", i );
		cModelSet*	pSet = addModelSetAssign( "Obj_St04_ENPBoss_Cube.mdl", "St04_Boss.mdl", strLoc, true );
		if( !pSet )
			break;
		pSet->setId( i );
		pSet->setOnScene( (1<<S4SCENE_HALL_VERTICAL) | (1<<S4SCENE_HALL_BOSS) | (1<<S4SCENE_HALL_RESULT4) );
	}

	//!	カメラパス追加
	applyCameraPathSetFromJson( "param/BG/Stage4/Stage4_camera.json" );

	//!	UVアニメ設定
	setModelUvAnim();

	//!	トーンカーブ
	ITGLRenderer->getFilterToneCurve()->setEnableValue( true );
	ITGLRenderer->getFilterToneCurve()->setTexCurve( IAppResource->getTexture("St04_Post_ToneCurve_nmip") );

	//!	シーン環境マップ
	mSceneParam.setEnvTex( "St04_envC.dds" );
	ICharacter->overwriteBGEnvTex();	//!< キャラクターへの適用
	overwriteEnvTex();	//!< 背景モデルへの適用

	//!	放電
	mElect.push_back( APP_NEW Elect(0,   -1, EFTNO_SCENES4_ELECTROOT) );
	mElect.push_back( APP_NEW Elect(1, 5400, EFTNO_SCENES4_ELECTHALLXZ) );
	mElect.push_back( APP_NEW Elect(1, 5400, EFTNO_SCENES4_ELECTHALLY) );
	mElect.push_back( APP_NEW Elect(1, 6000, EFTNO_SCENES4_ELECTHALLXZ) );
	//!	放電 通路
	mElect[ELECT_ROOT]->mTime.push_back( ElectTime(1700, getModelSet("St04_RoadLoop_A.mdl",0), ElectTime::TYPE_CAM_Z, -10000, 20000) );
	mElect[ELECT_ROOT]->mTime.push_back( ElectTime(1960, getModelSet("St04_Road3axis.mdl"),    ElectTime::TYPE_RAND,  -12000, 12000) );
	mElect[ELECT_ROOT]->mTime.push_back( ElectTime(2120, getModelSet("St04_Road3axis.mdl"),    ElectTime::TYPE_CAM_Y, -25000,  5000) );
	mElect[ELECT_ROOT]->mTime.push_back( ElectTime(2750, getModelSet("St04_RoadLoop_A.mdl",1), ElectTime::TYPE_CAM_Z, -10000, 20000) );
	mElect[ELECT_ROOT]->mTime.push_back( ElectTime(3600, getModelSet("St04_RoadLoop_A.mdl",2), ElectTime::TYPE_CAM_X, -10000, 20000) );
	mElect[ELECT_ROOT]->mTime.push_back( ElectTime(-1,   getModelSet("St04_RoadLoop_A.mdl",3), ElectTime::TYPE_CAM_Z,  -5000, 15000) );
	//!	放電 縦穴 X
	mElect[ELECT_HALLX]->mTime.push_back( ElectTime( 720, getModelSet("St04_BossEntrance.mdl"), ElectTime::TYPE_MODEL_X, -50000, 50000,  0, TGL::Vec3(0,0,30000)) );
	mElect[ELECT_HALLX]->mTime.push_back( ElectTime(2900, getModelSet("St04_BossLoop.mdl"),     ElectTime::TYPE_MODEL_X, -50000, 50000, 45, TGL::Vec3(0,0,30000)) );
	mElect[ELECT_HALLX]->mTime.push_back( ElectTime(  -1, getModelSet("St04_BossLoop.mdl"),     ElectTime::TYPE_MODEL_X, -50000, 50000,  0, TGL::Vec3(0,-50000,30000)) );
	//!	放電 縦穴 Y
	mElect[ELECT_HALLY]->mTime.push_back( ElectTime( 720, getModelSet("St04_BossEntrance.mdl"), ElectTime::TYPE_MODEL_Y, -50000, 50000,  0, TGL::Vec3(0,0,30000)) );
	mElect[ELECT_HALLY]->mTime.push_back( ElectTime(  -1, getModelSet("St04_BossLoop.mdl"),     ElectTime::TYPE_CAM_Y,   -30000,  5000,  0, TGL::Vec3(0,0,30000)) );
	//!	放電 縦穴 Z
	mElect[ELECT_HALLZ]->mTime.push_back( ElectTime( 720, getModelSet("St04_BossEntrance.mdl"), ElectTime::TYPE_MODEL_Z, -50000, 50000,  0, TGL::Vec3(0,0,30000)) );
	mElect[ELECT_HALLZ]->mTime.push_back( ElectTime(2900, getModelSet("St04_BossLoop.mdl"),     ElectTime::TYPE_MODEL_Z, -50000, 50000, 45, TGL::Vec3(0,0,30000)) );
	mElect[ELECT_HALLZ]->mTime.push_back( ElectTime(  -1, getModelSet("St04_BossLoop.mdl"),     ElectTime::TYPE_MODEL_Z, -50000, 50000,  0, TGL::Vec3(0,-50000,30000)) );

	//!	ドア
	mDoor.push_back( APP_NEW BGDoorStg04(getModelSet("Obj_St04_Door.mdl",0), S4SCENE_STRAIGHT_VERTICAL,	S4SCENE_3AXIS,			S4CAM_ROAD,		1500,  200) );	//!< ３軸通路
	mDoor.push_back( APP_NEW BGDoorStg04(getModelSet("Obj_St04_Door.mdl",1), S4SCENE_STRAIGHT_SIDE3, 	S4SCENE_STRAIGHT_END,	S4CAM_ROAD,		4650,  250) );	//!< 通路終了
	mDoor.push_back( APP_NEW BGDoorStg04(getModelSet("Obj_St04_Door.mdl",2), S4SCENE_STRAIGHT_SIDE3, 	S4SCENE_STRAIGHT_END,	S4CAM_ROAD,		4800,  150) );	//!< 通路終了
	mDoor.push_back( APP_NEW BGDoorStg04(getModelSet("Obj_St04_Door.mdl",3), S4SCENE_STRAIGHT_END, 		S4SCENE_STRAIGHT_END,	S4CAM_ROAD,		4870,  180) );	//!< 通路終了
	mDoor.push_back( APP_NEW BGDoorStg04(getModelSet("Obj_St04_Door.mdl",3), S4SCENE_STRAIGHT_END, 		S4SCENE_STRAIGHT_END,	S4CAM_HALL,		 150, -250) );	//!< 縦穴 閉まる
	mDoor.push_back( APP_NEW BGDoorStg04(getModelSet("Obj_St04_Door.mdl",1), S4SCENE_HALL_RESULT, 		S4SCENE_HALL_RESULT,	S4CAM_RESULT00,	   0,  120) );	//!< リザルト戦艦
	mDoor.push_back( APP_NEW BGDoorStg04(getModelSet("Obj_St04_Door.mdl",2), S4SCENE_HALL_RESULT, 		S4SCENE_HALL_RESULT,	S4CAM_RESULT00,	  30,  120) );	//!< リザルト戦艦
	mDoor.push_back( APP_NEW BGDoorStg04(getModelSet("Obj_St04_Door.mdl",3), S4SCENE_HALL_RESULT, 		S4SCENE_HALL_RESULT,	S4CAM_RESULT00,	  60,  120) );	//!< リザルト戦艦
	
	//!	シーン遷移
	{//!	通路	
		BGSceneChange::cSet	set;
		//!	シーン
		set.addScene( BGSceneChange::cSceneTable(  240, S4SCENE_ENTRANCE) );
		set.addScene( BGSceneChange::cSceneTable( 1460, S4SCENE_STRAIGHT_VERTICAL) );
		set.addScene( BGSceneChange::cSceneTable( 1750, S4SCENE_3AXIS_ENTRANCE) );
		set.addScene( BGSceneChange::cSceneTable( 2320, S4SCENE_3AXIS) );
		set.addScene( BGSceneChange::cSceneTable( 2440, S4SCENE_STRAIGHT_SIDE) );
		set.addScene( BGSceneChange::cSceneTable( 3020, S4SCENE_CURVE_SIDE) );
		set.addScene( BGSceneChange::cSceneTable( 3200, S4SCENE_STRAIGHT_SIDE2) );
		set.addScene( BGSceneChange::cSceneTable( 3800, S4SCENE_CURVE_SIDE2) );
		set.addScene( BGSceneChange::cSceneTable( 4800, S4SCENE_STRAIGHT_SIDE3) );
		set.addScene( BGSceneChange::cSceneTable(    0, S4SCENE_STRAIGHT_END) );
		//!	シーンパラメータ
//		set.addParam( BGSceneChange::cParamTable(   -1,   0, S4SP_ENTRANCE) );	//!< 入口
//		set.addParam( BGSceneChange::cParamTable(    0,   0, S4SP_ROUTE_F) );	//!< 通路 正面
		set.addParam( BGSceneChange::cParamTable( 4900,1200, S4SP_ROUTE_V) );	//!< 通路 縦
		set.addParam( BGSceneChange::cParamTable(    0, 300, S4SP_HALL) );		//!< 縦穴
		mSceneChange.getSet()[S4CAM_ROAD] = set;
	}
	{//!	縦穴	
		BGSceneChange::cSet	set;
		//!	シーン
		set.addScene( BGSceneChange::cSceneTable(  500, S4SCENE_STRAIGHT_END) );
		set.addScene( BGSceneChange::cSceneTable( 1810, S4SCENE_HALL_ENTRANCE) );
		set.addScene( BGSceneChange::cSceneTable( 4010, S4SCENE_HALL_SIDE) );
		set.addScene( BGSceneChange::cSceneTable( 6460, S4SCENE_HALL_VERTICAL) );
		set.addScene( BGSceneChange::cSceneTable(    0, S4SCENE_HALL_BOSS) );
		//!	シーンパラメータ
		set.addParam( BGSceneChange::cParamTable( 7100, 300, S4SP_HALL) );	//!< 縦穴
		set.addParam( BGSceneChange::cParamTable(    0, 200, S4SP_BOSS) );	//!< ボス戦
		mSceneChange.getSet()[S4CAM_HALL] = set;
	}
	{//!	ボス戦	
		BGSceneChange::cSet	set;
		//!	シーン
		set.addScene( BGSceneChange::cSceneTable(    0, S4SCENE_HALL_BOSS) );
		//!	シーンパラメータ
		set.addParam( BGSceneChange::cParamTable(    0,   0, S4SP_BOSS) );	//!< ボス戦
		mSceneChange.getSet()[S4CAM_BOSS] = set;
	}

	//!	更新関数登録
	addUpdateEvent( updatePointLightCB,			0, S4SCENE_STRAIGHT_END );	//!< 点光源
	addUpdateEvent( updateDoorCloseCB,			S4SCENE_STRAIGHT_END,		S4SCENE_STRAIGHT_END );			//!< ドア閉める
	addUpdateEvent( updateLensflareEntranceCB,	S4SCENE_STRAIGHT_VERTICAL,	S4SCENE_STRAIGHT_VERTICAL );	//!< レンズフレア 入口
	addUpdateEvent( updateLensflare3AxisCB,		S4SCENE_3AXIS_ENTRANCE,		S4SCENE_STRAIGHT_SIDE );		//!< レンズフレア ３軸通路
	addUpdateEvent( updateLensflare3AxisDCB,	S4SCENE_3AXIS,				S4SCENE_STRAIGHT_SIDE );		//!< レンズフレア ３軸通路下
	addUpdateEvent( updateLensflareHallCB,		S4SCENE_STRAIGHT_END,		S4SCENE_HALL_BOSS );			//!< レンズフレア 縦穴
	addUpdateEvent( updateLensflareHall2CB,		S4SCENE_HALL_ENTRANCE,		S4SCENE_HALL_BOSS );			//!< レンズフレア 縦穴２
	addUpdateEvent( NULL,						S4SCENE_HALL_RESULT,		S4SCENE_HALL_RESULT,	setupLensflareResultCB, finalLensflareResultCB );	//!< レンズフレア リザルト
	addUpdateEvent( NULL,						S4SCENE_HALL_RESULT2,		S4SCENE_HALL_RESULT2,	setupLensflareResult2CB, finalLensflareResult2CB );	//!< レンズフレア リザルト２
	addUpdateEvent( NULL,						S4SCENE_HALL_RESULT3,		S4SCENE_HALL_RESULT3,	setupLensflareResultCB, finalLensflareResultCB );	//!< レンズフレア リザルト

	return true;
}

// ---------------------------------------------
//! @brief  UVアニメ設定
// ---------------------------------------------
void tBGStg04::setModelUvAnim()
{
	cBGModel*	pModel;
	
	//!	エネルギーパイプUV
	const char*	_DRIVE[] = {
		"St04_Entrance.mdl",
		"St04_RoadLoop_A.mdl",
		"St04_RoadLoop_B.mdl",
		"St04_RoadT.mdl",
		"St04_RoadL.mdl",
		"St04_Road3axis.mdl",
		"St04_BossEntrance.mdl",
		"St04_BossLoop.mdl",
	};
	for( u8 i=0 ; i<numberof(_DRIVE) ; i++ ){
		pModel = getModel( _DRIVE[i] );
		if( pModel ){
			pModel->SetNodeUVAnime( "ENP_EFFCenterA_add", m01_UvA );
			pModel->SetNodeUVAnime( "ENP_EFFCenterB_add", m01_UvB );
			pModel->SetNodeUVAnime( "ENP_EFFCenterC_add", m01_UvC );
			pModel->SetNodeUVAnime( "ENP_EFFCenterD_add", m01_UvD );
		}
	}

	//!	ボス箱モーション
	pModel = getModel( "Obj_St04_ENPBoss_Cube.mdl" );
	if( pModel ){
		pModel->getAnim()->Play( 0, true, false );
		pModel->getAnim()->SetAnimationSpeed( mCubeSpd );
	}
	//!	ボスユニットモーション
	pModel = getModel( "St04_Boss_Unit_Roll.mdl" );
	if( pModel ){
		pModel->getAnim()->Play( 0, true, false );
		pModel->getAnim()->SetAnimationSpeed( mBossUnitRollSpd );
	}
}

// ---------------------------------------------
//! @brief  エフェクト開始
// ---------------------------------------------
void tBGStg04::beginEffect()
{
	//!	レンズフレア 入口
	setLensflare( "St04_Entrance.mdl", "LensFlare_LOC", TGL::Vec3::ZERO, "Lensflare_Entrance" );
}

// ---------------------------------------------
//! @brief  レンズフレア設定
//! @param  strModel	[in]	アタッチモデル
//! @param  strJoint	[in]	アタッチジョイント "":無効
//! @param  vOfs		[in]	オフセット座標
//! @param  strObj		[in]	パラメータ名
//! @param  nEft		[in]	エフェクト
//! @param  nSE			[in]	SE
//! @param  uIdx		[in]	配列ID
// ---------------------------------------------
void tBGStg04::setLensflare( const stl::string& strModel, const stl::string& strJoint, const TGL::Vec3& vOfs, const stl::string& strObj, s16 nEft/*=-1*/, s16 nSE/*=-1*/, u8 uIdx/*=0*/ )
{
	//!	基準モデル
	cModelSet*	pModelSet = getModelSet( strModel );
	if( !pModelSet )	return;
	
	//!	座標計算
	TGL::Matrix	mtx;
	if( strJoint.empty() ){	//!< モデル原点
		mtx = pModelSet->getWorldMtx();
	}else{	//!< ジョイント
		if( getModel(strModel) ){
			mtx = *getModel(strModel)->GetBoneWorldMatrix( strJoint.c_str() ) * pModelSet->getWorldMtx();
		}
	}
	TGL::Vec3	vPos( mtx.getW() + getOrigin() + vOfs );	//!< ステージ原点を加算

	mLensflare[uIdx]->setAttach( TGL::MtxGuide(vPos) );
	mLensflare[uIdx]->setEnable( true );

	//!	レンズフレアパラメータ
	TGL::JsonUtil	json( _PARAM_EX );
	TGL::Json::array	jsonArray = json.getArray( strObj );
	foreach( TGL::Json::array, jsonArray, it ){
		TGL::Json::object&	work = it->get<TGL::Json::object>();
		mLensflare[uIdx]->setupFromJson( work );
	}
	
	//!	エフェクト
	if( nEft >= 0 )
		IAppEffect->req( nEft, vPos, TGL::Vec3::ZERO, NULL, NULL, NULL, NULL, 0, 0, &mEftLink[uIdx+EFTLINK_LENSFLARE_A] );

	//!	SE
	mpSEENP[uIdx] = IAppSound->play( SDID_ONMEM, nSE, TGL::SoundChannel::PLAYF_LOOP | TGL::SoundChannel::PLAYF_RANDSTART );
	if( mpSEENP[uIdx] ){
		if( nSE == SE_S4_ENPLIGHTLOOP )
			mpSEENP[uIdx]->setField( vPos, 10000.0f, 0.3f, 1 );
		else
			mpSEENP[uIdx]->setField( vPos, 15000.0f, 0.1f, 1 );
		mpSEENP[uIdx]->setVolume( 0.01f, 1.0f, 15 );
		mpSEENP[uIdx]->setLoopKeep( 180 );
		mLoopKeepSE.push_back( mpSEENP[uIdx] );
	}
}

// ---------------------------------------------
//! @brief  更新
// ---------------------------------------------
void tBGStg04::update()
{
	//!	親クラス
	tBG::update();
	
	//!	シーン遷移
	mSceneChange.update( *this );
	
	//!	放電
	foreach( stl::vector<Elect*>, mElect, it )
		(*it)->update( *this );

	//!	ドア
	foreach( stl::vector<BGDoorStg04*>, mDoor, it )
		(*it)->update( *this );

	//!	ボス破片
	for( stl::vector<BossPiece*>::iterator it=mBossPiece.begin() ; it!=mBossPiece.end() ; ){
		if( (*it)->mbEnd ){
			SAFE_DELETE( *it );
			it = mBossPiece.erase( it );
		}else{
			(*it)->update( *this );
			++it;
		}
	}
}

// ---------------------------------------------
//! @brief  スクリプトメッセージ受け取り
//! @param  nId		[in]	メッセージID
//! @param  nParam	[in]	パラメータ
// ---------------------------------------------
void tBGStg04::receiveMessage( s32 nId, s32 nParam )
{
	const TGL::Vec3	_BOSS_POS( 120000, -190000, 430000 );
	
	switch( nId ){
		case S4PRMSG_BOSSUNITROLL:	//!	ボス台座回す
		{
			cBGModel*	pModel = getModel( "St04_Boss_Unit_Roll.mdl" );
			if( pModel )
				pModel->getAnim()->Play( 0, true, true );
		}	break;

		case S4PRMSG_CORESHINE_READY:	//!	ボス前光
			IAppEffect->req( EFTNO_PLANETCORE_ENTRYSHINE, _BOSS_POS, TGL::Vec3::ZERO, NULL, NULL, NULL, NULL, 0, 0, &mEftLink[EFTLINK_CORESHINE] );
			mpSEBossLight = IAppSound->play( SDID_ONMEM, SE_S4_ENPLIGHTLOOP, TGL::SoundChannel::PLAYF_LOOP );
			if( mpSEBossLight ){
				mpSEBossLight->setField( _BOSS_POS, 5000.0f, 0.5f, 1 );
				mpSEBossLight->setLoopKeep( 60 );
				mLoopKeepSE.push_back( mpSEBossLight );
			}
			break;

		case S4PRMSG_CORESHINE:	//!	ボス光
			IAppEffect->req( EFTNO_PLANETCORE_SHINEBG, _BOSS_POS, TGL::Vec3::ZERO, NULL, NULL, NULL, NULL, 0, 0, &mEftLink[EFTLINK_CORESHINEBG] );
			break;

		case S4PRMSG_CORESHINE_END:	//!	ボス光終わり
			EEMIT_End( &mEftLink[EFTLINK_CORESHINE] );
			EEMIT_End( &mEftLink[EFTLINK_CORESHINEBG] );
			eraseLoopKeepSE( mpSEBossLight );
			mpSEBossLight = NULL;
			break;

		case S4PRMSG_BOSSPIECE:	//!	ボス破片
		{
			tCharacter*	pBoss = ICharacter->getBoss();
			if( !pBoss )	break;
			for( u8 i=0 ; i<9 ; i++ ){
				mBossPiece.push_back( APP_NEW BossPiece(i%3, pBoss->getPos()) );
			}
		}	break;

		case S4PRMSG_RESULTSMOKE:	//!	リザルト煙
		{
			const TGL::Vec3	_POS( 120000, -199000, 430000 );
			IAppEffect->req( EFTNO_SCENES4_SMOKEZONE, _POS, TGL::Vec3::ZERO, NULL, NULL, NULL, NULL, 0, 0, &mEftLink[EFTLINK_RESULTSMOKE] );
		}	break;

		case S4PRMSG_RESULTSMOKE2:	//!	リザルト煙２
		{
			const TGL::Vec3	_POS( 120000, -199000, 430000 );
			EEMIT_End( &mEftLink[EFTLINK_RESULTSMOKE] );
			IAppEffect->req( EFTNO_SCENES4_SMOKEZONE2, _POS, TGL::Vec3::ZERO, NULL, NULL, NULL, NULL, 0, 0, &mEftLink[EFTLINK_RESULTSMOKE] );
		}	break;
	}
}

// ---------------------------------------------
//! @brief  更新 点光源
//! @param  bSceneChg	[in]	true:シーン変更瞬間
// ---------------------------------------------
void tBGStg04::updatePointLight( bool bSceneChg )
{
	cSceneParam::LightPoint*	pLight = mSceneParam.getPointLight(0);
	if( !pLight )	return;
	pLight->mPos = ICamera->getCamera(0)->getTar();
}

// ---------------------------------------------
//! @brief  更新 レンズフレア 入口
//! @param  bSceneChg	[in]	true:シーン変更瞬間
// ---------------------------------------------
void tBGStg04::updateLensflareEntrance( bool bSceneChg )
{
	mLensflare[0]->setEnable( false );
}

// ---------------------------------------------
//! @brief  更新 レンズフレア ３軸通路
//! @param  bSceneChg	[in]	true:シーン変更瞬間
// ---------------------------------------------
void tBGStg04::updateLensflare3Axis( bool bSceneChg )
{
	const u8	_IDX = 0;	//!< 使用ID
	if( !bSceneChg )	return;
	
	if( mScene == S4SCENE_3AXIS_ENTRANCE ){	//!< 発生
		EEMIT_End( &mEftLink[EFTLINK_LENSFLARE_A] );
		setLensflare( "St04_Road3axis.mdl", "", TGL::Vec3::ZERO, "Lensflare_3Axis", EFTNO_SCENES4_ENPLIGHT, SE_S4_ENPLIGHTLOOP, _IDX );
	}
	if( mScene == S4SCENE_STRAIGHT_SIDE ){	//!< 終了
		mLensflare[_IDX]->setEnable( false );
		EEMIT_End( &mEftLink[EFTLINK_LENSFLARE_A] );
		eraseLoopKeepSE( mpSEENP[_IDX] );
		mpSEENP[_IDX] = NULL;
	}
}

// ---------------------------------------------
//! @brief  更新 レンズフレア ３軸通路下
//! @param  bSceneChg	[in]	true:シーン変更瞬間
// ---------------------------------------------
void tBGStg04::updateLensflare3AxisD( bool bSceneChg )
{
	const u8	_IDX = 1;	//!< 使用ID
	if( !bSceneChg )	return;
	
	if( mScene == S4SCENE_3AXIS ){	//!< 発生
		EEMIT_End( &mEftLink[EFTLINK_LENSFLARE_B] );
		setLensflare( "St04_Road3axis.mdl", "", TGL::Vec3(0,-40000,0), "Lensflare_3Axis", EFTNO_SCENES4_ENPLIGHT, SE_S4_ENPLIGHTLOOP, _IDX );
	}
	if( mScene == S4SCENE_STRAIGHT_SIDE ){	//!< 終了
		mLensflare[_IDX]->setEnable( false );
		EEMIT_End( &mEftLink[EFTLINK_LENSFLARE_B] );
		eraseLoopKeepSE( mpSEENP[_IDX] );
		mpSEENP[_IDX] = NULL;
	}
}

// ---------------------------------------------
//! @brief  更新 レンズフレア 縦穴
//! @param  bSceneChg	[in]	true:シーン変更瞬間
// ---------------------------------------------
void tBGStg04::updateLensflareHall( bool bSceneChg )
{
	const u8	_IDX = 0;	//!< 使用ID
	if( !bSceneChg )	return;

	switch( mScene ){
		case S4SCENE_STRAIGHT_END:	//!	発生
			EEMIT_End( &mEftLink[EFTLINK_LENSFLARE_A] );
			setLensflare( "St04_BossEntrance.mdl", "", TGL::Vec3(0,0,30000), "Lensflare_Hall", EFTNO_SCENES4_ENPLIGHT2, SE_S4_ENPLIGHT2LOOP, _IDX );
			break;
		case S4SCENE_HALL_SIDE:	//!	発生
			EEMIT_End( &mEftLink[EFTLINK_LENSFLARE_A] );
			setLensflare( "St04_BossLoop.mdl", "", TGL::Vec3(0,-50000,30000), "Lensflare_Hall", EFTNO_SCENES4_ENPLIGHT2, SE_S4_ENPLIGHT2LOOP, _IDX );

			//!	SE
			for( u8 i=0 ; i<2 ; i++ ){
				const TGL::Vec3	_OFS[] = {
					TGL::Vec3(    0,-50000,0),
					TGL::Vec3(30000,-50000,30000),
				};
				mpSEENPSub[ENPSESUB_DA+i] = reqSE_ENPSub( _OFS[i] );
				if( mpSEENPSub[ENPSESUB_DA+i] )
					mLoopKeepSE.push_back( mpSEENPSub[ENPSESUB_DA+i] );
			}
			break;
		case S4SCENE_HALL_BOSS:	//!	終了
			mLensflare[_IDX]->setEnable( false );
			EEMIT_End( &mEftLink[EFTLINK_LENSFLARE_A] );
			eraseLoopKeepSE( mpSEENP[_IDX] );
			eraseLoopKeepSE( mpSEENPSub[ENPSESUB_DA] );
			eraseLoopKeepSE( mpSEENPSub[ENPSESUB_DB] );
			mpSEENP[_IDX] = NULL;
			mpSEENPSub[ENPSESUB_DA] = mpSEENPSub[ENPSESUB_DB] = NULL;
			break;
	}
}

// ---------------------------------------------
//! @brief  更新 レンズフレア 縦穴２
//! @param  bSceneChg	[in]	true:シーン変更瞬間
// ---------------------------------------------
void tBGStg04::updateLensflareHall2( bool bSceneChg )
{
	const u8	_IDX = 1;	//!< 使用ID

	TGL::PathCamera*	pCam = (TGL::PathCamera*)( ICamera->getCamera(0) );
	if( pCam && pCam->isPlay() && pCam->getTime() > 3600 ){
		eraseLoopKeepSE( mpSEENPSub[ENPSESUB_UA] );
		eraseLoopKeepSE( mpSEENPSub[ENPSESUB_UB] );
		mpSEENPSub[ENPSESUB_UA] = mpSEENPSub[ENPSESUB_UB] = NULL;
	}

	if( !bSceneChg )	return;
	
	if( mScene == S4SCENE_HALL_ENTRANCE ){	//!< 発生
		EEMIT_End( &mEftLink[EFTLINK_LENSFLARE_B] );
		setLensflare( "St04_BossLoop.mdl", "", TGL::Vec3(0,0,30000), "Lensflare_Hall", EFTNO_SCENES4_ENPLIGHT2, SE_S4_ENPLIGHT2LOOP, _IDX );

		//!	SE
		for( u8 i=0 ; i<2 ; i++ ){
			const TGL::Vec3	_OFS[] = {
				TGL::Vec3(-18000,0,48000),
				TGL::Vec3(-18000,0,12000),
			};
			mpSEENPSub[ENPSESUB_UA+i] = reqSE_ENPSub( _OFS[i], 0.6f );
			if( mpSEENPSub[ENPSESUB_UA+i] )
				mLoopKeepSE.push_back( mpSEENPSub[ENPSESUB_UA+i] );
		}
	}
	if( mScene == S4SCENE_HALL_BOSS ){	//!< 終了
		mLensflare[_IDX]->setEnable( false );
		EEMIT_End( &mEftLink[EFTLINK_LENSFLARE_B] );
		eraseLoopKeepSE( mpSEENP[_IDX] );
		eraseLoopKeepSE( mpSEENPSub[ENPSESUB_UA] );
		eraseLoopKeepSE( mpSEENPSub[ENPSESUB_UB] );
		mpSEENP[_IDX] = NULL;
		mpSEENPSub[ENPSESUB_UA] = mpSEENPSub[ENPSESUB_UB] = NULL;
	}
}

// ---------------------------------------------
//! @brief  セットアップ レンズフレア リザルト
// ---------------------------------------------
void tBGStg04::setupLensflareResult()
{
	EEMIT_End( &mEftLink[EFTLINK_LENSFLARE_A] );
	EEMIT_End( &mEftLink[EFTLINK_LENSFLARE_B] );
	setLensflare( "St04_BossLoop.mdl", "", TGL::Vec3(0,     0,30000), "Lensflare_Hall", EFTNO_SCENES4_ENPLIGHT2, -1, 0 );
	setLensflare( "St04_BossLoop.mdl", "", TGL::Vec3(0,-50000,30000), "Lensflare_Hall", EFTNO_SCENES4_ENPLIGHT2, -1, 1 );
}
// ---------------------------------------------
//! @brief  終了 レンズフレア リザルト
// ---------------------------------------------
void tBGStg04::finalLensflareResult()
{
	mLensflare[0]->setEnable( false );
	mLensflare[1]->setEnable( false );
	EEMIT_End( &mEftLink[EFTLINK_LENSFLARE_A] );
	EEMIT_End( &mEftLink[EFTLINK_LENSFLARE_B] );
}

// ---------------------------------------------
//! @brief  セットアップ レンズフレア リザルト２
// ---------------------------------------------
void tBGStg04::setupLensflareResult2()
{
	EEMIT_End( &mEftLink[EFTLINK_LENSFLARE_A] );
	EEMIT_End( &mEftLink[EFTLINK_LENSFLARE_B] );
	setLensflare( "St04_BossEntrance.mdl", "", TGL::Vec3(0,0,30000), "Lensflare_Hall", EFTNO_SCENES4_ENPLIGHT2, -1, 0 );
	setLensflare( "St04_BossLoop.mdl", "",     TGL::Vec3(0,0,30000), "Lensflare_Hall", EFTNO_SCENES4_ENPLIGHT2, -1, 1 );
}
// ---------------------------------------------
//! @brief  終了 レンズフレア リザルト２
// ---------------------------------------------
void tBGStg04::finalLensflareResult2()
{
	mLensflare[0]->setEnable( false );
	mLensflare[1]->setEnable( false );
	EEMIT_End( &mEftLink[EFTLINK_LENSFLARE_A] );
	EEMIT_End( &mEftLink[EFTLINK_LENSFLARE_B] );
}

// ---------------------------------------------
//! @brief  エネルギーバイパスサブSEリクエスト
//! @param  vOfs	[in]	オフセット座標
//! @return 再生されたチャンネル
// ---------------------------------------------
TGL::SoundChannel* tBGStg04::reqSE_ENPSub( const TGL::Vec3& vOfs, f32 fVol/*=1*/ )
{
	TGL::SoundChannel*	pCh = IAppSound->play( SDID_ONMEM, SE_S4_ENPLIGHTLOOP, TGL::SoundChannel::PLAYF_LOOP | TGL::SoundChannel::PLAYF_RANDSTART | TGL::SoundChannel::PLAYF_NOHISTORY );
	if( !pCh )	return NULL;

	cModelSet*	pModelSet = getModelSet( "St04_BossLoop.mdl" );
	if( !pModelSet )	return NULL;
	
	TGL::Matrix	mtx = pModelSet->getWorldMtx();
	TGL::Vec3	vPos( mtx.getW() + getOrigin() + vOfs );
	pCh->setField( vPos, 2500.0f, 0.3f, 1 );
	pCh->setVolume( 0.01f, fVol, 15 );
	pCh->setLoopKeep( 60 );
	return pCh;
}

// ---------------------------------------------
//! @brief  放電 更新
//! @param  Bg	[in]	背景タスク
// ---------------------------------------------
void tBGStg04::Elect::update( const tBGStg04& Bg )
{
	TGL::PathCamera*	pCam = (TGL::PathCamera*)( ICamera->getCamera(0) );
	if( !pCam || !pCam->isPlay() )	return;
	
	//!	終了
	if( (pCam->getCurrentGroupId() != mCamGroup)
		|| (mEndTime > 0.0f && pCam->getTime() > mEndTime) ){
		reset();
		return;
	}
	
	//!	発生
	if( !mbActive ){
		TGL::MtxGuide	guide( &mMtx );
		IAppEffect->req( mEftId, TGL::Vec3::ZERO, TGL::Vec3::ZERO, NULL, &guide, NULL, NULL, 0, 0, &mEftLink );
		mbActive = true;
	}

	//!	追従マトリクス更新
	foreach( stl::vector<ElectTime>, mTime, it ){
		if( (it != mTime.end()-1) && (pCam->getTime() >= (*it).mTime) )	continue;
		//!	モデル座標
		TGL::Matrix	mtxModel( (*it).mpModelSet->getWorldMtx() );
		TGL::Vec3	vModel( mtxModel.getW() );
		
		//!	ランダムオフセット
		f32	fRand = RANDF2( (*it).mMin, (*it).mMax );
		
		//!	座標計算
		TGL::Vec3	vPos( TGL::Vec3::ZERO );
		TGL::Vec3	v0( TGL::Vec3::ZERO );
		switch( (*it).mType ){
			case ElectTime::TYPE_CAM_X:	//!	カメラX
				vPos = TGL::Vec3( fRand + pCam->getPos().x, vModel.y, vModel.z );
				break;
			case ElectTime::TYPE_CAM_Y:	//!	カメラY
				vPos = TGL::Vec3( vModel.x, fRand + pCam->getPos().y, vModel.z );
				break;
			case ElectTime::TYPE_CAM_Z:	//!	カメラZ
				vPos = TGL::Vec3( vModel.x, vModel.y, fRand + pCam->getPos().z );
				break;
			case ElectTime::TYPE_MODEL_X:	//!	モデルX
				mtxModel.rollY( (*it).mRot );
				v0 = TGL::Vec3(fRand,0,0).applySR( mtxModel );
				vPos = vModel + v0;
				break;
			case ElectTime::TYPE_MODEL_Y:	//!	モデルY
				mtxModel.rollY( (*it).mRot );
				v0 = TGL::Vec3(0,fRand,0).applySR( mtxModel );
				vPos = vModel + v0;
				break;
			case ElectTime::TYPE_MODEL_Z:	//!	モデルZ
				mtxModel.rollY( (*it).mRot );
				v0 = TGL::Vec3(0,0,fRand).applySR( mtxModel );
				vPos = vModel + v0;
				break;
			case ElectTime::TYPE_RAND:	//!	ランダム軸
				vPos = vModel;
				vPos.addIndex( RANDI2(TGL_X, TGL_Z), fRand );
				break;
		}
		vPos += (*it).mOfs;
		
		//!	反映
		mMtx.setW( vPos );
		break;
	}
}

// ---------------------------------------------
//! @brief  更新 ドア閉める
//! @param  bSceneChg	[in]	true:シーン変更瞬間
// ---------------------------------------------
void tBGStg04::updateDoorClose( bool bSceneChg )
{
	TGL::PathCamera*	pCam = (TGL::PathCamera*)( ICamera->getCamera(0) );
	if( pCam->getCurrentGroupId() < 1 )	return;
	if( pCam->getTimeOld() < 200 )	return;
	if( pCam->getTime() >= 200 ){
		getModel( "Obj_St04_Door.mdl", 0 )->getAnim()->Play( 0, false, false );
		getModel( "Obj_St04_Door.mdl", 1 )->getAnim()->Play( 0, false, false );
	}
}


// ---------------------------------------------
//! @brief  コンストラクタ "BossPiece"
//! @param  uMesh	[in]	メッシュID
//! @param  vPos	[in]	座標
// ---------------------------------------------
tBGStg04::BossPiece::BossPiece( u8 uMesh, const TGL::Vec3& vPos )
:	mbEnd(false), mTime(0), mMeshId(uMesh),
	mRot( TGL::Vec3(RANDF2(0,360), RANDF2(0,360), RANDF2(0,360)) ),
	mMove( TGL::Vec3(RANDF2(-5,5), RANDF2(-5,5), RANDF2(-5,5)) ),
	mRoll( TGL::Vec3(RANDF2(-1,1), RANDF2(-1,1), RANDF2(-1,1)) )
{
	mpModel = APP_NEW TGL::Model( IResourceManager->getResourceBinary<TGL::ModelResource>("Obj_St04_BossShellPiece.mdl", TGL::Resource::TYPE_MODEL)->Get() );
	mpModel->SetLightBit( (1<<1) );	//!< ライト設定
	mpModel->SetFogId( cSceneParam::Data::FOG_CHR );	//!< フォグ設定
//	mpModel->SetScale( 1.5f );
	//!	表示メッシュ
	mpModel->SetNodeVisible( "St04_BossShellPiece_1", mMeshId == MESH_A );
	mpModel->SetNodeVisible( "St04_BossShellPiece_2", mMeshId == MESH_B );
	mpModel->SetNodeVisible( "St04_BossShellPiece_3", mMeshId == MESH_C );

	//!	座標
	mPos = vPos;
#if 1
	TGL::Vec3	v0;
	v0 = TGL::Vec3::rand();
	mPos += v0 * RANDF2( 200, 200 );
#else	
	TGL::Vec3	v0 = ICamera->getCamera(0)->getEyeVector();
	mPos += v0 * -1000.0f;
	
	v0 = TGL::Vec3::rand();
	mPos += v0 * RANDF2( 0, 1000 );
#endif
}

// ---------------------------------------------
//! @brief  デストラクタ "BossPiece"
// ---------------------------------------------
tBGStg04::BossPiece::~BossPiece()
{
	SAFE_DELETE( mpModel );
}

// ---------------------------------------------
//! @brief  更新
// ---------------------------------------------
void tBGStg04::BossPiece::update( tBG& Bg )
{
	mPos += mMove * Bg.getDeltaTime();
	mRot += mRoll * Bg.getDeltaTime();
	mMove.y -= 0.20f;
	
	mpModel->SetModelAlpha( getAlpha() );
	
#if 0
	//!	簡易衝突判定
	cModelSet*	pModel = Bg.getModelSet( "St04_Boss_Unit_Roll.mdl" );
	if( pModel ){
		TGL::Vec3	v0 = mPos;
		TGL::Vec3	v1 = pModel->getWorldMtx().getW() + Bg.getOrigin();
		TGL::Vec3	vWay = v0 - v1;
		
		const f32	BG_RADIUS = 2500.0f;
		const f32	MODEL_RADIUS = 120.0f;
		if( vWay.length() > BG_RADIUS-MODEL_RADIUS ){
			if( mMove.length() >= 1.0f ){
				vWay = vWay.normalize();
				IAppEffect->reqWay( EFTNO_SCENES5_DOORPIECE_HIT, v0, -vWay );
			}else{
				mRoll *= 0.5f;
			}
			mMove *= -0.5f;
		}
		mPos.x = TGL::Math::Clamp<f32>( mPos.x, v1.x-BG_RADIUS, v1.x+BG_RADIUS );
		mPos.y = TGL::Math::Clamp<f32>( mPos.y, v1.y-BG_RADIUS, v1.y+BG_RADIUS );
		mPos.z = TGL::Math::Clamp<f32>( mPos.z, v1.z-BG_RADIUS, v1.z+BG_RADIUS );
	}
#endif

	//!	モデル更新
	TGL::Matrix	mtxWorld = TGL::Matrix::UNIT;
	mtxWorld.rollYPR( mRot );	//!< 回転
	mtxWorld.setW( mPos );		//!< 座標
	
	mpModel->Update( mtxWorld );
	
	//!	描画リスト登録
	if( !IACtrl->getScene().tstSceneF(SCENEF_NORENDER) ){
		ITGLRenderer->addDrawList( TGL::Renderer::DPASS_MAIN, GPRI_CHR(0), Bg.getTskHead().mTot, ICamera->getMainDrawFlag(), drawlistExec, (void*)this );
	}
	
	//!	終了
	mTime += Bg.getDeltaTime();
	if( mTime >= LIFE )
		mbEnd = true;
}

/* ========================================================================= */
/*		デバッグ															 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  デバッグ情報表示
// ---------------------------------------------
void tBGStg04::dispDebugInfo()
{
#ifndef _TGL_FINAL
	if( !mbDebugInfo )	return;
	
	//!	親
	tBG::dispDebugInfo();
	
#endif	/*	_TGL_FINAL	*/
}
