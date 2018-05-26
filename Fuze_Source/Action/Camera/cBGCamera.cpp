// -------------------------------------------------------------------------- //
//! @file   cBGCamera.cpp
//! @brief  背景カメラ
//! @author Nal
//! @since  11/12/07(水)
//!
//! COPYRIGHT (C) 2011 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
/* ====< インクルード >====================================================== */
#include "prefix.h"
#include <TGL.h>
#include "cBGCamera.h"
#include "Score/sScore.h"

/* ========================================================================= */
/*		BGCamera															 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  コンストラクタ "BGCamera"
// ---------------------------------------------
BGCamera::BGCamera( u32 uFlag/*=FLAGS_DEFAULT*/, TGL::RenderTex* pRT/*=NULL*/ )
:	GameCamera( uFlag, pRT )
{
	adjust();
}

// ---------------------------------------------
//! @brief  デストラクタ "BGCamera"
// ---------------------------------------------
BGCamera::~BGCamera()
{
}

// ---------------------------------------------
//! @brief  基準値に設定
// ---------------------------------------------
void BGCamera::adjust()
{
	//!	親クラス
	GameCamera::adjust();
	
	mBGCamF		= 0;
	mOrderPos	= TGL::Vec3::ZERO;
	mOrderTar	= TGL::Vec3::ZERO;
	mOrderFov	= 40.0f;
	mOrderTwist	= 0;
}

// ---------------------------------------------
//! @brief  派生クラスの更新
// ---------------------------------------------
void BGCamera::updateClass()
{
	//!	リプレイ再生中の初期ステート
	if( IScore->getReplay().isStateApply(1) ){
		setTime( IScore->getReplay().getCurrentObject()->getState(0).mCameraPath );
	}

	//!	親クラス
	GameCamera::updateClass();
}

// ---------------------------------------------
//! @brief  派生クラスの更新（フリービュー中も実行）
// ---------------------------------------------
void BGCamera::updateClass2()
{
	//!	親クラス
	GameCamera::updateClass2();

	//!	予約更新
	if( tstFLG(mBGCamF, BGCAMF_ORDERPOS) )		mPos	+= mOrderPos;	//!< 座標
	if( tstFLG(mBGCamF, BGCAMF_ORDERTAR) )		mTar	+= mOrderTar;	//!< 注視点
	if( tstFLG(mBGCamF, BGCAMF_ORDERFOV) )		mFov	+= mOrderFov;	//!< 画角
	if( tstFLG(mBGCamF, BGCAMF_ORDERTWIST) )	mTwist	+= mOrderTwist;	//!< ツイスト
	clrFLG( mBGCamF, BGCAMFS_ORDER );
}

/* ========================================================================= */
/*		EOF																	 */
/* ========================================================================= */
