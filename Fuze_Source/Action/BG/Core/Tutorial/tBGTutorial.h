// -------------------------------------------------------------------------- //
//! @file   tBGTutorial.h
//! @brief  背景 チュートリアル
//! @author Nal
//! @since  13/06/01(土)
//! 
//! COPYRIGHT (C) 2013 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
#pragma once
/* ====< インクルード >====================================================== */
#include "../tBG.h"
#include "../../class/tDigitalLine.h"

/* ========================================================================= */
//!		tBGTutorial															 */
/* ========================================================================= */
class tBGTutorial : public tBG
{
public:
	tBGTutorial();
	virtual	~tBGTutorial();
	
public:
	// -----------------------------------------------------------------
	//!		定義		
	// -----------------------------------------------------------------
	enum {
		//!	シーン
		SCENE_READY = 0,	//!< 準備（真っ暗）
		SCENE_TUTORIAL,		//!< チュートリアル
		SCENE_EXTRAGALLERY,	//!< エクストラギャラリー
	};

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
	//!	更新 ライン
	void		updateDigitalLine( bool bSceneChg );
	static void	updateDigitalLineCB( tBG* pBG, bool bSceneChg )	{ ((tBGTutorial*)(pBG))->updateDigitalLine( bSceneChg ); }
	//!	更新 エクストラギャラリー
	void		setupExtraGallery();
	static void	setupExtraGalleryCB( tBG* pBG )	{ ((tBGTutorial*)(pBG))->setupExtraGallery(); }

	//!	デバッグ情報表示
	virtual void	dispDebugInfo();

	// -----------------------------------------------------------------
	//!		変数		
	// -----------------------------------------------------------------
	tDigitalLine*	mpDigitalLine;	//!< デジタルライン
	TGL::TransFloat	mLineAlpha;		//!< ラインアルファ
	u16		mExEnvSize;		//!< 特殊環境マップサイズ
};
