// -------------------------------------------------------------------------- //
//! @file   cAttackTarget.h
//! @brief  攻撃対象
//! @author Nal
//! @since  13/12/14(土)
//!
//! COPYRIGHT (C) 2013 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
#pragma	once
/* ====< インクルード >====================================================== */
#include "Action/Character/sCharacter.h"

/* ========================================================================= */
//		cAttackTarget														 */
/* ========================================================================= */
class cAttackTarget
{
public:
	cAttackTarget();
	cAttackTarget( cChrStatus::Life* pLife );
	cAttackTarget( tShot* pShot );
	virtual ~cAttackTarget();

	//!	オペレータ
	void	operator = ( cChrStatus::Life* p0 )	{ setObj( p0 ); }
	void	operator = ( tShot* p0 )			{ setObj( p0 ); }

public:	
	// -----------------------------------------------------------------
	//		定義		
	// -----------------------------------------------------------------
	enum {
		TYPE_UNKNOWN = 0,	//!< 不明
		TYPE_CHR,			//!< キャラクター（部位）
		TYPE_SHOT,			//!< ショット
	};

	// -----------------------------------------------------------------
	//		関数		
	// -----------------------------------------------------------------
	//!	初期化
	void	reset();
	//!	更新
	void	update();
	
	//!	有効か
	bool	isEnable() const	{ return (mType != TYPE_UNKNOWN) && mpObj; }
	
	//!	座標取得
	TGL::Vec3		getPos() const;
	
	//!	対象取得 キャラクター部位
	cChrStatus::Life*	getObjLife() const {
		if( mType != TYPE_CHR )	return NULL;
		return (cChrStatus::Life*)mpObj;
	}
	//!	対象設定 キャラクター部位
	void	setObj( cChrStatus::Life* p0 ) {
		mpObj	= p0;
		mType	= TYPE_CHR;
	}
	
	//!	対象取得 ショット
	tShot*	getObjShot() const {
		if( mType != TYPE_SHOT )	return NULL;
		return (tShot*)mpObj;
	}
	//!	対象設定 ショット
	void	setObj( tShot* p0 ) {
		mpObj	= p0;
		mType	= TYPE_SHOT;
	}

	//		アクセサ		
	u8		getType() const	{ return mType; }
	

private:
	// -----------------------------------------------------------------
	//		定義		
	// -----------------------------------------------------------------

	// -----------------------------------------------------------------
	//		関数		
	// -----------------------------------------------------------------
	//!	継続判定
	bool	isContinue() const;

	// -----------------------------------------------------------------
	//		変数		
	// -----------------------------------------------------------------
	u8			mType;	//!< TYPE_????
	void*		mpObj;	//!< 誘導対象
};

/* ========================================================================= */
/*		EOF																	 */
/* ========================================================================= */
