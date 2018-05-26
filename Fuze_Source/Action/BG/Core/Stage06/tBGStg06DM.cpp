// -------------------------------------------------------------------------- //
//! @file   tBGStg06DM.cpp
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
#include "tBGStg06DM.h"
#include "tBGStg06.h"

/* ========================================================================= */
/*		DMBGStg06															 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  コンストラクタ "DMBGStg06"
// ---------------------------------------------
DMBGStg06::DMBGStg06( TGL::DMenuRoot* pRoot, BGViewer* pOwner, tBGStg06* pTask )
:	TGL::DMenu(pRoot), mpOwner(pOwner), mpTask(pTask)
{
	setup();
}

// ---------------------------------------------
//! @brief  デストラクタ "DMBGStg06"
// ---------------------------------------------
DMBGStg06::~DMBGStg06()
{
	mpOwner->changeStep( BGViewer::STEP_VIEW );
}

// ---------------------------------------------
//! @brief  セットアップ
// ---------------------------------------------
void DMBGStg06::setup()
{
	//!	タイトル
	mTitle = stl::FormatString<stl::string>( "背景ビュワー ≪固有データ調整≫ \"%s\"", sBG::getName(mpOwner->getBGType()) );
	
	//!	クリア
	clearItem();
	
	//!	メニュー追加
	TGL::DMenuItem*	pItem;

	pItem = APP_NEW TGL::DMenuItem( ITEMID_DRIVEOBJ_ROLL );	pItem->setHead( "Drive" );	addItem( pItem );

	pItem = APP_NEW TGL::DMenuItem( TGL::DMenuItem::ITEMID_UVSPD, 1 );	pItem->setParamI( UVSPD_FAR01 );	pItem->setHead( "UV Scroll" );	addItem( pItem );
	pItem = APP_NEW TGL::DMenuItem( TGL::DMenuItem::ITEMID_UVSPD, 1 );	pItem->setParamI( UVSPD_LOOP01_A );	addItem( pItem );
	pItem = APP_NEW TGL::DMenuItem( TGL::DMenuItem::ITEMID_UVSPD, 1 );	pItem->setParamI( UVSPD_LOOP01_B );	addItem( pItem );
	pItem = APP_NEW TGL::DMenuItem( TGL::DMenuItem::ITEMID_UVSPD, 1 );	pItem->setParamI( UVSPD_LOOP01_C );	addItem( pItem );
	pItem = APP_NEW TGL::DMenuItem( TGL::DMenuItem::ITEMID_UVSPD, 1 );	pItem->setParamI( UVSPD_LOOP01_D );	addItem( pItem );
	pItem = APP_NEW TGL::DMenuItem( TGL::DMenuItem::ITEMID_UVSPD, 1 );	pItem->setParamI( UVSPD_DRIVEOBJ_A );	addItem( pItem );
	pItem = APP_NEW TGL::DMenuItem( TGL::DMenuItem::ITEMID_UVSPD, 1 );	pItem->setParamI( UVSPD_DRIVEOBJ_B );	addItem( pItem );
	pItem = APP_NEW TGL::DMenuItem( TGL::DMenuItem::ITEMID_UVSPD, 1 );	pItem->setParamI( UVSPD_MOTHER_GLOW );	addItem( pItem );
	pItem = APP_NEW TGL::DMenuItem( TGL::DMenuItem::ITEMID_UVSPD, 1 );	pItem->setParamI( UVSPD_MOTHER_GLOW2 );	addItem( pItem );
	pItem = APP_NEW TGL::DMenuItem( TGL::DMenuItem::ITEMID_UVSPD, 1 );	pItem->setParamI( UVSPD_MOTHER_CLOUD );	addItem( pItem );
	pItem = APP_NEW TGL::DMenuItem( TGL::DMenuItem::ITEMID_UVSPD, 1 );	pItem->setParamI( UVSPD_MOTHER_HOLE );	addItem( pItem );
	pItem = APP_NEW TGL::DMenuItem( TGL::DMenuItem::ITEMID_UVSPD, 1 );	pItem->setParamI( UVSPD_MOTHER_STRING );	addItem( pItem );
	pItem = APP_NEW TGL::DMenuItem( TGL::DMenuItem::ITEMID_UVSPD, 1 );	pItem->setParamI( UVSPD_MOTHER_STRING2 );	addItem( pItem );
}

// ---------------------------------------------
//! @brief  更新
// ---------------------------------------------
void DMBGStg06::update()
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
void DMBGStg06::updateItem()
{
	//!	項目別		
	TGL::DMenuItem*	pItem = getCurrentItem();	//!< 選択項目
	
	switch( pItem->getId() ){
		// -------------------------------------------------
		case ITEMID_DRIVEOBJ_ROLL:	//!	ドライブObj回転
			mpTask->mDriveObj_Roll += mAdd * 0.01f;
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
void DMBGStg06::dispItem( TGL::DMenuItem* pItem, bool bCurrent, RGBA32 uColor/*=0xFFFFFFFF*/ )
{
	switch( pItem->getId() ){
		// -------------------------------------------------
		case ITEMID_DRIVEOBJ_ROLL:	//!	ドライブObj回転
			IDPrint->addSh( uColor, "ドライブObj回転 : %0.2f\n", mpTask->mDriveObj_Roll );
			break;

		// -------------------------------------------------
		case TGL::DMenuItem::ITEMID_UVSPD:	//!	UV速度
		{
			const char*	_NAME[UVSPD_MAX] = {
				"遠景01",
				"ドライブA",
				"ドライブB",
				"ドライブC",
				"ドライブD",
				"ドライブObjA",
				"ドライブObjB",
				"母星 Glow",
				"母星 Glow2",
				"母星 Cloud",
				"母星 Hole",
				"母星 String",
				"母星 String2",
			};
			stl::string	str( stl::FormatString<stl::string>("%-12s UV : ", _NAME[pItem->getParamI()]) );
			pItem->dprintUV( str, *getTaskUV(pItem->getParamI()), isCurrentItem(pItem), uColor );
		}	break;
	}
}

// ---------------------------------------------
//! @brief  UV値取得
//! @param  uId	[in]	UVSPD_????
//! @return UV値
// ---------------------------------------------
TGL::Vec2* DMBGStg06::getTaskUV( u8 uId )
{
	TGL::Vec2*	pvRet[UVSPD_MAX] = {
		&mpTask->mFar01_Uv,
		&mpTask->mLoop01_UvA,
		&mpTask->mLoop01_UvB,
		&mpTask->mLoop01_UvC,
		&mpTask->mLoop01_UvD,
		&mpTask->mDriveObj_UvA,
		&mpTask->mDriveObj_UvB,
		&mpTask->mMother_Uv_Glow,
		&mpTask->mMother_Uv_Glow2,
		&mpTask->mMother_Uv_Cloud,
		&mpTask->mMother_Uv_Hole,
		&mpTask->mMother_Uv_String,
		&mpTask->mMother_Uv_String2,
	};
	return pvRet[uId];
}

/* ========================================================================= */
/*		EOF																	 */
/* ========================================================================= */
