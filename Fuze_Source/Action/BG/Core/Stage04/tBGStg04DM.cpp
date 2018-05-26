// -------------------------------------------------------------------------- //
//! @file   tBGStg04DM.cpp
//! @brief  
//! @author Nal
//! @since  13/04/25(木)
//!
//! COPYRIGHT (C) 2013 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
/* ====< インクルード >====================================================== */
#include "prefix.h"
#include <TGL.h>
#include "Game/Game.h"
#include "Viewer/BG/BGViewer.h"
#include "tBGStg04DM.h"
#include "tBGStg04.h"
#include "Effect/Lensflare/tLensFlareSFDMenu.h"

/* ========================================================================= */
/*		DMBGStg04															 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  コンストラクタ "DMBGStg04"
// ---------------------------------------------
DMBGStg04::DMBGStg04( TGL::DMenuRoot* pRoot, BGViewer* pOwner, tBGStg04* pTask )
:	TGL::DMenu(pRoot), mpOwner(pOwner), mpTask(pTask)
{
	setup();
}

// ---------------------------------------------
//! @brief  デストラクタ "DMBGStg04"
// ---------------------------------------------
DMBGStg04::~DMBGStg04()
{
	mpOwner->changeStep( BGViewer::STEP_VIEW );
}

// ---------------------------------------------
//! @brief  セットアップ
// ---------------------------------------------
void DMBGStg04::setup()
{
	//!	タイトル
	mTitle = stl::FormatString<stl::string>( "背景ビュワー ≪固有データ調整≫ \"%s\"", sBG::getName(mpOwner->getBGType()) );
	
	//!	クリア
	clearItem();
	
	//!	メニュー追加
	TGL::DMenuItem*		pItem;

	pItem = APP_NEW TGL::DMenuItem( TGL::DMenuItem::ITEMID_UVSPD, 1 );	pItem->setParamI( UVSPD_01A );		addItem( pItem );
	pItem = APP_NEW TGL::DMenuItem( TGL::DMenuItem::ITEMID_UVSPD, 1 );	pItem->setParamI( UVSPD_01B );		addItem( pItem );
	pItem = APP_NEW TGL::DMenuItem( TGL::DMenuItem::ITEMID_UVSPD, 1 );	pItem->setParamI( UVSPD_01C );		addItem( pItem );
	pItem = APP_NEW TGL::DMenuItem( TGL::DMenuItem::ITEMID_UVSPD, 1 );	pItem->setParamI( UVSPD_01D );		addItem( pItem );

	addItem( APP_NEW TGL::DMenuItem(ITEMID_CUBEANMSPD) );
	addItem( APP_NEW TGL::DMenuItem(ITEMID_LENSFLARE) );
}

// ---------------------------------------------
//! @brief  更新
// ---------------------------------------------
void DMBGStg04::update()
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
void DMBGStg04::updateItem()
{
	//!	項目別		
	TGL::DMenuItem*	pItem = getCurrentItem();	//!< 選択項目
	
	switch( pItem->getId() ){
		// -------------------------------------------------
		case ITEMID_CUBEANMSPD:	//! 箱アニメ速度
			mpTask->mCubeSpd += mAdd * 0.01f;
			break;
		// -------------------------------------------------
		case ITEMID_LENSFLARE:	//! レンズフレア
			if( !JOY_TRG(PADOK) )	break;
			mpRoot->addMenu( APP_NEW DMenuLensFlare(mpRoot, mpTask->mLensflare[0]) );
			break;
		// -------------------------------------------------
		case TGL::DMenuItem::ITEMID_UVSPD:	//!	UV速度
			pItem->setUVSpd( *getTaskUV(pItem->getParamI()), mAdd );
			break;
	}
}

// ---------------------------------------------
//! @brief  項目表示
//! @param  pItem		[i/o]	項目
//! @param  bCurrent	[in]	true:選択中
//! @param  uColor		[in]	色
// ---------------------------------------------
void DMBGStg04::dispItem( TGL::DMenuItem* pItem, bool bCurrent, RGBA32 uColor/*=0xFFFFFFFF*/ )
{
	switch( pItem->getId() ){
		// -------------------------------------------------
		case ITEMID_CUBEANMSPD:	//! 箱アニメ速度
			IDPrint->addSh( uColor, "箱アニメ速度 : %0.2f\n", mpTask->mCubeSpd );
			break;
		// -------------------------------------------------
		case ITEMID_LENSFLARE:	//! レンズフレア
			IDPrint->addSh( uColor, "レンズフレア :\n" );
			break;
		// -------------------------------------------------
		case TGL::DMenuItem::ITEMID_UVSPD:	//!	UV速度
		{
			const char*	_NAME[UVSPD_MAX] = {
				"01A",
				"01B",
				"01C",
				"01D",
			};
			stl::string	str( stl::FormatString<stl::string>("%-8s UV : ", _NAME[pItem->getParamI()]) );
			pItem->dprintUV( str, *getTaskUV(pItem->getParamI()), isCurrentItem(pItem), uColor );
		}	break;
	}
}

// ---------------------------------------------
//! @brief  UV値取得
//! @param  uId	[in]	UVSPD_????
//! @return UV値
// ---------------------------------------------
TGL::Vec2* DMBGStg04::getTaskUV( u8 uId )
{
	TGL::Vec2*	pvRet[UVSPD_MAX] = {
		&mpTask->m01_UvA,
		&mpTask->m01_UvB,
		&mpTask->m01_UvC,
		&mpTask->m01_UvD,
	};
	return pvRet[uId];
}

/* ========================================================================= */
/*		EOF																	 */
/* ========================================================================= */
