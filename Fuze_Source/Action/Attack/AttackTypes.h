// -------------------------------------------------------------------------- //
//! @file   AttackTypes.h
//! @brief  攻撃管理定義
//! @author Nal
//! @since  11/10/01(土)
//!
//! COPYRIGHT (C) 2011 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
#ifndef	_ATTACKTYPES_H
#define	_ATTACKTYPES_H
/* ========================================================================= */
/*		定義 																 */
/* ========================================================================= */
//!	フラグ（HITプリミティブのユーザーフラグ）
#define	COLUSERF_ATK		(1<< 0)	//!< 攻撃判定
#define	COLUSERF_DMG		(1<< 1)	//!< ダメージ判定
#define	COLUSERF_PUSH		(1<< 2)	//!< 押し合い判定
#define	COLUSERF_SCALE		(1<< 3)	//!< マトリクスアクセサのスケール有効

/* ========================================================================= */
/*		定義（パラメータ関連）												 */
/* ========================================================================= */
//!	威力
#define	ATKPOW_W	( 0)	//!< 弱
#define	ATKPOW_M	( 1)	//!< 中
#define	ATKPOW_S	( 2)	//!< 強
#define	ATKPOW_MAX	( 3)

//!	攻撃箇所
#define	ATKPNT_HIGH		( 0)	//!< 上段
#define	ATKPNT_MID		( 1)	//!< 中段
#define	ATKPNT_LOW		( 2)	//!< 下段
#define	ATKPNT_MAX		( 3)

//!	強度（相殺優先度）
#define	ATKHARD_HAND	( 0)	//!< 素手			（刃物に負ける / 殴る蹴るなどの肉弾攻撃）
#define	ATKHARD_WEAPON	( 1)	//!< 武器			（素手とも刃物とも相殺する / 棒、刃物にも屈しない鉄拳など）
#define	ATKHARD_EDGE	( 2)	//!< 刃物			（素手に勝つ / 剣）
#define	ATKHARD_ENERGY	( 3)	//!< 謎のエネルギー	（HAND、WEAPON、EDGEに優先して勝つ / 魔法、魔術）
#define	ATKHARD_MAX		( 4)

//!	ショット相殺レベル
#define	SHOTERASE_NONE	( 0)	//!< 
#define	SHOTERASE_L1	( 1)	//!< 小型弾まで消せる
#define	SHOTERASE_L2	( 2)	//!< ビームまで消せる
#define	SHOTERASE_MAX	( 3)

//!	ヒットの仕方（エフェクトに関わる / mHitKind）
#define	HITKIND_HIT		( 0)	//!< ヒット
#define	HITKIND_GUARD	( 1)	//!< ガード
#define	HITKIND_FIELD	( 2)	//!< 床・壁
#define	HITKIND_SOUSAI	( 3)	//!< 相殺
#define	HITKIND_MAX		( 4)

//!	ダメージステータスタイプ（ダメージモーション影響 / 下ほど優先度が高い）
#define	ATKDMGSTA_NONE		( 0)	//!< 指定無し
#define	ATKDMGSTA_NODMG		( 1)	//!< ダメージモーション無効
#define	ATKDMGSTA_SMASH		( 2)	//!< 叩きつけ
#define	ATKDMGSTA_BOUND		( 3)	//!< バウンド
#define	ATKDMGSTA_FLOAT		( 4)	//!< 浮き
#define	ATKDMGSTA_STRIKE	( 5)	//!< 吹っ飛び
#define	ATKDMGSTA_VROLL		( 6)	//!< 縦回転吹っ飛び
#define	ATKDMGSTA_MAX		( 7)

//!	フラグ 攻撃管理（モーション影響やキャラのアクション挙動に関連する）
#define	ATKF_CONTINUE			(1<< 0)	//!< 多段攻撃のアニメで、まだ終了攻撃ではない（コンボ入力処理で必要。終了経過が進まない）
#define	ATKF_CANCEL				(1<< 1)	//!< 動作キャンセル可 
#define	ATKF_JCANCEL			(1<< 2)	//!< ジャンプキャンセル可能（ATKF_CANCELとは独立して機能）
#define	ATKF_AIR				(1<< 3)	//!< 空中攻撃（地上の敵に当たらない）
#define	ATKF_HIGH				(1<< 4)	//!< 上段攻撃（しゃがみの敵に当たらない）
#define	ATKF_GROUND				(1<< 5)	//!< 地上攻撃（空中の敵に当たらない）
#define	ATKF_NOHITSTEP			(1<< 6)	//!< ステップ中の相手に当たらない
#define	ATKF_NODOWNESC			(1<< 7)	//!< ダウン回避不可
#define	ATKF_NOCHASE			(1<< 8)	//!< この攻撃が空中やられ中の相手に当たった場合、以降追撃は不可
#define	ATKF_NOSPARMOR			(1<< 9)	//!< スーパーアーマー無効攻撃
#define	ATKF_DIRREV				(1<<10)	//!< ヒット方向によるダメージ方向の反転を有効（ATRF_SHOT時のみ有効）
#define	ATKF_OTHERDIR			(1<<11)	//!< 攻撃方向算出方法の切替
#define	ATKF_BACKDIR			(1<<12)	//!< 攻撃方向算出時に後ろからに固定
#define	ATKF_ANMDIR				(1<<13)	//!< アニメ初期化時の向きを方向計算に使用
#define	ATKF_ROTDIR				(1<<14)	//!< 攻撃方向算出時に方向差から計算
#define	ATKF_NOTAERIALSPD		(1<<15)	//!< 空中コンボ時の垂直移動補正を無効
#define	ATKF_FLOATDMGHIT		(1<<16)	//!< 空中ダメージ中の敵にのみヒット
#define	ATKF_FINISHHITSTOP		(1<<17)	//!< トドメをさした時だけヒットストップ
#define	ATKF_ABSHITSTOP			(1<<18)	//!< 強制ヒットストップ
#define	ATKF_SHOTMTX			(1<<19)	//!< ショットマトリクス使用
#define	ATKF_HIT3D				(1<<20)	//!< 判定計算時に3D計算強制
#define	ATKF_21					(1<<21)	//!< 
#define	ATKF_22					(1<<22)	//!< 
#define	ATKF_23					(1<<23)	//!< 
#define	ATKF_24					(1<<24)	//!< 
#define	ATKF_25					(1<<25)	//!< 
#define	ATKF_26					(1<<26)	//!< 
#define	ATKF_27					(1<<27)	//!< 
#define	ATKF_28					(1<<28)	//!< 
#define	ATKF_29					(1<<29)	//!< 
#define	ATKF_30					(1<<30)	//!< 
#define	ATKF_GENERAL0			(1<<31)	//!< 命中時（ヒット・ガードに関わらず）にDMGF_GENERAL0を立てる

#define	ATKFS_TGSMASK			(0)	//!< アニメスクリプトからの攻撃設定時に消されないフラグ

//!	フラグ 攻撃属性管理（性質に関わる）
#define	ATRF_EX					(1<< 0)	//!< 超必殺
#define	ATRF_SHOT				(1<< 1)	//!< 飛び道具
#define	ATRF_THROW				(1<< 2)	//!< 投げ
#define	ATRF_GRAPPLE			(1<< 3)	//!< 打撃投げ（ATRF_THROWと同時に立つ必要がある）
#define	ATRF_ATEMI				(1<< 4)	//!< 当身
#define	ATRF_NGUARD				(1<< 5)	//!< ガード不可
#define	ATRF_ABSSOUSAI			(1<< 6)	//!< [高優先]	攻撃判定同士の優先判定時強制相殺
#define	ATRF_NSOUSAI			(1<< 7)	//!<   ↑		他のフラグに優先して相殺判定を無効化する
#define	ATRF_SOUSAI_EX			(1<< 8)	//!< [低優先]	強度に関係なく優先して勝つ。このフラグ同士をぶつけた場合は無視されるため強度設定もちゃんとしておくこと
#define	ATRF_DOWNATK			(1<< 9)	//!< ダウン追い討ち
#define	ATRF_KEZURIDMG			(1<<10)	//!< 削りダメージを与える
#define	ATRF_NHITCOUNT			(1<<11)	//!< ヒットカウントを増加しない
#define	ATRF_COMRADE			(1<<12)	//!< 同じチームへの攻撃
#define	ATRF_PLAYERTEAM			(1<<13)	//!< プレイヤーチームへの攻撃
#define	ATRF_ALLTEAM			(1<<14)	//!< 全チームへの攻撃
#define	ATRF_HITSHOTONLY		(1<<15)	//!< ショットにのみ当たる
#define	ATRF_FAR				(1<<16)	//!< 遠方の敵に有効
#define	ATRF_REQCRASH			(1<<17)	//!< 巻き込み発生
#define	ATRF_REQCRASHREBOUND	(1<<18)	//!< 跳ね返り状態の相手に当たると巻き込み発生
#define	ATRF_CRASH				(1<<19)	//!< 衝突
#define	ATRF_REBOUND			(1<<20)	//!< 衝突状態の相手に当たると衝突反射扱い
#define	ATRF_PLAYERATK			(1<<21)	//!< プレイヤーの攻撃力を参照
#define	ATRF_22					(1<<22)	//!< 
#define	ATRF_23					(1<<23)	//!< 
#define	ATRF_24					(1<<24)	//!< 
#define	ATRF_25					(1<<25)	//!< 
#define	ATRF_26					(1<<26)	//!< 
#define	ATRF_27					(1<<27)	//!< 
#define	ATRF_28					(1<<28)	//!< 
#define	ATRF_29					(1<<29)	//!< 
#define	ATRF_30					(1<<30)	//!< 
#define	ATRF_31					(1<<31)	//!< 

#define	ATRFS_TGSMASK			(0)	//!< アニメスクリプトからの攻撃設定時に消されないフラグ

//!	フラグ 投げ関係		
#define	THRF_THROWGROUND		(1<< 0)	//!< 地上投げ（投げ移行時に自分と相手の PRGF_FLOAT がクリアされる）
#define	THRF_THROWESC			(1<< 1)	//!< 投げ抜け可能（投げ攻撃に立てることで有効）
#define	THRF_TFAILEDBLOW		(1<< 2)	//!< 投げ判定時に失敗した場合、打撃として機能する
#define	THRF_THROWCHKLAND		(1<< 3)	//!< 投げ判定時、自分が地上にいないと失敗
#define	THRF_THROWCHKAIR		(1<< 4)	//!< 投げ判定時、自分が空中にいないと失敗
#define	THRF_TDOWNCHKLAND		(1<< 5)	//!< 投げ判定時、相手が地上にいないと失敗
#define	THRF_TDOWNCHKAIR		(1<< 6)	//!< 投げ判定時、相手が空中にいないと失敗
#define	THRF_TDOWNSAMEANM		(1<< 7)	//!< 投げ判定時、相手が既に移行アニメになっていた場合失敗
#define	THRF_TDOWNCHKDAMAGE		(1<< 8)	//!< 投げ判定時、相手がダメージ中でないと失敗
#define	THRF_TDOWNGUARDSTOP		(1<< 9)	//!< 投げ判定時、相手がガード硬直中だと失敗
#define	THRF_THROWNCHKSELF		(1<<10)	//!< 投げ判定時、自分の状態を無視する
#define	THRF_GRAPPLECHKSPARMOR	(1<<11)	//!< 打撃投げ判定時、スーパーアーマー相手には失敗
#define	THRF_ATEMISHOT			(1<<12)	//!< 当身判定時、飛び道具も取れる	（防御側に設定）
#define	THRF_ATEMIEX			(1<<13)	//!< 当身判定時、超必殺も取れる	（防御側に設定）
#define	THRF_ATEMINLOW			(1<<14)	//!< 当身判定時、下段は取れない	（防御側に設定）
#define	THRF_NATEMI				(1<<15)	//!< 当身判定時、当身されない		（攻撃側に設定 / 最優先）
#define	THRF_CHKTDMGALL			(1<<16)	//!< やられ側の投げ無敵を、通常投げ・打撃投げ両方チェックする（攻撃側に設定）
#define	THRF_THROWGUARD			(1<<17)	//!< ガードされた場合に投げ実行（投げ時のみ有効）
#define	THRF_TGUARDCHKDMG		(1<<18)	//!< THRF_THROWGUARD有効時に自分がダメージの場合は投げが発生しない。
#define	THRF_THROWSELFABS		(1<<19)	//!< 当たってもガードされても自分はアニメ移行実行
#define	THRF_THROWTARABS		(1<<20)	//!< 当たってもガードされても相手はアニメ移行実行
#define	THRF_THROWCHILD			(1<<21)	//!< CHIF_NTHROWが立っている子供に対して投げを成功できる
#define	THRF_TGRAPHITNOEFT		(1<<22)	//!< 打撃投げ成功時にヒットエフェクトを出さない
#define	THRF_POWER				(1<<23)	//!< 投げ成功時も通常の打撃ダメージが通る
#define	THRF_24					(1<<24)	//!< 
#define	THRF_25					(1<<25)	//!< 
#define	THRF_26					(1<<26)	//!< 
#define	THRF_27					(1<<27)	//!< 
#define	THRF_28					(1<<28)	//!< 
#define	THRF_29					(1<<29)	//!< 
#define	THRF_30					(1<<30)	//!< 
#define	THRF_31					(1<<31)	//!< 

#define	THRFS_TGSMASK			(0)	//!< アニメスクリプトからの攻撃設定時に消されないフラグ

//!	ユーザーID
#define	ATKUSERID_NONE			(0)	//!< 無効
#define	ATKUSERID_BLADE			(1)	//!< 自機 ブレード
#define	ATKUSERID_ESTDASHBLADE	(2)	//!< エスト ダッシュブレード
#define	ATKUSERID_PORTALWEAK	(3)	//!< ポータル コア外殻の内側からの攻撃

#endif	/*	_ATTACKTYPES_H	*/
//=============================================================================
// eof
//=============================================================================
