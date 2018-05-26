// -------------------------------------------------------------------------- //
//! @file   tBGTest2.cpp
//! @brief  背景 テスト
//! @author Nal
//! @since  13/06/01(土)
//! 
//! COPYRIGHT (C) 2013 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
/* ====< インクルード >====================================================== */
#include "prefix.h"
#include <TGL.h>
#include "Game/Game.h"
#include "Action/Camera/cBGCamera.h"
#include "tBGTest2.h"
#include "Action/ActionTypes.h"

/* ========================================================================= */
//!		tBGTest2															 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  コンストラクタ "tBGTest2"
// ---------------------------------------------
tBGTest2::tBGTest2()
:	tBG(),
	mRiver07_Uv(TGL::Vec2(0,0))
{
	reset();
}

// ---------------------------------------------
//! @brief  デストラクタ "tBGTest2"
// ---------------------------------------------
tBGTest2::~tBGTest2()
{
	IAppResource->releaseTexPool( "models/BG/common/St01R/St01_envC.dds" );
}

// ---------------------------------------------
//! @brief  リセット
// ---------------------------------------------
void tBGTest2::reset()
{
	tBG::reset();
}

// ---------------------------------------------
//! @brief  リソースロード
//! @param  uBG	[in]	BGID_????
//! @return true:成功, false:失敗
// ---------------------------------------------
bool tBGTest2::load( u8 uBG )
{
	//!		モデル		
	cBGModel*	pModel;
	//!	遠景 
	pModel = addModel( "St01_Far_03.mdl", cBGModel::TYPE_SKYDOME, cBGModel::FLAG_FAROBJ );
	if( pModel )	pModel->setOnScene( cBGModel::SCENE_00 );
	//!	近景
	pModel = addModel( "St01_07_Loop.mdl" );
	if( pModel )	pModel->setOnScene( cBGModel::SCENE_00 );

	//!	テクスチャ
	IAppResource->loadTexPool( "models/BG/common/St01R/St01_envC.dds" );

	//!		シーンパラメータ		
	mSceneParam.getFromJson( "param/BG/Test2/Test2_param.json" );

	{	//!	固有パラメータ
		TGL::JsonUtil	json( "param/BG/Test2/Test2_paramEx.json" );
		TGL::Json::array	jsonArray = json.getArray("-");
		foreach( TGL::Json::array, jsonArray, it ){
			TGL::Json::object&	work = it->get<TGL::Json::object>();
			mRiver07_Uv		= TGL::Vec2( (f32)atof( work["River07_Speed_U"].to_str().c_str() ),
									(f32)atof( work["River07_Speed_V"].to_str().c_str() ) );
		}
	}
	
	//!		パラメータ		
	//!	モデル配置
	applyModelSetFromJson( "param/BG/Test2/Test2_modelset.json" );
	
	//!	カメラパス追加
	applyCameraPathSetFromJson( "param/BG/Test2/Test2_camera.json" );

	//!	シーン環境マップ
	mSceneParam.setEnvTex( "St01_envC.dds" );
	ICharacter->overwriteBGEnvTex();	//!< キャラクターへの適用
	overwriteEnvTex();	//!< 背景モデルへの適用

	//!	UVアニメ設定
	setModelUvAnim();

	//!	更新関数登録

	return true;
}

// ---------------------------------------------
//! @brief  UVアニメ設定
// ---------------------------------------------
void tBGTest2::setModelUvAnim()
{
	cBGModel*	pModel;
	
	pModel = getModel( "St01_07_Loop.mdl" );
	if( pModel )
		pModel->SetNodeUVAnime( "River_alpha", mRiver07_Uv );
}

// ---------------------------------------------
//! @brief  エフェクト開始
// ---------------------------------------------
void tBGTest2::beginEffect()
{
}

// ---------------------------------------------
//! @brief  更新
// ---------------------------------------------
void tBGTest2::update()
{
	//!	親クラス
	tBG::update();
}

/* ========================================================================= */
/*		デバッグ															 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  デバッグ情報表示
// ---------------------------------------------
void tBGTest2::dispDebugInfo()
{
#ifndef _TGL_FINAL
	if( !mbDebugInfo )	return;
	
	//!	親
	tBG::dispDebugInfo();
	
	TGL::BaseCamera*	pCam = ICamera->getCamera(0);
	if( !pCam )	return;
	
#endif	/*	_TGL_FINAL	*/
}
