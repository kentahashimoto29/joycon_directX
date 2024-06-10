//============================================
//
//	テクスチャの管理 [texture.cpp]
//	Author:sakamoto kai
//
//============================================
#include "texture.h"
#include "renderer.h"
#include "manager.h"
#include "Xmodel.h"

//静的メンバ変数宣言
int CTexture::m_NumAll = 0;
//const char *CTexture::c_apTexturename[] =
//{
//	"data\\TEXTURE\\Player00.png",		//プレイヤー
//	"data\\TEXTURE\\ENEMY00.png",		//敵
//	"data\\TEXTURE\\block000.jpg",		//普通のブロック
//	"data\\TEXTURE\\block001.png",		//壊れるブロック
//	"data\\TEXTURE\\Life.png",			//ライフ
//	"data\\TEXTURE\\rain.png",			//しずく
//	"data\\TEXTURE\\watch.png",			//時計
//	"data\\TEXTURE\\Bullet00.png",		//弾
//	"data\\TEXTURE\\Effect000.jpg",		//エフェクト
//	"data\\TEXTURE\\magic001.png",		//爆発
//	"data\\TEXTURE\\Number00.png",		//数字
//	"data\\TEXTURE\\Test.jpg",			//テスト用
//	"data\\TEXTURE\\0.png",				//テストブロック1
//	"data\\TEXTURE\\1.png",				//テストブロック2
//	"data\\TEXTURE\\3.png",				//テストブロック3
//	"data\\TEXTURE\\5.png",				//テストブロック4
//	"data\\TEXTURE\\7.png",				//テストブロック5
//	"data\\TEXTURE\\9.png",				//テストブロック6
//	"data\\TEXTURE\\19.png",			//テストブロック7
//	"data\\TEXTURE\\37.png",			//テストブロック8
//	"data\\TEXTURE\\123.png",			//テストブロック9
//	"data\\TEXTURE\\147.png",			//テストブロック10
//	"data\\TEXTURE\\369.png",			//テストブロック11
//	"data\\TEXTURE\\789.png",			//テストブロック12
//	"data\\TEXTURE\\12347.png",			//テストブロック13
//	"data\\TEXTURE\\12369.png",			//テストブロック14
//	"data\\TEXTURE\\14789.png",			//テストブロック15
//	"data\\TEXTURE\\36789.png",			//テストブロック16
//};

char CTexture::c_apTexturename[MAX_TEXTURE][128] = {};

//====================================================================
//コンストラクタ
//====================================================================
CTexture::CTexture()
{
	for (int nCntTex = 0; nCntTex < MAX_TEXTURE; nCntTex++)
	{
		m_apTexture[nCntTex] = NULL;
	}
}

//====================================================================
//デストラクタ
//====================================================================
CTexture::~CTexture()
{

}

//====================================================================
//読み込み
//====================================================================
HRESULT CTexture::Load(void)
{
	////デバイスの取得
	//LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//for (int nCntTex = 0; nCntTex < MAX_TEXTURE; nCntTex++)
	//{
	//	//テクスチャの読み込み
	//	if (FAILED(D3DXCreateTextureFromFile(pDevice,
	//		c_apTexturename[nCntTex],
	//		&m_apTexture[nCntTex])))
	//	{
	//		if (c_apTexturename[nCntTex] == NULL)
	//		{
	//			m_NumAll = nCntTex;
	//			return S_OK;
	//		}
	//		else
	//		{
	//			return E_FAIL;
	//		}
	//	}
	//	else
	//	{
	//		int a = 0;
	//	}
	//}

	return S_OK;
}

//====================================================================
//破棄
//====================================================================
void CTexture::Unload(void)
{
	for (int nCntTex = 0; nCntTex < m_NumAll; nCntTex++)
	{
		//テクスチャの破棄
		if (m_apTexture[nCntTex] != NULL)
		{
			m_apTexture[nCntTex]->Release();
			m_apTexture[nCntTex] = NULL;
		}
	}
}

//====================================================================
//テクスチャの番号指定
//====================================================================
int CTexture::Regist(const char *pFilename)
{
	for (int nCntTex = 0; nCntTex < m_NumAll; nCntTex++)
	{
		if (strcmp(&c_apTexturename[nCntTex][0], pFilename) == 0)
		{
			return nCntTex;
		}
	}

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

		//テクスチャの読み込み
	if (SUCCEEDED(D3DXCreateTextureFromFile(pDevice,
		pFilename,
		&m_apTexture[m_NumAll])))
	{
		if (strcmp(&c_apTexturename[m_NumAll][0], "") == 0)
		{
			strcpy(&c_apTexturename[m_NumAll][0], pFilename);
			m_NumAll++;
			return m_NumAll - 1;
		}
		else
		{
			return -1;
		}
	}

	return -1;
}

//====================================================================
//テクスチャの番号指定
//====================================================================
void CTexture::XModelRegist(int *nTexIdx, int nXModelIdx)
{
	int nCheck = -1;

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	D3DXMATERIAL *pMat;	//マテリアルへのポインタ

	//Xモデルの取得
	CXModel::XModel *pXmodel = CManager::GetInstance()->GetXModel()->GetXModel(nXModelIdx);

	//マテリアル情報に対するポインタを所得
	pMat = (D3DXMATERIAL*)pXmodel->m_pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)pXmodel->m_dwNumMat; nCntMat++)
	{
		nCheck = -1;
		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			for (int nCntTex = 0; nCntTex < m_NumAll; nCntTex++)
			{
				if (strcmp(&c_apTexturename[nCntTex][0], pMat[nCntMat].pTextureFilename) == 0)
				{
					nTexIdx[nCntMat] = nCntTex;
					nCheck = nCntTex;
				}
			}

			if (nTexIdx[nCntMat] != nCheck)
			{
				//テクスチャの読み込み
				if (SUCCEEDED(D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&m_apTexture[m_NumAll])))
				{
					strcpy(&c_apTexturename[m_NumAll][0], pMat[nCntMat].pTextureFilename);
					m_NumAll++;
					nTexIdx[nCntMat] = m_NumAll - 1;
				}
			}
		}
		else
		{
			m_apTexture[m_NumAll] = NULL;
			nTexIdx[nCntMat] = -1;
		}
	}
}

//====================================================================
//指定のテクスチャを返す
//====================================================================
LPDIRECT3DTEXTURE9 CTexture::GetAddress(int nIdx)
{
	return m_apTexture[nIdx];
}