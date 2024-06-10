//============================================
//
//	エフェクト処理 [2DEffect.cpp]
//	Author:sakamoto kai
//
//============================================
#include "2DEffect.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"

//マクロ定義
#define SMALLER (2.0f)

//静的メンバ変数宣言
LPDIRECT3DTEXTURE9 C2DEffect::m_pTexture = NULL;

//====================================================================
//コンストラクタ
//====================================================================
C2DEffect::C2DEffect(int nPriority) : CObject2D(nPriority)
{
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_fRadius = 10.0f;
	m_nLife = 10;
	m_nLifeMax = m_nLife;
	m_fDel = SMALLER;
	m_cTexName = "";
	m_nIdxTexture = -1;
}

//====================================================================
//デストラクタ
//====================================================================
C2DEffect::~C2DEffect()
{

}

//====================================================================
//生成処理
//====================================================================
C2DEffect* C2DEffect::Create()
{
	C2DEffect* pPlayer = NULL;

	if (pPlayer == NULL)
	{
		//オブジェクト2Dの生成
		pPlayer = new C2DEffect();
	}

	//オブジェクトの初期化処理
	if (FAILED(pPlayer->Init()))
	{//初期化処理が失敗した場合
		return NULL;
	}

	return pPlayer;
}

//====================================================================
//初期化処理
//====================================================================
HRESULT C2DEffect::Init(void)
{
	CTexture* pTexture = CManager::GetInstance()->GetTexture();;
	m_nIdxTexture = pTexture->Regist("data\\TEXTURE\\Effect000.jpg");
	m_cTexName = "data\\TEXTURE\\Effect000.jpg";

	SetType(CObject::TYPE_EFFECT);

	CObject2D::Init();

	//頂点カラーの設定
	SetColor(m_col);

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void C2DEffect::Uninit(void)
{
	CObject2D::Uninit();
}

//====================================================================
//更新処理
//====================================================================
void C2DEffect::Update(void)
{
	//サイズの調整
	m_fRadius -= m_fDel;
	SetWidth(m_fRadius);
	SetHeight(m_fRadius);

	m_nLife--;

	//位置更新
	SetPos(GetPos() += m_move);

	//不透明度の設定
	SetColorA((float)m_nLife / (float)m_nLifeMax);

	if (m_nLife <= 0 || m_fRadius <= 0.0f)
	{
		Uninit();
		return;
	}

	//頂点情報の更新
	CObject2D::Update();

	CTexture* pTexture = CManager::GetInstance()->GetTexture();;
	m_nIdxTexture = pTexture->Regist(m_cTexName);
}

//====================================================================
//描画処理
//====================================================================
void C2DEffect::Draw(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//aブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	CObject2D::Draw();

	//aブレンディングを通常に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}