// -------------------------------------------------------------------------- //
//! @file   tBGStg02DM.cpp
//! @brief  
//! @author Nal
//! @since  13/04/24(水)
//!
//! COPYRIGHT (C) 2013 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
/* ====< インクルード >====================================================== */
#include "prefix.h"
#include <TGL.h>
#include "Game/Game.h"
#include "Viewer/BG/BGViewer.h"
#include "tBGStg02DM.h"
#include "tBGStg02.h"

/* ========================================================================= */
/*		DMBGStg02															 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  コンストラクタ "DMBGStg02"
// ---------------------------------------------
DMBGStg02::DMBGStg02( TGL::DMenuRoot* pRoot, BGViewer* pOwner, tBGStg02* pTask )
:	TGL::DMenu(pRoot), mpOwner(pOwner), mpTask(pTask)
{
	setup();
}

// ---------------------------------------------
//! @brief  デストラクタ "DMBGStg02"
// ---------------------------------------------
DMBGStg02::~DMBGStg02()
{
	mpOwner->changeStep( BGViewer::STEP_VIEW );
}

// ---------------------------------------------
//! @brief  セットアップ
// ---------------------------------------------
void DMBGStg02::setup()
{
	//!	タイトル
	mTitle = stl::FormatString<stl::string>( "背景ビュワー ≪固有データ調整≫ \"%s\"", sBG::getName(mpOwner->getBGType()) );
	
	//!	クリア
	clearItem();
	
	//!	メニュー追加
	TGL::DMenuItem*		pItem;

	pItem = APP_NEW TGL::DMenuItem( TGL::DMenuItem::ITEMID_UVSPD, 1 );	pItem->setParamI( UVSPD_FAR01 );	addItem( pItem );
	pItem = APP_NEW TGL::DMenuItem( TGL::DMenuItem::ITEMID_UVSPD, 1 );	pItem->setParamI( UVSPD_FAR02_A );	addItem( pItem );
	pItem = APP_NEW TGL::DMenuItem( TGL::DMenuItem::ITEMID_UVSPD, 1 );	pItem->setParamI( UVSPD_FAR02_B );	addItem( pItem );
	pItem = APP_NEW TGL::DMenuItem( TGL::DMenuItem::ITEMID_UVSPD, 1 );	pItem->setParamI( UVSPD_LOOP01_A );	addItem( pItem );
	pItem = APP_NEW TGL::DMenuItem( TGL::DMenuItem::ITEMID_UVSPD, 1 );	pItem->setParamI( UVSPD_LOOP01_B );	addItem( pItem );
	pItem = APP_NEW TGL::DMenuItem( TGL::DMenuItem::ITEMID_UVSPD, 1 );	pItem->setParamI( UVSPD_LOOP01_C );	addItem( pItem );
	pItem = APP_NEW TGL::DMenuItem( TGL::DMenuItem::ITEMID_UVSPD, 1 );	pItem->setParamI( UVSPD_LOOP01_D );	addItem( pItem );
	pItem = APP_NEW TGL::DMenuItem( TGL::DMenuItem::ITEMID_UVSPD, 1 );	pItem->setParamI( UVSPD_02_A );		addItem( pItem );
	pItem = APP_NEW TGL::DMenuItem( TGL::DMenuItem::ITEMID_UVSPD, 1 );	pItem->setParamI( UVSPD_02_B );		addItem( pItem );
	pItem = APP_NEW TGL::DMenuItem( TGL::DMenuItem::ITEMID_UVSPD, 1 );	pItem->setParamI( UVSPD_02_C );		addItem( pItem );
	pItem = APP_NEW TGL::DMenuItem( TGL::DMenuItem::ITEMID_UVSPD, 1 );	pItem->setParamI( UVSPD_02_D );		addItem( pItem );
	pItem = APP_NEW TGL::DMenuItem( TGL::DMenuItem::ITEMID_UVSPD, 1 );	pItem->setParamI( UVSPD_04_A );		addItem( pItem );
	pItem = APP_NEW TGL::DMenuItem( TGL::DMenuItem::ITEMID_UVSPD, 1 );	pItem->setParamI( UVSPD_04_B );		addItem( pItem );
	pItem = APP_NEW TGL::DMenuItem( TGL::DMenuItem::ITEMID_UVSPD, 1 );	pItem->setParamI( UVSPD_04_C );		addItem( pItem );
	pItem = APP_NEW TGL::DMenuItem( TGL::DMenuItem::ITEMID_UVSPD, 1 );	pItem->setParamI( UVSPD_04_D );		addItem( pItem );
	pItem = APP_NEW TGL::DMenuItem( TGL::DMenuItem::ITEMID_UVSPD, 1 );	pItem->setParamI( UVSPD_04_A2 );	addItem( pItem );
	pItem = APP_NEW TGL::DMenuItem( TGL::DMenuItem::ITEMID_UVSPD, 1 );	pItem->setParamI( UVSPD_04_B2 );	addItem( pItem );
	pItem = APP_NEW TGL::DMenuItem( TGL::DMenuItem::ITEMID_UVSPD, 1 );	pItem->setParamI( UVSPD_04_C2 );	addItem( pItem );
	pItem = APP_NEW TGL::DMenuItem( TGL::DMenuItem::ITEMID_UVSPD, 1 );	pItem->setParamI( UVSPD_04_D2 );	addItem( pItem );
	pItem = APP_NEW TGL::DMenuItem( TGL::DMenuItem::ITEMID_UVSPD, 1 );	pItem->setParamI( UVSPD_04_FAR );	addItem( pItem );
	pItem = APP_NEW TGL::DMenuItem( TGL::DMenuItem::ITEMID_UVSPD, 1 );	pItem->setParamI( UVSPD_04_FAR2 );	addItem( pItem );
}

// ---------------------------------------------
//! @brief  更新
// ---------------------------------------------
void DMBGStg02::update()
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
void DMBGStg02::updateItem()
{
	//!	項目別		
	TGL::DMenuItem*	pItem = getCurrentItem();	//!< 選択項目
	
	switch( pItem->getId() ){
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
void DMBGStg02::dispItem( TGL::DMenuItem* pItem, bool bCurrent, RGBA32 uColor/*=0xFFFFFFFF*/ )
{
	switch( pItem->getId() ){
		// -------------------------------------------------
		case TGL::DMenuItem::ITEMID_UVSPD:	//!	UV速度
		{
			const char*	_NAME[UVSPD_MAX] = {
				"遠景01",
				"遠景02A",
				"遠景02B",
				"01A",
				"01B",
				"01C",
				"01D",
				"02A",
				"02B",
				"02C",
				"02D",
				"04A",
				"04B",
				"04C",
				"04D",
				"04A2",
				"04B2",
				"04C2",
				"04D2",
				"04Far",
				"04Far2",
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
TGL::Vec2* DMBGStg02::getTaskUV( u8 uId )
{
	TGL::Vec2*	pvRet[UVSPD_MAX] = {
		&mpTask->mFar01_Uv,
		&mpTask->mFar02_UvA,
		&mpTask->mFar02_UvB,
		&mpTask->mLoop01_UvA,
		&mpTask->mLoop01_UvB,
		&mpTask->mLoop01_UvC,
		&mpTask->mLoop01_UvD,
		&mpTask->m02_UvA,
		&mpTask->m02_UvB,
		&mpTask->m02_UvC,
		&mpTask->m02_UvD,
		&mpTask->m04_UvA,
		&mpTask->m04_UvB,
		&mpTask->m04_UvC,
		&mpTask->m04_UvD,
		&mpTask->m04_UvA2,
		&mpTask->m04_UvB2,
		&mpTask->m04_UvC2,
		&mpTask->m04_UvD2,
		&mpTask->m04_UvFar,
		&mpTask->m04_UvFar2,
	};
	return pvRet[uId];
}

/* ========================================================================= */
/*		EOF																	 */
/* ========================================================================= */
