// -------------------------------------------------------------------------- //
//! @file   tRain.cpp
//! @brief  デジタルライン
//! @author Nal
//! @since  13/06/26(水)
//!
//! COPYRIGHT (C) 2013 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
/* ====< インクルード >====================================================== */
#include "prefix.h"
#include <TGL.h>
#include "sTGLMain.h"
#include "tRain.h"
#include "sTGLPause.h"
#include "Game/Game.h"
#include "Action/ActionTypes.h"
#include "Effect/sAppEffect.h"

/* ========================================================================= */
/*		tRain																 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  コンストラクタ "tRain"
//! @param  pParent	[i/o]	親
//! @param  uPri	[in]	描画優先
// ---------------------------------------------
tRain::tRain( tBG* pParent, u8 uPri )
:	mpParent(pParent), mbEnable(true), mPri(uPri),
	mColorMin(IRGBA(255,255,255,255)), mColorMax(IRGBA(255,255,255,255)), mSpd(TGL::Vec3::ZERO), mAlpha(1)
{
	//!	プリミティブ確保
	mPrimNum = (u16)(512.0f * ITGLSystem->getScreenRateY());
	mpPrim = APP_NEW Prim[mPrimNum];
	
	//!	頂点バッファ確保
	mpVBuf = APP_NEW TGL::VertexBuffer( mPrimNum*2, sizeof(TGL::Vertex::POS_COLOR_T), false );
	
	//!	タスク接続
	ZeroMemory( &mTskHead, sizeof(GLTSKHEAD_T) );
	GLTSK_SetNext( GLTSK_GetBase(GTOT_BG(1)), &mTskHead, this, (s32 (*)(void*,u32))tRain::updateTask );
}

// ---------------------------------------------
//! @brief  デストラクタ "tRain"
// ---------------------------------------------
tRain::~tRain()
{
	//!	タスク切断
	GLTSK_Remove( &mTskHead );
	SAFE_DELETE_ARRAY( mpPrim );
	SAFE_DELETE( mpVBuf );
}

// ---------------------------------------------
//! @brief  定期更新
// ---------------------------------------------
s32 tRain::updateTask( tRain* pTsk, u32 uMsg )
{
	// ---------------------------------
	//		タスクメッセージ受取		
	if( uMsg ){
		if( tstFLG(uMsg,SMSG_END) ){	// 終了
			return true;
		}
		return 0;
	}
	
	// ---------------------------------
	//		更新						
	pTsk->update();

	return 0;
}

// ---------------------------------------------
//! @brief  更新
// ---------------------------------------------
void tRain::update()
{
	if( !mbEnable )	return;
	
	//!	描画リスト登録
	if( !IACtrl->getScene().tstSceneF(SCENEF_NORENDER) )
		ITGLRenderer->addDrawList( TGL::Renderer::DPASS_MAIN, mPri, mTskHead.mTot, TGL::Camera::CAMF_00, tRain::drawlistExec, (void*)this );
	
	if( IMain->isPause() || IPause->isActive() )	return;
	if( IACtrl->getTime().isStop() )	return;
	
	//!	更新
	//!	視錐台の上と右を計算
	TGL::Plane	top, right;
	TGL::BaseCamera*	pCam = ICamera->getCamera(0);
	pCam->getFrustum( &top, NULL, NULL, &right );
	
	TGL::Vec3	vT, vR, vEye;
	f32	fDist;
	fDist = top.dotCoord( TGL::Vec3(pCam->getTar().x, pCam->getTar().y, pCam->getTar().z) );
	vT = pCam->getUpVector() * fDist * 1.5f;
	f32	fT = vT.length();

	fDist = right.dotCoord( TGL::Vec3(pCam->getTar().x, pCam->getTar().y, pCam->getTar().z) );
	vR = -pCam->getRightVector() * fDist * 1.5f;
	
	//!	雨
	for( u16 i=0 ; i<mPrimNum ; i++ )
		mpPrim[i].update( *this, vR, fT );
}

// ---------------------------------------------
//! @brief  更新
//! @param  fDelta	[in]	デルタタイム
// ---------------------------------------------
void tRain::Prim::update( const tRain& parent, const TGL::Vec3& vRight, f32 fUp )
{
	TGL::BaseCamera*	pCam = ICamera->getCamera(0);

	//!	新規生成
	if( mLife < DELTA ){
		mLife	= RANDF2( 10, 30 );
		//!	水平位置
		mPos	= pCam->getTar();
		mPos	+= TGL::Vec3(pCam->getMove().x, 0, pCam->getMove().z) * 10.0f;
		mPos	+= TGL::Vec3::Z * vRight.length() * RANDF2( -1, 1 );
		mPos	+= TGL::Vec3::X * vRight.length() * RANDF2( -1, 1 );
		//!	高さ
		mPos.y	+= fUp * RANDF2( -1.0f, 1.5f );
		//!	速度
		mSpd.y	= (-fUp * 1.0f) / (f32)mLife;
		mSpd.x	= parent.mSpd.x;
		mSpd.z	= parent.mSpd.z;
	}
	
	//!	移動
	mPosOld	= mPos;
	mPos	+= mSpd * parent.mpParent->getDeltaTime();
//	mPos	-= TGL::Vec3(pCam->getMove().x, 0, pCam->getMove().z) * fDelta * 0.125f;

	//!	色
	mColor	= RGBA32_Lerp( parent.mColorMin, parent.mColorMax, RANDF2(0,1) );

	mLife -= parent.mpParent->getDeltaTime();
}

// ---------------------------------------------
//! @brief  描画
// ---------------------------------------------
void tRain::draw()
{
	if( !mbEnable )	return;

	mpParent->getPerfGpu().begin();

	TGL::BaseCamera*	pCam = ICamera->getRenderCamera();
	if( !pCam )	return;
	
	TGL::Matrix	mtxWVP = *pCam->getViewMtx() * *pCam->getProjMtx();
	
	//!	デバイス設定
	IGraphics->setWorldTransform();
	IGraphics->getRS().setZ( TGL::RenderState::Z_TEST | TGL::RenderState::Z_WRITE );
	IGraphics->getRS().setCull( TGL::RenderState::CULL_NONE );

	//!		ライン		
	IGraphics->getRS().setBlend( RSBL_HALF, true );

	//!	頂点設定
	TGL::Vertex::POS_COLOR_T*	pV = (TGL::Vertex::POS_COLOR_T*)mpVBuf->lock();

	u16	vnum = 0;
	for( u16 i=0 ; i<mPrimNum ; i++ ){
		pV[vnum].mPos	= mpPrim[i].mPos;
		pV[vnum].mColor	= mpPrim[i].mColor;
		
		pV[vnum+1].mPos		= mpPrim[i].mPosOld;
		pV[vnum+1].mColor	= RGBA32_MulAlpha( mpPrim[i].mColor, 0 );
		vnum += 2;
	}
	mpVBuf->unlock();
	mpVBuf->setVtxNum( vnum );

	//!		描画	
	TGL::ShaderEffect*	pShader = IGraphics->getSystemShader( TGL::SystemShader::SH_COLOR, ICamera->getDrawMtx() );
	IVertex->setVertexLayout( TGL::Vertex::VTXDEC_COLOR, *mpVBuf, pShader );
	pShader->begin();
	IGraphics->draw( TGL::Graphics::PRIM_LINELIST );
	pShader->end();
	
	mpParent->getPerfGpu().end();
}

/* ========================================================================= */
/*		EOF																	 */
/* ========================================================================= */
