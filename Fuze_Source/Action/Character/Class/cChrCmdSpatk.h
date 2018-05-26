// -------------------------------------------------------------------------- //
//! @file   cChrCmdSpatk.h
//! @brief  コマンド入力 SPATK
//! @author Nal
//! @since  12/04/08(日)
//!
//! COPYRIGHT (C) 2012 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
#pragma	once
/* ====< インクルード >====================================================== */
#include "../Core/tCharacter.h"
#include "cChrCmd.h"

/* ========================================================================= */
//		cChrCmdSpatk														 */
/* ========================================================================= */
class cChrCmdSpatk : public cChrCmd
{
public:
	cChrCmdSpatk( tCharacter* pChr );
	virtual ~cChrCmdSpatk();

public:	
	// -----------------------------------------------------------------
	//		定義		
	// -----------------------------------------------------------------
	//!	スクリプトテーブル
	typedef struct {
		u32		mCmdLable;	//!< スクリプトラベル
		u8		mKind;		//!< 種類 [SPATKKIND_????]
		u8		mAttr;		//!< 属性 [SPATKATTR_????]
		u8		mOnly;		//!< 状況制限 [SPATKONLY_????]
		u8		mDummy;		//!< 未使用
		u32		mFlag;		//!< フラグ [SPATKF_????]
	} SCRIPTTBL_T;

	enum {
		//!	チェックモード
		CHK_BASIC = 0,	//!< 大まかなチェック
		CHK_IDLESAME,	//!< 待機中（同時押し対策）
		CHK_IDLE,		//!< 待機中
		CHK_ATTACK,		//!< 攻撃中
		CHK_DAMAGE,		//!< ガードorダメージorダウン中
		CHK_NODEFAULT,	//!< 攻撃中ではなく、PRGF_DEFAULTも立っていない
		
		//!	check進行値
		CHKSTEP_NONE = 0,	//!< 無し
		CHKSTEP_BASIC,		//!< 大まかなチェック
		CHKSTEP_LIMIT,		//!< 移行制限
		CHKSTEP_ATTR,		//!< 属性
		CHKSTEP_KIND,		//!< 種類
		CHKSTEP_IDLESAME,	//!< 同時押し
		CHKSTEP_IDLE,		//!< 待機中
		CHKSTEP_ATTACK,		//!< 攻撃中
		CHKSTEP_DAMAGE,		//!< ダメージ中
		CHKSTEP_NODEFAULT,	//!< その他
		CHKSTEP_SUCCESS,	//!< 成功
		CHKSTEP_MAX,
	};

public:	
	/* ========================================================================= */
	//		cChrCmdSpatk::Line													 */
	/* ========================================================================= */
	class Line : public cChrCmd::Line
	{
	public:
		Line( const TGS_T* pTgs, const SCRIPTTBL_T* pTable );
		virtual ~Line() {}

		//!	更新
		virtual void	update( cChrCmd& Parent, const tCharacter& Chr );

	protected:
		//		変数	
		const SCRIPTTBL_T*	mpScriptTable;	//!< スクリプトテーブル
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
	bool	check( u8 uMode );

	//!	チェックステップ名
	const char*	getCheckStepName() const;

	//		アクセサ	
	const SCRIPTTBL_T*	getSuccessTable() const						{ return mpSuccessTable; }
	void				setSuccessTable( const SCRIPTTBL_T* p0 )	{ mpSuccessTable = p0; }
	f32		getSuccessDelay() const		{ return mSuccessDelay; }
	void	setSuccessDelay( f32 f0 )	{ mSuccessDelay = f0; }
	u8		getCheckStep() const		{ return mCheckStep; }

private:
	// -----------------------------------------------------------------
	//		定義		
	// -----------------------------------------------------------------

	// -----------------------------------------------------------------
	//		関数		
	// -----------------------------------------------------------------
	//!	攻撃の種類を見てキャンセルできる技なのかどうか判定
	bool	checkAtk();
	//!	キャンセル可能かどうか
	bool	checkCancel();

	// -----------------------------------------------------------------
	//		変数		
	// -----------------------------------------------------------------
	const SCRIPTTBL_T*	mpScriptTable;	//!< スクリプトテーブル先頭
	const SCRIPTTBL_T*	mpSuccessTable;	//!< 成功したコマンドスクリプトテーブル
	f32					mSuccessDelay;	//!< 先行入力保持期間
	u8					mCheckStep;		//!< CHKSTEP_????
	stl::vector<Line*>	mLine;		//!< 各コマンド
};

/* ========================================================================= */
/*		EOF																	 */
/* ========================================================================= */
