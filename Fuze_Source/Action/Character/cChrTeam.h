// -------------------------------------------------------------------------- //
//! @file   cChrTeam.h
//! @brief  チーム
//! @author Nal
//! @since  11/09/13(火)
//!
//! COPYRIGHT (C) 2011 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
#pragma	once
/* ====< インクルード >====================================================== */
#include "Core/tCharacter.h"

/* ========================================================================= */
//		cChrTeam															 */
/* ========================================================================= */

class cChrTeam
{
public:
	cChrTeam( u8 uId=0 );
	virtual ~cChrTeam();

public:	
	// -----------------------------------------------------------------
	//		定義		
	// -----------------------------------------------------------------
	enum {
		FLAG_PLAYER		= (1<< 0),	//!< プレイヤー
		FLAG_ENEMY		= (1<< 1),	//!< 敵
		FLAG_02			= (1<< 2),	//!< 
		FLAG_03			= (1<< 3),	//!< 
		FLAG_04			= (1<< 4),	//!< 
		FLAG_05			= (1<< 5),	//!< 
		FLAG_06			= (1<< 6),	//!< 
		FLAG_07			= (1<< 7),	//!< 
		FLAG_08			= (1<< 8),	//!< 
		FLAG_09			= (1<< 9),	//!< 
		FLAGS_CLRINIT	= 0,
	};
	// -----------------------------------------------------------------
	//		関数		
	// -----------------------------------------------------------------
	//!	初期化
	void	reset();

	//!	キャラクターノードを接続
	u16			connectNode( tCharacter& Chr );
	//!	ルートノードの次のノードを取得
	cChrNode*	getNodeNext()		{ return mNode.getNext(); }
	//!	ルートノードの次のノードのキャラクターを取得
	tCharacter*	getNodeNextChr()	{ return getNodeNext()->getChr(); }
	//!	当たり判定用ルートノードの次のノードを取得
	cChrNode*	getColNodeNext()	{ return mNode.getColNext(); }
	//!	当たり判定用ルートノードの次のノードのキャラクターを取得
	tCharacter*	getColNodeNextChr()	{ return getNodeNext()->getChr(); }
	//!	番号指定でノード内のキャラクター取得
	tCharacter*	getNodeChrFromId( u16 uId=0 );
	//!	ユニークID指定でノード内のキャラクター取得
	tCharacter*	getNodeChrFromUniqueId( u32 uId );
	//!	ノードの接続総数を取得
	u16			getNodeConnectTotal();
	//!	アクティブなキャラクター総数を取得
	u16			getNodeActiveTotal();

	//!	接続キャラ全員のフラグアクセス
	void	chrOrFlag( u8 uFlagId, u32 uFlag );
	void	chrClrFlag( u8 uFlagId, u32 uFlag );
	void	chrXorFlag( u8 uFlagId, u32 uFlag );

	//!	キャラクタータスク追加
	tCharacter* addChr( tCharacter* pChr, u8 uCType, u32 uTot=GTOT_CHR(0) );
	tCharacter*	addChr( u8 uCType, u32 uTot=GTOT_CHR(0) );

	//!	指定タイプのキャラクタータスクを削除
	bool	deleteChr( u8 uCType );
	//!	プレイヤータスクを削除
	bool	deletePlayer();
	//!	タスク削除監視
	void	updateDeleteTask();
	//!	チーム内の全キャラを終了
	void	endChr();

	//!	プレイヤーチームかどうか
	bool	isPlayer() const	{ return (tstFlag(FLAG_PLAYER) != false); }
	//!	敵チームかどうか
	bool	isEnemy() const		{ return (tstFlag(FLAG_ENEMY) != false); }

	//!	敵生成
	tCharacter*	generateEnemy( u8 uType, u16 uEnemyId, const TGL::Vec3& vPos, const TGL::Vec3& vRot, s16 nAnmNo=ANMNO_ENTRY );

	//		アクセサ		
	u8		getId()				const	{ return mId; }
	void	setId( u8 i0 )				{ mId = i0; }
	u32		getFlag()			const	{ return mFlag; }
	void	setFlag( u32 i0 )			{ setFLG( mFlag, i0 ); }
	u32		tstFlag( u32 i0 )	const	{ return tstFLG( mFlag, i0 ); }
	void	orFlag( u32 i0 )			{ orFLG( mFlag, i0 ); }
	void	clrFlag( u32 i0 )			{ clrFLG( mFlag, i0 ); }
	void	xorFlag( u32 i0 )			{ xorFLG( mFlag, i0 ); }
	f32		getHitFrm()			const	{ return mHitFrm; }
	void	setHitFrm( f32 f0 )			{ mHitFrm = f0; }
	void	addHitFrm( f32 f0 )			{ mHitFrm += f0; }

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
	u8			mId;		//!< チームID
	cChrNode	mNode;		//!< ルートノード
	u32			mFlag;		//!< FLAG_????
	f32			mHitFrm;	//!< 与えた瞬間ダメージ
};

/* ========================================================================= */
/*		EOF																	 */
/* ========================================================================= */
