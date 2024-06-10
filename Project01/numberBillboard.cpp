//============================================
//
//	数字の処理 [numberBillboard.cpp]
//	Author:sakamoto kai
//
//============================================
#include "numberBillboard.h"
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
CNumberBillboard::CNumberBillboard(int nPriority) : CObjectBillboard(nPriority)
{
	SetWidth(NUMBER_WIGHT);
	SetHeight(NUMBER_HEIGHT);
	m_nNumber = 0;
}

//====================================================================
//デストラクタ
//====================================================================
CNumberBillboard::~CNumberBillboard()
{

}

//====================================================================
//生成処理
//====================================================================
CNumberBillboard* CNumberBillboard::Create(void)
{
	CNumberBillboard* pNumber = NULL;

	if (pNumber == NULL)
	{
		//オブジェクト2Dの生成
		pNumber = new CNumberBillboard();
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
HRESULT CNumberBillboard::Init(void)
{
	CObjectBillboard::Init();

	CTexture* pTexture = CManager::GetInstance()->GetTexture();;
	m_nIdxTexture = pTexture->Regist("data\\TEXTURE\\Number01.png");

	SetType(CObject::TYPE_NUMBER);

	SetAnim(D3DXVECTOR2(0.2f, 0.2f));

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CNumberBillboard::Uninit(void)
{
	CObjectBillboard::Uninit();
}

//====================================================================
//更新処理
//====================================================================
void CNumberBillboard::Update(void)
{
	VERTEX_3D* pVtx;	//頂点ポインタを所得

	//頂点バッファをロックし、両店情報へのポインタを所得
	GetVtxBuff()->Lock(0, 0, (void**)&pVtx, 0);

	//頂点情報の更新
	CObjectBillboard::Update();

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
void CNumberBillboard::Draw(void)
{
	CObjectBillboard::Draw();
}