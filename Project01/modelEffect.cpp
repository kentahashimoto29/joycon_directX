//============================================
//
//	敵の処理 [enemy.cpp]
//	Author:sakamoto kai
//
//============================================
#include "modelEffect.h"
#include "renderer.h"
#include "manager.h"
#include "game.h"
#include "texture.h"
#include "XModel.h"
#include "player.h"
#include "Particle.h"
#include "objGauge2D.h"
#include "number.h"
#include "input.h"
#include "CubeBlock.h"
#include "CubeDamage.h"
#include "sound.h"
#include "CubeSpin.h"
#include "camera.h"
#include "fade.h"

//マクロ定義
#define COLLISION_SIZE (D3DXVECTOR3(20.0f,20.0f,20.0f))		//当たり判定
#define MOVE_SPEED (3.0f)		//移動速度
//====================================================================
//コンストラクタ
//====================================================================
CModelEffect::CModelEffect(int nPriority) : CObjectX(nPriority)
{
	SetSize(COLLISION_SIZE);
	m_nIdxXModel = NULL;
	m_nIdxTexture = -1;
	m_pos = INITVECTOR3;
	m_rot = INITVECTOR3;
	m_color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

//====================================================================
//デストラクタ
//====================================================================
CModelEffect::~CModelEffect()
{

}

//====================================================================
//生成処理
//====================================================================
CModelEffect* CModelEffect::Create(char* pModelName)
{
	CModelEffect* pBlock = NULL;

	if (pBlock == NULL)
	{
		//オブジェクト2Dの生成
		pBlock = new CModelEffect();
	}

	//オブジェクトの初期化処理
	if (FAILED(pBlock->Init(pModelName)))
	{//初期化処理が失敗した場合
		return NULL;
	}

	return pBlock;
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CModelEffect::Init(char* pModelName)
{
	SetType(CObject::TYPE_EFFECT);

	CObjectX::Init(pModelName);

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CModelEffect::Uninit(void)
{
	CObjectX::Uninit();
}

//====================================================================
//更新処理
//====================================================================
void CModelEffect::Update(void)
{
	if (m_color.a > 0.0f)
	{
		SetMatColor(m_color);
		m_color.a -= 0.05f;
	}
	else
	{
		Uninit();
	}

	CObjectX::SetPos(m_pos);
	CObjectX::SetRot(m_rot);
}

//====================================================================
//描画処理
//====================================================================
void CModelEffect::Draw(void)
{
	CObjectX::Draw();
}