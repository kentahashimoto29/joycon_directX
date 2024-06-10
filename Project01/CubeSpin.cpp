//============================================
//
//	キューブスピン [CubeSpin.cpp]
//	Author:sakamoto kai
//
//============================================
#include "CubeSpin.h"
#include "CubeBlock.h"
#include "CubeDamage.h"
#include "input.h"
#include "renderer.h"
#include "manager.h"
#include "game.h"
#include "texture.h"
#include "object3D.h"

#define WAIGHT_SIZE (11)		//横の頂点数
#define HEIGHT_SIZE (11)		//縦の頂点数
#define WAIGHT_CENTER (0.5f)	//横の原点(0.0f～1.0f)
#define HEIGHT_CENTER (0.5f)	//縦の原点(0.0f～1.0f)
#define FIELD_SIZE (200.0f)		//床一枚の大きさ

//====================================================================
//コンストラクタ
//====================================================================
CCubeSpin::CCubeSpin(int NumCube, int nPriority) :CObject(nPriority)
{
	m_nNumCube = NumCube;
	m_fDamage = 0.0f;
	m_pos = INITVECTOR3;
	m_move = INITVECTOR3;
	m_nLife = -1;
	m_fSpinDistance = 0.0f;
	m_fSpinDisMove = 0.0f;
	m_fSpinSpeed = D3DXVECTOR3(-100.0f, -100.0f, -100.0f);

	for (int nCnt = 0; nCnt < 32; nCnt++)
	{
		m_apCubeDamage[nCnt] = nullptr;
	}
}

//====================================================================
//デストラクタ
//====================================================================
CCubeSpin::~CCubeSpin()
{

}

//====================================================================
//生成処理
//====================================================================
CCubeSpin* CCubeSpin::Create(int NumCube)
{
	CCubeSpin* pObject3D = NULL;

	if (pObject3D == NULL)
	{
		//オブジェクト3Dの生成
		pObject3D = new CCubeSpin(NumCube);
	}

	//オブジェクトの初期化処理
	if (FAILED(pObject3D->Init()))
	{//初期化処理が失敗した場合
		return NULL;
	}

	return pObject3D;
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CCubeSpin::Init(void)
{
	SetType(TYPE_CUBESPIN);

	for (int nCnt = 0; nCnt < m_nNumCube; nCnt++)
	{
		if (m_apCubeDamage[nCnt] == nullptr)
		{
			m_apCubeDamage[nCnt] = CCubeDamage::Create();
			m_apCubeDamage[nCnt]->SetUseSpin(true);
			m_apCubeDamage[nCnt]->SetSize(D3DXVECTOR3(10.0f, 10.0f, 10.0f));
			m_apCubeDamage[nCnt]->SetSpinPos(m_pos);
			m_apCubeDamage[nCnt]->SetSpinDistance(350.0f);
			m_apCubeDamage[nCnt]->SetSpinSpeedY(0.05f);
			m_apCubeDamage[nCnt]->SetCubeType(CCubeDamage::CUBETYPE_NORMAL);
			m_apCubeDamage[nCnt]->SetDamage(m_fDamage);
			m_apCubeDamage[nCnt]->SetSpinCount(D3DXVECTOR3(((2 * D3DX_PI) / m_nNumCube) * nCnt, ((2 * D3DX_PI) / m_nNumCube) * nCnt, ((2 * D3DX_PI) / m_nNumCube) * nCnt));
		}
	}

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CCubeSpin::Uninit(void)
{
	for (int nCnt = 0; nCnt < m_nNumCube; nCnt++)
	{
		if (m_apCubeDamage[nCnt] != nullptr)
		{
			m_apCubeDamage[nCnt]->Uninit();
		}
	}

	SetDeathFlag(true);
}

//====================================================================
//更新処理
//====================================================================
void CCubeSpin::Update(void)
{
	for (int nCnt = 0; nCnt < m_nNumCube; nCnt++)
	{
		if (m_apCubeDamage[nCnt] != nullptr)
		{
			m_apCubeDamage[nCnt]->SetSpinPos(m_pos);
			m_apCubeDamage[nCnt]->SetSpinDistance(m_fSpinDistance);
			m_apCubeDamage[nCnt]->SetSpinSpeedX(m_fSpinSpeed.x);
			m_apCubeDamage[nCnt]->SetSpinSpeedY(m_fSpinSpeed.y);
			m_apCubeDamage[nCnt]->SetSpinSpeedZ(m_fSpinSpeed.z);
			m_apCubeDamage[nCnt]->SetDamage(m_fDamage);
		}
	}

	m_pos += m_move;
}

//====================================================================
//描画処理
//====================================================================
void CCubeSpin::Draw(void)
{

}