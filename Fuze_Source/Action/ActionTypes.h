// -------------------------------------------------------------------------- //
//! @file   ActionTypes.h
//! @brief  define定義
//! @author Nal
//! @since  11/11/10(木)
//!
//! COPYRIGHT (C) 2011 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
#ifndef	_ACTIONTYPES_H
#define	_ACTIONTYPES_H
/* ========================================================================= */
/*		定義 																 */
/* ========================================================================= */
//!	シーンステップ
#define	SCENESTEP_READY		(0)	//!< 開幕
#define	SCENESTEP_STAGE		(1)	//!< 道中
#define	SCENESTEP_MIDBOSS	(2)	//!< 中ボス
#define	SCENESTEP_BOSS		(3)	//!< ボス
#define	SCENESTEP_RESULT	(4)	//!< リザルト
#define	SCENESTEP_MAX		(5)

//!	シーンフラグ
#define	SCENEF_NOINPUT			(1<< 0)	//!< 入力不可
#define	SCENEF_NOLIMITSCREEN	(1<< 1)	//!< 画面内移動制限の無効
#define	SCENEF_NORENDER			(1<< 2)	//!< ほとんどの表示無効（スクリーンバッファ描画中など）
#define	SCENEF_NOGAUGE			(1<< 3)	//!< ゲージ非表示
#define	SCENEF_NOPLAYER			(1<< 4)	//!< プレイヤー不在モード
#define	SCENEF_POSFIX_X			(1<< 5)	//!< X座標制限
#define	SCENEF_POSFIX_Y			(1<< 6)	//!< Y座標制限
#define	SCENEF_POSFIX_Z			(1<< 7)	//!< Z座標制限
#define	SCENEF_TARGETLOCK		(1<< 8)	//!< ターゲットロック
#define	SCENEF_TL360			(1<< 9)	//!< 全方位ターゲットロック（ターゲットロック併用して使う）
#define	SCENEF_BEHIND			(1<<10)	//!< ビハインドモード
#define	SCENEF_ABSTALK			(1<<11)	//!< 強制的に会話を有効にする
#define	SCENEF_PAUSEBGM			(1<<12)	//!< ポーズ時BGMを一時停止する
#define	SCENEF_STAGECONNECT		(1<<13)	//!< ステージを跨いだ
#define	SCENEF_LOSE				(1<<14)	//!< 敗北
#define	SCENEF_NOHIST_TENSION	(1<<15)	//!< テンショングラフを記録しない
#define	SCENEFS_POSFIX			(SCENEF_POSFIX_X | SCENEF_POSFIX_Y | SCENEF_POSFIX_Z)	//!< 座標制限

//!	デモフラグ
#define	DEMOF_ACTIVE			(1<< 0)	//!< デモ中
#define	DEMOF_TIMESTOP			(1<< 1)	//!< ステージ経過を停止
#define	DEMOF_INPUTENABLE		(1<< 2)	//!< 操作可
#define	DEMOF_LIMITMOVESCREEN	(1<< 3)	//!< 画面内移動制限
#define	DEMOF_GAUGEDISP			(1<< 4)	//!< ゲージ表示
#define	DEMOF_RESULT			(1<< 5)	//!< リザルト表示中
#define	DEMOF_06				(1<< 6)	//!< 
#define	DEMOF_07				(1<< 7)	//!< 
#define	DEMOF_08				(1<< 8)	//!< 
#define	DEMOF_09				(1<< 9)	//!< 

//!	デモ同期フラグ
#define	EVENTF_00			(1<< 0)	//!< 
#define	EVENTF_01			(1<< 1)	//!< 
#define	EVENTF_02			(1<< 2)	//!< 
#define	EVENTF_03			(1<< 3)	//!< 
#define	EVENTF_04			(1<< 4)	//!< 
#define	EVENTF_05			(1<< 5)	//!< 
#define	EVENTF_06			(1<< 6)	//!< 
#define	EVENTF_07			(1<< 7)	//!< 
#define	EVENTF_08			(1<< 8)	//!< 
#define	EVENTF_09			(1<< 9)	//!< 
#define	EVENTF_10			(1<<10)	//!< 
#define	EVENTF_11			(1<<11)	//!< 
#define	EVENTF_12			(1<<12)	//!< 
#define	EVENTF_13			(1<<13)	//!< 
#define	EVENTF_14			(1<<14)	//!< 
#define	EVENTF_15			(1<<15)	//!< 
#define	EVENTF_16			(1<<16)	//!< 
#define	EVENTF_17			(1<<17)	//!< 
#define	EVENTF_18			(1<<18)	//!< 
#define	EVENTF_19			(1<<19)	//!< 
#define	EVENTF_20			(1<<20)	//!< 
#define	EVENTF_21			(1<<21)	//!< 
#define	EVENTF_22			(1<<22)	//!< 
#define	EVENTF_23			(1<<23)	//!< 
#define	EVENTF_24			(1<<24)	//!< 
#define	EVENTF_25			(1<<25)	//!< 
#define	EVENTF_26			(1<<26)	//!< 
#define	EVENTF_27			(1<<27)	//!< 
#define	EVENTF_28			(1<<28)	//!< 
#define	EVENTF_29			(1<<29)	//!< 
#define	EVENTF_30			(1<<30)	//!< 
#define	EVENTF_31			(1<<31)	//!< 

//!	デモスロウ
#define	DEMOSLOWF_CAMERA	(1<< 0)	//!< カメラ
#define	DEMOSLOWF_EFFECT	(1<< 1)	//!< エフェクト
#define	DEMOSLOWFS_ALL		(DEMOSLOWF_CAMERA | DEMOSLOWF_EFFECT)

//!	スクロールタイプ
#define	SCROLL_VERTICAL		(0)	//!< 縦
#define	SCROLL_SIDE			(1)	//!< 横
#define	SCROLL_FRONT		(2)	//!< 正面
#define	SCROLL_MAX			(3)

//!	ゲームからの移行 [ACtrl.mSubStep]
#define	ACTRL_GAME_STEP_NONE			(0)	//!< 無し
#define	ACTRL_GAME_STEP_GAMEOVER		(1)	//!< ゲームオーバー
#define	ACTRL_GAME_STEP_RESULT			(2)	//!< リザルト
#define	ACTRL_GAME_STEP_RETRY			(3)	//!< リトライ
#define	ACTRL_GAME_STEP_TITLE			(4)	//!< タイトル
//!	ゲームオーバーからの移行 [ACtrl.mSubStep]
#define	ACTRL_GAMEOVER_STEP_NONE		(0)	//!< 無し
#define	ACTRL_GAMEOVER_STEP_RELEASE		(1)	//!< 解放
#define	ACTRL_GAMEOVER_STEP_CONTINUE	(2)	//!< コンティニュー
#define	ACTRL_GAMEOVER_STEP_RETRY		(3)	//!< リトライ
#define	ACTRL_GAMEOVER_STEP_TITLE		(4)	//!< タイトル


#endif	/*	_ACTIONTYPES_H	*/
//=============================================================================
// eof
//=============================================================================
