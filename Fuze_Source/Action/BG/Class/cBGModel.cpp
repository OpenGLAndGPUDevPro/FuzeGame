// -------------------------------------------------------------------------- //
//! @file   cBGModel.cpp
//! @brief  背景モデル
//! @author Nal
//! @since  11/11/08(火)
//!
//! COPYRIGHT (C) 2011 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
/* ====< インクルード >====================================================== */
#include "prefix.h"
#include <TGL.h>
#include <Graphics/Model/cTGLModelData.h>
#include "cBGModel.h"

/* ========================================================================= */
/*		cBGModel															 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  コンストラクタ "cBGModel"
//! @param  pData	[in]	リソース
//! @param  uType	[in]	TYPE_????
//! @param  uFlag	[in]	FLAG_????
// ---------------------------------------------
cBGModel::cBGModel( const TGL::ModelData* pData, u8 uType/*=TYPE_NORMAL*/, u32 uFlag/*=0*/ )
:	super( pData ),
	mbActive(true), mType(uType), mFlag(uFlag), mOnScene(0xFFFFFFFF), mPos(TGL::Vec3::ZERO), mRot(TGL::Vec3::ZERO), mpAnim(NULL)
{
}

// ---------------------------------------------
//! @brief  デストラクタ "cBGModel"
// ---------------------------------------------
cBGModel::~cBGModel()
{
	SAFE_DELETE( mpAnim );
}

// ---------------------------------------------
//! @brief  モーション追加
//! @param  uMotId		[in]	登録するモーションID
//! @param  strMotName	[in]	モーションリソース名
//! @return true:成功, false:失敗
// ---------------------------------------------
bool cBGModel::addMotion( u16 uMotId, const stl::string& strMotName )
{
	//!	アニメーションコントローラに登録
	if( !mpAnim )	return false;
	mpAnim->AttachAnimationData( uMotId, IResourceManager->getResourceBinary<TGL::AnimationResource>(strMotName, TGL::Resource::TYPE_ANIMATION)->Get() );
	return true;
}

// ---------------------------------------------
//! @brief  更新
//! @param  fDelta	[in]	デルタタイム [rate]
// ---------------------------------------------
void cBGModel::update( f32 fDelta )
{
	TGL::BaseCamera*	pCam = ICamera->getCamera(0);
	
	if( !IsBindTexture() ) BindTexture();

	//!	モーション更新
	if( fDelta > FLT_EPSILON ){
		fDelta = fDelta / TGL::Timer::FPS_BASE;
		if( mpAnim )
			mpAnim->Update( fDelta );
		//!	マテリアル更新
		UpdateMaterial( fDelta );
	}
	
	//!	ワールドマトリクス生成
	TGL::Matrix	mtxWorld = TGL::Matrix::UNIT;

	if( !mRot.isZero() )	//!< 回転
		mtxWorld.rollYPR( mRot );
		
	switch( mType ){
		// -------------------------------------
		case TYPE_SKYDOME:	//!< 天球用処理（カメラに座標追従）	
		{	TGL::Vec3	vPos = pCam->getPos();
			mtxWorld.setW( TGL::Vec3(vPos.x, 0, vPos.z) );
		}	break;

		// -------------------------------------
		case TYPE_SKYDOME_Y:	//!< 天球用処理（カメラに座標完全追従）	
			mtxWorld.setW( pCam->getPos() );
			break;

		// -------------------------------------
		case TYPE_LOOP_Z:	//!< Z方向ループ	
		{	const f32	_SIZE = getModelData()->GetBoundingVolume().getSize().z;
			//!	カメラ相対の座標
			s32	nOfs = -1;
			TGL::Vec3	vOfs = pCam->getTar();
			if( vOfs.z > getPos().z+DELTA ){	//!< 指定座標から先でループ
				nOfs = 0;
				vOfs.z += _SIZE * 0.5f - getPos().z;
				while( FABS(vOfs.z) > _SIZE+DELTA ){	//!< Z
					if( vOfs.z > 0.0f )	vOfs.z -= _SIZE;
					else				vOfs.z += _SIZE;
					nOfs++;
				}
			}else
				vOfs.z -= getPos().z;
			vOfs = pCam->getTar() - vOfs;
			
			//!	始点
			vOfs.x += getPos().x;
			vOfs.y += getPos().y;
			switch( nOfs ){
				case -1:	vOfs.z += _SIZE * 0.5f;	break;
				case 0:		vOfs.z += _SIZE * 1.0f;	break;
			}
			
			mtxWorld.setW( vOfs );
		}	break;

		// -------------------------------------
		default:	//!< 通常	
			mtxWorld.setW( getPos() );
			break;
	}

	TGL::Model::Update( mtxWorld );
	
	//!	階層更新
	if( tstFlag(FLAG_UPDATE_HIERARCHY) && GetSkelton() )
		GetSkelton()->CalcHierarchy();
}

// ---------------------------------------------
//! @brief  更新 インスタンシング用
//! @param  matArray	[]	
// ---------------------------------------------
void cBGModel::Update( const TGL::Model::MODEL_MATRIX_ARRAY& matArray )
{
	update( 0 );
	TGL::Model::Update( matArray );
}

// ---------------------------------------------
//! @brief  描画
//! @param  mtxWorld	[in]	描画マトリクス
// ---------------------------------------------
void cBGModel::Render( const TGL::Matrix& mtxWorld, TGL::nModel::RENDER_PASS iPass, bool renderAlphaModel )
{
PIX_BEGIN_EVENT(IRGBA(0xff, 0,0,0xff), "cBGModel::Render");

	super::Render( mtxWorld, iPass, renderAlphaModel );
PIX_END_EVENT();

	//!	ループモデル 再描画
	if( mType == TYPE_LOOP_Z ){
PIX_BEGIN_EVENT(IRGBA(0xff, 0,0,0xff), "cBGModel::Render::LoopZ");
		//!	描画用マトリクス
		TGL::Matrix	mtx = mtxWorld;
		mtx._43 += getModelData()->GetBoundingVolume().getSize().z - 1.5f;
		//!	描画
		super::Render( mtx, iPass );
PIX_END_EVENT();
	}
}

/* ========================================================================= */
/*		EOF																	 */
/* ========================================================================= */
