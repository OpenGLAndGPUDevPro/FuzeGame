// -------------------------------------------------------------------------- //
//! @file   tBGStg02Result.h
//! @brief  背景 ２面リザルト
//! @author Nal
//! @since  12/07/21(土)
//! 
//! COPYRIGHT (C) 2012 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
#pragma once
/* ====< インクルード >====================================================== */
#include "../tBG.h"

/* ========================================================================= */
//!		tBGStg02Result														 */
/* ========================================================================= */
class tBGStg02Result : public tBG
{
public:
	tBGStg02Result();
	virtual	~tBGStg02Result();
	
public:
	// -----------------------------------------------------------------
	//!		関数		
	// -----------------------------------------------------------------
	//!	リセット
	virtual void	reset();
	//!	エフェクト開始
	virtual void	beginEffect();
	
	//	アクセサ
	
protected:
	// -----------------------------------------------------------------
	//!		定義		
	// -----------------------------------------------------------------

	// -----------------------------------------------------------------
	//!		関数		
	// -----------------------------------------------------------------
	//!	リソースロード
	virtual bool	load( u8 uBG );
	//!	UVアニメ設定
	void	setModelUvAnim();

	//!	更新
	virtual void	update();
	//!	更新 ドライブ
	void		updateDrive( bool bSceneChg );
	static void	updateDriveCB( tBG* pBG, bool bSceneChg )	{ ((tBGStg02Result*)(pBG))->updateDrive( bSceneChg ); }
	//!	更新 ドライブ終了
	void		updateDriveEnd( bool bSceneChg );
	static void	updateDriveEndCB( tBG* pBG, bool bSceneChg )	{ ((tBGStg02Result*)(pBG))->updateDriveEnd( bSceneChg ); }

	// -----------------------------------------------------------------
	//!		変数		
	// -----------------------------------------------------------------
	TGL::TransVec	mDrivePos;	//!< ドライブ座標
};
