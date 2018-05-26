// -------------------------------------------------------------------------- //
//! @file   cSceneParam.h
//! @brief  シーンパラメータ
//! @author Nal
//! @since  12/03/05(月)
//!
//! COPYRIGHT (C) 2012 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
#pragma	once
/* ====< インクルード >====================================================== */

/* ========================================================================= */
//		cSceneParam															 */
/* ========================================================================= */
class tBG;
class cSceneParam
{
	friend class BGViewer;
	friend class DMScene;
	friend class DMPointLight;
public:
	cSceneParam( tBG* pOwner );
	virtual ~cSceneParam();

public:	
	/* ========================================================================= */
	//		cSceneParam::LightDir												 */
	/* ========================================================================= */
	class LightDir
	{
	public:
		//!	コンストラクタ
		LightDir()
		:	mDirection(0.958f, 0.287f, 0.0f), mPower(0.5f)
		{
			FRGB_Set( &mColor, 1.0f, 1.0f, 1.0f );
		}
		//!	デストラクタ
		virtual ~LightDir() {}

		FRGB	mColor;		//!< 色
		TGL::Vec3		mDirection;	//!< 方向
		f32		mPower;		//!< 強さ
	};

	/* ========================================================================= */
	//		cSceneParam::LightPoint												 */
	/* ========================================================================= */
	class LightPoint
	{
	public:
		//!	コンストラクタ
		LightPoint( cSceneParam* pParent )
		:	mpParent(pParent), mPos(TGL::Vec3::ZERO), mRangeMin(1000), mRangeMax(2000), mPower(1.0f),
			mType(Data::LIGHT_BG), mOnScene(0xFFFFFFFF), mLoop(1), mLoopPos(TGL::Vec3::ZERO), 
			mSortPos(TGL::Vec3::ZERO), mSortDist(0),
			mPosOfs(TGL::Vec3::ZERO)
		{
			FRGB_Set( &mColor, 1.0f, 1.0f, 1.0f );
			mBlink.clear( 1.0f );
			mBlink.setStart( 0 );
			mBlink.setEnd( 1 );
		}
		//!	デストラクタ
		virtual ~LightPoint() {}
		
		//		関数		
		//!	更新
		void	update( f32 fDelta );
		//!	有効か
		bool	isEnable( u8 uType ) const;
		//!	ソート
		static bool	sortCB_PointLight( LightPoint* pA, LightPoint* pB );

		//		アクセサ		
		u32					tstOnScene( u32 i0 ) const		 { return tstFLG(mOnScene,i0); }
		const TGL::Vec3&	getPosOfs() const				 { return mPosOfs; }
		void				setPosOfs( const TGL::Vec3& v0 ) { mPosOfs = v0; }
		TGL::Vec3			getWorldPos() const;

		//		変数		
		cSceneParam*	mpParent;	//!< 親
		FRGB			mColor;		//!< 色
		TGL::Vec3		mPos;		//!< 座標
		f32				mRangeMin;	//!< 減衰開始距離
		f32				mRangeMax;	//!< 減衰完了距離
		f32				mPower;		//!< 強さ
		TGL::TransFloat	mBlink;		//!< 明滅
		u8				mType;		//!< 対象 [LIGHT_????]
		u32				mOnScene;	//!< 有効シーン
		u8				mLoop;		//!< ループ数
		TGL::Vec3		mLoopPos;	//!< ループ座標
		TGL::Vec3		mSortPos;	//!< ソート比較用座標（カメラから近いとこ）
		f32				mSortDist;	//!< ソート比較用カメラ距離

		TGL::Vec3		mPosOfs;	//!< 外部操作用オフセット座標
	};

	/* ========================================================================= */
	//		cSceneParam::Light													 */
	/* ========================================================================= */
	class Light
	{
	public:
		Light();
		virtual ~Light();

		//		関数		
		//!	クリア
		void	clear() {
			foreach( stl::vector<LightDir*>, mDir, it )
				SAFE_DELETE( *it );
			mDir.clear();
		}

		//!	平行光削除
		void	eraseDir( LightDir* p0 ){
			for( stl::vector<LightDir*>::iterator it=mDir.begin() ; it!=mDir.end() ; ){
				if( p0 == *it ){
					SAFE_DELETE( *it );
					it = mDir.erase( it );
				}else{
					++it;
				}
			}
		}
		
		//		変数		
		FRGB		mAmbStdColor;	//!< 環境光 色
		FRGB		mAmbHSSkyColor;	//!< 半球光 色(空)
		FRGB		mAmbHSGrdColor;	//!< 半球光 色(地)
		TGL::Vec3	mAmbHSSkyDir;	//!< 半球光 方向
		f32			mAmbHSPower;	//!< 半球光 強さ

		FRGB		mCamColor;		//!< 視線光 色
		f32			mCamPower;		//!< 視線光 強さ [0以下:無効]

		stl::vector<LightDir*>		mDir;	//!< 平行光
	};

	/* ========================================================================= */
	//		cSceneParam::Data													 */
	/* ========================================================================= */
	class Data
	{
	public:
		Data();
		virtual ~Data();
	
		//		定義		
		enum {
			//!	ライトID
			LIGHT_BG = 0,	//!< 背景
			LIGHT_CHR,		//!< キャラクター
			LIGHT_MAX,

			//!	フォグID
			FOG_BG = 0,		//!< 背景
			FOG_BGHOR,		//!< 背景地平
			FOG_CHR,		//!< キャラクター
			FOG_MAX,
		};
		
		//!	フォグパラメータ
		typedef struct {
			f32		mNear;		//!< ニア [cm]
			f32		mFar;		//!< ファー [cm]
			FRGB	mColor;		//!< 色
		} FOG_T;

		//		関数		
		//!	クリア
		void	clear() {
			for( u8 i=0 ; i<LIGHT_MAX ; i++ )
				mLight[i].clear();
		}
		//!	パラメータ反映
		void	apply( u8 uAmbLights[LIGHT_MAX] );
		
		//!	ライト リソースヘッダ取得
		static const char*	getLightJsonHead( u8 uIdx );
		//!	フォグ リソースヘッダ取得
		static const char*	getFogJsonHead( u8 uIdx );
		
		//		変数		
		Light	mLight[LIGHT_MAX];		//!< ライト [背景, キャラ]
		FOG_T	mFog[FOG_MAX];			//!< フォグ [基本, 地平, キャラ]
		f32		mBloom_Threshold;		//!< ブルーム 閾値
		f32		mBloom_Dispersion;		//!< ブルーム ガウス分散
		f32		mBloom_BlendDensityMin;	//!< ブルーム 合成濃度 最小
		f32		mBloom_BlendDensityMax;	//!< ブルーム 合成濃度 最大
	};

	/* ========================================================================= */
	//		cSceneParam::RimLight												 */
	/* ========================================================================= */
	class RimLight
	{
	public:
		//!	コンストラクタ
		RimLight() {
			reset();
		}
		//!	デストラクタ
		virtual ~RimLight() {}
		
		//!	初期化
		void	reset() {
			FRGB_Set( &mColor, 0, 0, 0 );
			mKr		= 0;
			mPower	= 0;
		}
		
		//!	リソースからパラメータ設定
		void	setupFromJson( TGL::Json::object& work );
		
		//!	有効か
		bool	isEnable() const	{ return (FABS(mColor.fR) + FABS(mColor.fG) + FABS(mColor.fB)) > DELTA; }
		
		//		変数		
		FRGB	mColor;		//!< 色
		f32		mKr;		//!< 強さ
		f32		mPower;		//!< 範囲
	};

public:	
	// -----------------------------------------------------------------
	//		定義		
	// -----------------------------------------------------------------
	enum {
		POINTLIGHT_BG_NUM	= 2,	//!< 扱う点光源数
		POINTLIGHT_CHR_NUM	= 1,	//!< 扱う点光源数
	};

	// -----------------------------------------------------------------
	//		関数		
	// -----------------------------------------------------------------
	//!	初期化
	void	reset();
	//!	セットアップ
	void	setup();
	//!	更新
	void	update( f32 fDelta );
	//!	反映
	void	apply();
	//!	カレントデータを反映
	void	applyCurrentData(){
		if( getCurrentData() )
			getCurrentData()->apply( mAmbLightType );
	}

	//!	指定Jsonからパラメータをロード
	void	getFromJson( const stl::string& strPath, const stl::string& strObj="-" );
	//!	現在のパラメータをファイル出力
	void	save();

	//!	データ取得
	Data*	getData( u8 uId )	{
		if( uId >= mData.size() )	return NULL;
		return mData.at( uId );
	}

	//!	カレントデータ取得
	Data*	getCurrentData()	{ return getData(mCurrentId); }
	//!	補間済データを取得
	void	culcBlendData( Data& Ret );

	//!	カレントデータID取得
	u8		getCurrentId() const	{ return mCurrentId; }
	u8		getPrevId() const		{ return mPrevId; }
	//!	カレントデータ設定
	void	setCurrentId( u8 uId ){
		mPrevId		= mCurrentId;
		mCurrentId	= uId;
		mBlendRate.clear( 1 );
	}
	//!	カレントデータ補間設定
	void	setCurrentId( u8 uId, u32 uFrame, u8 uCurve=TGL::TransValue::CURVE_LINEAR ){
		culcBlendData( mDataPrev );	//!< 補間用に現在値を記憶
		mPrevId		= mCurrentId;
		mCurrentId	= uId;
		mBlendRate.init( TGL::TransValue::MODE_NORMAL, (f32)uFrame, uCurve, 999, 0.0f, 1.0f );
	}

	//!	データ削除
	void	eraseData( Data* p0 ){
		for( stl::vector<Data*>::iterator it=mData.begin() ; it!=mData.end() ; ){
			if( p0 == *it ){
				SAFE_DELETE( *it );
				it = mData.erase( it );
			}else{
				++it;
			}
		}
	}
	//!	データ削除
	void	eraseData( u8 i0 )	{ eraseData( mData.at(i0) ); }

	//!	点光源反映
	void	applyPointLight();
	//!	点光源取得
	LightPoint*	getPointLight( u8 idx ) {
		if( idx >= mPointLight.size() )	return NULL;
		return mPointLight[idx];
	}
	//!	点光源削除
	void	erasePointLight( LightPoint* p0 ){
		for( stl::vector<LightPoint*>::iterator it=mPointLight.begin() ; it!=mPointLight.end() ; ){
			if( p0 == *it ){
				SAFE_DELETE( *it );
				it = mPointLight.erase( it );
			}else{
				++it;
			}
		}
	}
	//!	点光源削除
	void	erasePointLight( u8 i0 )	{ erasePointLight( mPointLight.at(i0) ); }

	//		アクセサ	
	tBG*				getOwner()				{ return mpOwner; }
	bool				isEnable() const		{ return mbEnable; }
	void				setEnable( bool b0 )	{ mbEnable = b0; }
	u8					getAmbLightType( u8 idx ) const	{ return mAmbLightType[idx]; }
	u8					getDirLightNum( u8 idx ) const	{ return mDirLightNum[idx]; }
	u8					getPointLightNum( u8 idx ) const	{ return mPointLightNum[idx]; }
	TGL::TransFloat&	getBlendTime()	{ return mBlendRate; }	//!< 補間レート
	const stl::string&	getEnvTex() const							{ return mEnvTex; }
	void					setEnvTex( const stl::string& str )	{ mEnvTex = str; }
	RimLight&			getRimLight()	{ return mRimLight; }
	bool				isClearSyncFog() const		{ return mbClearSyncFog; }
	void				setClearSyncFog( bool b0 )	{ mbClearSyncFog = b0; }

private:
	// -----------------------------------------------------------------
	//		定義		
	// -----------------------------------------------------------------

	// -----------------------------------------------------------------
	//		関数		
	// -----------------------------------------------------------------

	// -----------------------------------------------------------------
	//		変数		
	// -----------------------------------------------------------------
	tBG*					mpOwner;	//!< オーナー
	bool					mbEnable;	//!< 有効かどうか
	stl::string		mResPath;	//!< リソースパス
	u8						mAmbLightType[Data::LIGHT_MAX];		//!< 環境光タイプ [TGL::LightGroup::AMBTYPE_????]
	u8						mDirLightNum[Data::LIGHT_MAX];		//!< 平行光数
	u8						mPointLightNum[Data::LIGHT_MAX];	//!< 点光源数
	stl::vector<Data*>	mData;		//!< データ配列
	u8						mCurrentId;	//!< カレントID
	u8						mPrevId;	//!< 前回のID
	Data					mDataPrev;	//!< 補間用データ
	Data					mDataBlend;	//!< 補間用テンポラリ
	TGL::TransFloat			mBlendRate;	//!< 補間レート
	stl::vector<LightPoint*>	mPointLight;	//!< 点光源配列
	stl::string		mEnvTex;	//!< 環境マップテクスチャ名
	RimLight				mRimLight;	//!< リムライトパラメータ
	bool					mbClearSyncFog;	//!< 背景色とフォグ色の同期
};

/* ========================================================================= */
/*		EOF																	 */
/* ========================================================================= */
