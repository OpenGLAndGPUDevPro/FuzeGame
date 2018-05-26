// -------------------------------------------------------------------------- //
//! @file   cChrAbility.h
//! @brief  アビリティ
//! @author Nal
//! @since  11/09/28(水)
//!
//! COPYRIGHT (C) 2011 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
#pragma	once
/* ====< インクルード >====================================================== */
#include "../Core/tCharacter.h"

/* ========================================================================= */
//		cChrAbility															 */
/* ========================================================================= */
class tCharacter;

class cChrAbility
{
public:
	cChrAbility( tCharacter* pChr );
	virtual ~cChrAbility();

public:	
	/* ========================================================================= */
	//		cChrAbility::Sub													 */
	/* ========================================================================= */
	class Sub
	{
	public:
		Sub();
		virtual ~Sub();
		
		//		関数		
		//!	初期化
		void	reset();
		//!	更新
		void	update( const tCharacter& Chr, f32 fDelta );

		//!	有効かどうか
		bool	isActive()	const { return (mType != ABLTYPE_NONE); }
		
		//		アクセサ		
		//!	管理フラグ
		u32		getFlag()			const	{ return mFlag; }
		void	setFlag( u32 i0 )			{ mFlag = i0; }
		//!	アビリティタイプ
		u8		getType()			const	{ return mType; }
		void	setType( u8 i0 )			{ mType = i0; }
		//!	生成した時のアニメ番号
		s16		getStartAnmNo()		const	{ return mStartAnmNo; }
		void	setStartAnmNo( s16 i0 )		{ mStartAnmNo = i0; }
		//!	残り有効時間。0以下なら無限
		f32		getCount()			const	{ return mCount; }
		void	setCount( f32 f0 )			{ mCount = f0; }
		//!	キャラに付加されるアビリティフラグ。事前設定系
		u32		getAblF()			const	{ return mAblF; }
		void	setAblF( u32 i0 )			{ mAblF = i0; }
		void	orAblF( u32 i0 )			{ orFLG( mAblF, i0 ); }
		void	clrAblF( u32 i0 )			{ clrFLG( mAblF, i0 ); }
		void	xorAblF( u32 i0 )			{ xorFLG( mAblF, i0 ); }
		//!	HP加算値
		f32		getHPAdd()			const	{ return mHPAdd; }
		void	setHPAdd( f32 f0 )			{ mHPAdd = f0; }
		//!	HP減算値
		f32		getHPSub()			const	{ return mHPSub; }
		void	setHPSub( f32 f0 )			{ mHPSub = f0; }
		//!	最大HP補正率		[rate]
		f32		getHPMaxRate()			const	{ return mHPMaxRate; }
		void	setHPMaxRate( f32 f0 )			{ mHPMaxRate = f0; }
		//!	攻撃力補正率			[rate]
		f32		getAttackRate()			const	{ return mAttackRate; }
		void	setAttackRate( f32 f0 )			{ mAttackRate = f0; }
		//!	ダメージ補正率			[rate]
		f32		getDamageRate()			const	{ return mDamageRate; }
		void	setDamageRate( f32 f0 )			{ mDamageRate = f0; }
		//!	動作速度補正率	[rate]
		f32		getSpeedRate()			const	{ return mSpeedRate; }
		void	setSpeedRate( f32 f0 )			{ mSpeedRate = f0; }
		//!	スーパーアーマー ダメージ補正率	[rate / 0以上で有効]
		f32		getSpArmorDmgRate()			const	{ return mSpArmorDmgRate; }
		void	setSpArmorDmgRate( f32 f0 )			{ mSpArmorDmgRate = f0; }
		//!	スーパーアーマー 耐久値	[0以下で無限]
		s16		getSpArmorShield()			const	{ return mSpArmorShield; }
		void	setSpArmorShield( s16 i0 )			{ mSpArmorShield = i0; }
		void	addSpArmorShield( s16 i0 )			{ mSpArmorShield += i0; }
		//!	汎用整数。アビリティタイプ毎に自由に使用可
		s32		getUserI( u8 index ) const;
		void	setUserI( u8 index, s32 i0 );
		//!	汎用実数。アビリティタイプ毎に自由に使用可
		f32		getUserF( u8 index ) const;
		void	setUserF( u8 index, f32 f0 );
		
	private:
		//		関数		
		//!	終了判定
		bool	updateEnd( const tCharacter& Chr );

		//		変数		
		u32		mFlag;				//!< 管理フラグ
		u8		mType;				//!< アビリティタイプ
		s16		mStartAnmNo;		//!< 生成した時のアニメ番号
		f32		mCount;				//!< 残り有効時間。0以下なら無限
		
		u32		mAblF;				//!< キャラに付加されるアビリティフラグ
		
		f32		mHPAdd;				//!< HP加算値
		f32		mHPSub;				//!< HP減算値
		
		f32		mHPMaxRate;			//!< 最大HP補正率		[rate]
		f32		mAttackRate;		//!< 攻撃力補正率		[rate]
		f32		mDamageRate;		//!< ダメージ補正率		[rate]
		f32		mSpeedRate;			//!< 動作速度補正率		[rate]
		f32		mSpArmorDmgRate;	//!< スーパーアーマー ダメージ補正率	[rate / 0以上で有効]
		s16		mSpArmorShield;		//!< スーパーアーマー 耐久値	[0以下で無限]
		
		s32		mUserI[4];			//!< 汎用整数。アビリティタイプ毎に自由に使用可
		f32		mUserF[4];			//!< 汎用実数。アビリティタイプ毎に自由に使用可
	};

public:	
	// -----------------------------------------------------------------
	//		関数		
	// -----------------------------------------------------------------
	//!	リセット
	void	reset();

	//!	更新
	void	update();

	//!	指定アビリティタイプのアビリティを取得
	Sub*	get( u8 uType );
	//!	指定配列のアビリティを取得
	Sub*	getFromIndex( u8 uIndex );
	//!	アビリティを生成
	Sub*	create( u8 uType, u32 AblF=0, bool bOver=false );
	//!	指定タイプのアビリティを削除
	void	kill( u8 uType );

	//!	全アビリティを加味したABLFを取得
	u32		getAblF() const;
	//!	全アビリティを加味したHP加算値を取得
	f32		getHPAdd() const;
	//!	全アビリティを加味したHP減算値を取得
	f32		getHPSub() const;
	//!	全アビリティを加味したHP最大補正率を取得
	f32		getHPMaxRate() const;
	//!	全アビリティを加味した攻撃力補正率を取得
	f32		getAttackRate() const;
	//!	全アビリティを加味したダメージ補正率を取得
	f32		getDamageRate() const;
	//!	全アビリティを加味した動作速度補正率を取得
	f32		getSpeedRate() const;
	//!	スーパーアーマーのダメージ率最低値を取得
	f32		getSpArmorDmgRate( s16 nDmgHeap ) const;
	//!	スーパーアーマーのダメージヒープ減算による破壊
	void	subSpArmorShield( s16 nDmgHeap );
	//!	ベースアビリティに or ABLF
	void	orBaseAblF( u32 i0 );
	//!	ベースアビリティに clr ABLF
	void	clrBaseAblF( u32 i0 );
	//!	ベースアビリティに xor ABLF
	void	xorBaseAblF( u32 i0 );

	//!	オーナー取得
	tCharacter*	getChr()	{ return mpChr; }

	//		アクセサ		

private:
	// -----------------------------------------------------------------
	//		関数		
	// -----------------------------------------------------------------

	// -----------------------------------------------------------------
	//		変数		
	// -----------------------------------------------------------------
	tCharacter*	mpChr;		//!< オーナー
	Sub			mSub[8];	//!< 中身
};

/* ========================================================================= */
/*		EOF																	 */
/* ========================================================================= */
