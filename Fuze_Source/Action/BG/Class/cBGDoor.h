// -------------------------------------------------------------------------- //
//! @file   cBGDoor.h
//! @brief  ドア
//! @author Nal
//! @since  13/09/14(土)
//!
//! COPYRIGHT (C) 2013 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
#pragma	once
/* ====< インクルード >====================================================== */
#include "cModelSet.h"
#include "../Core/tBG.h"

/* ========================================================================= */
//		BGDoor																 */
/* ========================================================================= */
class BGDoor
{
public:
	//!	コンストラクタ
	BGDoor( cModelSet* pModelSet, u8 uSceneS, u8 uSceneE, u8 uCamGroup, f32 fCall, f32 fAnmFrame );
	//!	デストラクタ
	virtual	~BGDoor();

public:	
	// -----------------------------------------------------------------
	//		関数		
	// -----------------------------------------------------------------
	//!	リセット
	virtual void	reset() {
		if( mpSE )	mpSE->stop();
		mbActive	= false;
		mAnmOld		= 0;
		mpSE		= NULL;
	}

	//!	更新
	void	update( tBG& Bg );

	//		アクセサ		
	cBGModel*	getModel( tBG& Bg );
	cModelSet*	getModelSet()	{ return mpModelSet; }

protected:
	// -----------------------------------------------------------------
	//		定義		
	// -----------------------------------------------------------------

	// -----------------------------------------------------------------
	//		関数		
	// -----------------------------------------------------------------
	//!	ドア移動音
	virtual s16	getSELoop() const			{ return -1; }
	virtual f32	getSELoopDistance() const	{ return 6000.0f; }
	//!	ドア閉まった音
	virtual s16	getSEClose() const			{ return -1; }
	virtual f32	getSECloseDistance() const	{ return 8000.0f; }
	
	//!	カメラ振動判定
	virtual bool	isShake( cBGModel* pModel );
	//!	カメラ振動強さ
	virtual TGL::Vec3		getShakePower() const	{ return TGL::Vec3(50,50,0); }

	// -----------------------------------------------------------------
	//		変数		
	// -----------------------------------------------------------------
	bool				mbActive;			//!< 動作中
	cModelSet*			mpModelSet;			//!< ドアモデル
	const u8			mSceneS, mSceneE;	//!< 有効シーン
	const u8			mCamGroup;			//!< カメラグループ
	const f32			mCallTiming;		//!< モーションコールタイミング
	const f32			mAnmFrame;			//!< モーション時間
	f32					mAnmOld;			//!< 前回のモーション再生時間
	TGL::SoundChannel*	mpSE;				//!< ループ音
};

/* ========================================================================= */
/*		EOF																	 */
/* ========================================================================= */
