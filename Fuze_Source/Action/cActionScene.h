// -------------------------------------------------------------------------- //
//! @file   cActionScene.h
//! @brief  シーン制御
//! @author Nal
//! @since  11/11/10(木)
//!
//! COPYRIGHT (C) 2011 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
#pragma	once
/* ====< インクルード >====================================================== */

/* ========================================================================= */
//		cActionScene														 */
/* ========================================================================= */
class cActionScene
{
public:
	cActionScene();
	virtual ~cActionScene();

public:	
	// -----------------------------------------------------------------
	//		定義		
	// -----------------------------------------------------------------
	enum {
		//!	初期化モード
		RESET_ALL = 0,	//!< 完全
		RESET_RESTART,	//!< リスタート
		RESET_STAGE,	//!< ステージ移行
	};

	// -----------------------------------------------------------------
	//		関数		
	// -----------------------------------------------------------------
	//!	初期化
	void	reset( u8 uMode=RESET_ALL );
	
	//!	リソース解放
	void	freeResource();
	//!	シーンスクリプト解放
	void	freeSceneScript()		{ freeScript(&mpTgsScene, mScenePath); }
	//!	リザルトスクリプト解放
	void	freeResultScript()		{ freeScript(&mpTgsResult, mResultPath); }
	//!	ゲームオーバースクリプト解放
	void	freeGameoverScript()	{ freeScript(&mpTgsGameover, mGameoverPath); }

	//!	シーンスクリプト読み込み＆開始
	void	beginSceneScript();
	//!	リザルトスクリプト読み込み＆開始
	void	beginResultScript();
	//!	ゲームオーバースクリプト読み込み＆開始
	void	beginGameoverScript();

	//!	更新
	void	update();
	
	//!	デモ開始
	void	startDemo( u32 uDemoF );
	//!	デモ終了
	void	endDemo();

	//!	敗北判定
	bool	isLose();

	//!	現在ステージをACT名で取得
	const char*	getStageAct();

	//		アクセサ		
	u32			getActiveFrame() const		{ return mActiveFrame; }
	u8			getStage() const			{ return mStage; }
	void		setStage( u8 i0 )			{ mStage = i0; }
	void		setStage( u8 i0, u8 i1 )	{ mStage = i0;	mStageArea = i1; }
	u8			getStageArea() const		{ return mStageArea; }
	void		setStageArea( u8 i0 ) 		{ mStageArea = i0; }
	bool		isContinueBgm() const 		{ return mbContinueBgm; }
	void		setContinueBgm( bool b0 ) 	{ mbContinueBgm = b0; }
	void		resumeContinueStageArea() 	{ mStageArea = mContinueArea; }
	void		setContinueArea( u8 i0 ) 	{ mContinueArea = i0; }
	u8			getStageOrder() const		{ return mStageOrder; }
	void		setStageOrder( u8 i0 )		{ mStageOrder = i0; }
	f32			getTotalTime() const		{ return mTotalTime; }
	f32			getStageTime() const		{ return mStageTime; }
	void		setStageTime( f32 f0 )		{ mStageTime = f0; }
	u8			getStep() const				{ return mStep; }
	void		setStep( u8 i0 )			{ mStep = i0; }
	f32			getBossTime() const			{ return mBossTime; }
	void		setBossTime( f32 f0 )		{ mBossTime = f0; }
	f32			getBossTimeLimit() const	{ return mBossTimeLimit; }
	void		setBossTimeLimit( u16 i0 )	{ mBossTimeLimit = i0; }
	f32			getBossRestTime() const		{ return TGL::Math::Clamp<f32>( (f32)mBossTimeLimit-mBossTime, 0, 9999999.0f ); }
	f32			getDemoTime() const			{ return mDemoTime; }
	const TGL::Vec3&	getPosFix() const			{ return mPosFix; }
	void		setPosFix( const TGL::Vec3& v0 )	{ mPosFix = v0; }
	u8			getScrollType() const		{ return mScrollType; }
	void		setScrollType( u8 i0 )		{ mScrollType = i0; }
	TGL_ACCESSOR_FLAG32( SceneF, mSceneF )
	bool		isDemo() const;
	u32			getDemoF() const			{ return mDemoF; }
	u32			tstDemoF( u32 i0 ) const{
					if( !isDemo() )	return 0;
					return tstFLG(mDemoF, i0);
				}
	void		setDemoF( u32 i0 )			{ mDemoF = i0; }
	void		orDemoF( u32 i0 )			{ orFLG(mDemoF, i0); }
	void		clrDemoF( u32 i0 )			{ clrFLG(mDemoF, i0); }
	void		xorDemoF( u32 i0 )			{ xorFLG(mDemoF, i0); }
	TGL_ACCESSOR_FLAG32( EventF, mEventF )
	s32			getEventWorkI() const		{ return mEventWorkI; }
	void		setEventWorkI( s32 i0 ) 	{ mEventWorkI = i0; }
	TGL::TransVec&	getListenerOfs() 		{ return mListenerOfs; }
	

private:
	// -----------------------------------------------------------------
	//		定義		
	// -----------------------------------------------------------------

	// -----------------------------------------------------------------
	//		関数		
	// -----------------------------------------------------------------
	//!	スクリプト解放
	void	freeScript( TGSTSK_T** pTgs, stl::string& Path );

	// -----------------------------------------------------------------
	//		変数		
	// -----------------------------------------------------------------
	u32				mActiveFrame;	//!< アクティブ経過フレーム
	TGSTSK_T*		mpTgsScene;		//!< シーンスクリプト
	stl::string		mScenePath;		//!< シーンスクリプトパス
	TGSTSK_T*		mpTgsResult;	//!< リザルトスクリプト
	stl::string		mResultPath;	//!< リザルトスクリプトパス
	TGSTSK_T*		mpTgsGameover;	//!< ゲームオーバースクリプト
	stl::string		mGameoverPath;	//!< ゲームオーバースクリプトパス
	u8				mStage;			//!< ステージ
	u8				mStageArea;		//!< ステージ開始エリア
	u8				mContinueArea;	//!< コンティニュー時のエリア
	bool			mbContinueBgm;	//!< コンティニュー時にBGM継続
	u8				mStageOrder;	//!< ステージ予約
	f32				mTotalTime;		//!< 経過時間
	f32				mStageTime;		//!< ステージ経過
	u8				mStep;			//!< SCENESTEP_????
	u16				mBossTimeLimit;	//!< ボス戦制限時間
	f32				mBossTime;		//!< ボス戦経過
	u32				mSceneF;		//!< SCENEF_????
	TGL::Vec3		mPosFix;		//!< プレイヤーの座標制限
	u8				mScrollType;	//!< SCROLL_????
	u32				mDemoF;			//!< DEMOF_????
	f32				mDemoTime;		//!< デモ経過
	u32				mEventF;		//!< EVENTF_????
	s32				mEventWorkI;	//!< 同期ワーク
	TGL::TransVec	mListenerOfs;	//!< サウンドリスナーオフセット
};

/* ========================================================================= */
/*		EOF																	 */
/* ========================================================================= */
