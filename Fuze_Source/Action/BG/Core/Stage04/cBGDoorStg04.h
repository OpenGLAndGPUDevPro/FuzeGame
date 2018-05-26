// -------------------------------------------------------------------------- //
//! @file   cBGDoorStg04.h
//! @brief  ドア
//! @author Nal
//! @since  13/09/14(土)
//!
//! COPYRIGHT (C) 2013 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
#pragma	once
/* ====< インクルード >====================================================== */
#include "../../Class/cBGDoor.h"
#include "Sound/sAppSound.h"

/* ========================================================================= */
//		BGDoorStg04															 */
/* ========================================================================= */
class BGDoorStg04 : public BGDoor
{
public:
	//!	コンストラクタ
	BGDoorStg04( cModelSet* pModelSet, u8 uSceneS, u8 uSceneE, u8 uCamGroup, f32 fCall, f32 fAnmFrame )
	:	BGDoor( pModelSet, uSceneS, uSceneE, uCamGroup, fCall, fAnmFrame )
	{
		reset();
	}
	
	//!	デストラクタ
	virtual	~BGDoorStg04() {}

public:	
	// -----------------------------------------------------------------
	//		関数		
	// -----------------------------------------------------------------

protected:
	// -----------------------------------------------------------------
	//		定義		
	// -----------------------------------------------------------------

	// -----------------------------------------------------------------
	//		関数		
	// -----------------------------------------------------------------
	//!	ドア移動音
	virtual s16	getSELoop() const	{ return SE_S4_DOORMOVE_LOOP; }
	//!	ドア閉まった音
	virtual s16	getSEClose() const	{ return SE_S4_DOORCLOSE; }
	
	//!	カメラ振動判定
	virtual bool	isShake( cBGModel* pModel ) {
		if( BGDoor::isShake(pModel) )
			return true;
		
		if( mAnmFrame > 0.0f ){	//!< 順再生
			if( mAnmOld < 0.5f && pModel->getAnim()->GetCurrentTime() >= 0.5f )
				return true;
		}else{					//!< 逆再生
			if( mAnmOld > 0.5f && pModel->getAnim()->GetCurrentTime() <= 0.5f )
				return true;
		}
		return false;
	}

	// -----------------------------------------------------------------
	//		変数		
	// -----------------------------------------------------------------
};

/* ========================================================================= */
/*		EOF																	 */
/* ========================================================================= */
