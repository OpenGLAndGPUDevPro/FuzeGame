// -------------------------------------------------------------------------- //
//! @file   cActionTime.h
//! @brief  時間制御
//! @author Nal
//! @since  11/10/14(金)
//!
//! COPYRIGHT (C) 2011 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
#pragma	once
/* ====< インクルード >====================================================== */

/* ========================================================================= */
//		cActionTime															 */
/* ========================================================================= */
class cActionTime
{
public:
	cActionTime();
	virtual ~cActionTime();

public:	
	// -----------------------------------------------------------------
	//		定義		
	// -----------------------------------------------------------------
	enum {
		//!	カットインポーズ
		CUTINPAUSE_NONE = 0,
		CUTINPAUSE_SYSATK,
		CUTINPAUSE_EXATK,
	};

	// -----------------------------------------------------------------
	//		関数		
	// -----------------------------------------------------------------
	//!	初期化
	void	reset();
	
	//!	更新
	void	update();
	
	//!	ヒットスロウ設定
	void	setHitSlow( s32 nDelay, s32 nFrame, f32 fRate, tCharacter* pAtk, tCharacter* pDmg );
	//!	ヒットスロウ終了
	void	endHitSlow();
	//!	フィニッシュスロウのリクエスト
	void	reqFinishSlow();
	//!	フィニッシュスロウレート取得
	f32		getFinishSlow() const;
	//!	フィニッシュ後かどうか
	bool	isFinished() const	{ return mbFinished; }

	//!	ヒットスロウレート取得
	f32		getHitSlow() const	{ return mHitSlow; }
	//!	カットインポーズ取得
	f32		getCutinPause( tCharacter* pChr=NULL ) const;
	//!	演出スロウレート取得		
	TGL::TransFloat&	getDemoSlow()	{ return mDemoSlow; }
	
	//!	時間レート取得（キャラ以外から全体の時間が取りたい場合に使用）
	f32		getRateStd() const	{ return ITimer->getRate() * getHitSlow() * getFinishSlow() * getCutinPause() * mDemoSlow.getValue(); }
	//!	時間レート取得（カメラ）
	f32		getRateCamera() const;
	//!	時間レート取得（エフェクト）
	f32		getRateEffect() const;
	
	//!	時間停止中かどうか
	bool	isStop() const	{ return (getRateStd() < DELTA); }

	//		アクセサ		
	f32		getHitSlowTime() const			{ return mHitSlowTime; }
	u32		tstDemoSlowFlag( u32 i0 ) const	{ return tstFLG( mDemoSlowFlag, i0 ); }
	void	setDemoSlowFlag( u32 i0 ) 		{ setFLG( mDemoSlowFlag, i0 ); }
	void	orDemoSlowFlag( u32 i0 ) 		{ orFLG( mDemoSlowFlag, i0 ); }
	void	clrDemoSlowFlag( u32 i0 ) 		{ clrFLG( mDemoSlowFlag, i0 ); }

private:
	// -----------------------------------------------------------------
	//		定義		
	// -----------------------------------------------------------------

	// -----------------------------------------------------------------
	//		関数		
	// -----------------------------------------------------------------

	// -----------------------------------------------------------------
	//		変数		
	// -----------------------------------------------------------------
	f32			mHitSlowTime;		//!< ヒットスロウ有効時間	[frame]
	f32			mHitSlowDelay;		//!< ヒットスロウ設定ディレイ	[frame]
	f32			mHitSlowOrder;		//!< ヒットスロウレート予約値	[rate]
	f32			mHitSlow;			//!< ヒットスロウレート	[rate]
	tCharacter*	mpHitSlowAtkChr;	//!< ヒットスロウ攻撃キャラ
	tCharacter*	mpHitSlowDmgChr;	//!< ヒットスロウダメージキャラ
	
	bool		mbFinished;			//!< フィニッシュ後
	f32			mFinishSlowTime;	//!< フィニッシュスロウ経過

	tCharacter*	mpPauseChr;			//!< 反映されている停止リクエスト中のキャラ
	u8			mPauseMode;			//!< ポーズモード
	
	TGL::TransFloat	mDemoSlow;		//!< 演出スロウ
	u32				mDemoSlowFlag;	//!< 演出スロウフラグ
};

/* ========================================================================= */
/*		EOF																	 */
/* ========================================================================= */
