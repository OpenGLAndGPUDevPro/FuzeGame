// -------------------------------------------------------------------------- //
//! @file   tBGStgBefore.h
//! @brief  背景 C81面
//! @author Nal
//! @since  11/11/15(火)
//! 
//! COPYRIGHT (C) 2011 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
#pragma once
/* ====< インクルード >====================================================== */
#include "../tBG.h"
#include "../../class/tDigitalLine.h"

/* ========================================================================= */
//!		tBGStgBefore														 */
/* ========================================================================= */
class tBGStgBefore : public tBG
{
public:
	tBGStgBefore();
	virtual	~tBGStgBefore();
	
public:
	// -----------------------------------------------------------------
	//!		関数		
	// -----------------------------------------------------------------
	//!	リセット
	virtual void	reset();
	
	//	アクセサ
	
protected:
	// -----------------------------------------------------------------
	//!		関数		
	// -----------------------------------------------------------------
	//!	リソースロード
	virtual bool	load( u8 uBG );

	//!	更新
	virtual void	update();
	//!	スクロール更新
	virtual void	updateScroll();

	//!	フィルタ描画リスト実行
	static void	drawlistExecFilter( void *pData )	{ ((tBGStgBefore*)pData)->drawFilter(); }
	//!	フィルタ描画
	void	drawFilter();

	// -----------------------------------------------------------------
	//!		変数		
	// -----------------------------------------------------------------
	f32				mTimeOld;		//!< 経過時間テンポラリ
	u8				mScrollOld;		//!< 前回のスクロールタイプ
	TGL::TransVec	mScrollWay;		//!< 座標スクロール
	TGL::TransVec	mScrollTar;		//!< 注視点スクロール
	TGL::TransFloat	mScrollTwist;	//!< ツイストスクロール
	tDigitalLine*	mpDigitalLine;	//!< デジタルライン
};
