// -------------------------------------------------------------------------- //
//! @file   cSceneParam.cpp
//! @brief  シーンパラメータ
//! @author Nal
//! @since  12/03/05(月)
//!
//! COPYRIGHT (C) 2012 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
/* ====< インクルード >====================================================== */
#include "prefix.h"
#include <TGL.h>
#include <Graphics/Filter/TGLFilterBloom.h>
#include "cSceneParam.h"
#include "../Core/tBG.h"

//#pragma optimize("",off)
/* ========================================================================= */
/*		cSceneParam															 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  コンストラクタ "cSceneParam"
// ---------------------------------------------
cSceneParam::cSceneParam( tBG* pOwner )
:	mpOwner(pOwner), mbEnable(true)
{
	reset();
}

// ---------------------------------------------
//! @brief  デストラクタ "cSceneParam"
// ---------------------------------------------
cSceneParam::~cSceneParam()
{
	reset();
}

// ---------------------------------------------
//! @brief  初期化
// ---------------------------------------------
void cSceneParam::reset()
{
	if( !isEnable() )	return;
	
	foreach( stl::vector<Data*>, mData, it )
		SAFE_DELETE( *it );

	foreach( stl::vector<LightPoint*>, mPointLight, it )
		SAFE_DELETE( *it );

	mResPath	= stl::nullstr();
	mCurrentId	= 0;
	mPrevId		= 0xFF;
	mBlendRate.clear( 1 );
	for( u8 i=0 ; i<Data::LIGHT_MAX ; i++ ){
		mAmbLightType[i]	= TGL::LightGroup::AMBTYPE_STANDARD;
		mDirLightNum[i]		= 1;
		mPointLightNum[i]	= 0;
	}
	mPointLightNum[Data::LIGHT_BG]	= POINTLIGHT_BG_NUM;
	mPointLightNum[Data::LIGHT_CHR]	= POINTLIGHT_CHR_NUM;
	mDataPrev.clear();
	mDataBlend.clear();
	mEnvTex			= stl::nullstr();
	mRimLight.reset();
	mbClearSyncFog	= false;
}

// ---------------------------------------------
//! @brief  セットアップ
//! @note   Jsonからのリソース読み込み後には自動的に呼ばれる
// ---------------------------------------------
void cSceneParam::setup()
{
	//!	ライト生成
	ILight->destroyGroup();
	for( u8 i=0 ; i<Data::LIGHT_MAX ; i++ ){
		TGL::LightGroup*	pGroup = APP_NEW TGL::LightGroup;

		//!	環境光タイプ
		pGroup->setAmbType( mAmbLightType[i] );

		//!	平行光追加
		for( u8 j=0 ; j<mDirLightNum[i] ; j++ )
			pGroup->addDirectional( APP_NEW TGL::DirectionalLight );

		//!	点光源追加
		for( u8 j=0 ; j<mPointLightNum[i] ; j++ ){
			TGL::PointLight*	pLight = APP_NEW TGL::PointLight;
			pLight->setEnable( false );
			pGroup->addPoint( pLight );
		}
		//!	キャラクター ヒットライト追加
		if( i == Data::LIGHT_CHR ){
			TGL::PointLight*	pLight = APP_NEW TGL::PointLight;
			pLight->setEnable( false );
			pGroup->addPoint( pLight );
		}

		ILight->addGroup( pGroup );
	}
	
	//!	補間用データセットアップ
	mDataPrev.clear();
	mDataBlend.clear();
	for( u8 i=0 ; i<Data::LIGHT_MAX ; i++ ){
		mDataPrev.mLight[i].mDir.push_back( APP_NEW LightDir );
		mDataBlend.mLight[i].mDir.push_back( APP_NEW LightDir );
	}
	
	//!	カレントデータ反映
	applyCurrentData();
}

// ---------------------------------------------
//! @brief  更新
//! @param  fDelta	[in]	デルタタイム
// ---------------------------------------------
void cSceneParam::update( f32 fDelta )
{
	if( !isEnable() )	return;

	mBlendRate.update( fDelta );
	
	//!	点光源
	foreach( stl::vector<LightPoint*>, mPointLight, it )
		(*it)->update( fDelta );
}

// ---------------------------------------------
//! @brief  反映
//! @note   ブレンド対応
// ---------------------------------------------
void cSceneParam::apply()
{
	if( !isEnable() )	return;

	if( mPrevId >= 0xFF || mBlendRate.getValue() > (1.0f-DELTA) ){	//!< 非ブレンド
		if( getCurrentData() )
			getCurrentData()->apply( mAmbLightType );
	}else{	//!< ブレンド
		culcBlendData( mDataBlend );
		mDataBlend.apply( mAmbLightType );
	}

	//!	点光源
	applyPointLight();

	//!	背景色
	if( mbClearSyncFog ){
		TGL::BaseFog*	pFog = IFog->getFog( Data::FOG_BG );
		if( pFog )
			ITGLRenderer->setClearColor( pFog->getColor().getValue() );
	}else
		ITGLRenderer->setClearColor( IRGBA(0,0,0,255) );
}

// ---------------------------------------------
//! @brief  補間済データを計算
//! @param  Ret	[i/o]	結果
// ---------------------------------------------
void cSceneParam::culcBlendData( Data& Ret )
{
	//!	カレントデータ
	Data*	pDataCurr = getCurrentData();
	if( !pDataCurr )	return;
	
	//!	初回除外
	if( mPrevId >= 0xFF || mBlendRate.getValue() > (1.0f-DELTA) ){	//!< 非ブレンド
		for( u8 i=0 ; i<Data::LIGHT_MAX ; i++ ){
			Ret.mLight[i].mAmbStdColor		= pDataCurr->mLight[i].mAmbStdColor;
			Ret.mLight[i].mAmbHSSkyColor	= pDataCurr->mLight[i].mAmbHSSkyColor;
			Ret.mLight[i].mAmbHSGrdColor	= pDataCurr->mLight[i].mAmbHSGrdColor;
			Ret.mLight[i].mAmbHSSkyDir		= pDataCurr->mLight[i].mAmbHSSkyDir;
			Ret.mLight[i].mAmbHSPower		= pDataCurr->mLight[i].mAmbHSPower;
			Ret.mLight[i].mCamColor			= pDataCurr->mLight[i].mCamColor;
			Ret.mLight[i].mCamPower			= pDataCurr->mLight[i].mCamPower;
			for( u8 j=0 ; j<Ret.mLight[i].mDir.size() ; j++ )
				*Ret.mLight[i].mDir[j] = *pDataCurr->mLight[i].mDir[j];
		}
		CopyMemory( Ret.mFog, pDataCurr->mFog, sizeof(Ret.mFog) );
		Ret.mBloom_Threshold		= pDataCurr->mBloom_Threshold;
		Ret.mBloom_Dispersion		= pDataCurr->mBloom_Dispersion;
		Ret.mBloom_BlendDensityMin	= pDataCurr->mBloom_BlendDensityMin;
		Ret.mBloom_BlendDensityMax	= pDataCurr->mBloom_BlendDensityMax;
		return;
	}
	
	//!	ライト
	for( u8 i=0 ; i<Data::LIGHT_MAX ; i++ ){
		Light&	_Ret = Ret.mLight[i];
		Light&	_Curr = pDataCurr->mLight[i];
		Light&	_Prev = mDataPrev.mLight[i];
		
		//!	環境光
		switch( mAmbLightType[i] ){
			case TGL::LightGroup::AMBTYPE_STANDARD:	//!< 標準
				FRGB_Lerp( &_Ret.mAmbStdColor, &_Prev.mAmbStdColor, &_Curr.mAmbStdColor, mBlendRate.getValue() );
				break;
			case TGL::LightGroup::AMBTYPE_HEMISPHERE:	//!< 半球
				FRGB_Lerp( &_Ret.mAmbHSSkyColor, &_Prev.mAmbHSSkyColor, &_Curr.mAmbHSSkyColor, mBlendRate.getValue() );
				FRGB_Lerp( &_Ret.mAmbHSGrdColor, &_Prev.mAmbHSGrdColor, &_Curr.mAmbHSGrdColor, mBlendRate.getValue() );
				_Ret.mAmbHSSkyDir = _Prev.mAmbHSSkyDir.lerp( _Curr.mAmbHSSkyDir, mBlendRate.getValue() );
				_Ret.mAmbHSPower	= TGL::Math::Lerp<f32>( _Prev.mAmbHSPower, _Curr.mAmbHSPower, mBlendRate.getValue() );
				break;
		}
		
		//!	視線光
		FRGB_Lerp( &_Ret.mCamColor, &_Prev.mCamColor, &_Curr.mCamColor, mBlendRate.getValue() );
		_Ret.mCamPower	= TGL::Math::Lerp<f32>( _Prev.mCamPower, _Curr.mCamPower, mBlendRate.getValue() );
		
		//!	平行光
		for( u8 j=0 ; j<_Ret.mDir.size() ; j++ ){
			LightDir*	pDir = _Ret.mDir[j];
			FRGB_Lerp( &pDir->mColor, &_Prev.mDir[j]->mColor, &_Curr.mDir[j]->mColor, mBlendRate.getValue() );
			pDir->mDirection = _Prev.mDir[j]->mDirection.lerp( _Curr.mDir[j]->mDirection, mBlendRate.getValue() );
			pDir->mPower = TGL::Math::Lerp<f32>( _Prev.mDir[j]->mPower, _Curr.mDir[j]->mPower, mBlendRate.getValue() );
		}
	}
	//!	フォグ
	for( u8 i=0 ; i<numberof(Ret.mFog) ; i++ ){
		Ret.mFog[i].mNear	= TGL::Math::Lerp<f32>( mDataPrev.mFog[i].mNear, pDataCurr->mFog[i].mNear, mBlendRate.getValue() );
		Ret.mFog[i].mFar	= TGL::Math::Lerp<f32>( mDataPrev.mFog[i].mFar, pDataCurr->mFog[i].mFar, mBlendRate.getValue() );
		FRGB_Lerp( &Ret.mFog[i].mColor, &mDataPrev.mFog[i].mColor, &pDataCurr->mFog[i].mColor, mBlendRate.getValue() );
	}
	//!	その他
	Ret.mBloom_Threshold		= TGL::Math::Lerp<f32>( mDataPrev.mBloom_Threshold, pDataCurr->mBloom_Threshold, mBlendRate.getValue() );
	Ret.mBloom_Dispersion		= TGL::Math::Lerp<f32>( mDataPrev.mBloom_Dispersion, pDataCurr->mBloom_Dispersion, mBlendRate.getValue() );
	Ret.mBloom_BlendDensityMin	= TGL::Math::Lerp<f32>( mDataPrev.mBloom_BlendDensityMin, pDataCurr->mBloom_BlendDensityMin, mBlendRate.getValue() );
	Ret.mBloom_BlendDensityMax	= TGL::Math::Lerp<f32>( mDataPrev.mBloom_BlendDensityMax, pDataCurr->mBloom_BlendDensityMax, mBlendRate.getValue() );
}

// ---------------------------------------------
//! @brief  点光源反映
// ---------------------------------------------
void cSceneParam::applyPointLight()
{
	for( u8 i=0 ; i<Data::LIGHT_MAX ; i++ ){
		TGL::LightGroup*	pGroup = ILight->getGroup(i);
		if( !pGroup )	continue;
		
		stl::vector<LightPoint*>	Array = mPointLight;
		//!	無効データ除去
		for( stl::vector<LightPoint*>::iterator it=Array.begin() ; it!=Array.end() ; ){
			if( !(*it)->isEnable(i) )
				it = Array.erase( it );
			else
				++it;
		}
		//!	優先度ソート
		std::sort( Array.begin(), Array.end(), LightPoint::sortCB_PointLight );
		
		//!	ライト実体へ反映
		for( u8 j=0 ; j<mPointLightNum[i] ; j++ ){
			TGL::PointLight*	pLight = pGroup->getPoint( j );
			if( !pLight )	break;
			pLight->setEnable( false );	//!< まず無効

			if( Array.size() <= j )	break;
			LightPoint*	pData = Array[j];

			pLight->setEnable( true );
			pLight->setColor( pData->mColor );
			pLight->setPos( pData->mSortPos );
			pLight->setRangeMin( pData->mRangeMin );
			pLight->setRangeMax( pData->mRangeMax );
			pLight->setPower( pData->mPower * pData->mBlink.getValue() );
		}
		//IDPrint->add( "%d\n", Array.size() );
	}
}
// ---------------------------------------------
//! @brief  ソート関数 近い順に並べる
//! @param  pA	[in]	A
//! @param  pB	[in]	B
//! @return 正:a<b, 負:a>b
// ---------------------------------------------
bool cSceneParam::LightPoint::sortCB_PointLight( LightPoint* pA, LightPoint* pB )
{
	return (pB->mSortDist - pA->mSortDist) > DELTA;
}

// ---------------------------------------------
//! @brief  指定Jsonからパラメータをロード
//! @param  strPath	[in]	Jsonパス
//! @param  strObj	[in]	オブジェクト名
// ---------------------------------------------
void cSceneParam::getFromJson( const stl::string& strPath, const stl::string& strObj/*="-"*/ )
{
	if( !isEnable() )	return;

	TGL::JsonUtil	json( strPath );
	TGL::Json::array	jsonArray = json.getArray( strObj );
	foreach( TGL::Json::array, jsonArray, it ){
		TGL::Json::object&	work = it->get<TGL::Json::object>();
		
		//!	ヘッダ
		for( u8 i=0 ; i<Data::LIGHT_MAX ; i++ ){
			stl::string	strHead( Data::getLightJsonHead(i) );
			mAmbLightType[i]	= atoi( work[ strHead+"AmbType" ].to_str().c_str() );	//!< 環境光タイプ
			mDirLightNum[i]		= atoi( work[ strHead+"DirNum" ].to_str().c_str() );	//!< 平行光数
			//mPointLightNum[i]	= atoi( work[ strHead+"PointNum" ].to_str().c_str() );	//!< 点光源数	※今のとこ固定
		}
		
		//!	配列
		for( u8 i=0 ; ; i++ ){
			stl::string	str = stl::FormatString<stl::string>( "%d", i );
			if( !work[str.c_str()].is<TGL::Json::array>() )	break;
			TGL::Json::array	jsonArray2 = work[str.c_str()].get<TGL::Json::array>();
			foreach( TGL::Json::array, jsonArray2, it2 ){
				TGL::Json::object&	work2 = it2->get<TGL::Json::object>();

				Data*	pData = APP_NEW Data;

				//!	ライト
				for( u8 j=0 ; j<Data::LIGHT_MAX ; j++ ){
					stl::string	strHead( Data::getLightJsonHead(j) );
					//!	環境光
					pData->mLight[j].mAmbStdColor.fR	= (f32)atof( work2[ strHead+"Ambient_R" ].to_str().c_str() );
					pData->mLight[j].mAmbStdColor.fG	= (f32)atof( work2[ strHead+"Ambient_G" ].to_str().c_str() );
					pData->mLight[j].mAmbStdColor.fB	= (f32)atof( work2[ strHead+"Ambient_B" ].to_str().c_str() );
					//!	半球光
					pData->mLight[j].mAmbHSSkyColor.fR	= (f32)atof( work2[ strHead+"HSSkyColor_R" ].to_str().c_str() );
					pData->mLight[j].mAmbHSSkyColor.fG	= (f32)atof( work2[ strHead+"HSSkyColor_G" ].to_str().c_str() );
					pData->mLight[j].mAmbHSSkyColor.fB	= (f32)atof( work2[ strHead+"HSSkyColor_B" ].to_str().c_str() );
					pData->mLight[j].mAmbHSGrdColor.fR	= (f32)atof( work2[ strHead+"HSGrdColor_R" ].to_str().c_str() );
					pData->mLight[j].mAmbHSGrdColor.fG	= (f32)atof( work2[ strHead+"HSGrdColor_G" ].to_str().c_str() );
					pData->mLight[j].mAmbHSGrdColor.fB	= (f32)atof( work2[ strHead+"HSGrdColor_B" ].to_str().c_str() );
					pData->mLight[j].mAmbHSSkyDir.x		= (f32)atof( work2[ strHead+"HSSkyDir_X" ].to_str().c_str() );
					pData->mLight[j].mAmbHSSkyDir.y		= (f32)atof( work2[ strHead+"HSSkyDir_Y" ].to_str().c_str() );
					pData->mLight[j].mAmbHSSkyDir.z		= (f32)atof( work2[ strHead+"HSSkyDir_Z" ].to_str().c_str() );
					pData->mLight[j].mAmbHSPower		= (f32)atof( work2[ strHead+"HSPower" ].to_str().c_str() );
					//!	視線光
					pData->mLight[j].mCamColor.fR	= (f32)atof( work2[ strHead+"CamColor_R" ].to_str().c_str() );
					pData->mLight[j].mCamColor.fG	= (f32)atof( work2[ strHead+"CamColor_G" ].to_str().c_str() );
					pData->mLight[j].mCamColor.fB	= (f32)atof( work2[ strHead+"CamColor_B" ].to_str().c_str() );
					pData->mLight[j].mCamPower		= (f32)atof( work2[ strHead+"CamPower" ].to_str().c_str() );
					//!	平行光
					for( u8 k=0 ; k<mDirLightNum[j] ; k++ ){
						stl::string	strHead2( strHead + stl::FormatString<stl::string>("Dir%d_",k) );	//!< 平行光ID
						LightDir*	pDir = APP_NEW LightDir;
						pDir->mColor.fR		= (f32)atof( work2[ strHead2+"Color_R" ].to_str().c_str() );
						pDir->mColor.fG		= (f32)atof( work2[ strHead2+"Color_G" ].to_str().c_str() );
						pDir->mColor.fB		= (f32)atof( work2[ strHead2+"Color_B" ].to_str().c_str() );
						pDir->mDirection.x	= (f32)atof( work2[ strHead2+"Direction_X" ].to_str().c_str() );
						pDir->mDirection.y	= (f32)atof( work2[ strHead2+"Direction_Y" ].to_str().c_str() );
						pDir->mDirection.z	= (f32)atof( work2[ strHead2+"Direction_Z" ].to_str().c_str() );
						pDir->mPower		= (f32)atof( work2[ strHead2+"Power" ].to_str().c_str() );
						pData->mLight[j].mDir.push_back( pDir );
					}
				}
				//!	フォグ
				for( u8 j=0 ; j<Data::FOG_MAX ; j++ ){
					stl::string	strHead( Data::getFogJsonHead(j) );
					pData->mFog[j].mNear		= (f32)atof( work2[ strHead+"Near" ].to_str().c_str() );
					pData->mFog[j].mFar			= (f32)atof( work2[ strHead+"Far" ].to_str().c_str() );
					pData->mFog[j].mColor.fR	= (f32)atof( work2[ strHead+"R" ].to_str().c_str() );
					pData->mFog[j].mColor.fG	= (f32)atof( work2[ strHead+"G" ].to_str().c_str() );
					pData->mFog[j].mColor.fB	= (f32)atof( work2[ strHead+"B" ].to_str().c_str() );
				}
				//!	その他
				pData->mBloom_Threshold			= (f32)atof( work2["Bloom_Threshold"].to_str().c_str() );
				pData->mBloom_Dispersion		= (f32)atof( work2["Bloom_Dispersion"].to_str().c_str() );
				pData->mBloom_BlendDensityMin	= (f32)atof( work2["Bloom_BlendDensityMin"].to_str().c_str() );
				pData->mBloom_BlendDensityMax	= (f32)atof( work2["Bloom_BlendDensityMax"].to_str().c_str() );

				mData.push_back( pData );
			}
		}
		//!	点光源配列
		for( u8 i=0 ; ; i++ ){
			stl::string	str = stl::FormatString<stl::string>( "PL%d", i );
			if( !work[str.c_str()].is<TGL::Json::array>() )	break;
			TGL::Json::array	jsonArray2 = work[str.c_str()].get<TGL::Json::array>();
			foreach( TGL::Json::array, jsonArray2, it2 ){
				TGL::Json::object&	work2 = it2->get<TGL::Json::object>();

				//!	点光源
				LightPoint*	pPoint = APP_NEW LightPoint( this );
				pPoint->mColor.fR	= (f32)atof( work2[ "Color_R" ].to_str().c_str() );
				pPoint->mColor.fG	= (f32)atof( work2[ "Color_G" ].to_str().c_str() );
				pPoint->mColor.fB	= (f32)atof( work2[ "Color_B" ].to_str().c_str() );
				pPoint->mPos.x		= (f32)atof( work2[ "Pos_X" ].to_str().c_str() );
				pPoint->mPos.y		= (f32)atof( work2[ "Pos_Y" ].to_str().c_str() );
				pPoint->mPos.z		= (f32)atof( work2[ "Pos_Z" ].to_str().c_str() );
				pPoint->mRangeMin	= (f32)atof( work2[ "Range_Min" ].to_str().c_str() );
				pPoint->mRangeMax	= (f32)atof( work2[ "Range_Max" ].to_str().c_str() );
				pPoint->mPower		= (f32)atof( work2[ "Power" ].to_str().c_str() );
				
				f32	fTime = (f32)atof( work2[ "Blink_Time" ].to_str().c_str() );
				if( fTime > DELTA ){
					f32	fS = (f32)atof( work2[ "Blink_Min" ].to_str().c_str() );
					f32	fE = (f32)atof( work2[ "Blink_Max" ].to_str().c_str() );
					pPoint->mBlink.init( TGL::TransValue::MODE_LOOP, fTime, TGL::TransValue::CURVE_LINEAR, 999, fS, fE );
				}
				
				pPoint->mType		= atoi( work2[ "Type" ].to_str().c_str() );
				pPoint->mOnScene	= atoi( work2[ "OnScene" ].to_str().c_str() );
				pPoint->mLoop		= atoi( work2[ "Loop" ].to_str().c_str() );
				pPoint->mLoopPos.x	= (f32)atof( work2[ "LoopPos_X" ].to_str().c_str() );
				pPoint->mLoopPos.y	= (f32)atof( work2[ "LoopPos_Y" ].to_str().c_str() );
				pPoint->mLoopPos.z	= (f32)atof( work2[ "LoopPos_Z" ].to_str().c_str() );
				
				mPointLight.push_back( pPoint );
			}
		}
	}
	mResPath = strPath;
	
	//!	セットアップ
	setup();
}

// ---------------------------------------------
//! @brief  現在のパラメータをファイル出力
// ---------------------------------------------
void cSceneParam::save()
{
#ifdef _WINDOWS
	if( !isEnable() )		return;
	if( mResPath.empty() )	return;
	
	//!	ファイルハンドル
	HANDLE	hFile = TGL::nFile::openHandle( mResPath.c_str() );
	if( hFile == INVALID_HANDLE_VALUE )	return;

	stl::string	str(stl::nullstr());
	//!	ヘッダ
	str += "{\n";

	str += "	\"-\":[\n";
	str += "		{\n";
	
	//!	セットアップ
	for( u8 i=0 ; i<Data::LIGHT_MAX ; i++ ){
		str += stl::FormatString<stl::string>("			\"%sAmbType\"	: \"%d\",\n", Data::getLightJsonHead(i), mAmbLightType[i] );
		str += stl::FormatString<stl::string>("			\"%sDirNum\"	: \"%d\",\n", Data::getLightJsonHead(i), mDirLightNum[i] );
		str	+= stl::FormatString<stl::string>("			\"%sPointNum\"	: \"%d\",\n", Data::getLightJsonHead(i), mPointLightNum[i] );
	}
	str += "\n";

	//!	データ配列
	foreach( stl::vector<Data*>, mData, it ){
		u8	uIdx = (u8)(it - mData.begin());
		str	+= stl::FormatString<stl::string>("			\"%d\":[\n", uIdx );
		str += "				{\n";
		//!	ライト
		for( u8 i=0 ; i<Data::LIGHT_MAX ; i++ ){
			stl::string	strHead( Data::getLightJsonHead(i) );
			Light&	light = (*it)->mLight[i];
			//!	環境光
			str += stl::FormatString<stl::string>("					\"%sAmbient_R\"		: \"%6.3f\",  \"%sAmbient_G\"	: \"%6.3f\",  \"%sAmbient_B\"	: \"%6.3f\",\n", strHead.c_str(), light.mAmbStdColor.fR, strHead.c_str(), light.mAmbStdColor.fG, strHead.c_str(), light.mAmbStdColor.fB );
			//!	半球光
			str += stl::FormatString<stl::string>("					\"%sHSSkyColor_R\"	: \"%6.3f\",  \"%sHSSkyColor_G\"	: \"%6.3f\",  \"%sHSSkyColor_B\"	: \"%6.3f\",\n", strHead.c_str(), light.mAmbHSSkyColor.fR, strHead.c_str(), light.mAmbHSSkyColor.fG, strHead.c_str(), light.mAmbHSSkyColor.fB );
			str += stl::FormatString<stl::string>("					\"%sHSGrdColor_R\"	: \"%6.3f\",  \"%sHSGrdColor_G\"	: \"%6.3f\",  \"%sHSGrdColor_B\"	: \"%6.3f\",\n", strHead.c_str(), light.mAmbHSGrdColor.fR, strHead.c_str(), light.mAmbHSGrdColor.fG, strHead.c_str(), light.mAmbHSGrdColor.fB );
			str += stl::FormatString<stl::string>("					\"%sHSSkyDir_X\"	: \"%6.3f\",  \"%sHSSkyDir_Y\"	: \"%6.3f\",  \"%sHSSkyDir_Z\"	: \"%6.3f\",\n", strHead.c_str(), light.mAmbHSSkyDir.x, strHead.c_str(), light.mAmbHSSkyDir.y, strHead.c_str(), light.mAmbHSSkyDir.z );
			str += stl::FormatString<stl::string>("					\"%sHSPower\"		: \"%6.3f\",\n", strHead.c_str(), light.mAmbHSPower );
			//!	視線光
			str += stl::FormatString<stl::string>("					\"%sCamColor_R\"	: \"%6.3f\",  \"%sCamColor_G\"	: \"%6.3f\",  \"%sCamColor_B\"	: \"%6.3f\",\n", strHead.c_str(), light.mCamColor.fR, strHead.c_str(), light.mCamColor.fG, strHead.c_str(), light.mCamColor.fB );
			str += stl::FormatString<stl::string>("					\"%sCamPower\"		: \"%6.3f\",\n", strHead.c_str(), light.mCamPower );
			//!	平行光
			for( u8 j=0 ; j<mDirLightNum[i] ; j++ ){
				stl::string	strHead2( strHead + stl::FormatString<stl::string>("Dir%d_",j) );	//!< 平行光ID
				LightDir*	pDir = light.mDir[j];
				str += stl::FormatString<stl::string>("					\"%sColor_R\"	: \"%6.3f\",  \"%sColor_G\"	: \"%6.3f\",  \"%sColor_B\"	: \"%6.3f\",\n", strHead2.c_str(), pDir->mColor.fR, strHead2.c_str(), pDir->mColor.fG, strHead2.c_str(), pDir->mColor.fB );
				str += stl::FormatString<stl::string>("					\"%sDirection_X\"	: \"%6.3f\",  \"%sDirection_Y\"	: \"%6.3f\",  \"%sDirection_Z\"	: \"%6.3f\",\n", strHead2.c_str(), pDir->mDirection.x, strHead2.c_str(), pDir->mDirection.y, strHead2.c_str(), pDir->mDirection.z );
				str += stl::FormatString<stl::string>("					\"%sPower\"		: \"%6.3f\",\n", strHead2.c_str(), pDir->mPower );
				break;
			}
		}
		//!	フォグ
		for( u8 i=0 ; i<Data::FOG_MAX ; i++ ){
			stl::string	strHead( Data::getFogJsonHead(i) );
			Data::FOG_T&	fog = (*it)->mFog[i];
			str += stl::FormatString<stl::string>("					\"%sNear\"			: \"%6.0f\",  \"%sFar\"	: \"%6.0f\",\n", strHead.c_str(), fog.mNear, strHead.c_str(), fog.mFar );
			str += stl::FormatString<stl::string>("					\"%sR\"				: \"%6.3f\",  \"%sG\"	: \"%6.3f\",  \"%sB\"	: \"%6.3f\",\n", strHead.c_str(), fog.mColor.fR, strHead.c_str(), fog.mColor.fG, strHead.c_str(), fog.mColor.fB );
		}
		//!	その他
		str += stl::FormatString<stl::string>("					\"Bloom_Threshold\"			: \"%6.3f\",\n", (*it)->mBloom_Threshold );
		str += stl::FormatString<stl::string>("					\"Bloom_Dispersion\"		: \"%6.3f\",\n", (*it)->mBloom_Dispersion );
		str += stl::FormatString<stl::string>("					\"Bloom_BlendDensityMin\"	: \"%6.3f\",\n", (*it)->mBloom_BlendDensityMin );
		str += stl::FormatString<stl::string>("					\"Bloom_BlendDensityMax\"	: \"%6.3f\"\n", (*it)->mBloom_BlendDensityMax );
		str += "				}\n";
		str += "			]";

		if( uIdx >= mData.size()-1 && mPointLight.size() <= 0 )	str += "\n";
		else													str += ",\n";
	}
	
	//!	点光源配列
	foreach( stl::vector<LightPoint*>, mPointLight, it ){
		u8	uIdx = (u8)(it - mPointLight.begin());
		str	+= stl::FormatString<stl::string>("			\"PL%d\":[\n", uIdx );
		str += "				{\n";
		//!	点光源
		str += stl::FormatString<stl::string>("					\"Color_R\"		: \"%6.3f\",  \"Color_G\"	: \"%6.3f\",  \"Color_B\"	: \"%6.3f\",\n", (*it)->mColor.fR, (*it)->mColor.fG, (*it)->mColor.fB );
		str += stl::FormatString<stl::string>("					\"Pos_X\"			: \"%5.0f\",  \"Pos_Y\"		: \"%5.0f\",  \"Pos_Z\"		: \"%5.0f\",\n", (*it)->mPos.x, (*it)->mPos.y, (*it)->mPos.z );
		str += stl::FormatString<stl::string>("					\"Range_Min\"		: \"%5.0f\",  \"Range_Max\"	: \"%5.0f\",\n", (*it)->mRangeMin, (*it)->mRangeMax );
		str += stl::FormatString<stl::string>("					\"Power\"			: \"%6.3f\",\n", (*it)->mPower );
		
		if( (*it)->mBlink.isActive() ){
			str += stl::FormatString<stl::string>("					\"Blink_Time\"	: \"%0.0f\",\n", (*it)->mBlink.getEndTime() );
			str += stl::FormatString<stl::string>("					\"Blink_Min\"		: \"%6.3f\",	\"Blink_Max\"	: \"%6.3f\",\n", (*it)->mBlink.getStart(), (*it)->mBlink.getEnd() );
		}else{
			str += stl::FormatString<stl::string>("					\"Blink_Time\"	: \"0\",\n" );
		}

		str += stl::FormatString<stl::string>("					\"Type\"			: \"%d\",\n", (*it)->mType );
		str += stl::FormatString<stl::string>("					\"OnScene\"		: \"%u\",\n", (*it)->mOnScene );
		str += stl::FormatString<stl::string>("					\"Loop\"			: \"%d\",\n", (*it)->mLoop );
		str += stl::FormatString<stl::string>("					\"LoopPos_X\"		: \"%5.0f\",  \"LoopPos_Y\"		: \"%5.0f\",  \"LoopPos_Z\"		: \"%5.0f\"\n", (*it)->mLoopPos.x, (*it)->mLoopPos.y, (*it)->mLoopPos.z );
		
		str += "				}\n";
		str += "			]";

		if( uIdx >= mPointLight.size()-1 )	str += "\n";
		else								str += ",\n";
	}

	//!	フッター
	str += "		}\n";
	str += "	]\n";
	str += "}\n";

	//!	書き込み
	TGL::nFile::writeHandle( hFile, (void*)str.c_str() );
	TGL::nFile::closeHandle( hFile );
	//!	リロード
	IFileManager->reload( mResPath );
	
	PRINT("Num:%d, Size:%u, \"%s\"\n", mData.size(), str.length(), mResPath.c_str() );
#endif	/*	_WINDOWS	*/
}

/* ========================================================================= */
/*		cSceneParam::Data													 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  コンストラクタ "Data"
// ---------------------------------------------
cSceneParam::Data::Data()
:	mBloom_Threshold(1), mBloom_Dispersion(5), mBloom_BlendDensityMin(1), mBloom_BlendDensityMax(1)
{
	//!	フォグ
	for( u8 i=0 ; i<numberof(mFog) ; i++ ){
		mFog[i].mNear	= 10000.0f;
		mFog[i].mFar	= 10000.0f;
		FRGB_Set( &mFog[i].mColor,	1.0f, 1.0f, 1.0f );
	}
}

// ---------------------------------------------
//! @brief  デストラクタ "Data"
// ---------------------------------------------
cSceneParam::Data::~Data()
{
}

// ---------------------------------------------
//! @brief  反映
//! @param  uLightType	[TGL::LightGroup::AMBTYPE_????]	
// ---------------------------------------------
void cSceneParam::Data::apply( u8 uAmbLights[LIGHT_MAX] )
{
	//!	ライト
	for( u8 i=0 ; i<LIGHT_MAX ; i++ ){
		TGL::LightGroup*	pGroup = ILight->getGroup(i);
		if( !pGroup )	continue;
		
		//!	環境光
		switch( uAmbLights[i] ){
			case TGL::LightGroup::AMBTYPE_STANDARD:	//!< 標準
				pGroup->getAmbient()->setColor( mLight[i].mAmbStdColor );
				break;
			case TGL::LightGroup::AMBTYPE_HEMISPHERE:	//!< 半球
			{	TGL::HemisphereLight*	pLight = pGroup->getHemisphere();
				pLight->setSkyColor( mLight[i].mAmbHSSkyColor );
				pLight->setColor( mLight[i].mAmbHSGrdColor );
				pLight->setSkyDirection( mLight[i].mAmbHSSkyDir );
				pLight->setPower( mLight[i].mAmbHSPower );
			}	break;
		}
		//!	視線光
		pGroup->getCameraLight()->setColor( mLight[i].mCamColor );
		pGroup->getCameraLight()->setPower( mLight[i].mCamPower );
		//!	平行光
		for( u8 j=0 ; j<mLight[i].mDir.size() ; j++ ){
			TGL::DirectionalLight*	pLight = pGroup->getDirectional(j);
			if( pLight ){
				LightDir*	pDir = mLight[i].mDir[j];
				pLight->setColor( pDir->mColor );
				pLight->setDirection( pDir->mDirection );
				pLight->setPower( pDir->mPower );
			}
		}
	}
	
	//!	フォグ
	for( u8 i=0 ; i<numberof(mFog) ; i++ ){
		if( IFog->getFog(i) )
			IFog->getFog(i)->set( mFog[i].mNear, mFog[i].mFar, mFog[i].mColor );
	}

	//!	ブルーム
	ITGLRenderer->getFilterBloom()->setThreshold( mBloom_Threshold );
	ITGLRenderer->getFilterBloom()->setDispersion( mBloom_Dispersion );
	ITGLRenderer->getFilterBloom()->getBlendDensity().setMin( mBloom_BlendDensityMin );
	ITGLRenderer->getFilterBloom()->getBlendDensity().setMax( mBloom_BlendDensityMax );
}

// ---------------------------------------------
//! @brief  ライト リソースヘッダ取得
//! @param  uIdx	[in]	LIGHT_????
//! @return 文字列
// ---------------------------------------------
const char*	cSceneParam::Data::getLightJsonHead( u8 uIdx )
{
	static const char*	_HEAD[LIGHT_MAX] = {
		"BGLight_",
		"CHRLight_",
	};
	return _HEAD[ uIdx ];
}

// ---------------------------------------------
//! @brief  フォグ リソースヘッダ取得
//! @param  uIdx	[in]	FOG_????
//! @return 文字列
// ---------------------------------------------
const char*	cSceneParam::Data::getFogJsonHead( u8 uIdx )
{
	static const char*	_HEAD[FOG_MAX] = {
		"Fog_",
		"FogHorizon_",
		"FogChr_",
	};
	return _HEAD[ uIdx ];
}

/* ========================================================================= */
/*		cSceneParam::Light													 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  コンストラクタ "Light"
// ---------------------------------------------
cSceneParam::Light::Light()
{
	FRGB_Set( &mAmbStdColor,	0.3f, 0.3f, 0.3f );
	FRGB_Set( &mAmbHSSkyColor,	0.258f, 0.258f, 0.4f );
	FRGB_Set( &mAmbHSGrdColor,	0.4f, 0.4f, 0.28f );
	mAmbHSSkyDir	= TGL::Vec3( -0.69f, -0.73f, -0.0f );
	mAmbHSPower		= 1.5f;
	FRGB_Set( &mCamColor,		0.25f, 0.25f, 0.25f );
	mCamPower		= 0.0f;
}

// ---------------------------------------------
//! @brief  デストラクタ "Light"
// ---------------------------------------------
cSceneParam::Light::~Light()
{
	clear();
}

/* ========================================================================= */
/*		cSceneParam::LightPoint												 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  有効か
//! @param  Bg		[in]	背景
//! @param  uType	[in]	Data::LIGHT_????
//! @return true:有効
// ---------------------------------------------
bool cSceneParam::LightPoint::isEnable( u8 uType ) const
{
	//!	対象
	if( mType != uType )	return false;
	//!	有効シーン
	if( !tstOnScene(1<<mpParent->getOwner()->getScene()) )	return false;

	//!	※クリッピング
	

	return true;
}

// ---------------------------------------------
//! @brief  更新
//! @param  fDelta	[in]	デルタタイム
// ---------------------------------------------
void cSceneParam::LightPoint::update( f32 fDelta )
{
	mBlink.update( fDelta );
	
	//!	カメラから最も近い位置
	TGL::Vec3	vWorld( getWorldPos() );
	mSortPos	= vWorld;
	mSortDist	= ICamera->getCamera(0)->getTar().distance( mSortPos );
	for( u8 i=1 ; i<mLoop ; i++ ){
		TGL::Vec3	v0 = vWorld + mLoopPos * i;
		f32	f0 = ICamera->getCamera(0)->getTar().distance( v0 );
		if( f0 < mSortDist ){
			mSortPos	= v0;
			mSortDist	= f0;
		}
	}
}

// ---------------------------------------------
//! @brief  ワールド座標
//! @return TGL::Vec3
// ---------------------------------------------
TGL::Vec3 cSceneParam::LightPoint::getWorldPos() const
{
	if( mType == Data::LIGHT_BG )
		return mPos + mPosOfs + mpParent->getOwner()->getOrigin();
	else
		return mPos + mPosOfs;
}

/* ========================================================================= */
/*		cSceneParam::RimLight												 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  リソースからパラメータ設定
//! @param  work	[in]	Json
// ---------------------------------------------
void cSceneParam::RimLight::setupFromJson( TGL::Json::object& work )
{
	mColor.fR	= (f32)atof( work["RimLight_R"].to_str().c_str() );
	mColor.fG	= (f32)atof( work["RimLight_G"].to_str().c_str() );
	mColor.fB	= (f32)atof( work["RimLight_B"].to_str().c_str() );
	mKr			= (f32)atof( work["RimLight_Kr"].to_str().c_str() );
	mPower		= (f32)atof( work["RimLight_Power"].to_str().c_str() );
}

/* ========================================================================= */
/*		EOF																	 */
/* ========================================================================= */
