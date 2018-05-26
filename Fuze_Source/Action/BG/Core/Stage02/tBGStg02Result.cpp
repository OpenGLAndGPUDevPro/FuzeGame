// -------------------------------------------------------------------------- //
//! @file   tBGStg02Result.cpp
//! @brief  背景 ２面リザルト
//! @author Nal
//! @since  12/07/21(土)
//! 
//! COPYRIGHT (C) 2012 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
/* ====< インクルード >====================================================== */
#include "prefix.h"
#include <TGL.h>
#include "Game/Game.h"
#include "Action/Camera/cBGCamera.h"
#include "tBGStg02Result.h"
#include "Action/ActionTypes.h"

/* ========================================================================= */
//!		tBGStg02Result														 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  コンストラクタ "tBGStg02Result"
// ---------------------------------------------
tBGStg02Result::tBGStg02Result()
:	tBG()
{
	mSceneParam.setEnable( false );
	reset();
}

// ---------------------------------------------
//! @brief  デストラクタ "tBGStg02Result"
// ---------------------------------------------
tBGStg02Result::~tBGStg02Result()
{
}

// ---------------------------------------------
//! @brief  リセット
// ---------------------------------------------
void tBGStg02Result::reset()
{
	tBG::reset();
	mbAreaWait = false;
}

// ---------------------------------------------
//! @brief  リソースロード
//! @param  uBG	[in]	BGID_????
//! @return true:成功, false:失敗
// ---------------------------------------------
bool tBGStg02Result::load( u8 uBG )
{
	//!		モデル		
	cBGModel*	pModel;
	//!	0 : 
	pModel = addModel( "St02_05_Connect.mdl" );
	if( pModel )	pModel->setOnScene( cBGModel::SCENE_01 | cBGModel::SCENE_02 );
	
	//!		シーンパラメータ		
	//mSceneParam.getFromJson( "param/BG/stg02/stg02_param.json" );
	//mSceneParam.apply();
	
	//!		パラメータ		
	//!	モデル配置
	cModelSet*	pSet = APP_NEW cModelSet;
	pSet->setName( "St02_05_Connect.mdl" );
	pSet->setPos( TGL::Vec3(136, 109, 504083) );
	pSet->setRot( TGL::Vec3(-11, -7, 0) );
	mModelSet.push_back( pSet );
	
	//!	カメラパス追加
	//applyCameraPathSetFromJson( "param/BG/stg02/stg02_camera.json" );

	//!	UVアニメ設定
	setModelUvAnim();

	//!	更新関数登録
	addUpdateEvent( updateDriveCB,		1, 1 );		//!< ドライブ
	addUpdateEvent( updateDriveEndCB,	2, 2 );		//!< ドライブ終了

	return true;
}

// ---------------------------------------------
//! @brief  UVアニメ設定
// ---------------------------------------------
void tBGStg02Result::setModelUvAnim()
{
	cBGModel*	pModel;
	pModel = getModel( "St02_05_Connect.mdl" );
	if( pModel ){
		pModel->SetNodeUVAnime( "SLSD_A_add_alpha_nofog", TGL::Vec2(2.00f, -0.2f) );
		pModel->SetNodeUVAnime( "SLSD_B_add_alpha_nofog", TGL::Vec2(1.60f,  0.4f) );
		pModel->SetNodeUVAnime( "SLSD_C_add_alpha_nofog", TGL::Vec2(3.00f, -0.3f) );
		pModel->SetNodeUVAnime( "SLSD_D_add_alpha_nofog", TGL::Vec2(2.50f, -0.4f) );
	}
}

// ---------------------------------------------
//! @brief  エフェクト開始
// ---------------------------------------------
void tBGStg02Result::beginEffect()
{
}

// ---------------------------------------------
//! @brief  更新
// ---------------------------------------------
void tBGStg02Result::update()
{
	//!	親クラス
	tBG::update();
}

// ---------------------------------------------
//! @brief  更新 ドライブ
//! @param  bSceneChg	[in]	true:シーン変更瞬間
// ---------------------------------------------
void tBGStg02Result::updateDrive( bool bSceneChg )
{
	cModelSet*	pSet = getModelSet( "St02_05_Connect.mdl" );
	if( !pSet )	return;

//	if( bSceneChg ){
		//!	ドライブを視線方向に向ける
		TGL::Vec3	vEye = ICamera->getCamera(0)->getEyeVector();	//!< 視線
		TGL::Vec3	vRot = TGL::Vec3::ZERO;
		vRot.y = RAD2DEG( atan2f( -vEye.z, vEye.x ) ) + 90.0f;	//!< Y回転

		f32	fFront	= sqrtf((vEye.x * vEye.x) + (vEye.z * vEye.z));
		vRot.x = RAD2DEG( atan2f( fFront, vEye.y ) ) - 90.0f;	//!< X回転

		pSet->setRot( vRot );
//	}

	pSet->setPos( ICamera->getCamera(0)->getTar() );
}

// ---------------------------------------------
//! @brief  更新 ドライブ終了
//! @param  bSceneChg	[in]	true:シーン変更瞬間
// ---------------------------------------------
void tBGStg02Result::updateDriveEnd( bool bSceneChg )
{
	cModelSet*	pSet = getModelSet( "St02_05_Connect.mdl" );
	if( !pSet )	return;
	
	if( bSceneChg ){
		//!	ドライブ移動
		TGL::Vec3	vEye = ICamera->getCamera(0)->getEyeVector();	//!< 視線
		TGL::Vec3	vS = pSet->getPos();			//!< 始点
		TGL::Vec3	vE = vS - (vEye * 50000.0f);	//!< 終点
		mDrivePos.init( TGL::TransValue::MODE_NORMAL, 120.0f, TGL::TransValue::CURVE_LINEAR, 999, vS, vE );
	}

	//!	ドライブ座標
	mDrivePos.update( getDeltaTime() );
	pSet->setPos( mDrivePos.getValue() );
}

