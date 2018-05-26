// -------------------------------------------------------------------------- //
//! @file   tBGTutorial.cpp
//! @brief  背景 チュートリアル
//! @author Nal
//! @since  13/06/01(土)
//! 
//! COPYRIGHT (C) 2013 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
/* ====< インクルード >====================================================== */
#include "prefix.h"
#include <TGL.h>
#include "Game/Game.h"
#include "Action/Camera/cBGCamera.h"
#include "tBGTutorial.h"
#include "Action/ActionTypes.h"
#include "Effect/sAppEffect.h"

/* ========================================================================= */
//!		tBGTutorial															 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  コンストラクタ "tBGTutorial"
// ---------------------------------------------
tBGTutorial::tBGTutorial()
:	tBG(), mpDigitalLine(NULL),
	mExEnvSize(32)
{
	reset();
}

// ---------------------------------------------
//! @brief  デストラクタ "tBGTutorial"
// ---------------------------------------------
tBGTutorial::~tBGTutorial()
{
	IAppResource->releaseTexPool( "models/BG/common/Tutorial/Tutorial_envC.dds" );
	IAppResource->releaseTexPool( "models/BG/common/Tutorial/ExtraGallery_envC.dds" );
	IAppResource->releaseTexPool( "models/BG/common/Tutorial/Tutorial_Post_ToneCurve_nmip.dds" );
	SAFE_DELETE( mpDigitalLine );
//	IAppEffect->releaseExEnvironment();
//	ITGLRenderer->setFBCTexEnable( false );
	ITGLRenderer->getFilterToneCurve()->setTexCurve( NULL );
	ITGLRenderer->getFilterToneCurve()->setEnableValue( false );
}

// ---------------------------------------------
//! @brief  リセット
// ---------------------------------------------
void tBGTutorial::reset()
{
	tBG::reset();
	SAFE_DELETE( mpDigitalLine );
	mLineAlpha.clear();
}

// ---------------------------------------------
//! @brief  リソースロード
//! @param  uBG	[in]	BGID_????
//! @return true:成功, false:失敗
// ---------------------------------------------
bool tBGTutorial::load( u8 uBG )
{
/*
	//!		モデル		
	cBGModel*	pModel;
	//!	0 : 遠景 
	pModel = addModel( "St02_Far_02.mdl", cBGModel::TYPE_SKYDOME, cBGModel::FLAG_FAROBJ );
	if( pModel )	pModel->setOnScene( cBGModel::SCENE_00 );
*/
	//!		テクスチャ		
	IAppResource->loadTexPool( "models/BG/common/Tutorial/Tutorial_envC.dds" );
	IAppResource->loadTexPool( "models/BG/common/Tutorial/ExtraGallery_envC.dds" );
	IAppResource->loadTexPool( "models/BG/common/Tutorial/Tutorial_Post_ToneCurve_nmip.dds" );

	//!	デジタルライン
	bool	bGauss = !ITGLSystem->isPostFilterLevelLow();
	SAFE_DELETE( mpDigitalLine );
	mpDigitalLine = APP_NEW tDigitalLine( this, GPRI_BG(0), bGauss );
	mpDigitalLine->setEnable( false );

	//!		シーンパラメータ		
	mSceneParam.getFromJson( "param/BG/tutorial/Tutorial_param.json" );
	mSceneParam.setCurrentId( 1, 0 );
	mSceneParam.getBlendTime().init( TGL::TransValue::MODE_LOOP, 40, TGL::TransValue::CURVE_LINEAR, 999, 0.0f, 1.0f );
	
	{	//!	固有パラメータ
		TGL::JsonUtil	json( "param/BG/tutorial/Tutorial_paramEx.json" );
		TGL::Json::array	jsonArray = json.getArray("-");
		foreach( TGL::Json::array, jsonArray, it ){
			TGL::Json::object&	work = it->get<TGL::Json::object>();
			mExEnvSize		= atoi( work["ExEnvironmentSize"].to_str().c_str() );

			mSceneParam.getRimLight().setupFromJson( work );
		}
	}

	//!		パラメータ		
	//!	モデル配置
//	applyModelSetFromJson( "param/BG/Title/Title_modelset.json" );
	
	//!	カメラパス追加
//	applyCameraPathSetFromJson( "param/BG/Title/Title_camera.json" );

	//!	UVアニメ設定
	setModelUvAnim();

	//!	トーンカーブ
	ITGLRenderer->getFilterToneCurve()->setEnableValue( true );
	ITGLRenderer->getFilterToneCurve()->setTexCurve( IAppResource->getTexture("Tutorial_Post_ToneCurve_nmip") );

	//!	特殊環境マップ
//	ITGLRenderer->setFBCTexEnable( true );
//	IAppEffect->createExEnvironment( mExEnvSize );
	mSceneParam.setEnvTex( "Tutorial_envC.dds" );
	ICharacter->overwriteBGEnvTex();	//!< キャラクターへの適用
	overwriteEnvTex();	//!< 背景モデルへの適用

	//!	更新関数登録
	addUpdateEvent( updateDigitalLineCB,	SCENE_TUTORIAL, -1 );		//!< ライン
	addUpdateEvent( NULL,					SCENE_EXTRAGALLERY, -1, setupExtraGalleryCB );	//!< エクストラギャラリー

	return true;
}

// ---------------------------------------------
//! @brief  UVアニメ設定
// ---------------------------------------------
void tBGTutorial::setModelUvAnim()
{
//	cBGModel*	pModel;
	
//	pModel = getModel( "St02_Far_01.mdl" );
//	if( pModel )	pModel->SetNodeUVAnime( "St02_01_Star_nofog", TGL::Vec2(mFar01_StarSpd,0) );
}

// ---------------------------------------------
//! @brief  エフェクト開始
// ---------------------------------------------
void tBGTutorial::beginEffect()
{
	mpDigitalLine->setEnable( false );
}

// ---------------------------------------------
//! @brief  更新
// ---------------------------------------------
void tBGTutorial::update()
{
	//!	親クラス
	tBG::update();
	
	//!	特殊環境マップの更新
	if( IAppEffect->getExEnvTex() )
		IAppEffect->getExEnvTex()->setTrigger( true );
}

// ---------------------------------------------
//! @brief  更新 ライン
//! @param  bSceneChg	[in]	true:シーン変更瞬間
// ---------------------------------------------
void tBGTutorial::updateDigitalLine( bool bSceneChg )
{
	if( bSceneChg ){
		mLineAlpha.init( TGL::TransValue::MODE_NORMAL, 20, TGL::TransValue::CURVE_SUB2, 999, 0.0f, 1.0f );
		mpDigitalLine->setEnable( true );
	}else
		mLineAlpha.update( getDeltaTime() );
	mpDigitalLine->setAlpha( mLineAlpha.getValue() );
}

// ---------------------------------------------
//! @brief  エクストラギャラリー
// ---------------------------------------------
void tBGTutorial::setupExtraGallery()
{
	mSceneParam.setCurrentId( 2 );	//!< シーンパラメータ
	//!	環境マップ
	mSceneParam.setEnvTex( "ExtraGallery_envC.dds" );
	ICharacter->overwriteBGEnvTex();	//!< キャラクターへの適用
	overwriteEnvTex();	//!< 背景モデルへの適用
}

/* ========================================================================= */
/*		デバッグ															 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  デバッグ情報表示
// ---------------------------------------------
void tBGTutorial::dispDebugInfo()
{
#ifndef _TGL_FINAL
	if( !mbDebugInfo )	return;
	
	//!	親
	tBG::dispDebugInfo();
	
	TGL::BaseCamera*	pCam = ICamera->getCamera(0);
	if( !pCam )	return;
	
#endif	/*	_TGL_FINAL	*/
}
