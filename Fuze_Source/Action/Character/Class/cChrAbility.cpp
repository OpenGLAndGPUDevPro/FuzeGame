// -------------------------------------------------------------------------- //
//! @file   cChrAbility.cpp
//! @brief  アビリティ
//! @author Nal
//! @since  11/09/28(水)
//!
//! COPYRIGHT (C) 2011 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
/* ====< インクルード >====================================================== */
#include "prefix.h"
#include <TGL.h>
#include "Game/Game.h"

/* ========================================================================= */
/*		cChrAbility															 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  コンストラクタ "cChrAbility"
//! @param  pChr	[in]	オーナー
// ---------------------------------------------
cChrAbility::cChrAbility( tCharacter* pChr )
:	mpChr(pChr)
{
	reset();
}

// ---------------------------------------------
//! @brief  デストラクタ "cChrAbility"
// ---------------------------------------------
cChrAbility::~cChrAbility()
{
}

// ---------------------------------------------
//! @brief  全アビリティの初期化してベースアビリティを生成する
// ---------------------------------------------
void cChrAbility::reset()
{
	//!	初期化
	for( u8 i=0 ; i<numberof(mSub) ; i++ ){
		mSub[i].reset();
	}
	//!	ベースアビリティ生成
	Sub*	pAbl = create( ABLTYPE_BASE );
	if( mpChr->getTeam() ){
		if( mpChr->getTeam()->isPlayer() && pAbl ){
			//!	不死
			if( IGCtrl->getSetupAction().tstFlag(GCtrl::ACTF_UNDEAD) )
				pAbl->orAblF( ABLF_UNDEAD );
			//!	ゲージ最大
			if( !IGCtrl->getSetupAction().tstFlag(GCtrl::ACTF_GAUGEDISABLE) && IGCtrl->getSetupAction().tstFlag(GCtrl::ACTF_GAUGEMAX) )
				pAbl->orAblF( ABLF_GAUGERECOVER );
		}
	}
}

// ---------------------------------------------
//! @brief  指定アビリティタイプのアビリティを取得
//! @param  uType	[in]	ABLTYPE_????
//! @return アビリティ。無ければNULL
// ---------------------------------------------
cChrAbility::Sub* cChrAbility::get( u8 uType )
{
	for( u8 i=0 ; i<numberof(mSub) ; i++ ){
		if( mSub[i].getType() == uType )	return &mSub[i];
	}
	return NULL;
}

// ---------------------------------------------
//! @brief  指定配列のアビリティを取得
//! @param  uIndex	[in]	配列
//! @return アビリティ。無ければNULL
// ---------------------------------------------
cChrAbility::Sub* cChrAbility::getFromIndex( u8 uIndex )
{
	if( uIndex >= numberof(mSub) )	return NULL;
	return &mSub[uIndex];
}

// ---------------------------------------------
//! @brief  アビリティの新規作成
//! @param  uType	[in]	ABLTYPE_????
//! @param  uAblF	[in]	付加するABLF。Or演算される
//! @param  bOver	[in]	trueなら重複をチェックし、見つかった場合にそれを返す
//! @return 成功すればアビリティ。失敗すればNULL
// ---------------------------------------------
cChrAbility::Sub* cChrAbility::create( u8 uType, u32 uAblF, bool bOver/*=false*/ )
{
	Sub*	pRet = NULL;
	bool	bFlag = false;
	
	//!	重複チェック
	if( bOver ){
		pRet = get(uType);
		if( pRet )	bFlag = true;
	}
	//!	新規作成
	if( !bFlag ){
		for( u8 i=0 ; i<numberof(mSub) ; i++ ){
			if( mSub[i].isActive() )	continue;
			pRet = &mSub[i];
			break;
		}
	}
	//!	パラメータ付加
	if( pRet ){
		pRet->setType( uType );
		pRet->setAblF( pRet->getAblF() | uAblF );
		pRet->setStartAnmNo( mpChr->getAnmNo() );
	}
	
	return pRet;
}

// ---------------------------------------------
//! @brief  指定タイプのアビリティを削除
//! @param  uType	[in]	ABLTYPE_????
// ---------------------------------------------
void cChrAbility::kill( u8 uType )
{
	Sub*	pAbl = get(uType);
	if( pAbl )	pAbl->reset();
}

// ---------------------------------------------
//! @brief  更新
// ---------------------------------------------
void cChrAbility::update()
{
	//!	除外する時間速度
	f32	fDelta = getSpeedRate();

	for( u8 i=0 ; i<numberof(mSub) ; i++ ){
		mSub[i].update( *mpChr, fDelta );
	}
}

// ---------------------------------------------
//! @brief  全アビリティを加味したABLFを取得
//! @return ABLF
// ---------------------------------------------
u32 cChrAbility::getAblF() const
{
	u32	uRet = 0;
	for( u8 i=0 ; i<numberof(mSub) ; i++ ){
		if( mSub[i].isActive() )	orFLG( uRet, mSub[i].getAblF() );
	}
	return uRet;
}
// ---------------------------------------------
//! @brief  全アビリティを加味したHP加算値を取得
//! @return HP加算値
// ---------------------------------------------
f32 cChrAbility::getHPAdd() const
{
	f32	fRet = 0.0f;
	for( u8 i=0 ; i<numberof(mSub) ; i++ ){
		if( !mSub[i].isActive() )	continue;
		f32	fAdd = mSub[i].getHPAdd();
		fRet += fAdd;
	}
	return fRet;
}
// ---------------------------------------------
//! @brief  全アビリティを加味したHP減算値を取得
//! @return HP減算値
// ---------------------------------------------
f32 cChrAbility::getHPSub() const
{
	f32	fRet = 0.0f;
	for( u8 i=0 ; i<numberof(mSub) ; i++ ){
		if( !mSub[i].isActive() )	continue;
		f32	fSub = mSub[i].getHPSub();
		fRet += fSub;
	}
	return fRet;
}
// ---------------------------------------------
//! @brief  全アビリティを加味したHP最大補正率を取得
//! @return 最大ライフ補正率	[rate]
// ---------------------------------------------
f32 cChrAbility::getHPMaxRate() const
{
	f32	fRet = 1.0f;
	for( u8 i=0 ; i<numberof(mSub) ; i++ ){
		if( mSub[i].isActive() )	fRet *= mSub[i].getHPMaxRate();
	}
	return fRet;
}
// ---------------------------------------------
//! @brief  全アビリティを加味した攻撃力補正率を取得
//! @return 攻撃力補正率	[rate]
// ---------------------------------------------
f32 cChrAbility::getAttackRate() const
{
	f32	fRet = 1.0f;
	for( u8 i=0 ; i<numberof(mSub) ; i++ ){
		if( mSub[i].isActive() )	fRet *= mSub[i].getAttackRate();
	}
	return fRet;
}
// ---------------------------------------------
//! @brief  全アビリティを加味したダメージ補正率を取得
//! @return ダメージ補正率	[rate]
// ---------------------------------------------
f32 cChrAbility::getDamageRate() const
{
	f32	fRet = 1.0f;
	for( u8 i=0 ; i<numberof(mSub) ; i++ ){
		if( mSub[i].isActive() )	fRet *= mSub[i].getDamageRate();
	}
	return fRet;
}
// ---------------------------------------------
//! @brief  全アビリティを加味した動作速度補正率を取得
//! @return モーション速度補正率	[rate]
// ---------------------------------------------
f32 cChrAbility::getSpeedRate() const
{
	f32	fRet = 1.0f;
	for( u8 i=0 ; i<numberof(mSub) ; i++ ){
		if( mSub[i].isActive() )	fRet *= mSub[i].getSpeedRate();
	}
	return fRet;
}
// ---------------------------------------------
//! @brief  スーパーアーマーのダメージ率最低値を取得
//! @param  nDmgHeap	[in]	ダメージ蓄積
//! @return rate。有効なものが無い場合は-1.0f
// ---------------------------------------------
f32 cChrAbility::getSpArmorDmgRate( s16 nDmgHeap ) const
{
	f32	fRet = 9999.0f;
	for( u8 i=0 ; i<numberof(mSub) ; i++ ){
		if( !mSub[i].isActive() )	continue;
		if( mSub[i].getSpArmorDmgRate() < -DELTA )	continue;	//!< スーパーアーマーでない
		if( mSub[i].getSpArmorShield() > 0 && mSub[i].getSpArmorShield() <= nDmgHeap )	continue;	//!< 耐久値オーバー
		if( fRet > mSub[i].getSpArmorDmgRate() )	fRet = mSub[i].getSpArmorDmgRate();
	}
	if( fRet >= 9998.0f )	return -1.0f;	//!< 見つからない
	
	return fRet;
}
// ---------------------------------------------
//! @brief  スーパーアーマーのダメージヒープ減算による破壊
//! @param  nDmgHeap	[in]	ダメージ蓄積
// ---------------------------------------------
void cChrAbility::subSpArmorShield( s16 nDmgHeap )
{
	for( u8 i=0 ; i<numberof(mSub) ; i++ ){
		if( !mSub[i].isActive() )	continue;
		s16 sShield = mSub[i].getSpArmorShield() - nDmgHeap;
		if( sShield <= 0 ){
			mSub[i].setSpArmorDmgRate( -1.0f );
			mSub[i].setSpArmorShield( 0 );
		}
	}
}
// ---------------------------------------------
//! @brief  ベースアビリティに or ABLF
//! @param  i0	[in]	ABLF_????
// ---------------------------------------------
void cChrAbility::orBaseAblF( u32 i0 )
{
	Sub*	pAbl = get( ABLTYPE_BASE );
	if( pAbl )	pAbl->orAblF( i0 );
}
// ---------------------------------------------
//! @brief  ベースアビリティに clr ABLF
//! @param  i0	[in]	ABLF_????
// ---------------------------------------------
void cChrAbility::clrBaseAblF( u32 i0 )
{
	Sub*	pAbl = get( ABLTYPE_BASE );
	if( pAbl )	pAbl->clrAblF( i0 );
}
// ---------------------------------------------
//! @brief  ベースアビリティに xor ABLF
//! @param  i0	[in]	ABLF_????
// ---------------------------------------------
void cChrAbility::xorBaseAblF( u32 i0 )
{
	Sub*	pAbl = get( ABLTYPE_BASE );
	if( pAbl )	pAbl->xorAblF( i0 );
}

/* ========================================================================= */
/*		cChrAbility::Sub													 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  コンストラクタ "cChrAbility"
// ---------------------------------------------
cChrAbility::Sub::Sub()
{
	reset();
}

// ---------------------------------------------
//! @brief  デストラクタ "Sub"
// ---------------------------------------------
cChrAbility::Sub::~Sub()
{
}

// ---------------------------------------------
//! @brief  初期化
// ---------------------------------------------
void cChrAbility::Sub::reset()
{
	mFlag			= 0;
	mType			= ABLTYPE_NONE;
	mCount			= 0;
	mStartAnmNo		= -1;
	mAblF			= 0;
	mHPAdd			= 0;
	mHPSub			= 0;
	mHPMaxRate		= 1.0f;
	mAttackRate		= 1.0f;
	mDamageRate		= 1.0f;
	mSpeedRate		= 1.0f;
	mSpArmorDmgRate	= -1.0f;
	mSpArmorShield	= -1;
	ZeroMemory( mUserI, sizeof(mUserI) );
	ZeroMemory( mUserF, sizeof(mUserF) );
}

// ---------------------------------------------
//! @brief  更新
//! @param  Chr		[in]	キャラクター
//! @param  fDelta	[in]	除外する時間速度 [rate]
// ---------------------------------------------
void cChrAbility::Sub::update( const tCharacter& Chr, f32 fDelta )
{
	//!	未設定
	if( !isActive() )	return;
	
	//!	残り時間更新
	bool	bFlag = true;
	if( mCount < DELTA )	bFlag = false;	//!< 無限時間
//	if( IAction->mActionTime.getRateStandard() < DELTA )	bFlag = false;	//!< 時間停止中	
	if( bFlag ){
		f32	fTime = Chr.getDeltaTime();
		if( fDelta > DELTA )	fTime /= fDelta;	//!< カウントに影響させない時間速度
		mCount -= fTime;
		if( mCount < DELTA ){	//!< 時間切れ
			//!	終了許可
			bool	bNotEnd = false;
//			if( tstFLG(mFlag,cChrAbility::FLAG_ATTACKCON) && Chr.tstAnmF(ANMF_ATTACK) )	bNotEnd = true;	//!< 攻撃中は時間終了しない判定
			
			if( bNotEnd ){	//!< 終了しない
				mCount = 1.0f;
			}else{			//!< 終了
				reset();
				return;
			}
		}
	}
	
	// ---------------------------------
	//!		終了判定					
	if( updateEnd(Chr) ){
		reset();
		return;
	}
}

// ---------------------------------------------
//! @brief  終了判定
//! @param  Chr	[in]	キャラクター
//! @return trueなら終了する
// ---------------------------------------------
bool cChrAbility::Sub::updateEnd( const tCharacter& Chr )
{
	//!	バトル終了時に終了する
//	if( tstFLG(mFlag,FLAG_END_BTLEND) ){
//		if( IAction->getStep() != sAction::GSTEP_BATTLE )	return true;
//	}
	//!	アニメが変わってたら終了する
//	if( tstFLG(mFlag,FLAG_END_ANMCHG) ){
//		if( Chr.getAnmNo() != mStartAnmNo )	return true;
//	}
	//!	ダメージ時に終了する
//	if( tstFLG(mFlag,FLAG_END_DMG) ){
//		if( Chr.tstAnmF(ANMF_DAMAGE|ANMF_TDOWN) || Chr.tstPrgF(PRGF_DMGFRM) )	return true;
//	}
	//!	ガード時に終了する
//	if( tstFLG(mFlag,FLAG_END_GUARD) ){
//		if( Chr.tstAnmF(ANMF_GUARD) )	return true;
//	}
	//!	死亡時に終了する
//	if( tstFLG(mFlag,FLAG_END_DEAD) ){
//		if( Chr.tstChrF(CHRF_DEAD) )	return true;
//	}
	//!	ANMF_ATTACKが立ってなければ終了する
//	if( tstFLG(mFlag,FLAG_END_NOTATK) ){
//		if( !Chr.tstAnmF(ANMF_ATTACK) )	return true;
//	}
	//!	地上にいたら終了する
//	if( tstFLG(mFlag,FLAG_END_TAKEON) ){
//		if( !Chr.isFloat() )	return true;
//	}
	
	return false;
}

// ---------------------------------------------
//! @brief  汎用整数。アビリティタイプ毎に自由に使用可
//! @param  index	[in]	配列
//! @return s32
// ---------------------------------------------
s32 cChrAbility::Sub::getUserI( u8 index ) const
{
	ASSERT( index < numberof(mUserI) && "配列オーバー" );
	return mUserI[index];
}
// ---------------------------------------------
//! @brief  汎用整数。アビリティタイプ毎に自由に使用可
//! @param  index	[in]	配列
//! @param  i0		[in]	値
// ---------------------------------------------
void cChrAbility::Sub::setUserI( u8 index, s32 i0 )
{
	ASSERT( index < numberof(mUserI) && "配列オーバー" );
	mUserI[index] = i0;
}
// ---------------------------------------------
//! @brief  汎用実数。アビリティタイプ毎に自由に使用可
//! @param  index	[in]	配列
//! @return s32
// ---------------------------------------------
f32 cChrAbility::Sub::getUserF( u8 index ) const
{
	ASSERT( index < numberof(mUserF) && "配列オーバー" );
	return mUserF[index];
}
// ---------------------------------------------
//! @brief  汎用実数。アビリティタイプ毎に自由に使用可
//! @param  index	[in]	配列
//! @param  i0		[in]	値
// ---------------------------------------------
void cChrAbility::Sub::setUserF( u8 index, f32 f0 )
{
	ASSERT( index < numberof(mUserF) && "配列オーバー" );
	mUserF[index] = f0;
}

/* ========================================================================= */
/*		EOF																	 */
/* ========================================================================= */
