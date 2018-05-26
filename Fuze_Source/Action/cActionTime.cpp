// -------------------------------------------------------------------------- //
//! @file   cActionTime.cpp
//! @brief  時間制御
//! @author Nal
//! @since  11/10/14(金)
//!
//! COPYRIGHT (C) 2011 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
/* ====< インクルード >====================================================== */
#include "prefix.h"
#include <TGL.h>
#include "Game/Game.h"
#include "ActionTypes.h"

namespace {
	static const f32	FINISHSTOP_TIME = 30.0f;	//!< フィニッシュストップ時間
	static const f32	FINISHSLOW_TIME = 90.0f;	//!< フィニッシュスロウ時間
	static const f32	FINISHSLOW_RATE = 0.1f;		//!< フィニッシュスロウレート
};
/* ========================================================================= */
/*		cActionTime															 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  コンストラクタ "cActionTime"
// ---------------------------------------------
cActionTime::cActionTime()
{
	reset();
}

// ---------------------------------------------
//! @brief  デストラクタ "cActionTime"
// ---------------------------------------------
cActionTime::~cActionTime()
{
}

// ---------------------------------------------
//! @brief  初期化
// ---------------------------------------------
void cActionTime::reset()
{
	endHitSlow();
	
	mbFinished		= false;
	mFinishSlowTime	= -1.0f;
	mpPauseChr		= NULL;
	mPauseMode		= 0;
	
	mDemoSlow.clear( 1.0f );
	mDemoSlowFlag	= 0;
}

// ---------------------------------------------
//! @brief  時間制御更新
// ---------------------------------------------
void cActionTime::update()
{
	// ---------------------------------
	//!	フィニッシュスロウ更新
	if( mFinishSlowTime > -DELTA ){
		mFinishSlowTime += ITimer->getRate();
		//!	終了
		if( mFinishSlowTime > FINISHSTOP_TIME + FINISHSLOW_TIME )	mFinishSlowTime = -1.0f;
	}

	// ---------------------------------
	//!	時間停止監視
	//!	停止リクエスト中キャラの取得
/*	mpPauseChr = NULL;
	for( u8 i=0 ; i<sCharacter::TEAM_MAX ; i++ ){
		for( cChrNode* pNode = ICharacter->getTeam(j)->getNodeNext(); pNode != NULL; pNode = pNode->getNext() ){
			if( pNode->getChr()->getPauseMode() == CUTINPAUSE_NONE )	continue;
			mpPauseChr	= pNode->pChr;
			break;
		}
	}
	//!	停止更新
	if( mpPauseChr ){
		if( mPauseMode == CUTINPAUSE_NONE ){
			endHitSlow();	//!< ヒットスロウの終了
			mPauseMode = mpPauseChr->getPauseMode();
		}
	}else{
		mPauseMode = CUTINPAUSE_NONE;
	}
*/	
	// ---------------------------------
	//!	ヒットスロウ設定
	//!	ディレイ
	if( mHitSlowDelay > -DELTA ){
		mHitSlowDelay -= ITimer->getRate();
		//!	設定
		if( mHitSlowDelay < DELTA ){
			mHitSlowDelay	= -1.0f;
			mHitSlow		= mHitSlowOrder;
		}
	}
	//!	ヒットスロウ更新
	if( mHitSlowDelay < DELTA ){
		if( mHitSlowTime > DELTA ){
			mHitSlowTime -= ITimer->getRate();
			//!	終了
			if( mHitSlowTime < DELTA ){
				endHitSlow();
			}
		}
	}
	
	// ---------------------------------
	//!	演出スロウ
	mDemoSlow.update( ITimer->getRate() );
}

// ---------------------------------------------
//! @brief  ヒットスロウ設定
//! @note   既に設定されている場合、既存設定より有効時間が長ければ上書きされる
//! @param  nDelay	[in]	設定ディレイ	[frame]
//! @param  nFrame	[in]	有効時間		[frame]
//! @param  fRate	[in]	スロウレート	[rate]
//! @param  pAtk	[in]	攻撃キャラ
//! @param  pDmg	[in]	ダメージキャラ
// ---------------------------------------------
void cActionTime::setHitSlow( s32 nDelay, s32 nFrame, f32 fRate, tCharacter* pAtk, tCharacter* pDmg )
{
	if( (int)mHitSlowTime < nFrame ){
		mHitSlowDelay	= (f32)nDelay;
		mHitSlowTime	= (f32)(nFrame+1);
		mHitSlowOrder	= fRate;
		mpHitSlowAtkChr	= pAtk;
		mpHitSlowDmgChr	= pDmg;
		if( mHitSlowDelay < DELTA ){
			mHitSlowDelay = 0.0f;	//!< 初期フレーム判定用に0.0を下限とする
		}
	}
}

// ---------------------------------------------
//! @brief  ヒットスロウ終了
// ---------------------------------------------
void cActionTime::endHitSlow()
{
	mHitSlowTime	= 0.0f;
	mHitSlowDelay	= -1.0f;
	mHitSlowOrder	= 1.0f;
	mHitSlow		= 1.0f;
	mpHitSlowAtkChr	= NULL;
	mpHitSlowDmgChr	= NULL;
}

// ---------------------------------------------
//! @brief  フィニッシュスロウのリクエスト
// ---------------------------------------------
void cActionTime::reqFinishSlow()
{
	mbFinished		= true;
	mFinishSlowTime = 0.0f;
	endHitSlow();
}

// ---------------------------------------------
//! @brief  フィニッシュスロウレート取得
//! @return rate
// ---------------------------------------------
f32 cActionTime::getFinishSlow() const
{
	if( mFinishSlowTime > -DELTA ){
		if( mFinishSlowTime < FINISHSTOP_TIME )	return 0.0f;
		else									return FINISHSLOW_RATE;
	}
	return 1.0f;
}

// ---------------------------------------------
//! @brief  カットインポーズ取得
//! @param  pChr	[in]	キャラ
//! @return rate
// ---------------------------------------------
f32 cActionTime::getCutinPause( tCharacter* pChr/*=NULL*/ ) const
{
	switch( mPauseMode ){
		case CUTINPAUSE_SYSATK:	//!< システムカットイン
		case CUTINPAUSE_EXATK:	//!< 超必殺カットイン
			if( mpPauseChr ){
				if( mpPauseChr != pChr )	return 0.0f;
			}
			break;
	}
	return 1.0f;
}

// ---------------------------------------------
//! @brief  時間レート取得（カメラ）
//! @return rate
// ---------------------------------------------
f32 cActionTime::getRateCamera() const
{
	if( tstDemoSlowFlag(DEMOSLOWF_CAMERA) )
		return getHitSlow() * getFinishSlow() * getCutinPause() * mDemoSlow.getValue();
	else
		return getHitSlow() * getFinishSlow() * getCutinPause();
}

// ---------------------------------------------
//! @brief  時間レート取得（エフェクト）
//! @return rate
// ---------------------------------------------
f32 cActionTime::getRateEffect() const
{
	if( tstDemoSlowFlag(DEMOSLOWF_EFFECT) )
		return getFinishSlow() * mDemoSlow.getValue();
	else
		return getFinishSlow();
}

/* ========================================================================= */
/*		EOF																	 */
/* ========================================================================= */
