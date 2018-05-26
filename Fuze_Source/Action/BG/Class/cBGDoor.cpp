// -------------------------------------------------------------------------- //
//! @file   cBGDoor.cpp
//! @brief  ドア
//! @author Nal
//! @since  13/09/14(土)
//!
//! COPYRIGHT (C) 2013 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
/* ====< インクルード >====================================================== */
#include "prefix.h"
#include <TGL.h>
#include "cBGDoor.h"
#include "Sound/sAppSound.h"

/* ========================================================================= */
/*		BGDoor																 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  コンストラクタ "BGDoor"
//! @param  pModelSet	[in]	モデルセット
//! @param  uSceneS		[in]	有効シーン開始
//! @param  uSceneE		[in]	有効シーン終了
//! @param  uCamGroup	[in]	カメラグループ
//! @param  fCall		[in]	モーションコールタイミング
//! @param  fAnmFrame	[in]	モーション時間
// ---------------------------------------------
BGDoor::BGDoor( cModelSet* pModelSet, u8 uSceneS, u8 uSceneE, u8 uCamGroup, f32 fCall, f32 fAnmFrame )
:	mpModelSet(pModelSet), 
	mSceneS(uSceneS), mSceneE(uSceneE), 
	mCamGroup(uCamGroup), mCallTiming(fCall), mAnmFrame(fAnmFrame), 
	mpSE(NULL)
{
	reset();
}

// ---------------------------------------------
//! @brief  デストラクタ "BGDoor"
// ---------------------------------------------
BGDoor::~BGDoor()
{
	if( mpSE )
		mpSE->stop();
}

// ---------------------------------------------
//! @brief  更新
//! @param  Bg	[in]	背景タスク
// ---------------------------------------------
void BGDoor::update( tBG& Bg )
{
	cBGModel*	pModel = getModel( Bg );
	if( !pModel || !pModel->getAnim() )	return;
	
	//!	リセット
	TGL::PathCamera*	pCam = (TGL::PathCamera*)( ICamera->getCamera(0) );
	if( pCam->getCurrentGroupId() < mCamGroup
		|| (Bg.getScene() < mSceneS || Bg.getScene() > mSceneE) ){
		reset();
		return;
	}
	if( pCam->getCurrentGroupId() == mCamGroup && pCam->getTime() < mCallTiming ){
		if( mCamGroup == 0 )
			pModel->getAnim()->Play( 0, false, false );
		reset();
		return;
	}

	if( !mbActive ){
		//!	再生
		if( pCam->getCurrentGroupId() == mCamGroup && pCam->getTimeOld() < mCallTiming ){
			pModel->getAnim()->Play( 1, false, true );
			pModel->getAnim()->SetAnimationSpeed( 60.0f/mAnmFrame );
			mAnmOld		= 0;
			mbActive	= true;
			//!	SE
			mpSE = IAppSound->play( SDID_ONMEM, getSELoop(), TGL::SoundChannel::PLAYF_LOOP | TGL::SoundChannel::PLAYF_RANDSTART | TGL::SoundChannel::PLAYF_NOHISTORY );
			if( mpSE ){
				mpSE->setField( mpModelSet->getPos()+Bg.getOrigin(), getSELoopDistance(), 0.3f, 1 );
				mpSE->setVolume( 0.01f, 1.0f, 15 );
				mpSE->setLoopKeep( 15 );
			}
		}
	}else{
		//!	振動
		if( isShake(pModel) ){
			if( ICamera->getCamera(0) )
				ICamera->getCamera(0)->reqShake( 180, getShakePower(), TGL::TransValue::CURVE_SUB1 );
			if( ICamera->getCamera(1) )
				ICamera->getCamera(1)->reqShake( 180, TGL::Vec3(5,5,0), TGL::TransValue::CURVE_SUB1 );
			//!	SE
			TGL::SoundChannel*	pCh = IAppSound->play( SDID_ONMEM, getSEClose() );
			if( pCh )
				pCh->setField( mpModelSet->getPos()+Bg.getOrigin(), getSECloseDistance(), 0.3f, 1 );
		}

		//!	ループSE監視
		if( mpSE ){
			if( pModel->getAnim()->IsPlay() ){
				SAFE_LOOPKEEP_ON( mpSE );
			}else
				mpSE = NULL;
		}
		
		mAnmOld		= pModel->getAnim()->GetCurrentTime();
		mbActive	= pModel->getAnim()->IsPlay();
	}
}

// ---------------------------------------------
//! @brief  カメラ振動判定
//! @param  pModel	[in]	ドアモデル
//! @return true:振動
// ---------------------------------------------
bool BGDoor::isShake( cBGModel* pModel )
{
	if( !pModel->getAnim()->IsPlay() )	//!< 再生終了
		return true;
	
	return false;
}

// ---------------------------------------------
//! @brief  モデル取得
//! @return モデル
// ---------------------------------------------
cBGModel* BGDoor::getModel( tBG& Bg )
{
	return Bg.getModel( mpModelSet->getName(), mpModelSet->getModelId() );
}

/* ========================================================================= */
/*		EOF																	 */
/* ========================================================================= */
