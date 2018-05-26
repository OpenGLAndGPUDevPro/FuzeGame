// -------------------------------------------------------------------------- //
//! @file   tDust.h
//! @brief  塵
//! @author Nal
//! @since  13/11/19(火)
//!
//! COPYRIGHT (C) 2013 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
#pragma	once
/* ====< インクルード >====================================================== */
#include "../Core/tBG.h"
#include "Action/GUI/tGaussScreenGUI.h"

/* ========================================================================= */
//		tDust																 */
/* ========================================================================= */
class tDust
{
public:
	tDust( tBG* pParent, u8 uPri );
	virtual ~tDust();

	/* ========================================================================= */
	//		Prim																 */
	/* ========================================================================= */
	class Prim
	{
	public:
		Prim()
		:	mLife(0), mSpd(TGL::Vec3::ZERO), mPosOld(TGL::Vec3::ZERO), mPos(TGL::Vec3::ZERO), mColor(0)
		{}
		virtual ~Prim() {}
		
		//		関数		
		//!	更新
		void	update( const tDust& parent, u16 uIdx, const TGL::Vec3& vTar, const TGL::Vec3& vRight, const TGL::Vec3& vUp, const TGL::Vec3& vEye, f32 fRight, f32 fUp );
		//!	初期化判定
		bool	isInit( const tDust& parent, u16 uIdx ) const;
		
		//		変数		
		f32		mLife;		//!< 寿命
		TGL::Vec3		mSpd;		//!< 速度
		TGL::Vec3		mPosOld;	//!< 前回の座標
		TGL::Vec3		mPos;		//!< 座標
		RGBA32	mColor;		//!< 色
	};

public:	
	// -----------------------------------------------------------------
	//		定義		
	// -----------------------------------------------------------------

	// -----------------------------------------------------------------
	//		関数		
	// -----------------------------------------------------------------
	
	//		アクセサ		
	bool		isEnable() const		{ return mbEnable; }
	void		setEnable( bool b0 )	{ mbEnable = b0; }
	RGBA32		getColorMin() const		{ return mColorMin; }
	void		setColorMin( RGBA32 i0 ){ mColorMin = i0; }
	RGBA32		getColorMax() const		{ return mColorMax; }
	void		setColorMax( RGBA32 i0 ){ mColorMax = i0; }
	RGBA32		getFogColor() const		{ return mFogColor; }
	void		setFogColor( RGBA32 i0 ){ mFogColor = i0; }
	f32			getFogNear() const		{ return mFogNear; }
	void		setFogNear( f32 f0 )	{ mFogNear = f0; }
	f32			getFogFar() const		{ return mFogFar; }
	void		setFogFar( f32 f0 )		{ mFogFar = f0; }
	f32			getSpd() const			{ return mSpd; }
	void		setSpd( f32 f0 )		{ mSpd = f0; }
	f32			getCameraMove() const	{ return mCameraMove; }
	void		setCameraMove( f32 f0 )	{ mCameraMove = f0; }
	f32			getAlpha() const		{ return mAlpha; }
	void		setAlpha( f32 f0 )		{ mAlpha = f0; }

protected:
	// -----------------------------------------------------------------
	//		関数		
	// -----------------------------------------------------------------
	//!	タスク更新
	static s32	updateTask( tDust* pTsk, u32 uMsg );
	//!	更新
	virtual void	update();

	//!	描画リスト実行
	static void	drawlistExec( void* pData )	{ ((tDust*)pData)->draw(); }
	//!	描画
	virtual void	draw();
	
	// -----------------------------------------------------------------
	//		変数		
	// -----------------------------------------------------------------
	tBG*			mpParent;		//!< 親
	bool			mbEnable;		//!< 有効
	u8				mPri;			//!< 描画優先
	RGBA32			mColorMin;		//!< 基準色
	RGBA32			mColorMax;		//!< 基準色
	RGBA32			mFogColor;		//!< フォグ色
	f32				mFogNear;		//!< フォグ近
	f32				mFogFar;		//!< フォグ遠
	f32				mSpd;			//!< 速度
	f32				mCameraMove;	//!< カメラ移動影響
	f32				mAlpha;			//!< アルファ
	Prim*			mpPrim;			//!< プリミティブ
	u16				mPrimNum;		//!< プリミティブ数
	GLTSKHEAD_T		mTskHead;		//!< タスクヘッダ
	TGL::VertexBuffer*	mpVBuf;		//!< 頂点バッファ
};

/* ========================================================================= */
/*		EOF																	 */
/* ========================================================================= */
