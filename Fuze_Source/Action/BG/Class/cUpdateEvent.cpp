// -------------------------------------------------------------------------- //
//! @file   cUpdateEvent.cpp
//! @brief  更新イベント
//! @author Nal
//! @since  13/07/07(日)
//!
//! COPYRIGHT (C) 2013 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
/* ====< インクルード >====================================================== */
#include "prefix.h"
#include <TGL.h>
#include "cUpdateEvent.h"

/* ========================================================================= */
/*		cUpdateEvent														 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  コンストラクタ "cUpdateEvent"
//! @param  tBG			[in]	オーナー
//! @param  pUpdateCB	[in]	更新コールバック
//! @param  nSceneMin	[in]	更新シーン範囲
//! @param  nSceneMax	[in]	更新シーン範囲
//! @param  pSetupCB	[in]	初期化コールバック
//! @param  pFinalCB	[in]	終了コールバック
// ---------------------------------------------
cUpdateEvent::cUpdateEvent( tBG& owner, void (*pUpdateCB)(tBG*,bool), s8 nSceneMin, s8 nSceneMax, void (*pSetupCB)(tBG*), void (*pFinalCB)(tBG*) )
:	mOwner(owner), mpSetupCB(pSetupCB), mpUpdateCB(pUpdateCB), mpFinalCB(pFinalCB),
	mSceneMin(nSceneMin), mSceneMax(nSceneMax),
	mbExec(false)
{
}

// ---------------------------------------------
//! @brief  デストラクタ "cUpdateEvent"
// ---------------------------------------------
cUpdateEvent::~cUpdateEvent()
{
}

// ---------------------------------------------
//! @brief  更新イベント 実行チェック
//! @return true:実行
// ---------------------------------------------
bool cUpdateEvent::checkExec() const
{
	if( mSceneMin >= 0 && mOwner.getScene() < (u8)mSceneMin )	return false;	//!< 更新シーン範囲チェック
	if( mSceneMax >= 0 && mOwner.getScene() > (u8)mSceneMax )	return false;	//!< 更新シーン範囲チェック
	return true;
}

// ---------------------------------------------
//! @brief  更新イベント
// ---------------------------------------------
void cUpdateEvent::exec()
{
	//!	更新チェック
	if( !checkExec() ){
		if( mbExec && mpFinalCB )	//!< 終了処理
			mpFinalCB( &mOwner );
		mbExec = false;
		return;
	}
	
	//!	初期化処理
	if( !mbExec && mpSetupCB )
		mpSetupCB( &mOwner );
	
	//!	更新処理
	if( mpUpdateCB )
		mpUpdateCB( &mOwner, (mOwner.getScene()!=mOwner.getSceneOld()) );

	mbExec = true;
}

/* ========================================================================= */
/*		EOF																	 */
/* ========================================================================= */
