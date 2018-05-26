// -------------------------------------------------------------------------- //
//! @file   tBGStg06.cpp
//! @brief  背景 ６面
//! @author Nal
//! @since  13/08/30(金)
//! 
//! COPYRIGHT (C) 2013 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
/* ====< インクルード >====================================================== */
#include "prefix.h"
#include <TGL.h>
#include "Game/Game.h"
#include "Action/Camera/cBGCamera.h"
#include "tBGStg06.h"
#include "Action/ActionTypes.h"
#include "Effect/sAppEffect.h"
#include "Sound/sAppSound.h"
#include "../../../../../tgs/scene/Stage6/scn_Stage6.h"

#ifdef _WINDOWS
#pragma warning(disable : 4355)
#endif

static const char*	_PARAM_EX = "param/BG/Stage6/Stage6_paramEx.json";	//!< 固有パラメータリソース
/* ========================================================================= */
//!		tBGStg06															 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  コンストラクタ "tBGStg06"
// ---------------------------------------------
tBGStg06::tBGStg06()
:	tBG(),
	mFar01_Uv(TGL::Vec2(0,0)),
	mLoop01_UvA(TGL::Vec2(0,0)), mLoop01_UvB(TGL::Vec2(0,0)), mLoop01_UvC(TGL::Vec2(0,0)), mLoop01_UvD(TGL::Vec2(0,0)),
	mDriveObj_UvA(TGL::Vec2(0,0)), mDriveObj_UvB(TGL::Vec2(0,0)), mDriveObj_Roll(0),
	mMother_Uv_Glow(TGL::Vec2(0,0)), mMother_Uv_Glow2(TGL::Vec2(0,0)), mMother_Uv_Cloud(TGL::Vec2(0,0)), mMother_Uv_Hole(TGL::Vec2(0,0)), mMother_Uv_String(TGL::Vec2(0,0)), mMother_Uv_String2(TGL::Vec2(0,0)),
	mpDust(NULL), mpBlade(NULL), mpLensflare(NULL)
{
	SLLINK_Init( &mEftDustLink );
	reset();
}

// ---------------------------------------------
//! @brief  デストラクタ "tBGStg06"
// ---------------------------------------------
tBGStg06::~tBGStg06()
{
	IAppResource->releaseTexPool( "models/BG/common/St06/St06_Flare01.dds" );
	IAppResource->releaseTexPool( "models/BG/common/St06/St06_Flare02.dds" );
	IAppResource->releaseTexPool( "models/BG/common/St06/St06_Flare03.dds" );
	IAppResource->releaseTexPool( "models/BG/common/St06/St06_Flare04.dds" );
	IAppResource->releaseTexPool( "models/BG/common/St06/St06_Post_ToneCurve_nmip.dds" );
	IAppResource->releaseTexPool( "models/BG/common/St06/St06_Post_ToneCurve_Mother_nmip.dds" );
	IAppResource->releaseTexPool( "models/BG/common/St06/St06_Post_ToneCurve_Space_nmip.dds" );
	IAppResource->releaseTexPool( "models/BG/common/St06R/St06_Drive_envC.dds" );
	ITGLSound->stop( 0, TGL::Sound::STOPFS_ONLY_AMBIENT | TGL::Sound::STOPF_N_ONMEMORY );
	ITGLRenderer->getFilterToneCurve()->setTexCurve( NULL );
	ITGLRenderer->getFilterToneCurve()->setEnableValue( false );
	EEMIT_End( &mEftDustLink );
	SAFE_DELETE( mpDust );
	SAFE_DELETE( mpBlade );
	SAFE_DELETE( mpLensflare );
}

// ---------------------------------------------
//! @brief  リセット
// ---------------------------------------------
void tBGStg06::reset()
{
	tBG::reset();
	ITGLSound->stop( 0, TGL::Sound::STOPFS_ONLY_AMBIENT | TGL::Sound::STOPF_N_ONMEMORY );
	EEMIT_End( &mEftDustLink );
	mEftDustMtx	= TGL::Matrix::UNIT;
	mLensflareAlpha.clear( 1 );

	cBGModel*	pModel = getModel( "St06_Far_02.mdl" );
	if( pModel )
		pModel->setRot( TGL::Vec3(0, 0, 0) );
}

// ---------------------------------------------
//! @brief  リソースロード
//! @param  uBG	[in]	BGID_????
//! @return true:成功, false:失敗
// ---------------------------------------------
bool tBGStg06::load( u8 uBG )
{
	//!		テクスチャ		
	IAppResource->loadTexPool( "models/BG/common/St06/St06_Flare01.dds" );
	IAppResource->loadTexPool( "models/BG/common/St06/St06_Flare02.dds" );
	IAppResource->loadTexPool( "models/BG/common/St06/St06_Flare03.dds" );
	IAppResource->loadTexPool( "models/BG/common/St06/St06_Flare04.dds" );
	IAppResource->loadTexPool( "models/BG/common/St06/St06_Post_ToneCurve_nmip.dds" );
	IAppResource->loadTexPool( "models/BG/common/St06/St06_Post_ToneCurve_Mother_nmip.dds" );
	IAppResource->loadTexPool( "models/BG/common/St06/St06_Post_ToneCurve_Space_nmip.dds" );
	IAppResource->loadTexPool( "models/BG/common/St06R/St06_Drive_envC.dds" );

	//!		モデル		
	cBGModel*	pModel;
	const u32	_SCENE_DRIVE	= (1<<S6SCENE_DRIVE);	//!< ドライブ
	const u32	_SCENE_MOTHER	= (1<<S6SCENE_MOTHER);	//!< 母星
	const u32	_SCENE_DIAGRA	= (1<<S6SCENE_DIAGRA);	//!< ディアグラ戦
	const u32	_SCENE_SPACE	= (1<<S6SCENE_SPACE);	//!< 宇宙
	//!	遠景 ドライブ
	pModel = addModel( "St06_Far_01.mdl", cBGModel::TYPE_SKYDOME, cBGModel::FLAG_FAROBJ );
	if( pModel )	pModel->setOnScene( _SCENE_DRIVE );
	pModel = addModel( "Obj_St06_EFF.mdl", cBGModel::TYPE_SKYDOME, cBGModel::FLAG_FAROBJ );
	if( pModel )	pModel->setOnScene( _SCENE_DRIVE );
	//!	遠景 母星
	pModel = addModel( "St06_Far_02.mdl", cBGModel::TYPE_SKYDOME_Y, cBGModel::FLAG_FAROBJ );
	if( pModel )	pModel->setOnScene( _SCENE_MOTHER|_SCENE_DIAGRA );
	pModel = addModel( "Obj_St06_Far_02.mdl", cBGModel::TYPE_SKYDOME_Y, cBGModel::FLAG_FAROBJ );
	if( pModel ){
		pModel->setOnScene( _SCENE_MOTHER|_SCENE_DIAGRA );
		//!	モーション
		TGL::AnimationController*	pAnim = APP_NEW TGL::AnimationController( pModel );
		pModel->setAnim( pAnim );
		pModel->addMotion( 0, "Obj_St06_Far_02.anm" );
		pModel->getAnim()->Play( 0, true, true );
	}
	//!	ドライブループ
	pModel = addModel( "St06_01_Loop.mdl", cBGModel::TYPE_SKYDOME, cBGModel::FLAG_FAROBJ );
	if( pModel )	pModel->setOnScene( _SCENE_DRIVE );
	//!	ドライブ終端
	pModel = addModel( "St06_02_Connect.mdl" );
	if( pModel )	pModel->setOnScene( _SCENE_MOTHER );
	//!	遠景 宇宙
	pModel = addModel( "St06_Far_Space.mdl", cBGModel::TYPE_SKYDOME, cBGModel::FLAG_FAROBJ );
	if( pModel ){
		pModel->setOnScene( _SCENE_SPACE );
		SAFE_DELETE( mpLensflare );
		mpLensflare = createLensFlare( TGL::MtxGuide(pModel->GetWorldMatrix(), TGL::Vec3(-45490,11477,-45307)), "St06_Flare", _PARAM_EX, "Lensflare_Space" );
	}
	//!	宇宙 艦隊
	pModel = addModel( "St06_Ship.mdl" );
	if( pModel )	pModel->setOnScene( _SCENE_SPACE );

	//!		シーンパラメータ		
	mSceneParam.getFromJson( "param/BG/Stage6/Stage6_param.json" );
	{	//!	固有パラメータ
		TGL::JsonUtil	json( _PARAM_EX );
		TGL::Json::array	jsonArray = json.getArray("-");
		foreach( TGL::Json::array, jsonArray, it ){
			TGL::Json::object&	work = it->get<TGL::Json::object>();
			mFar01_Uv		= TGL::Vec2( (f32)atof( work["Far01_Speed_U"].to_str().c_str() ),
									(f32)atof( work["Far01_Speed_V"].to_str().c_str() ) );
			mLoop01_UvA		= TGL::Vec2( (f32)atof( work["Loop01_A_Speed_U"].to_str().c_str() ),
									(f32)atof( work["Loop01_A_Speed_V"].to_str().c_str() ) );
			mLoop01_UvB		= TGL::Vec2( (f32)atof( work["Loop01_B_Speed_U"].to_str().c_str() ),
									(f32)atof( work["Loop01_B_Speed_V"].to_str().c_str() ) );
			mLoop01_UvC		= TGL::Vec2( (f32)atof( work["Loop01_C_Speed_U"].to_str().c_str() ),
									(f32)atof( work["Loop01_C_Speed_V"].to_str().c_str() ) );
			mLoop01_UvD		= TGL::Vec2( (f32)atof( work["Loop01_D_Speed_U"].to_str().c_str() ),
									(f32)atof( work["Loop01_D_Speed_V"].to_str().c_str() ) );
			mDriveObj_UvA	= TGL::Vec2( (f32)atof( work["DriveObj_A_Speed_U"].to_str().c_str() ),
									(f32)atof( work["DriveObj_A_Speed_V"].to_str().c_str() ) );
			mDriveObj_UvB	= TGL::Vec2( (f32)atof( work["DriveObj_B_Speed_U"].to_str().c_str() ),
									(f32)atof( work["DriveObj_B_Speed_V"].to_str().c_str() ) );
			mDriveObj_Roll	= (f32)atof( work["DriveObj_Roll"].to_str().c_str() );

			mMother_Uv_Glow		= TGL::Vec2( (f32)atof( work["Mother_Glow_Speed_U"].to_str().c_str() ),
										(f32)atof( work["Mother_Glow_Speed_V"].to_str().c_str() ) );
			mMother_Uv_Glow2	= TGL::Vec2( (f32)atof( work["Mother_Glow2_Speed_U"].to_str().c_str() ),
										(f32)atof( work["Mother_Glow2_Speed_V"].to_str().c_str() ) );
			mMother_Uv_Cloud	= TGL::Vec2( (f32)atof( work["Mother_Cloud_Speed_U"].to_str().c_str() ),
										(f32)atof( work["Mother_Cloud_Speed_V"].to_str().c_str() ) );
			mMother_Uv_Hole		= TGL::Vec2( (f32)atof( work["Mother_Hole_Speed_U"].to_str().c_str() ),
										(f32)atof( work["Mother_Hole_Speed_V"].to_str().c_str() ) );
			mMother_Uv_String	= TGL::Vec2( (f32)atof( work["Mother_String_Speed_U"].to_str().c_str() ),
										(f32)atof( work["Mother_String_Speed_V"].to_str().c_str() ) );
			mMother_Uv_String2	= TGL::Vec2( (f32)atof( work["Mother_String2_Speed_U"].to_str().c_str() ),
										(f32)atof( work["Mother_String2_Speed_V"].to_str().c_str() ) );
			
			//!	遠景モーション速度
			pModel = getModel( "Obj_St06_Far_02.mdl" );
			if( pModel && pModel->getAnim() )
				pModel->getAnim()->SetAnimationSpeed( (f32)atof(work["Far02Obj_MotionSpeed"].to_str().c_str()) );
			
			//!	宇宙 カット２
			mSpaceCut2Rot	= TGL::Vec3( (f32)atof( work["Space_Cut2_Sky_RotX"].to_str().c_str() ),
										 (f32)atof( work["Space_Cut2_Sky_RotY"].to_str().c_str() ),
										 (f32)atof( work["Space_Cut2_Sky_RotZ"].to_str().c_str() ) );
			mSpaceCut2Fov	= (f32)atof( work["Space_Cut2_Sky_Fov"].to_str().c_str() );
		}
	}
	
	//!		パラメータ		
	//!	モデル配置
	applyModelSetFromJson( "param/BG/Stage6/Stage6_modelset.json" );

	//!	カメラパス追加
	applyCameraPathSetFromJson( "param/BG/Stage6/Stage6_camera.json" );

	//!	UVアニメ設定
	setModelUvAnim();

	//!	トーンカーブ
	ITGLRenderer->getFilterToneCurve()->setEnableValue( true );
	ITGLRenderer->getFilterToneCurve()->setTexCurve( IAppResource->getTexture("St06_Post_ToneCurve_nmip") );

	//!	シーン環境マップ
	mSceneParam.setEnvTex( "St06_Drive_envC.dds" );
	ICharacter->overwriteBGEnvTex();	//!< キャラクターへの適用
	overwriteEnvTex();	//!< 背景モデルへの適用

	//!	更新関数登録
	addUpdateEvent( updateDriveCB,		S6SCENE_DRIVE,	S6SCENE_DRIVE, setupDriveCB, finalDriveCB );	//!< ドライブ
	addUpdateEvent( updateDriveEndCB,	S6SCENE_MOTHER,	S6SCENE_MOTHER, setupDriveEndCB );	//!< ドライブ抜ける
	addUpdateEvent( updateDustParCB,	S6SCENE_MOTHER, S6SCENE_DIAGRA, setupDustParCB, finalDustParCB );	//!< 塵パーティクル
	addUpdateEvent( NULL,				S6SCENE_MOTHER, S6SCENE_DIAGRA, setupDustCB, finalDustCB );	//!< 塵

	return true;
}

// ---------------------------------------------
//! @brief  ドライブ 終了
// ---------------------------------------------
void tBGStg06::setupDrive()
{
	//!	ドライブループSE
	TGL::SoundChannel*	pCh = IAppSound->play( SDID_STREAM, AMBSTR_DRIVE );
	if( pCh ){
		pCh->setVolume( 0.01f, 0.5f, 60 );
		pCh->setSPan( -1.0f );
	}
}
// ---------------------------------------------
//! @brief  ドライブ 終了
// ---------------------------------------------
void tBGStg06::finalDrive()
{
	//!	ドライブループSE停止
	ITGLSound->stop( 60, TGL::Sound::STOPFS_ONLY_AMBIENT );
}
// ---------------------------------------------
//! @brief  ドライブ
//! @param  bSceneChg	[in]	true:シーン変更瞬間
// ---------------------------------------------
void tBGStg06::updateDrive( bool bSceneChg )
{
	cBGModel*	pModel = getModel( "Obj_St06_EFF.mdl" );
	if( pModel ){
		f32	fRotZ = TGL::Math::Loop<f32>( pModel->getRot().z, mDriveObj_Roll, 0.0f, 360.0f );
		pModel->setRot( TGL::Vec3(0, 0, fRotZ) );
	}
}

// ---------------------------------------------
//! @brief  ドライブ抜ける 初期化
// ---------------------------------------------
void tBGStg06::setupDriveEnd()
{
	mDriveEndPos.init( TGL::TransValue::MODE_NORMAL, 120, TGL::TransValue::CURVE_ADD1, 0, 0.0f, -60000.0f );
}
// ---------------------------------------------
//! @brief  ドライブ抜ける
//! @param  bSceneChg	[in]	true:シーン変更瞬間
// ---------------------------------------------
void tBGStg06::updateDriveEnd( bool bSceneChg )
{
	cModelSet*	pModel = getModelSet( "St06_02_Connect.mdl" );
	if( pModel ){
		pModel->setPosOfs( TGL::Vec3(0, 0, mDriveEndPos.getValue()) );

		mDriveEndPos.update( getDeltaTime() );
		pModel->setEnable( mDriveEndPos.isActive() );
	}
}

// ---------------------------------------------
//! @brief  UVアニメ設定
// ---------------------------------------------
void tBGStg06::setModelUvAnim()
{
	cBGModel*	pModel;
	
	//!	ドライブ
	pModel = getModel( "St06_Far_01.mdl" );
	if( pModel )	pModel->SetNodeUVAnime( "St06_01_Star_nofog", mFar01_Uv );
	//!	ドライブ
	pModel = getModel( "St06_01_Loop.mdl" );
	if( pModel ){
		pModel->SetNodeUVAnime( "SLSD_A_add", mLoop01_UvA );
		pModel->SetNodeUVAnime( "SLSD_B_add", mLoop01_UvB );
		pModel->SetNodeUVAnime( "SLSD_C_add", mLoop01_UvC );
		pModel->SetNodeUVAnime( "SLSD_D_add", mLoop01_UvD );
	}
	//!	ドライブ
	pModel = getModel( "St06_02_Connect.mdl" );
	if( pModel ){
		pModel->SetNodeUVAnime( "SLSD_A_add_alpha_nofog", mLoop01_UvA );
		pModel->SetNodeUVAnime( "SLSD_B_add_alpha_nofog", mLoop01_UvB );
		pModel->SetNodeUVAnime( "SLSD_C_add_alpha_nofog", mLoop01_UvC );
		pModel->SetNodeUVAnime( "SLSD_D_add_alpha_nofog", mLoop01_UvD );
	}
	//!	ドライブObj
	pModel = getModel( "Obj_St06_EFF.mdl" );
	if( pModel ){
		pModel->SetNodeUVAnime( "PlasmaBK_add", mDriveObj_UvA );
		pModel->SetNodeUVAnime( "PlasmaBK2_add", mDriveObj_UvA );
		pModel->SetNodeUVAnime( "Plasma_add", mDriveObj_UvB );
		pModel->SetNodeUVAnime( "Plasma2_add", mDriveObj_UvB );
	}
	//!	母星
	pModel = getModel( "St06_Far_02.mdl" );
	if( pModel ){
		pModel->SetNodeUVAnime( "MPBaseGlow_add", mMother_Uv_Glow );
		pModel->SetNodeUVAnime( "MPBaseGlow2_add", mMother_Uv_Glow2 );
		pModel->SetNodeUVAnime( "MPBase_Cloud_add", mMother_Uv_Cloud );
		pModel->SetNodeUVAnime( "MPBase_Hole_Glow2_add", mMother_Uv_Hole );
		pModel->SetNodeUVAnime( "String_Glow_add", mMother_Uv_String );
		pModel->SetNodeUVAnime( "MPBase_String_Glow_add", mMother_Uv_String2 );
	}
}

// ---------------------------------------------
//! @brief  塵パーティクル 初期化
// ---------------------------------------------
void tBGStg06::setupDustPar()
{
	//!	パーティクル
	if( !ITGLSystem->isGraphicsLevelLow() && !ITGLSystem->isPostFilterLevelLow() ){
		TGL::MtxGuide	guide( &mEftDustMtx );
		IAppEffect->req( EFTNO_SCENES6_DUST, TGL::Vec3::ZERO, TGL::Vec3::ZERO, NULL, &guide, NULL, NULL, 0, 0, &mEftDustLink );
	}
	//!	SE
	TGL::SoundChannel*	pCh = IAppSound->play( SDID_STREAM, AMBSTR_S6_MOTHER );
	if( pCh ){
		pCh->setVolume( 0.01f, 0.8f, 60 );
		pCh->setUserId( SNDOWNER_AMBIENT );
	}
}
// ---------------------------------------------
//! @brief  塵パーティクル 終了
// ---------------------------------------------
void tBGStg06::finalDustPar()
{
	EEMIT_End( &mEftDustLink );
	//!	SE停止
	ITGLSound->stop( 60, TGL::Sound::STOPFS_ONLY_AMBIENT );
}
// ---------------------------------------------
//! @brief  塵パーティクル
//! @param  bSceneChg	[in]	true:シーン変更瞬間
// ---------------------------------------------
void tBGStg06::updateDustPar( bool bSceneChg )
{
	//!	パーティクル追従マトリクス更新
	TGL::BaseCamera*	pCam = ICamera->getCamera(0);
	//!	回転
	TGL::Vec3	v0 = -pCam->getMove();
	if( !v0.isZero() ){
		//!	Y : ツイスト回転済みカメラUP
		TGL::Matrix	m0 = TGL::Matrix::UNIT;
		m0.setX( pCam->getRightVector() );
		m0.setY( pCam->getUpVector() );
		m0.setZ( pCam->getEyeVector() );
		m0.rollVec( pCam->getEyeVector(), -pCam->getTwist() );
		mEftDustMtx.setY( m0.getY() );

		//!	Z : カメラ移動逆方向
		v0 = v0.normalize();
		mEftDustMtx.setZ( v0 );
		
		//!	X : ZY垂線
		v0 = mEftDustMtx.getZ().cross( mEftDustMtx.getY() );
		mEftDustMtx.setX( v0 );
	}
	
	//!	座標 : カメラ位置から、カメラ移動量を足す
	v0 = pCam->getPos();
	v0 += pCam->getMove() * 120.0f;
	v0 += pCam->getEyeVector() * 3000.0f;
	mEftDustMtx.setW( v0 );
	
#if 0
	IPrim->add( TGL::PrimCross(mEftDustMtx.getW(), 1000.f, IRGBA(255,255,0,255)), 0, TGL::Camera::CAMF_00 );
	
	TGL::Matrix	m0 = mEftDustMtx;
	m0.setW( pCam->getTar() );
	IPrim->add( TGL::PrimPivot(m0, 1000.0f), 0, TGL::Camera::CAMF_00 );
#endif
}

// ---------------------------------------------
//! @brief  塵 初期化
// ---------------------------------------------
void tBGStg06::setupDust()
{
	if( mpDust )	return;
	if( !ITGLSystem->isGraphicsLevelLow() && !ITGLSystem->isPostFilterLevelLow() ){
		mpDust = APP_NEW tDust( this, GPRI_EFTPRE(0) );
		mpDust->setColorMin( IRGBA( 96, 96, 96,255) );
		mpDust->setColorMax( IRGBA(255,255,255,255) );
		mpDust->setFogColor( IRGBA(0,0,0,255) );
		mpDust->setFogNear(	500 );
		mpDust->setFogFar(	3000 );
		mpDust->setSpd( 0.3f );
	}
}
// ---------------------------------------------
//! @brief  塵 終了
// ---------------------------------------------
void tBGStg06::finalDust()
{
	SAFE_DELETE( mpDust );
}

// ---------------------------------------------
//! @brief  更新
// ---------------------------------------------
void tBGStg06::update()
{
	//!	トーンカーブ
	if( mScene != mSceneOld ){
		if( mScene >= S6SCENE_SPACE )
			ITGLRenderer->getFilterToneCurve()->setTexCurve( IAppResource->getTexture("St06_Post_ToneCurve_Space_nmip") );
		else if( mScene >= S6SCENE_MOTHER )
			ITGLRenderer->getFilterToneCurve()->setTexCurve( IAppResource->getTexture("St06_Post_ToneCurve_Mother_nmip") );
		else
			ITGLRenderer->getFilterToneCurve()->setTexCurve( IAppResource->getTexture("St06_Post_ToneCurve_nmip") );
	}

	//!	親クラス
	tBG::update();
	
	//!	剣破壊
	if( mpBlade ){
		if( mpBlade->mbEnd )
			SAFE_DELETE( mpBlade );
		else
			mpBlade->update();
	}

	//!	レンズフレア
	if( mpLensflare ){
		mLensflareAlpha.update( getDeltaTime() );
		mpLensflare->setEnable( (mScene == S6SCENE_SPACE) );
		mpLensflare->setAlpha( mLensflareAlpha.getValue() );
	}
}

// ---------------------------------------------
//! @brief  カメラ更新後の処理
// ---------------------------------------------
void tBGStg06::updateCameraAfter()
{
	tBG::updateCameraAfter();
}

// ---------------------------------------------
//! @brief  スクリプトメッセージ受け取り
//! @param  nId		[in]	メッセージID
//! @param  nParam	[in]	パラメータ
// ---------------------------------------------
void tBGStg06::receiveMessage( s32 nId, s32 nParam )
{
	cBGModel*	pModel;
	switch( nId ){
		case S6PRMSG_MOTHERROLL:	//!	天球回す
			pModel = getModel( "St06_Far_02.mdl" );
			if( pModel )
				pModel->setRot( TGL::Vec3(180, 0, 0) );
			break;

		case S6PRMSG_BLADEBREAK:	//!	剣破壊
			SAFE_DELETE( mpBlade );
			mpBlade = APP_NEW BladeBreak( this );
			break;

		case S6PRMSG_SPACEROLL:	//!	天球回す
			pModel = getModel( "St06_Far_Space.mdl" );
			if( pModel )
				pModel->setRot( TGL::Vec3(0, (f32)nParam, 0) );
			break;

		case S6PRMSG_SPACESTAR_VISIBLE:	//!	移動惑星表示
			pModel = getModel( "St06_Far_Space.mdl" );
			if( pModel ){
				pModel->SetNodeVisible( "MvPlanetGlow_add", nParam != false );
				pModel->SetNodeVisible( "MvPlanet_nofog_alpha", nParam != false );
				pModel->SetNodeVisible( "EFF_Cloud_add", nParam != false );
				pModel->SetNodeVisible( "EFF_Cloud2_add", nParam != false );
			}
			break;

		case S6PRMSG_SPACEDRIVE_VISIBLE:	//!	ドライブ表示
			pModel = getModel( "St06_Far_Space.mdl" );
			if( pModel ){
				pModel->SetNodeVisible( "SLSD_add", nParam != false );
				pModel->SetNodeVisible( "SLSD2_add", nParam != false );
			}
			break;

		case S6PRMSG_LENSFLARE_ON:	//!	レンズフレア出す
			mLensflareAlpha.init( TGL::TransValue::MODE_NORMAL, (f32)nParam, TGL::TransValue::CURVE_LINEAR, 0, 1.0f );
			break;

		case S6PRMSG_LENSFLARE_OFF:	//!	レンズフレア消す
			mLensflareAlpha.init( TGL::TransValue::MODE_NORMAL, (f32)nParam, TGL::TransValue::CURVE_LINEAR, 0, 0.0f );
			break;

		case S6PRMSG_SPACE_CUT2_ON:	//!	カット2
			pModel = getModel( "St06_Far_Space.mdl" );
			if( pModel )
				pModel->setRot( mSpaceCut2Rot );
			mSkyFov = mSpaceCut2Fov;
			break;

		case S6PRMSG_SPACE_CUT2_OFF:	//!	カット2
			pModel = getModel( "St06_Far_Space.mdl" );
			if( pModel )
				pModel->setRot( TGL::Vec3::ZERO );
			mSkyFov = 0;
			break;
	}
}

// ---------------------------------------------
//! @brief  コンストラクタ "BladeBreak"
//! @param  strModel	[in]	モデル名
// ---------------------------------------------
tBGStg06::BladeBreak::BladeBreak( tBG* pBg )
:	mpOwner(pBg), mbEnd(false), mTime(0)
{
	//!	モデル読み込み
	mpModel = APP_NEW TGL::Model( IResourceManager->getResourceBinary<TGL::ModelResource>("Obj_St06_BladeBreak.mdl", TGL::Resource::TYPE_MODEL)->Get() );
	mpModel->SetLightBit( (1<<0) );
	mpModel->SetFogId( cSceneParam::Data::FOG_BG );
	//!	アニメーションコントローラに登録
	mpAnim = APP_NEW TGL::AnimationController( mpModel );
	mpAnim->AttachAnimationData( 0, IResourceManager->getResourceBinary<TGL::AnimationResource>("Obj_St06_BladeBreak.anm", TGL::Resource::TYPE_ANIMATION)->Get() );
	//!	再生
	mpAnim->Play( 0, false, true );
	mpAnim->SetAnimationSpeed( 0.75f );
	
	//!	位置
	tCharacter*	pPlayer = ICharacter->getPlayer();
	if( pPlayer ){
		mMtx = pPlayer->getBoneWorldMatrix( "R_hand_buki2" );
		//!	位置
		TGL::Vec3	v0;
		v0 = TGL::Vec3(6,70,0).apply( mMtx );
		mMtx.setW( v0 );
		//!	回転
		mMtx.rollVec( mMtx.getX(), -90.0f );
		mMtx.rollVec( mMtx.getZ(), 180.0f );
	}
}

// ---------------------------------------------
//! @brief  デストラクタ "BladeBreak"
// ---------------------------------------------
tBGStg06::BladeBreak::~BladeBreak()
{
	SAFE_DELETE( mpModel );
	SAFE_DELETE( mpAnim );
}

// ---------------------------------------------
//! @brief  更新
// ---------------------------------------------
void tBGStg06::BladeBreak::update()
{
	//!	モデル更新
	mpAnim->Update( mpOwner->getDeltaTime() / TGL::Timer::FPS_BASE );
	mpModel->Update( mMtx );
	
	//!	描画リスト登録
	if( !IACtrl->getScene().tstSceneF(SCENEF_NORENDER) ){
		ITGLRenderer->addDrawList( TGL::Renderer::DPASS_MAIN, GPRI_BG(3), mpOwner->getTskHead().mTot, TGL::Camera::CAMF_00, drawlistExec, (void*)this );
	}
	
	//!	終了
	mTime += mpOwner->getDeltaTime();
	if( mTime > 60.0f ){
		if( ICamera->getCamera(0)->isOutScreen(mMtx.getW(), 150.0f) )
			mbEnd = true;
	}
}

/* ========================================================================= */
/*		デバッグ															 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  デバッグ情報表示
// ---------------------------------------------
void tBGStg06::dispDebugInfo()
{
#ifndef _TGL_FINAL
	if( !mbDebugInfo )	return;
	
	//!	親
	tBG::dispDebugInfo();

	TGL::BaseCamera*	pCam = ICamera->getCamera(0);
	if( !pCam )	return;
	
#endif	/*	_TGL_FINAL	*/
}
