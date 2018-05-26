// -------------------------------------------------------------------------- //
//! @file   tBGTitle.cpp
//! @brief  背景 タイトル
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
#include "tBGTitle.h"
#include "Action/ActionTypes.h"

static const char*	_PARAM_EX = "param/BG/Title/Title_paramEx.json";	//!< 固有パラメータリソース

/* ========================================================================= */
//!		tBGTitle															 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  コンストラクタ "tBGTitle"
// ---------------------------------------------
tBGTitle::tBGTitle()
:	tBG(), mpLensflare(NULL)
{
	reset();
}

// ---------------------------------------------
//! @brief  デストラクタ "tBGTitle"
// ---------------------------------------------
tBGTitle::~tBGTitle()
{
	IAppResource->releaseTexPool( "models/BG/common/Title/Title_Flare01.dds" );
	IAppResource->releaseTexPool( "models/BG/common/Title/Title_Flare02.dds" );
	IAppResource->releaseTexPool( "models/BG/common/Title/Title_Flare03.dds" );
	IAppResource->releaseTexPool( "models/BG/common/Title/Title_Flare04.dds" );
	IAppResource->releaseTexPool( "models/BG/common/Title/Title_Post_ToneCurve_nmip.dds" );
	SAFE_DELETE( mpLensflare );
	ITGLRenderer->getFilterToneCurve()->setTexCurve( NULL );
	ITGLRenderer->getFilterToneCurve()->setEnableValue( false );
}

// ---------------------------------------------
//! @brief  リセット
// ---------------------------------------------
void tBGTitle::reset()
{
	tBG::reset();
}

// ---------------------------------------------
//! @brief  リソースロード
//! @param  uBG	[in]	BGID_????
//! @return true:成功, false:失敗
// ---------------------------------------------
bool tBGTitle::load( u8 uBG )
{
	//!		テクスチャ		
	IAppResource->loadTexPool( "models/BG/common/Title/Title_Flare01.dds" );
	IAppResource->loadTexPool( "models/BG/common/Title/Title_Flare02.dds" );
	IAppResource->loadTexPool( "models/BG/common/Title/Title_Flare03.dds" );
	IAppResource->loadTexPool( "models/BG/common/Title/Title_Flare04.dds" );
	IAppResource->loadTexPool( "models/BG/common/Title/Title_Post_ToneCurve_nmip.dds" );

	//!		モデル		
	cBGModel*	pModel;
	//!	0 : 遠景 
	pModel = addModel( "Title_BG_01_Far.mdl", cBGModel::TYPE_SKYDOME, cBGModel::FLAG_FAROBJ );
	if( pModel ){
		pModel->setOnScene( cBGModel::SCENE_00 );
		SAFE_DELETE( mpLensflare );
		mpLensflare = createLensFlare( TGL::MtxGuide(pModel->GetWorldMatrix(), TGL::Vec3(0, 8765, 36889)), "Title_Flare", _PARAM_EX, "Lensflare_Space" );
	}
	pModel = addModel( "Title_BG_01.mdl" );
	if( pModel )	pModel->setOnScene( cBGModel::SCENE_00 );

	//!		シーンパラメータ		
	mSceneParam.getFromJson( "param/BG/Title/Title_param.json" );
	
	//!		パラメータ		

	//!	モデル配置
	applyModelSetFromJson( "param/BG/Title/Title_modelset.json" );
	
	//!	カメラパス追加
	applyCameraPathSetFromJson( "param/BG/Title/Title_camera.json" );

	//!	UVアニメ設定
	setModelUvAnim();

	//!	トーンカーブ
	ITGLRenderer->getFilterToneCurve()->setEnableValue( true );
	ITGLRenderer->getFilterToneCurve()->setTexCurve( IAppResource->getTexture("Title_Post_ToneCurve_nmip") );

	//!	更新関数登録

	return true;
}

// ---------------------------------------------
//! @brief  UVアニメ設定
// ---------------------------------------------
void tBGTitle::setModelUvAnim()
{
//	cBGModel*	pModel;
	
//	pModel = getModel( "St02_Far_01.mdl" );
//	if( pModel )	pModel->SetNodeUVAnime( "St02_01_Star_nofog", TGL::Vec2(mFar01_StarSpd,0) );
}

// ---------------------------------------------
//! @brief  エフェクト開始
// ---------------------------------------------
void tBGTitle::beginEffect()
{
}

// ---------------------------------------------
//! @brief  更新
// ---------------------------------------------
void tBGTitle::update()
{
	//!	親クラス
	tBG::update();
}

/* ========================================================================= */
/*		デバッグ															 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  デバッグ情報表示
// ---------------------------------------------
void tBGTitle::dispDebugInfo()
{
#ifndef _TGL_FINAL
	if( !mbDebugInfo )	return;
	
	//!	親
	tBG::dispDebugInfo();
	
	TGL::BaseCamera*	pCam = ICamera->getCamera(0);
	if( !pCam )	return;
	
#endif	/*	_TGL_FINAL	*/
}
