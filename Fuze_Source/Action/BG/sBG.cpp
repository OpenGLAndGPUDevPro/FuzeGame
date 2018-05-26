// -------------------------------------------------------------------------- //
//! @file   sBG.cpp
//! @brief  背景システム
//! @author Nal
//! @since  11/09/05(月)
//!
//! COPYRIGHT (C) 2011 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
/* ====< インクルード >====================================================== */
#include "prefix.h"
#include <TGL.h>
#include "sBG.h"
#include "Core/tBGResult.h"
#include "Core/Before/tBGStgBefore.h"
#include "Core/Prologue/tBGPrologue.h"
#include "Core/Stage01/tBGStg01.h"
#include "Core/Stage02/tBGStg02.h"
#include "Core/Stage02/tBGStg02Result.h"
#include "Core/Stage03/tBGStg03.h"
#include "Core/Stage04/tBGStg04.h"
#include "Core/Stage05/tBGStg05.h"
#include "Core/Stage06/tBGStg06.h"
#include "Core/Title/tBGTitle.h"
#include "Core/Epilogue/tBGEpilogue.h"
#include "Core/Tutorial/tBGTutorial.h"
#include "Core/Test2/tBGTest2.h"
#include "Game/GameTypes.h"

/* ====< 定義 >====================================================== */
//	インスタンス
sBG*	sBG::mpInstance = NULL;

//!	ステージID
const u8	sBG::BGID_TO_STGID[BGID_MAX] = {
	0,			//!< BGID_NONE
	STG_BEFORE,	//!< BGID_BEFORE
	STG_00,		//!< BGID_PROLOGUE
	STG_01,		//!< BGID_01
	STG_01,		//!< BGID_01R
	STG_02,		//!< BGID_02
	STG_02,		//!< BGID_02R
	STG_03,		//!< BGID_03
	STG_03,		//!< BGID_03R
	STG_04,		//!< BGID_04
	STG_04,		//!< BGID_04R
	STG_05,		//!< BGID_05
	STG_05,		//!< BGID_05R
	STG_06,		//!< BGID_06
	STG_06,		//!< BGID_06R
	0,			//!< BGID_EPILOGUE
	0,			//!< BGID_TITLE
	0,			//!< BGID_TUTORIAL
	0,			//!< BGID_TEST2
};

//!	紐付くリザルトID
const u8	sBG::BGID_TO_RESULTID[BGID_MAX] = {
	0,			//!< BGID_NONE
	0,			//!< BGID_BEFORE
	0,			//!< BGID_PROLOGUE
	BGID_01R,	//!< BGID_01
	BGID_01,	//!< BGID_01R
	BGID_02R,	//!< BGID_02
	BGID_02,	//!< BGID_02R
	BGID_03R,	//!< BGID_03
	BGID_03,	//!< BGID_03R
	BGID_04R,	//!< BGID_04
	BGID_04,	//!< BGID_04R
	BGID_05R,	//!< BGID_05
	BGID_05,	//!< BGID_05R
	BGID_06R,	//!< BGID_06
	BGID_06,	//!< BGID_06R
	0,			//!< BGID_EPILOGUE
	0,			//!< BGID_TITLE
	0,			//!< BGID_TUTORIAL
	0,			//!< BGID_TEST2
};

//!	環境マップ用テクスチャノード
const char*	sBG::ENV_TEXNODE[ENVTEXNODE_MAX] = {
	"Common_EnvC_Tex",
	"R_:Common_EnvC_Tex",
};

namespace {
	//!	リソースリスト名
	static const char*	RESOURCELIST_OBJ[BGID_MAX] = {
		"",			//!< BGID_NONE
		"Before",	//!< BGID_BEFORE
		"Prologue",	//!< BGID_PROLOGUE
		"01",		//!< BGID_01
		"01R",		//!< BGID_01R
		"02",		//!< BGID_02
		"02R",		//!< BGID_02R
		"03",		//!< BGID_03
		"03R",		//!< BGID_03R
		"04",		//!< BGID_04
		"04R",		//!< BGID_04R
		"05",		//!< BGID_05
		"05R",		//!< BGID_05R
		"06",		//!< BGID_06
		"06R",		//!< BGID_06R
		"Epilogue",	//!< BGID_EPILOGUE
		"Title",	//!< BGID_TITLE
		"Tutorial",	//!< BGID_TUTORIAL
		"Test2",	//!< BGID_TEST2
	};
	//!	パックファイルID
	typedef struct {
		u16		mId;		//!< パックファイルID。0xFFFFで無効
		bool	mbCommon;	//!< 常駐
	} PACKFILE_T;
	static const PACKFILE_T	PACKFILE_ID[BGID_MAX] = {
		{ 0xFFFF,	false },	//!< BGID_NONE
		{ 0xFFFF,	false },	//!< BGID_BEFORE
		{ 100,		false },	//!< BGID_PROLOGUE
		{ 110,		false },	//!< BGID_01
		{ 111,		false },	//!< BGID_01R
		{ 120,		false },	//!< BGID_02
		{ 121,		false },	//!< BGID_02R
		{ 130,		false },	//!< BGID_03
		{ 131,		false },	//!< BGID_03R
		{ 140,		false },	//!< BGID_04
		{ 141,		false },	//!< BGID_04R
		{ 150,		false },	//!< BGID_05
		{ 151,		false },	//!< BGID_05R
		{ 160,		false },	//!< BGID_06
		{ 161,		false },	//!< BGID_06R
		{ 190,		false },	//!< BGID_EPILOGUE
		{ 300,		true },		//!< BGID_TITLE
		{ 301,		false },	//!< BGID_TUTORIAL
		{ 0xFFFF,	false },	//!< BGID_TEST2
	};
};
/* ========================================================================= */
/*		sBG																	 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  コンストラクタ "sBG"
// ---------------------------------------------
sBG::sBG()
{
	//!	インスタンスの生成
	ASSERT( !mpInstance && "既にインスタンスが生成されています" );
	mpInstance		= this;
	mpTask			= NULL;
	mpTaskResult	= NULL;
	ZeroMemory( mbResource, sizeof(mbResource) );
}

// ---------------------------------------------
//! @brief  デストラクタ "sBG"
// ---------------------------------------------
sBG::~sBG()
{
	//!	インスタンスの解放
	ASSERT( mpInstance && "インスタンスが生成されていないか、破棄されています" );
	mpInstance = NULL;
	SAFE_DELETE( mpTask );
	SAFE_DELETE( mpTaskResult );
	freeResource();
}

// ---------------------------------------------
//! @brief  定期更新
//! @param  fDeltaTime	[in]	デルタタイム
// ---------------------------------------------
void sBG::OnUpdate( f32 fDeltaTime )
{
}

// ---------------------------------------------
//! @brief  描画
// ---------------------------------------------
void sBG::OnDraw()
{
}

/* ========================================================================= */
/*																			 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  リソース非同期読み込みリクエスト
//! @param  uBG	[in]	BGID_????
// ---------------------------------------------
void sBG::readResource( u8 uBG )
{
	if( uBG == BGID_NONE )	return;
	if( uBG >= BGID_MAX ){	T_ERROR("不正なID");	return;	}

	//!	読み込み
#ifdef _LOAD_FROM_PACK
	if( PACKFILE_ID[uBG].mId < 0xFFFF ){
		stl::string	str;
		//!	本体
		str = getPackFileName( uBG );
		if( !str.empty() )	IFileManager->read( str );
		//T_LOG( "Read Main : %s", str.c_str() );
		//!	サウンド
		str = getPackFileName( uBG, "s" );
		if( !str.empty() )	IFileManager->read( str );
		//T_LOG( "Read Sound : %s", str.c_str() );
	}
#else
	TGL::JsonUtil::readResourceList( "resource/ResList_BG.json", RESOURCELIST_OBJ[uBG] );
#endif	/*	_LOAD_FROM_PACK	*/
	mbResource[uBG] = true;
}

// ---------------------------------------------
//! @brief  リソース解放
//! @param  uBG	[in]	BGID_???? [BGID_NONE:全て]
// ---------------------------------------------
void sBG::freeResource( u8 uBG/*=BGID_NONE*/ )
{
	u8	uS = uBG;
	u8	uE = uBG;
	if( uBG == BGID_NONE ){	//!< 全部
		uS = BGID_NONE+1;
		uE = BGID_MAX-1;
	}
	for( u8 i=uS ; i<=uE ; i++ ){
		//!	読んでない
		if( !mbResource[i] )	continue;
		//!	カレントタスクの場合はタスクも解放
		if( (mpTask && mpTask->getBGId() == i)
			|| (mpTaskResult && mpTaskResult->getBGId() == i) )
			resetTask();
		
		//!	解放
#ifdef _LOAD_FROM_PACK
		if( PACKFILE_ID[i].mId < 0xFFFF && !PACKFILE_ID[i].mbCommon ){
			stl::string	str;
			//!	本体
			str = getPackFileName( i );
			if( !str.empty() )	IFileManager->free( str );
			//T_LOG( "Free Main : %s", str.c_str() );
			//!	サウンド
			str = getPackFileName( i, "s" );
			if( !str.empty() )	IFileManager->free( str );
			//T_LOG( "Free Sound : %s", str.c_str() );
		}
#else
		TGL::JsonUtil::freeResourceList( "resource/ResList_BG.json", RESOURCELIST_OBJ[i] );
#endif	/*	_LOAD_FROM_PACK	*/
		mbResource[i] = false;
	}
}

// ---------------------------------------------
//! @brief  パックファイル名の取得
//! @param  uBG			[in]	BGID_????
//! @param  strSuffix	[in]	末尾に追加する文字列
//! @return ""で無効
// ---------------------------------------------
stl::string sBG::getPackFileName( u8 uBG, const stl::string& strSuffix/*=stl::nullstr()*/ )
{
	stl::string	str = stl::FormatString<stl::string>( "%04d%s.tgp", PACKFILE_ID[uBG].mId, strSuffix.c_str() );
	if( !TGL::nFile::search(str.c_str()) )	return stl::nullstr();	//!< ディレクトリに無い
	return str;
}

// ---------------------------------------------
//! @brief  タスクセット
//! @note   BGID_NONEを渡すと解放命令として動作
//! @param  pTask	[i/o]	タスクポインタ
//! @param  uBG		[in]	BGID_????
// ---------------------------------------------
void sBG::setTask( tBG** pTask, u8 uBG )
{
	//!	メモリ確保
	SAFE_DELETE( *pTask );
	switch( uBG ){
		default:			*pTask = APP_NEW tBG;	break;
		case BGID_BEFORE:	*pTask = APP_NEW tBGStgBefore;		break;	//!< C81面
		case BGID_PROLOGUE:	*pTask = APP_NEW tBGPrologue;		break;	//!< プロローグ
		case BGID_01:		*pTask = APP_NEW tBGStg01;			break;	//!< １面
		case BGID_01R:		*pTask = APP_NEW tBGResult;			break;	//!< １面リザルト
		case BGID_02:		*pTask = APP_NEW tBGStg02;			break;	//!< ２面
		case BGID_02R:		*pTask = APP_NEW tBGStg02Result;	break;	//!< ２面リザルト
		case BGID_03:		*pTask = APP_NEW tBGStg03;			break;	//!< ３面
		case BGID_03R:		*pTask = APP_NEW tBGResult;			break;	//!< ３面リザルト
		case BGID_04:		*pTask = APP_NEW tBGStg04;			break;	//!< ４面
		case BGID_04R:		*pTask = APP_NEW tBGResult;			break;	//!< ４面リザルト
		case BGID_05:		*pTask = APP_NEW tBGStg05;			break;	//!< ５面
		case BGID_05R:		*pTask = APP_NEW tBGResult;			break;	//!< ５面リザルト
		case BGID_06:		*pTask = APP_NEW tBGStg06;			break;	//!< ６面
		case BGID_06R:		*pTask = APP_NEW tBGResult;			break;	//!< ６面リザルト
		case BGID_TITLE:	*pTask = APP_NEW tBGTitle;			break;	//!< タイトル
		case BGID_EPILOGUE:	*pTask = APP_NEW tBGEpilogue;		break;	//!< エピローグ
		case BGID_TUTORIAL:	*pTask = APP_NEW tBGTutorial;		break;	//!< チュートリアル
		case BGID_TEST2:	*pTask = APP_NEW tBGTest2;			break;	//!< テスト
		case BGID_NONE:		return;	//!< 解放命令
	}
	//!	禁則
	if( !mbResource[uBG] )	return;	//!< リソース読み込んでるかどうか
	//!	初期設定
	(*pTask)->set( uBG );
}

// ---------------------------------------------
//! @brief  背景名を取得
//! @param  uId	[in]	BGID_????
//! @return 文字列
// ---------------------------------------------
const char* sBG::getName( u8 uId )
{
	static const char* _NAME[BGID_MAX] = {
		"----",
		"C81",
		"Prologue",
		"Stage 1",
		"Stage 1 Result",
		"Stage 2",
		"Stage 2 Result",
		"Stage 3",
		"Stage 3 Result",
		"Stage 4",
		"Stage 4 Result",
		"Stage 5",
		"Stage 5 Result",
		"Stage 6",
		"Stage 6 Result",
		"Epilogue",
		"Title",
		"Tutorial",
		"Test",
	};
	if( uId >= BGID_MAX )	return "----";
	return _NAME[uId];
}

/* ========================================================================= */
/*		EOF																	 */
/* ========================================================================= */
