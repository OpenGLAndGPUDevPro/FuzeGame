// -------------------------------------------------------------------------- //
//! @file   tDigitalLine.h
//! @brief  デジタルライン
//! @author Nal
//! @since  12/10/09(火)
//!
//! COPYRIGHT (C) 2012 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
#pragma	once
/* ====< インクルード >====================================================== */
#include "../Core/tBG.h"
#include "Action/GUI/tGaussScreenGUI.h"

/* ========================================================================= */
//		tDigitalLine														 */
/* ========================================================================= */
class tDigitalLine
{
public:
	tDigitalLine( tBG* pParent, u8 uPri, bool bGauss=false );
	virtual ~tDigitalLine();

public:	
	// -----------------------------------------------------------------
	//		定義		
	// -----------------------------------------------------------------
	enum {
		LINE = 20,	//!< ライン数
		VNUM = 8,	//!< ライン頂点数
		
		//!	軸数
		AXIS_X = 0,
		AXIS_Y,
		AXIS_Z,
		AXIS_MAX,
	};

	// -----------------------------------------------------------------
	//		関数		
	// -----------------------------------------------------------------
	
	//		アクセサ		
	bool	isEnable() const		{ return mbEnable; }
	void	setEnable( bool b0 )	{ mbEnable = b0; }
	f32		getAlpha() const		{ return mAlpha; }
	void	setAlpha( f32 f0 )		{ mAlpha = f0; }

protected:
	// -----------------------------------------------------------------
	//		関数		
	// -----------------------------------------------------------------
	//!	タスク更新
	static s32	updateTask( tDigitalLine* pTsk, u32 uMsg );
	//!	更新
	virtual void	update();

	//!	描画リスト実行
	static void	drawlistExec( void* pData )	{ ((tDigitalLine*)pData)->draw(); }
	//!	描画
	virtual void	draw();
	
	// -----------------------------------------------------------------
	//		変数		
	// -----------------------------------------------------------------
	tBG*			mpParent;			//!< 親
	bool			mbEnable;			//!< 有効
	f32				mAlpha;				//!< アルファ
	u8				mPri;				//!< 描画優先
	GLTSKHEAD_T		mTskHead;			//!< タスクヘッダ
	TGL::VertexBuffer*	mpVBuf;			//!< 頂点バッファ
	f32				mPosCell[LINE];		//!< 係数 セルサイズ
	TGL::Vec3		mSizeOfs[3][VNUM];	//!< 係数 
	GaussScreenGUI*	mpGUI;				//!< GUI
};

/* ========================================================================= */
/*		EOF																	 */
/* ========================================================================= */
