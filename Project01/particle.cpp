//============================================
//
//	パーティクル処理 [particle.cpp]
//	Author:sakamoto kai
//
//============================================
#include "particle.h"
#include "effect.h"
#include "manager.h"
#include "camera.h"

//マクロ定義
#define STANDERD_SPEED1 (1.0f)	//パーティクルの基準のスピード
#define STANDERD_SPEED2 (15.0f)	//パーティクルの基準のスピード
#define STANDERD_SPEED3 (400.0f)	//パーティクルの基準のスピード

#define RAND_PAI1 (731)			//角度のランダム
#define PUT_PARTICLE1 (15)		//１フレームに出すパーティクルの数
#define RAND_PAI2 (731)			//角度のランダム
#define PUT_PARTICLE2 (45)		//１フレームに出すパーティクルの数

#define RAND_PAI3 (731)			//角度のランダム
#define PUT_PARTICLE3 (1)		//１フレームに出すパーティクルの数

//====================================================================
//コンストラクタ
//====================================================================
CParticle::CParticle()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	m_nLife = 0;
	m_nSize = 0.0f;
	m_nType = 0;
}

//====================================================================
//コンストラクタ
//====================================================================
CParticle::CParticle(D3DXVECTOR3 pos, D3DXCOLOR col, int nLife, float nSize, int nType)
{
	m_pos = pos;
	m_col = col;
	m_nLife = nLife;
	m_nSize = nSize;
	m_nType = nType;
}

//====================================================================
//デストラクタ
//====================================================================
CParticle::~CParticle()
{

}

//====================================================================
//生成処理
//====================================================================
CParticle *CParticle::Create(D3DXVECTOR3 pos, D3DXCOLOR col, int nLife, float nSize, int nType)
{
	CParticle *pParticle = NULL;

	if (pParticle == NULL)
	{
		//オブジェクト2Dの生成
		pParticle = new CParticle(pos, col, nLife, nSize, nType);
	}

	//オブジェクトの初期化処理
	if (FAILED(pParticle->Init()))
	{//初期化処理が失敗した場合
		return NULL;
	}

 	return pParticle;
}

//====================================================================
//初期化
//====================================================================
HRESULT CParticle::Init(void)
{
	CEffect *pEffect = NULL;
	SetType(CObject::TYPE_PARTICLE);

	if (m_nType == 1)
	{
		for (int nCntAppear = 0; nCntAppear < PUT_PARTICLE1; nCntAppear++)
		{
			//移動量の設定
			m_move.x = sinf((float)(rand() % RAND_PAI1 - (RAND_PAI1 - 1) / 2) / 100.0f + D3DX_PI * -0.5f)*(float)((rand() % 100) / 10.0f) + STANDERD_SPEED1;
			m_move.y = cosf((float)(rand() % RAND_PAI1 - (RAND_PAI1 - 1) / 2) / 100.0f + D3DX_PI * -0.5f)*(float)((rand() % 100) / 10.0f) + STANDERD_SPEED1;
			m_move.z = 0.0f;

			//エフェクトの生成
			pEffect = CEffect::Create();

			pEffect->SetPos(m_pos);
			pEffect->SetMove(m_move);
			pEffect->SetColor(m_col);
			pEffect->SetRadius(m_nSize);
			pEffect->SetLife(m_nLife);
		}
	}
	if (m_nType == 2)
	{
		for (int nCntAppear = 0; nCntAppear < PUT_PARTICLE2; nCntAppear++)
		{
			//移動量の設定
			m_move.x = sinf((float)(rand() % RAND_PAI2 - (RAND_PAI2 - 1) / 2) / 100.0f + D3DX_PI * -0.5f);
			m_move.y = cosf((float)(rand() % RAND_PAI2 - (RAND_PAI2 - 1) / 2) / 100.0f + D3DX_PI * -0.5f);
			m_move.z = 0.0f;

			D3DXVec3Normalize(&m_move, &m_move);
			m_move *= STANDERD_SPEED2;

			//エフェクトの生成
			pEffect = CEffect::Create();

			pEffect->SetPos(m_pos);
			pEffect->SetDel(4.0f);
			pEffect->SetMove(m_move);
			pEffect->SetColor(m_col);
			pEffect->SetRadius(m_nSize);
			pEffect->SetLife(m_nLife);
		}
	}
	if (m_nType == 3)
	{
		for (int nCntAppear = 0; nCntAppear < PUT_PARTICLE3; nCntAppear++)
		{
			//位置の設定
			m_pos.x = sinf((float)(rand() % RAND_PAI2 - (RAND_PAI2 - 1) / 2) / 100.0f + D3DX_PI * -0.5f);
			m_pos.y = 0.0f;
			m_pos.z = cosf((float)(rand() % RAND_PAI2 - (RAND_PAI2 - 1) / 2) / 100.0f + D3DX_PI * -0.5f);

			D3DXVec3Normalize(&m_pos, &m_pos);
			m_pos *= STANDERD_SPEED3;
			m_pos.y = CManager::GetInstance()->GetCamera()->GetPosY() - 400.0f;

			//エフェクトの生成
			pEffect = CEffect::Create();

			pEffect->SetPos(m_pos);
			pEffect->SetColor(m_col);
			pEffect->SetLife(m_nLife);
		}
	}
	if (m_nType == 4)
	{
		for (int nCntAppear = 0; nCntAppear < PUT_PARTICLE2; nCntAppear++)
		{
			//移動量の設定
			m_move.x = sinf((float)(rand() % RAND_PAI2 - (RAND_PAI2 - 1) / 2) / 100.0f + D3DX_PI * -0.5f);
			m_move.y = 0.0f;
			m_move.z = cosf((float)(rand() % RAND_PAI2 - (RAND_PAI2 - 1) / 2) / 100.0f + D3DX_PI * -0.5f);

			D3DXVec3Normalize(&m_move, &m_move);
			m_move *= 8.0f;

			//エフェクトの生成
			pEffect = CEffect::Create();

			pEffect->SetPos(m_pos);
			pEffect->SetDel(-3.5f);
			pEffect->SetMove(m_move);
			pEffect->SetColor(m_col);
			pEffect->SetRadius(m_nSize);
			pEffect->SetLife(m_nLife);
		}
	}

	Uninit();

	return S_OK;
}

//====================================================================
//パーティクルの終了処理
//====================================================================
void CParticle::Uninit(void)
{
	SetDeathFlag(true);
}

//====================================================================
//パーティクルの更新処理
//====================================================================
void CParticle::Update(void)
{

}

//====================================================================
//パーティクルの描画処理
//====================================================================
void CParticle::Draw(void)
{

}