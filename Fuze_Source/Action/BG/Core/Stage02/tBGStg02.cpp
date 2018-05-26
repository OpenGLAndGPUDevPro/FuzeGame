// -------------------------------------------------------------------------- //
//! @file   tBGStg02.cpp
//! @brief  背景 １面
//! @author Nal
//! @since  12/07/06(金)
//! 
//! COPYRIGHT (C) 2012 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
/* ====< インクルード >====================================================== */
#include "prefix.h"
#include <TGL.h>
#include "Game/Game.h"
#include "Action/Camera/cBGCamera.h"
#include "tBGStg02.h"
#include "Action/ActionTypes.h"
#include "Effect/sAppEffect.h"
#include "Sound/sAppSound.h"

#ifdef _WINDOWS
#pragma warning(disable : 4355)
#endif
/* ========================================================================= */
//!		tBGStg02															 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  コンストラクタ "tBGStg02"
// ---------------------------------------------
tBGStg02::tBGStg02()
:	tBG(),
	mFar01_Uv(TGL::Vec2(0,0)), mFar02_UvA(TGL::Vec2(0,0)), mFar02_UvB(TGL::Vec2(0,0)), 
	mLoop01_UvA(TGL::Vec2(0,0)), mLoop01_UvB(TGL::Vec2(0,0)), mLoop01_UvC(TGL::Vec2(0,0)), mLoop01_UvD(TGL::Vec2(0,0)),
	m02_UvA(TGL::Vec2(0,0)), m02_UvB(TGL::Vec2(0,0)), m02_UvC(TGL::Vec2(0,0)), m02_UvD(TGL::Vec2(0,0)),
	m04_UvA(TGL::Vec2(0,0)), m04_UvB(TGL::Vec2(0,0)), m04_UvC(TGL::Vec2(0,0)), m04_UvD(TGL::Vec2(0,0)),
	m04_UvA2(TGL::Vec2(0,0)), m04_UvB2(TGL::Vec2(0,0)), m04_UvC2(TGL::Vec2(0,0)), m04_UvD2(TGL::Vec2(0,0)), m04_UvFar(TGL::Vec2(0,0)), m04_UvFar2(TGL::Vec2(0,0)),
	mExEnvSize(32), mSortMeteor(*this)
{
	reset();
}

// ---------------------------------------------
//! @brief  デストラクタ "tBGStg02"
// ---------------------------------------------
tBGStg02::~tBGStg02()
{
	IAppResource->releaseTexPool( "models/BG/common/St02/St02_Post_ToneCurve_nmip.dds" );
	IAppResource->releaseTexPool( "models/BG/common/St02R/St02_Drive_envC.dds" );
	IAppResource->releaseTexPool( "models/BG/common/St02R/St02_Space_envC.dds" );
	ITGLRenderer->getFilterToneCurve()->setTexCurve( NULL );
	ITGLRenderer->getFilterToneCurve()->setEnableValue( false );
	IAppEffect->releaseExEnvironment();
	ITGLRenderer->setFBCTexEnable( false );
}

// ---------------------------------------------
//! @brief  リセット
// ---------------------------------------------
void tBGStg02::reset()
{
	tBG::reset();
}

// ---------------------------------------------
//! @brief  リソースロード
//! @param  uBG	[in]	BGID_????
//! @return true:成功, false:失敗
// ---------------------------------------------
bool tBGStg02::load( u8 uBG )
{
	//!		モデル		
	cBGModel*	pModel;
	//!	0 : 遠景 ドライブ
	pModel = addModel( "St02_Far_01.mdl", cBGModel::TYPE_SKYDOME, cBGModel::FLAG_FAROBJ );
	if( pModel )	pModel->setOnScene( cBGModel::SCENE_00 | cBGModel::SCENE_04 );
	//!	1 : 遠景 宇宙
	pModel = addModel( "St02_Far_02.mdl", cBGModel::TYPE_SKYDOME, cBGModel::FLAG_FAROBJ );
	if( pModel )	pModel->setOnScene( cBGModel::SCENE_02 | cBGModel::SCENE_03 );
	//!	2 : ドライブループ
	pModel = addModel( "St02_01_Loop.mdl", cBGModel::TYPE_SKYDOME, cBGModel::FLAG_FAROBJ );
	if( pModel )	pModel->setOnScene( cBGModel::SCENE_00 | cBGModel::SCENE_04 );
	//!	3 : ドライブ抜けつなぎ
	pModel = addModel( "St02_02.mdl" );
	if( pModel )	pModel->setOnScene( cBGModel::SCENE_01 );
	//!	4 : 隕石地帯（板用ロケータ）
	pModel = addModel( "St02_03_Loop.mdl" );
	if( pModel )	pModel->setOnScene( cBGModel::SCENE_02 | cBGModel::SCENE_03 );
	//!	5 : 隕石板
	pModel = addModel( "Obj_St02_Meteor_BB.mdl" );
	if( pModel )	pModel->setOnScene( cBGModel::SCENE_02 | cBGModel::SCENE_03 );
	//!	6 : 復帰ドライブ
	pModel = addModel( "St02_04.mdl" );
	if( pModel )	pModel->setOnScene( cBGModel::SCENE_03 );

	//!	テクスチャ
	IAppResource->loadTexPool( "models/BG/common/St02/St02_Post_ToneCurve_nmip.dds" );
	IAppResource->loadTexPool( "models/BG/common/St02R/St02_Drive_envC.dds" );
	IAppResource->loadTexPool( "models/BG/common/St02R/St02_Space_envC.dds" );

	//!		シーンパラメータ		
	mSceneParam.getFromJson( "param/BG/Stage2/Stage2_param.json" );
	
	//!		パラメータ		
	{	//!	固有パラメータ
		TGL::JsonUtil	json( "param/BG/Stage2/Stage2_paramEx.json" );
		TGL::Json::array	jsonArray = json.getArray("-");
		foreach( TGL::Json::array, jsonArray, it ){
			TGL::Json::object&	work = it->get<TGL::Json::object>();
			mFar01_Uv		= TGL::Vec2( (f32)atof( work["Far01_Speed_U"].to_str().c_str() ),
									(f32)atof( work["Far01_Speed_V"].to_str().c_str() ) );
			mFar02_UvA		= TGL::Vec2( (f32)atof( work["Far02_A_Speed_U"].to_str().c_str() ),
									(f32)atof( work["Far02_A_Speed_V"].to_str().c_str() ) );
			mFar02_UvB		= TGL::Vec2( (f32)atof( work["Far02_B_Speed_U"].to_str().c_str() ),
									(f32)atof( work["Far02_B_Speed_V"].to_str().c_str() ) );
			mLoop01_UvA		= TGL::Vec2( (f32)atof( work["Loop01_A_Speed_U"].to_str().c_str() ),
									(f32)atof( work["Loop01_A_Speed_V"].to_str().c_str() ) );
			mLoop01_UvB		= TGL::Vec2( (f32)atof( work["Loop01_B_Speed_U"].to_str().c_str() ),
									(f32)atof( work["Loop01_B_Speed_V"].to_str().c_str() ) );
			mLoop01_UvC		= TGL::Vec2( (f32)atof( work["Loop01_C_Speed_U"].to_str().c_str() ),
									(f32)atof( work["Loop01_C_Speed_V"].to_str().c_str() ) );
			mLoop01_UvD		= TGL::Vec2( (f32)atof( work["Loop01_D_Speed_U"].to_str().c_str() ),
									(f32)atof( work["Loop01_D_Speed_V"].to_str().c_str() ) );
			m02_UvA			= TGL::Vec2( (f32)atof( work["02_A_Speed_U"].to_str().c_str() ),
									(f32)atof( work["02_A_Speed_V"].to_str().c_str() ) );
			m02_UvB			= TGL::Vec2( (f32)atof( work["02_B_Speed_U"].to_str().c_str() ),
									(f32)atof( work["02_B_Speed_V"].to_str().c_str() ) );
			m02_UvC			= TGL::Vec2( (f32)atof( work["02_C_Speed_U"].to_str().c_str() ),
									(f32)atof( work["02_C_Speed_V"].to_str().c_str() ) );
			m02_UvD			= TGL::Vec2( (f32)atof( work["02_D_Speed_U"].to_str().c_str() ),
									(f32)atof( work["02_D_Speed_V"].to_str().c_str() ) );
			m04_UvA			= TGL::Vec2( (f32)atof( work["04_A_Speed_U"].to_str().c_str() ),
									(f32)atof( work["04_A_Speed_V"].to_str().c_str() ) );
			m04_UvB			= TGL::Vec2( (f32)atof( work["04_B_Speed_U"].to_str().c_str() ),
									(f32)atof( work["04_B_Speed_V"].to_str().c_str() ) );
			m04_UvC			= TGL::Vec2( (f32)atof( work["04_C_Speed_U"].to_str().c_str() ),
									(f32)atof( work["04_C_Speed_V"].to_str().c_str() ) );
			m04_UvD			= TGL::Vec2( (f32)atof( work["04_D_Speed_U"].to_str().c_str() ),
									(f32)atof( work["04_D_Speed_V"].to_str().c_str() ) );
			m04_UvA2		= TGL::Vec2( (f32)atof( work["04_A2_Speed_U"].to_str().c_str() ),
									(f32)atof( work["04_A2_Speed_V"].to_str().c_str() ) );
			m04_UvB2		= TGL::Vec2( (f32)atof( work["04_B2_Speed_U"].to_str().c_str() ),
									(f32)atof( work["04_B2_Speed_V"].to_str().c_str() ) );
			m04_UvC2		= TGL::Vec2( (f32)atof( work["04_C2_Speed_U"].to_str().c_str() ),
									(f32)atof( work["04_C2_Speed_V"].to_str().c_str() ) );
			m04_UvD2		= TGL::Vec2( (f32)atof( work["04_D2_Speed_U"].to_str().c_str() ),
									(f32)atof( work["04_D2_Speed_V"].to_str().c_str() ) );
			m04_UvFar		= TGL::Vec2( (f32)atof( work["04_Far_Speed_U"].to_str().c_str() ),
									(f32)atof( work["04_Far_Speed_V"].to_str().c_str() ) );
			m04_UvFar2		= TGL::Vec2( (f32)atof( work["04_Far2_Speed_U"].to_str().c_str() ),
									(f32)atof( work["04_Far2_Speed_V"].to_str().c_str() ) );

			mExEnvSize		= atoi( work["ExEnvironmentSize"].to_str().c_str() );

			mSceneParam.getRimLight().setupFromJson( work );
		}
	}

	//!	モデル配置
	applyModelSetFromJson( "param/BG/Stage2/Stage2_modelset.json" );
	//!	隕石板
	mSortMeteor.setBegin( mModelSet.size() );
	for( u8 i=1 ;; i++ ){
		stl::string	strLoc = stl::FormatString<stl::string>( "Meteor_BB_LOC_%02d", i );
		cModelSet*	pSet = addModelSetAssign( "Obj_St02_Meteor_BB.mdl", "St02_03_Loop.mdl", strLoc );
		if( !pSet )
			break;
		pSet->setId( i );
	}
	//!	隕石 ソート用設定
	if( mModelSet.size() > 0 ){
		mSortMeteor.setEnd( mModelSet.size() );
		mSortMeteor.setScene( cBGModel::SCENE_02 | cBGModel::SCENE_03 );
		mSortMeteor.setArray( &mModelSet );
		mSortMeteor.setSortCB( cModelSetSort::sortCB_ZFar );
	}
	
	//!	カメラパス追加
	applyCameraPathSetFromJson( "param/BG/Stage2/Stage2_camera.json" );

	//!	UVアニメ設定
	setModelUvAnim();

	//!	トーンカーブ
	ITGLRenderer->getFilterToneCurve()->setEnableValue( true );
	ITGLRenderer->getFilterToneCurve()->setTexCurve( IAppResource->getTexture("St02_Post_ToneCurve_nmip") );

	//!	特殊環境マップ
	ITGLRenderer->setFBCTexEnable( true );
	IAppEffect->createExEnvironment( mExEnvSize );
	//!	シーン環境マップ
	mSceneParam.setEnvTex( "St02_Drive_envC.dds" );
	ICharacter->overwriteBGEnvTex();	//!< キャラクターへの適用
	overwriteEnvTex();	//!< 背景モデルへの適用

	//!	更新関数登録
	addUpdateEvent( updateDriveOutCB,	2, 2 );		//!< ドライブ抜けた
	addUpdateEvent( updateDriveWatchCB,	3, -1 );	//!< 復帰ドライブ視認
	addUpdateEvent( updateDriveInCB,	4, 4 );		//!< ドライブ再突入

	return true;
}

// ---------------------------------------------
//! @brief  UVアニメ設定
// ---------------------------------------------
void tBGStg02::setModelUvAnim()
{
	cBGModel*	pModel;
	
	pModel = getModel( "St02_Far_01.mdl" );
	if( pModel )	pModel->SetNodeUVAnime( "St02_01_Star_nofog", mFar01_Uv );
	
	pModel = getModel( "St02_Far_02.mdl" );
	if( pModel ){
		pModel->SetNodeUVAnime( "St02_Far_02_SLSD_add", mFar02_UvA );
		pModel->SetNodeUVAnime( "St02_Far_02_SLSD2_add", mFar02_UvB );
	}
	
	pModel = getModel( "St02_01_Loop.mdl" );
	if( pModel ){
		pModel->SetNodeUVAnime( "SLSD_A_add", mLoop01_UvA );
		pModel->SetNodeUVAnime( "SLSD_B_add", mLoop01_UvB );
		pModel->SetNodeUVAnime( "SLSD_C_add", mLoop01_UvC );
		pModel->SetNodeUVAnime( "SLSD_D_add", mLoop01_UvD );
	}
	
	pModel = getModel( "St02_02.mdl" );
	if( pModel ){
		pModel->SetNodeUVAnime( "St02_02_SLSD_A_add", m02_UvA );
		pModel->SetNodeUVAnime( "St02_02_SLSD_B_add", m02_UvB );
		pModel->SetNodeUVAnime( "St02_02_SLSD_C_add", m02_UvC );
		pModel->SetNodeUVAnime( "St02_02_SLSD_D_add", m02_UvD );
	}
	
	pModel = getModel( "St02_04.mdl" );
	if( pModel ){
		pModel->SetNodeUVAnime( "St02_04_SLSD_A_add", m04_UvA );
		pModel->SetNodeUVAnime( "St02_04_SLSD_B_add", m04_UvB );
		pModel->SetNodeUVAnime( "St02_04_SLSD_C_add", m04_UvC );
		pModel->SetNodeUVAnime( "St02_04_SLSD_D_add", m04_UvD );
		pModel->SetNodeUVAnime( "St02_04_SLSD_A2_add", m04_UvA2 );
		pModel->SetNodeUVAnime( "St02_04_SLSD_B2_add", m04_UvB2 );
		pModel->SetNodeUVAnime( "St02_04_SLSD_C2_add", m04_UvC2 );
		pModel->SetNodeUVAnime( "St02_04_SLSD_D2_add", m04_UvD2 );
		pModel->SetNodeUVAnime( "St02_04_FarSLSD1_add", m04_UvFar );
		pModel->SetNodeUVAnime( "St02_04_FarSLSD2_add", m04_UvFar2 );
	}
}

// ---------------------------------------------
//! @brief  エフェクト開始
// ---------------------------------------------
void tBGStg02::beginEffect()
{
	//!	ドライブループSE再生
	playDriveLoopSE();

	cBGModel*	pModel = getModel( "St02_Far_02.mdl" );
	if( pModel ){
		pModel->SetNodeVisible( "St02_Far_02_SLSD_add", true );
		pModel->SetNodeVisible( "St02_Far_02_SLSD2_add", true );
	}
}

// ---------------------------------------------
//! @brief  ドライブループSE再生
// ---------------------------------------------
void tBGStg02::playDriveLoopSE()
{
	TGL::SoundChannel*	pCh = IAppSound->play( SDID_STREAM, AMBSTR_DRIVE );
	if( pCh ){
		pCh->setVolume( 0.01f, 0.5f, 60 );
		pCh->setSPan( -1.0f );
	}
}

// ---------------------------------------------
//! @brief  更新
// ---------------------------------------------
void tBGStg02::update()
{
	//!	親クラス
	tBG::update();
	
	//!	特殊環境マップの更新
	if( mScene != 2 && mScene != 3 && IAppEffect->getExEnvTex() )
		IAppEffect->getExEnvTex()->setTrigger( true );
}

// ---------------------------------------------
//! @brief  カメラ更新後の処理
// ---------------------------------------------
void tBGStg02::updateCameraAfter()
{
	tBG::updateCameraAfter();
	
	//!	隕石のソート
	mSortMeteor.sort();
}

// ---------------------------------------------
//! @brief  更新 ドライブ抜けた
//! @param  bSceneChg	[in]	true:シーン変更瞬間
// ---------------------------------------------
void tBGStg02::updateDriveOut( bool bSceneChg )
{
	if( bSceneChg ){
		//!	ドライブループSE停止
		ITGLSound->stop( 60, TGL::Sound::STOPFS_ONLY_AMBIENT );

		//!	シーン環境マップ
		mSceneParam.setEnvTex( "St02_Space_envC.dds" );
		ICharacter->overwriteBGEnvTex();	//!< キャラクターへの適用
		overwriteEnvTex();	//!< 背景モデルへの適用
	}
}

// ---------------------------------------------
//! @brief  更新 復帰ドライブ視認
//! @param  bSceneChg	[in]	true:シーン変更瞬間
// ---------------------------------------------
void tBGStg02::updateDriveWatch( bool bSceneChg )
{
	//!	遠景ドライブOFF
	if( bSceneChg ){
		cBGModel*	pModel = getModel( "St02_Far_02.mdl" );
		if( pModel ){
			pModel->SetNodeVisible( "St02_Far_02_SLSD_add", false );
			pModel->SetNodeVisible( "St02_Far_02_SLSD2_add", false );
		}
	}
}

// ---------------------------------------------
//! @brief  更新 ドライブ再突入
//! @param  bSceneChg	[in]	true:シーン変更瞬間
// ---------------------------------------------
void tBGStg02::updateDriveIn( bool bSceneChg )
{
	if( bSceneChg ){
		//!	ドライブループSE再生
		playDriveLoopSE();

		//!	シーン環境マップ
		mSceneParam.setEnvTex( "St02_Drive_envC.dds" );
		ICharacter->overwriteBGEnvTex();	//!< キャラクターへの適用
		overwriteEnvTex();	//!< 背景モデルへの適用
	}
}

/* ========================================================================= */
/*		デバッグ															 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  デバッグ情報表示
// ---------------------------------------------
void tBGStg02::dispDebugInfo()
{
#ifndef _TGL_FINAL
	if( !mbDebugInfo )	return;
	
	//!	親
	tBG::dispDebugInfo();
	
	TGL::BaseCamera*	pCam = ICamera->getCamera(0);
	if( !pCam )	return;
	
	//!	隕石ロケータ表示
	dispDebugInfoLocator( pCam, "St02_03_Loop.mdl", "Meteor_BB_LOC_" );
	
#endif	/*	_TGL_FINAL	*/
}
