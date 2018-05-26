// -------------------------------------------------------------------------- //
//! @file   tBG.h
//! @brief  背景タスク
//! @author Nal
//! @since  11/09/05(月)
//!
//! COPYRIGHT (C) 2011 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
#pragma	once
/* ====< インクルード >====================================================== */
#include <Graphics/Filter/TGLFilterToneCurve.h>
#include "../sBG.h"
#include "../Class/cBGModel.h"
#include "../Class/cSceneParam.h"
#include "../Class/cModelSet.h"
#include "../Class/cUpdateEvent.h"

/* ========================================================================= */
//		tBG																	 */
/* ========================================================================= */
class cUpdateEvent;

class tBG
{
	friend class BGViewer;
	friend class DMCamera;
	friend class DMModel;
public:
	tBG();
	virtual ~tBG();

public:
	// -----------------------------------------------------------------
	//		関数		
	// -----------------------------------------------------------------
	//!	リセット
	virtual void	reset();
	//!	リスタート
	virtual void	restart();
	//!	新規設定
	bool	set( u8 uBG );
	//!	カメラ更新後の処理
	virtual void	updateCameraAfter();
	//!	エフェクト開始
	virtual void	beginEffect(){}
	//!	SE停止
	void	stopSE( s32 nFrame=0 ) {
		foreach( stl::vector<TGL::SoundChannel*>, mSE, it )
			(*it)->stop( nFrame );
		mSE.clear();
		foreach( stl::vector<TGL::SoundChannel*>, mLoopKeepSE, it )
			(*it)->stop( nFrame );
		mLoopKeepSE.clear();
	}
	//!	ループキープ監視追加
	void	addLoopKeepSE( TGL::SoundChannel* pCh )	{ mLoopKeepSE.push_back( pCh ); }
	//!	ループキープ監視外す
	void	eraseLoopKeepSE( TGL::SoundChannel* pCh ) {
		if( !pCh )	return;
		stl::vector<TGL::SoundChannel*>::iterator	it = std::find( mLoopKeepSE.begin(), mLoopKeepSE.end(), pCh );
		if( it != mLoopKeepSE.end() )
			mLoopKeepSE.erase( it );
	}

	//!	指定名のモデル数
	u8			getModelNum( const stl::string& strName );
	//!	指定名のモデルを取得する
	cBGModel*	getModel( const stl::string& strName, u8 uIdx=0 );
	//!	指定名のモデルの番号を取得する
	u8			getModelId( const stl::string& strName );
	//!	指定名のモデルジョイントを取得する
	TGL::Skelton::Joint*	getModelJoint( const stl::string& strModel, const stl::string& strJoint, u8 uIdx=0 ) {
		cBGModel*	pModel = getModel( strModel, uIdx );
		if( !pModel || !pModel->GetSkelton() )	return NULL;
		return pModel->GetSkelton()->GetJoint( strJoint.c_str() );
	}

	//!	指定名のモデルセットを取得する
	cModelSet*	getModelSet( const stl::string& strName, u8 uId=0xFF ) {
		foreach( stl::vector<cModelSet*>, mModelSet, it ){
			if( (*it)->getName() == strName ){
				if( uId == 0xFF )	return *it;
				else if( (*it)->getId() == uId )	return *it;
			}
		}
		return NULL;
	}
	

	//!	更新イベント追加
	void	addUpdateEvent( cUpdateEvent* pEvent )	{ mUpdateEvent.push_back( pEvent ); }
	//!	更新イベント追加
	cUpdateEvent*	addUpdateEvent( void (*pUpdateCB)(tBG*,bool), s8 nSceneMin=-1, s8 nSceneMax=-1, void (*pSetupCB)(tBG*)=NULL, void (*pFinalCB)(tBG*)=NULL );

	//!	スクリプトメッセージ受け取り
	virtual void	receiveMessage( s32 nId, s32 nParam ) {}

	//!	セットアップ済みかどうか
	bool	isEnable() const	{ return (mbEnable && (mBGId != BGID_NONE)); }

	//!	ビューワーモード
	bool	isViewer() const		{ return mbViewer; }
	void	setViewer( bool b0 )	{ mbViewer = b0; }

	//!	シーンパラメータ
	cSceneParam&	getSceneParam()	{ return mSceneParam; }
	//!	原点位置の取得
	virtual TGL::Vec3	getOrigin() const { return TGL::Vec3::ZERO; }
	//!	シーンの環境マップをモデルへ適用する
	void	overwriteEnvTex();
	
	//!	背景名を取得
	const char*	getName() const;

	//		アクセサ		
	f32		getDeltaTime() const	{ return mDeltaTime; }
	u8		getBGId() const			{ return mBGId; }
	u8		getScene() const		{ return mScene; }
	void	setScene( u8 i0 )		{ mScene = i0; }
	u8		getSceneOld() const		{ return mSceneOld; }
	bool	isAllScene() const		{ return mbAllScene; }
	void	setAllScene( bool b0 )	{ mbAllScene = b0; }
	void	setSceneApplyOff( bool b0 )	{ mbSceneApplyOff = b0; }
	void	setAreaWait( bool b0 )	{ mbAreaWait = b0; }
	f32		getTimePass() const		{ return mTimePass; }
	GLTSKHEAD_T&	getTskHead()	{ return mTskHead; }
	bool	isDebugInfo() const		{ return mbDebugInfo; }
	void	setDebugInfo( bool b0 )	{ mbDebugInfo = b0; }
	void	xorDebugInfo()			{ mbDebugInfo ^= 1; }
	TGL::PerfCount&	getPerfCpu()	{ return mPerfCpu; }
	TGL::PerfCount&	getPerfGpu()	{ return mPerfGpu; }

protected:
	// -----------------------------------------------------------------
	//		定義		
	// -----------------------------------------------------------------
	//!	シーンパラメータ遷移コール
	typedef struct {
		f32	mTiming;	//!< コールタイミング
		u32	mTransTime;	//!< 推移時間
	} SCENECHG_PARAM_T;

	// -----------------------------------------------------------------
	//		関数		
	// -----------------------------------------------------------------
	//!	リソースロード
	virtual bool	load( u8 uBG ){
		mSceneParam.setup();
		return true;
	}
	//!	モデル追加
	cBGModel*	addModel( const stl::string& strFName, u8 uType=cBGModel::TYPE_NORMAL, u32 uFlag=0 );
	//!	指定Jsonからモデル配置
	void	applyModelSetFromJson( const stl::string& strPath, const stl::string& strObj="ModelSet" );
	//!	指定モデルにアサインさせてモデルセット追加（モデルセットへのアサイン）
	cModelSet*	addModelSetAssign( const stl::string& strAddModel, const stl::string& strModel, const stl::string& strBone, bool bScale=false );
	//!	指定モデルにアサインさせてモデルセット追加
	cModelSet*	addModelSetAssignExec( const stl::string& strAddModel, const stl::string& strModel, const stl::string& strBone, bool bScale=false, const TGL::Matrix& mtxOfs=TGL::Matrix::UNIT );
	//!	現在のモデル配置をファイル出力
	void	saveModelSet();

	//!	指定Jsonからカメラパス配置
	void	applyCameraPathSetFromJson( const stl::string& strPath, const stl::string& strObj="Group" );
	//!	現在のカメラパスをファイル出力
	void	saveCameraPath();
	//!	カメラパスリロード
	void	reloadCameraPath(){
		if( mCameraPath.empty() )	return;
		IFileManager->reload( mCameraPath );
		applyCameraPathSetFromJson( mCameraPath );
	}

	//!	レンズフレア生成
	class tLensFlareSF*	createLensFlare( const TGL::MtxGuide& guide, const stl::string& strTexPrefix, const stl::string& strParamRes, const stl::string& strParam );

	//!	タスク更新
	static s32	updateTask( tBG* pBG, u32 uMsg );
	//!	スクロール更新
	virtual void	updateScroll(){}
	//!	遠景描画リスト実行
	static void	drawlistFarExec( void *pData ){
		PIX_BEGIN_EVENT(IRGBA(0xff, 0,0,0xff), "Render BG Far");
		PERF_BEGIN("tBG::drawFar",IRGBA( 200,0,0,128 ));
		((tBG*)pData)->drawFar();
		PERF_END("tBG::drawFar");
		PIX_END_EVENT();
	}
	//!	近景描画リスト実行
	static void	drawlistNearExec( void *pData ){
		PIX_BEGIN_EVENT(IRGBA(0xff, 0,0,0xff), "Render BG Near");
		PERF_BEGIN("tBG::drawNear",IRGBA( 200,0,0,128 ));
		((tBG*)pData)->drawNear();
		PERF_END("tBG::drawNear");
		PIX_END_EVENT();
	}
	//!	近景影描画リスト実行
	static void	drawlistNearShadowExec( void *pData ){
		PIX_BEGIN_EVENT(IRGBA(0xff, 0,0,0xff), "Render BG Near Shadow");
		PERF_BEGIN("tBG::drawNearShadow",IRGBA( 200,0,0,128 ));
		((tBG*)pData)->drawNear(true);
		PERF_END("tBG::drawNearShadow");
		PIX_END_EVENT();
	}
	//!	Zバッファクリア
	static void	clearZBuffer( void *pData );
	//!	更新
	virtual void	update();
	//!	遠景描画
	virtual void	drawFar();
	//!	固有遠景描画	ビューマトリクスの再更新が必要な場合trueを返す
	virtual bool	drawFarEx() { return false; }
	//!	近景描画
	virtual void	drawNear(bool bDrawShadow=false);

	//!	モデルのシーン有効チェック
	template <typename T> bool	isTstOnScene( const T& Model ) const;

	//!	デバッグ情報表示
	virtual void	dispDebugInfo();
	//!	指定モデルのロケータを画面表示
	void	dispDebugInfoLocator( const TGL::BaseCamera* pCam, const stl::string& strModel, const stl::string& strLocator, stl::vector<TGL::Vec2*>* pPosBuf=NULL );
	//!	指定モデルのロケータを画面表示（モデル直接指定）
	void	dispDebugInfoLocator( const TGL::BaseCamera* pCam, cBGModel* pModel, const stl::string& strLocator, stl::vector<TGL::Vec2*>* pPosBuf=NULL );
	//!	指定モデルのロケータを画面表示
	void	dispDebugInfoLocatorExec( const TGL::BaseCamera* pCam, cModelSet* pSet, cBGModel* pModel, const stl::string& strLocator, stl::vector<TGL::Vec2*>* pPosBuf );

	// -----------------------------------------------------------------
	//		変数		
	// -----------------------------------------------------------------
	f32			mDeltaTime;		//!< デルタタイム（外部参照用）	
	f32			mDeltaTimeLocal;//!< デルタタイム（ローカル）	
	bool		mbEnable;		//!< セットアップ済み
	bool		mbViewer;		//!< ビューワーモード
	bool		mbSceneApplyOff;//!< シーンパラメータの反映
	bool		mbAreaWait;		//!< スクリプト側からのエリアセットアップウェイト
	u8			mBGId;			//!< 背景ID [BGID_????]
	cSceneParam	mSceneParam;	//!< シーンパラメータ
	u8			mScene;			//!< シーン設定
	u8			mSceneOld;		//!< 前回のシーン設定
	bool		mbAllScene;		//!< モデルのシーン設定を無視（デバッグ用）
	stl::vector<cUpdateEvent*>		mUpdateEvent;	//!< 更新イベント
	stl::vector<cBGModel*>			mModelArray;	//!< モデル
	stl::vector<cModelSet*>			mModelSet;		//!< モデル配置
	stl::vector<TGL::SoundChannel*>	mSE;			//!< SE
	stl::vector<TGL::SoundChannel*>	mLoopKeepSE;	//!< ループキープ監視SE
	stl::string	mModelSetPath;	//!< モデル配置リソースのパス
	stl::string	mCameraPath;	//!< カメラリソースのパス
	f32			mSkyNear;		//!< 天球描画カメラニア
	f32			mSkyFar;		//!< 天球描画カメラファー
	f32			mSkyFov;		//!< 天球描画カメラFOV [0以下:変更なし]
	f32			mTimePass;		//!< 経過時間
	bool		mbDebugInfo;	//!< デバッグ情報表示
	TGL::PerfCount	mPerfCpu;	//!< パフォーマンス計測
	TGL::PerfCount	mPerfGpu;	//!< パフォーマンス計測
	GLTSKHEAD_T	mTskHead;		//!< タスクヘッダ
};

/* ========================================================================= */
/*		EOF																	 */
/* ========================================================================= */
