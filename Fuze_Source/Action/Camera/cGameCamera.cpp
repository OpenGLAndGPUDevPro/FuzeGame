// -------------------------------------------------------------------------- //
//! @file   cGameCamera.cpp
//! @brief  ゲームカメラ
//! @author Nal
//! @since  12/10/09(火)
//!
//! COPYRIGHT (C) 2012 EDELWEISS ALL RIGHT RESERVED
// -------------------------------------------------------------------------- //
/* ====< インクルード >====================================================== */
#include "prefix.h"
#include <TGL.h>
#include "cGameCamera.h"
#include "Game/Game.h"
#include "Action/Character/Ex/tPlayer.h"

/* ========================================================================= */
/*		GameCamera															 */
/* ========================================================================= */
// ---------------------------------------------
//! @brief  コンストラクタ "GameCamera"
// ---------------------------------------------
GameCamera::GameCamera( u32 uFlag/*=FLAGS_DEFAULT*/, TGL::RenderTex* pRT/*=NULL*/ )
:	PathCamera( uFlag, pRT ),
	mViewMode(VIEW_NORMAL), mViewModeOld(VIEW_NORMAL), mBehindRoll(TGL::Vec2(0,0))
{
	setWarpLength( 2000.0f );
	adjust();
}

// ---------------------------------------------
//! @brief  デストラクタ "GameCamera"
// ---------------------------------------------
GameCamera::~GameCamera()
{
}

// ---------------------------------------------
//! @brief  基準値に設定
// ---------------------------------------------
void GameCamera::adjust()
{
	//!	親クラス
	PathCamera::adjust();
	mTargetRate.clear( 0 );
}

// ---------------------------------------------
//! @brief  派生クラスの更新
// ---------------------------------------------
void GameCamera::updateClass()
{
	switch( mViewMode ){
		case VIEW_BEHIND:	updateBehind();		break;	//!< ビハインド
		case VIEW_TARBOSS:	updateTarBoss();	break;	//!< ボス注目
		default:			PathCamera::updateClass();	break;
	}
	mViewModeOld = mViewMode;
}

// ---------------------------------------------
//! @brief  更新 ビハインド
// ---------------------------------------------
void GameCamera::updateBehind()
{
#if 0
	tCharacter*	pPlayer = ICharacter->getPlayer();
	if( !pPlayer )	return;

	//!	回転
	if( !mBehindRoll.isZero() ){
		TGL::Vec3		v0;
		TGL::Matrix		m0;
		TGL::Quat	q0;
		TGL::Vec3	vEye = getEyeVector();
		TGL::Vec3	vUp  = getUpVector();

		//!	上下限界
		const f32	_RANGE = 40.0f;
		v0 = vEye.eulerZ();
		if( v0.x + mBehindRoll.y > _RANGE )
			mBehindRoll.y = _RANGE - v0.x;
		if( v0.x + mBehindRoll.y < -_RANGE )
			mBehindRoll.y = -_RANGE - v0.x;

		D3DXQuaternionRotationAxis( &q0, &getRightVector(), DEG2RAD(mBehindRoll.y) );
		D3DXMatrixRotationQuaternion( &m0, &q0 );
		v0 = vEye.applySR( m0 );

		m0 = TGL::Matrix::createRoll( vUp, mBehindRoll.x );
		v0 = v0.apply( m0 );
		v0 *= mPos.distance( mTar );
		mTar = mPos + v0;
	}
	mBehindRoll = TGL::Vec2(0,0);

	{//! 移動
		TGL::Vec3	vMove = getEyeVector() * 0.5f;

		//!	移動量
		if( FABS(pPlayer->getISpd()) > DELTA ){	//!< プレイヤーのダッシュ
			vMove = pPlayer->getIWay() * pPlayer->getISpd() * pPlayer->getDeltaTime();
		}else{	//!< 通常推進
			vMove = getEyeVector() * 0.5f * IACtrl->getTime().getRateStd();
		}
		
		//!	小隊リーダーとの押し合い
		tCharacter*	pChrT = NULL;
		f32	fDist = 999999.0f;
		//!	最も近い対象
		for( u8 i=0 ; i<sCharacter::TEAM_MAX ; i++ ){
			if( pPlayer->getTeam() == ICharacter->getTeam(i) )	continue;	//!< プレイヤーチーム
			for( cChrNode* pNode = ICharacter->getTeam(i)->getNodeNext(); pNode != NULL; pNode = pNode->getNext() ){
				if( !pNode->getChr()->getPlatoon() )	continue;
				if( !pNode->getChr()->isTarget() )		continue;
				
				f32	f0 = pPlayer->getPos().distance( pNode->getChr()->getPos());
				if( fDist > f0 ){
					fDist = f0;
					pChrT = pNode->getChr();
				}
			}
		}
		//!	押し合い
		if( pChrT ){
			//!	自機側の判定
			TGL::ColLine	line;
			line.setup( TGL::MtxGuide(pPlayer->getPos()), TGL::MtxGuide(pPlayer->getPos()+vMove), pPlayer->getBoundSph() );
			
			//!	判定チェック
			bool	bHit = false;
			TGL::ColBase::RET_T	HitRet[tCharacter::COLBODY_MAX];	//!< 詳細情報格納用
			ZeroMemory( HitRet, sizeof(HitRet) );
			
			//!	押し合い判定をぶつける
			for( u8 i=0 ; i<tCharacter::COLBODY_MAX ; i++ )
				bHit |= line.check( *pChrT->getColPush(i), &HitRet[i] );
			//!	１つでも当たった
			if( bHit ){
				f32	fRangeMax = 0.0f;
				for( u8 i=0 ; i<tCharacter::COLBODY_MAX ; i++ ){	//!< 最もめり込みの大きかった配列を検索
					if( HitRet[i].mHitRange > fRangeMax )
						fRangeMax = HitRet[i].mHitRange;
				}
				//!	めり込み分を移動量から引く
				vMove -= vMove * fRangeMax;
			}
		}

		mPos += vMove;
		mTar += vMove;
	}
#endif
}

// ---------------------------------------------
//! @brief  更新 ボス注目
// ---------------------------------------------
void GameCamera::updateTarBoss()
{
	PathCamera::updateClass();
	
	//!	注目率初期化
	if( mViewMode != mViewModeOld ){
		mTargetRate.init( TGL::TransValue::MODE_NORMAL, 60.0f, TGL::TransValue::CURVE_SUB1, 999, 0.0f, 1.0f );
	}
	
	//!	ボス注目
	tCharacter*	pBoss = ICharacter->getBoss();
	if( pBoss ){
		mTar = mTar.lerp( pBoss->getPos(), mTargetRate.getValue() );
		mTransTar.clear( mTar );
	}

	mTargetRate.update( getDeltaTime() );
}

// ---------------------------------------------
//! @brief  派生クラスの更新（フリービュー中も実行）
// ---------------------------------------------
void GameCamera::updateClass2()
{
	//!	親クラス
	PathCamera::updateClass2();
}

// ---------------------------------------------
//! @brief  キャラクター注目
//! @param  pChr	[in]	キャラクター
// ---------------------------------------------
void GameCamera::adjustChr( const tCharacter* pChr )
{
	mTar = pChr->getPos();
	mPos = pChr->getViewerCamPos();
	
	setupAdjust( mPos, mTar );
	getZoomRange().setType( TGL::RangeBase::TYPE_CLAMP );
	getZoomRange().setRange( pChr->getViewerCamZoomMin(), pChr->getViewerCamZoomMax() );
}

/* ========================================================================= */
/*		EOF																	 */
/* ========================================================================= */
