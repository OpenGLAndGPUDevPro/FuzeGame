// -------------------------------------------------------------------------- //
//! @file   tBGStgBefore.cpp
//! @brief  背景 C81面
//! @author Nal
//! @since  11/11/15(火)
//! 
//! COPYRIGHT (C) 2011 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
/* ====< インクルード >====================================================== */
#include "prefix.h"
#include <TGL.h>
#include "Game/Game.h"
#include "Action/Camera/cBGCamera.h"
#include "tBGStgBefore.h"
#include "Action/ActionTypes.h"
#include "Effect/sAppEffect.h"

/* ========================================================================= */
//!		tBGStgBefore														 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  コンストラクタ "tBGStgBefore"
// ---------------------------------------------
tBGStgBefore::tBGStgBefore()
:	tBG(), mpDigitalLine(NULL)
{
	reset();
}

// ---------------------------------------------
//! @brief  デストラクタ "tBGStgBefore"
// ---------------------------------------------
tBGStgBefore::~tBGStgBefore()
{
	IAppResource->releaseTexPool( "models/BG/common/St02R/St02_Space_envC.dds" );
	SAFE_DELETE( mpDigitalLine );
}

// ---------------------------------------------
//! @brief  リセット
// ---------------------------------------------
void tBGStgBefore::reset()
{
	tBG::reset();

	mTimeOld		= 0;
	mScrollOld		= 0;
	mScrollWay.setEnd( TGL::Vec3::ZERO );
	mScrollWay.clear();
	mScrollTar.clear( TGL::Vec3(1000, 0, 0) );
	mScrollTwist.clear();

	SAFE_DELETE( mpDigitalLine );
}

// ---------------------------------------------
//! @brief  リソースロード
//! @param  uBG	[in]	BGID_????
//! @return true:成功, false:失敗
// ---------------------------------------------
bool tBGStgBefore::load( u8 uBG )
{
	addModel( "St02_Far_02.mdl", cBGModel::TYPE_SKYDOME, cBGModel::FLAG_FAROBJ );

	//!	テクスチャ
	IAppResource->loadTexPool( "models/BG/common/St02R/St02_Space_envC.dds" );

	//!	デジタルライン
	SAFE_DELETE( mpDigitalLine );
	mpDigitalLine = APP_NEW tDigitalLine( this, GPRI_EFT(0) );
	mpDigitalLine->setAlpha( 0 );

	//!		シーンパラメータ		
	mSceneParam.getFromJson( "param/BG/C81/C81_param.json" );

	//!	シーン環境マップ
	mSceneParam.setEnvTex( "St02_Space_envC.dds" );
	ICharacter->overwriteBGEnvTex();	//!< キャラクターへの適用
	overwriteEnvTex();	//!< 背景モデルへの適用

	return true;
}

// ---------------------------------------------
//! @brief  更新
// ---------------------------------------------
void tBGStgBefore::update()
{
	mTimeOld = mTimePass;

	//!	親クラス
	tBG::update();
	
	//!	仮想空間フェードイン
	if( mScene >= 1 ){
		mpDigitalLine->setEnable( true );
		mpDigitalLine->setAlpha( TGL::Math::Lerp<f32>(mpDigitalLine->getAlpha(), 1.0f, 0.01f * getDeltaTime()) );
	}else{
		mpDigitalLine->setEnable( false );
	}

	mScrollOld = IACtrl->getScene().getScrollType();

	//!	フィルタ描画登録
	ITGLRenderer->addDrawList( TGL::Renderer::DPASS_MAIN, GPRI_EFT(5), mTskHead.mTot, TGL::Camera::CAMF_00, tBGStgBefore::drawlistExecFilter, (void*)this );
}

// ---------------------------------------------
//! @brief  スクロール更新
// ---------------------------------------------
void tBGStgBefore::updateScroll()
{
	//!	カメラ移動
	if( mScene >= 2 ){
		BGCamera*	pCam = (BGCamera*)( ICamera->getCamera(0) );
		
		mScrollWay.update( getDeltaTime() );
		mScrollTar.update( getDeltaTime() );
		mScrollTwist.update( getDeltaTime() );

		//!	注視点・ツイスト
		if( (((u32)mTimePass / 600) > ((u32)mTimeOld / 600))
			|| (mScrollOld != IACtrl->getScene().getScrollType())	//!< スクロール変更瞬間
			|| mScrollWay.getEnd().isZero() ){		//!< スクロール未設定時
			
			//!	ツイスト
			f32	fTwist = 10.0f;
			if( IACtrl->getScene().getScrollType() == SCROLL_SIDE )	fTwist = 60.0f;
			mScrollTwist.init( TGL::TransValue::MODE_NORMAL, 600, TGL::TransValue::CURVE_PARABOLIC, 999, RANDF2(-fTwist, fTwist) );

			//!	注視点
			TGL::Vec3	v0;
			v0 = TGL::Vec3::rand();
			v0 *= 1000.0f;
			if( IACtrl->getScene().getScrollType() == SCROLL_SIDE ){	//!< サイドビュー
				v0.x += 1000.0f;
			}else{	//!< トップビュー
				v0.z = FABS(v0.z) + 300.0f;
				v0.y -= 1000.0f;
			}
			mScrollTar.init( TGL::TransValue::MODE_NORMAL, 600, TGL::TransValue::CURVE_PARABOLIC, 999, v0 );
		}
		//!	スクロール
		if( (((u32)mTimePass / 300) > ((u32)mTimeOld / 300))
			|| mScrollWay.getEnd().isZero() ){
			TGL::Vec3	v0 = TGL::Vec3( RANDF2(-15.0f,15.0f), RANDF2(-5.0f,5.0f), RANDF2(5.0f,40.0f) );
			mScrollWay.init( TGL::TransValue::MODE_NORMAL, 300, TGL::TransValue::CURVE_PARABOLIC, 999, v0 );
		}
		
		if( pCam && !pCam->isFree() ){
			pCam->setOrderPos( mScrollWay.getValue() * getDeltaTime() );
			pCam->setOrderTar( (pCam->getPos()+mScrollTar.getValue()) - pCam->getTar() );
			pCam->setOrderTwist( mScrollTwist.getValue() - pCam->getTwist() );
		}
	}
}

// ---------------------------------------------
//! @brief  フィルタ描画
// ---------------------------------------------
void tBGStgBefore::drawFilter()
{
#if 0
	TGL::BaseCamera*	pCam = ICamera->getRenderCamera();
	if( !pCam )	return;

	//!		仮想空間		
	if( mScene >= 1 ){
		//	デバイス設定
		IGraphics->setWorldTransform();
		IGraphics->getRS().setZ( TGL::RenderState::Z_TEST );
		IGraphics->getRS().setCull( TGL::RenderState::CULL_NONE );
		IGraphics->getRS().setBlend( RSBL_HALF, true );

		//		フィルタ		
		//!	シェーダー取得
		TGL::ShaderEffect*	pEffect = IAppResource->getShaderEffect( "Eft_RadialWave" );
		
		const u32	_VTX = 8+2;
		TGL::Vertex::POS_COLOR_TEX_T	vtx[_VTX];

		//!	頂点座標
		vtx[0].mPos = TGL::Vec3( 0, 0, 0 );
		f32	fDeg = 0;
		for( u8 i=1 ; i<_VTX-1 ; i++ ){
			vtx[i].mPos = TGL::Vec3( 5000.0f*SIN(fDeg), 5000.0f*COS(fDeg), 0.0f );
			fDeg += 360.0f / (f32)(_VTX-2);
		}
		vtx[_VTX-1].mPos = vtx[1].mPos;
		//!	カメラ座標系に変換
		for( u8 i=0 ; i<_VTX ; i++ ){
			vtx[i].mPos = vtx[i].mPos.applySR( *pCam->getViewMtxInv() );
			vtx[i].mPos += pCam->getTar();
			pCam->adjustPosScreen( vtx[i].mPos, 0.0f );
		}

		//!	UV
		for( u8 i=0 ; i<_VTX ; i++ ){
			vtx[i].mColor = IRGBA(16, 255, 80, 255*mpDigitalLine->getAlpha());
			TGL::Vec3	v0 = pCam->getScreenPos( vtx[i].mPos );
			vtx[i].mTex.x = (v0.x+0.5f) / (f32)ITGLRenderer->getBBTexWidth();
			vtx[i].mTex.y = (v0.y+0.5f) / (f32)ITGLRenderer->getBBTexHeight();
		}
		vtx[0].mColor = IRGBA(255,255,255,0);

		//!	重心
		f32	fUVCenter[2];
		fUVCenter[0] = vtx[0].mTex.x;
		fUVCenter[1] = vtx[0].mTex.y;
	
		//!	テクスチャ
		TGL::BaseTex*	pTex = ITexture->getCmn( ETEX_RS );
		f32	fAspect = (f32)pTex->getHeight() / (f32)pTex->getWidth();

		//!	変換マトリクス計算
		TGL::Matrix	mtxWVP = *pCam->getViewMtx() * *pCam->getProjMtx();
		//!	マトリクス
		pEffect->setMatrix( "WvpXf", mtxWVP );
		//!	パラメータ
		f32	fAmp	= 0.020f;
		f32	fFreq	= 200.0f;
		f32	fPhase	= mTimePass * 0.2f;
		pEffect->setValue( "gAmplitude",	&fAmp,		sizeof(f32) );
		pEffect->setValue( "gFrequency",	&fFreq,		sizeof(f32) );
		pEffect->setValue( "gPhase",		&fPhase,	sizeof(f32) );
		pEffect->setValue( "gCenter",		fUVCenter,	sizeof(f32)*2 );
		pEffect->setValue( "gAspect",		&fAspect,	sizeof(f32) );
		//!	テクスチャ
		pEffect->setTexture( "MeshTexture", pTex );
		//!	適用
		pEffect->commitChange();

		IVertex->setVertexLayout( TGL::Vertex::VTXDEC_COLOR_TEX, vtx, 10, pEffect );
		//!	描画
		pEffect->begin();
		IGraphics->draw( TGL::Graphics::PRIM_TRIANGLEFAN );
		pEffect->end();
	}
#endif
}
