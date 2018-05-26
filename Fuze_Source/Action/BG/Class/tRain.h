// -------------------------------------------------------------------------- //
//! @file   tRain.h
//! @brief  
//! @author Nal
//! @since  13/06/26(水)
//!
//! COPYRIGHT (C) 2013 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
#pragma	once
/* ====< インクルード >====================================================== */
#include "../Core/tBG.h"
#include "Action/GUI/tGaussScreenGUI.h"

/* ========================================================================= */
//		tRain																 */
/* ========================================================================= */
class tRain
{
public:
	tRain( tBG* pParent, u8 uPri );
	virtual ~tRain();

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
		void	update( const tRain& parent, const TGL::Vec3& vRight, f32 fUp );
		
		//		変数		
		f32			mLife;		//!< 寿命
		TGL::Vec3	mSpd;		//!< 速度
		TGL::Vec3	mPosOld;	//!< 前回の座標
		TGL::Vec3	mPos;		//!< 座標
		RGBA32		mColor;		//!< 色
	};

public:	
	// -----------------------------------------------------------------
	//		定義		
	// -----------------------------------------------------------------
	enum {
		LINE = 768,	//!< ライン数
	};

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
	const TGL::Vec3&	getSpd() const			{ return mSpd; }
	void		setSpd( const TGL::Vec3& v0 )	{ mSpd = v0; }
	f32			getAlpha() const		{ return mAlpha; }
	void		setAlpha( f32 f0 )		{ mAlpha = f0; }

protected:
	// -----------------------------------------------------------------
	//		関数		
	// -----------------------------------------------------------------
	//!	タスク更新
	static s32	updateTask( tRain* pTsk, u32 uMsg );
	//!	更新
	virtual void	update();

	//!	描画リスト実行
	static void	drawlistExec( void* pData )	{ ((tRain*)pData)->draw(); }
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
	TGL::Vec3		mSpd;			//!< 雨速度
	f32				mAlpha;			//!< アルファ
	Prim*			mpPrim;			//!< プリミティブ
	u16				mPrimNum;		//!< プリミティブ数
	GLTSKHEAD_T		mTskHead;		//!< タスクヘッダ
	TGL::VertexBuffer*	mpVBuf;		//!< 頂点バッファ
};

/* ========================================================================= */
/*		EOF																	 */
/* ========================================================================= */
