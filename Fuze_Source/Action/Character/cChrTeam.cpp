// -------------------------------------------------------------------------- //
//! @file   cChrTeam.cpp
//! @brief  チーム
//! @author Nal
//! @since  11/09/13(火)
//!
//! COPYRIGHT (C) 2011 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
/* ====< インクルード >====================================================== */
#include "prefix.h"
#include <TGL.h>
#include "Game/Game.h"
#include "Action/ActionTypes.h"

/* ========================================================================= */
/*		cChrTeam															 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  コンストラクタ "cChrTeam"
// ---------------------------------------------
cChrTeam::cChrTeam( u8 uId )
:	mId(uId), mFlag(0)
{
	reset();
}

// ---------------------------------------------
//! @brief  デストラクタ "cChrTeam"
// ---------------------------------------------
cChrTeam::~cChrTeam()
{
	reset();
}

// ---------------------------------------------
//! @brief  初期化
// ---------------------------------------------
void cChrTeam::reset()
{
	//!	キャラクタータスク解放
	for( cChrNode* pNode = getNodeNext(); pNode != NULL;  ){
		cChrNode* pNext = pNode->getNext();	//!< ノード切断される前に次を調べる
		tCharacter*	pChr = pNode->getChr();
		SAFE_DELETE( pChr );
		pNode = pNext;
	}
	mNode.reset();
	
	//!	変数
	clrFLG( mFlag, FLAGS_CLRINIT );
	mHitFrm	= 0;
}

// ---------------------------------------------
//! @brief  キャラクタータスク追加
//! @param  pChr	[i/o]	追加するキャラ
//! @param  uCType	[in]	CTYPE_????
//! @param  uTot	[in]	処理優先
//! @return 生成されたキャラ
// ---------------------------------------------
tCharacter* cChrTeam::addChr( tCharacter* pChr, u8 uCType, u32 uTot/*=GTOT_CHR(0)*/ )
{
	//!	タスク生成
	pChr->mpTeam	= this;
	pChr->mType		= uCType;
	if( !pChr->isPlayer() || IACtrl->getScene().tstSceneF(SCENEF_NOPLAYER) )	//!< プレイヤー以外は非アクティブで開始
		pChr->orChrF( CHRF_NOACTIVE );
	pChr->loadScript( uTot );	//!< スクリプト読み込み
	
	//!	ノード接続
	connectNode( *pChr );
	
	//!	付属キャラクターの生成
	pChr->generateChild();

	return pChr;
}
// ---------------------------------------------
//! @brief  キャラクタータスク追加（内部でインスタンス生成）
//! @param  uCType	[in]	CTYPE_????
//! @param  uTot	[in]	処理優先
//! @return 生成されたキャラ
// ---------------------------------------------
tCharacter* cChrTeam::addChr( u8 uCType, u32 uTot/*=GTOT_CHR(0)*/ )
{
	return addChr( ICharacter->createChrInstance(uCType), uCType, uTot );
}

// ---------------------------------------------
//! @brief  指定タイプのキャラクタータスクを削除
//! @param  uCType	[in]	CTYPE_????
//! @return true:アクティブなキャラがいた
// ---------------------------------------------
bool cChrTeam::deleteChr( u8 uCType )
{
	bool	bRet = false;
	for( cChrNode* pNode = getNodeNext(); pNode != NULL;  ){
		cChrNode* pNext = pNode->getNext();	//!< ノード切断される前に次を調べる
		tCharacter*	pChr = pNode->getChr();
		if( pChr->getType() == uCType ){
			bRet |= pChr->isActive();	//!< アクティブなキャラがいた
			SAFE_DELETE( pChr );
		}
		pNode = pNext;
	}
	return bRet;
}

// ---------------------------------------------
//! @brief  プレイヤータスクを削除
//! @return true:アクティブなキャラがいた
// ---------------------------------------------
bool cChrTeam::deletePlayer()
{
	bool	bRet = false;
	for( cChrNode* pNode = getNodeNext(); pNode != NULL;  ){
		cChrNode* pNext = pNode->getNext();	//!< ノード切断される前に次を調べる
		tCharacter*	pChr = pNode->getChr();
		if( pChr->isPlayerTask() ){
			bRet |= pChr->isActive();	//!< アクティブなキャラがいた
			SAFE_DELETE( pChr );
		}
		pNode = pNext;
	}
	return bRet;
}

// ---------------------------------------------
//! @brief  キャラクタータスク削除監視
// ---------------------------------------------
void cChrTeam::updateDeleteTask()
{
	for( cChrNode* pNode = getNodeNext(); pNode != NULL; ){
		cChrNode*	pNext = pNode->getNext();	//!< ノード切断される前に次を調べる
		tCharacter*	pChr = pNode->getChr();

		switch( pChr->getActiveOrder() ){
			case tCharacter::ACTIVEORDER_SUSPEND:	//!< 休止
				pChr->endActive();
				pChr->setActiveOrder( tCharacter::ACTIVEORDER_NONE );
				break;

			case tCharacter::ACTIVEORDER_END:		//!< 終了
				pChr->endActive();
				pChr->setActiveOrder( tCharacter::ACTIVEORDER_ENDWAIT );
				break;

			case tCharacter::ACTIVEORDER_ENDWAIT:	//!< 終了待ち
				//!	タスク削除
				SAFE_DELETE( pChr );
				break;
		}

		pNode = pNext;
	}
}

// ---------------------------------------------
//! @brief  チーム内の全キャラを終了
// ---------------------------------------------
void cChrTeam::endChr()
{
	for( cChrNode* pNode = getNodeNext(); pNode != NULL; pNode=pNode->getNext() )
		pNode->getChr()->setActiveOrder( tCharacter::ACTIVEORDER_END );
}

/* ========================================================================= */
//!		ノード																 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  キャラクターノードを接続
//! @note   チーム内IDの割り振りも行う
//! @param  Chr		[i/o]	キャラクター
//! @return 接続ルート内でのノード番号
// ---------------------------------------------
u16 cChrTeam::connectNode( tCharacter& Chr )
{
	u16	uNodeId = 0;
	//!	現在の最後尾を探す
	cChrNode*	pNode = &mNode;
	while( pNode->getNext() != NULL ){
		pNode = pNode->getNext();
		uNodeId++;
	}
	
	Chr.getNode().setChr( &Chr );
	//!	前後接続
	Chr.getNode().setPrev( pNode );
	pNode->setNext( &Chr.getNode() );
	//!	親子接続
/*	if( Chr.isChild() ){
		ASSERT( Chr.getParentChrInfo().getChr() && "connectNode: Parent is Null" );
		//!	子供の末端を検索
		cChrNode*	pNodeParent = &Chr.getParentChrInfo().getChr()->getNode();	//!< 一番親
		while( pNodeParent->getChild() ){
			pNodeParent	= pNodeParent->getChild();
		}
		//!	親子接続
		Chr.getNode().setParent( pNodeParent );
		pNodeParent->setChild( &Chr.getNode() );
		//!	チーム内IDの割り振り
		Chr.setDefMemberId( Chr.getParentChrInfo().getChr()->getDefMemberId() );
		
	}else{
*/		//!	親前後接続
		cChrNode*	pNodeP = pNode;
		//!	親検索
		while( pNodeP->getPrev() != NULL ){
			if( !pNodeP->getChr() )				break;	//!< 一番上
			if( !pNodeP->getChr()->isChild() )	break;	//!< 親発見
			pNodeP = pNodeP->getPrev();
		}
		Chr.getNode().setMainPrev( pNodeP );
		pNodeP->setMainNext( &Chr.getNode() );
		//!	チーム内IDの割り振り
//		if( pNodeP->getChr() )	Chr.setDefMemberId( pNodeP->getChr()->getMemberId()+1 );
//		else					Chr.setDefMemberId( 0 );
//	}
	
	//!	チーム内IDの割り振り
//	Chr.setMemberId( Chr.getDefMemberId() );

	//!	当たり判定用ノード
	if( Chr.isCollisionNode() ){
		cChrNode*	pNodeP = &mNode;
		while( pNodeP->getColNext() != NULL )
			pNodeP = pNodeP->getColNext();
		Chr.getNode().setColPrev( pNodeP );
		pNodeP->setColNext( &Chr.getNode() );
	}
	
	return uNodeId;
}

// ---------------------------------------------
//! @brief  番号指定でノード内のキャラクター取得
//! @param  uId	[in]	番号
//! @return tCharacter*
// ---------------------------------------------
tCharacter* cChrTeam::getNodeChrFromId( u16 uId/*=0*/ )
{
	cChrNode*	pNode = mNode.getNext();
	u16	uRet = 0;
	while( pNode != NULL ){
		if( uRet == uId )	return pNode->getChr();
		uRet++;
		pNode = pNode->getNext();
	}
	return NULL;
}

// ---------------------------------------------
//! @brief  ユニークID指定でノード内のキャラクター取得
//! @param  uId	[in]	番号
//! @return tCharacter*
// ---------------------------------------------
tCharacter* cChrTeam::getNodeChrFromUniqueId( u32 uId )
{
	for( cChrNode* pNode = getNodeNext(); pNode != NULL; pNode=pNode->getNext() ){
		if( uId == pNode->getChr()->getId() )	return pNode->getChr();
	}
	return NULL;
}

// ---------------------------------------------
//! @brief  ノードの接続総数を取得
//! @return 接続総数
// ---------------------------------------------
u16 cChrTeam::getNodeConnectTotal()
{
	cChrNode*	pNode = &mNode;
	u16	uRet = 0;
	while( pNode->getNext() != NULL ){
		uRet++;
		pNode = pNode->getNext();
	}
	return uRet;
}

// ---------------------------------------------
//! @brief  アクティブなキャラクター総数を取得
//! @return 接続総数
// ---------------------------------------------
u16 cChrTeam::getNodeActiveTotal()
{
	u16	uRet = 0;
	for( cChrNode* pNode = getNodeNext(); pNode != NULL; pNode=pNode->getNext() ){
		if( pNode->getChr()->isActive() )
			uRet++;
	}
	return uRet;
}

// ---------------------------------------------
//! @brief  接続キャラ全員のフラグアクセス or
//! @param  uFlagId	[in]	FLAGID_????
//! @param  uFlag	[in]	フラグ
// ---------------------------------------------
void cChrTeam::chrOrFlag( u8 uFlagId, u32 uFlag )
{
	for( cChrNode* pNode = getNodeNext(); pNode != NULL; pNode=pNode->getNext() )
		pNode->getChr()->orFlagFromId( uFlagId, uFlag );
}
// ---------------------------------------------
//! @brief  接続キャラ全員のフラグアクセス clr
//! @param  uFlagId	[in]	FLAGID_????
//! @param  uFlag	[in]	フラグ
// ---------------------------------------------
void cChrTeam::chrClrFlag( u8 uFlagId, u32 uFlag )
{
	for( cChrNode* pNode = getNodeNext(); pNode != NULL; pNode=pNode->getNext() )
		pNode->getChr()->clrFlagFromId( uFlagId, uFlag );
}
// ---------------------------------------------
//! @brief  接続キャラ全員のフラグアクセス xor
//! @param  uFlagId	[in]	FLAGID_????
//! @param  uFlag	[in]	フラグ
// ---------------------------------------------
void cChrTeam::chrXorFlag( u8 uFlagId, u32 uFlag )
{
	for( cChrNode* pNode = getNodeNext(); pNode != NULL; pNode=pNode->getNext() )
		pNode->getChr()->xorFlagFromId( uFlagId, uFlag );
}

/* ========================================================================= */
//!																			 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  敵生成
//! @param  nType		[in]	CTYPE_????
//! @param  uEnemyId	[in]	敵ID
//! @param  vPos		[in]	座標
//! @param  vRot		[in]	向き
//! @param  nAnmNo		[in]	初期アニメ番号
//! @return 生成されたキャラクター
// ---------------------------------------------
tCharacter*	cChrTeam::generateEnemy( u8 uType, u16 uEnemyId, const TGL::Vec3& vPos, const TGL::Vec3& vRot, s16 nAnmNo/*=ANMNO_ENTRY*/ )
{
	tCharacter*	pChr = addChr( uType );
	pChr->generateEnemy( uEnemyId, vPos, vRot, nAnmNo );
	return pChr;
}

/* ========================================================================= */
/*		EOF																	 */
/* ========================================================================= */
