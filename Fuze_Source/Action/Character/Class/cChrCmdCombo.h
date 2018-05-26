// -------------------------------------------------------------------------- //
//! @file   cChrCmdCombo.h
//! @brief  コマンド入力 コンボ
//! @author Nal
//! @since  12/04/10(火)
//!
//! COPYRIGHT (C) 2012 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
#pragma	once
/* ====< インクルード >====================================================== */
#include "../Core/tCharacter.h"
#include "cChrCmd.h"

/* ========================================================================= */
//		cChrCmdCombo														 */
/* ========================================================================= */
class cChrCmdCombo : public cChrCmd
{
public:
	cChrCmdCombo( tCharacter* pChr );
	virtual ~cChrCmdCombo();

public:	
	// -----------------------------------------------------------------
	//		定義		
	// -----------------------------------------------------------------
	//!	スクリプトテーブル
	typedef struct {
		u32		mCmdLable;	//!< スクリプトラベル
		s8		mReceive;	//!< 入力受付開始時間。0未満でいつでも
		s8		mDelay;		//!< 攻撃判定発生からの遅延受付時間。0未満でいつでも
		u8		mCancel;	//!< 攻撃判定発生からのキャンセルがかかるタイミング
		u8		mDummy1;
		u32		mFlag;		//!< フラグ [COMBOF_????]
	} SCRIPTTBL_T;

	enum {
		//!	check進行値
		CHKSTEP_NONE = 0,		//!< 無し
		CHKSTEP_LIMIT,			//!< 移行制限
		CHKSTEP_COLLISION,		//!< 判定発生
		CHKSTEP_DELAY,			//!< ディレイ
		CHKSTEP_CANCELFRAME,	//!< キャンセルフレーム
		CHKSTEP_MULTIATK,		//!< 多段攻撃
		CHKSTEP_SUCCESS,		//!< 成功
		CHKSTEP_MAX,
	};

public:	
	/* ========================================================================= */
	//		cChrCmdCombo::Line													 */
	/* ========================================================================= */
	class Line : public cChrCmd::Line
	{
	public:
		Line( const TGS_T* pTgs, const SCRIPTTBL_T* pTable );
		virtual ~Line() {}

		//!	クリア
		virtual void	clear();
		//!	更新
		virtual void	update( cChrCmd& Parent, const tCharacter& Chr );
		//!	入力を受け付けるかどうか
		virtual bool	isInputReceive( const tCharacter& Chr );

	protected:
		//		変数	
		const SCRIPTTBL_T*	mpScriptTable;	//!< スクリプトテーブル
		s32*				mpAnmList;		//!< 入力有効なアニメ番号のリスト
	};

public:	
	// -----------------------------------------------------------------
	//		関数		
	// -----------------------------------------------------------------
	//!	セットアップ
	void	setup( const SCRIPTTBL_T* pScriptTable );

	//!	クリア
	virtual void	clear();
	//!	更新
	virtual void	update();

	//!	コマンド技が出せるかどうか判定する
	bool	check();

	//!	チェックステップ名
	const char*	getCheckStepName() const;

	//		アクセサ	
	const SCRIPTTBL_T*	getSuccessTable() const						{ return mpSuccessTable; }
	void				setSuccessTable( const SCRIPTTBL_T* p0 )	{ mpSuccessTable = p0; }
	u8		getCheckStep() const		{ return mCheckStep; }

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
	const SCRIPTTBL_T*	mpScriptTable;	//!< スクリプトテーブル先頭
	const SCRIPTTBL_T*	mpSuccessTable;	//!< 成功したコマンドスクリプトテーブル
	u8					mCheckStep;		//!< CHKSTEP_????
	stl::vector<Line*>	mLine;		//!< 各コマンド
};

/* ========================================================================= */
/*		EOF																	 */
/* ========================================================================= */
