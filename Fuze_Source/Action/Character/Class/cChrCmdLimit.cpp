// -------------------------------------------------------------------------- //
//! @file   cChrCmdLimit.cpp
//! @brief  コマンド入力 移行条件
//! @author Nal
//! @since  13/02/24(日)
//!
//! COPYRIGHT (C) 2013 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
/* ====< インクルード >====================================================== */
#include "prefix.h"
#include <TGL.h>
#include "Game/Game.h"
#include "cChrCmdLimit.h"
#include "cChrCmdTypes.h"

/* ========================================================================= */
/*																			 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  フラグ比較 &
//! @param  Chr	[in]	キャラクター
//! @return true:成立, false:非成立
// ---------------------------------------------
bool cChrCmdLimit_FlagTst::check( const tCharacter& Chr ) const
{
	return ( Chr.tstFlagFromId(mFlagId, mFlag) != false );
}

// ---------------------------------------------
//! @brief  フラグ比較 !&
//! @param  Chr	[in]	キャラクター
//! @return true:成立, false:非成立
// ---------------------------------------------
bool cChrCmdLimit_FlagNTst::check( const tCharacter& Chr ) const
{
	return ( Chr.tstFlagFromId(mFlagId, mFlag) == false );
}

// ---------------------------------------------
//! @brief  フラグ比較 ==
//! @param  Chr	[in]	キャラクター
//! @return true:成立, false:非成立
// ---------------------------------------------
bool cChrCmdLimit_FlagEq::check( const tCharacter& Chr ) const
{
	return ( Chr.tstFlagFromId(mFlagId, mFlag) == mFlag );
}

// ---------------------------------------------
//! @brief  アニメ番号比較 ==
//! @param  Chr	[in]	キャラクター
//! @return true:成立, false:非成立
// ---------------------------------------------
bool cChrCmdLimit_AnmNo::check( const tCharacter& Chr ) const
{
	return Chr.getAnmNo() == mAnmNo;
}

// ---------------------------------------------
//! @brief  コールバック設定
//! @param  Chr	[in]	キャラクター
//! @return true:成立, false:非成立
// ---------------------------------------------
bool cChrCmdLimit_ExtraCB::check( const tCharacter& Chr ) const
{
	return mpCB( Chr, *this );
}

/* ========================================================================= */
/*		EOF																	 */
/* ========================================================================= */
