// -------------------------------------------------------------------------- //
//! @file   tBGStg05DM.cpp
//! @brief  
//! @author Nal
//! @since  13/08/30(金)
//!
//! COPYRIGHT (C) 2013 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
/* ====< インクルード >====================================================== */
#include "prefix.h"
#include <TGL.h>
#include "Game/Game.h"
#include "Viewer/BG/BGViewer.h"
#include "tBGStg05DM.h"
#include "tBGStg05.h"
#include "Effect/Lensflare/tLensFlareSFDMenu.h"

/* ========================================================================= */
/*		DMBGStg05															 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  コンストラクタ "DMBGStg05"
// ---------------------------------------------
DMBGStg05::DMBGStg05( TGL::DMenuRoot* pRoot, BGViewer* pOwner, tBGStg05* pTask )
:	TGL::DMenu(pRoot), mpOwner(pOwner), mpTask(pTask)
{
	setup();
}

// ---------------------------------------------
//! @brief  デストラクタ "DMBGStg05"
// ---------------------------------------------
DMBGStg05::~DMBGStg05()
{
	mpOwner->changeStep( BGViewer::STEP_VIEW );
}

// ---------------------------------------------
//! @brief  セットアップ
// ---------------------------------------------
void DMBGStg05::setup()
{
	//!	タイトル
	mTitle = stl::FormatString<stl::string>( "背景ビュワー ≪固有データ調整≫ \"%s\"", sBG::getName(mpOwner->getBGType()) );
	
	//!	クリア
	clearItem();
	
	//!	メニュー追加
	TGL::DMenuItem*	pItem;

	pItem = APP_NEW TGL::DMenuItem( TGL::DMenuItem::ITEMID_UVSPD, 1 );	pItem->setParamI( UVSPD_CITYFALL );	pItem->setHead( "UV Scroll" );	addItem( pItem );
	addItem( APP_NEW TGL::DMenuItem(ITEMID_LENSFLARE) );
}

// ---------------------------------------------
//! @brief  更新
// ---------------------------------------------
void DMBGStg05::update()
{
	//!		親クラス	
	TGL::DMenu::update();
	
	//!		終了処理	
	//!	UVスクロール更新
	mpTask->setModelUvAnim();
}

// ---------------------------------------------
//! @brief  項目更新
// ---------------------------------------------
void DMBGStg05::updateItem()
{
	//!	項目別		
	TGL::DMenuItem*	pItem = getCurrentItem();	//!< 選択項目
	
	switch( pItem->getId() ){
		// -------------------------------------------------
		case TGL::DMenuItem::ITEMID_UVSPD:	//!	UV速度
			pItem->setUVSpd( *getTaskUV(pItem->getParamI()), mAdd );
			break;
		// -------------------------------------------------
		case ITEMID_LENSFLARE:	//! レンズフレア
			if( !JOY_TRG(PADOK) )	break;
			mpRoot->addMenu( APP_NEW DMenuLensFlare(mpRoot, mpTask->mpLensflare) );
			break;
	}
}

// ---------------------------------------------
//! @brief  項目表示
//! @param  pItem		[i/o]	項目
//! @param  bCurrent	[in]	true:選択中
//! @param  uColor		[in]	色
// ---------------------------------------------
void DMBGStg05::dispItem( TGL::DMenuItem* pItem, bool bCurrent, RGBA32 uColor/*=0xFFFFFFFF*/ )
{
	switch( pItem->getId() ){
		// -------------------------------------------------
		case TGL::DMenuItem::ITEMID_UVSPD:	//!	UV速度
		{
			const char*	_NAME[UVSPD_MAX] = {
				"都市滝",
			};
			stl::string	str( stl::FormatString<stl::string>("%-8s UV : ", _NAME[pItem->getParamI()]) );
			pItem->dprintUV( str, *getTaskUV(pItem->getParamI()), isCurrentItem(pItem), uColor );
		}	break;
		// -------------------------------------------------
		case ITEMID_LENSFLARE:	//! レンズフレア
			IDPrint->addSh( uColor, "レンズフレア :\n" );
			break;
	}
}

// ---------------------------------------------
//! @brief  UV値取得
//! @param  uId	[in]	UVSPD_????
//! @return UV値
// ---------------------------------------------
TGL::Vec2* DMBGStg05::getTaskUV( u8 uId )
{
	TGL::Vec2*	pvRet[UVSPD_MAX] = {
		&mpTask->mCityFall_Uv,
	};
	return pvRet[uId];
}

/* ========================================================================= */
/*		EOF																	 */
/* ========================================================================= */
