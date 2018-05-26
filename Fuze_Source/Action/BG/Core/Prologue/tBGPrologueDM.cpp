// -------------------------------------------------------------------------- //
//! @file   tBGPrologueDM.cpp
//! @brief  
//! @author Nal
//! @since  13/06/26(水)
//!
//! COPYRIGHT (C) 2013 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
/* ====< インクルード >====================================================== */
#include "prefix.h"
#include <TGL.h>
#include "Game/Game.h"
#include "Viewer/BG/BGViewer.h"
#include "tBGPrologueDM.h"
#include "tBGPrologue.h"
#include "Effect/Lensflare/tLensFlareSFDMenu.h"

/* ========================================================================= */
/*		DMBGPrologue														 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  コンストラクタ "DMBGPrologue"
// ---------------------------------------------
DMBGPrologue::DMBGPrologue( TGL::DMenuRoot* pRoot, BGViewer* pOwner, tBGPrologue* pTask )
:	TGL::DMenu(pRoot), mpOwner(pOwner), mpTask(pTask)
{
	setup();
}

// ---------------------------------------------
//! @brief  デストラクタ "DMBGPrologue"
// ---------------------------------------------
DMBGPrologue::~DMBGPrologue()
{
	mpOwner->changeStep( BGViewer::STEP_VIEW );
}

// ---------------------------------------------
//! @brief  セットアップ
// ---------------------------------------------
void DMBGPrologue::setup()
{
	//!	タイトル
	mTitle = stl::FormatString<stl::string>( "背景ビュワー ≪固有データ調整≫ \"%s\"", sBG::getName(mpOwner->getBGType()) );
	
	//!	クリア
	clearItem();
	
	//!	メニュー追加
	TGL::DMenuItem*	pItem;
	pItem = APP_NEW TGL::DMenuItem( ITEMID_LIGHTNING_INTERVAL );
	pItem->setHead( "稲光" );
	addItem( pItem );
	addItem( APP_NEW TGL::DMenuItem( ITEMID_LIGHTNING_RELEASE ) );
	
	pItem = APP_NEW TGL::DMenuItem( ITEMID_RAIN_SPEED_VALLEY );
	pItem->setHead( "雨" );
	addItem( pItem );
	addItem( APP_NEW TGL::DMenuItem( ITEMID_RAIN_SPEED_CITY ) );
	addItem( APP_NEW TGL::DMenuItem( ITEMID_RAIN_COLORBASE_MIN ) );
	addItem( APP_NEW TGL::DMenuItem( ITEMID_RAIN_COLORBASE_MAX ) );
	addItem( APP_NEW TGL::DMenuItem( ITEMID_RAIN_COLORLIGHT_MIN ) );
	addItem( APP_NEW TGL::DMenuItem( ITEMID_RAIN_COLORLIGHT_MAX ) );

	pItem = APP_NEW TGL::DMenuItem( TGL::DMenuItem::ITEMID_UVSPD, 1 );	pItem->setParamI( UVSPD_CITYFALL );	pItem->setHead( "UV Scroll" );	addItem( pItem );

	pItem = APP_NEW TGL::DMenuItem( ITEMID_LENSFLARE );
	pItem->setHead( "OTHER" );
	addItem( pItem );
}

// ---------------------------------------------
//! @brief  更新
// ---------------------------------------------
void DMBGPrologue::update()
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
void DMBGPrologue::updateItem()
{
	//!	項目別		
	TGL::DMenuItem*	pItem = getCurrentItem();	//!< 選択項目
	
	switch( pItem->getId() ){
		// -------------------------------------------------
		case ITEMID_LIGHTNING_INTERVAL:	//!	稲光 間隔
			pItem->setRange2( mpTask->mLightningInt, (f32)mAdd, 1.0f, 99999.0f );
			break;
		// -------------------------------------------------
		case ITEMID_LIGHTNING_RELEASE:	//!	稲光 減衰
			pItem->setRange2( mpTask->mLightningRel, (f32)mAdd, 1.0f, 99999.0f );
			break;
		// -------------------------------------------------
		case ITEMID_RAIN_SPEED_VALLEY:	//!	雨 速度 崖
			mpTask->mRainSpeedValley += mAdd * 1.0f;
			break;
		// -------------------------------------------------
		case ITEMID_RAIN_SPEED_CITY:	//!	雨 速度 都市
			mpTask->mRainSpeedCity += mAdd * 1.0f;
			break;
		// -------------------------------------------------
		case ITEMID_RAIN_COLORBASE_MIN:	//!	雨 基準色 最小
			pItem->setColor( mpTask->mRainColorBaseMin, mAdd );
			break;
		// -------------------------------------------------
		case ITEMID_RAIN_COLORBASE_MAX:	//!	雨 基準色 最大
			pItem->setColor( mpTask->mRainColorBaseMax, mAdd );
			break;
		// -------------------------------------------------
		case ITEMID_RAIN_COLORLIGHT_MIN:	//!	雨 稲光色 最小
			pItem->setColor( mpTask->mRainColorLightMin, mAdd );
			break;
		// -------------------------------------------------
		case ITEMID_RAIN_COLORLIGHT_MAX:	//!	雨 稲光色 最大
			pItem->setColor( mpTask->mRainColorLightMax, mAdd );
			break;
		// -------------------------------------------------
		case TGL::DMenuItem::ITEMID_UVSPD:	//!	UV速度
			pItem->setUVSpd( *getTaskUV(pItem->getParamI()), mAdd );
			break;
		// -------------------------------------------------
		case ITEMID_LENSFLARE:	//! レンズフレア
			if( !JOY_TRG(PADOK) || !mpTask->mpLensflare )	break;
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
void DMBGPrologue::dispItem( TGL::DMenuItem* pItem, bool bCurrent, RGBA32 uColor/*=0xFFFFFFFF*/ )
{
	switch( pItem->getId() ){
		// -------------------------------------------------
		case ITEMID_LIGHTNING_INTERVAL:	//!	稲光 間隔
			pItem->dprintRange2( "間隔 : ", TGL::Vec2(mpTask->mLightningInt.getMin(), mpTask->mLightningInt.getMax()), bCurrent, uColor );
			break;
		// -------------------------------------------------
		case ITEMID_LIGHTNING_RELEASE:	//!	稲光 減衰
			pItem->dprintRange2( "減衰 : ", TGL::Vec2(mpTask->mLightningRel.getMin(), mpTask->mLightningRel.getMax()), bCurrent, uColor );
			break;
		// -------------------------------------------------
		case ITEMID_RAIN_SPEED_VALLEY:	//!	雨 速度 崖
			IDPrint->addSh( uColor, "速度 崖     : %0.1f\n", mpTask->mRainSpeedValley );
			break;
		// -------------------------------------------------
		case ITEMID_RAIN_SPEED_CITY:	//!	雨 速度 都市
			IDPrint->addSh( uColor, "速度 都市   : %0.1f\n", mpTask->mRainSpeedCity );
			break;
		// -------------------------------------------------
		case ITEMID_RAIN_COLORBASE_MIN:	//!	雨 基準色 最小
			pItem->dprintColor( "基準色 最小 : ", mpTask->mRainColorBaseMin, uColor );
			break;
		// -------------------------------------------------
		case ITEMID_RAIN_COLORBASE_MAX:	//!	雨 基準色 最大
			pItem->dprintColor( "基準色 最大 : ", mpTask->mRainColorBaseMax, uColor );
			break;
		// -------------------------------------------------
		case ITEMID_RAIN_COLORLIGHT_MIN:	//!	雨 稲光色 最小
			pItem->dprintColor( "稲光色 最小 : ", mpTask->mRainColorLightMin, uColor );
			break;
		// -------------------------------------------------
		case ITEMID_RAIN_COLORLIGHT_MAX:	//!	雨 稲光色 最大
			pItem->dprintColor( "稲光色 最大 : ", mpTask->mRainColorLightMax, uColor );
			break;
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
TGL::Vec2* DMBGPrologue::getTaskUV( u8 uId )
{
	TGL::Vec2*	pvRet[UVSPD_MAX] = {
		&mpTask->mCityFall_Uv,
	};
	return pvRet[uId];
}

/* ========================================================================= */
/*		EOF																	 */
/* ========================================================================= */
