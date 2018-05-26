// -------------------------------------------------------------------------- //
//! @file   tBGStg05.h
//! @brief  背景 ５面
//! @author Nal
//! @since  13/08/30(金)
//! 
//! COPYRIGHT (C) 2013 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
#pragma once
/* ====< インクルード >====================================================== */
#include "../tBG.h"
#include "../../Class/cBGSceneChange.h"
#include "Effect/Lensflare/tLensFlareSF.h"
#include "../../Class/tDust.h"
#include "cBGDoorStg05.h"

/* ========================================================================= */
//!		tBGStg05															 */
/* ========================================================================= */
class tBGStg05 : public tBG
{
	friend class DMBGStg05;
public:
	tBGStg05();
	virtual	~tBGStg05();
	
public:
	/* ========================================================================= */
	//!		tBGStg05::DoorPiece													 */
	/* ========================================================================= */
	class DoorPiece
	{
	public:
		DoorPiece( tBG* pBg, const stl::string& strModel, const TGL::Vec3& vPos, const TGL::Vec3& vSpeed=TGL::Vec3::ZERO );
		virtual	~DoorPiece();

		//!	更新
		void	update();
		
		//!	描画リスト実行
		static void	drawlistExec( void *pData ){
			PIX_BEGIN_EVENT(IRGBA(0xff, 0,0,0xff), "Render BG DoorPiece");
			PERF_BEGIN("tBGStg05::DoorPiece::draw",IRGBA( 200,0,0,128 ));
			((tBGStg05::DoorPiece*)pData)->draw();
			PERF_END("tBGStg05::DoorPiece::draw");
			PIX_END_EVENT();
		}
		//!	描画
		void	draw() {
			mpModel->Render( TGL::nModel::PASS_COLOR, false );
		}

		tBG*				mpOwner;	//!< オーナー
		bool				mbEnd;		//!< 終了判定
		f32					mTime;		//!< 経過時間
		TGL::Model*			mpModel;	//!< モデル
		TGL::Vec3			mPos;		//!< 座標
		TGL::Vec3			mRot;		//!< 回転
		TGL::Vec3			mMove;		//!< 移動量
		TGL::Vec3			mRoll;		//!< 回転量
		TGL::SoundChannel*	mpSE;		//!< 飛来音
	};

	// -----------------------------------------------------------------
	//!		定義		
	// -----------------------------------------------------------------
	enum {
		//!	エフェクトリンク
		EFTLINK_SMOKE = 0,	//!< ボスエリア煙
		EFTLINK_SMOKE2,		//!< ボスエリア煙
		EFTLINK_CLOUD,		//!< 雲
		EFTLINK_MAX,
		
		//!	ドア
		DOOR_HALL = 0,	//!< ホール
		DOOR_EXIT,		//!< 通路出口
	};

	// -----------------------------------------------------------------
	//!		関数		
	// -----------------------------------------------------------------
	//!	リセット
	virtual void	reset();
	//!	エフェクト開始
	virtual void	beginEffect();
	
	//!	更新 塵パーティクル
	void		setupDustPar();
	static void	setupDustParCB( tBG* pBG )	{ ((tBGStg05*)(pBG))->setupDustPar(); }
	void		finalDustPar();
	static void	finalDustParCB( tBG* pBG )	{ ((tBGStg05*)(pBG))->finalDustPar(); }
	void		updateDustPar( bool bSceneChg );
	static void	updateDustParCB( tBG* pBG, bool bSceneChg )	{ ((tBGStg05*)(pBG))->updateDustPar( bSceneChg ); }
	//!	更新 塵
	void		setupDust();
	static void	setupDustCB( tBG* pBG )	{ ((tBGStg05*)(pBG))->setupDust(); }
	void		finalDust();
	static void	finalDustCB( tBG* pBG )	{ ((tBGStg05*)(pBG))->finalDust(); }
	//!	更新 ４ボスエリア
	void		setup4Boss();
	static void	setup4BossCB( tBG* pBG )	{ ((tBGStg05*)(pBG))->setup4Boss(); }
	void		final4Boss();
	static void	final4BossCB( tBG* pBG )	{ ((tBGStg05*)(pBG))->final4Boss(); }
	//!	更新 通路光源
	void		setupRoadLight();
	static void	setupRoadLightCB( tBG* pBG )	{ ((tBGStg05*)(pBG))->setupRoadLight(); }
	void		updateRoadLight( bool bSceneChg );
	static void	updateRoadLightCB( tBG* pBG, bool bSceneChg )	{ ((tBGStg05*)(pBG))->updateRoadLight( bSceneChg ); }
	//!	更新 通路出口
	void		setupRoadExit();
	static void	setupRoadExitCB( tBG* pBG )	{ ((tBGStg05*)(pBG))->setupRoadExit(); }
	void		finalRoadExit();
	static void	finalRoadExitCB( tBG* pBG )	{ ((tBGStg05*)(pBG))->finalRoadExit(); }
	void		updateRoadExit( bool bSceneChg );
	static void	updateRoadExitCB( tBG* pBG, bool bSceneChg )	{ ((tBGStg05*)(pBG))->updateRoadExit( bSceneChg ); }
	//!	更新 空
	void		setupSky();
	static void	setupSkyCB( tBG* pBG )	{ ((tBGStg05*)(pBG))->setupSky(); }
	void		finalSky();
	static void	finalSkyCB( tBG* pBG )	{ ((tBGStg05*)(pBG))->finalSky(); }
	void		updateSky( bool bSceneChg );
	static void	updateSkyCB( tBG* pBG, bool bSceneChg )	{ ((tBGStg05*)(pBG))->updateSky( bSceneChg ); }

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

	//!	更新
	virtual void	update();
	//!	カメラ更新後の処理
	virtual void	updateCameraAfter();
	//!	スクリプトメッセージ受け取り
	virtual void	receiveMessage( s32 nId, s32 nParam );

	//!	原点位置の取得
	virtual TGL::Vec3	getOrigin() const;

	//!	デバッグ情報表示
	virtual void	dispDebugInfo();

	// -----------------------------------------------------------------
	//!		変数		
	// -----------------------------------------------------------------
	BGSceneChange	mSceneChange;			//!< シーン遷移
	TGL::Vec2		mCityFall_Uv;			//!< 都市滝 UV速度
	f32				mSkyPillarCullDist;		//!< 空 柱 カリング距離
	tLensFlareSF*	mpLensflare;			//!< レンズフレア
	TGL::TransFloat	mRoadLightOfsBg;		//!< 通路光源位置 背景
	u16				mRoadLightOfsBgSpeed;	//!< 通路光源速度 背景
	TGL::TransFloat	mRoadLightOfsChr;		//!< 通路光源位置 キャラ
	u16				mRoadLightOfsChrSpeed;	//!< 通路光源速度 キャラ
	SLLINK_T		mEftLink[EFTLINK_MAX];	//!< エフェクトリンク
	TGL::Matrix		mEftCloudMtx;			//!< 雲パーティクル追従マトリクス
	stl::vector<BGDoorStg05*>	mDoor;		//!< ドア
	stl::vector<DoorPiece*>	mDoorPiece;		//!< ドア破片
	SLLINK_T		mEftDustLink;			//!< 塵エフェクトリンク
	TGL::Matrix		mEftDustMtx;			//!< 塵パーティクル追従マトリクス
	tDust*			mpDust;					//!< 塵
	RGBA32			mDustColorMin;			//!< 塵色
	RGBA32			mDustColorMax;			//!< 塵色
	TGL::SoundChannel*	mpSEFire;			//!< SE 火事
	TGL::SoundChannel*	mpSEQuake;			//!< SE 地鳴り
	bool			mbWindVolDown;			//!< 風音量下げ
};
