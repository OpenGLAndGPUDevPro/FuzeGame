// -------------------------------------------------------------------------- //
//! @file   cChrCmd.h
//! @brief  コマンド入力 基底
//! @author Nal
//! @since  12/04/07(土)
//!
//! COPYRIGHT (C) 2012 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
#pragma	once
/* ====< インクルード >====================================================== */
#include "../Core/tCharacter.h"
#include "cChrCmdLimit.h"

/* ========================================================================= */
//		cChrCmd																 */
/* ========================================================================= */
class cChrCmd
{
	friend class Line;
public:
	cChrCmd( tCharacter* pChr );
	virtual ~cChrCmd();

public:	
	/* ========================================================================= */
	//		cChrCmd::Line														 */
	/* ========================================================================= */
	class Line
	{
	public:
		Line()	{ clear(); }
		virtual ~Line() {}

		//!	クリア
		virtual void	clear();
		//!	更新
		virtual void	update( cChrCmd& Parent, const tCharacter& Chr ){}
		//!	入力を受け付けるかどうか
		virtual bool	isInputReceive( const tCharacter& Chr ) { return true; }

	protected:
		//!	コマンド実行
		xu32*	execCmd( xu32* pc, cChrCmd& Parent, const tCharacter& Chr );
		//!	パッド入力
		xu32*	cmdPad( xu32* pc, cChrCmd& Parent, const tCharacter& Chr );
		//!	パッド回転入力
		xu32*	cmdPadRoll( xu32* pc, cChrCmd& Parent, const tCharacter& Chr );
		//!	パッド共通 時間経過
		void	cmdPad_TimeAdd( s32 nTime );
		//!	フラグ比較
		xu32*	cmdCheckFlag( xu32* pc, const tCharacter& Chr );
		//!	アニメ番号比較
		xu32*	cmdAnmNoNot( xu32* pc, const tCharacter& Chr );
		//!	固有処理
		xu32*	cmdEx( xu32* pc, const tCharacter& Chr );
		//!	移行制限 フラグ比較
		xu32*	cmdLimitFlagTst( xu32* pc, cChrCmd& Parent );
		//!	移行制限 フラグ否定
		xu32*	cmdLimitFlagNotTst( xu32* pc, cChrCmd& Parent );
		//!	移行制限 フラグ一致
		xu32*	cmdLimitFlagEqTst( xu32* pc, cChrCmd& Parent );
		//!	移行制限 MP
		xu32*	cmdLimitMP( xu32* pc, cChrCmd& Parent );
		//!	移行制限 アニメ番号
		xu32*	cmdLimitAnmNo( xu32* pc, cChrCmd& Parent );
		//!	移行制限 固有
		xu32*	cmdLimitEx( xu32* pc, cChrCmd& Parent );

		//		変数		
		const void*	mpCmdStart;		//!< コマンド先頭位置
		const void*	mpCmd;			//!< コマンド現在位置
		f32			mTime;			//!< 入力時間
		f32			mCTime;			//!< 持続時間
		u32			mPadBuf[2];		//!< 入力したパッド情報。回転入力で使用
	};

public:	
	// -----------------------------------------------------------------
	//		定義		
	// -----------------------------------------------------------------

	// -----------------------------------------------------------------
	//		関数		
	// -----------------------------------------------------------------
	//!	クリア
	virtual void	clear();
	//!	更新
	virtual void	update();
	
	//!	移行制限 クリア
	void	clearLimit();
	//!	移行制限 チェック
	bool	checkLimit() const;

	//		アクセサ	
	s16		getSuccessId() const	{ return mSuccessId; }
	void	setSuccessId( s16 i0 )	{ mSuccessId = i0; }
	u32		getSuccessPad() const	{ return mSuccessPad; }
	void	setSuccessPad( u32 i0 )	{ mSuccessPad = i0; }
	const TGL::Vec2&	getSuccessLever() const					{ return mSuccessLever; }
	void				setSuccessLever( const TGL::Vec2& v0 )	{ mSuccessLever = v0; }

protected:
	// -----------------------------------------------------------------
	//		定義		
	// -----------------------------------------------------------------

	// -----------------------------------------------------------------
	//		関数		
	// -----------------------------------------------------------------

	// -----------------------------------------------------------------
	//		変数		
	// -----------------------------------------------------------------
	tCharacter*		mpChr;			//!< オーナー
	s16				mSuccessId;		//!< 成功したコマンド入力
	u32				mSuccessPad;	//!< 成功時の入力
	TGL::Vec2		mSuccessLever;	//!< 成功時のレバー入力
	stl::vector<cChrCmdLimit*>	mLimit;	//!< 成功後移行条件
};

/* ========================================================================= */
/*		EOF																	 */
/* ========================================================================= */
