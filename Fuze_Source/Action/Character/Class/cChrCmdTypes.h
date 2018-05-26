// -------------------------------------------------------------------------- //
//! @file   cChrCmdTypes.h
//! @brief  コマンド入力系 define定義
//! @author Nal
//! @since  13/01/05(土)
//!
//! COPYRIGHT (C) 2013 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
#ifndef	_CHRCMDTYPES_H
#define	_CHRCMDTYPES_H
/* ========================================================================= */
/*																			 */
/* ========================================================================= */
//!	ボタン入力
#define CHRPAD_DAT	0	//!< 押されている
#define CHRPAD_TRG	1	//!< 押した
#define CHRPAD_POP	2	//!< 離した
#define CHRPAD_PRE	3	//!< 先行入力
#define CHRPAD_REP	4	//!< リピート
#define CHRPAD_SAME	5	//!< 同時押し
#define CHRPAD_BEAT	6	//!< 連打

//!	攻撃テーブル
#define ATKTBLF_SAME		(1<< 0)		//!< 同時押し用
#define ATKTBLF_EQLEVER		(1<< 1)		//!< レバー比較を "=" で行う
#define ATKTBLF_02			(1<< 2)		//!< 
#define ATKTBLF_03			(1<< 3)		//!< 
#define ATKTBLF_04			(1<< 4)		//!< 
#define ATKTBLF_05			(1<< 5)		//!< 
#define ATKTBLF_06			(1<< 6)		//!< 
#define ATKTBLF_07			(1<< 7)		//!< 
#define ATKTBLF_08			(1<< 8)		//!< 
#define ATKTBLF_09			(1<< 9)		//!< 

/* ========================================================================= */
/*		コマンドスクリプト													 */
/* ========================================================================= */
#define	CHRCMD_SETSPATK			(0)		//!< 成功時のアニメ番号設定
#define	CHRCMD_SETSPATK2		(1)		//!< 成功時の特殊攻撃設定
#define	CHRCMD_SETCOMBOATK		(2)		//!< 成功時のコンボ設定
#define	CHRCMD_SETCOMBOTHROW	(3)		//!< 成功時の投げコンボ設定
#define	CHRCMD_CHKCOMBOLIST		(4)		//!< 入力有効なアニメ番号のリスト
#define	CHRCMD_CHKPAD			(5)		//!< パッド入力チェック（==比較）
#define	CHRCMD_CHKPADTST		(6)		//!< パッド入力チェック（and比較）
#define	CHRCMD_CHKPADROLL		(7)		//!< パッド入力チェック（回転入力）
#define	CHRCMD_CHKMP			(8)		//!< MPチェック
#define	CHRCMD_CHKFLAG			(9)		//!< フラグチェック
#define	CHRCMD_CHKANMNO_N		(10)	//!< アニメ番号チェック（否定）
#define	CHRCMD_CHKEX			(11)	//!< 固有処理
#define	CHRCMD_LIMITFTST		(12)	//!< 移行条件設定 合致すればNG &
#define	CHRCMD_LIMITFNTST		(13)	//!< 移行条件設定 合致すればNG !&
#define	CHRCMD_LIMITFEQ			(14)	//!< 移行条件設定 合致すればNG ==
#define	CHRCMD_LIMITMP			(15)	//!< 移行条件設定 MP
#define	CHRCMD_LIMITANMNO		(16)	//!< 移行条件設定 アニメ番号
#define	CHRCMD_LIMITEX			(17)	//!< 移行条件設定 固有処理コールバック設定
#define	CHRCMD_PRINT			(18)	//!< ログ出力
#define	CHRCMD_END				(19)	//!< テーブル終了
#define	CHRCMD_MAX				(20)	//!< 

//!		SPATK	
//!	種類
#define SPATKKIND_BASEATK		(0)	//!< 通常								
#define SPATKKIND_LEVATK		(1)	//!< レバー								
#define SPATKKIND_SPATK			(2)	//!< 必殺技								
#define SPATKKIND_EXATK			(3)	//!< 超必殺技							
//!	属性
#define SPATKATTR_NONE			(0)	//!< 指定なし							
#define SPATKATTR_GROUND		(1)	//!< 地上技								
#define SPATKATTR_AIR			(2)	//!< 空中技								
//! 状況制限
#define SPATKONLY_NONE			(0)	//!< 特になし							
#define SPATKONLY_HIT			(1)	//!< ヒットキャンセルでのみ出せる技		
#define SPATKONLY_SP			(2)	//!< スーパーキャンセルのみ出せる技		
#define SPATKONLY_GD			(3)	//!< ガードキャンセルでのみ出せる技		
#define SPATKONLY_DMG			(4)	//!< ダメージキャンセルでのみ出せる技	
#define SPATKONLY_ETCF			(5)	//!< ETCF_CANCELでのみ出せる技			
//! フラグ
#define SPATKF_CAN_DISABLE		(1<<0)	//!< キャンセル不可						
#define SPATKF_CAN_SP			(1<<1)	//!< スーパーキャンセルでも出せる技		
#define SPATKF_CAN_GD			(1<<2)	//!< ガードキャンセルでも出せる技		
#define SPATKF_CAN_DMG			(1<<3)	//!< ダメージキャンセルでも出せる技		
#define SPATKF_CAN_GUARDIDLE	(1<<4)	//!< ガード移行							
#define SPATKF_CAN_ALWAYS		(1<<5)	//!< キャンセル可能技以外でもキャンセルで出せる	
#define SPATKF_CAN_NOHIT		(1<<6)	//!< 技がヒットしていなくてもキャンセルで出せる（空キャンセル）	
#define SPATKF_CAN_FAST			(1<<7)	//!< 攻撃判定発生前にキャンセルできる		
#define SPATKF_SAME				(1<<8)	//!< 同時押し								
#define SPATKF_NCLEAR			(1<<9)	//!< 入力成功時に先行入力クリアしない		
#define SPATKF_NCLEAR2			(1<<10)	//!< 他のSPATK入力成功時に入力クリアされない	
#define SPATKF_MOVE				(1<<11)	//!< 移動技属性（移動無効時に出せない）	
#define SPATKF_SATK_N			(1<<12)	//!< 特殊攻撃 N							
#define SPATKF_SATK_U			(1<<13)	//!< 特殊攻撃 ↑						
#define SPATKF_SATK_LR			(1<<14)	//!< 特殊攻撃 ←→						
#define SPATKF_SATK_D			(1<<15)	//!< 特殊攻撃 ↓						
#define SPATKF_COMBO_NOWAITEND	(1<<16)	//!< コンボ 判定終了を待たない			

//!	固有処理
#define	SPATKCHKEX_CHRBIT_LOCKONTARGET		(0)	//!< 有ビット機 ロックオンターゲット有
#define	SPATKCHKEX_CHRBIT_NOLOCKONTARGET	(1)	//!< 有ビット機 ロックオンターゲット無
#define	SPATKCHKEX_CHRBIT_FULLLOCKONTARGET	(2)	//!< 有ビット機 ターゲットへ全弾ロックオン
#define	SPATKCHKEX_CHRBIT_LOCKONSEARCH		(3)	//!< 有ビット機 ロックオンサーチ出してる
//!	固有処理コールバックID
#define	SPATKEX_PLAYEREX		(0)	//!< Player EXアタック
#define	SPATKEX_PLAYEREXFULL	(1)	//!< Player EX乱舞
#define	SPATKEX_PLAYERDASH		(2)	//!< Player ダッシュ(アレンジ)


#endif	/*	_CHRCMDTYPES_H	*/
//=============================================================================
// eof
//=============================================================================
