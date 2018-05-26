// -------------------------------------------------------------------------- //
//! @file   cAttackTarget.cpp
//! @brief  攻撃対象
//! @author Nal
//! @since  13/12/14(土)
//!
//! COPYRIGHT (C) 2013 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
/* ====< インクルード >====================================================== */
#include "prefix.h"
#include <TGL.h>
#include "cAttackTarget.h"
#include "Action/Shot/sShot.h"

/* ====< 定義 >====================================================== */
/* ========================================================================= */
/*		cAttackTarget														 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  コンストラクタ "cAttackTarget"
// ---------------------------------------------
cAttackTarget::cAttackTarget()
{
	reset();
}
// ---------------------------------------------
//! @brief  コンストラクタ "cAttackTarget"
//! @param  pLife	[in]	ライフ
// ---------------------------------------------
cAttackTarget::cAttackTarget( cChrStatus::Life* pLife )
{
	reset();
	setObj( pLife );
}
// ---------------------------------------------
//! @brief  コンストラクタ "cAttackTarget"
//! @param  pShot	[in]	ショット
// ---------------------------------------------
cAttackTarget::cAttackTarget( tShot* pShot )
{
	reset();
	setObj( pShot );
}

// ---------------------------------------------
//! @brief  デストラクタ "cAttackTarget"
// ---------------------------------------------
cAttackTarget::~cAttackTarget()
{
}

// ---------------------------------------------
//! @brief  初期化
// ---------------------------------------------
void cAttackTarget::reset()
{
	mType	= TYPE_UNKNOWN;
	mpObj	= NULL;
}

// ---------------------------------------------
//! @brief  更新
// ---------------------------------------------
void cAttackTarget::update()
{
	//!	継続判定
	if( !isContinue() )
		reset();
}

// ---------------------------------------------
//! @brief  座標取得
//! @return TGL::Vec3
// ---------------------------------------------
TGL::Vec3 cAttackTarget::getPos() const
{
	switch( mType ){
		case TYPE_CHR:	//!< キャラクター部位
			return getObjLife()->getPos();
		case TYPE_SHOT:	//!< ショット
			return getObjShot()->getPos();
	}
	return TGL::Vec3::ZERO;
}

// ---------------------------------------------
//! @brief  継続判定
//! @return true:継続
// ---------------------------------------------
bool cAttackTarget::isContinue() const
{
	switch( mType ){
		case TYPE_CHR:	//!< キャラクター部位
			//!	ターゲット不可
			if( !getObjLife()->isParentTarget() )	return false;
			//!	ライフターゲット
			if( !getObjLife()->isTarget() )	return false;
			return true;

		case TYPE_SHOT:	//!< ショット
			//!	終了
			if( !getObjShot()->isActive() )		return false;
			if( !getObjShot()->isLockEnable() )	return false;
			return true;
	}
	return false;
}

/* ========================================================================= */
/*		EOF																	 */
/* ========================================================================= */
