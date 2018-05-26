// -------------------------------------------------------------------------- //
//! @file   tBGStg02.h
//! @brief  背景 ２面
//! @author Nal
//! @since  12/07/06(金)
//! 
//! COPYRIGHT (C) 2012 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
#pragma once
/* ====< インクルード >====================================================== */
#include "../tBG.h"
#include "../../Class/cModelSetSort.h"

/* ========================================================================= */
//!		tBGStg02															 */
/* ========================================================================= */
class tBGStg02 : public tBG
{
	friend class DMBGStg02;
public:
	tBGStg02();
	virtual	~tBGStg02();
	
public:
	// -----------------------------------------------------------------
	//!		関数		
	// -----------------------------------------------------------------
	//!	リセット
	virtual void	reset();
	//!	エフェクト開始
	virtual void	beginEffect();
	
	//!	原点位置の取得
	virtual TGL::Vec3	getOrigin() const { return TGL::Vec3(0, 0, 310000); }

	//	アクセサ
	
protected:
	// -----------------------------------------------------------------
	//!		定義		
	// -----------------------------------------------------------------

	// -----------------------------------------------------------------
	//!		関数		
	// -----------------------------------------------------------------
	//!	リソースロード
	virtual bool	load( u8 uBG );
	//!	UVアニメ設定
	void	setModelUvAnim();
	//!	ドライブループSE再生
	void	playDriveLoopSE();

	//!	更新
	virtual void	update();
	//!	カメラ更新後の処理
	virtual void	updateCameraAfter();
	//!	更新 ドライブ抜けた
	void		updateDriveOut( bool bSceneChg );
	static void	updateDriveOutCB( tBG* pBG, bool bSceneChg )	{ ((tBGStg02*)(pBG))->updateDriveOut( bSceneChg ); }
	//!	更新 復帰ドライブ視認
	void		updateDriveWatch( bool bSceneChg );
	static void	updateDriveWatchCB( tBG* pBG, bool bSceneChg )	{ ((tBGStg02*)(pBG))->updateDriveWatch( bSceneChg ); }
	//!	更新 ドライブ再突入
	void		updateDriveIn( bool bSceneChg );
	static void	updateDriveInCB( tBG* pBG, bool bSceneChg )		{ ((tBGStg02*)(pBG))->updateDriveIn( bSceneChg ); }

	//!	デバッグ情報表示
	virtual void	dispDebugInfo();

	// -----------------------------------------------------------------
	//!		変数		
	// -----------------------------------------------------------------
	TGL::Vec2	mFar01_Uv;		//!< 遠景01 星UV速度
	TGL::Vec2	mFar02_UvA;		//!< 遠景02A UV速度
	TGL::Vec2	mFar02_UvB;		//!< 遠景02B UV速度
	TGL::Vec2	mLoop01_UvA;	//!< ループ01A UV速度
	TGL::Vec2	mLoop01_UvB;	//!< ループ01B UV速度
	TGL::Vec2	mLoop01_UvC;	//!< ループ01C UV速度
	TGL::Vec2	mLoop01_UvD;	//!< ループ01D UV速度
	TGL::Vec2	m02_UvA;		//!< 02A UV速度
	TGL::Vec2	m02_UvB;		//!< 02B UV速度
	TGL::Vec2	m02_UvC;		//!< 02C UV速度
	TGL::Vec2	m02_UvD;		//!< 02D UV速度
	TGL::Vec2	m04_UvA;		//!< 04A UV速度
	TGL::Vec2	m04_UvB;		//!< 04B UV速度
	TGL::Vec2	m04_UvC;		//!< 04C UV速度
	TGL::Vec2	m04_UvD;		//!< 04D UV速度
	TGL::Vec2	m04_UvA2;		//!< 04A2 UV速度
	TGL::Vec2	m04_UvB2;		//!< 04B2 UV速度
	TGL::Vec2	m04_UvC2;		//!< 04C2 UV速度
	TGL::Vec2	m04_UvD2;		//!< 04D2 UV速度
	TGL::Vec2	m04_UvFar;		//!< 04Far UV速度
	TGL::Vec2	m04_UvFar2;		//!< 04Far2 UV速度
	u16		mExEnvSize;		//!< 特殊環境マップサイズ
	cModelSetSort	mSortMeteor;	//!< 隕石ソート
};
