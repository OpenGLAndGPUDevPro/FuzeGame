// -------------------------------------------------------------------------- //
//! @file   tBGEpilogue.cpp
//! @brief  背景 エピローグ
//! @author Nal
//! @since  13/05/14(火)
//! 
//! COPYRIGHT (C) 2013 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
/* ====< インクルード >====================================================== */
#include "prefix.h"
#include <TGL.h>
#include "Game/Game.h"
#include "Action/Camera/cBGCamera.h"
#include "tBGEpilogue.h"
#include "Action/ActionTypes.h"

/* ========================================================================= */
//!		tBGEpilogue															 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  コンストラクタ "tBGEpilogue"
// ---------------------------------------------
tBGEpilogue::tBGEpilogue()
:	tBG()
{
	reset();
}

// ---------------------------------------------
//! @brief  デストラクタ "tBGEpilogue"
// ---------------------------------------------
tBGEpilogue::~tBGEpilogue()
{
	ITGLRenderer->getFilterToneCurve()->setEnableValue( false );
}

// ---------------------------------------------
//! @brief  リセット
// ---------------------------------------------
void tBGEpilogue::reset()
{
	tBG::reset();
}

// ---------------------------------------------
//! @brief  リソースロード
//! @param  uBG	[in]	BGID_????
//! @return true:成功, false:失敗
// ---------------------------------------------
bool tBGEpilogue::load( u8 uBG )
{
/*
	//!		モデル		
	cBGModel*	pModel;
	//!	0 : 遠景 
	pModel = addModel( "St02_Far_02.mdl", cBGModel::TYPE_SKYDOME, cBGModel::FLAG_FAROBJ );
	if( pModel )	pModel->setOnScene( cBGModel::SCENE_00 );
*/
	//!	テクスチャ

	//!		シーンパラメータ		
//	mSceneParam.getFromJson( "param/BG/lucis/lucis_param.json" );
//	mSceneParam.setCurrentId( 1, 0 );
//	mSceneParam.getBlendTime().init( TGL::TransValue::MODE_LOOP, 40, TGL::TransValue::CURVE_LINEAR, 999, 0.0f, 1.0f );
	
	//!		パラメータ		

	//!	モデル配置
//	applyModelSetFromJson( "param/BG/Title/Title_modelset.json" );
	
	//!	カメラパス追加
//	applyCameraPathSetFromJson( "param/BG/Title/Title_camera.json" );

	//!	UVアニメ設定
	setModelUvAnim();

	//!	更新関数登録

	return true;
}

// ---------------------------------------------
//! @brief  UVアニメ設定
// ---------------------------------------------
void tBGEpilogue::setModelUvAnim()
{
//	cBGModel*	pModel;
	
//	pModel = getModel( "St02_Far_01.mdl" );
//	if( pModel )	pModel->SetNodeUVAnime( "St02_01_Star_nofog", TGL::Vec2(mFar01_StarSpd,0) );
}

// ---------------------------------------------
//! @brief  エフェクト開始
// ---------------------------------------------
void tBGEpilogue::beginEffect()
{
}

// ---------------------------------------------
//! @brief  更新
// ---------------------------------------------
void tBGEpilogue::update()
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
void tBGEpilogue::dispDebugInfo()
{
#ifndef _TGL_FINAL
	if( !mbDebugInfo )	return;
	
	//!	親
	tBG::dispDebugInfo();
	
	TGL::BaseCamera*	pCam = ICamera->getCamera(0);
	if( !pCam )	return;
	
#endif	/*	_TGL_FINAL	*/
}
