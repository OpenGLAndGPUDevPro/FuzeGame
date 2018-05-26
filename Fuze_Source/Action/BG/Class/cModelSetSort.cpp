// -------------------------------------------------------------------------- //
//! @file   cModelSetSort.cpp
//! @brief  シーンパラメータ
//! @author Nal
//! @since  12/12/17(月)
//!
//! COPYRIGHT (C) 2012 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
/* ====< インクルード >====================================================== */
#include "prefix.h"
#include <TGL.h>
#include "cModelSetSort.h"

/* ========================================================================= */
/*		cModelSetSort														 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  コンストラクタ "cModelSetSort"
// ---------------------------------------------
cModelSetSort::cModelSetSort( tBG& owner )
:	mOwner(owner), mpArray(NULL), mBegin(0), mEnd(0), mScene(0), mpSortCB(NULL)
{
}

// ---------------------------------------------
//! @brief  デストラクタ "cModelSetSort"
// ---------------------------------------------
cModelSetSort::~cModelSetSort()
{
}

// ---------------------------------------------
//! @brief  ソート実行
// ---------------------------------------------
void cModelSetSort::sort()
{
	if( !tstFLG(mScene, (1<<mOwner.getScene())) )	return;
	if( !mpArray || !mpSortCB )	return;

	std::sort( mpArray->begin()+mBegin, mpArray->begin()+mEnd, mpSortCB );

#if 0
	foreach( stl::vector<cModelSet*>, *mpArray, it ){
		IDPrint->addSh( "%s, %d\n", (*it)->getName().c_str(), (*it)->getId() );
	}
#endif
}

// ---------------------------------------------
//! @brief  ソート関数 Zソート（遠くから）
//! @param  pA	[in]	A
//! @param  pB	[in]	B
//! @return 正:a<b, 負:a>b
// ---------------------------------------------
bool cModelSetSort::sortCB_ZFar( cModelSet* pA, cModelSet* pB )
{
	TGL::BaseCamera*	pCam = ICamera->getCamera(0);
	f32	fA = pCam->getPos().distance( pA->getSortPos() );
	f32	fB = pCam->getPos().distance( pB->getSortPos() );
	return (fA - fB) > DELTA;
}

// ---------------------------------------------
//! @brief  ソート関数 Zソート（近くから）
//! @param  pA	[in]	A
//! @param  pB	[in]	B
//! @return 正:a<b, 負:a>b
// ---------------------------------------------
bool cModelSetSort::sortCB_ZNear( cModelSet* pA, cModelSet* pB )
{
	TGL::BaseCamera*	pCam = ICamera->getCamera(0);
	f32	fA = pCam->getPos().distance( pA->getSortPos() );
	f32	fB = pCam->getPos().distance( pB->getSortPos() );
	return (fB - fA) > DELTA;
}

/* ========================================================================= */
/*		EOF																	 */
/* ========================================================================= */
