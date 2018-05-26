// -------------------------------------------------------------------------- //
//! @file   cChrCmd.cpp
//! @brief  コマンド入力 基底
//! @author Nal
//! @since  12/04/07(土)
//!
//! COPYRIGHT (C) 2012 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
/* ====< インクルード >====================================================== */
#include "prefix.h"
#include <TGL.h>
#include "Game/Game.h"
#include "cChrCmd.h"
#include "cChrCmdTypes.h"
#include "../Ex/tPlayer.h"

/* ========================================================================= */
/*		cChrCmd																 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  コンストラクタ "cChrCmd"
//! @param  pChr	[in]	オーナー
// ---------------------------------------------
cChrCmd::cChrCmd( tCharacter* pChr )
:	mpChr(pChr)
{
	clear();
}

// ---------------------------------------------
//! @brief  デストラクタ "cChrCmd"
// ---------------------------------------------
cChrCmd::~cChrCmd()
{
	clearLimit();
}

// ---------------------------------------------
//! @brief  クリア
// ---------------------------------------------
void cChrCmd::clear()
{
	mSuccessId		= -1;
	mSuccessPad		= 0;
	mSuccessLever	= TGL::Vec2::ZERO;
	clearLimit();
}

// ---------------------------------------------
//! @brief  更新
// ---------------------------------------------
void cChrCmd::update()
{
	//!	同時押しキャンセル
	if( (s32)mpChr->getAnmTime() >= tCharacter::SAMEBUTTON_FRAME )
		mpChr->clrAtkTblF( ATKTBLF_SAME );
}

// ---------------------------------------------
//! @brief  移行制限 クリア
// ---------------------------------------------
void cChrCmd::clearLimit()
{
	foreach( stl::vector<cChrCmdLimit*>, mLimit, it )
		SAFE_DELETE( *it );
	mLimit.clear();
}

// ---------------------------------------------
//! @brief  移行制限 チェック
//! @return true:成立, false:非成立
// ---------------------------------------------
bool cChrCmd::checkLimit() const
{
	const_foreach( stl::vector<cChrCmdLimit*>, mLimit, it ){
		if( !(*it)->check(*mpChr) )	return false;
	}
	return true;
}

/* ========================================================================= */
/*		cChrCmd::Line														 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  クリア
// ---------------------------------------------
void cChrCmd::Line::clear()
{
	mpCmd	= mpCmdStart;
	mTime	= mCTime = 0;
	ZeroMemory( mPadBuf, sizeof(mPadBuf) );
}

// ---------------------------------------------
//! @brief  スクリプトコマンド実行
//! @param  pc		[i/o]	スクリプト
//! @param  Parent	[i/o]	親
//! @param  Chr		[in]	キャラクター
//! @return pc
// ---------------------------------------------
xu32* cChrCmd::Line::execCmd( xu32* pc, cChrCmd& Parent, const tCharacter& Chr )
{
	switch( TGS_PC_I(pc) ){
		case CHRCMD_CHKPAD:		return cmdPad( pc, Parent, Chr );			//!< chrcmd_Pad		
		case CHRCMD_CHKPADTST:	return cmdPad( pc, Parent, Chr );			//!< chrcmd_PadTst	
		case CHRCMD_CHKPADROLL:	return cmdPadRoll( pc, Parent, Chr );		//!< chrcmd_PadRoll	
		case CHRCMD_CHKMP:		return TGS_AddPC(pc, 2);					//!< chrcmd_ChkMp	
		case CHRCMD_CHKFLAG:	return cmdCheckFlag( pc, Chr );				//!< chrcmd_ChkFlag	
		case CHRCMD_CHKANMNO_N:	return cmdAnmNoNot( pc, Chr );				//!< chrcmd_ChkAnmnoN	
		case CHRCMD_CHKEX:		return cmdEx( pc, Chr );					//!< chrcmd_ChkEx	
		case CHRCMD_LIMITFTST:	return cmdLimitFlagTst( pc, Parent );		//!< 移行条件設定 合致すればNG &	
		case CHRCMD_LIMITFNTST:	return cmdLimitFlagNotTst( pc, Parent );	//!< 移行条件設定 合致すればNG !&	
		case CHRCMD_LIMITFEQ:	return cmdLimitFlagEqTst( pc, Parent );		//!< 移行条件設定 合致すればNG ==	
		case CHRCMD_LIMITMP:	return cmdLimitMP( pc, Parent );			//!< 移行条件設定 MP	
		case CHRCMD_LIMITANMNO:	return cmdLimitAnmNo( pc, Parent );			//!< 移行条件設定 アニメ番号	
		case CHRCMD_LIMITEX:	return cmdLimitEx( pc, Parent );			//!< 移行条件設定 固有処理コールバック設定	

		// -------------------------------
		case CHRCMD_PRINT:	//!< ログ出力
			pc = TGS_AddPC(pc);
		  	PRINT("%s", (char *)pc );
			for(;;pc = TGS_AddPC(pc)){
				if(!(*pc&0xff000000)) break;
				if(!(*pc&0x00ff0000)) break;
				if(!(*pc&0x0000ff00)) break;
				if(!(*pc&0x000000ff)) break;
			}
			pc = TGS_AddPC(pc);
			break;
		// -------------------------------
		case CHRCMD_END:	//!< chrcmd_End
			clear();
			return NULL;
		// -------------------------------
		default:
			T_WARNING( "cChrCmd::Line::execCmd: 不明なコマンド" );
			return NULL;
	}
	return pc;
}

// ---------------------------------------------
//! @brief  パッド入力
// ---------------------------------------------
xu32* cChrCmd::Line::cmdPad( xu32* pc, cChrCmd& Parent, const tCharacter& Chr )
{
	//!	入力を受け付けるかどうか
	if( !isInputReceive(Chr) ){
		clear();
		return NULL;
	}
	
	s16	nTimeMax	= (s16)TGS_PC_I(pc,1);
	s16	nCTimeMax	= (s16)TGS_PC_I(pc,2);
	u8	uPad		= (u8)TGS_PC_I(pc,3);
	u32	uPadAnd		= (u32)TGS_PC_I(pc,4);
	u32	uPadChk		= (u32)TGS_PC_I(pc,5);

	// -------------------------------------
	//!	入力判定		
	u32	uPadDat = 0;
	switch( uPad ){
		case CHRPAD_DAT:	uPadDat = Chr.getInput_C().getDat();	break;
		case CHRPAD_TRG:	uPadDat = Chr.getInput_C().getTrg();	break;
		case CHRPAD_POP:	uPadDat = Chr.getInput_C().getPop();	break;
		case CHRPAD_PRE:	uPadDat = Chr.getInput_C().getPre();	break;
		case CHRPAD_SAME:	uPadDat = Chr.getInput_C().getPre();	break;
		case CHRPAD_BEAT:	uPadDat = Chr.getInput_C().getBeat();	break;
	}
	uPadDat &= uPadAnd;
	//!	押されている
	bool	bInput = false;
	switch( TGS_PC_I(pc) ){
		//!	==比較				
		case CHRCMD_CHKPAD:
			bInput = (uPadDat == uPadChk);
			break;
		//!	and比較				
		case CHRCMD_CHKPADTST:
			bInput = (tstFLG(uPadDat,uPadChk) != false);
			break;
	}

	// -------------------------------------
	//!	押し続け		
	if( (s32)mCTime >= nCTimeMax ){
		if( bInput ){
			mCTime		= mTime = 0;
			mPadBuf[1]	= mPadBuf[0];
			mPadBuf[0]	= uPadDat;
			//!	next step
			pc = TGS_AddPC(pc,6);
			return pc;
		}
		//!	next command
		mCTime = 0;
		mpCmd = pc;
		return NULL;
	// -------------------------------------
	//!	入力受付時間中		
	} else if( (s32)mTime < nTimeMax || nTimeMax < 0 ){
		if( bInput ){
			if( !IACtrl->getTime().isStop() )
				mCTime += ITimer->getRate();

			//!	押し続け判定
			bool	bSuccess = false;
			if( nCTimeMax > 1 && uPad == CHRPAD_DAT )
				bSuccess = (s32)mCTime >= nCTimeMax;
			else
				bSuccess = true;

			if( bSuccess ){
				mCTime		= mTime = 0;
				mPadBuf[1]	= mPadBuf[0];
				mPadBuf[0]	= uPadDat;
				//! next step
				pc = TGS_AddPC(pc,6);
				return pc;
			}
		}else{
			mCTime = 0;
		}
		//!	時間経過
		cmdPad_TimeAdd( nTimeMax );
		//!	next command
		if( nTimeMax >= 0 && (s32)mTime >= nTimeMax )
			clear();
		else
			mpCmd = pc;
		return NULL;
	} else {
		//! next command
		clear();
		return NULL;
	}
}

// ---------------------------------------------
//! @brief  パッド 回転入力
// ---------------------------------------------
xu32* cChrCmd::Line::cmdPadRoll( xu32* pc, cChrCmd& Parent, const tCharacter& Chr )
{
	//!	入力を受け付けるかどうか
	if( !isInputReceive(Chr) ){
		clear();
		return NULL;
	}

	s16 nTimeMax	= (s16)TGS_PC_I(pc,1);
	if( (s32)mTime < nTimeMax || nTimeMax < 0 ){	//!< 入力時間チェック
		bool	bInput = false;
		u32	uPadDat	= Chr.getInput_C().getDat();
		//!	判定
		if( mPadBuf[1] == 0 ){	//!< 入力二回目
			switch( tstFLG(mPadBuf[0],GPADS_LEVER) ){
			  case GPAD_UP:
			  case GPAD_DOWN:	if( tstFLG(uPadDat,GPAD_RIGHT|GPAD_LEFT) && !tstFLG(uPadDat,GPAD_UP|GPAD_DOWN) )	bInput = true;	break;
			  case GPAD_RIGHT:
			  case GPAD_LEFT:	if( tstFLG(uPadDat,GPAD_UP|GPAD_DOWN) && !tstFLG(uPadDat,GPAD_RIGHT|GPAD_LEFT) )	bInput = true;	break;
			}
			
		}else{	//!< 二回目以降
			switch( tstFLG(mPadBuf[0],GPADS_LEVER) ){
			  case GPAD_UP:
			  case GPAD_DOWN:
				switch( tstFLG(mPadBuf[1],GPADS_LEVER) ){
				  case GPAD_RIGHT:	if( tstFLG(uPadDat,GPAD_LEFT) && !tstFLG(uPadDat,GPAD_UP|GPAD_DOWN) )	bInput = true;	break;	// →↑"←" or →↓"←"
				  case GPAD_LEFT:	if( tstFLG(uPadDat,GPAD_RIGHT) && !tstFLG(uPadDat,GPAD_UP|GPAD_DOWN) )	bInput = true;	break;	// ←↑"→" or ←↓"→"
				}
				break;
			  case GPAD_RIGHT:
			  case GPAD_LEFT:
				switch( tstFLG(mPadBuf[1],GPADS_LEVER) ){
				  case GPAD_UP:		if( tstFLG(uPadDat,GPAD_DOWN) && !tstFLG(uPadDat,GPAD_RIGHT|GPAD_LEFT) )	bInput = true;	break;	// ↑→"↓" or ↑←"↓"
				  case GPAD_DOWN:	if( tstFLG(uPadDat,GPAD_UP) && !tstFLG(uPadDat,GPAD_RIGHT|GPAD_LEFT) )		bInput = true;	break;	// ↓→"↑" or ↓←"↑"
				}
				break;
			}
		}
		
		//!	時間経過
		cmdPad_TimeAdd( nTimeMax );
		
		//!	入力成功可否
		if( bInput ){
			mCTime 		= 0;
			mTime	 	= 0;
			mPadBuf[1]	= mPadBuf[0];
			mPadBuf[0]	= uPadDat;
			pc = TGS_AddPC(pc,2);
		}

		//! next command
		if( nTimeMax >= 0 && (s32)mTime >= nTimeMax ){	//!< 受付フレームを越えた瞬間にリセットしないと次のフレームの入力が取れない
			clear();
			if( nTimeMax > 1 )	//!< 受付フレームを越えた瞬間の入力を取るため再起する
				update( Parent, Chr );
		}else{
			mpCmd = pc;
		}
		return NULL;
	}else{
		clear();
		return NULL;
	}
	return pc;
}

// ---------------------------------------------
//! @brief  共通 時間経過
//! @param  nTime	[in]	入力制限時間
// ---------------------------------------------
void cChrCmd::Line::cmdPad_TimeAdd( s32 nTime )
{
	if( nTime < 0 )	return;
	if( IACtrl->getTime().isStop() )	return;
	
	f32	fTimeOld = mTime;
	mTime += ITimer->getRate();
	//!	１フレームで制限時間を突破すると無限ループに繋がる
	if( (s32)mTime >= nTime ){
		if( fTimeOld < DELTA )
			mTime = (f32)nTime - 0.01f;
	}
}

// ---------------------------------------------
//! @brief  フラグ比較
// ---------------------------------------------
xu32* cChrCmd::Line::cmdCheckFlag( xu32* pc, const tCharacter& Chr )
{
	u32	uChk = Chr.tstFlagFromId( (u8)TGS_PC_I(pc,2), (u32)TGS_PC_I(pc,3) );

	//!	比較
	switch( TGS_PC_I(pc,1) ){
		case 0:	//!< and比較	
			if( uChk ){
				clear();
				pc = NULL;
			}
			break;
		case 1:	//!< !and比較	
			if( !uChk ){
				clear();
				pc = NULL;
			}
			break;
		case 2:	//!< ==比較		
			if( uChk == (u32)TGS_PC_I(pc,3) ){
				clear();
				pc = NULL;
			}
			break;
		default:
			ASSERT(0 && "cmdCheckFlag: 比較タイプ不正" );
			break;
	}
	if( pc )
		pc = TGS_AddPC(pc,4);
	return pc;
}

// ---------------------------------------------
//! @brief  アニメ番号比較
// ---------------------------------------------
xu32* cChrCmd::Line::cmdAnmNoNot( xu32* pc, const tCharacter& Chr )
{
	if( (s16)TGS_PC_I(pc,1) == Chr.getAnmNo() ){
		clear();
		return NULL;
	}
	return TGS_AddPC(pc,2);
}

// ---------------------------------------------
//! @brief  固有処理
// ---------------------------------------------
xu32* cChrCmd::Line::cmdEx( xu32* pc, const tCharacter& Chr )
{
	bool	bFlag = true;
	switch( TGS_PC_I(pc,1) ){
		case SPATKCHKEX_CHRBIT_LOCKONTARGET:	//!< 有ビット機 ロックオンターゲット有
		{	const tChrBit&	chr = (const tChrBit&)( Chr );
			if( chr.getBitGroupConst().getTargetTotal(true) <= 0 )
				bFlag = false;
		}	break;

		case SPATKCHKEX_CHRBIT_NOLOCKONTARGET:	//!< 有ビット機 ロックオンターゲット無
		{	const tChrBit&	chr = (const tChrBit&)( Chr );
			if( chr.getBitGroupConst().getTargetTotal(true) > 0 )
				bFlag = false;
		}	break;

		case SPATKCHKEX_CHRBIT_FULLLOCKONTARGET:	//!< 有ビット機 ターゲットへ全弾ロックオン
		{	const tChrBit&	chr = (const tChrBit&)( Chr );
			bFlag = false;
			if( chr.getBitGroupConst().getTargetTotal(true) == 1 ){
				LockTarget*	pTarget = chr.getBitGroupConst().getTarget( 0, true );
				if( pTarget->getBit()+pTarget->getBitSetOrder() == chr.getBitGroupConst().getBitTotal() )
					bFlag = true;
			}
		}	break;

		case SPATKCHKEX_CHRBIT_LOCKONSEARCH:	//!< 有ビット機 ロックオンサーチ出してる
		{	const tChrBit&	chr = (const tChrBit&)( Chr );
			if( !chr.isLockSearch() )
				bFlag = false;
		}	break;

	}
	if( !bFlag ){	//!< 失敗
		clear();
		return NULL;
	}
	return TGS_AddPC(pc,3);
}

// ---------------------------------------------
//! @brief  移行制限 フラグ比較
// ---------------------------------------------
xu32* cChrCmd::Line::cmdLimitFlagTst( xu32* pc, cChrCmd& Parent )
{
	Parent.mLimit.push_back( APP_NEW cChrCmdLimit_FlagTst((u8)TGS_PC_I(pc,1), TGS_PC_I(pc,2)) );
	return TGS_AddPC(pc,3);
}
// ---------------------------------------------
//! @brief  移行制限 フラグ否定
// ---------------------------------------------
xu32* cChrCmd::Line::cmdLimitFlagNotTst( xu32* pc, cChrCmd& Parent )
{
	Parent.mLimit.push_back( APP_NEW cChrCmdLimit_FlagNTst((u8)TGS_PC_I(pc,1), TGS_PC_I(pc,2)) );
	return TGS_AddPC(pc,3);
}
// ---------------------------------------------
//! @brief  移行制限 フラグ一致
// ---------------------------------------------
xu32* cChrCmd::Line::cmdLimitFlagEqTst( xu32* pc, cChrCmd& Parent )
{
	Parent.mLimit.push_back( APP_NEW cChrCmdLimit_FlagEq((u8)TGS_PC_I(pc,1), TGS_PC_I(pc,2)) );
	return TGS_AddPC(pc,3);
}
// ---------------------------------------------
//! @brief  移行制限 MP
// ---------------------------------------------
xu32* cChrCmd::Line::cmdLimitMP( xu32* pc, cChrCmd& Parent )
{
	Parent.mLimit.push_back( APP_NEW cChrCmdLimit_MP((u16)TGS_PC_I(pc,1)) );
	return TGS_AddPC(pc,2);
}
// ---------------------------------------------
//! @brief  移行制限 アニメ番号
// ---------------------------------------------
xu32* cChrCmd::Line::cmdLimitAnmNo( xu32* pc, cChrCmd& Parent )
{
	Parent.mLimit.push_back( APP_NEW cChrCmdLimit_AnmNo((s16)TGS_PC_I(pc,1)) );
	return TGS_AddPC(pc,2);
}
// ---------------------------------------------
//! @brief  移行制限 固有
// ---------------------------------------------
xu32* cChrCmd::Line::cmdLimitEx( xu32* pc, cChrCmd& Parent )
{
	switch( TGS_PC_I(pc,1) ){
		case SPATKEX_PLAYEREX:	//!< Player EXアタック
			Parent.mLimit.push_back( APP_NEW cChrCmdLimit_ExtraCB( tPlayer::isSpatkCB_ExBlade, TGS_PC_I(pc,2) ) );
			break;
		case SPATKEX_PLAYEREXFULL:	//!< Player EX乱舞
			Parent.mLimit.push_back( APP_NEW cChrCmdLimit_ExtraCB( tPlayer::isSpatkCB_ExBladeFull, TGS_PC_I(pc,2) ) );
			break;
		case SPATKEX_PLAYERDASH:	//!< Player ダッシュ(アレンジ)
			Parent.mLimit.push_back( APP_NEW cChrCmdLimit_ExtraCB( tPlayer::isSpatkCB_Dash, TGS_PC_I(pc,2) ) );
			break;
	}
	return TGS_AddPC(pc,3);
}

/* ========================================================================= */
/*		EOF																	 */
/* ========================================================================= */
