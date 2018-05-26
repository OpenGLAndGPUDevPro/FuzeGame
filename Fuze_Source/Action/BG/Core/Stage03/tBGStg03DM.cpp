// -------------------------------------------------------------------------- //
//! @file   tBGStg03DM.cpp
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
#include "tBGStg03DM.h"
#include "tBGStg03.h"
#include "Effect/Lensflare/tLensFlareSFDMenu.h"

/* ========================================================================= */
/*		DMBGStg03															 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  コンストラクタ "DMBGStg03"
// ---------------------------------------------
DMBGStg03::DMBGStg03( TGL::DMenuRoot* pRoot, BGViewer* pOwner, tBGStg03* pTask )
:	TGL::DMenu(pRoot), mpOwner(pOwner), mpTask(pTask)
{
	setup();
}

// ---------------------------------------------
//! @brief  デストラクタ "DMBGStg03"
// ---------------------------------------------
DMBGStg03::~DMBGStg03()
{
	mpOwner->changeStep( BGViewer::STEP_VIEW );
}

// ---------------------------------------------
//! @brief  セットアップ
// ---------------------------------------------
void DMBGStg03::setup()
{
	//!	タイトル
	mTitle = stl::FormatString<stl::string>( "背景ビュワー ≪固有データ調整≫ \"%s\"", sBG::getName(mpOwner->getBGType()) );
	
	//!	クリア
	clearItem();
	
	//!	メニュー追加
	TGL::DMenuItem*		pItem;

	addItem( APP_NEW TGL::DMenuItem(ITEMID_METEOR_ALPHAREF) );
	addItem( APP_NEW TGL::DMenuItem(ITEMID_BOSSSKYSCALE) );
	addItem( APP_NEW TGL::DMenuItem(ITEMID_CLOUDSIZE) );
	addItem( APP_NEW TGL::DMenuItem(ITEMID_CLOUDROT) );

	for( u8 i=0 ; i<3 ; i++ ){
		pItem = APP_NEW TGL::DMenuItem( ITEMID_CLOUD_BLEND, 1 );	pItem->setParamI( i );	addItem( pItem );
		pItem = APP_NEW TGL::DMenuItem( ITEMID_CLOUD_HEIGHT, 1 );	pItem->setParamI( i );	addItem( pItem );
		pItem = APP_NEW TGL::DMenuItem( ITEMID_CLOUD_COLOR, 1 );	pItem->setParamI( i );	addItem( pItem );
		pItem = APP_NEW TGL::DMenuItem( ITEMID_CLOUD_UVOFS, 1 );	pItem->setParamI( i );	addItem( pItem );
		pItem = APP_NEW TGL::DMenuItem( ITEMID_CLOUD_UVREPEAT, 1 );	pItem->setParamI( i );	addItem( pItem );
	}
	
	pItem = APP_NEW TGL::DMenuItem( TGL::DMenuItem::ITEMID_UVSPD, 1 );	pItem->setParamI( UVSPD_06A );		addItem( pItem );
	pItem = APP_NEW TGL::DMenuItem( TGL::DMenuItem::ITEMID_UVSPD, 1 );	pItem->setParamI( UVSPD_06B );		addItem( pItem );
	pItem = APP_NEW TGL::DMenuItem( TGL::DMenuItem::ITEMID_UVSPD, 1 );	pItem->setParamI( UVSPD_06C );		addItem( pItem );
	pItem = APP_NEW TGL::DMenuItem( TGL::DMenuItem::ITEMID_UVSPD, 1 );	pItem->setParamI( UVSPD_06D );		addItem( pItem );
	pItem = APP_NEW TGL::DMenuItem( TGL::DMenuItem::ITEMID_UVSPD, 1 );	pItem->setParamI( UVSPD_SPIRALA );	addItem( pItem );
	pItem = APP_NEW TGL::DMenuItem( TGL::DMenuItem::ITEMID_UVSPD, 1 );	pItem->setParamI( UVSPD_SPIRALB );	addItem( pItem );
	pItem = APP_NEW TGL::DMenuItem( TGL::DMenuItem::ITEMID_UVSPD, 1 );	pItem->setParamI( UVSPD_SPIRALC );	addItem( pItem );
	pItem = APP_NEW TGL::DMenuItem( TGL::DMenuItem::ITEMID_UVSPD, 1 );	pItem->setParamI( UVSPD_CLOUDA );	addItem( pItem );
	pItem = APP_NEW TGL::DMenuItem( TGL::DMenuItem::ITEMID_UVSPD, 1 );	pItem->setParamI( UVSPD_CLOUDB );	addItem( pItem );
	pItem = APP_NEW TGL::DMenuItem( TGL::DMenuItem::ITEMID_UVSPD, 1 );	pItem->setParamI( UVSPD_CLOUDC );	addItem( pItem );

	addItem( APP_NEW TGL::DMenuItem(ITEMID_LENSFLARE) );
}

// ---------------------------------------------
//! @brief  更新
// ---------------------------------------------
void DMBGStg03::update()
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
void DMBGStg03::updateItem()
{
	//!	項目別		
	TGL::DMenuItem*	pItem = getCurrentItem();	//!< 選択項目
	
	switch( pItem->getId() ){
		// -------------------------------------------------
		case ITEMID_METEOR_ALPHAREF:	//! 隕石 AlphaRef
			mpTask->mMeteorAlphaRef = TGL::Math::Clamp<s32>( mpTask->mMeteorAlphaRef, mAdd, 1, 255 );
			break;
		// -------------------------------------------------
		case ITEMID_BOSSSKYSCALE:	//! ボス戦天球サイズ
			mpTask->mBossSkyScaleEnd += mAdd * 0.01f;
			break;
		// -------------------------------------------------
		case ITEMID_CLOUDSIZE:	//! 雲海サイズ
			mpTask->mCloudSize += mAdd * 1000.f;
			break;
		// -------------------------------------------------
		case ITEMID_CLOUDROT:	//! 雲海回転
			mpTask->mCloudRot = TGL::Math::NormalizeDegree( mpTask->mCloudRot + (mAdd*1.0f) );
			break;
		// -------------------------------------------------
		case ITEMID_CLOUD_BLEND:	//! 雲海ブレンドモード
			mpTask->mCloudParam[pItem->getParamI()].mBlend = TGL::Math::Loop<s32>( mpTask->mCloudParam[pItem->getParamI()].mBlend, mAdd, 0, RSBL_MAX-1 );
			break;
		// -------------------------------------------------
		case ITEMID_CLOUD_HEIGHT:	//! 雲海高さ
			mpTask->mCloudParam[pItem->getParamI()].mHeight += mAdd * 10.0f;
			break;
		// -------------------------------------------------
		case ITEMID_CLOUD_COLOR:	//! 雲海色
			pItem->setColor( mpTask->mCloudParam[pItem->getParamI()].mColor, mAdd );
			break;
		// -------------------------------------------------
		case ITEMID_CLOUD_UVOFS:	//! 雲海UVオフセット
			mpTask->mCloudParam[pItem->getParamI()].mUVOfs += mAdd * 0.01f;
			break;
		// -------------------------------------------------
		case ITEMID_CLOUD_UVREPEAT:	//! 雲海UVリピート
			mpTask->mCloudParam[pItem->getParamI()].mUVRepeat += mAdd * 0.1f;
			break;
		// -------------------------------------------------
		case ITEMID_LENSFLARE:	//! レンズフレア
			if( !JOY_TRG(PADOK) )	break;
			mpRoot->addMenu( APP_NEW DMenuLensFlare(mpRoot, mpTask->mpLensflare) );
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
void DMBGStg03::dispItem( TGL::DMenuItem* pItem, bool bCurrent, RGBA32 uColor/*=0xFFFFFFFF*/ )
{
	switch( pItem->getId() ){
		// -------------------------------------------------
		case ITEMID_METEOR_ALPHAREF:	//! 隕石 AlphaRef
			IDPrint->addSh( uColor, "隕石 AlphaRef : %d\n", mpTask->mMeteorAlphaRef );
			break;
		// -------------------------------------------------
		case ITEMID_BOSSSKYSCALE:	//! ボス戦天球サイズ
			IDPrint->addSh( uColor, "ボス戦天球サイズ : %0.2f\n", mpTask->mBossSkyScaleEnd );
			break;
		// -------------------------------------------------
		case ITEMID_CLOUDSIZE:	//! 雲海サイズ
			IDPrint->addSh( uColor, "雲海 サイズ   : %0.0f\n", mpTask->mCloudSize );
			break;
		// -------------------------------------------------
		case ITEMID_CLOUDROT:	//! 雲海回転
			IDPrint->addSh( uColor, "　　 回転     : %4.1f\n", mpTask->mCloudRot );
			break;
		// -------------------------------------------------
		case ITEMID_CLOUD_BLEND:	//!	雲海 ブレンドモード
			IDPrint->addSh( uColor, "　%2d 合成     : %d/%d \"%s\"\n", pItem->getParamI(), mpTask->mCloudParam[pItem->getParamI()].mBlend, RSBL_MAX-1, TGL::RenderState::getBlendName(mpTask->mCloudParam[pItem->getParamI()].mBlend) );
			break;
		// -------------------------------------------------
		case ITEMID_CLOUD_HEIGHT:	//! 雲海高さ
			IDPrint->addSh( uColor, "　　 高さ     : %0.1f\n", mpTask->mCloudParam[pItem->getParamI()].mHeight );
			break;
		// -------------------------------------------------
		case ITEMID_CLOUD_COLOR:	//! 雲海 色
			pItem->dprintColor( "　　 色       : ", mpTask->mCloudParam[pItem->getParamI()].mColor, uColor );
			break;
		// -------------------------------------------------
		case ITEMID_CLOUD_UVOFS:	//! 雲海 UVオフセット
			IDPrint->addSh( uColor, "　　 UV加算   : %0.2f\n", mpTask->mCloudParam[pItem->getParamI()].mUVOfs );
			break;
		// -------------------------------------------------
		case ITEMID_CLOUD_UVREPEAT:	//! 雲海 UVオフセット
			IDPrint->addSh( uColor, "　　 リピート : %0.1f\n", mpTask->mCloudParam[pItem->getParamI()].mUVRepeat );
			break;
		// -------------------------------------------------
		case ITEMID_LENSFLARE:	//! レンズフレア
			IDPrint->addSh( uColor, "レンズフレア :\n" );
			break;
		// -------------------------------------------------
		case TGL::DMenuItem::ITEMID_UVSPD:	//!	UV速度
		{
			const char*	_NAME[UVSPD_MAX] = {
				"06A",
				"06B",
				"06C",
				"06D",
				"螺旋雲A",
				"螺旋雲B",
				"螺旋雲C",
				"雲海A",
				"雲海B",
				"雲海C",
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
TGL::Vec2* DMBGStg03::getTaskUV( u8 uId )
{
	TGL::Vec2*	pvRet[UVSPD_MAX] = {
		&mpTask->m06_UvA,
		&mpTask->m06_UvB,
		&mpTask->m06_UvC,
		&mpTask->m06_UvD,
		&mpTask->mSpiral_UvA,
		&mpTask->mSpiral_UvB,
		&mpTask->mSpiral_UvC,
		&mpTask->mCloud_UvA,
		&mpTask->mCloud_UvB,
		&mpTask->mCloud_UvC,
	};
	return pvRet[uId];
}

/* ========================================================================= */
/*		EOF																	 */
/* ========================================================================= */
