//============================================
//
//	数字の処理 [numberBillboard.cpp]
//	Author:sakamoto kai
//
//============================================
#include "numberFall.h"
#include "input.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"

//マクロ定義
#define NUMBER_WIGHT (20.0f)		//横幅
#define NUMBER_HEIGHT (20.0f)		//縦幅

//====================================================================
//コンストラクタ
//====================================================================
CNumberFall::CNumberFall(int nPriority) : CNumberBillboard(nPriority)
{
	SetWidth(NUMBER_WIGHT);
	SetHeight(NUMBER_HEIGHT);
	m_move.x = 0.0f;
	m_move.y = 5.0f;
	m_move.z = 0.0f;
	m_ColorA = 1.0f;
}

//====================================================================
//デストラクタ
//====================================================================
CNumberFall::~CNumberFall()
{

}

//====================================================================
//生成処理
//====================================================================
CNumberFall* CNumberFall::Create(void)
{
	CNumberFall* pNumber = NULL;

	if (pNumber == NULL)
	{
		//オブジェクト2Dの生成
		pNumber = new CNumberFall();
	}

	//オブジェクトの初期化処理
	if (FAILED(pNumber->Init()))
	{//初期化処理が失敗した場合
		return NULL;
	}

	return pNumber;
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CNumberFall::Init(void)
{
	CNumberBillboard::Init();

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CNumberFall::Uninit(void)
{
	CNumberBillboard::Uninit();
}

//====================================================================
//更新処理
//====================================================================
void CNumberFall::Update(void)
{
	D3DXVECTOR3 pos = GetPos();

	m_move.y -= 0.98f;
	m_ColorA -= 0.03f;

	pos += m_move;

	SetPos(pos);
	SetColorA(m_ColorA);

	//頂点情報の更新
	CNumberBillboard::Update();

	if (m_ColorA < 0.0f)
	{
		Uninit();
	}
}

//====================================================================
//描画処理
//====================================================================
void CNumberFall::Draw(void)
{
	CNumberBillboard::Draw();
}