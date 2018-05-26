// -------------------------------------------------------------------------- //
//! @file   cBGSceneChange.cpp
//! @brief  シーン遷移
//! @author Nal
//! @since  13/09/19(木)
//!
//! COPYRIGHT (C) 2013 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
/* ====< インクルード >====================================================== */
#include "prefix.h"
#include <TGL.h>
#include "cBGSceneChange.h"
#include "Action/ActionTypes.h"

/* ========================================================================= */
/*		BGSceneChange														 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  コンストラクタ "BGSceneChange"
// ---------------------------------------------
BGSceneChange::BGSceneChange()
{
}

// ---------------------------------------------
//! @brief  デストラクタ "BGSceneChange"
// ---------------------------------------------
BGSceneChange::~BGSceneChange()
{
}

// ---------------------------------------------
//! @brief  更新
//! @param  Bg	[in]	背景タスク
// ---------------------------------------------
void BGSceneChange::update( tBG& Bg )
{
	if( IACtrl->getScene().getStep() == SCENESTEP_RESULT )	return;
	
	TGL::PathCamera*	pCam = (TGL::PathCamera*)( ICamera->getCamera(0) );
	if( !pCam || !pCam->isPlay() )	return;
	
	stl::map<u16, cSet>::iterator	it = mSet.find(pCam->getCurrentGroupId());
	if( it != mSet.end() )
		it->second.update( Bg, pCam );
}

// ---------------------------------------------
//! @brief  更新
//! @param  Bg		[in]	背景タスク
//! @param  pCam	[in]	背景カメラ
// ---------------------------------------------
void BGSceneChange::cSet::update( tBG& Bg, TGL::PathCamera* pCam )
{
	//!	シーン
	foreach( stl::vector<cSceneTable>, mScene, it ){
		if( it == mScene.end()-1 || pCam->getTime() < (*it).mTiming ){
			Bg.setScene( (*it).mScene );
			break;
		}
	}
	
	//!	シーンパラメータ
	foreach( stl::vector<cParamTable>, mParam, it ){
		if( it == mParam.end()-1 || pCam->getTime() < (*it).mTiming ){
			if( Bg.getSceneParam().getCurrentId() == (*it).mParam )	break;	//!< 変更無し
			
			if( pCam->getTime() >= pCam->getTimeOld() )	//!< 順再生
				Bg.getSceneParam().setCurrentId( (*it).mParam, (*it).mTransTime );
			else	//!< 逆再生
				Bg.getSceneParam().setCurrentId( (*it).mParam );
			break;
		}
	}
}

/* ========================================================================= */
/*		EOF																	 */
/* ========================================================================= */
