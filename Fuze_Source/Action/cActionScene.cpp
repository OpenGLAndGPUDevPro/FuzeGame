// -------------------------------------------------------------------------- //
//! @file   cActionScene.cpp
//! @brief  シーン制御
//! @author Nal
//! @since  11/11/10(木)
//!
//! COPYRIGHT (C) 2011 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
/* ====< インクルード >====================================================== */
#include "prefix.h"
#include <TGL.h>
#include <Sound/EWSoundInclude.inc>
#include "Game/Game.h"
#include "Action/ActionTypes.h"
#include "Effect/sAppEffect.h"
#include "Score/sScore.h"
#include "SubScene/sSubScene.h"

/* ========================================================================= */
/*		cActionScene														 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  コンストラクタ "cActionScene"
// ---------------------------------------------
cActionScene::cActionScene()
:	mActiveFrame(0), 
	mpTgsScene(NULL), mScenePath(stl::nullstr()), 
	mpTgsResult(NULL), mResultPath(stl::nullstr()), 
	mpTgsGameover(NULL), mGameoverPath(stl::nullstr())
{
	reset();
}

// ---------------------------------------------
//! @brief  デストラクタ "cActionScene"
// ---------------------------------------------
cActionScene::~cActionScene()
{
	freeResource();	//!< リソース解放
}

// ---------------------------------------------
//! @brief  初期化
//! @param  uMode	[in]	RESET_????
// ---------------------------------------------
void cActionScene::reset( u8 uMode/*=RESET_ALL*/ )
{
	//!	スクリプト解放
	freeSceneScript();		//!< シーン
	freeGameoverScript();	//!< ゲームオーバー
	if( uMode == RESET_ALL )
		freeResultScript();	//!< リザルト

	//!	変数初期化 完全
	if( uMode == RESET_ALL ){
		mActiveFrame	= 0;
		mStage			= mStageArea = 0;
		mStageOrder		= 0;
	}
	//!	変数初期化 ステージ移行以外
	if( uMode != RESET_STAGE ){
		mStep		= SCENESTEP_STAGE;
		mPosFix		= TGL::Vec3::ZERO;
		mScrollType	= 0;
		mSceneF		= 0;
		mDemoF		= 0;
		mDemoTime	= 0;
		mEventF		= 0xFFFFFFFF;
		mEventWorkI	= 0;
		mListenerOfs.clear( TGL::Vec3::ZERO );
	}
	//!	変数初期化
	mTotalTime		= 0;
	mStageTime		= 0;
	mBossTime		= 0;
	mBossTimeLimit	= 0;
	mContinueArea	= 0;
	mbContinueBgm	= false;
	//!	スキップ無効
	ISubScene->setSkipEnable( false );
}

// ---------------------------------------------
//! @brief  リソース解放
// ---------------------------------------------
void cActionScene::freeResource()
{
	//!	スクリプト
	freeSceneScript();		//!< シーン
	freeResultScript();		//!< リザルト
	freeGameoverScript();	//!< ゲームオーバー
}

// ---------------------------------------------
//! @brief  スクリプト解放
//! @param  pTgs	[i/o]	スクリプト
//! @param  Path	[i/o]	スクリプトパス
// ---------------------------------------------
void cActionScene::freeScript( TGSTSK_T** pTgs, stl::string& Path )
{
	//!	スクリプトタスク削除
	if( *pTgs ){
		GLTSK_Remove( &(*pTgs)->mTskHead );	//!< タスク切断
		TGS_Free( &(*pTgs)->mTgs );
		SAFE_DELETE( *pTgs );
		*pTgs = NULL;
	}
	//!	スクリプト解放
	if( !Path.empty() ){
		IFileManager->freeDev( Path.c_str() );
		Path = stl::nullstr();
	}
}

// ---------------------------------------------
//! @brief  シーンスクリプト読み込み＆開始
// ---------------------------------------------
void cActionScene::beginSceneScript()
{
	//!	解放
	freeSceneScript();
	//!	読み込み
	const char*	SCRIPT_PATH[STG_MAX] = {
		"script/scene/C81/scn_C81.tgs",		//!< C81
		"script/scene/Prologue/scn_Prologue.tgs",	//!< プロローグ
		"script/scene/Stage1/scn_Stage1.tgs",	//!< １面
		"script/scene/Stage2/scn_Stage2.tgs",	//!< ２面
		"script/scene/Stage3/scn_Stage3.tgs",	//!< ３面
		"script/scene/Stage4/scn_Stage4.tgs",	//!< ４面
		"script/scene/Stage5/scn_Stage5.tgs",	//!< ５面
		"script/scene/Stage6/scn_Stage6.tgs",	//!< ６面
		"script/scene/Tutorial/scn_Tutorial.tgs",	//!< チュートリアル
		"script/scene/Test/scn_Test.tgs",		//!< テスト
		"script/scene/Test2/scn_Test2.tgs",	//!< テスト
	};
	mScenePath = SCRIPT_PATH[mStage];
	IFileManager->readDev( mScenePath.c_str(), TGL::FileManager::TYPE_IMMEDIATE );
	mpTgsScene = TGS_ReqLoadNext( mScenePath.c_str(), GTOT_CTRL(0), &mpTgsScene );
}

// ---------------------------------------------
//! @brief  リザルトスクリプト読み込み＆開始
// ---------------------------------------------
void cActionScene::beginResultScript()
{
	//!	解放
	freeResultScript();
	//!	読み込み
	const char*	SCRIPT_PATH[STG_MAX] = {
		"-",											//!< C81
		"-",											//!< プロローグ
		"script/scene/Stage1/result_Stage1.tgs",	//!< １面
		"script/scene/Stage2/result_Stage2.tgs",	//!< ２面
		"script/scene/Stage3/result_Stage3.tgs",	//!< ３面
		"script/scene/Stage4/result_Stage4.tgs",	//!< ４面
		"script/scene/Stage5/result_Stage5.tgs",	//!< ５面
		"script/scene/Stage6/result_Stage6.tgs",	//!< ６面
		"-",											//!< チュートリアル
		"-",											//!< テスト
		"-",											//!< テスト
	};
	if( *SCRIPT_PATH[mStage] == '-' )	return;
	
	mResultPath = SCRIPT_PATH[mStage];
	IFileManager->readDev( mResultPath.c_str(), TGL::FileManager::TYPE_IMMEDIATE );
	mpTgsResult = TGS_ReqLoadNext( mResultPath.c_str(), GTOT_CTRL(0), &mpTgsResult );
}

// ---------------------------------------------
//! @brief  ゲームオーバースクリプト読み込み＆開始
// ---------------------------------------------
void cActionScene::beginGameoverScript()
{
	//!	解放
	freeGameoverScript();
	//!	読み込み
	mGameoverPath = "script/scene/GameOver/scn_GameOver.tgs";
	IFileManager->readDev( mGameoverPath.c_str(), TGL::FileManager::TYPE_IMMEDIATE );
	mpTgsGameover = TGS_ReqLoadNext( mGameoverPath.c_str(), GTOT_CTRL(0), &mpTgsGameover );
}

// ---------------------------------------------
//! @brief  更新
// ---------------------------------------------
void cActionScene::update()
{
	//!	フィニッシュ監視
	if( !IACtrl->getTime().isFinished() ){
		if( isLose() ){	//!< 敗北監視
			orSceneF( SCENEF_LOSE );
			IACtrl->getTime().reqFinishSlow();
			IAppEffect->req( EFTNO_GAMEOVERFLASH );
		}
	}
	
	f32	fDelta = IACtrl->getTime().getRateStd();
	mTotalTime += fDelta;

	//!	プレイ時間経過
	IPlayData->addTime();
	
	//!	ステージ経過
	if( !tstDemoF(DEMOF_TIMESTOP) ){
		mStageTime += fDelta;
		IScore->getScore()->addTime( fDelta );
	}
	
	//!	ボス戦経過
	if( mStep == SCENESTEP_MIDBOSS || mStep == SCENESTEP_BOSS )
		mBossTime += fDelta;
	
	//!	デモ経過
	if( isDemo() )
		mDemoTime += fDelta;

	//!	ゲームオーバー監視
	if( IACtrl->getStep() == ACtrl::STEP_GAME && isLose() )
		IACtrl->setSubStep( ACTRL_GAME_STEP_GAMEOVER );

	//!	サウンドリスナー更新
	mListenerOfs.update( IACtrl->getTime().getRateStd() );
	ITGLSound->getField()->getListener(0)->setOffset( mListenerOfs.getValue() );
	
	//!	アクティブ経過
	mActiveFrame++;
}

// ---------------------------------------------
//! @brief  デモ開始
//! @param  uDemoF	[in]	DEMOF_????
// ---------------------------------------------
void cActionScene::startDemo( u32 uDemoF )
{
	mDemoF		= uDemoF | DEMOF_ACTIVE;
	mEventF		= 0xFFFFFFFF;
	mEventWorkI	= 0;
	mDemoTime	= 0;
}

// ---------------------------------------------
//! @brief  デモ終了
// ---------------------------------------------
void cActionScene::endDemo()
{
	clrDemoF( DEMOF_ACTIVE );
	//mEventF = 0;
}

// ---------------------------------------------
//! @brief  敗北判定
//! @return true:敗北
// ---------------------------------------------
bool cActionScene::isLose()
{
	//!	プレイヤー死亡
	tCharacter*	pPlayer = ICharacter->getPlayer();
	if( pPlayer ){
		if( pPlayer->isDead() )	return true;
	}
	return false;
}

// ---------------------------------------------
//! @brief  デモ中かどうか
//! @return true:デモ中, false:デモ中ではない
// ---------------------------------------------
bool cActionScene::isDemo() const
{
	return (tstFLG(mDemoF, DEMOF_ACTIVE) != false);
}

// ---------------------------------------------
//! @brief  現在ステージをACT名で取得
//! @return 文字列
// ---------------------------------------------
const char* cActionScene::getStageAct()
{
	const char* _NAME[STG_MAX] = {
		"Before",
		"Prologue",
		"１",
		"２",
		"３",
		"４",
		"５",
		"６",
		"Tutorial",
		"Test",
		"Test2",
	};
	return _NAME[mStage];
}

/* ========================================================================= */
/*		EOF																	 */
/* ========================================================================= */
