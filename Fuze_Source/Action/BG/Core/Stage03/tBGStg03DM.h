// -------------------------------------------------------------------------- //
//! @file   tBGStg03DM.h
//! @brief  
//! @author Nal
//! @since  13/04/24(水)
//!
//! COPYRIGHT (C) 2013 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
#pragma	once
/* ====< インクルード >====================================================== */
#include "tBGStg03.h"

/* ========================================================================= */
//		DMBGStg03															 */
/* ========================================================================= */
class BGViewer;
class tBGStg03;

class DMBGStg03 : public TGL::DMenu
{
public:
	DMBGStg03( TGL::DMenuRoot* pRoot, BGViewer* pOwner, tBGStg03* pTask );
	virtual ~DMBGStg03();

public:
	// -----------------------------------------------------------------
	//		定義		
	// -----------------------------------------------------------------
	enum {
		//!	項目ID
		ITEMID_METEOR_ALPHAREF = 0,	//!< 隕石 AlphaRef
		ITEMID_BOSSSKYSCALE,		//!< ボス戦天球サイズ
		ITEMID_CLOUDSIZE,			//!< 雲海サイズ
		ITEMID_CLOUDROT,			//!< 雲海回転
		ITEMID_CLOUD_BLEND,			//!< 雲海 ブレンドモード
		ITEMID_CLOUD_HEIGHT,		//!< 雲海 高さ
		ITEMID_CLOUD_COLOR,			//!< 雲海 色
		ITEMID_CLOUD_UVOFS,			//!< 雲海 UVオフセット
		ITEMID_CLOUD_UVREPEAT,		//!< 雲海 UVリピート
		ITEMID_LENSFLARE,			//!< レンズフレア
		ITEMID_MAX,
		
		//!	UV速度
		UVSPD_06A = 0,
		UVSPD_06B,
		UVSPD_06C,
		UVSPD_06D,
		UVSPD_SPIRALA,
		UVSPD_SPIRALB,
		UVSPD_SPIRALC,
		UVSPD_CLOUDA,
		UVSPD_CLOUDB,
		UVSPD_CLOUDC,
		UVSPD_MAX,
	};

	// -----------------------------------------------------------------
	//		関数		
	// -----------------------------------------------------------------
	//!	セットアップ
	virtual void	setup();

	//!	更新
	virtual void	update();
	//!	項目更新
	virtual void	updateItem();
	//!	項目表示
	virtual void	dispItem( TGL::DMenuItem* pItem, bool bCurrent, RGBA32 uColor=0xFFFFFFFF );
	//!	表示判定
	virtual bool	isDisp() const	{ return !KBBTN_DAT(KBBTN_F3); }

	//		アクセサ		
	

private:
	// -----------------------------------------------------------------
	//		定義		
	// -----------------------------------------------------------------

	// -----------------------------------------------------------------
	//		関数		
	// -----------------------------------------------------------------
	//!	UV値取得
	TGL::Vec2*	getTaskUV( u8 uId );

	// -----------------------------------------------------------------
	//		変数		
	// -----------------------------------------------------------------
	BGViewer*	mpOwner;		//!< オーナー
	tBGStg03*	mpTask;			//!< 背景タスク
};

/* ========================================================================= */
/*		EOF																	 */
/* ========================================================================= */
