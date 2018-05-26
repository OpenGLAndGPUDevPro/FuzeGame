// -------------------------------------------------------------------------- //
//! @file   tBGStg04.h
//! @brief  背景 ４面
//! @author Nal
//! @since  12/12/04(火)
//! 
//! COPYRIGHT (C) 2012 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
#pragma once
/* ====< インクルード >====================================================== */
#include "../tBG.h"
#include "../../Class/cBGSceneChange.h"
#include "Effect/Lensflare/tLensFlareSF.h"
#include "cBGDoorStg04.h"

/* ========================================================================= */
//!		tBGStg04															 */
/* ========================================================================= */
class tBGStg04 : public tBG
{
	friend class DMBGStg04;
public:
	tBGStg04();
	virtual	~tBGStg04();
	
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
	/* ========================================================================= */
	//!		tBGStg04::BossPiece													 */
	/* ========================================================================= */
	class BossPiece
	{
	public:
		BossPiece( u8 uMesh, const TGL::Vec3& vPos );
		virtual	~BossPiece();

		//		定義		
		enum {
			MESH_A = 0,
			MESH_B,
			MESH_C,
			
			LIFE = 360,	//!< 寿命
		};
		
		//		関数		
		//!	更新
		void	update( tBG& Bg );
		
		//!	描画リスト実行
		static void	drawlistExec( void *pData ){
			PIX_BEGIN_EVENT(IRGBA(0xff, 0,0,0xff), "Render BG BossPiece");
			PERF_BEGIN("tBGStg04::BossPiece::draw",IRGBA( 200,0,0,128 ));
			((tBGStg04::BossPiece*)pData)->draw();
			PERF_END("tBGStg04::BossPiece::draw");
			PIX_END_EVENT();
		}
		//!	描画
		void	draw() {
			if( getAlpha() < 1.0f )
				mpModel->RenderAlpha();
			else
				mpModel->Render( TGL::nModel::PASS_COLOR, false );
		}
		
		//!	アルファ取得
		f32		getAlpha()	{ return TGL::Math::Clamp<f32>( ((LIFE-mTime)/30.0f), 0.0f, 1.0f ); }

		//		変数		
		bool		mbEnd;		//!< 終了判定
		f32			mTime;		//!< 経過時間
		TGL::Model*	mpModel;	//!< モデル
		u8			mMeshId;	//!< 表示メッシュ [MESH_????]
		TGL::Vec3	mPos;		//!< 座標
		TGL::Vec3	mRot;		//!< 回転
		TGL::Vec3	mMove;		//!< 移動量
		TGL::Vec3	mRoll;		//!< 回転量
	};

	// -----------------------------------------------------------------
	//!		tBGStg04::ElectTime		
	// -----------------------------------------------------------------
	class ElectTime
	{
	public:
		//!	コンストラクタ
		ElectTime( f32 fTime, cModelSet* pModelSet, u8 uType, f32 fMin, f32 fMax, f32 fRot=0, const TGL::Vec3& vOfs=TGL::Vec3::ZERO )
		:	mTime(fTime), mpModelSet(pModelSet), mType(uType), mMin(fMin), mMax(fMax), mRot(fRot), mOfs(vOfs)
		{}
		//!	デストラクタ
		virtual	~ElectTime() {}
		
		enum {
			//!	座標タイプ
			TYPE_CAM_X = 0,	//!< カメラX
			TYPE_CAM_Y,		//!< カメラY
			TYPE_CAM_Z,		//!< カメラZ
			TYPE_MODEL_X,	//!< モデルX
			TYPE_MODEL_Y,	//!< モデルY
			TYPE_MODEL_Z,	//!< モデルZ
			TYPE_RAND,		//!< ランダム軸
		};
		
		//		変数		
		f32			mTime;		//!< 時間
		cModelSet*	mpModelSet;	//!< モデルセット
		u8			mType;		//!< 座標タイプ
		f32			mMin;		//!< 最小距離
		f32			mMax;		//!< 最大距離
		f32			mRot;		//!< 距離回転
		TGL::Vec3			mOfs;		//!< オフセット座標
	};

	// -----------------------------------------------------------------
	//!		tBGStg04::Elect		
	// -----------------------------------------------------------------
	class Elect
	{
	public:
		//!	コンストラクタ
		Elect( u8 uCamGroup, f32 fEndTime, s16 nEftId )
		:	mCamGroup(uCamGroup), mEndTime(fEndTime), mEftId(nEftId)
		{
			SLLINK_Init( &mEftLink );
			reset();
		}
		//!	デストラクタ
		virtual	~Elect() {
			EEMIT_End( &mEftLink );
		}
		
		//!	初期化
		void	reset() {
			mbActive	= false;
			mMtx		= TGL::Matrix::UNIT;
			EEMIT_End( &mEftLink );
		}
		//!	更新
		void	update( const tBGStg04& Bg );

		//		変数		
		bool		mbActive;	//!< 動作中
		SLLINK_T	mEftLink;	//!< エフェクトリンク
		TGL::Matrix			mMtx;		//!< 放電追従マトリクス
		const u8	mCamGroup;	//!< カメラグループ
		const f32	mEndTime;	//!< 終了時間 [0未満:無効]
		const s16	mEftId;		//!< 発生エフェクト
		stl::vector<ElectTime>	mTime;	//!< 時間イベント
	};

	// -----------------------------------------------------------------
	//!		定義		
	// -----------------------------------------------------------------
	enum {
		//!	エフェクトリンク
		EFTLINK_LENSFLARE_A = 0,	//!< レンズフレア追従
		EFTLINK_LENSFLARE_B,		//!< レンズフレア追従
		EFTLINK_CORESHINE,			//!< コア光
		EFTLINK_CORESHINEBG,		//!< コア光
		EFTLINK_RESULTSMOKE,		//!< リザルト煙
		EFTLINK_MAX,
		
		//!	放電
		ELECT_ROOT = 0,
		ELECT_HALLX,
		ELECT_HALLY,
		ELECT_HALLZ,
		
		//!	ドア
		DOOR_A = 0,
		DOOR_B,
		DOOR_C,
		DOOR_MAX,
		
		//!	SE エネルギーバイパスサブ
		ENPSESUB_UA = 0,
		ENPSESUB_UB,
		ENPSESUB_DA,
		ENPSESUB_DB,
		ENPSESUB_MAX,
	};

	// -----------------------------------------------------------------
	//!		関数		
	// -----------------------------------------------------------------
	//!	リソースロード
	virtual bool	load( u8 uBG );
	//!	UVアニメ設定
	void	setModelUvAnim();

	//!	レンズフレア設定
	void	setLensflare( const stl::string& strModel, const stl::string& strJoint, const TGL::Vec3& vOfs, const stl::string& strObj, s16 nEft=-1, s16 nSE=-1, u8 uIdx=0 );

	//!	更新
	virtual void	update();
	//!	スクリプトメッセージ受け取り
	virtual void	receiveMessage( s32 nId, s32 nParam );
	//!	更新 点光源
	void		updatePointLight( bool bSceneChg );
	static void	updatePointLightCB( tBG* pBG, bool bSceneChg )	{ ((tBGStg04*)(pBG))->updatePointLight( bSceneChg ); }
	//!	更新 レンズフレア 入口
	void		updateLensflareEntrance( bool bSceneChg );
	static void	updateLensflareEntranceCB( tBG* pBG, bool bSceneChg )	{ ((tBGStg04*)(pBG))->updateLensflareEntrance( bSceneChg ); }
	//!	更新 レンズフレア ３軸通路
	void		updateLensflare3Axis( bool bSceneChg );
	static void	updateLensflare3AxisCB( tBG* pBG, bool bSceneChg )	{ ((tBGStg04*)(pBG))->updateLensflare3Axis( bSceneChg ); }
	//!	更新 レンズフレア ３軸通路下
	void		updateLensflare3AxisD( bool bSceneChg );
	static void	updateLensflare3AxisDCB( tBG* pBG, bool bSceneChg )	{ ((tBGStg04*)(pBG))->updateLensflare3AxisD( bSceneChg ); }
	//!	更新 レンズフレア 縦穴
	void		updateLensflareHall( bool bSceneChg );
	static void	updateLensflareHallCB( tBG* pBG, bool bSceneChg )	{ ((tBGStg04*)(pBG))->updateLensflareHall( bSceneChg ); }
	//!	更新 レンズフレア 縦穴２
	void		updateLensflareHall2( bool bSceneChg );
	static void	updateLensflareHall2CB( tBG* pBG, bool bSceneChg )	{ ((tBGStg04*)(pBG))->updateLensflareHall2( bSceneChg ); }
	//!	更新 レンズフレア リザルト
	void		setupLensflareResult();
	static void	setupLensflareResultCB( tBG* pBG )	{ ((tBGStg04*)(pBG))->setupLensflareResult(); }
	void		finalLensflareResult();
	static void	finalLensflareResultCB( tBG* pBG )	{ ((tBGStg04*)(pBG))->finalLensflareResult(); }
	//!	更新 レンズフレア リザルト２
	void		setupLensflareResult2();
	static void	setupLensflareResult2CB( tBG* pBG )	{ ((tBGStg04*)(pBG))->setupLensflareResult2(); }
	void		finalLensflareResult2();
	static void	finalLensflareResult2CB( tBG* pBG )	{ ((tBGStg04*)(pBG))->finalLensflareResult2(); }
	//!	更新 ドア閉める
	void		updateDoorClose( bool bSceneChg );
	static void	updateDoorCloseCB( tBG* pBG, bool bSceneChg )	{ ((tBGStg04*)(pBG))->updateDoorClose( bSceneChg ); }

	//!	エネルギーバイパスサブSEリクエスト
	TGL::SoundChannel*	reqSE_ENPSub( const TGL::Vec3& vOfs, f32 fVol=1 );

	//!	デバッグ情報表示
	virtual void	dispDebugInfo();

	// -----------------------------------------------------------------
	//!		変数		
	// -----------------------------------------------------------------
	BGSceneChange		mSceneChange;		//!< シーン遷移
	TGL::Vec2			m01_UvA;			//!< 01A UV速度
	TGL::Vec2			m01_UvB;			//!< 01B UV速度
	TGL::Vec2			m01_UvC;			//!< 01C UV速度
	TGL::Vec2			m01_UvD;			//!< 01D UV速度
	f32					mCubeSpd;			//!< 箱アニメ速度
	f32					mBossUnitRollSpd;	//!< ボスユニットアニメ速度
	SLLINK_T			mEftLink[EFTLINK_MAX];		//!< エフェクトリンク
	TGL::SoundChannel*	mpSEENP[2];					//!< SE エネルギーバイパス
	TGL::SoundChannel*	mpSEENPSub[ENPSESUB_MAX];	//!< SE エネルギーバイパス
	TGL::SoundChannel*	mpSEBossLight;				//!< SE ボス光
	stl::vector<Elect*>	mElect;				//!< 放電
	stl::vector<BGDoorStg04*>	mDoor;			//!< ドア
	stl::vector<BossPiece*>		mBossPiece;		//!< ボス破片
	stl::vector<tLensFlareSF*>	mLensflare;		//!< レンズフレア
};
