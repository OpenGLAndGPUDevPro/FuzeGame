// -------------------------------------------------------------------------- //
//! @file   cAttack.cpp
//! @brief  攻撃管理
//! @author Nal
//! @since  11/10/01(土)
//!
//! COPYRIGHT (C) 2011 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
/* ====< インクルード >====================================================== */
#include "prefix.h"
#include <TGL.h>
#include "cAttack.h"
#include "Game/Game.h"
#include "Sound/sAppSound.h"
#include "nAttack.h"

/* ========================================================================= */
/*		cAttack																 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  コンストラクタ "cAttack"
//! @note   直後にsetupを行うこと
// ---------------------------------------------
cAttack::cAttack()
:	mParentType(TYPE_UNKNOWN)
{
	mpEftId	= APP_NEW s16[HITKIND_MAX];
	mpSeId	= APP_NEW s16[HITKIND_MAX];
}

// ---------------------------------------------
//! @brief  セットアップ
//! @param  uType		[in]	TYPE_????
//! @param  uId			[in]	本体ID
//! @param  uColType	[in]	COL_????
//! @param  pCol		[in]	当たり判定の先頭ポインタ
//! @param  pvColPosOld	[in]	当たり判定の座標バッファの先頭ポインタ
//! @param  uColNum		[in]	当たり判定の数
// ---------------------------------------------
void cAttack::setup( u8 uType, u32 uId, u8 uColType, TGL::ColBase* pCol, TGL::Vec3* pvColPosOld, u8 uColNum )
{
	mParentType		= uType;		//!< 本体の種類
	mParentId		= uId;			//!< 本体のID
	mColType		= uColType;		//!< 判定の型
	mpCol			= pCol;			//!< 判定本体
	mpColPosOld		= pvColPosOld;	//!< 判定座標バッファ
	mColNum			= uColNum;		//!< 判定の数
	reset();
}

// ---------------------------------------------
//! @brief  デストラクタ "cAttack"
// ---------------------------------------------
cAttack::~cAttack()
{
	SAFE_DELETE_ARRAY( mpEftId );
	SAFE_DELETE_ARRAY( mpSeId );
}

// ---------------------------------------------
//! @brief  初期化
// ---------------------------------------------
void cAttack::reset()
{
	mFlag			= 0;			//!< FLAG_????
	mDelay			= -1.0f;		//!< [PARAMETER] 判定発生までの時間
	mLife			= -1.0f;		//!< [PARAMETER] 判定消滅までの時間
	mDelayField		= -1.0f;		//!< [PARAMETER] フィールドヒット判定発生までの時間（0未満で無効）
	mLifeField		= -1.0f;		//!< [PARAMETER] フィールドヒット判定消滅までの時間（0未満で無効）
	
	mMaxCombo		= -1;			//!< [PARAMETER] 同じ対象にヒットする最大回数（0以下で無制限）
	mEndTotalCombo	= -1;			//!< [PARAMETER] 判定消滅するヒット回数（0以下で無制限 / 現在の仕様だと更新時に判定される。つまり同フレーム内ではいくらヒットさせても消えない）
	mInterval		= -1.0f;		//!< [PARAMETER] 複数ヒットする際の間隔時間（0未満で１回のみ）

	mAtkF			= 0;			//!< [PARAMETER] ATKF_???? 攻撃フラグ
	mAtrF			= 0;			//!< [PARAMETER] ATRF_???? 属性フラグ
	mThrF			= 0;			//!< [PARAMETER] THRF_???? 投げフラグ

	mPowLevel		= ATKPOW_W;			//!< [PARAMETER] ATKPOW_???? 威力段階
	mPointType		= ATKPNT_HIGH;		//!< [PARAMETER] ATKPNT_???? 攻撃箇所
	mHardType		= ATKHARD_HAND;		//!< [PARAMETER] ATKHARD_???? 強度
	mUserId			= ATKUSERID_NONE;	//!< [PARAMETER] ATKUSERID_???? 特殊判別用ID
	mShotEraseLv	= SHOTERASE_NONE;	//!< [PARAMETER] SHOTERASE_???? ショット相殺レベル
	mDmgStaLand		= ATKDMGSTA_NONE;	//!< [PARAMETER] ATKDMGSTA_???? 地上ダメージステータス（ダメージモーション影響）
	mDmgStaAir		= ATKDMGSTA_NONE;	//!< [PARAMETER] ATKDMGSTA_???? 空中ダメージステータス（ダメージモーション影響）

	mPower			= 10.0f;		//!< [PARAMETER] 攻撃力	[初期値10]
	mPowerGuard		= 0.125f;		//!< [PARAMETER] ガード削り率（初期値0.125）
	mPowerSelf		= 0.0f;			//!< [PARAMETER] 自身への攻撃力	[0:無効]
	mPowerPlayer	= 1.0f;			//!< [PARAMETER] プレイヤーへの攻撃力倍率
	mPowerEnemy		= 1.0f;			//!< [PARAMETER] 敵への攻撃力倍率
	mStiffDamage	= 0;			//!< [PARAMETER] ダメージ硬直加算値
	mStiffGuard		= 0;			//!< [PARAMETER] ガード硬直加算値
	mStiffDown		= 0;			//!< [PARAMETER] ダウン硬直指定値
	mDmgSpd			= -9.0f;		//!< [PARAMETER] ヒットバック 初速
	mDmgAcl			=  1.3f;		//!< [PARAMETER] ヒットバック 加速度
	mGuardSpd		= -8.0f;		//!< [PARAMETER] ガードバック 初速
	mGuardAcl		=  0.8f;		//!< [PARAMETER] ガードバック 加速度
	mFloatSpd		= 10.0f;		//!< [PARAMETER] 空中コンボ時の自身のYSpd代入値
	mFloatAcl		= -0.98f;		//!< [PARAMETER] 空中コンボ時の自身のYAcl代入値
	mBoundSpd		= 12.0f;		//!< [PARAMETER] バウンド量指定叩きつけのバウンド初速
	mBoundAcl		=  0.98f;		//!< [PARAMETER] バウンド量指定叩きつけのバウンド重力
	mHitStop		= -1;			//!< [PARAMETER] ヒットストップ値（0未満で無効）
	mHitStopDelay	= 0;			//!< [PARAMETER] ヒットストップディレイ値（判定発生からの相対時間）
	mJCancelDelay	= 0;			//!< [PARAMETER] ジャンプキャンセルディレイ値（判定発生からの相対時間）

	mAirTime		= 30;			//!< [PARAMETER] 浮かしパラメータ 着地までの時間（0以下で無効）
	mAirTimeOfs		= 0;			//!< [PARAMETER] 浮かしパラメータ 最高高度に達する時間オフセット（0で無効）
	mAirDist		= 150.0f;		//!< [PARAMETER] 浮かしパラメータ 距離
	mAirSpeed		= 20.0f;		//!< [PARAMETER] 浮かしパラメータ 初速
	mAirStiffUkemi	= 20;			//!< [PARAMETER] 浮かしパラメータ 受身不可時間

	mThrowNo		= -1;			//!< [PARAMETER] 投げ番号。		（ヒット時に自分が移行する先）ATRF_THROWと同時に指定すると有効
	mTdownNo		= -1;			//!< [PARAMETER] 投げられ番号。	（ヒット時に相手が移行する先）ATRF_THROWと同時に指定すると有効

	for( u8 i=0 ; i<HITKIND_MAX ; i++ ){
		mpEftId[i]	= -1;			//!< [PARAMETER] ヒットエフェクト	[ヒット]
		mpSeId[i]	= -1;			//!< [PARAMETER] ヒット音			[ヒット]
	}
	mSwingSeId		= -1;			//!< [PARAMETER] 判定発生瞬間に再生される効果音
	mWayOfs			= TGL::Vec3::ZERO;	//!< [PARAMETER] ヒット方向計算用オフセット
	mWayRand		= 0;			//!< [PARAMETER] ヒット方向計算用オフセット
	mCamShakeTime	= -1;			//!< [PARAMETER] カメラ振動時間 [0未満で自動設定]
	mCamShake		= -1.0f;		//!< [PARAMETER] カメラ振動値

	mScoreRate			= 0.0f;		//!< [PARAMETER] スコアテンション影響率
	mScoreRateAdd		= 0.0f;		//!< [PARAMETER] スコアテンション加算 撃ち込み
	mScoreRateAddFin	= 0.0f;		//!< [PARAMETER] スコアテンション加算 撃破

	mTotalCombo		= 0;			//!< 現在のヒット回数
	mCountTime		= 0.0f;			//!< 判定発生からの経過時間
	mEndTime		= 0.0f;			//!< 判定終了からの経過時間
	
	mAnmNo			= -1;			//!< 攻撃設定時のアニメ番号。ヒット時に変更されていた場合コンボに繋がったりしない
	
	mTarget.clear();
	
	endCollision();	//!< 判定終了
	
	clrFLG( mFlag, HITF_ENABLE );
}

// ---------------------------------------------
//! @brief  攻撃判定開始
//! @note   ここでは最小限のパラメータだけ指定し、それ以外は初期値が設定される。変更したいものはこの後に書き換えれば良い
//! @param  nAnmNo		[in]	この時点でのアニメ番号
//! @param  fNowTime	[in]	現在時間
//! @param  fStartTime	[in]	開始時間。fNowTime互換で計算される
//! @param  fEndTime	[in]	終了時間。fNowTime互換で計算される。0未満なら無制限
// ---------------------------------------------
void cAttack::start( s16 nAnmNo, f32 fNowTime, f32 fStartTime, f32 fEndTime )
{
	//!	まずリセット
	reset();
	
	//!	パラメータ設定
	orFLG( mFlag, HITF_DELAY );
	mAnmNo	= nAnmNo;
	mDelay	= fStartTime - fNowTime + 0.0f;	//!< 今から開始までの時間
	if( fEndTime < 0.0f )	mLife = -0.2f;
	else					mLife = fEndTime - fStartTime + 0.0f;	//!< 開始してからの継続時間
}

// ---------------------------------------------
//! @brief  攻撃判定の座標関連を設定する 球
//! @param  Guide	[in]	マトリクスアクセサ
//! @param  fRadius	[in]	半径
// ---------------------------------------------
void cAttack::setCollisionSphere( const TGL::MtxGuide& Guide, f32 fRadius )
{
	//!	エラーチェック
	if( mColType != COL_SPHERE ){
		T_WARNING("当たり判定の型が不整合 (%d)", mColType);
		ASSERT(0);
	}
	//!	
	for( u8 i=0 ; i<mColNum ; i++ ){
		TGL::ColSphere*	pCol = ((TGL::ColSphere*)mpCol)+i;
		if( pCol->isSetup() )	continue;	//!< 空き検索
		pCol->setup( Guide, fRadius );
		pCol->setUserF( COLUSERF_ATK );	//!< 攻撃属性
		if( tstFlag(HITF_ENABLE) )	pCol->enable();		//!< 攻撃が有効な状態で始まった場合
		else						pCol->disable();
		*(((TGL::Vec3*)mpColPosOld)+i) = pCol->getPos();	//!< 座標バッファ更新
		return;
	}
	T_WARNING("判定設定可能数オーバー");
}
// ---------------------------------------------
//! @brief  攻撃判定の座標関連を設定する 線
//! @param  GuideS	[in]	始点マトリクスアクセサ
//! @param  GuideE	[in]	終点マトリクスアクセサ
//! @param  fWidth	[in]	幅
// ---------------------------------------------
void cAttack::setCollisionLine( const TGL::MtxGuide& GuideS, const TGL::MtxGuide& GuideE, float fWidth )
{
	//!	エラーチェック
	if( mColType != COL_LINE ){
		T_WARNING("当たり判定の型が不整合 (%d)", mColType);
		ASSERT(0);
	}
	//!	
	for( u8 i=0 ; i<mColNum ; i++ ){
		TGL::ColLine*	pCol = ((TGL::ColLine*)mpCol)+i;
		if( pCol->isSetup() )	continue;	//!< 空き検索
		pCol->setup( GuideS, GuideE, fWidth );
		pCol->setUserF( COLUSERF_ATK );	//!< 攻撃属性
		if( tstFlag(HITF_ENABLE) )	pCol->enable();		//!< 攻撃が有効な状態で始まった場合
		else						pCol->disable();
		*(((TGL::Vec3*)mpColPosOld)+i) = pCol->getPosE();	//!< 座標バッファ更新
		//!	座標バッファで線を構築する場合、座標を初期化する
		if( !GuideS.isFix() && tstFlag(HITF_TALELINE) )
			pCol->getMtxGuideS().set( *(((TGL::Vec3*)mpColPosOld)+i) );
		return;
	}
	T_WARNING("判定設定可能数オーバー");
}

/* ======================================================================== */
/*		判定チェック														*/
/* ======================================================================== */
// ---------------------------------------------
//! @brief  攻撃判定がターゲットに有効かチェック
//! @param  pTarget		[in]	対象（キャラ、ショット、ステージ…）
//! @return true:有効
// ---------------------------------------------
bool cAttack::checkTarget( const void* pTarget )
{
	//!	無効チェック
	if( tstFLG(mFlag,HITF_PAUSE) )	return false;	//!< 一時停止中
	//!	ターゲット処理
	stl::map<xu32,Target>::iterator	it = mTarget.find( (xu32)pTarget );
	if( it != mTarget.end() ){
		if( mInterval < 0.0f )				return false;	//!< ヒット間隔制限（１回ヒットのみ）
		if( mMaxCombo > 0 && it->second.mCombo >= mMaxCombo )	return false;	//!< ヒット数制限
		if( it->second.mTime < mInterval )	return false;	//!< ヒット間隔制限
	}
	return true;
}

// ---------------------------------------------
//! @brief  全攻撃判定と指定した判定とをチェックする
//! @param  ColDmg			[in]	やられ判定
//! @param  uNotCheckFlag	[in]	判定しない当たり判定判断用フラグ(HITSPHEREUSERF_??等)
//! @param  pRet			[i/o]	判定詳細結果。NULLを指定すると計算が軽くなる
//! @return true:ヒット
// ---------------------------------------------
bool cAttack::check( const TGL::ColBase& ColDmg, u32 uNotCheckFlag, TGL::ColBase::RET_T* pRet/*=NULL*/ )
{
	//!	3D計算の強制
	ICollision->set2DDisable( tstAtkF(ATKF_HIT3D) != false );
	
	bool	bRet = false;
	for( u8 i=0 ; i<mColNum ; i++ ){
		TGL::ColBase*	pCol = getColBase(i);
		if( !pCol )	continue;
		if( pCol->tstUserF(uNotCheckFlag) ) continue; //!< 判定しない当たり判定チェック
		if( pCol->check(ColDmg, pRet) ){
			bRet = true;
			break;
		}
	}
	ICollision->set2DDisable( false );
	return bRet;
}

// ---------------------------------------------
//! @brief  全攻撃判定と指定した判定とをチェックする。やられが「攻撃判定」（相殺用）
//! @param  AtkT	[in]	攻撃判定Ｂ
//! @param  pRet	[i/o]	判定詳細結果。NULLを指定すると計算が軽くなる
//! @return true:ヒット
// ---------------------------------------------
bool cAttack::checkAtk( const cAttack& AtkT, TGL::ColBase::RET_T* pRet/*=NULL*/ )
{
	//!	無効チェック
	if( tstFLG(mFlag,HITF_PAUSE) )	return false;	//!< 一時停止中

	//!	3D計算の強制
	ICollision->set2DDisable( tstAtkF(ATKF_HIT3D) != false );

	//!	判定
	bool	bRet = false;
	for( u8 i=0 ; i<mColNum ; i++ ){
		TGL::ColBase*	pCol = getColBase(i);
		if( !pCol )	break;
		for( u8 j=0 ; j<AtkT.mColNum ; j++ ){
			const TGL::ColBase*	pColT = AtkT.getColBaseConst(j);
			if( !pColT )	break;
			if( pCol->check(*pColT, pRet) ){
				bRet = true;
				break;
			}
		}
	}
	ICollision->set2DDisable( false );
	return bRet;
}

// ---------------------------------------------
//! @brief  ヒット時に呼び出して、各値を更新する
//! @param  pTarget		[in]	対象（キャラ、ショット、ステージ…）
// ---------------------------------------------
void cAttack::hit( void* pTarget )
{
	mTotalCombo++;
	mTarget[(xu32)pTarget].mCombo++;
	mTarget[(xu32)pTarget].mTime = 0;
}

// ---------------------------------------------
//! @brief  攻撃判定同士の相殺優先判定を取る
//! @param  AtkT	[in]	攻撃判定
//! @return 判定結果を返す。SOUSAI_????
// ---------------------------------------------
u8 cAttack::checkSousai( const cAttack& AtkT )
{
	u32	uAtrF0	= mAtrF;
	u32	uAtrF1	= AtkT.mAtrF;
	
	//!	強制相殺
	if( tstFLG(uAtrF0,ATRF_ABSSOUSAI))	return SOUSAI_DRAW;
	if( tstFLG(uAtrF1,ATRF_ABSSOUSAI))	return SOUSAI_DRAW;
	
	//!	無効フラグ
	if( tstFLG(uAtrF0,ATRF_NSOUSAI) )	return SOUSAI_DISABLE;
	if( tstFLG(uAtrF1,ATRF_NSOUSAI) )	return SOUSAI_DISABLE;

	//!	優先フラグ判定
	if( tstFLG(uAtrF0,ATRF_SOUSAI_EX) && !tstFLG(uAtrF1,ATRF_SOUSAI_EX) ) return SOUSAI_WIN_A; // A が EX
	if( !tstFLG(uAtrF0,ATRF_SOUSAI_EX) && tstFLG(uAtrF1,ATRF_SOUSAI_EX) ) return SOUSAI_WIN_B; // B が EX
	
	//!	必殺 vs 通常攻撃
	if( tstFLG(uAtrF0,ATRF_EX) && !tstFLG(uAtrF1,ATRF_EX) )				return SOUSAI_WIN_A;
	if( tstFLG(uAtrF1,ATRF_EX) && !tstFLG(uAtrF0,ATRF_EX) )				return SOUSAI_WIN_B;
//	if( tstFLG(uAtrF0,ATRF_SPATK) && !tstFLG(uAtrF1,ATRF_SPATK|ATRF_EX) )	return SOUSAI_WIN_A;
//	if( tstFLG(uAtrF1,ATRF_SPATK) && !tstFLG(uAtrF0,ATRF_SPATK|ATRF_EX) )	return SOUSAI_WIN_B;

#if 0
	//	判定
	switch( mHardType ){
	  // ---------------------------------------
	  //	物理								
	  // ---------------------------------------
	  case ATKHARD_HAND:	// 素手				
		switch( AtkT.mHardType ){
		  case ATKHARD_HAND:	return SOUSAI_DRAW;		// 素手 vs 素手
		  case ATKHARD_WEAPON:	return SOUSAI_DRAW;		// 素手 vs 武器
		  case ATKHARD_EDGE:	return SOUSAI_WIN_B;	// 素手 vs 刃物
		  case ATKHARD_ENERGY:	return SOUSAI_WIN_B;	// 素手 vs 謎のエネルギー
		}
		PRINT("SOUSAICheck: WARNING A 強度設定が変 (%d vs %d)\n", mHardType, AtkT.mHardType );
		return SOUSAI_DISABLE;

	  // ---------------------------------------
	  case ATKHARD_WEAPON:	// 武器				
		switch( AtkT.mHardType ){
		  case ATKHARD_HAND:	return SOUSAI_DRAW;		// 武器 vs 素手
		  case ATKHARD_WEAPON:	return SOUSAI_DRAW;		// 武器 vs 武器
		  case ATKHARD_EDGE:	return SOUSAI_DRAW;		// 武器 vs 刃物
		  case ATKHARD_ENERGY:	return SOUSAI_WIN_B;	// 武器 vs 謎のエネルギー
		}
		PRINT("SOUSAICheck: WARNING B 強度設定が変 (%d vs %d)\n", mHardType, AtkT.mHardType );
		return SOUSAI_DISABLE;

	  // ---------------------------------------
	  case ATKHARD_EDGE:	// 刃物				
		switch( AtkT.mHardType ){
		  case ATKHARD_HAND:	return SOUSAI_WIN_A;	// 刃物 vs 素手
		  case ATKHARD_WEAPON:	return SOUSAI_DRAW;		// 刃物 vs 武器
		  case ATKHARD_EDGE:	return SOUSAI_DRAW;		// 刃物 vs 刃物
		  case ATKHARD_ENERGY:	return SOUSAI_WIN_B;	// 刃物 vs 謎のエネルギー
		}
		PRINT("SOUSAICheck: WARNING C 強度設定が変 (%d vs %d)\n", mHardType, AtkT.mHardType );
		return SOUSAI_DISABLE;

	  // ---------------------------------------
	  //	非物理								
	  // ---------------------------------------
	  case ATKHARD_ENERGY:	// 謎のエネルギー	
		switch( AtkT.mHardType ){
		  case ATKHARD_HAND:	return SOUSAI_WIN_A;	// 謎のエネルギー vs 素手
		  case ATKHARD_WEAPON:	return SOUSAI_WIN_A;	// 謎のエネルギー vs 武器
		  case ATKHARD_EDGE:	return SOUSAI_WIN_A;	// 謎のエネルギー vs 刃物
		  case ATKHARD_ENERGY:	return SOUSAI_DRAW;		// 謎のエネルギー vs 謎のエネルギー
		}
		PRINT("SOUSAICheck: WARNING D 強度設定が変 (%d vs %d)\n", mHardType, AtkT.mHardType );
		return SOUSAI_DISABLE;
	}
	PRINT("SOUSAICheck: WARNING E 強度設定が変 (%d vs %d)\n", mHardType, AtkT.mHardType );
	return SOUSAI_DISABLE;

#else	//	属性判定無効
	return SOUSAI_DRAW;
#endif
}

// ---------------------------------------------
//! @brief  攻撃判定同士の相殺優先判定を取る(ATKPOW_???で優劣をつける)
//! @param  AtkT	[in]	攻撃判定
//! @return 判定結果を返す。SOUSAI_????
// ---------------------------------------------
u8 cAttack::checkSousaiPowLevel( const cAttack& AtkT )
{
	u32	uAtrF0	= mAtrF;
	u32	uAtrF1	= AtkT.mAtrF;

	//!	強制相殺
	if( tstFLG(uAtrF0,ATRF_ABSSOUSAI))	return SOUSAI_DRAW;
	if( tstFLG(uAtrF1,ATRF_ABSSOUSAI))	return SOUSAI_DRAW;

	//!	無効フラグ
	if( tstFLG(uAtrF0,ATRF_NSOUSAI) )	return SOUSAI_DISABLE;
	if( tstFLG(uAtrF1,ATRF_NSOUSAI) )	return SOUSAI_DISABLE;

	//!	優先フラグ判定
	if( tstFLG(uAtrF0,ATRF_SOUSAI_EX) && !tstFLG(uAtrF1,ATRF_SOUSAI_EX) ) return SOUSAI_WIN_A; // A が EX
	if( !tstFLG(uAtrF0,ATRF_SOUSAI_EX) && tstFLG(uAtrF1,ATRF_SOUSAI_EX) ) return SOUSAI_WIN_B; // B が EX

	//!	必殺 vs 通常攻撃
	if( tstFLG(uAtrF0,ATRF_EX) && !tstFLG(uAtrF1,ATRF_EX) )				return SOUSAI_WIN_A;
	if( tstFLG(uAtrF1,ATRF_EX) && !tstFLG(uAtrF0,ATRF_EX) )				return SOUSAI_WIN_B;
//	if( tstFLG(uAtrF0,ATRF_SPATK) && !tstFLG(uAtrF1,ATRF_SPATK|ATRF_EX) )	return SOUSAI_WIN_A;
//	if( tstFLG(uAtrF1,ATRF_SPATK) && !tstFLG(uAtrF0,ATRF_SPATK|ATRF_EX) )	return SOUSAI_WIN_B;

	//	判定
	if     ( mPowLevel == AtkT.mPowLevel ) return SOUSAI_DRAW;
	else if( mPowLevel > AtkT.mPowLevel  ) return SOUSAI_WIN_A;
	else if( mPowLevel < AtkT.mPowLevel  ) return SOUSAI_WIN_B;

	T_WARNING("A 威力段階設定が変 (%d vs %d)", mPowLevel, AtkT.mPowLevel );
	return SOUSAI_DISABLE;
}

// ---------------------------------------------
//! @brief  ヒット方向計算
//! @param  uType	[in]	計算タイプ
//! @param  mtxBase	[in]	基準マトリクス
//! @param  vPos	[in]	ヒット座標
//! @return 方向ベクトル
// ---------------------------------------------
TGL::Vec3 cAttack::culcHitWay( u8 uType, const TGL::Matrix& mtxBase, const TGL::Vec3& vPos )
{
	//!	攻撃側位置
	TGL::Vec3	v0;
	v0 = mWayOfs.apply( mtxBase );
	//!	防御側位置
	TGL::Vec3	v1 = vPos;
	
	//!	スクリーン変換
	if( uType == HITWAY_2D ){
		v0 = ICollision->adjustPos2D( v0 );
		v1 = ICollision->adjustPos2D( v1 );
	}
	
	//!	ヒット方向
	TGL::Vec3	vWay;
	vWay = v0.wayNL( v1 );
	
	//!	方向誤差
	if( FABS(mWayRand) > DELTA ){
		//!	ヒット座標系作成
		TGL::Vec3	vEye = ICamera->getMain()->getEyeVector();
		TGL::Vec3	vUp;
		vUp = vWay.cross( vEye );
		
		TGL::Matrix	mtx = TGL::Matrix::UNIT;
		mtx.setX( vWay );
		mtx.setY( vUp );
		mtx.setZ( vEye );

		switch( uType ){
			case HITWAY_2D:	//!< 2D : 視線ベクトル回転
			{
				f32	fDeg = RANDF2( -mWayRand, mWayRand );
				mtx.rollVec( mtx.getZ(), fDeg );
				vWay = mtx.getX();
			}	break;
			
			case HITWAY_3D:	//!< 3D : 円錐状に回転
			{
				f32	fVer = RANDF2( -mWayRand, mWayRand );	//!< 仰角
				f32	fRot = RANDF2( -180, 180 );	//!< 回転角
				v0 = TGL::Vec3::waySphere( TGL_X, fVer, fRot );	//!< 円錐状のベクトル
				
				vWay = v0.applySR( mtx );	//!< ヒット座標系に変換
				vWay = vWay.normalize();
			}	break;
		}
	}
	
//	f32	fFront = SQRT( (vWay.x*vWay.x) + (vWay.z*vWay.z) );
	
	return vWay;
}

/* ======================================================================== */
/*		更新																*/
/* ======================================================================== */
// ---------------------------------------------
//! @brief  攻撃判定更新。判定の発生と終了を管理する
//! @param  fDelta	[in]	デルタタイム
// ---------------------------------------------
void cAttack::update( f32 fDelta )
{
	//!	無効チェック
	if( mLife <= -1.0f )			return;	//!< 攻撃してない
	if( tstFLG(mFlag,HITF_PAUSE) )	return;	//!< 一時停止中

	// ---------------------------------
	//!	フィールドヒット時間処理
	updateFieldHit( fDelta );

	// ---------------------------------
	//!	ディレイ中
	//!	判定発生
	if( mDelay >= 0.0f ){
		mDelay -= fDelta;
		if( mDelay > 0.0f )	return;	//!< ディレイ中につきreturn
		//!	発生
		enableCollision();
		mDelay = -1.0f;
		//!	SE再生
		if( mSwingSeId >= 0 && mpCol->isSetup() ){	// 判定が付いてなければ鳴らさない
//			switch( mSwingSeId ){
//			  case SE_SWINGBLOW_W:	// 打撃スイング
//				if     ( mPowLevel == ATKPOW_M )		mSwingSeId = SE_SWINGBLOW_M;
//				else if( mPowLevel == ATKPOW_S )		mSwingSeId = SE_SWINGBLOW_S;
//				else if( mPowLevel == ATKPOW_EX )		mSwingSeId = SE_SWINGBLOW_S;
//				break;
//			}
			TGL::Vec3	v0 = getPos( 0 );	//!< 先頭の判定の座標からサウンド再生
			TGL::SoundChannel*	pCh = IAppSound->play( SDID_ONMEM, mSwingSeId );	// 再生
			if( pCh ){
				pCh->setField( v0 );	//!< 音場
				IAppSound->setStageFX( pCh );
			}
		}
	}
	
	// ---------------------------------
	//!	発生中 
	clrFLG( mFlag, HITF_DELAY );
	//!	ヒット経過時間を進める
	for( stl::map<xu32,Target>::iterator it=mTarget.begin() ; it!=mTarget.end() ; it++ )
		it->second.update( fDelta );

	//!	判定発生経過時間を進める
	mCountTime	 += fDelta;

	//! ヒットマーク用更新
//	_ATKCTRL_UpdateHitMark( pAtk );

	//!	判定終了
	if( mLife >= 0.0f ){
		mLife -= fDelta;	//!< 判定終了までの時間

		// ---------------------------------
		//!	終了判定
		bool	bFlag = false;
		//!	時間
		if( mLife <= 0.0f )
			bFlag = true;
		//!	ヒット数
		if( mEndTotalCombo > 0 && mTotalCombo >= mEndTotalCombo )
			bFlag = true;
		if( !bFlag )	return;	//!< 判定発生中につきreturn

		//!	終了
		endCollision();
		clrFLG( mFlag, HITF_ENABLE );
		mLife = -0.1f;	//!< 判定終了無効化
	}
	if( mLife <= -0.2f+DELTA )	return;	//!< 寿命無制限

	// ---------------------------------
	//!	終了後
	orFLG( mFlag, HITF_END );
	if( !tstFLG(mAtkF,ATKF_CONTINUE) ){	//!< 多段攻撃の最後ではない攻撃の場合は、終了経過を進めるとコンボ入力が正常に動かない
		mEndTime += fDelta;	//!< 判定終了からの時間
	}
}

// ---------------------------------------------
//! @brief  フィールドヒット用パラメータ更新
//! @param  fDelta	[in]	デルタタイム
// ---------------------------------------------
void cAttack::updateFieldHit( f32 fDelta )
{
	if( mLifeField <= -1.0f )	return;	// 無効
	
	//	ディレイ
	if( mDelayField >= 0.0f ){
		mDelayField -= fDelta;
		if( mDelayField > 0.0f )	return;	//!< ディレイ中
	}

	//!	発生
	if( mLifeField >= 0.0f ){
		orFLG( mFlag, HITF_FIELDHIT );
		mLifeField -= fDelta;
		//!	終了
		if( mLifeField <= 0.0f ){
			clrFLG( mFlag, HITF_FIELDHIT );
			mLifeField = -0.1f;
		}
	}
}

// ---------------------------------------------
//! @brief  攻撃判定の座標バッファ更新
//! @note   バッファを必要としない場合と動かない場合は呼ばなくても問題ない
// ---------------------------------------------
void cAttack::updatePos()
{
	if( mLife <= -1.0f )			return;	//!< 攻撃してない
	if( tstFLG(mFlag,HITF_PAUSE) )	return;	//!< 一時停止中
	switch( mColType ){
	  case COL_SPHERE:	//!< 球	
		for( u8 i=0 ; i<mColNum ; i++ )	*(((TGL::Vec3*)mpColPosOld)+i) = ((TGL::ColSphere*)mpCol)[i].getPos();
		break;
	  case COL_LINE:	//!< 線	
		for( u8 i=0 ; i<mColNum ; i++ )	*(((TGL::Vec3*)mpColPosOld)+i) = ((TGL::ColLine*)mpCol)[i].getPosE();
		break;
	  case COL_DISABLE:	//!< 無効	
		break;
	  default:			//!< 不明	
		T_WARNING("当たり判定の型が不明");
		ASSERT(0);
	}
}

/* ======================================================================== */
/*		判定本体操作														*/
/* ======================================================================== */
// ---------------------------------------------
//! @brief  判定本体の終了
// ---------------------------------------------
void cAttack::endCollision()
{
	for( u8 i=0 ; i<mColNum ; i++ ){
		TGL::ColBase*	pCol = getColBase(i);
		if( pCol )	pCol->end();
	}
}
// ---------------------------------------------
//! @brief  判定本体を有効に
// ---------------------------------------------
void cAttack::enableCollision()
{
	if( tstFLG(mFlag, HITF_ANTI) )	return;	// 無効化されている
	orFLG( mFlag, HITF_ENABLE );
	for( u8 i=0 ; i<mColNum ; i++ ){
		TGL::ColBase*	pCol = getColBase(i);
		if( pCol )	pCol->enable();
	}
}
// ---------------------------------------------
//! @brief  判定本体を無効に
// ---------------------------------------------
void cAttack::disableCollision()
{
	clrFLG( mFlag, HITF_ENABLE );
	for( u8 i=0 ; i<mColNum ; i++ ){
		TGL::ColBase*	pCol = getColBase(i);
		if( pCol )	pCol->disable();
	}
}
// ---------------------------------------------
//! @brief  判定本体を無効化し、リセットがかかるまで有効にはならない
//! @note   当たり判定としては無効化したいが更新は続けたい場合などに使用する
// ---------------------------------------------
void cAttack::antiCollision()
{
	disableCollision();
	orFLG( mFlag, HITF_ANTI );
}

/* ======================================================================== */
/*		情報取得															*/
/* ======================================================================== */
// ---------------------------------------------
//! @brief  攻撃判定の座標を取得
//! @note   失敗した場合、ベクトルにはアクセスされずに返る
//! @param  index	[in]	取得する攻撃判定本体の配列番号
//! @return TGL::Vec3
// ---------------------------------------------
TGL::Vec3 cAttack::getPos( u8 index )
{
	switch( mColType ){
	  case COL_SPHERE:	//!< 球
		return ((TGL::ColSphere*)mpCol)[index].getPos();
	  case COL_LINE:	//!< 線
		return ((TGL::ColLine*)mpCol)[index].getPosE();
	  case COL_DISABLE:	//!< 無効
		return TGL::Vec3::ZERO;
	  default:			//!< 不明
		T_WARNING("当たり判定の型が不明");
		ASSERT(0);
	}
	return TGL::Vec3::ZERO;
}

// ---------------------------------------------
//! @brief  攻撃情報プリミティブ表示
// ---------------------------------------------
void cAttack::dispCollision()
{
	for( u8 i=0 ; i<mColNum ; i++ ){
		TGL::ColBase*	pCol = getColBase(i);
		if( pCol )	pCol->dispDebug2();
	}

	//!	移動方向
	TGL::Vec3	vS(TGL::Vec3::ZERO), vE(TGL::Vec3::ZERO);
	switch( mColType ){
	  case COL_SPHERE:
		if( tstFLG(mFlag,HITF_DELAY|HITF_ENABLE) && mpColPosOld ){	//!< 移動方向
			vS = ((TGL::ColSphere*)mpCol)->getPos();
			vE = *mpColPosOld;
		}
		break;
	  case COL_LINE:
		if( tstFLG(mFlag,HITF_DELAY|HITF_ENABLE) && mpColPosOld ){	//!< 移動方向
			vS = ((TGL::ColLine*)mpCol)->getPosE();
			vE = *mpColPosOld;
		}
		break;
	}
	if( vS.distance(vE) >= DELTA )
		IPrim->add( TGL::PrimLine(vS, vE, IRGBA(255,0,255,255), IRGBA(255,0,255,255)) );
}

// ---------------------------------------------
//! @brief  判定基底取得
//! @param  index	[in]	配列
//! @return 判定基底
// ---------------------------------------------
TGL::ColBase* cAttack::getColBase( u8 index )
{
	if( index >= mColNum )	return NULL;
	switch( mColType ){
	  case COL_SPHERE:	return &(((TGL::ColSphere*)mpCol)[index]);
	  case COL_LINE:	return &(((TGL::ColLine*)mpCol)[index]);
	}
	return NULL;
}
// ---------------------------------------------
//! @brief  判定基底取得
// ---------------------------------------------
const TGL::ColBase* cAttack::getColBaseConst( u8 index ) const
{
	if( index >= mColNum )	return NULL;
	switch( mColType ){
	  case COL_SPHERE:	return &(((TGL::ColSphere*)mpCol)[index]);
	  case COL_LINE:	return &(((TGL::ColLine*)mpCol)[index]);
	}
	return NULL;
}

/* ========================================================================= */
/*		EOF																	 */
/* ========================================================================= */
