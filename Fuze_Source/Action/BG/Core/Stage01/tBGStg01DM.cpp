// -------------------------------------------------------------------------- //
//! @file   tBGStg01DM.cpp
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
#include "tBGStg01DM.h"
#include "tBGStg01.h"
#include "Effect/Lensflare/tLensFlareSFDMenu.h"

/* ========================================================================= */
/*		DMBGStg01															 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  コンストラクタ "DMBGStg01"
// ---------------------------------------------
DMBGStg01::DMBGStg01( TGL::DMenuRoot* pRoot, BGViewer* pOwner, tBGStg01* pTask )
:	TGL::DMenu(pRoot), mpOwner(pOwner), mpTask(pTask)
{
	setup();
}

// ---------------------------------------------
//! @brief  デストラクタ "DMBGStg01"
// ---------------------------------------------
DMBGStg01::~DMBGStg01()
{
	mpOwner->changeStep( BGViewer::STEP_VIEW );
}

// ---------------------------------------------
//! @brief  セットアップ
// ---------------------------------------------
void DMBGStg01::setup()
{
	//!	タイトル
	mTitle = stl::FormatString<stl::string>( "背景ビュワー ≪固有データ調整≫ \"%s\"", sBG::getName(mpOwner->getBGType()) );
	
	//!	クリア
	clearItem();
	
	//!	メニュー追加
	TGL::DMenuItem*		pItem;

	addItem( APP_NEW TGL::DMenuItem(ITEMID_SEA_NML1REPEAT) );
	addItem( APP_NEW TGL::DMenuItem(ITEMID_SEA_NML1SPDU) );
	addItem( APP_NEW TGL::DMenuItem(ITEMID_SEA_NML1SPDV) );
	addItem( APP_NEW TGL::DMenuItem(ITEMID_SEA_NML2REPEAT) );
	addItem( APP_NEW TGL::DMenuItem(ITEMID_SEA_NML2SPDU) );
	addItem( APP_NEW TGL::DMenuItem(ITEMID_SEA_NML2SPDV) );
	addItem( APP_NEW TGL::DMenuItem(ITEMID_SEA_NML2ALPHA) );
	addItem( APP_NEW TGL::DMenuItem(ITEMID_SEA_AMBIENT) );
	addItem( APP_NEW TGL::DMenuItem(ITEMID_SEA_REFLECTION) );
	addItem( APP_NEW TGL::DMenuItem(ITEMID_SEA_DIFFUSE, 1) );
	addItem( APP_NEW TGL::DMenuItem(ITEMID_SEA_SPECULAR) );
	addItem( APP_NEW TGL::DMenuItem(ITEMID_SEA_SPECULARPOWER) );
	addItem( APP_NEW TGL::DMenuItem(ITEMID_SEA_FRESNELSCALE) );
	addItem( APP_NEW TGL::DMenuItem(ITEMID_SEA_FLICKERSPEED) );
	addItem( APP_NEW TGL::DMenuItem(ITEMID_SEA_FLICKERFREQ) );
	addItem( APP_NEW TGL::DMenuItem(ITEMID_SEA_FLICKERAMP) );
	
	pItem = APP_NEW TGL::DMenuItem( TGL::DMenuItem::ITEMID_UVSPD, 1 );	pItem->setParamI( UVSPD_FOAM_A );	addItem( pItem );
	pItem = APP_NEW TGL::DMenuItem( TGL::DMenuItem::ITEMID_UVSPD, 1 );	pItem->setParamI( UVSPD_FOAM_B );	addItem( pItem );
	pItem = APP_NEW TGL::DMenuItem( TGL::DMenuItem::ITEMID_UVSPD, 1 );	pItem->setParamI( UVSPD_RIVER06 );	addItem( pItem );
	pItem = APP_NEW TGL::DMenuItem( TGL::DMenuItem::ITEMID_UVSPD, 1 );	pItem->setParamI( UVSPD_RIVER07 );	addItem( pItem );
	pItem = APP_NEW TGL::DMenuItem( TGL::DMenuItem::ITEMID_UVSPD, 1 );	pItem->setParamI( UVSPD_SLSD_A );	addItem( pItem );
	pItem = APP_NEW TGL::DMenuItem( TGL::DMenuItem::ITEMID_UVSPD, 1 );	pItem->setParamI( UVSPD_SLSD_B );	addItem( pItem );
	pItem = APP_NEW TGL::DMenuItem( TGL::DMenuItem::ITEMID_UVSPD, 1 );	pItem->setParamI( UVSPD_SLSD_C );	addItem( pItem );
	pItem = APP_NEW TGL::DMenuItem( TGL::DMenuItem::ITEMID_UVSPD, 1 );	pItem->setParamI( UVSPD_SLSD_D );	addItem( pItem );

	addItem( APP_NEW TGL::DMenuItem(ITEMID_LENSFLARE) );
	addItem( APP_NEW TGL::DMenuItem(ITEMID_LENSFLARE_SPACE) );
}

// ---------------------------------------------
//! @brief  更新
// ---------------------------------------------
void DMBGStg01::update()
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
void DMBGStg01::updateItem()
{
	//!	項目別		
	TGL::DMenuItem*	pItem = getCurrentItem();	//!< 選択項目
	
	switch( pItem->getId() ){
		// -------------------------------------------------
		case ITEMID_SEA_NML1REPEAT:	//! 法線マップリピート
			mpTask->mSeaNml1Repeat = TGL::Math::Clamp<f32>( mpTask->mSeaNml1Repeat, mAdd*0.1f, 0.1f, 999.0f );
			break;
		// -------------------------------------------------
		case ITEMID_SEA_NML1SPDU:	//! 法線マップ横速度
			mpTask->mSeaNml1SpdU = TGL::Math::Clamp<f32>( mpTask->mSeaNml1SpdU, mAdd*0.01f, -100.0f, 100.0f );
			break;
		// -------------------------------------------------
		case ITEMID_SEA_NML1SPDV:	//! 法線マップ縦速度
			mpTask->mSeaNml1SpdV = TGL::Math::Clamp<f32>( mpTask->mSeaNml1SpdV, mAdd*0.01f, -100.0f, 100.0f );
			break;
		// -------------------------------------------------
		case ITEMID_SEA_NML2REPEAT:	//! 法線マップリピート
			mpTask->mSeaNml2Repeat = TGL::Math::Clamp<f32>( mpTask->mSeaNml2Repeat, mAdd*0.1f, 0.1f, 999.0f );
			break;
		// -------------------------------------------------
		case ITEMID_SEA_NML2SPDU:	//! 法線マップ横速度
			mpTask->mSeaNml2SpdU = TGL::Math::Clamp<f32>( mpTask->mSeaNml2SpdU, mAdd*0.01f, -100.0f, 100.0f );
			break;
		// -------------------------------------------------
		case ITEMID_SEA_NML2SPDV:	//! 法線マップ縦速度
			mpTask->mSeaNml2SpdV = TGL::Math::Clamp<f32>( mpTask->mSeaNml2SpdV, mAdd*0.01f, -100.0f, 100.0f );
			break;
		// -------------------------------------------------
		case ITEMID_SEA_NML2ALPHA:	//! 法線マップ透過度
			mpTask->mSeaNml2Alpha = TGL::Math::Clamp<f32>( mpTask->mSeaNml2Alpha, mAdd*0.01f, 0.0f, 1.0f );
			break;
		// -------------------------------------------------
		case ITEMID_SEA_AMBIENT:	//! アンビエント
			mpTask->mSeaAmbient = TGL::Math::Clamp<f32>( mpTask->mSeaAmbient, mAdd*0.01f, 0.00f, 10.00f );
			break;
		// -------------------------------------------------
		case ITEMID_SEA_REFLECTION:	//! 環境マップ強度
			mpTask->mSeaReflection = TGL::Math::Clamp<f32>( mpTask->mSeaReflection, mAdd*0.01f, 0.00f, 100.00f );
			break;
		// -------------------------------------------------
		case ITEMID_SEA_DIFFUSE:		//! ディフューズ
			if( JOY_TRG(PADOK) )	pItem->setParamI( TGL::Math::Loop<s32>(pItem->getParamI(), 1, 0, 2) );
			if( KBBTN_DAT(KBBTNS_SHIFT) ){	//!< 全部
				FRGB_Add( &mpTask->mSeaDiffuse, mAdd*0.01f, mAdd*0.01f, mAdd*0.01f, 100.0f );
			}else{	//!< 個別
				switch( pItem->getParamI() ){
					case 0:	FRGB_Add( &mpTask->mSeaDiffuse, mAdd*0.01f, 0, 0, 100.0f );	break;
					case 1:	FRGB_Add( &mpTask->mSeaDiffuse, 0, mAdd*0.01f, 0, 100.0f );	break;
					case 2:	FRGB_Add( &mpTask->mSeaDiffuse, 0, 0, mAdd*0.01f, 100.0f );	break;
				}
			}
			break;
		// -------------------------------------------------
		case ITEMID_SEA_SPECULAR:		//! スペキュラ
			mpTask->mSeaSpecular = TGL::Math::Clamp<f32>( mpTask->mSeaSpecular, mAdd*0.01f, 0.00f, 100.00f );
			break;
		// -------------------------------------------------
		case ITEMID_SEA_SPECULARPOWER:	//! スペキュラ範囲
			mpTask->mSeaSpecularPower = TGL::Math::Clamp<f32>( mpTask->mSeaSpecularPower, mAdd*0.1f, 1.0f, 128.0f );
			break;
		// -------------------------------------------------
		case ITEMID_SEA_FRESNELSCALE:	//! フレネル項スケール
			mpTask->mSeaFresnelScale = TGL::Math::Clamp<f32>( mpTask->mSeaFresnelScale, mAdd*0.1f, 0.0f, 100.0f );
			break;
		// -------------------------------------------------
		case ITEMID_SEA_FLICKERSPEED:	//! 揺らぎ速度
			mpTask->mSeaFlickerSpeed = TGL::Math::Clamp<f32>( mpTask->mSeaFlickerSpeed, mAdd*0.01f, 0.00f, 100.0f );
			break;
		// -------------------------------------------------
		case ITEMID_SEA_FLICKERFREQ:	//! 揺らぎ周期
			mpTask->mSeaFlickerFrequency = TGL::Math::Clamp<f32>( mpTask->mSeaFlickerFrequency, mAdd*1.0f, 0.0f, 10000.0f );
			break;
		// -------------------------------------------------
		case ITEMID_SEA_FLICKERAMP:		//! 揺らぎ振幅
			mpTask->mSeaFlickerAmplitude = TGL::Math::Clamp<f32>( mpTask->mSeaFlickerAmplitude, mAdd*0.001f, 0.0f, 100.0f );
			break;
		// -------------------------------------------------
		case ITEMID_LENSFLARE:	//! レンズフレア
			if( !JOY_TRG(PADOK) )	break;
			mpRoot->addMenu( APP_NEW DMenuLensFlare(mpRoot, mpTask->mpLensflare) );
			break;
		// -------------------------------------------------
		case ITEMID_LENSFLARE_SPACE:	//! レンズフレア(宇宙)
			if( !JOY_TRG(PADOK) )	break;
			mpRoot->addMenu( APP_NEW DMenuLensFlare(mpRoot, mpTask->mpLensflareSpace) );
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
void DMBGStg01::dispItem( TGL::DMenuItem* pItem, bool bCurrent, RGBA32 uColor/*=0xFFFFFFFF*/ )
{
	switch( pItem->getId() ){
		// -------------------------------------------------
		case ITEMID_SEA_NML1REPEAT:	//! 法線マップリピート
			IDPrint->addSh( uColor, "法線　 リピート : %4.1f\n", mpTask->mSeaNml1Repeat );
			break;
		// -------------------------------------------------
		case ITEMID_SEA_NML1SPDU:	//! 法線マップ横速度
			IDPrint->addSh( uColor, "　　　 横速度   : %4.2f\n", mpTask->mSeaNml1SpdU );
			break;
		// -------------------------------------------------
		case ITEMID_SEA_NML1SPDV:	//! 法線マップ縦速度
			IDPrint->addSh( uColor, "　　　 縦速度   : %4.2f\n", mpTask->mSeaNml1SpdV );
			break;
		// -------------------------------------------------
		case ITEMID_SEA_NML2REPEAT:	//! 法線マップリピート
			IDPrint->addSh( uColor, "法線２ リピート : %4.1f\n", mpTask->mSeaNml2Repeat );
			break;
		// -------------------------------------------------
		case ITEMID_SEA_NML2SPDU:	//! 法線マップ横速度
			IDPrint->addSh( uColor, "　　　 横速度   : %4.2f\n", mpTask->mSeaNml2SpdU );
			break;
		// -------------------------------------------------
		case ITEMID_SEA_NML2SPDV:	//! 法線マップ縦速度
			IDPrint->addSh( uColor, "　　　 縦速度   : %4.2f\n", mpTask->mSeaNml2SpdV );
			break;
		// -------------------------------------------------
		case ITEMID_SEA_NML2ALPHA:	//! 法線マップ透過度
			IDPrint->addSh( uColor, "　　　 透過度   : %4.2f\n", mpTask->mSeaNml2Alpha );
			break;
		// -------------------------------------------------
		case ITEMID_SEA_AMBIENT:	//! アンビエント
			IDPrint->addSh( uColor, "アンビエント    : %4.2f\n", mpTask->mSeaAmbient );
			break;
		// -------------------------------------------------
		case ITEMID_SEA_REFLECTION:	//! 環境マップ強度
			IDPrint->addSh( uColor, "環境マップ 強度 : %5.2f\n", mpTask->mSeaReflection );
			break;
		// -------------------------------------------------
		case ITEMID_SEA_DIFFUSE:		//! ディフューズ
			IDPrint->addSh( uColor, "ディフューズ    : " );
			if( !isCurrentItem(pItem) )
				IDPrint->addSh( uColor, "%4.2f, %4.2f, %4.2f\n", mpTask->mSeaDiffuse.fR, mpTask->mSeaDiffuse.fG, mpTask->mSeaDiffuse.fB );
			else{
				if( KBBTN_DAT(KBBTNS_SHIFT) ){	//!< 全部
					IDPrint->addSh( uColor, "[%4.2f], [%4.2f], [%4.2f]\n", mpTask->mSeaDiffuse.fR, mpTask->mSeaDiffuse.fG, mpTask->mSeaDiffuse.fB );
				}else{	//!< 個別
					switch( pItem->getParamI() ){
						case 0:	IDPrint->addSh( uColor, "[%4.2f], %4.2f, %4.2f", mpTask->mSeaDiffuse.fR, mpTask->mSeaDiffuse.fG, mpTask->mSeaDiffuse.fB ); break;
						case 1:	IDPrint->addSh( uColor, "%4.2f, [%4.2f], %4.2f", mpTask->mSeaDiffuse.fR, mpTask->mSeaDiffuse.fG, mpTask->mSeaDiffuse.fB ); break;
						case 2:	IDPrint->addSh( uColor, "%4.2f, %4.2f, [%4.2f]", mpTask->mSeaDiffuse.fR, mpTask->mSeaDiffuse.fG, mpTask->mSeaDiffuse.fB ); break;
					}
					IDPrint->addSh( uColor, "    Shift:AllSelect\n" );
				}
			}
			break;
		// -------------------------------------------------
		case ITEMID_SEA_SPECULAR:		//! スペキュラ
			IDPrint->addSh( uColor, "スペキュラ      : %4.2f\n", mpTask->mSeaSpecular );
			break;
		// -------------------------------------------------
		case ITEMID_SEA_SPECULARPOWER:	//! スペキュラ範囲
			IDPrint->addSh( uColor, "　　　　　 範囲 : %4.1f\n", mpTask->mSeaSpecularPower );
			break;
		// -------------------------------------------------
		case ITEMID_SEA_FRESNELSCALE:	//! フレネル項スケール
			IDPrint->addSh( uColor, "フレネル項 係数 : %4.1f\n", mpTask->mSeaFresnelScale );
			break;
		// -------------------------------------------------
		case ITEMID_SEA_FLICKERSPEED:	//! 揺らぎ速度
			IDPrint->addSh( uColor, "揺らぎ 速度     : %4.2f\n", mpTask->mSeaFlickerSpeed );
			break;
		// -------------------------------------------------
		case ITEMID_SEA_FLICKERFREQ:	//! 揺らぎ周期
			IDPrint->addSh( uColor, "       周期     : %3.0f\n", mpTask->mSeaFlickerFrequency );
			break;
		// -------------------------------------------------
		case ITEMID_SEA_FLICKERAMP:		//! 揺らぎ振幅
			IDPrint->addSh( uColor, "       振幅     : %5.3f\n", mpTask->mSeaFlickerAmplitude );
			break;
		// -------------------------------------------------
		case ITEMID_LENSFLARE:	//! レンズフレア
			IDPrint->addSh( uColor, "レンズフレア :\n" );
			break;
		// -------------------------------------------------
		case ITEMID_LENSFLARE_SPACE:	//! レンズフレア(宇宙)
			IDPrint->addSh( uColor, "レンズフレア(宇宙) :\n" );
			break;
		// -------------------------------------------------
		case TGL::DMenuItem::ITEMID_UVSPD:	//!	UV速度
		{
			const char*	_NAME[UVSPD_MAX] = {
				"泡 A",
				"泡 B",
				"川 06",
				"川 07",
				"ドライブ軌道A",
				"ドライブ軌道B",
				"ドライブ軌道C",
				"ドライブ軌道D",
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
TGL::Vec2* DMBGStg01::getTaskUV( u8 uId )
{
	TGL::Vec2*	pvRet[UVSPD_MAX] = {
		&mpTask->mFoamA_Uv,
		&mpTask->mFoamB_Uv,
		&mpTask->mRiver06_Uv,
		&mpTask->mRiver07_Uv,
		&mpTask->mSLSD_UvA,
		&mpTask->mSLSD_UvB,
		&mpTask->mSLSD_UvC,
		&mpTask->mSLSD_UvD,
	};
	return pvRet[uId];
}

/* ========================================================================= */
/*		EOF																	 */
/* ========================================================================= */
