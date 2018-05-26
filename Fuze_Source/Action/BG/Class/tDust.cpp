// -------------------------------------------------------------------------- //
//! @file   tDust.cpp
//! @brief  塵
//! @author Nal
//! @since  13/11/19(火)
//!
//! COPYRIGHT (C) 2013 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
/* ====< インクルード >====================================================== */
#include "prefix.h"
#include <TGL.h>
#include "sTGLMain.h"
#include "tDust.h"
#include "sTGLPause.h"
#include "Game/Game.h"
#include "Action/ActionTypes.h"
#include "Effect/sAppEffect.h"

/* ========================================================================= */
/*		tDust																 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  コンストラクタ "tDust"
//! @param  pParent	[i/o]	親
//! @param  uPri	[in]	描画優先
// ---------------------------------------------
tDust::tDust( tBG* pParent, u8 uPri )
:	mpParent(pParent), mbEnable(true), mPri(uPri),
	mColorMin(IRGBA(255,255,255,255)), mColorMax(IRGBA(255,255,255,255)), 
	mFogColor(0), mFogNear(0), mFogFar(0), mSpd(0), mCameraMove(1), mAlpha(1)
{
	//!	頂点バッファ確保
	mPrimNum	= (u16)(1536.0f * ITGLSystem->getScreenRateY());
	mpPrim		= APP_NEW Prim[mPrimNum];
	mpVBuf		= APP_NEW TGL::VertexBuffer( mPrimNum, sizeof(TGL::Vertex::POS_COLOR_T), false );
	
	//!	タスク接続
	ZeroMemory( &mTskHead, sizeof(GLTSKHEAD_T) );
	GLTSK_SetNext( GLTSK_GetBase(GTOT_BG(1)), &mTskHead, this, (s32 (*)(void*,u32))tDust::updateTask );
}

// ---------------------------------------------
//! @brief  デストラクタ "tDust"
// ---------------------------------------------
tDust::~tDust()
{
	//!	タスク切断
	GLTSK_Remove( &mTskHead );
	SAFE_DELETE( mpVBuf );
	SAFE_DELETE_ARRAY( mpPrim );
}

// ---------------------------------------------
//! @brief  定期更新
// ---------------------------------------------
s32 tDust::updateTask( tDust* pTsk, u32 uMsg )
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
void tDust::update()
{
	if( !mbEnable )	return;
	
	//!	描画リスト登録
	if( !IACtrl->getScene().tstSceneF(SCENEF_NORENDER) )
		ITGLRenderer->addDrawList( TGL::Renderer::DPASS_MAIN, mPri, mTskHead.mTot, TGL::Camera::CAMF_00, tDust::drawlistExec, (void*)this );
	
	if( IMain->isPause() || IPause->isActive() )	return;
	if( IACtrl->getTime().isStop() )	return;
	
	//!	更新
	//!	視錐台の上と右を計算
	TGL::Plane	top, right;
	TGL::BaseCamera*	pCam = ICamera->getCamera(0);
	pCam->getFrustum( &top, NULL, NULL, &right );
	
	//!	基準位置
	TGL::Vec3	vTar;
	if( mFogFar > DELTA )
		vTar = pCam->getPos() + pCam->getEyeVector() * (mFogFar*0.3f);
	else
		vTar = pCam->getTar();
	
	TGL::Vec3	vT, vR, vEye;
	f32	fDist;
	fDist = top.dotCoord( TGL::Vec3(vTar.x,vTar.y,vTar.z) );
	vT = pCam->getUpVector() * fDist * 1.5f;
	f32	fT = vT.length();

	fDist = right.dotCoord( TGL::Vec3(vTar.x,vTar.y,vTar.z) );
	vR = -pCam->getRightVector() * fDist * 1.5f;
	f32	fR = vR.length();
	
	//!	雨
	for( u16 i=0 ; i<mPrimNum ; i++ )
		mpPrim[i].update( *this, i, vTar, pCam->getRightVector(), pCam->getUpVector(), pCam->getEyeVector(), fR, fT );
}

// ---------------------------------------------
//! @brief  更新
//! @param  parent	[in]	親
//! @param  uIdx	[in]	配列番号
//! @param  fRight	[in]	ライトベクトル スカラ
//! @param  fUp		[in]	アップベクトル スカラ
// ---------------------------------------------
void tDust::Prim::update( const tDust& parent, u16 uIdx, const TGL::Vec3& vTar, const TGL::Vec3& vRight, const TGL::Vec3& vUp, const TGL::Vec3& vEye, f32 fRight, f32 fUp )
{
	TGL::BaseCamera*	pCam = ICamera->getCamera(0);

	//!	新規生成
	if( isInit(parent,uIdx) ){
		mLife	= 480;
		//!	位置
		mPos	= vTar;
		mPos	+= vRight	* fRight * RANDF2( -1, 1 );
		mPos	+= vEye		* parent.mFogFar * RANDF2( -0.3f, 0.7f );
		mPos	+= vUp		* fUp * RANDF2( -1, 1 );
		//!	速度
		mSpd = TGL::Vec3::rand();
		mSpd	*= RANDF2( 0, parent.mSpd );
	}
	
	//!	移動
	mPosOld	= mPos;
	mPos	+= mSpd * parent.mpParent->getDeltaTime();
	mPos	+= pCam->getMove() * (1.0f-parent.mCameraMove);

	//!	色
	mColor	= RGBA32_Lerp( parent.mColorMin, parent.mColorMax, RANDF2(0,1) );

	//!	フォグ
	if( parent.mFogFar > DELTA ){
		f32	fDist = TGL::Math::Clamp<f32>( pCam->getPos().distance(mPos), parent.mFogNear, parent.mFogFar );
		mColor	= RGBA32_Lerp( mColor, parent.mFogColor, ((fDist-parent.mFogNear)/parent.mFogFar) );
	}
	
	mLife -= parent.mpParent->getDeltaTime();
}

// ---------------------------------------------
//! @brief  初期化判定
//! @param  uIdx	[in]	配列番号
//! @return true:初期化
// ---------------------------------------------
bool tDust::Prim::isInit( const tDust& parent, u16 uIdx ) const
{
	if( mLife < DELTA )	return true;
	if( (ITimer->getTotalFrame() % 20) != (uIdx % 20) )	return false;	//!< 更新頻度チェック

	TGL::BaseCamera*	pCam = ICamera->getCamera(0);
	//!	遠い
	if( parent.mFogFar > DELTA ){
		f32	fDist = pCam->getPos().distance( mPos );
		if( fDist > parent.mFogFar )	return true;
	}
	//!	画面外
	return pCam->isOutScreen(mPos, 0) != 0;
}

// ---------------------------------------------
//! @brief  描画
// ---------------------------------------------
void tDust::draw()
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

	//!		プリミティブ		
	IGraphics->getRS().setBlend( RSBL_HALF, false );

	//!	頂点設定
	TGL::Vertex::POS_COLOR_T*	pV = (TGL::Vertex::POS_COLOR_T*)mpVBuf->lock();

	u16	vnum = 0;
	for( u16 i=0 ; i<mPrimNum ; i++ ){
		pV[vnum].mPos	= mpPrim[i].mPos;
		pV[vnum].mColor	= mpPrim[i].mColor;
		vnum += 1;
	}
	mpVBuf->unlock();
	mpVBuf->setVtxNum( vnum );

	//!		描画	
	TGL::ShaderEffect*	pShader = IGraphics->getSystemShader( TGL::SystemShader::SH_COLOR, ICamera->getDrawMtx() );
	IVertex->setVertexLayout( TGL::Vertex::VTXDEC_COLOR, *mpVBuf, pShader );
	pShader->begin();
	IGraphics->draw( TGL::Graphics::PRIM_POINTLIST );
	pShader->end();
	
	mpParent->getPerfGpu().end();
}

/* ========================================================================= */
/*		EOF																	 */
/* ========================================================================= */
