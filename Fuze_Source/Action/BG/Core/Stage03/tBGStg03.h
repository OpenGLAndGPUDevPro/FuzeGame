// -------------------------------------------------------------------------- //
//! @file   tBGStg03.h
//! @brief  背景 ３面
//! @author Nal
//! @since  12/08/17(金)
//! 
//! COPYRIGHT (C) 2012 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
#pragma once
/* ====< インクルード >====================================================== */
#include "../tBG.h"
#include "Effect/Lensflare/tLensFlareSF.h"
#include "../../Class/cModelSetSort.h"
#include "../../Class/tDust.h"

/* ========================================================================= */
//!		tBGStg03															 */
/* ========================================================================= */
class tBGStg03 : public tBG
{
	friend class DMBGStg03;
public:
	tBGStg03();
	virtual	~tBGStg03();
	
public:
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
	//!		tBGStg03::CloudParam		
	// -----------------------------------------------------------------
	class CloudParam {
	public:
		CloudParam()
		: mBlend(RSBL_HALF), mHeight(-1000), mColor(0xFFFFFFFF), mUVRepeat(30), mUVOfs(0)
		{}
		virtual	~CloudParam() {}

		u8		mBlend;		//!< ブレンドモード
		f32		mHeight;	//!< 高さ
		RGBA32	mColor;		//!< 色
		f32		mUVRepeat;	//!< UVリピート
		f32		mUVOfs;		//!< UVオフセット
	};

	// -----------------------------------------------------------------
	//!		定義		
	// -----------------------------------------------------------------
	//!	雲配置データ
	typedef struct {
		f32					mZ;				//!< Z値
		stl::string	mStrLocator;	//!< ロケータ名
		stl::string	mStrModel;		//!< モデル名
	} CLOUDSET_T;

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
	//!	更新 塵パーティクル
	void		setupDustPar();
	static void	setupDustParCB( tBG* pBG )	{ ((tBGStg03*)(pBG))->setupDustPar(); }
	void		finalDustPar();
	static void	finalDustParCB( tBG* pBG )	{ ((tBGStg03*)(pBG))->finalDustPar(); }
	void		updateDustPar( bool bSceneChg );
	static void	updateDustParCB( tBG* pBG, bool bSceneChg )	{ ((tBGStg03*)(pBG))->updateDustPar( bSceneChg ); }
	//!	更新 塵
	void		setupDust();
	static void	setupDustCB( tBG* pBG )	{ ((tBGStg03*)(pBG))->setupDust(); }
	void		finalDust();
	static void	finalDustCB( tBG* pBG )	{ ((tBGStg03*)(pBG))->finalDust(); }
	//!	更新 遠景回す
	void		updateFar( bool bSceneChg );
	static void	updateFarCB( tBG* pBG, bool bSceneChg )	{ ((tBGStg03*)(pBG))->updateFar( bSceneChg ); }
	//!	更新 雲海開始
	void		updateCloudBegin( bool bSceneChg );
	static void	updateCloudBeginCB( tBG* pBG, bool bSceneChg )	{ ((tBGStg03*)(pBG))->updateCloudBegin( bSceneChg ); }
	//!	更新 雲パーティクル
	void		updateCloudPar( bool bSceneChg );
	static void	updateCloudParCB( tBG* pBG, bool bSceneChg )	{ ((tBGStg03*)(pBG))->updateCloudPar( bSceneChg ); }
	//!	更新 天球スケール
	void		updateFarScale( bool bSceneChg );
	static void	updateFarScaleCB( tBG* pBG, bool bSceneChg )	{ ((tBGStg03*)(pBG))->updateFarScale( bSceneChg ); }

	//!	固有遠景描画	ビューマトリクスの再更新が必要な場合trueを返す
	virtual bool	drawFarEx();
	//!	雲海描画リスト実行
	static void	drawlistExecCloud( void* pData )	{ ((tBGStg03*)pData)->drawCloud(); }
	//!	雲海描画
	void	drawCloud();

	//!	デバッグ情報表示
	virtual void	dispDebugInfo();

	// -----------------------------------------------------------------
	//!		変数		
	// -----------------------------------------------------------------
	u8				mMeteorAlphaRef;	//!< 隕石 AlphaRef
	f32				mCloudSize;			//!< 雲海サイズ
	f32				mCloudRot;			//!< 雲海回転
	CloudParam		mCloudParam[3];		//!< 雲海パラメータ
	tLensFlareSF*	mpLensflare;		//!< レンズフレア
	cModelSetSort	mSortCloud;			//!< 道中雲ソート
	cModelSetSort	mSortCloudBoss;		//!< ボス雲ソート
	SLLINK_T		mEftCloudLink;		//!< エフェクトリンク
	TGL::Matrix		mEftCloudMtx;		//!< 雲パーティクル追従マトリクス
	tDust*			mpDust;				//!< 塵
	TGL::Vec2		m06_UvA;			//!< 06A UV速度
	TGL::Vec2		m06_UvB;			//!< 06B UV速度
	TGL::Vec2		m06_UvC;			//!< 06C UV速度
	TGL::Vec2		m06_UvD;			//!< 06D UV速度
	TGL::Vec2		mSpiral_UvA;		//!< 螺旋雲A UV速度
	TGL::Vec2		mSpiral_UvB;		//!< 螺旋雲B UV速度
	TGL::Vec2		mSpiral_UvC;		//!< 螺旋雲C UV速度
	TGL::Vec2		mCloud_UvA;			//!< 雲海A UV速度
	TGL::Vec2		mCloud_UvB;			//!< 雲海B UV速度
	TGL::Vec2		mCloud_UvC;			//!< 雲海C UV速度
	f32				mBossSkyScaleEnd;	//!< 天球スケール目標値
	TGL::TransFloat	mBossSkyScale;		//!< 天球スケール
	u16				mExEnvSize;			//!< 特殊環境マップサイズ
};
