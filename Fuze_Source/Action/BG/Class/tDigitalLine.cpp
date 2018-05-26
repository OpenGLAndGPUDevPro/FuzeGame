// -------------------------------------------------------------------------- //
//! @file   tDigitalLine.cpp
//! @brief  デジタルライン
//! @author Nal
//! @since  12/10/19(金)
//!
//! COPYRIGHT (C) 2012 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
/* ====< インクルード >====================================================== */
#include "prefix.h"
#include <TGL.h>
#include "sTGLMain.h"
#include "tDigitalLine.h"
#include "sTGLPause.h"
#include "Game/Game.h"
#include "Action/ActionTypes.h"
#include "Effect/sAppEffect.h"

//!	シェーダー定数バッファ
#include "CbSystem.h"
#include "BG/BG_DigitalLine.h"

namespace {
	//!	サイズ定義
	const f32	_SIZE = 100000.0f * 2.0f / (f32)tDigitalLine::LINE;
	const f32	_SIZEHALF = _SIZE / 2.0f;
	const f32	_SIZECELL = _SIZE / (f32)tDigitalLine::LINE;
	const f32	_SIZEVNUM = _SIZE / (f32)tDigitalLine::VNUM;

	const f32	_GRADE = 12.0f;	//!< ライングラデパラメータ
};
/* ========================================================================= */
/*		tDigitalLine														 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  コンストラクタ "tDigitalLine"
//! @param  pParent	[i/o]	親
//! @param  uPri	[in]	描画優先
//! @param  bGauss	[in]	true:ぼかし有効
// ---------------------------------------------
tDigitalLine::tDigitalLine( tBG* pParent, u8 uPri, bool bGauss/*=false*/ )
:	mpParent(pParent), mbEnable(true), mAlpha(1), mPri(uPri), mpGUI(NULL)
{
	//!	頂点バッファ確保
	const u16	_LINE_TOTAL = LINE * LINE * AXIS_MAX;
	const u16	_VTX		= _LINE_TOTAL * (VNUM+2);
	mpVBuf = APP_NEW TGL::VertexBuffer( _VTX, sizeof(TGL::Vertex::POS_COLOR_T), false );
	
	//!	ぼかし
	if( bGauss && !ITGLSystem->isGraphicsLevelLow() ){
		mpGUI = APP_NEW GaussScreenGUI;
		mpGUI->setBlend( RSBL_ADD );
		mpGUI->setPri( mPri-1 );
		mpGUI->setUV( 0, 0, ITGLSystem->getWidth(), ITGLSystem->getHeight() );
		mpGUI->getSclZ().clear( 8.0f );
		mpGUI->setDensity( 4.0f );
		mpGUI->orGuiF( GUIF_MAINPASS );
		mpGUI->setCamFlag( TGL::Camera::CAMF_00 );
		mpGUI->setRenderScreen( true );
	}
	
	//!		定数計算	
	//!	セルサイズ
	for( u8 i=0 ; i<LINE ; i++ ){
		mPosCell[i] = (f32)(-(_SIZEHALF-_SIZECELL/2.0f)+(_SIZECELL)*(f32)i);
	}
	//!	
	for( u8 i=0 ; i<VNUM ; i++ ){
		mSizeOfs[0][i] = TGL::Vec3( -_SIZEVNUM*(f32)i, 0, 0 );
		mSizeOfs[1][i] = TGL::Vec3( 0, 0, -_SIZEVNUM*(f32)i );
		mSizeOfs[2][i] = TGL::Vec3( 0, -_SIZEVNUM*(f32)i, 0 );
	}
	
	//!	タスク接続
	ZeroMemory( &mTskHead, sizeof(GLTSKHEAD_T) );
	GLTSK_SetNext( GLTSK_GetBase(GTOT_BG(1)), &mTskHead, this, (s32 (*)(void*,u32))tDigitalLine::updateTask );
}

// ---------------------------------------------
//! @brief  デストラクタ "tDigitalLine"
// ---------------------------------------------
tDigitalLine::~tDigitalLine()
{
	SAFE_DELETE( mpVBuf );

	//!	タスク切断
	GLTSK_Remove( &mTskHead );
	
	if( mpGUI )
		mpGUI->end();
}

// ---------------------------------------------
//! @brief  定期更新
// ---------------------------------------------
s32 tDigitalLine::updateTask( tDigitalLine* pTsk, u32 uMsg )
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
void tDigitalLine::update()
{
	if( !mbEnable )	return;
	
	//!	描画リスト登録
	if( !IACtrl->getScene().tstSceneF(SCENEF_NORENDER) )
		ITGLRenderer->addDrawList( TGL::Renderer::DPASS_MAIN, mPri, mTskHead.mTot, TGL::Camera::CAMF_00, tDigitalLine::drawlistExec, (void*)this );
	
	if( IMain->isPause() || IPause->isActive() )	return;
	if( IACtrl->getTime().isStop() )	return;
}

// ---------------------------------------------
//! @brief  描画
// ---------------------------------------------
void tDigitalLine::draw()
{
	if( !mbEnable )	return;

	mpParent->getPerfGpu().begin();

	TGL::BaseCamera*	pCam = ICamera->getRenderCamera();
	if( !pCam )	return;
	
	//!	シェーダー取得
	TGL::ShaderEffect*	pEffect = IAppResource->getShaderEffect( "BG_DigitalLine" );

	//!	デバイス設定
	IGraphics->setWorldTransform();
	IGraphics->getRS().setZ( TGL::RenderState::Z_TEST );
	IGraphics->getRS().setCull( TGL::RenderState::CULL_NONE );

	//!		ライン		
	IGraphics->getRS().setBlend( RSBL_ADD, true );

	//!	カメラ相対の中心座標
	s32	nOfsX=0, nOfsY=0, nOfsZ=0;
	TGL::Vec3	vOfs = pCam->getTar();
	while( FABS(vOfs.x) > _SIZECELL+DELTA ){	//!< X
		if( vOfs.x > 0.0f ){
			vOfs.x -= _SIZECELL;
			nOfsX++;
		}else{
			vOfs.x += _SIZECELL;
			nOfsX--;
		}
	}
	while( FABS(vOfs.y) > _SIZECELL+DELTA ){	//!< Y
		if( vOfs.y > 0.0f ){
			vOfs.y -= _SIZECELL;
			nOfsY++;
		}else{
			vOfs.y += _SIZECELL;
			nOfsY--;
		}
	}
	while( FABS(vOfs.z) > _SIZECELL+DELTA ){	//!< Z
		if( vOfs.z > 0.0f ){
			vOfs.z -= _SIZECELL;
			nOfsZ++;
		}else{
			vOfs.z += _SIZECELL;
			nOfsZ--;
		}
	}
	vOfs = pCam->getTar() - vOfs;

	//!		係数計算	
	//!	グラデーション係数
	const f32	GRADEBASE = mpParent->getTimePass() * 1.0f;
	const f32	GRADEBASE_X = GRADEBASE + (_GRADE*(f32)(-nOfsX));
	const f32	GRADEBASE_Y = GRADEBASE + (_GRADE*(f32)(-nOfsY));
	const f32	GRADEBASE_Z = GRADEBASE + (_GRADE*(f32)(-nOfsZ));
	
	f32	GRADE_VNUM_MUL_X[LINE], GRADE_VNUM_MUL_Y[LINE], GRADE_VNUM_MUL_Z[LINE];
	for( u8 i=0 ; i<LINE ; i++ ){
		const f32	GRADE_VNUM_MUL =_GRADE * (f32)VNUM;
		GRADE_VNUM_MUL_X[i] = GRADE_VNUM_MUL * (f32)(i+nOfsX);
		GRADE_VNUM_MUL_Y[i] = GRADE_VNUM_MUL * (f32)(i+nOfsY);
		GRADE_VNUM_MUL_Z[i] = GRADE_VNUM_MUL * (f32)(i+nOfsZ);
	}

	//!	アルファ
	f32	ALPHA_BASE[VNUM];
	for( u8 i=0 ; i<VNUM ; i++ ){
		if( i == 0 || i == VNUM-1 )	ALPHA_BASE[i] = 0;	//!< 端は0
		else						ALPHA_BASE[i] = mAlpha;
	}

	//!	頂点設定
	TGL::Vertex::POS_COLOR_T*	pV = (TGL::Vertex::POS_COLOR_T*)mpVBuf->lock();

	//!	定数バッファ
	TGL::cbSystem	cb;
	cb.gCameraToProjectionMatrix = ICamera->getDrawMtx();

	TGL::cbLocal	cbL;
	cbL.gPow		= 8.0f;
	cbL.gCameraPos	= pCam->getPos();
	cbL.gFogStart	= _SIZEHALF - _SIZECELL;
	cbL.gFogEnd		= (_SIZE + _SIZEHALF) * 0.5f;
#ifdef PS4
	pEffect->setConstantBuffer( "cbSystem", &cb, sizeof(cb) );
	pEffect->setConstantBuffer( "cbLocal", &cbL, sizeof(cbL) );
#else
	pEffect->setMatrix(	"gCameraToProjectionMatrix", cb.gCameraToProjectionMatrix );
	pEffect->setFloat(	"gPow",			cbL.gPow );
	pEffect->setVector(	"gCameraPos",	cbL.gCameraPos );
	pEffect->setFloat(	"gFogStart",	cbL.gFogStart );
	pEffect->setFloat(	"gFogEnd",		cbL.gFogEnd );
#endif

	//!		描画	
	u16 vnum = 0;
	for( u8 uAxis=0 ; uAxis<AXIS_MAX ; uAxis++ ){
		for( u8 u=0 ; u<LINE ; u++ ){
			for( u8 v=0 ; v<LINE ; v++ ){
				//!	基準座標・グラデ
				TGL::Vec3	vBase = vOfs;
				volatile f32	fGrade;
				switch( uAxis ){
					case AXIS_X:	//!< X軸
						vBase += TGL::Vec3( _SIZEHALF, mPosCell[u], mPosCell[v] );
						fGrade = GRADEBASE_X + GRADE_VNUM_MUL_Z[v] + GRADE_VNUM_MUL_Y[u];
						break;
					case AXIS_Y:	//!< Y軸
						vBase += TGL::Vec3( mPosCell[u], mPosCell[v], _SIZEHALF );
						fGrade = GRADEBASE_Z + GRADE_VNUM_MUL_Y[v] + GRADE_VNUM_MUL_X[u];
						break;
					case AXIS_Z:	//!< Z軸
						vBase += TGL::Vec3( mPosCell[u], _SIZEHALF, mPosCell[v] );
						fGrade = GRADEBASE_Y + GRADE_VNUM_MUL_Z[v] + GRADE_VNUM_MUL_X[u];
						break;
				}
				while( fGrade >= 512.0f )
					fGrade -= 512.0f;
				
				//!	繋ぎ
				if( vnum > 0 ){
					pV[vnum].mPos	= vBase + mSizeOfs[uAxis][0];
					pV[vnum].mColor	= 0;
					vnum++;
				}

				//!	頂点設定
				for( u8 i=0 ; i<VNUM ; i++ ){
					//!	座標
					pV[vnum].mPos = vBase + mSizeOfs[uAxis][i];
					
					//!	グラデーション計算
					f32	fGradeRate = fGrade / 256.0f;
					if( fGradeRate > 1.0f )
						fGradeRate = 1.0f - (fGradeRate - 1.0f);
					
					u8	i0 = (u8)(255.0f * fGradeRate);
					pV[vnum].mColor	= IRGBA( 16, i0, 255, i0*ALPHA_BASE[i] );
					vnum++;

					fGrade += _GRADE;
					while( fGrade >= 512.0f )
						fGrade -= 512.0f;
				}
				
				//!	繋ぎ
				pV[vnum].mPos	= pV[vnum-1].mPos;
				pV[vnum].mColor	= 0;
				vnum++;
			}
		}
	}
	mpVBuf->unlock();
	mpVBuf->setVtxNum( vnum );
	IVertex->setVertexLayout( TGL::Vertex::VTXDEC_COLOR, *mpVBuf, pEffect );

	pEffect->begin();
	IGraphics->draw( TGL::Graphics::PRIM_LINESTRIP );
//	IGraphics->drawIndexed( TGL::Graphics::PRIM_LINELIST );
	pEffect->end();

	mpParent->getPerfGpu().end();
}

/* ========================================================================= */
/*		EOF																	 */
/* ========================================================================= */
