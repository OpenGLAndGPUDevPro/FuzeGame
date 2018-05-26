// -------------------------------------------------------------------------- //
//! @file   cAttackInfo.h
//! @brief  攻撃命中時に記憶する情報
//! @author Nal
//! @since  13/12/14(土)
//!
//! COPYRIGHT (C) 2013 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
#pragma	once
/* ====< インクルード >====================================================== */
#include "cAttackTarget.h"

/* ========================================================================= */
//		cAttackInfo															 */
/* ========================================================================= */
class cAttackInfo
{
public:
	//!	コンストラクタ
	cAttackInfo() {
		reset();
	}
	//!	コンストラクタ
	cAttackInfo( const cAttackTarget& target, s16 nAnmNo, u32 uAtkF, u32 uAtrF, u32 uThrF, f32 fDamage, const TGL::Vec3& vWay )
	:	mTarget(target), mAnmNo(nAnmNo), mAtkF(uAtkF), mAtrF(uAtrF), mThrF(uThrF), mDamage(fDamage), mWay(vWay)
	{}
	
	//!	デストラクタ
	virtual ~cAttackInfo() {}

public:	
	// -----------------------------------------------------------------
	//		関数		
	// -----------------------------------------------------------------
	//!	初期化
	void	reset() {
		mAnmNo	= 0;
		mAtkF	= 0;
		mAtrF	= 0;
		mThrF	= 0;
		mWay	= TGL::Vec3::ZERO;
	}

	//		アクセサ		
	cAttackTarget&	getTarget()	{ return mTarget; }
	s16		getAnmNo() const	{ return mAnmNo; }
	void	setAnmNo( s16 i0 )	{ mAnmNo = i0; }
	TGL_ACCESSOR_FLAG32( AtkF, mAtkF )
	TGL_ACCESSOR_FLAG32( AtrF, mAtrF )
	TGL_ACCESSOR_FLAG32( ThrF, mThrF )
	f32		getDamage() const	{ return mDamage; }
	void	setDamage( f32 f0 )	{ mDamage = f0; }
	const TGL::Vec3&	getWay() const					{ return mWay; }
	void				setWay( const TGL::Vec3& v0 )	{ mWay = v0; }

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
	cAttackTarget	mTarget;	//!< 対象
	s16				mAnmNo;		//!< 攻撃側のアニメ番号
	u32				mAtkF;		//!< ATKF_????
	u32				mAtrF;		//!< ATRF_????
	u32				mThrF;		//!< THRF_????
	f32				mDamage;	//!< ダメージ量
	TGL::Vec3		mWay;		//!< ヒット方向
};

/* ========================================================================= */
/*		EOF																	 */
/* ========================================================================= */
