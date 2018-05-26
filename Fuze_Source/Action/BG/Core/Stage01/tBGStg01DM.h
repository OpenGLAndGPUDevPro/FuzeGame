// -------------------------------------------------------------------------- //
//! @file   tBGStg01DM.h
//! @brief  
//! @author Nal
//! @since  13/04/24(水)
//!
//! COPYRIGHT (C) 2013 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
#pragma	once
/* ====< インクルード >====================================================== */
#include "tBGStg01.h"

/* ========================================================================= */
//		DMBGStg01															 */
/* ========================================================================= */
class BGViewer;
class tBGStg01;

class DMBGStg01 : public TGL::DMenu
{
public:
	DMBGStg01( TGL::DMenuRoot* pRoot, BGViewer* pOwner, tBGStg01* pTask );
	virtual ~DMBGStg01();

public:
	// -----------------------------------------------------------------
	//		定義		
	// -----------------------------------------------------------------
	enum {
		//!	項目ID
		ITEMID_SEA_NML1REPEAT = 0,	//!< 法線マップリピート
		ITEMID_SEA_NML1SPDU,		//!< 法線マップ横速度
		ITEMID_SEA_NML1SPDV,		//!< 法線マップ縦速度
		ITEMID_SEA_NML2REPEAT,		//!< 法線マップリピート
		ITEMID_SEA_NML2SPDU,		//!< 法線マップ横速度
		ITEMID_SEA_NML2SPDV,		//!< 法線マップ縦速度
		ITEMID_SEA_NML2ALPHA,		//!< 法線マップ透過度
		ITEMID_SEA_AMBIENT,			//!< アンビエント
		ITEMID_SEA_REFLECTION,		//!< 環境マップ強度
		ITEMID_SEA_DIFFUSE,			//!< ディフューズ
		ITEMID_SEA_SPECULAR,		//!< スペキュラ
		ITEMID_SEA_SPECULARPOWER,	//!< スペキュラ範囲
		ITEMID_SEA_FRESNELSCALE,	//!< フレネル項スケール
		ITEMID_SEA_FLICKERSPEED,	//!< 揺らぎ速度
		ITEMID_SEA_FLICKERFREQ,		//!< 揺らぎ周期
		ITEMID_SEA_FLICKERAMP,		//!< 揺らぎ振幅
		ITEMID_LENSFLARE,			//!< レンズフレア
		ITEMID_LENSFLARE_SPACE,		//!< レンズフレア(宇宙)
		ITEMID_MAX,
		
		//!	UV速度
		UVSPD_FOAM_A = 0,	//!< 泡 UV速度
		UVSPD_FOAM_B,		//!< 泡 UV速度
		UVSPD_RIVER06,		//!< 川 UV速度
		UVSPD_RIVER07,		//!< 川 UV速度
		UVSPD_SLSD_A,		//!< ドライブ軌道A UV
		UVSPD_SLSD_B,		//!< ドライブ軌道B UV
		UVSPD_SLSD_C,		//!< ドライブ軌道C UV
		UVSPD_SLSD_D,		//!< ドライブ軌道D UV
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
	tBGStg01*	mpTask;			//!< 背景タスク
};

/* ========================================================================= */
/*		EOF																	 */
/* ========================================================================= */
