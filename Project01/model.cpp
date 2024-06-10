//============================================
//
//	モデルの処理 [model.cpp]
//	Author:sakamoto kai
//
//============================================
#include "model.h"
#include "manager.h"
#include "renderer.h"

//====================================================================
//コンストラクタ
//====================================================================
CModel::CModel()
{
	m_pMesh = NULL;								//メッシュ(頂点情報)へのポインタ
	m_pBuffMat = NULL;							//マテリアルへのポインタ
	m_dwNumMat = 0;								//マテリアルの数

	m_Startpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//位置
	m_Startrot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//位置
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き
	m_pParent = NULL;							//親モデルへのポインタ

	m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_bColorChange = false;
	m_bDisp = true;
}

//====================================================================
//デストラクタ
//====================================================================
CModel::~CModel()
{

}

//====================================================================
//生成処理
//====================================================================
CModel *CModel::Create(const char *pModelName)
{
	CModel *pModel = NULL;

	if (pModel == NULL)
	{
		//モデルの生成
		pModel = new CModel();
	}

	//オブジェクトの初期化処理
	if (FAILED(pModel->Init(pModelName)))
	{//初期化処理が失敗した場合
		return NULL;
	}

	return pModel;
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CModel::Init(const char *pModelName)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//Xファイルの読み込み
	D3DXLoadMeshFromX(pModelName,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_dwNumMat,
		&m_pMesh);

	D3DXMATERIAL *pMat;	//マテリアルへのポインタ

	//テクスチャのメモリを確保
	if (m_pTexture == NULL)
	{
		//メモリを確保する
		m_pTexture = new LPDIRECT3DTEXTURE9[m_dwNumMat];
	}

	//マテリアル情報に対するポインタを所得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			//テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice,
				pMat[nCntMat].pTextureFilename,
				&m_pTexture[nCntMat]);
		}
		else
		{
			m_pTexture[nCntMat] = NULL;
		}
	}

	//位置の設定

	//向きの設定


	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CModel::Uninit(void)
{
	//バッファの破棄
	if (m_pBuffMat != NULL)
	{
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}

	//メッシュの破棄
	if (m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}

	//テクスチャの破棄
	if (m_pTexture != NULL)
	{
		//メモリを確保する
		delete[] m_pTexture;
		m_pTexture = NULL;
	}
}

//====================================================================
//更新処理
//====================================================================
void CModel::Update(void)
{

}

//====================================================================
//描画処理
//====================================================================
void CModel::Draw(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス
	D3DMATERIAL9 matDef;			//現在のマテリアル保存用
	D3DXMATERIAL *pMat;				//マテリアルデータへのポインタ

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	D3DXMATRIX mtxParent;

	if (m_pParent != NULL)
	{//親が存在する
		mtxParent = m_pParent->GetMtxWorld();
	}
	else
	{//親が存在しない
		//現在(最新)のマトリックスを取得する[=プレイヤーのマトリックス]
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
	}

	//算出したマトリクスをかけ合わせる
	D3DXMatrixMultiply(&m_mtxWorld,
		&m_mtxWorld,
		&mtxParent);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	m_WorldPos = D3DXVECTOR3(m_mtxWorld._41, m_mtxWorld._42, m_mtxWorld._43);

	//現在のマテリアルを所得
	pDevice->GetMaterial(&matDef);

	//マテリアルデータへのポインタを所得する
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		if (m_bColorChange == false)
		{
			//マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
		}
		else
		{
			D3DCOLORVALUE color = pMat[nCntMat].MatD3D.Diffuse;
			pMat[nCntMat].MatD3D.Diffuse = m_Color;
				
			//マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			pMat[nCntMat].MatD3D.Diffuse = color;
		}

		//テクスチャの設定
		pDevice->SetTexture(0, m_pTexture[nCntMat]);

		if (m_bDisp == true)
		{
			//モデル(パーツ)の描画
			m_pMesh->DrawSubset(nCntMat);
		}
	}
	//保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}