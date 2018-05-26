// -------------------------------------------------------------------------- //
//! @file   cAttack.h
//! @brief  攻撃管理
//! @author Nal
//! @since  11/10/01(土)
//!
//! COPYRIGHT (C) 2011 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
#pragma	once
/* ====< インクルード >====================================================== */

/* ========================================================================= */
//		cAttack																 */
/* ========================================================================= */
class cAttack
{
public:
	cAttack();
	virtual ~cAttack();

public:	
	/* ========================================================================= */
	//		cAttack::Target														 */
	/* ========================================================================= */
	class Target
	{
	public:
		//!	コンストラクタ
		Target()
		:	mCombo(0), mTime(0)
		{}
		//!	デストラクタ
		virtual ~Target() {}
		
		//		関数		
		void	update( f32 fDelta ) {
			mTime += fDelta;
		}
		
		//		変数		
		u16		mCombo;		//!< 攻撃を当てた対象ごとのHIT数（maxCombo と関係）
		f32		mTime;		//!< 攻撃を当てた対象ごとのHIT経過時間（interval と関係）
	};

	// -----------------------------------------------------------------
	//		定義		
	// -----------------------------------------------------------------
	enum {
		//!	当たった相手のタイプ（mTargetType, mParentType）
		TYPE_UNKNOWN = 0,	//!< 未設定
		TYPE_CHR,			//!< キャラ
		TYPE_SHOT,			//!< 飛び道具
		TYPE_STG,			//!< 背景
		TYPE_FIELD,			//!< フィールド
		TYPE_MAX,

		//!	当たり判定の種類（mColType）
		COL_SPHERE,			//!< 当たり判定種類「球」
		COL_LINE,			//!< 当たり判定種類「線」
		COL_DISABLE,		//!< 判定しない

		//!	ターゲット種類（ATKCTRL_Check, ATKCTRL_Hitの引数として使用）
		HITCHKMODE_CHR	= 0,	//!< キャラ・球
		HITCHKMODE_FIELD,		//!< フィールド

		//!	フラグ（mFlag）
		HITF_ENABLE		= (1<< 0),	//!< 判定発生中
		HITF_DELAY		= (1<< 1),	//!< 判定発生前
		HITF_END		= (1<< 2),	//!< 判定終了後
		HITF_PAUSE		= (1<< 3),	//!< 一時停止中
		HITF_ANTI		= (1<< 4),	//!< 外部からリセットがかかった（更新はするが当たることは無い）
		HITF_FIELDHIT	= (1<< 5),	//!< フィールドヒット有効
		HITF_TALELINE	= (1<< 6),	//!< 座標バッファで線判定を作る
		HITF_DUMMY		= (1<< 7),	//!< ダミー判定
		HITF_08			= (1<< 8),	//!< 
		HITF_09			= (1<< 9),	//!< 
		
		//!	checkSousai返り値
		SOUSAI_DISABLE = 0,	//!< 相殺判定無効の組み合わせ
		SOUSAI_WIN_A,		//!< 左辺の勝利
		SOUSAI_WIN_B,		//!< 右辺の勝利
		SOUSAI_DRAW,		//!< 引き分け
		
		//!	culcHitWayタイプ
		HITWAY_2D = 0,	//!< 2D
		HITWAY_3D,		//!< 3D
	};

	// -----------------------------------------------------------------
	//		関数		
	// -----------------------------------------------------------------
	//!	セットアップ 球
	void	setup( u8 uType, u32 uId, TGL::ColSphere* pCol, TGL::Vec3* pvColPosOld, u8 uColNum ){
		setup( uType, uId, COL_SPHERE, pCol, pvColPosOld, uColNum );
	}
	//!	セットアップ 線
	void	setup( u8 uType, u32 uId, TGL::ColLine* pCol, TGL::Vec3* pvColPosOld, u8 uColNum ){
		setup( uType, uId, COL_LINE, pCol, pvColPosOld, uColNum );
	}
	//!	セットアップ 判定無効
	void	setup( u8 uType=TYPE_UNKNOWN, u32 uId=0 ){
		setup( uType, uId, COL_DISABLE, NULL, NULL, 0 );
	}

	//!	初期化
	void	reset();
	//!	開始
	void	start( s16 nAnmNo, f32 fNowTime, f32 fStartTime, f32 fEndTime );

	//!	攻撃判定の座標関連を設定する 球
	void	setCollisionSphere( const TGL::MtxGuide& Guide, f32 fRadius );
	//!	攻撃判定の座標関連を設定する 線
	void	setCollisionLine( const TGL::MtxGuide& GuideS, const TGL::MtxGuide& GuideE, f32 fWidth );

	//!	攻撃判定がターゲットに有効かチェック
	bool	checkTarget( const void* pTarget );
	//!	全攻撃判定と指定した判定とをチェックする
	bool	check( const TGL::ColBase& ColDmg, u32 uNotCheckFlag, TGL::ColBase::RET_T* pRet=NULL );
	//!	全攻撃判定と指定した判定とをチェックする。やられが「攻撃判定」（相殺用）
	bool	checkAtk( const cAttack& AtkT, TGL::ColBase::RET_T* pRet=NULL );
	//!	ヒット時に呼び出して、各値を更新する
	void	hit( void* pTarget );
	//!	攻撃判定同士の相殺優先判定を取る
	u8		checkSousai( const cAttack& AtkT );
	//!	攻撃判定同士の相殺優先判定を取る(ATKPOW_???で優劣をつける)
	u8		checkSousaiPowLevel( const cAttack& AtkT );
	//!	ヒット方向計算
	TGL::Vec3	culcHitWay( u8 uType, const TGL::Matrix& mtxBase, const TGL::Vec3& vPos );
	
	//!	攻撃判定更新。判定の発生と終了を管理する
	void	update( f32 fDelta );
	//!	フィールドヒット用パラメータ更新
	void	updateFieldHit( f32 fDelta );
	//!	攻撃判定の座標バッファ更新
	void	updatePos();

	//!	判定本体の終了
	void	endCollision();
	//!	判定本体を有効に
	void	enableCollision();
	//!	判定本体を無効に
	void	disableCollision();
	//!	判定本体を無効化し、リセットがかかるまで有効にはならない
	void	antiCollision();
	
	//!	攻撃判定の座標を取得
	TGL::Vec3		getPos( u8 index );
	//!	攻撃情報プリミティブ表示
	void	dispCollision();

	//		アクセサ		
	TGL_ACCESSOR_FLAG32( Flag, mFlag )
	TGL_ACCESSOR_FLAG32( AtkF, mAtkF )
	TGL_ACCESSOR_FLAG32( AtrF, mAtrF )
	TGL_ACCESSOR_FLAG32( ThrF, mThrF )
	f32			getDelay()				const	{ return mDelay; }
	f32			getLife()				const	{ return mLife; }
	f32			getDelayField()			const	{ return mDelayField; }
	f32			getLifeField()			const	{ return mLifeField; }
	s16			getMaxCombo()			const	{ return mMaxCombo; }
	void		setMaxCombo( s16 i0 )			{ mMaxCombo = i0; }
	s16			getEndTotalCombo()		const	{ return mEndTotalCombo; }
	void		setEndTotalCombo( s16 i0 )		{ mEndTotalCombo = i0; }
	f32			getInterval()			const	{ return mInterval; }
	void		setInterval( f32 f0 )			{ mInterval = f0; }
	u8			getPowLevel()			const	{ return mPowLevel; }
	void		setPowLevel( u8 i0 )			{ mPowLevel = i0; }
	u8			getPointType()			const	{ return mPointType; }
	void		setPointType( u8 i0 )			{ mPointType = i0; }
	u8			getHardType()			const	{ return mHardType; }
	void		setHardType( u8 i0 )			{ mHardType = i0; }
	u8			getUserId()			const		{ return mUserId; }
	void		setUserId( u8 i0 )				{ mUserId = i0; }
	u8			getShotEraseLv()		const	{ return mShotEraseLv; }
	void		setShotEraseLv( u8 i0 )			{ mShotEraseLv = i0; }
	u8			getDmgStaLand()			const	{ return mDmgStaLand; }
	void		setDmgStaLand( u8 i0 )			{ mDmgStaLand = i0; }
	u8			getDmgStaAir()			const	{ return mDmgStaAir; }
	void		setDmgStaAir( u8 i0 )			{ mDmgStaAir = i0; }
	f32			getPower()				const	{ return mPower; }
	void		setPower( f32 f0 )				{ mPower = f0; }
	f32			getPowerGuard()			const	{ return mPowerGuard; }
	void		setPowerGuard( f32 f0 )			{ mPowerGuard = f0; }
	f32			getPowerSelf()			const	{ return mPowerSelf; }
	void		setPowerSelf( f32 f0 )			{ mPowerSelf = f0; }
	f32			getPowerPlayer()		const	{ return mPowerPlayer; }
	void		setPowerPlayer( f32 f0 )		{ mPowerPlayer = f0; }
	f32			getPowerEnemy()			const	{ return mPowerEnemy; }
	void		setPowerEnemy( f32 f0 )			{ mPowerEnemy = f0; }
	f32			getStiffDamage()		const	{ return mStiffDamage; }
	void		setStiffDamage( f32 f0 )		{ mStiffDamage = f0; }
	f32			getStiffGuard()			const	{ return mStiffGuard; }
	void		setStiffGuard( f32 f0 )			{ mStiffGuard = f0; }
	f32			getStiffDown()			const	{ return mStiffDown; }
	void		setStiffDown( f32 f0 )			{ mStiffDown = f0; }
	f32			getDmgSpd()				const	{ return mDmgSpd; }
	void		setDmgSpd( f32 f0 )				{ mDmgSpd = f0; }
	void		setDmgSpd( f32 f0, f32 f1 )		{ mDmgSpd = f0;	 mDmgAcl = f1; }
	f32			getDmgAcl()				const	{ return mDmgAcl; }
	void		setDmgAcl( f32 f0 )				{ mDmgAcl = f0; }
	f32			getGuardSpd()			const	{ return mGuardSpd; }
	void		setGuardSpd( f32 f0 )			{ mGuardSpd = f0; }
	void		setGuardSpd( f32 f0, f32 f1 )	{ mGuardSpd = f0;	 mGuardAcl = f1; }
	f32			getGuardAcl()			const	{ return mGuardAcl; }
	void		setGuardAcl( f32 f0 )			{ mGuardAcl = f0; }
	f32			getFloatSpd()			const	{ return mFloatSpd; }
	void		setFloatSpd( f32 f0 )			{ mFloatSpd = f0; }
	void		setFloatSpd( f32 f0, f32 f1 )	{ mFloatSpd = f0;	 mFloatAcl = f1; }
	f32			getFloatAcl()			const	{ return mFloatAcl; }
	void		setFloatAcl( f32 f0 )			{ mFloatAcl = f0; }
	f32			getBoundSpd()			const	{ return mBoundSpd; }
	void		setBoundSpd( f32 f0 )			{ mBoundSpd = f0; }
	void		setBoundSpd( f32 f0, f32 f1 )	{ mBoundSpd = f0;	 mBoundAcl = f1; }
	f32			getBoundAcl()			const	{ return mBoundAcl; }
	void		setBoundAcl( f32 f0 )			{ mBoundAcl = f0; }
	s8			getHitStop()			const	{ return mHitStop; }
	void		setHitStop( s8 i0 )				{ mHitStop = i0; }
	s8			getHitStopDelay()		const	{ return mHitStopDelay; }
	void		setHitStopDelay( s8 i0 )		{ mHitStopDelay = i0; }
	s8			getJCancelDelay()		const	{ return mJCancelDelay; }
	void		setJCancelDelay( s8 i0 )		{ mJCancelDelay = i0; }
	s16			getAirTime()			const	{ return mAirTime; }
	void		setAirTime( s16 i0 )			{ mAirTime = i0; }
	s16			getAirTimeOfs()			const	{ return mAirTimeOfs; }
	void		setAirTimeOfs( s16 i0 )			{ mAirTimeOfs = i0; }
	f32			getAirDist()			const	{ return mAirDist; }
	void		setAirDist( f32 f0 )			{ mAirDist = f0; }
	f32			getAirSpeed()			const	{ return mAirSpeed; }
	void		setAirSpeed( f32 f0 )			{ mAirSpeed = f0; }
	f32			getAirStiffUkemi()		const	{ return mAirStiffUkemi; }
	void		setAirStiffUkemi( f32 f0 )		{ mAirStiffUkemi = f0; }
	s16			getThrowNo()			const	{ return mThrowNo; }
	void		setThrowNo( s16 i0 )			{ mThrowNo = i0; }
	s16			getTdownNo()			const	{ return mTdownNo; }
	void		setTdownNo( s16 i0 )			{ mTdownNo = i0; }
	s16			getEftId( u8 index )	const	{ return mpEftId[index]; }
	void		setEftId( u8 index, s16 i0 )	{ mpEftId[index] = i0; }
	s16			getSeId( u8 index )		const	{ return mpSeId[index]; }
	void		setSeId( u8 index, s16 i0 )		{ mpSeId[index] = i0; }
	s16			getSwingSeId()			const	{ return mSwingSeId; }
	void		setSwingSeId( s16 i0 )			{ mSwingSeId = i0; }
	const TGL::Vec3&	getWayOfs()				const	{ return mWayOfs; }
	void		setWayOfs( const TGL::Vec3& v0 )		{ mWayOfs = v0; }
	f32			getWayRand()			const	{ return mWayRand; }
	void		setWayRand( f32 f0 )			{ mWayRand = f0; }
	s16			getCamShakeTime()		const	{ return mCamShakeTime; }
	void		setCamShakeTime( s16 i0 )		{ mCamShakeTime = i0; }
	f32			getCamShake()			const	{ return mCamShake; }
	void		setCamShake( f32 f0 )			{ mCamShake = f0; }
	f32			getScoreRate()			const	{ return mScoreRate; }
	void		setScoreRate( f32 f0 )			{ mScoreRate = f0; }
	f32			getScoreRateAdd()		const	{ return mScoreRateAdd; }
	void		setScoreRateAdd( f32 f0 )		{ mScoreRateAdd = f0; }
	f32			getScoreRateAddFin()	const	{ return mScoreRateAddFin; }
	void		setScoreRateAddFin( f32 f0 )	{ mScoreRateAddFin = f0; }
	u16			getTotalCombo()			const	{ return mTotalCombo; }
	void		setTotalCombo( u16 i0 )			{ mTotalCombo = i0; }
	f32			getCountTime()			const	{ return mCountTime; }
	void		setCountTime( f32 f0 )			{ mCountTime = f0; }
	f32			getEndTime()			const	{ return mEndTime; }
	void		setEndTime( f32 f0 )			{ mEndTime = f0; }
	s16			getAnmNo()				const	{ return mAnmNo; }
	void		setAnmNo( s16 i0 )				{ mAnmNo = i0; }

private:
	// -----------------------------------------------------------------
	//		定義		
	// -----------------------------------------------------------------

	// -----------------------------------------------------------------
	//		関数		
	// -----------------------------------------------------------------
	//!	セットアップ
	void	setup( u8 uType, u32 uId, u8 uColType, TGL::ColBase* pCol, TGL::Vec3* pvColPosOld, u8 uColNum );
	
	//!	判定基底取得
	TGL::ColBase*		getColBase( u8 index );
	const TGL::ColBase*	getColBaseConst( u8 index ) const;

	// -----------------------------------------------------------------
	//		変数		
	// -----------------------------------------------------------------
	u32		mFlag;					//!< FLAG_????
	f32		mDelay;					//!< [PARAMETER] 判定発生までの時間
	f32		mLife;					//!< [PARAMETER] 判定消滅までの時間
	f32		mDelayField;			//!< [PARAMETER] フィールドヒット判定発生までの時間（0未満で無効）
	f32		mLifeField;				//!< [PARAMETER] フィールドヒット判定消滅までの時間（0未満で無効）
	
	s16		mMaxCombo;				//!< [PARAMETER] 同じ対象にヒットする最大回数（0以下で無制限）
	s16		mEndTotalCombo;			//!< [PARAMETER] 判定消滅するヒット回数（0以下で無制限 / 現在の仕様だと更新時に判定される。つまり同フレーム内ではいくらヒットさせても消えない）
	f32		mInterval;				//!< [PARAMETER] 複数ヒットする際の間隔時間（0未満で１回のみ）

	u32		mAtkF;					//!< [PARAMETER] ATKF_???? 攻撃フラグ
	u32		mAtrF;					//!< [PARAMETER] ATRF_???? 属性フラグ
	u32		mThrF;					//!< [PARAMETER] THRF_???? 投げフラグ

	u8		mPowLevel;				//!< [PARAMETER] ATKPOW_???? 威力段階
	u8		mPointType;				//!< [PARAMETER] ATKPNT_???? 攻撃箇所
	u8		mHardType;				//!< [PARAMETER] ATKHARD_???? 強度
	u8		mUserId;				//!< [PARAMETER] ATKUSERID_???? 特殊判別用ID
	u8		mShotEraseLv;			//!< [PARAMETER] SHOTERASE_???? ショット相殺レベル
	u8		mDmgStaLand;			//!< [PARAMETER] ATKDMGSTA_???? 地上ダメージステータス（ダメージモーション影響）
	u8		mDmgStaAir;				//!< [PARAMETER] ATKDMGSTA_???? 空中ダメージステータス（ダメージモーション影響）

	f32		mPower;					//!< [PARAMETER] 攻撃力	[初期値10]
	f32		mPowerGuard;			//!< [PARAMETER] ガード削り率（初期値0.125）
	f32		mPowerSelf;				//!< [PARAMETER] 自身への攻撃力	[0:無効]
	f32		mPowerPlayer;			//!< [PARAMETER] プレイヤーへの攻撃力倍率
	f32		mPowerEnemy;			//!< [PARAMETER] 敵への攻撃力倍率
	f32		mStiffDamage;			//!< [PARAMETER] ダメージ硬直加算値
	f32		mStiffGuard;			//!< [PARAMETER] ガード硬直加算値
	f32		mStiffDown;				//!< [PARAMETER] ダウン硬直指定値
	f32		mDmgSpd;				//!< [PARAMETER] ヒットバック 初速
	f32		mDmgAcl;				//!< [PARAMETER] ヒットバック 加速度
	f32		mGuardSpd;				//!< [PARAMETER] ガードバック 初速
	f32		mGuardAcl;				//!< [PARAMETER] ガードバック 加速度
	f32		mFloatSpd;				//!< [PARAMETER] 空中コンボ時の自身のYSpd代入値
	f32		mFloatAcl;				//!< [PARAMETER] 空中コンボ時の自身のYAcl代入値
	f32		mBoundSpd;				//!< [PARAMETER] バウンド量指定叩きつけのバウンド初速
	f32		mBoundAcl;				//!< [PARAMETER] バウンド量指定叩きつけのバウンド重力
	s8		mHitStop;				//!< [PARAMETER] ヒットストップ値（0未満で無効）
	s8		mHitStopDelay;			//!< [PARAMETER] ヒットストップディレイ値（判定発生からの相対時間）
	s8		mJCancelDelay;			//!< [PARAMETER] ジャンプキャンセルディレイ値（判定発生からの相対時間）

	s16		mAirTime;				//!< [PARAMETER] 浮かしパラメータ 着地までの時間（0以下で無効）
	s16		mAirTimeOfs;			//!< [PARAMETER] 浮かしパラメータ 最高高度に達する時間オフセット（0で無効）
	f32		mAirDist;				//!< [PARAMETER] 浮かしパラメータ 距離
	f32		mAirSpeed;				//!< [PARAMETER] 浮かしパラメータ 初速
	f32		mAirStiffUkemi;			//!< [PARAMETER] 浮かしパラメータ 受身不可時間

	s16		mThrowNo;				//!< [PARAMETER] 投げ番号。		（ヒット時に自分が移行する先）ATRF_THROWと同時に指定すると有効
	s16		mTdownNo;				//!< [PARAMETER] 投げられ番号。	（ヒット時に相手が移行する先）ATRF_THROWと同時に指定すると有効

	s16*	mpEftId;				//!< [PARAMETER] ヒットエフェクト
	s16*	mpSeId;					//!< [PARAMETER] ヒット音
	s16		mSwingSeId;				//!< [PARAMETER] 判定発生瞬間に再生される効果音
	TGL::Vec3		mWayOfs;				//!< [PARAMETER] ヒット方向計算用オフセット
	f32		mWayRand;				//!< [PARAMETER] ヒット方向ランダム範囲
	s16		mCamShakeTime;			//!< [PARAMETER] カメラ振動時間 [0未満で自動設定]
	f32		mCamShake;				//!< [PARAMETER] カメラ振動値

	f32		mScoreRate;				//!< [PARAMETER] スコアテンション影響率
	f32		mScoreRateAdd;			//!< [PARAMETER] スコアテンション加算 撃ち込み
	f32		mScoreRateAddFin;		//!< [PARAMETER] スコアテンション加算 撃破

	u16		mTotalCombo;			//!< 現在のヒット回数
	f32		mCountTime;				//!< 判定発生からの経過時間
	f32		mEndTime;				//!< 判定終了からの経過時間
	
	s16		mAnmNo;					//!< 攻撃設定時のアニメ番号。ヒット時に変更されていた場合コンボに繋がったりしない
	
	stl::map<xu32,Target>	mTarget;	//!< 攻撃を当てた対象
	
	//!	ここから下はCreate時に設定され、消されることは無い
	u8		mParentType;			//!< 本体の種類
	u32		mParentId;				//!< 本体のID
	
	u8				mColType;		//!< 当たり判定の種類
	TGL::ColBase*	mpCol;			//!< 当たり判定の先頭
	TGL::Vec3*			mpColPosOld;	//!< 当たり判定の前回の座標の先頭
	u8				mColNum;		//!< 当たり判定の数
};

/* ========================================================================= */
/*		EOF																	 */
/* ========================================================================= */
