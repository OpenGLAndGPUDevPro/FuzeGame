// -------------------------------------------------------------------------- //
//! @file   cBGCamera.h
//! @brief  背景カメラ
//! @author Nal
//! @since  11/12/07(水)
//!
//! COPYRIGHT (C) 2011 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
#pragma	once
/* ====< インクルード >====================================================== */
#include "cGameCamera.h"

/* ========================================================================= */
//		BGCamera															 */
/* ========================================================================= */
class BGCamera : public GameCamera
{
public:
	BGCamera( u32 uFlag=FLAGS_DEFAULT, TGL::RenderTex* pRT=NULL );
	virtual ~BGCamera();

public:	
	// -----------------------------------------------------------------
	//		定義		
	// -----------------------------------------------------------------
	enum {
		BGCAMF_ORDERPOS		= (1<< 0),	//!< 更新予約 座標
		BGCAMF_ORDERTAR		= (1<< 1),	//!< 更新予約 注視点
		BGCAMF_ORDERFOV		= (1<< 2),	//!< 更新予約 画角
		BGCAMF_ORDERTWIST	= (1<< 3),	//!< 更新予約 ツイスト値
		BGCAMFS_ORDER		= BGCAMF_ORDERPOS | BGCAMF_ORDERTAR | BGCAMF_ORDERFOV | BGCAMF_ORDERTWIST,
	};
	
	// -----------------------------------------------------------------
	//		関数		
	// -----------------------------------------------------------------
	//!	基準値に設定
	virtual void	adjust();
	
	//!	外部からの更新予約
	void	setOrderPos( const TGL::Vec3& v0 )	{ mOrderPos = v0;	orFLG(mBGCamF, BGCAMF_ORDERPOS); }
	void	setOrderTar( const TGL::Vec3& v0 )	{ mOrderTar = v0;	orFLG(mBGCamF, BGCAMF_ORDERTAR); }
	void	setOrderFov( f32 f0 )			{ mOrderFov = f0;	orFLG(mBGCamF, BGCAMF_ORDERFOV); }
	void	setOrderTwist( f32 f0 )			{ mOrderTwist = f0;	orFLG(mBGCamF, BGCAMF_ORDERTWIST); }

protected:
	// -----------------------------------------------------------------
	//		定義		
	// -----------------------------------------------------------------

	// -----------------------------------------------------------------
	//		関数		
	// -----------------------------------------------------------------
	//!	派生クラスの更新
	virtual void	updateClass();
	//!	派生クラスの更新（フリービュー中も実行）
	virtual void	updateClass2();

	// -----------------------------------------------------------------
	//		変数		
	// -----------------------------------------------------------------
	u32		mBGCamF;		//!< フラグ
	TGL::Vec3		mOrderPos;		//!< 次回更新時反映用 座標
	TGL::Vec3		mOrderTar;		//!< 次回更新時反映用 注視点
	f32		mOrderFov;		//!< 次回更新時反映用 画角
	f32		mOrderTwist;	//!< 次回更新時反映用 ツイスト値
};

/* ========================================================================= */
/*		EOF																	 */
/* ========================================================================= */
