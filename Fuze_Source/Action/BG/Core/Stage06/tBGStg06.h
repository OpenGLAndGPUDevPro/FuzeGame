// -------------------------------------------------------------------------- //
//! @file   tBGStg06.h
//! @brief  背景 ６面
//! @author Nal
//! @since  13/08/30(金)
//! 
//! COPYRIGHT (C) 2013 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
#pragma once
/* ====< インクルード >====================================================== */
#include "../tBG.h"
#include "../../Class/tDust.h"
#include "Effect/Lensflare/tLensFlareSF.h"

/* ========================================================================= */
//!		tBGStg06															 */
/* ========================================================================= */
class tBGStg06 : public tBG
{
	friend class DMBGStg06;
public:
	tBGStg06();
	virtual	~tBGStg06();
	
public:
	/* ========================================================================= */
	//!		tBGStg06::BladeBreak												 */
	/* ========================================================================= */
	class BladeBreak
	{
	public:
		BladeBreak( tBG* pBg );
		virtual	~BladeBreak();

		//!	更新
		void	update();
		
		//!	描画リスト実行
		static void	drawlistExec( void *pData ){
			PIX_BEGIN_EVENT(IRGBA(0xff, 0,0,0xff), "Render BG BladeBreak");
			PERF_BEGIN("tBGStg06::BladeBreak::draw",IRGBA( 200,0,0,128 ));
			((tBGStg06::BladeBreak*)pData)->draw();
			PERF_END("tBGStg06::BladeBreak::draw");
			PIX_END_EVENT();
		}
		//!	描画
		void	draw() {
			mpModel->Render( TGL::nModel::PASS_COLOR, false );
		}

		tBG*		mpOwner;	//!< オーナー
		bool		mbEnd;		//!< 終了判定
		f32			mTime;		//!< 経過時間
		TGL::Matrix					mMtx;		//!< マトリクス
		TGL::Model*					mpModel;	//!< モデル
		TGL::AnimationController*	mpAnim;		//!< モーション
	};

	// -----------------------------------------------------------------
	//!		定義		
	// -----------------------------------------------------------------

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
	//!	カメラ更新後の処理
	virtual void	updateCameraAfter();
	//!	スクリプトメッセージ受け取り
	virtual void	receiveMessage( s32 nId, s32 nParam );
	//!	更新 ドライブ
	void		setupDrive();
	static void	setupDriveCB( tBG* pBG )	{ ((tBGStg06*)(pBG))->setupDrive(); }
	void		finalDrive();
	static void	finalDriveCB( tBG* pBG )	{ ((tBGStg06*)(pBG))->finalDrive(); }
	void		updateDrive( bool bSceneChg );
	static void	updateDriveCB( tBG* pBG, bool bSceneChg )	{ ((tBGStg06*)(pBG))->updateDrive( bSceneChg ); }
	//!	更新 ドライブ終了
	void		setupDriveEnd();
	static void	setupDriveEndCB( tBG* pBG )	{ ((tBGStg06*)(pBG))->setupDriveEnd(); }
	void		updateDriveEnd( bool bSceneChg );
	static void	updateDriveEndCB( tBG* pBG, bool bSceneChg )	{ ((tBGStg06*)(pBG))->updateDriveEnd( bSceneChg ); }
	//!	更新 塵パーティクル
	void		setupDustPar();
	static void	setupDustParCB( tBG* pBG )	{ ((tBGStg06*)(pBG))->setupDustPar(); }
	void		finalDustPar();
	static void	finalDustParCB( tBG* pBG )	{ ((tBGStg06*)(pBG))->finalDustPar(); }
	void		updateDustPar( bool bSceneChg );
	static void	updateDustParCB( tBG* pBG, bool bSceneChg )	{ ((tBGStg06*)(pBG))->updateDustPar( bSceneChg ); }
	//!	更新 塵
	void		setupDust();
	static void	setupDustCB( tBG* pBG )	{ ((tBGStg06*)(pBG))->setupDust(); }
	void		finalDust();
	static void	finalDustCB( tBG* pBG )	{ ((tBGStg06*)(pBG))->finalDust(); }

	//!	デバッグ情報表示
	virtual void	dispDebugInfo();

	// -----------------------------------------------------------------
	//!		変数		
	// -----------------------------------------------------------------
	TGL::Vec2		mFar01_Uv;			//!< 遠景01 星UV速度
	TGL::Vec2		mLoop01_UvA;		//!< ループ01A UV速度
	TGL::Vec2		mLoop01_UvB;		//!< ループ01B UV速度
	TGL::Vec2		mLoop01_UvC;		//!< ループ01C UV速度
	TGL::Vec2		mLoop01_UvD;		//!< ループ01D UV速度
	TGL::Vec2		mDriveObj_UvA;		//!< ドライブObj UV速度
	TGL::Vec2		mDriveObj_UvB;		//!< ドライブObj UV速度
	f32				mDriveObj_Roll;		//!< ドライブObj 回転
	TGL::TransFloat	mDriveEndPos;		//!< ドライブ終端 座標
	TGL::Vec2		mMother_Uv_Glow;	//!< 母星 UV速度
	TGL::Vec2		mMother_Uv_Glow2;	//!< 母星 UV速度
	TGL::Vec2		mMother_Uv_Cloud;	//!< 母星 UV速度
	TGL::Vec2		mMother_Uv_Hole;	//!< 母星 UV速度
	TGL::Vec2		mMother_Uv_String;	//!< 母星 UV速度
	TGL::Vec2		mMother_Uv_String2;	//!< 母星 UV速度
	SLLINK_T		mEftDustLink;		//!< 塵エフェクトリンク
	TGL::Matrix		mEftDustMtx;		//!< 塵パーティクル追従マトリクス
	tDust*			mpDust;				//!< 塵
	BladeBreak*		mpBlade;			//!< 剣破壊
	tLensFlareSF*	mpLensflare;		//!< レンズフレア
	TGL::TransFloat	mLensflareAlpha;	//!< レンズフレアアルファ
	TGL::Vec3		mSpaceCut2Rot;		//!< 宇宙 カット２ 天球角度
	f32				mSpaceCut2Fov;		//!< 宇宙 カット２ FOV
};
