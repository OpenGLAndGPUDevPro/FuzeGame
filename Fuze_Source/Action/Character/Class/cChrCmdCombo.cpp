// -------------------------------------------------------------------------- //
//! @file   cChrCmdCombo.cpp
//! @brief  コマンド入力 コンボ
//! @author Nal
//! @since  12/04/10(火)
//!
//! COPYRIGHT (C) 2012 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
/* ====< インクルード >====================================================== */
#include "prefix.h"
#include <TGL.h>
#include "Game/Game.h"
#include "cChrCmdCombo.h"
#include "cChrCmdTypes.h"

/* ========================================================================= */
/*		cChrCmdCombo														 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  コンストラクタ "cChrCmdCombo"
//! @param  pChr	[in]	オーナー
// ---------------------------------------------
cChrCmdCombo::cChrCmdCombo( tCharacter* pChr )
:	cChrCmd(pChr), mpScriptTable(NULL)
{
}

// ---------------------------------------------
//! @brief  デストラクタ "cChrCmdCombo"
// ---------------------------------------------
cChrCmdCombo::~cChrCmdCombo()
{
	foreach( stl::vector<Line*>, mLine, it )
		SAFE_DELETE( *it );
}

// ---------------------------------------------
//! @brief  セットアップ
//! @param  pScriptTable	[in]	スクリプトテーブル
// ---------------------------------------------
void cChrCmdCombo::setup( const SCRIPTTBL_T* pScriptTable )
{
	mpScriptTable = pScriptTable;

	//!	ライン確保
	foreach( stl::vector<Line*>, mLine, it )
		SAFE_DELETE( *it );
	mLine.clear();
	
	for( u8 i=0 ;; i++ ){	//!< テーブル数調査
		if( mpScriptTable[i].mCmdLable == 0xFFFFFFFF )	break;
		mLine.push_back( APP_NEW Line(&mpChr->getScript()->mTgs, &mpScriptTable[i]) );
	}
}

// ---------------------------------------------
//! @brief  クリア
// ---------------------------------------------
void cChrCmdCombo::clear()
{
	cChrCmd::clear();
	mpSuccessTable	= NULL;
	mCheckStep		= 0;

	foreach( stl::vector<Line*>, mLine, it )
		(*it)->clear();
}

// ---------------------------------------------
//! @brief  更新
// ---------------------------------------------
void cChrCmdCombo::update()
{
	cChrCmd::update();

	//!	ライン更新
	foreach( stl::vector<Line*>, mLine, it )
		(*it)->update( *this, *mpChr );
}

/* ========================================================================= */
/*		cChrCmdCombo::Line													 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  コンストラクタ "Line"
//! @param  pTgs	[in]	スクリプト
//! @param  pTable	[in]	スクリプトテーブル
// ---------------------------------------------
cChrCmdCombo::Line::Line( const TGS_T* pTgs, const SCRIPTTBL_T* pTable )
:	cChrCmd::Line()
{
	mpScriptTable	= pTable;
	mpAnmList		= NULL;
	mpCmdStart		= TGS_Label( pTgs, mpScriptTable->mCmdLable );
	clear();
}

// ---------------------------------------------
//! @brief  クリア
// ---------------------------------------------
void cChrCmdCombo::Line::clear()
{
	cChrCmd::Line::clear();
	mpAnmList	= NULL;
}

// ---------------------------------------------
//! @brief  更新
//! @param  Parent	[i/o]	親
//! @param  Chr		[in]	キャラクター
// ---------------------------------------------
void cChrCmdCombo::Line::update( cChrCmd& Parent, const tCharacter& Chr )
{
	bool	bFlag;
	
	xu32*	pc = (xu32*)mpCmd;
	while( pc ){
		u8	nCmd = TGS_PC_I(pc);
		switch( nCmd ){
			// -------------------------------
			case CHRCMD_SETCOMBOATK:	//!< 通常コンボセット	
				Parent.setSuccessId( TGS_PC_I(pc,1) );
				Parent.setSuccessPad( Chr.getInput_C().getDat() );
				Parent.setSuccessLever( Chr.getInput_C().getLever() );
				((cChrCmdCombo*)(&Parent))->setSuccessTable( mpScriptTable );
				Parent.clearLimit();
				pc = TGS_AddPC(pc,2);
				break;

			// -------------------------------
			case CHRCMD_SETCOMBOTHROW:	//!< 投げコンボセット	
//				Parent.setSuccessId( TGS_PC_I(pc,1) );
//				Parent.setSuccessTable( mpScriptTable );
				Parent.clearLimit();
				pc = TGS_AddPC(pc,3);
				break;

			// -------------------------------
			case CHRCMD_CHKCOMBOLIST:	//!< 入力有効なアニメ番号のリスト	
			{
				mpAnmList = (s32*)pc;
				s8	uMax = (u8)*(mpAnmList + 1);
				bFlag = false;
				for( u8 i=0 ; i<uMax ; i++ ){
					if( Chr.getAnmNo() == *(mpAnmList+2+i) ){
						//! 繋がる
						//mpChr->orPrgF( PRGF_COMBO2 );
						//pChr->combo[i].nAnmNo = mpChr->getAnmNo();
						bFlag = true;
						pc = TGS_AddPC(pc, uMax + 2);
						break;
					}
				}
				if( !bFlag ){	//!< 繋がらない
					mpCmd = pc;
					pc = NULL;
				}
			}	break;

			// -------------------------------
			default:	//!< 親クラスコマンド	
				pc = execCmd( pc, Parent, Chr );
			  	break;
		}
	}
}

// ---------------------------------------------
//! @brief  入力を受け付けるかどうか
//! @param  Chr	[in]	キャラクター
//! @return true:受け付ける
// ---------------------------------------------
bool cChrCmdCombo::Line::isInputReceive( const tCharacter& Chr )
{
	//!	入力を受け付けるアニメ番号か判定
	if( !mpAnmList )	return true;
	bool	bFlag = false;
	u8	uMax = *(mpAnmList + 1);
	for( u8 i=0 ; i<uMax ; i++ ){
		if( Chr.getAnmNo() != *(mpAnmList+2+i) )	continue;
		bFlag = true;
		break;
	}
	if( !bFlag )	return false;	//!< アニメ番号が違う

	//!	パッド受付時間待ち
	if( mpScriptTable->mReceive != -1 ){
		if( (mpScriptTable->mReceive-1) > (s32)Chr.getAnmTime() )	/* -1 : 更新タイミング（EVALで更新してる）に誤差があるための処置 */
			return false;
	}
	
	return true;
}

/* ========================================================================= */
/*		遷移処理															 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  コンボが出せるかどうか判定する
//! @return true:出せる
// ---------------------------------------------
bool cChrCmdCombo::check()
{
	mCheckStep = CHKSTEP_NONE;
	if( !mpChr )	return false;
	if( mSuccessId < 0 || !mpSuccessTable )	return false;
	
	//!	移行条件
	mCheckStep = CHKSTEP_LIMIT;
	if( !checkLimit() )	return false;

	//!	攻撃判定発生
	mCheckStep = CHKSTEP_COLLISION;
	if( !mpChr->getAttack().tstFlag(cAttack::HITF_ENABLE | cAttack::HITF_END | cAttack::HITF_DUMMY) )	return false;
	
	//!	ディレイチェック
	mCheckStep = CHKSTEP_DELAY;
	if( mpSuccessTable->mDelay >= 0 ){
		if( mpChr->getAttack().getCountTime() >= (f32)mpSuccessTable->mDelay )	return false;
	}
	
	//!	キャンセルフレームチェック
	mCheckStep = CHKSTEP_CANCELFRAME;
	if( mpChr->getAttack().getCountTime() < (f32)mpSuccessTable->mCancel )	return false;
	
	//!	多段攻撃の途中チェック
	mCheckStep = CHKSTEP_MULTIATK;
	if( !tstFLG(mpSuccessTable->mFlag,SPATKF_COMBO_NOWAITEND) ){
		if( mpChr->getAttack().getEndTime() < DELTA )	return false;
	}

	mCheckStep = CHKSTEP_SUCCESS;
	return true;
}

// ---------------------------------------------
//! @brief  チェックステップ名
//! @return 文字列
// ---------------------------------------------
const char* cChrCmdCombo::getCheckStepName() const
{
	const char* _NAME[CHKSTEP_MAX] = {
		"None",			//!< 無し
		"Limit",		//!< 移行制限
		"Collision",	//!< 判定発生
		"Delay",		//!< ディレイ
		"CancelFrame",	//!< キャンセルフレーム
		"MultiAttack",	//!< 多段攻撃
		"Success",		//!< 成功
	};
	return _NAME[mCheckStep];
}

/* ========================================================================= */
/*		EOF																	 */
/* ========================================================================= */
