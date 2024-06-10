//============================================
//
//	ライトの処理 [light.cpp]
//	Author:sakamoto kai
//
//============================================
#include "light.h"
#include "manager.h"
#include "renderer.h"

//====================================================================
//コンストラクタ
//====================================================================
CLight::CLight()
{

}

//====================================================================
//デストラクタ
//====================================================================
CLight::~CLight()
{

}

//====================================================================
//カメラの初期化処理
//====================================================================
HRESULT CLight::Init(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	D3DXVECTOR3 vecDir[MAX_LIGHT];	//設定用方向ベクトル

	//ライトの情報をクリアする
	ZeroMemory(&m_aLight, sizeof(D3DLIGHT9));

	for (int nCntlight = 0; nCntlight < MAX_LIGHT; nCntlight++)
	{
		//ライトの種類を設定
		m_aLight[nCntlight].Type = D3DLIGHT_DIRECTIONAL;

		//ライトの拡散光を設定
		m_aLight[nCntlight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//ライトの方向を設定
		switch (nCntlight)
		{
		case 0:
			vecDir[nCntlight] = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
			break;
		case 1:
			vecDir[nCntlight] = D3DXVECTOR3(-1.0f, -1.0f, -1.0f);
			break;
		case 2:
			vecDir[nCntlight] = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
			break;
		}

		D3DXVec3Normalize(&vecDir[nCntlight], &vecDir[nCntlight]);	//ベクトルを正規化する
		m_aLight[nCntlight].Direction = vecDir[nCntlight];

		//ライトを設定する
		pDevice->SetLight(nCntlight, &m_aLight[nCntlight]);

		//ライトを有効にする
		pDevice->LightEnable(nCntlight, TRUE);
	}

	return S_OK;
}

//====================================================================
//カメラの終了処理
//====================================================================
void CLight::Uninit(void)
{

}

//====================================================================
//カメラの更新処理
//====================================================================
void CLight::Update(void)
{

}