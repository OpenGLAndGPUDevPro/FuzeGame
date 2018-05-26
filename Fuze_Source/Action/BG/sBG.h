// -------------------------------------------------------------------------- //
//! @file   sBG.h
//! @brief  背景システム
//! @author Nal
//! @since  11/09/05(月)
//!
//! COPYRIGHT (C) 2011 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
#pragma	once
/* ====< インクルード >====================================================== */
#include "BGTypes.h"
#include "Core/tBG.h"

/* ========================================================================= */
//!		sBG																	 */
/* ========================================================================= */
class tBG;

//!	インスタンスへのショートカット
#define IBG	(sBG::getInstance())

class sBG
{
public:
	sBG();
	virtual ~sBG();

public:	
	//! 定期更新
	virtual void OnUpdate( f32 fDeltaTime );
	//! 描画
	virtual void OnDraw();

	//! インスタンス生成
	static sBG*	createInstance()	{ return APP_NEW sBG(); }
	//! インスタンス破棄
	static void	deleteInstance()	{ delete IBG; }
	//! インスタンスを取得
	static sBG*	getInstance()		{ return mpInstance; }

	// -----------------------------------------------------------------
	//!		定義		
	// -----------------------------------------------------------------
	//!	BGIDをSTGIDに変換
	static const u8	BGID_TO_STGID[BGID_MAX];
	//!	紐付くリザルトID
	static const u8	BGID_TO_RESULTID[BGID_MAX];
	//!	環境マップ用テクスチャノード
	enum { ENVTEXNODE_MAX = 2 };
	static const char*	ENV_TEXNODE[ENVTEXNODE_MAX];

	// -----------------------------------------------------------------
	//!		関数		
	// -----------------------------------------------------------------
	//!	リソース非同期読み込みリクエスト
	void	readResource( u8 uBG );
	//!	リソース解放
	void	freeResource( u8 uBG=BGID_NONE );

	//!	タスクセット
	void	setTask( tBG** pTask, u8 uBG );
	//!	メインタスクセット
	void	setTask( u8 uBG )	{ setTask(&mpTask, uBG); }
	//!	メインタスクリセット
	void	resetTask()			{ setTask(&mpTask, BGID_NONE); }
	//!	リザルトタスクセット
	void	setTaskResult( u8 uBG )	{ setTask(&mpTaskResult, uBG); }
	//!	リザルトタスクリセット
	void	resetTaskResult()		{ setTask(&mpTaskResult, BGID_NONE); }

	//!	背景名を取得
	static const char*	getName( u8 uId );

	//		アクセサ
	tBG*	getTask()				{ return mpTask; }			//!< タスク取得
	tBG*	getTaskResult()			{ return mpTaskResult; }	//!< リザルトタスク取得
	

private:
	// -----------------------------------------------------------------
	//!		定義		
	// -----------------------------------------------------------------

	// -----------------------------------------------------------------
	//!		関数		
	// -----------------------------------------------------------------
	//!	パックファイル名の取得
	stl::string	getPackFileName( u8 uBG, const stl::string& strSuffix=stl::nullstr() );

	// -----------------------------------------------------------------
	//!		変数		
	// -----------------------------------------------------------------
	static sBG*	mpInstance;
	bool	mbResource[BGID_MAX];	//!< リソース読み込みフラグ
	tBG*	mpTask;					//!< 背景タスク
	tBG*	mpTaskResult;			//!< リザルトタスク
};

/* ========================================================================= */
/*		EOF																	 */
/* ========================================================================= */
