// -------------------------------------------------------------------------- //
//! @file   tBGStg01.h
//! @brief  背景 １面
//! @author Nal
//! @since  12/01/16(月)
//! 
//! COPYRIGHT (C) 2012 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
#pragma once
/* ====< インクルード >====================================================== */
#include "../tBG.h"
#include "../../Class/cBGSceneChange.h"
#include "Effect/Lensflare/tLensFlareSF.h"

/* ========================================================================= */
//!		tBGStg01															 */
/* ========================================================================= */
class tBGStg01 : public tBG
{
	friend class DMBGStg01;
public:
	tBGStg01();
	virtual	~tBGStg01();
	
public:
	/* ========================================================================= */
	//!		tBGStg01::Cannon													 */
	/* ========================================================================= */
	class Cannon
	{
	public:
		Cannon( tBGStg01& owner, cBGModel* pModel, cModelSet* pSet )
		:	mOwner(owner), mpModel(pModel), mpSet(pSet),
			mShotAfter(0), mShotTime( RANDF2(SHOTTIME_MIN, SHOTTIME_MAX) ), mbRand(false) {}
		virtual	~Cannon() {}
		
		//!	発射タイミング
		const f32	SHOTTIME_MIN = 120.0f;
		const f32	SHOTTIME_MAX = 300.0f;

		//!	更新
		void	update( f32 fDelta );
		//!	発射
		void	shot( bool bFirst=false );
		
		tBGStg01&	mOwner;		//!< オーナー
		cBGModel*	mpModel;	//!< モデル
		cModelSet*	mpSet;		//!< モデルセット
		f32			mShotAfter;	//!< 発射経過時間
		f32			mShotTime;	//!< 発射時間
		bool		mbRand;		//!< ランダム発射
		TGL::TransVec	mSwing;	//!< 首振り
	};
	
	/* ========================================================================= */
	//!		tBGStg01::SpaceLaser												 */
	/* ========================================================================= */
	class SpaceLaser
	{
	public:
		SpaceLaser();
		virtual	~SpaceLaser() {}
		
		//!	更新
		bool	update( f32 fDelte );
		
		TGL::Matrix	mMtx;	//!< エフェクト用マトリクス
		f32			mTime;	//!< 発生経過
	};
	
	// -----------------------------------------------------------------
	//!		関数		
	// -----------------------------------------------------------------
	//!	リセット
	virtual void	reset();
	
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
	//!	スクリプトメッセージ受け取り
	virtual void	receiveMessage( s32 nId, s32 nParam );
	//!	更新 宇宙 遠景
	void		setupSpaceFar();
	static void	setupSpaceFarCB( tBG* pBG )	{ ((tBGStg01*)(pBG))->setupSpaceFar(); }
	//!	更新 宇宙 戦艦内
	void		setupSpaceNear();
	static void	setupSpaceNearCB( tBG* pBG )	{ ((tBGStg01*)(pBG))->setupSpaceNear(); }
	//!	更新 宇宙 カタパルト
	void		setupSpaceCatapult();
	static void	setupSpaceCatapultCB( tBG* pBG )	{ ((tBGStg01*)(pBG))->setupSpaceCatapult(); }
	//!	更新 宇宙
	void		setupSpace();
	static void	setupSpaceCB( tBG* pBG )	{ ((tBGStg01*)(pBG))->setupSpace(); }
	void		finalSpace();
	static void	finalSpaceCB( tBG* pBG )	{ ((tBGStg01*)(pBG))->finalSpace(); }
	void		updateSpace( bool bSceneChg );
	static void	updateSpaceCB( tBG* pBG, bool bSceneChg )	{ ((tBGStg01*)(pBG))->updateSpace( bSceneChg ); }
	//!	更新 雲パーティクル
	void		setupCloudPar();
	static void	setupCloudParCB( tBG* pBG )	{ ((tBGStg01*)(pBG))->setupCloudPar(); }
	void		finalCloudPar();
	static void	finalCloudParCB( tBG* pBG )	{ ((tBGStg01*)(pBG))->finalCloudPar(); }
	void		updateCloudPar( bool bSceneChg );
	static void	updateCloudParCB( tBG* pBG, bool bSceneChg )	{ ((tBGStg01*)(pBG))->updateCloudPar( bSceneChg ); }
	//!	更新 海
	void		setupSea();
	static void	setupSeaCB( tBG* pBG )	{ ((tBGStg01*)(pBG))->setupSea(); }
	void		finalSea();
	static void	finalSeaCB( tBG* pBG )	{ ((tBGStg01*)(pBG))->finalSea(); }
	void		updateSea( bool bSceneChg );
	static void	updateSeaCB( tBG* pBG, bool bSceneChg )	{ ((tBGStg01*)(pBG))->updateSea( bSceneChg ); }
	//!	更新 遠景 山表示
	void		setupFarMount();
	static void	setupFarMountCB( tBG* pBG )	{ ((tBGStg01*)(pBG))->setupFarMount(); }
	//!	更新 滝終了
	void		updateWaterfallEnd( bool bSceneChg );
	static void	updateWaterfallEndCB( tBG* pBG, bool bSceneChg )	{ ((tBGStg01*)(pBG))->updateWaterfallEnd( bSceneChg ); }
	//!	更新 ゲート閉まる
	void		updateGateClose( bool bSceneChg );
	static void	updateGateCloseCB( tBG* pBG, bool bSceneChg )	{ ((tBGStg01*)(pBG))->updateGateClose( bSceneChg ); }
	//!	更新 砲台無力化
	void		updateCannonOff( bool bSceneChg );
	static void	updateCannonOffCB( tBG* pBG, bool bSceneChg )	{ ((tBGStg01*)(pBG))->updateCannonOff( bSceneChg ); }
	//!	更新 ドライブ起動
	void		updateDriveOpen( bool bSceneChg );
	static void	updateDriveOpenCB( tBG* pBG, bool bSceneChg )	{ ((tBGStg01*)(pBG))->updateDriveOpen( bSceneChg ); }
	//!	更新 ドライブ開始
	void		updateDriveStart( bool bSceneChg );
	static void	updateDriveStartCB( tBG* pBG, bool bSceneChg )	{ ((tBGStg01*)(pBG))->updateDriveStart( bSceneChg ); }

	//!	海描画リスト実行
	static void	drawlistExecSea( void* pData )	{ ((tBGStg01*)pData)->drawSea(); }
	//!	海描画
	void	drawSea();

	// -----------------------------------------------------------------
	//!		変数		
	// -----------------------------------------------------------------
	TGL::Vertex::InputLayout	mInputLayout;	//!< 頂点レイアウト
	BGSceneChange	mSceneChange;	//!< シーン遷移

	TGL::SoundChannel*	mpSEHatchMove;	//!< SEハッチ開閉
	bool			mbCatapultFix;		//!< 発進 自機をカタパルトへ固定
	TGL::TransFloat	mCatapultMove;		//!< 発進 カタパルトの位置レート
	bool						mbSpaceLaser;		//!< 発進 レーザー発生
	f32							mSpaceLaserTime;	//!< 発進 レーザータイミング
	stl::vector<SpaceLaser*>	mSpaceLaser;		//!< 発進 レーザー

	f32		mSeaNml1Repeat;			//!< 海 法線マップリピート
	f32		mSeaNml1SpdU;			//!< 海 法線マップ横速度
	f32		mSeaNml1SpdV;			//!< 海 法線マップ縦速度
	f32		mSeaNml2Repeat;			//!< 海 法線マップリピート
	f32		mSeaNml2SpdU;			//!< 海 法線マップ横速度
	f32		mSeaNml2SpdV;			//!< 海 法線マップ縦速度
	f32		mSeaAmbient;			//!< 海 アンビエント
	f32		mSeaReflection;			//!< 海 環境マップ強度
	FRGB	mSeaDiffuse;			//!< 海 ディフューズ
	f32		mSeaSpecular;			//!< 海 スペキュラ
	f32		mSeaSpecularPower;		//!< 海 スペキュラ範囲
	f32		mSeaFresnelScale;		//!< 海 フレネル項スケール
	f32		mSeaFlickerSpeed;		//!< 海 揺らぎ 速度
	f32		mSeaFlickerFrequency;	//!< 海 揺らぎ 周期
	f32		mSeaFlickerAmplitude;	//!< 海 揺らぎ 振幅
	
	TGL::Vec2	mSeaNmlUV;		//!< 海 法線マップ
	TGL::Vec2	mSeaNml2UV;		//!< 海 法線マップ２
	f32			mSeaNml2Alpha;	//!< 海 法線マップ２透過度
	f32			mSeaFlicker;	//!< 海 揺らぎ
	
	TGL::Vec2	mFoamA_Uv;		//!< 泡 UV速度
	TGL::Vec2	mFoamB_Uv;		//!< 泡 UV速度
	TGL::Vec2	mRiver06_Uv;	//!< 川 UV速度
	TGL::Vec2	mRiver07_Uv;	//!< 川 UV速度

	SLLINK_T	mEftCloudLink;	//!< 雲エフェクトリンク
	TGL::Matrix	mEftCloudMtx;	//!< 雲パーティクル追従マトリクス

	stl::vector<Cannon*>	mCannon;	//!< 対空砲
	bool		mbCannonEffect;	//!< 対空砲発射エフェクト

	TGL::SoundChannel*	mpSEGateMoveL;	//!< SEゲート移動
	TGL::SoundChannel*	mpSEGateMoveR;	//!< SEゲート移動

	f32		mDriveAnmOld;	//!< 前回のドライブ起動アニメ時間
	TGL::Vec2	mSLSD_UvA;	//!< ドライブ軌道A UV速度
	TGL::Vec2	mSLSD_UvB;	//!< ドライブ軌道B UV速度
	TGL::Vec2	mSLSD_UvC;	//!< ドライブ軌道C UV速度
	TGL::Vec2	mSLSD_UvD;	//!< ドライブ軌道D UV速度

	SLLINK_T	mEftLink[4];	//!< エフェクトリンク
	
	tLensFlareSF*	mpLensflare;		//!< レンズフレア
	tLensFlareSF*	mpLensflareSpace;	//!< レンズフレア(宇宙)
};
