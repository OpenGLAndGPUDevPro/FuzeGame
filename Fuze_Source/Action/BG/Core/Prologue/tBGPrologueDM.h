// -------------------------------------------------------------------------- //
//! @file   tBGPrologueDM.h
//! @brief  
//! @author Nal
//! @since  13/06/26(水)
//!
//! COPYRIGHT (C) 2013 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
#pragma	once
/* ====< インクルード >====================================================== */
#include "tBGPrologue.h"

/* ========================================================================= */
//		DMBGPrologue														 */
/* ========================================================================= */
class BGViewer;
class tBGPrologue;

class DMBGPrologue : public TGL::DMenu
{
public:
	DMBGPrologue( TGL::DMenuRoot* pRoot, BGViewer* pOwner, tBGPrologue* pTask );
	virtual ~DMBGPrologue();

public:
	// -----------------------------------------------------------------
	//		定義		
	// -----------------------------------------------------------------
	enum {
		//!	項目ID
		ITEMID_LIGHTNING_INTERVAL = 0,	//!< 稲光 間隔
		ITEMID_LIGHTNING_RELEASE,		//!< 稲光 減衰
		ITEMID_RAIN_SPEED_VALLEY,		//!< 雨 速度 崖
		ITEMID_RAIN_SPEED_CITY,			//!< 雨 速度 都市
		ITEMID_RAIN_COLORBASE_MIN,		//!< 雨 基準色 最小
		ITEMID_RAIN_COLORBASE_MAX,		//!< 雨 基準色 最大
		ITEMID_RAIN_COLORLIGHT_MIN,		//!< 雨 稲光色 最小
		ITEMID_RAIN_COLORLIGHT_MAX,		//!< 雨 稲光色 最大
		ITEMID_LENSFLARE,				//!< レンズフレア
		ITEMID_MAX,

		//!	UV速度
		UVSPD_CITYFALL = 0,	//!< 都市滝 UV速度
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
	BGViewer*		mpOwner;	//!< オーナー
	tBGPrologue*	mpTask;		//!< 背景タスク
};

/* ========================================================================= */
/*		EOF																	 */
/* ========================================================================= */
