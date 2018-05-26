// -------------------------------------------------------------------------- //
//! @file   cModelSet.h
//! @brief  モデルセット
//! @author Nal
//! @since  12/04/06(金)
//!
//! COPYRIGHT (C) 2012 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
#pragma	once
/* ====< インクルード >====================================================== */

/* ========================================================================= */
//		cModelSet															 */
/* ========================================================================= */
class cModelSet
{
	friend class BGViewer;
	friend class DMModel;
public:
	//!	コンストラクタ
	cModelSet(){
		mbResAdd		= false;
		mId				= 0;
		mbEnable		= true;
		mOnScene		= 0xFFFFFFFF;
		mName			= stl::nullstr();
		mModelId		= 0;
		mPos			= mPosOfs = TGL::Vec3::ZERO;
		mSortPos		= TGL::Vec3::ZERO;
		mRot			= TGL::Vec3::ZERO;
		mRotType		= ROTTYPE_NORMAL;
		mScale			= TGL::Vec3( 1, 1, 1 );
		mCullingDist	= -1;
		mLoop			= 1;
		mLoopPos		= TGL::Vec3::ZERO;
		mLoopRot		= TGL::Vec3::ZERO;
	}
	//!	デストラクタ
	virtual ~cModelSet()	{}

public:	
	// -----------------------------------------------------------------
	//		定義		
	// -----------------------------------------------------------------
	enum {
		//!	回転タイプ
		ROTTYPE_NORMAL = 0,	//!< 通常
		ROTTYPE_BBD_Y,		//!< Y軸ビルボード
	};

	// -----------------------------------------------------------------
	//		関数		
	// -----------------------------------------------------------------
	//!	ワールドマトリクスを取得
	TGL::Matrix		getWorldMtx( TGL::BaseCamera* pCam=NULL ) const {
		TGL::Matrix	mtxWorld = TGL::Matrix::UNIT;
		mtxWorld.setW( getPos() );
		culcWorldMtxRoll( mtxWorld, pCam );
		return mtxWorld;
	}

	//!	ワールドマトリクスを取得（ループ対応）
	TGL::Matrix		getWorldMtx( u8 uLoop, TGL::BaseCamera* pCam=NULL ) const {
		TGL::Matrix	mtxWorld = TGL::Matrix::UNIT;
		mtxWorld.setW( getPos() + (getLoopPos()*(f32)uLoop) );
		culcWorldMtxRoll( mtxWorld, pCam );
		return mtxWorld;
	}
	
	//!	カリング判定
	bool	isCulling( TGL::BaseCamera* pCam, const TGL::Vec3& vPos ) const {
		if( mCullingDist < 0 )	return false;
		return pCam->getPos().distance(vPos) >= mCullingDist;
	}
	
	//		アクセサ	
	bool				isResAdd() const					{ return mbResAdd; }
	void				setResAdd( bool b0 ) 				{ mbResAdd = b0; }
	u8					getId() const						{ return mId; }
	void				setId( u8 i0 ) 						{ mId = i0; }
	void				setEnable( bool b0 ) 				{ mbEnable = b0; }
	u32					getOnScene() const					{ return mOnScene; }
	u32					tstOnScene( u32 i0 ) const			{ return mbEnable ? tstFLG(mOnScene, i0) : 0; }
	void				setOnScene( u32 i0 ) 				{ mOnScene = i0; }
	const stl::string&	getName() const						{ return mName; }
	void				setName( const stl::string& str )	{ mName = str; }
	u8					getModelId() const					{ return mModelId; }
	void				setModelId( u8 i0 ) 				{ mModelId = i0; }
	TGL::Vec3			getPos() const						{ return mPos + mPosOfs; }
	void				setPos( const TGL::Vec3& v0 )		{ mPos = v0; }
	const TGL::Vec3&	getPosOfs() const					{ return mPosOfs; }
	void				setPosOfs( const TGL::Vec3& v0 )	{ mPosOfs = v0; }
	const TGL::Vec3&	getSortPos() const					{ return mSortPos; }
	void				setSortPos( const TGL::Vec3& v0 )	{ mSortPos = v0; }
	const TGL::Vec3&	getRot() const						{ return mRot; }
	void				setRot( const TGL::Vec3& v0 )		{ mRot = v0; }
	const u8			getRotType() const					{ return mRotType; }
	void				setRotType( u8 i0 ) 				{ mRotType = i0; }
	const TGL::Vec3&	getScale() const					{ return mScale; }
	void				setScale( const TGL::Vec3& v0 )		{ mScale = v0; }
	f32					getCullingDist() const				{ return mCullingDist; }
	void				setCullingDist( f32 f0 ) 			{ mCullingDist = f0; }
	const u8			getLoop() const						{ return mLoop; }
	void				setLoop( u8 i0 ) 					{ mLoop = i0; }
	const TGL::Vec3&	getLoopPos() const					{ return mLoopPos; }
	void				setLoopPos( const TGL::Vec3& v0 )	{ mLoopPos = v0; }
	const TGL::Vec3&	getLoopRot() const					{ return mLoopRot; }
	void				setLoopRot( const TGL::Vec3& v0 )	{ mLoopRot = v0; }
	TGL::MtxGuide&		getAttach()							{ return mAttach; }
	
private:
	// -----------------------------------------------------------------
	//		定義		
	// -----------------------------------------------------------------

	// -----------------------------------------------------------------
	//		関数		
	// -----------------------------------------------------------------
	//!	回転行列計算
	void	culcWorldMtxRoll( TGL::Matrix& mtx, TGL::BaseCamera* pCam ) const {
		if( !pCam || mRotType == ROTTYPE_NORMAL ){
			//!	通常
			mtx.rollXYZ( getRot() );
		}else{
			//!	特殊
			TGL::Vec3	v0, v1;
			switch( mRotType ){
				case ROTTYPE_BBD_Y:	//!	Y軸ビルボード
					v0 = mtx.getW();
					v1 = pCam->getPos();
					v0.y = v1.y = 0;
					v0 = v0.wayNL( v1 );
					mtx.setZ( v0 );
					v0 = v0.cross( TGL::Vec3::Y );
					mtx.setX( v0 );
					mtx.rollXYZ( getRot() );
					break;
			}
		}
	}

	// -----------------------------------------------------------------
	//		変数		
	// -----------------------------------------------------------------
	bool			mbResAdd;		//!< リソースからの追加
	u8				mId;			//!< 任意のID
	bool			mbEnable;		//!< 有効
	u32				mOnScene;		//!< 有効シーン（基本的に0xFFFFFFFFで、モデル本体と併せて使用）
	stl::string		mName;			//!< モデル名
	u8				mModelId;		//!< モデルID（同じ名前のモデルが複数ある場合に使用）
	TGL::Vec3		mPos;			//!< 座標
	TGL::Vec3		mPosOfs;		//!< 相対座標
	TGL::Vec3		mSortPos;		//!< ソート比較用座標
	TGL::Vec3		mRot;			//!< 回転 [DEG]
	u8				mRotType;		//!< 回転タイプ [ROTTYPE_????]
	TGL::Vec3		mScale;			//!< スケール
	f32				mCullingDist;	//!< カリング距離 [0未満:無効]
	u8				mLoop;			//!< ループ配置数
	TGL::Vec3		mLoopPos;		//!< ループ配置 座標
	TGL::Vec3		mLoopRot;		//!< ループ配置 回転 [DEG]
	TGL::MtxGuide	mAttach;		//!< 接続モデル
};

/* ========================================================================= */
/*		EOF																	 */
/* ========================================================================= */
