// -------------------------------------------------------------------------- //
//! @file   tBGPrologue.h
//! @brief  背景 プロローグ
//! @author Nal
//! @since  13/05/14(火)
//! 
//! COPYRIGHT (C) 2013 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
#pragma once
/* ====< インクルード >====================================================== */
#include "../tBG.h"
#include "../../Class/cModelSetSort.h"
#include "Effect/Lensflare/tLensFlareSF.h"
#include "../../Class/tRain.h"

/* ========================================================================= */
//!		tBGPrologue															 */
/* ========================================================================= */
class tBGPrologue : public tBG
{
	friend class DMBGPrologue;
public:
	tBGPrologue();
	virtual	~tBGPrologue();
	
public:
	/* ========================================================================= */
	//!		tBGPrologue::Meteor													 */
	/* ========================================================================= */
	class Meteor
	{
	public:
		//!	コンストラクタ
		Meteor( tBGPrologue* pBg, const TGL::Matrix& mtx, f32 fSpd );
		//!	デストラクタ
		virtual	~Meteor();
		
		//!	更新
		void	update();
		
		tBGPrologue*	mpOwner;	//!< 親
		bool			mbEnd;		//!< 終了待ち
		TGL::Matrix		mMtx;		//!< マトリクス
		f32				mSpd;		//!< 速度
		TGL::ColLine	mCol;		//!< 当たり判定
		SLLINK_T		mEft;		//!< エフェクトリンク
		u32				mhSE;		//!< 飛来音
	};

	// -----------------------------------------------------------------
	//!		定義		
	// -----------------------------------------------------------------

	// -----------------------------------------------------------------
	//!		関数		
	// -----------------------------------------------------------------
	//!	リセット
	virtual void	reset();
	//!	エフェクト開始
	virtual void	beginEffect();
	
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
	//!	更新 雲パーティクル
	void		setupCloudPar();
	static void	setupCloudParCB( tBG* pBG )	{ ((tBGPrologue*)(pBG))->setupCloudPar(); }
	void		finalCloudPar();
	static void	finalCloudParCB( tBG* pBG )	{ ((tBGPrologue*)(pBG))->finalCloudPar(); }
	void		updateCloudPar( bool bSceneChg );
	static void	updateCloudParCB( tBG* pBG, bool bSceneChg )	{ ((tBGPrologue*)(pBG))->updateCloudPar( bSceneChg ); }
	//!	更新 雨
	void		setupRain();
	static void	setupRainCB( tBG* pBG )	{ ((tBGPrologue*)(pBG))->setupRain(); }
	void		finalRain();
	static void	finalRainCB( tBG* pBG )	{ ((tBGPrologue*)(pBG))->finalRain(); }
	void		updateRain( bool bSceneChg );
	static void	updateRainCB( tBG* pBG, bool bSceneChg )	{ ((tBGPrologue*)(pBG))->updateRain( bSceneChg ); }
	//!	更新 稲光
	void		updateLightning( bool bSceneChg );
	static void	updateLightningCB( tBG* pBG, bool bSceneChg )	{ ((tBGPrologue*)(pBG))->updateLightning( bSceneChg ); }
	//!	更新 戦艦周辺破片
	void		setupFire();
	static void	setupFireCB( tBG* pBG )	{ ((tBGPrologue*)(pBG))->setupFire(); }
	void		finalFire();
	static void	finalFireCB( tBG* pBG )	{ ((tBGPrologue*)(pBG))->finalFire(); }
	//!	更新 破片
	void		updateMeteor( bool bSceneChg );
	static void	updateMeteorCB( tBG* pBG, bool bSceneChg )	{ ((tBGPrologue*)(pBG))->updateMeteor( bSceneChg ); }
	//!	更新 空
	void		setupSky();
	static void	setupSkyCB( tBG* pBG )	{ ((tBGPrologue*)(pBG))->setupSky(); }
	void		finalSky();
	static void	finalSkyCB( tBG* pBG )	{ ((tBGPrologue*)(pBG))->finalSky(); }
	//!	更新 宇宙
	void		setupSpace();
	static void	setupSpaceCB( tBG* pBG )	{ ((tBGPrologue*)(pBG))->setupSpace(); }
	void		finalSpace();
	static void	finalSpaceCB( tBG* pBG )	{ ((tBGPrologue*)(pBG))->finalSpace(); }

	//!	デバッグ情報表示
	virtual void	dispDebugInfo();

	// -----------------------------------------------------------------
	//!		変数		
	// -----------------------------------------------------------------
	f32				mLightning;				//!< 稲光タイミング
	f32				mLightningSE;			//!< 稲光SEタイミング
	bool			mbLightningSE[4];		//!< 稲光SEテーブル
	TGL::TransFloat	mLightningAlpha;		//!< 稲光アルファ
	TGL::RangeNow<f32>	mLightningInt;		//!< 稲光間隔
	TGL::RangeNow<f32>	mLightningRel;		//!< 稲光減衰
	f32				mMeteorReq;				//!< 破片タイミング
	tLensFlareSF*	mpLensflare;			//!< レンズフレア
	stl::vector<Meteor*>		mMeteor;	//!< 破片
	stl::vector<TGL::ColPlane>	mValleyCol;	//!< 崖当たり
	SLLINK_T		mEftCloudLink;			//!< 雲エフェクトリンク
	TGL::Matrix		mEftCloudMtx;			//!< 雲パーティクル追従マトリクス
	tRain*			mpRain;					//!< 雨
	f32				mRainSpeedValley;		//!< 雨 速度 崖
	f32				mRainSpeedCity;			//!< 雨 速度 都市
	RGBA32			mRainColorBaseMin;		//!< 雨 基準色
	RGBA32			mRainColorBaseMax;		//!< 雨 基準色
	RGBA32			mRainColorLightMin;		//!< 雨 稲光色
	RGBA32			mRainColorLightMax;		//!< 雨 稲光色
	TGL::Vec2		mCityFall_Uv;			//!< 都市滝 UV速度
	cModelSetSort	mSortCity;				//!< 都市ソート
	TGL::Vec2		mDriveStar_Uv;			//!< ドライブ 星 UV速度
	TGL::Vec2		mDrive_UvA;				//!< ドライブ UV速度
	TGL::Vec2		mDrive_UvB;				//!< ドライブ UV速度
	TGL::Vec2		mDrive_UvC;				//!< ドライブ UV速度
	TGL::Vec2		mDrive_UvD;				//!< ドライブ UV速度
};
