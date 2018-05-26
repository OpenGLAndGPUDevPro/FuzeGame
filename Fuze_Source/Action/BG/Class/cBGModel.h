// -------------------------------------------------------------------------- //
//! @file   cBGModel.h
//! @brief  背景モデル
//! @author Nal
//! @since  11/11/08(火)
//!
//! COPYRIGHT (C) 2011 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
#pragma	once
/* ====< インクルード >====================================================== */
#include <Graphics/Model/cTGLModel.h>

/* ========================================================================= */
//		cBGModel															 */
/* ========================================================================= */
class cBGModel : public TGL::Model
{
	TGL_NON_COPYABLE(cBGModel);	// コピー禁止
	TGL_DECLARE_SUPER(Model);	// 親
public:
	cBGModel( const TGL::ModelData* pData, u8 uType=TYPE_NORMAL, u32 uFlag=0 );
	virtual ~cBGModel();

public:	
	// -----------------------------------------------------------------
	//		定義		
	// -----------------------------------------------------------------
	enum {
		//!	タイプ
		TYPE_NORMAL = 0,	//!< 通常
		TYPE_SKYDOME,		//!< 天球（カメラに座標追従）
		TYPE_SKYDOME_Y,		//!< 天球（カメラに座標完全追従）
		TYPE_LOOP_Z,		//!< Z方向ループ

		//!	フラグ
		FLAG_FAROBJ				= (1<< 0),	//!< 遠景オブジェクト。描画時にNear, Farを高く設定する
		FLAG_UPDATE_HIERARCHY	= (1<< 1),	//!< 更新時に階層更新（ロケータがセットアップ時しか更新されないため）
		FLAG_INSTANCING			= (1<< 2),	//!< インスタンシング
		
		//!	シーン
		SCENE_00		= (1<< 0),
		SCENE_01		= (1<< 1),
		SCENE_02		= (1<< 2),
		SCENE_03		= (1<< 3),
		SCENE_04		= (1<< 4),
		SCENE_05		= (1<< 5),
		SCENE_06		= (1<< 6),
		SCENE_07		= (1<< 7),
		SCENE_08		= (1<< 8),
		SCENE_09		= (1<< 9),
		SCENE_10		= (1<<10),
		SCENE_11		= (1<<11),
		SCENE_12		= (1<<12),
		SCENE_13		= (1<<13),
		SCENE_14		= (1<<14),
		SCENE_15		= (1<<15),
		SCENE_16		= (1<<16),
		SCENE_17		= (1<<17),
		SCENE_18		= (1<<18),
		SCENE_19		= (1<<19),
		SCENE_20		= (1<<20),
		SCENE_21		= (1<<21),
		SCENE_22		= (1<<22),
		SCENE_23		= (1<<23),
		SCENE_24		= (1<<24),
		SCENE_25		= (1<<25),
		SCENE_26		= (1<<26),
		SCENE_27		= (1<<27),
		SCENE_28		= (1<<28),
		SCENE_29		= (1<<29),
		SCENE_30		= (1<<30),
		SCENE_31		= (1<<31),
	};

	// -----------------------------------------------------------------
	//		関数		
	// -----------------------------------------------------------------
	//!	モーション追加
	bool	addMotion( u16 uMotId, const stl::string& strMotName );

	//!	更新
	void	update( f32 fDelta );
	virtual void Update( const TGL::Model::MODEL_MATRIX_ARRAY& matArray );
	//!	描画
	virtual void Render( TGL::nModel::RENDER_PASS iPass=TGL::nModel::PASS_COLOR,bool renderAlphaModel = true)	{ Render( *GetWorldMatrix(),iPass,renderAlphaModel ); }
	virtual void Render( const TGL::Matrix& mtxWorld,TGL::nModel::RENDER_PASS iPass,bool renderAlphaModel = true );

	//!	動作状態
	bool	isActive() const		{ return mbActive; }
	void	setActive( bool b0 )	{ mbActive = b0; }

	//!	タイプ TYPE_????
	u8		getType() const			{ return mType; }

	//!	フラグ FLAG_????
	u32		tstFlag( u32 i0 ) const	{ return tstFLG(mFlag,i0); }
	void	orFlag( u32 i0 )		{ orFLG(mFlag,i0); }
	void	clrFlag( u32 i0 )		{ clrFLG(mFlag,i0); }

	//!	更新シーン SCENE_????
	u32		getOnScene() const			{ return mOnScene; }
	u32		tstOnScene( u32 i0 ) const	{ return tstFLG(mOnScene,i0); }
	void	setOnScene( u32 i0 )		{ mOnScene = i0; }
	void	orOnScene( u32 i0 )			{ orFLG(mOnScene,i0); }
	void	clrOnScene( u32 i0 )		{ clrFLG(mOnScene,i0); }

	//!	座標
	const TGL::Vec3&	getPos() const			{ return mPos; }
	void		setPos( const TGL::Vec3& v0 )	{ mPos = v0; }
	//!	回転
	const TGL::Vec3&	getRot() const			{ return mRot; }
	void		setRot( const TGL::Vec3& v0 )	{ mRot = v0; }

	//!	モーション
	TGL::AnimationController*		getAnim()				{ return mpAnim; }
	const TGL::AnimationController*	getAnim_const() const	{ return mpAnim; }
	void							setAnim( TGL::AnimationController* p0 )	{ mpAnim = p0; }

protected:
	// -----------------------------------------------------------------
	//		定義		
	// -----------------------------------------------------------------

	// -----------------------------------------------------------------
	//		関数		
	// -----------------------------------------------------------------

	// -----------------------------------------------------------------
	//		変数		
	// -----------------------------------------------------------------
	bool	mbActive;	//!< 動作状態
	u8		mType;		//!< タイプ TYPE_????
	u32		mFlag;		//!< フラグ FLAG_????
	u32		mOnScene;	//!< 更新シーン SCENE_????
	TGL::Vec3		mPos;		//!< 座標
	TGL::Vec3		mRot;		//!< 回転
	TGL::AnimationController*	mpAnim;	//!< モーション
};

/* ========================================================================= */
/*		EOF																	 */
/* ========================================================================= */
