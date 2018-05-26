// -------------------------------------------------------------------------- //
//! @file   cChrCmdLimit.h
//! @brief  コマンド入力 移行条件
//! @author Nal
//! @since  13/02/24(日)
//!
//! COPYRIGHT (C) 2013 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
#pragma	once
/* ====< インクルード >====================================================== */
#include "../Core/tCharacter.h"

/* ========================================================================= */
//		cChrCmdLimit [基底 / 仮想]											 */
/* ========================================================================= */
class cChrCmdLimit
{
public:
	cChrCmdLimit() {}
	virtual ~cChrCmdLimit() {}

	//!	チェック
	virtual bool	check( const tCharacter& Chr ) const	{ return true; }
};

/* ========================================================================= */
//																			 */
/* ========================================================================= */
// ---------------------------------------------
//!	フラグ比較 &								
class cChrCmdLimit_FlagTst : public cChrCmdLimit
{
public:
	cChrCmdLimit_FlagTst( u8 uFlagId, u32 uFlag ) : cChrCmdLimit(), mFlagId(uFlagId), mFlag(uFlag) {}
	virtual ~cChrCmdLimit_FlagTst() {}

	//!	チェック
	virtual bool	check( const tCharacter& Chr ) const;

	//		変数		
	u8		mFlagId;	//!< FLAGID_????
	u32		mFlag;		//!< Flag
};

// ---------------------------------------------
//!	フラグ比較 !&								
class cChrCmdLimit_FlagNTst : public cChrCmdLimit
{
public:
	cChrCmdLimit_FlagNTst( u8 uFlagId, u32 uFlag ) : cChrCmdLimit(), mFlagId(uFlagId), mFlag(uFlag) {}
	virtual ~cChrCmdLimit_FlagNTst() {}

	//!	チェック
	virtual bool	check( const tCharacter& Chr ) const;

	//		変数		
	u8		mFlagId;	//!< FLAGID_????
	u32		mFlag;		//!< Flag
};

// ---------------------------------------------
//!	フラグ比較 ==								
class cChrCmdLimit_FlagEq : public cChrCmdLimit
{
public:
	cChrCmdLimit_FlagEq( u8 uFlagId, u32 uFlag ) : cChrCmdLimit(), mFlagId(uFlagId), mFlag(uFlag) {}
	virtual ~cChrCmdLimit_FlagEq() {}

	//!	チェック
	virtual bool	check( const tCharacter& Chr ) const;

	//		変数		
	u8		mFlagId;	//!< FLAGID_????
	u32		mFlag;		//!< Flag
};


// ---------------------------------------------
//!	MP比較 >=									
class cChrCmdLimit_MP : public cChrCmdLimit
{
public:
	cChrCmdLimit_MP( u16 uMP ) : cChrCmdLimit(), mMP(uMP) {}
	virtual ~cChrCmdLimit_MP() {}

	//!	チェック
	virtual bool	check( const tCharacter& Chr ) const {
		return true;
	}

	//		変数		
	u16		mMP;	//!< MP
};


// ---------------------------------------------
//!	アニメ番号比較 ==							
class cChrCmdLimit_AnmNo : public cChrCmdLimit
{
public:
	cChrCmdLimit_AnmNo( s16 nAnmNo ) : cChrCmdLimit(), mAnmNo(nAnmNo) {}
	virtual ~cChrCmdLimit_AnmNo() {}

	//!	チェック
	virtual bool	check( const tCharacter& Chr ) const;

	//		変数		
	s16		mAnmNo;	//!< アニメ番号
};


// ---------------------------------------------
//!	コールバック設定							
class cChrCmdLimit_ExtraCB : public cChrCmdLimit
{
public:
	cChrCmdLimit_ExtraCB( bool (*pCB)(const tCharacter&, const cChrCmdLimit_ExtraCB&), s32 nWork=0, f32 fWork=0 ) :
		cChrCmdLimit(), mpCB(pCB), mWorkI(nWork), mWorkF(fWork) {}
	virtual ~cChrCmdLimit_ExtraCB() {}

	//!	チェック
	virtual bool	check( const tCharacter& Chr ) const;

	//		アクセサ	
	s32		getWorkI() const	{ return mWorkI; }	//!< 整数ワーク
	f32		getWorkF() const	{ return mWorkF; }	//!< 実数ワーク

	//		変数		
	bool	(*mpCB)(const tCharacter&, const cChrCmdLimit_ExtraCB&);	//!< コールバック
	s32		mWorkI;	//!< 整数ワーク
	f32		mWorkF;	//!< 実数ワーク
};


/* ========================================================================= */
/*		EOF																	 */
/* ========================================================================= */
