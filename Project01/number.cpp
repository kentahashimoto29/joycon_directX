//============================================
//
//	数字の処理 [number.cpp]
//	Author:sakamoto kai
//
//============================================
#include "number.h"
#include "input.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"

//マクロ定義
#define NUMBER_WIGHT (100.0f)		//横幅
#define NUMBER_HEIGHT (100.0f)		//縦幅

//====================================================================
//コンストラクタ
//====================================================================
CNumber::CNumber(int nPriority) : CObject2D(nPriority)
{
	SetWidth(NUMBER_WIGHT);
	SetHeight(NUMBER_HEIGHT);
	m_nNumber = 0;
}

//====================================================================
//デストラクタ
//====================================================================
CNumber::~CNumber()
{

}

//====================================================================
//生成処理
//====================================================================
CNumber *CNumber::Create(void)
{
	CNumber *pNumber = NULL;

	if (pNumber == NULL)
	{
		//オブジェクト2Dの生成
		pNumber = new CNumber();
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
HRESULT CNumber::Init(void)
{
	CObject2D::Init();

	//テクスチャ設定
	SetTexture("data\\TEXTURE\\Number01.png");

	//タイプ設定
	SetType(CObject::TYPE_NUMBER);

	//アニメーションの初期値
	SetAnim(D3DXVECTOR2(0.2f, 0.2f));

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CNumber::Uninit(void)
{
	CObject2D::Uninit();
}

//====================================================================
//更新処理
//====================================================================
void CNumber::Update(void)
{
	VERTEX_2D*pVtx;	//頂点ポインタを所得

	//頂点バッファをロックし、両店情報へのポインタを所得
	GetVtxBuff()->Lock(0, 0, (void**)&pVtx, 0);

	//頂点情報の更新
	CObject2D::Update();

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f + (m_nNumber * 0.1f), 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.1f + (m_nNumber * 0.1f), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f + (m_nNumber * 0.1f), 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f + (m_nNumber * 0.1f), 1.0f);

	//頂点バッファをアンロックする
	GetVtxBuff()->Unlock();
}

//====================================================================
//描画処理
//====================================================================
void CNumber::Draw(void)
{
	CObject2D::Draw();
}