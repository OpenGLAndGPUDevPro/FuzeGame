// -------------------------------------------------------------------------- //
//! @file   tBGResult.cpp
//! @brief  背景 リザルト
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
#include "tBGResult.h"
#include "Action/ActionTypes.h"

/* ========================================================================= */
//!		tBGResult															 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  コンストラクタ "tBGResult"
// ---------------------------------------------
tBGResult::tBGResult()
:	tBG()
{
	mSceneParam.setEnable( false );
	reset();
}

// ---------------------------------------------
//! @brief  デストラクタ "tBGResult"
// ---------------------------------------------
tBGResult::~tBGResult()
{
}

// ---------------------------------------------
//! @brief  リセット
// ---------------------------------------------
void tBGResult::reset()
{
	tBG::reset();
	mbAreaWait = false;
}

// ---------------------------------------------
//! @brief  リソースロード
//! @param  uBG	[in]	BGID_????
//! @return true:成功, false:失敗
// ---------------------------------------------
bool tBGResult::load( u8 uBG )
{
	//!		モデル		
//	cBGModel*	pModel;
	//!	0 : 
//	pModel = addModel( "Meteor_M.mdl" );
	
	//!		シーンパラメータ		
//	mSceneParam.getFromJson( "param/BG/stg02/stg02_param.json" );
//	mSceneParam.apply();
	
	//!		パラメータ		

	//!	モデル配置
//	applyModelSetFromJson( "param/BG/stg02/stg02_modelset.json" );
	
	//!	カメラパス追加
//	applyCameraPathSetFromJson( "param/BG/stg02/stg02_camera.json" );

	//!	UVアニメ設定

	return true;
}

// ---------------------------------------------
//! @brief  エフェクト開始
// ---------------------------------------------
void tBGResult::beginEffect()
{
}

// ---------------------------------------------
//! @brief  更新
// ---------------------------------------------
void tBGResult::update()
{
	//!	親クラス
	tBG::update();
}

