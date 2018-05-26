// -------------------------------------------------------------------------- //
//! @file   cChrCmdSpatk.cpp
//! @brief  コマンド入力 SPATK
//! @author Nal
//! @since  12/04/08(日)
//!
//! COPYRIGHT (C) 2012 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
/* ====< インクルード >====================================================== */
#include "prefix.h"
#include <TGL.h>
#include "Game/Game.h"
#include "cChrCmdSpatk.h"
#include "cChrCmdTypes.h"
#include "Action/Attack/nAttack.h"

/* ========================================================================= */
/*		cChrCmdSpatk														 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  コンストラクタ "cChrCmdSpatk"
//! @param  pChr	[in]	オーナー
// ---------------------------------------------
cChrCmdSpatk::cChrCmdSpatk( tCharacter* pChr )
:	cChrCmd(pChr), mpScriptTable(NULL)
{
}

// ---------------------------------------------
//! @brief  デストラクタ "cChrCmdSpatk"
// ---------------------------------------------
cChrCmdSpatk::~cChrCmdSpatk()
{
	foreach( stl::vector<Line*>, mLine, it )
		SAFE_DELETE( *it );
}

// ---------------------------------------------
//! @brief  セットアップ
//! @param  pScriptTable	[in]	スクリプトテーブル
// ---------------------------------------------
void cChrCmdSpatk::setup( const SCRIPTTBL_T* pScriptTable )
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
void cChrCmdSpatk::clear()
{
	cChrCmd::clear();
	mpSuccessTable	= NULL;
	mSuccessDelay	= 0;
	mCheckStep		= 0;

	foreach( stl::vector<Line*>, mLine, it )
		(*it)->clear();
}

// ---------------------------------------------
//! @brief  更新
// ---------------------------------------------
void cChrCmdSpatk::update()
{
	cChrCmd::update();

	//!	成功必殺技時間切れチェック
	if( mSuccessDelay > DELTA )
		mSuccessDelay -= mpChr->getDeltaTime();
	if( mSuccessDelay < DELTA )
		mSuccessId		= -1;

	//!	ライン更新
	foreach( stl::vector<Line*>, mLine, it )
		(*it)->update( *this, *mpChr );
}

/* ========================================================================= */
/*		cChrCmdSpatk::Line													 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  コンストラクタ "Line"
//! @param  pTgs	[in]	スクリプト
//! @param  pTable	[in]	スクリプトテーブル
// ---------------------------------------------
cChrCmdSpatk::Line::Line( const TGS_T* pTgs, const SCRIPTTBL_T* pTable )
:	cChrCmd::Line()
{
	mpScriptTable	= pTable;
	mpCmdStart		= TGS_Label( pTgs, mpScriptTable->mCmdLable );
	clear();
}

// ---------------------------------------------
//! @brief  更新
//! @param  Parent	[i/o]	親
//! @param  Chr		[in]	キャラクター
// ---------------------------------------------
void cChrCmdSpatk::Line::update( cChrCmd& Parent, const tCharacter& Chr )
{
	bool	bFlag;

	xu32*	pc = (xu32*)mpCmd;
	while( pc ){
		u8	nCmd = TGS_PC_I(pc);
		switch( nCmd ){
			// -------------------------------
			case CHRCMD_SETSPATK:	//!< chrcmd_Spatk	
			case CHRCMD_SETSPATK2:	//!< chrcmd_Spatk2	
			{
				s16	nAnmNo	= (s16)TGS_PC_I(pc,1);
				
				//!	特殊攻撃設定
/*				if( nCmd == CHRCMD_SETSPATK2 ){
					s16	nId = G_GSpec.Data.PlayerSpec.nSAtkEquip[TGS_PC_I(pc,1)];
					if( nId <= SSKILL_NONE || nId >= SSKILL_MAX ){	//!< 無効値
						clear();
						pc = NULL;
						break;
					}
					nAnmNo	= Chr.pSatkParam[nId].nAnmNo;
					orFLG( mpScriptTable->mFlag, Chr.pSatkParam[nId].uSpatkF );
					if( nAnmNo == SATKANM_NONE ){	//!< 無効値
						clear();
						pc = NULL;
						break;
					}
				}
*/			
				bFlag = true;
				//!	属性
				switch( mpScriptTable->mAttr ){
					case SPATKATTR_GROUND:	//!< 地上技	
						if( Chr.isFloat() )
							bFlag = false;
						break;
					case SPATKATTR_AIR:		//!< 空中技	
						if( !Chr.isFloat() )
							bFlag = false;
				}
				//!	SPスキル使用不可
//				if( tstFLG(mpScriptTable->mFlag, SPATKFS_ETC_SATK) ){
//					if( tstFLG(Chr.uNotF,NOTF_SATK) )	bFlag = false;
//				}
				//!	状況制限
				switch( mpScriptTable->mOnly ){
					case SPATKONLY_SP:	//!< スーパーキャンセル技	
						bFlag = false;
						if( Chr.tstAnmF(ANMF_SPATK) )	bFlag = true;
						break;
					case SPATKONLY_GD:	//!< ガードキャンセル技	
						bFlag = false;
						if( Chr.tstAnmF(ANMF_GUARD) )	bFlag = true;
						break;
					case SPATKONLY_DMG:	//!< ダメージキャンセル技	
						bFlag = false;
						if( Chr.tstAnmF(ANMF_DAMAGE) && !Chr.tstAnmF(ANMF_DOWN)	)	bFlag = true;
						break;
				}
				
				if( bFlag ){	//!< 成功
					Parent.setSuccessId( nAnmNo );
					Parent.setSuccessPad( Chr.getInput_C().getDat() );
					Parent.setSuccessLever( Chr.getInput_C().getLever() );
					((cChrCmdSpatk*)(&Parent))->setSuccessDelay( (f32)TGS_PC_I(pc,2) );
					((cChrCmdSpatk*)(&Parent))->setSuccessTable( mpScriptTable );
					Parent.clearLimit();
					pc = TGS_AddPC( pc, 3 );
				} else {	//!< 失敗
					clear();
					pc = NULL;
				}
			}	break;
			  
			// -------------------------------
			default:	//!	親クラスコマンド	
				pc = execCmd( pc, Parent, Chr );
			  	break;
		}
	}
}

/* ========================================================================= */
/*		遷移処理															 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  コマンド技が出せるかどうか判定する
//! @param  uMode	[in]	チェックモード	[CHK_????]
//! @return trueなら出せる
// ---------------------------------------------
bool cChrCmdSpatk::check( u8 uMode )
{
	if( !mpChr )	return false;
	
	bool	bRet, bFlag;
	switch( uMode ){
	  // ---------------------------------------
	  case CHK_BASIC:	//!< 大まかなチェック
		mCheckStep = CHKSTEP_NONE;
		if( mSuccessId < 0 )				return false;
		
		mCheckStep = CHKSTEP_BASIC;
		if( mpChr->tstAnmF(ANMF_EXATK) )	return false;	//!< 超必殺技中

		// -----------------------------
		//!	移行条件判定
		mCheckStep = CHKSTEP_LIMIT;
		if( !checkLimit() )	return false;

		//!	移動技判定	
		if( tstFLG(mpSuccessTable->mFlag,SPATKF_MOVE) ){
			//if( mpChr->tstNotF(NOTF_MOVE) || mpChr->tstAb2F(AB2F_NMOVE) )	return false;
		}

		// -----------------------------
		//!	属性
		mCheckStep = CHKSTEP_ATTR;
		switch( mpSuccessTable->mAttr ){
			case SPATKATTR_GROUND:	//!< 地上	
				if( mpChr->isFloat() )	return false;
				break;
			case SPATKATTR_AIR:		//!< 空中	
				if( !mpChr->isFloat() )	return false;
				break;
		}

		// -----------------------------
		//!	発生済み特殊攻撃
//		if( tstFLG(mpSuccessTable->mFlag,SPATKF_SATK_N) && mpChr->bSAtkUse[SATKSLOT_N] )	return false;	// ニュートラル
//		if( tstFLG(mpSuccessTable->mFlag,SPATKF_SATK_U) && mpChr->bSAtkUse[SATKSLOT_U] )	return false;	// 上
//		if( tstFLG(mpSuccessTable->mFlag,SPATKF_SATK_LR) && mpChr->bSAtkUse[SATKSLOT_LR] )	return false;	// 左右
//		if( tstFLG(mpSuccessTable->mFlag,SPATKF_SATK_D) && mpChr->bSAtkUse[SATKSLOT_D] )	return false;	// 下

		// -----------------------------
		mCheckStep = CHKSTEP_KIND;
		switch( mpSuccessTable->mKind ){
		  //!	超必殺技								
		  case SPATKKIND_EXATK:
			if( mpChr->tstAnmF(ANMF_EXATK) )	return false;
			break;
		  //!	必殺技									
		  case SPATKKIND_SPATK:
		  case SPATKKIND_LEVATK:
			if( mpChr->tstAnmF(ANMF_SPATK) && !tstFLG(mpSuccessTable->mFlag,SPATKF_SAME) )	return false;
			break;
		  //!	通常技									
		  case SPATKKIND_BASEATK:
			if( !mpChr->tstPrgF(PRGF_DEFAULT) && !tstFLG(mpSuccessTable->mFlag,SPATKF_SAME) )	return false;
			break;
		  //!	その他									
		  default:
			break;
		}
		return true;

	  // ---------------------------------------
	  case CHK_IDLESAME:	//!< 待機中（同時押し対策）
		mCheckStep = CHKSTEP_IDLESAME;
		//!	キャンセルでのみ出せる技
		if( !tstFLG(mpSuccessTable->mFlag,SPATKF_SAME) )	return false;
		if( (s32)mpChr->getAnmTime() >= tCharacter::SAMEBUTTON_FRAME
			|| mpChr->tstPrgF(PRGF_COMBO)
			|| mpChr->tstAnmF(ANMF_THROW)
			/*|| mpChr->tstNotF(NOTF_BTNSAMESPATK)	*/
			|| !mpChr->tstAtkTblF(ATKTBLF_SAME)
		){
			return false;
		}
	  // ---------------------------------------
	  case CHK_IDLE:	//!< 待機中	
		mCheckStep = CHKSTEP_IDLE;
		//!	キャンセルでのみ出せる技
		if( mpSuccessTable->mOnly )	return false;
		//!	ジャンプキャンセルから出す場合、前回と同じ技は使えない
		//if( mpChr->nAnmNo == ANMNO_JUMP || mpChr->nAnmNo == ANMNO_HIJUMP ){
		//	if( mSuccessId == mpChr->anmNoOld2 )	return false;
		//}
		//!	リバーサル判定
		//if( mpChr->tstAnmF(ANMF_REVERSAL) && tstFLG(mpSuccessTable->mFlag,SPATK_SPATK|SPATK_SPATK2|SPATK_EXATK) ){
		//}
		break;

	  // ---------------------------------------
	  case CHK_ATTACK:	//!< 攻撃中	
		mCheckStep = CHKSTEP_ATTACK;
		if( !mpChr->tstAnmF(ANMF_ATTACK) )	return false;
		//!	キャンセルはできない
		if( tstFLG(mpSuccessTable->mFlag,SPATKF_CAN_DISABLE) )	return false;
		//!	喰らいキャンセル系
		if( mpSuccessTable->mOnly == SPATKONLY_GD || mpSuccessTable->mOnly == SPATKONLY_DMG )	return false;

		//!	ETCF_CANCEL限定技
		if( mpSuccessTable->mOnly == SPATKONLY_ETCF ){
			if( mpChr->tstEtcF(ETCF_CANCEL) && checkAtk() )	break;
			else	return false;
		}

		//!	移動技制限
		if( tstFLG(mpSuccessTable->mFlag,SPATKF_MOVE) ){
			if( mpChr->tstEtcF(ETCF_CANCELMOVE) )	return true;	//!< 許可
			//if( mpChr->getNMoveCanCount() > DELTA )	return false;
		}

		//!	ガード移行許可
		if( tstFLG(mpSuccessTable->mFlag,SPATKF_CAN_GUARDIDLE) ){
			if( mpChr->tstEtcF(ETCF_GUARDOK) )	return true;
		}

		bRet = false;
		//!	空キャンセルできる技
		if( tstFLG(mpSuccessTable->mFlag,SPATKF_CAN_NOHIT) && mpSuccessTable->mOnly != SPATKONLY_HIT ){
			//!	受付時間判定
			bFlag = false;
			if( mpChr->getAttack().tstFlag(cAttack::HITF_DELAY | cAttack::HITF_ENABLE) ){	//!< 判定発生前
				if( tstFLG(mpSuccessTable->mFlag,SPATKF_CAN_FAST) )	bFlag = true;
			}else{	//! 判定発生後
				bFlag = true;
			}
			if( bFlag )
				bRet |= checkCancel();
		}
		if( bRet )	break;	//!< 空キャン受付成功

		//!	ヒットしてる
		if( mpChr->tstPrgF(PRGF_HIT|PRGF_GHIT) || mpChr->tstEtcF(ETCF_CANCEL) )
			bRet |= checkCancel();
		
		if( bRet )	break;	//!< キャンセル受付成功
		return false;

	  // ---------------------------------------
	  case CHK_DAMAGE:	//!< ガードorダメージorダウン中
		mCheckStep = CHKSTEP_DAMAGE;
		//!	死んでる
		if( mpChr->tstChrF(CHRF_DEAD) )	return false;
		//!	喰らい中でない＆喰らった瞬間＆（投げられ中or特殊ダメージ中）
		if( !mpChr->tstAnmF(ANMF_GUARD|ANMF_DAMAGE|ANMF_DOWN|ANMF_WAKEUP) ||
			mpChr->tstPrgF(PRGF_DAMAGE) ||
			mpChr->tstAnmF(ANMF_TDOWN /*|ANMF_SUBDMG*/ )
		){
			return false;
		}
		//!	喰らいキャンセル系でない
		if( mpSuccessTable->mOnly != SPATKONLY_GD && mpSuccessTable->mOnly != SPATKONLY_DMG &&
			!tstFLG(mpSuccessTable->mFlag, SPATKF_CAN_GD|SPATKF_CAN_DMG) ){
			return false;
		}

		bRet = false;
		//!	ガードキャンセル技
		if( mpSuccessTable->mOnly == SPATKONLY_GD || tstFLG(mpSuccessTable->mFlag,SPATKF_CAN_GD) ){
			if( mpChr->tstAnmF(ANMF_GUARD) ){
				bRet = true;
				mpChr->setGuardStiff( 0 );
			}
		}
		//!	ダメージキャンセル技
		if( mpSuccessTable->mOnly == SPATKONLY_DMG || tstFLG(mpSuccessTable->mFlag,SPATKF_CAN_DMG) )
			bRet = true;

		if( bRet )	break;	//!< 喰らいキャンセル成功
		return false;

	  // ---------------------------------------
	  case CHK_NODEFAULT:	//!< 攻撃中ではなく、PRGF_DEFAULTも立っていない
		mCheckStep = CHKSTEP_NODEFAULT;
	  	bRet = false;
	  	
		//!	いつでもキャンセルできる技
		if( tstFLG(mpSuccessTable->mFlag,SPATKF_CAN_ALWAYS) )	return true;

		//!	
		if( mpChr->tstEtcF(ETCF_CANCEL) )	//!< 受付中
			bRet |= checkCancel();
		
		if( bRet )	break;	//!< キャンセル成功

		return false;
	}

	//!	成功
	mpChr->clrPrgF( PRGF_DEFAULT );
	mpChr->orPrgF( PRGF_ACTIVE );
	mSuccessDelay	= 0;
	mCheckStep		= CHKSTEP_SUCCESS;
	if( tstFLG(mpSuccessTable->mFlag,SPATKF_SAME) )	mpChr->clrAtkTblF( ATKTBLF_SAME );
	else											mpChr->orAtkTblF( ATKTBLF_SAME );

	//!	技使用
//	if( tstFLG(mpSuccessTable->mFlag,SPATKF_SATK_N) )	mpChr->bSAtkUse[SATKSLOT_N] = true;
//	if( tstFLG(mpSuccessTable->mFlag,SPATKF_SATK_U) )	mpChr->bSAtkUse[SATKSLOT_U] = true;
//	if( tstFLG(mpSuccessTable->mFlag,SPATKF_SATK_LR) )	mpChr->bSAtkUse[SATKSLOT_LR] = true;
//	if( tstFLG(mpSuccessTable->mFlag,SPATKF_SATK_D) )	mpChr->bSAtkUse[SATKSLOT_D] = true;

	return true;
}

// ---------------------------------------------
//! @brief  攻撃の種類を見てキャンセルできる技なのかどうか判定
//! @return true:キャンセル可能
// ---------------------------------------------
bool cChrCmdSpatk::checkAtk()
{
	//!	どんな攻撃技でもキャンセルできる技
	if( tstFLG(mpSuccessTable->mFlag,SPATKF_CAN_ALWAYS) ){
		return true;

	//!	ジャンプキャンセル
/*	}else if( mpSuccessTable->mOnly == SPATKONLY_JCANCEL) ){
		tCharacter*	pChrT = mpChr->getAttackChr();
		if( !mpChr->tstPrgF(PRGF_HIT) )	return false;	//!< 攻撃がヒットしたかどうか
		if( !pChrT )					return false;	//!< 攻撃対象の有無
		if( !pChrT->isFloat() )	return false;	//!< 相手が浮いてるかどうか
		if( mpChr->getAttack().tstAtkF(ATKF_JCANCEL) && mpChr->getAttack().getCountTime() >= (f32)mpChr->getAttack().getJCancelDelay() ){
			mpChr->orPrgF( PRGF_JCANCEL );
			return true;
		}
*/
	//!	キャンセルできる技
	}else{
		if( mpChr->getAttack().tstAtkF(ATKF_CANCEL) )	return true;
	}
	
	return false;
}

// ---------------------------------------------
//! @brief  キャンセル可能かどうか
//! @return true:可
// ---------------------------------------------
bool cChrCmdSpatk::checkCancel()
{
	//!	スーパーキャンセルできる技
	if( mpSuccessTable->mOnly == SPATKONLY_SP || tstFLG(mpSuccessTable->mFlag,SPATKF_CAN_SP) ){
		//!	必殺技中
		if( mpChr->tstAnmF(ANMF_SPATK) ){
//			if( tstFLG(mpSuccessTable->mFlag,SPATKF_EXATK) ){	//!< 出そうとしている技が超必殺の場合はアビリティが必要
//				if( GCHARABL_TstAb2Flag(pChr,AB2F_SPCANCEL) )	return true;
//			}else{
				return true;
//			}
		}
	}
	//!	通常キャンセル技
	if( mpSuccessTable->mOnly != SPATKONLY_SP ){
		return checkAtk();
	}
	
	return false;
}

// ---------------------------------------------
//! @brief  チェックステップ名
//! @return 文字列
// ---------------------------------------------
const char* cChrCmdSpatk::getCheckStepName() const
{
	const char* _NAME[CHKSTEP_MAX] = {
		"None",			//!< 無し
		"Basic",		//!< 大まかなチェック
		"Limit",		//!< 移行制限
		"Attribute",	//!< 属性
		"Kind",			//!< 種類
		"IdleSame",		//!< 同時押し
		"Idle",			//!< 待機中
		"Attack",		//!< 攻撃中
		"Damage",		//!< ダメージ中
		"NoDefault",	//!< その他
		"Success",		//!< 成功
	};
	return _NAME[mCheckStep];
}

/* ========================================================================= */
/*		EOF																	 */
/* ========================================================================= */
