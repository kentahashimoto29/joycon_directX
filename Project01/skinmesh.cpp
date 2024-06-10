//============================================
//
//	3Dポリゴンでスキンメッシュを行う [skinmesh.cpp]
//	Author:sakamoto kai
//
//============================================
#include "skinmesh.h"
#include "input.h"
#include "renderer.h"
#include "manager.h"
#include "game.h"
#include "texture.h"
#include "CubeBlock.h"

#define POLYDON_SIZE (10.0f)

//====================================================================
//コンストラクタ
//====================================================================
CSkinmesh::CSkinmesh(int nPriority) :CObject(nPriority)
{
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
	m_Width = 50.0f;
	m_Height = 150.0f;
	m_pos = D3DXVECTOR3(0.0f, 100.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_AddDarw = false;
}

//====================================================================
//デストラクタ
//====================================================================
CSkinmesh::~CSkinmesh()
{

}

//====================================================================
//生成処理
//====================================================================
CSkinmesh* CSkinmesh::Create(void)
{
	CSkinmesh* pObject3D = NULL;

	if (pObject3D == NULL)
	{
		//オブジェクト3Dの生成
		pObject3D = new CSkinmesh();
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
HRESULT CSkinmesh::Init(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

	LoadData("data\\TXT\\skinmesh.txt");

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL)))
	{
		return E_FAIL;
	}

	VERTEX_3D* pVtx;	//頂点ポインタを所得

	//頂点バッファをロックし、両店情報へのポインタを所得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < m_nNumVertex; nCnt++)
	{

		//頂点座標の設定 
		pVtx[nCnt].pos.x = m_Vertex[nCnt].fPosX;
		pVtx[nCnt].pos.y = m_Vertex[nCnt].fPosY;
		pVtx[nCnt].pos.z = m_Vertex[nCnt].fPosZ;

		//法線ベクトルの設定
		pVtx[nCnt].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		//頂点カラーの設定
		pVtx[nCnt].col = D3DXCOLOR(m_Vertex[nCnt].WeightValue[1], 0.0f, m_Vertex[nCnt].WeightValue[0], 1.0f);

		//テクスチャ座標の設定
		pVtx[nCnt].tex = m_Vertex[nCnt].Tex;
	}

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//位置を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//ボーンの初期設定
	for (int nCnt = 0; nCnt < m_nNumBone; nCnt++)
	{
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&m_Bone[nCnt].m_mtxWorld);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Bone[nCnt].fRotY, m_Bone[nCnt].fRotX, m_Bone[nCnt].fRotZ);
		D3DXMatrixMultiply(&m_Bone[nCnt].m_mtxWorld, &m_Bone[nCnt].m_mtxWorld, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, m_Bone[nCnt].fPosX, m_Bone[nCnt].fPosY, m_Bone[nCnt].fPosZ);
		D3DXMatrixMultiply(&m_Bone[nCnt].m_mtxWorld, &m_Bone[nCnt].m_mtxWorld, &mtxTrans);

		D3DXMATRIX mtxParent;

		if (m_Bone[nCnt].nParent != -1)
		{//親が存在する
			mtxParent = GetBoneMtxWorld(m_Bone[nCnt].nParent);
		}
		else
		{//親が存在しない
			//現在(最新)のマトリックスを取得する
			mtxParent = m_mtxWorld;
		}

		//算出したマトリクスをかけ合わせる
		D3DXMatrixMultiply(&m_Bone[nCnt].m_mtxWorld,
			&m_Bone[nCnt].m_mtxWorld,
			&mtxParent);

		////算出したマトリクスをかけ合わせる
		//D3DXMatrixMultiply(&m_mtxWorld,
		//	&m_mtxWorld,
		//	&mtxParent);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &m_Bone[nCnt].m_mtxWorld);


		m_pCubeBlock[nCnt] = CCubeBlock::Create();
		m_pCubeBlock[nCnt]->SetPos(D3DXVECTOR3(m_Bone[nCnt].m_mtxWorld._41, m_Bone[nCnt].m_mtxWorld._42, m_Bone[nCnt].m_mtxWorld._43));
		m_pCubeBlock[nCnt]->SetRot(D3DXVECTOR3(m_Bone[nCnt].fRotX, m_Bone[nCnt].fRotY, m_Bone[nCnt].fRotZ));
		m_pCubeBlock[nCnt]->SetSize(D3DXVECTOR3(10.0f, 10.0f, 10.0f));
	}

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CSkinmesh::Uninit(void)
{
	SetDeathFlag(true);
}

//====================================================================
//終了処理
//====================================================================
void CSkinmesh::SetNULL(void)
{
	//頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//====================================================================
//更新処理
//====================================================================
void CSkinmesh::Update(void)
{
	//SetVerTex();

	WaitControl();
}

//====================================================================
//描画処理
//====================================================================
void CSkinmesh::Draw(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

	if (m_AddDarw == true)
	{
		//aブレンディングを加算合成に設定
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//位置を反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	assert(GetIdx() != -1);		//テクスチャの番号を入れ忘れた場合エラーを吐く

								//テクスチャの設定
	pDevice->SetTexture(0, pTexture->GetAddress(GetIdx()));

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
		0,										//プリミティブ(ポリゴン)数
		m_nNumVertex - 2);

	////ポリゴンの描画
	//pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
	//	0,
	//	0,
	//	0,//用意した頂点の数
	//	0,
	//	m_nNumVertex - 2);//描画するプリミティブの数

	if (m_AddDarw == true)
	{
		//aブレンディングを通常に設定
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
}

//====================================================================
//スキンメッシュのウェイト値分だけ頂点を動かす
//====================================================================
void CSkinmesh::WaitControl()
{
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	D3DXMATRIX mtxParent;

	if (pInputKeyboard->GetPress(DIK_Q) == true)
	{
		m_Bone[0].fRotZ += 0.02f;
	}
	if (pInputKeyboard->GetPress(DIK_E) == true)
	{
		m_Bone[0].fRotZ -= 0.02f;
	}

	for (int nCnt = 0; nCnt < m_nNumBone; nCnt++)
	{
		m_pCubeBlock[nCnt]->SetRot(D3DXVECTOR3(m_Bone[nCnt].fRotX, m_Bone[nCnt].fRotY, m_Bone[nCnt].fRotZ));
	}


	VERTEX_3D* pVtx;	//頂点ポインタを所得

	//頂点バッファをロックし、両店情報へのポインタを所得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntVtx = 0; nCntVtx < m_nNumVertex; nCntVtx++)
	{
		//頂点座標の設定 
		pVtx[nCntVtx].pos.x = m_Vertex[nCntVtx].fPosX;
		pVtx[nCntVtx].pos.y = m_Vertex[nCntVtx].fPosY;
		pVtx[nCntVtx].pos.z = m_Vertex[nCntVtx].fPosZ;
	}

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//====================================================================
//テクスチャの割り当て
//====================================================================
void CSkinmesh::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//====================================================================
//頂点座標の設定
//====================================================================
void CSkinmesh::SetVerPos(D3DXVECTOR3 Pos0, D3DXVECTOR3 Pos1, D3DXVECTOR3 Pos2, D3DXVECTOR3 Pos3)
{
	VERTEX_3D* pVtx;	//頂点ポインタを所得

	//頂点バッファをロックし、両店情報へのポインタを所得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定 
	pVtx[0].pos = Pos0;
	pVtx[1].pos = Pos1;
	pVtx[2].pos = Pos2;
	pVtx[3].pos = Pos3;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//====================================================================
//頂点座標の設定
//====================================================================
void CSkinmesh::SetVerTex(void)
{
	VERTEX_3D* pVtx;	//頂点ポインタを所得

	//頂点バッファをロックし、両店情報へのポインタを所得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定 
	pVtx[0].pos = D3DXVECTOR3(-m_Width * 0.5f, +m_Height * 0.5f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(+m_Width * 0.5f, +m_Height * 0.5f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_Width * 0.5f, -m_Height * 0.5f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(+m_Width * 0.5f, -m_Height * 0.5f, 0.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//====================================================================
//テクスチャ座標の設定
//====================================================================
void CSkinmesh::SetAnim(D3DXVECTOR2 Tex)
{
	VERTEX_3D* pVtx;	//頂点ポインタを所得

	//頂点バッファをロックし、両店情報へのポインタを所得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(Tex.x, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, Tex.y);
	pVtx[3].tex = D3DXVECTOR2(Tex.x, Tex.y);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//====================================================================
//頂点カラーの設定
//====================================================================
void CSkinmesh::SetColor(D3DXCOLOR col)
{
	m_Color = col;

	VERTEX_3D* pVtx;	//頂点ポインタを所得

					//頂点バッファをロックし、両店情報へのポインタを所得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点カラーの設定
	pVtx[0].col = m_Color;
	pVtx[1].col = m_Color;
	pVtx[2].col = m_Color;
	pVtx[3].col = m_Color;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//====================================================================
//頂点カラーの設定
//====================================================================
void CSkinmesh::SetTexture(const char* name)
{
	CTexture* pTexture = CManager::GetInstance()->GetTexture();
	m_nIdxTexture = pTexture->Regist(name);
}

//====================================================================
//モーションのロード処理
//====================================================================
void CSkinmesh::LoadData(const char* pFilename)
{
	FILE* pFile; //ファイルポインタを宣言

	//ファイルを開く
	pFile = fopen(pFilename, "r");

	if (pFile != NULL)
	{//ファイルが開けた場合

		int nCntBone = 0;
		int nCntVertex = 0;
		int nCntModel = 0;
		int nCntParts = 0;
		int nCntMotion = 0;
		int nCntKeySet = 0;
		int nCntKey = 0;

		char aString[128] = {};				//ゴミ箱
		char aMessage[128] = {};			//スタートメッセージ
		char aBool[128] = {};				//bool変換用メッセージ
		char aTextureName[128] = {};		//テクスチャのパス名

		// 読み込み開始-----------------------------------------------------
		while (1)
		{//「SCRIPT」を探す
			fscanf(pFile, "%s", &aMessage[0]);
			if (strcmp(&aMessage[0], "SCRIPT") == 0)
			{
				// テクスチャ情報読み込み-----------------------------------------------------
				while (1)
				{//「TEXTURE_FILENAME」を探す
					fscanf(pFile, "%s", &aMessage[0]);
					if (strcmp(&aMessage[0], "TEXTURE_FILENAME") == 0)
					{
						fscanf(pFile, "%s", &aString[0]);
						fscanf(pFile, "%s", &aTextureName[0]);
						SetTexture(&aTextureName[0]);
						break;
					}
				}

				// ボーン情報読み込み-----------------------------------------------------
				while (1)
				{//「BONESET」を探す
					fscanf(pFile, "%s", &aMessage[0]);
					if (strcmp(&aMessage[0], "BONESET") == 0)
					{
						while (1)
						{//各種変数を探す
							fscanf(pFile, "%s", &aMessage[0]);
							if (strcmp(&aMessage[0], "NUM_BONE") == 0)
							{
								fscanf(pFile, "%s", &aString[0]);
								fscanf(pFile, "%d", &m_nNumBone);	//ボーンの総数を設定
								break;
							}
						}

						while (m_nNumBone != nCntBone)
						{//各種変数を探す
							fscanf(pFile, "%s", &aMessage[0]);
							if (strcmp(&aMessage[0], "BONE") == 0)
							{
								while (1)
								{//各種変数を探す
									fscanf(pFile, "%s", &aMessage[0]);
									if (strcmp(&aMessage[0], "PARENT") == 0)
									{
										fscanf(pFile, "%s", &aString[0]);
										fscanf(pFile, "%d", &m_Bone[nCntBone].nParent);	//親を設定
									}
									if (strcmp(&aMessage[0], "POS") == 0)
									{
										fscanf(pFile, "%s", &aString[0]);
										fscanf(pFile, "%f", &m_Bone[nCntBone].fPosX);	//位置を設定
										fscanf(pFile, "%f", &m_Bone[nCntBone].fPosY);	//位置を設定
										fscanf(pFile, "%f", &m_Bone[nCntBone].fPosZ);	//位置を設定
									}
									if (strcmp(&aMessage[0], "ROT") == 0)
									{
										fscanf(pFile, "%s", &aString[0]);
										fscanf(pFile, "%f", &m_Bone[nCntBone].fRotX);	//向きを設定
										fscanf(pFile, "%f", &m_Bone[nCntBone].fRotY);	//向きを設定
										fscanf(pFile, "%f", &m_Bone[nCntBone].fRotZ);	//向きを設定
									}
									if (strcmp(&aMessage[0], "END_BONE") == 0)
									{
										nCntBone++;
										break;
									}
								}
							}
						}
					}
					if (strcmp(&aMessage[0], "END_BONESET") == 0)
					{
						break;
					}
				}

				// 頂点情報読み込み-----------------------------------------------------
				while (1)
				{//「VERTEXSET」を探す
					fscanf(pFile, "%s", &aMessage[0]);
					if (strcmp(&aMessage[0], "VERTEXSET") == 0)
					{
						while (1)
						{//各種変数を探す
							fscanf(pFile, "%s", &aMessage[0]);
							if (strcmp(&aMessage[0], "NUM_VERTEX") == 0)
							{
								fscanf(pFile, "%s", &aString[0]);
								fscanf(pFile, "%d", &m_nNumVertex);	//頂点の総数を設定
								break;
							}
						}

						while (m_nNumVertex != nCntVertex)
						{//各種変数を探す
							fscanf(pFile, "%s", &aMessage[0]);
							if (strcmp(&aMessage[0], "VERTEX") == 0)
							{
								while (1)
								{
									fscanf(pFile, "%s", &aMessage[0]);
									if (strcmp(&aMessage[0], "POS") == 0)
									{
										fscanf(pFile, "%s", &aString[0]);
										fscanf(pFile, "%f", &m_Vertex[nCntVertex].fPosX);	//位置を設定
										fscanf(pFile, "%f", &m_Vertex[nCntVertex].fPosY);	//位置を設定
										fscanf(pFile, "%f", &m_Vertex[nCntVertex].fPosZ);	//位置を設定
									}
									if (strcmp(&aMessage[0], "BONE_REF") == 0)
									{
										fscanf(pFile, "%s", &aString[0]);
										fscanf(pFile, "%d", &m_Vertex[nCntVertex].BoneRef[0]);	//対応ボーンを設定
										fscanf(pFile, "%d", &m_Vertex[nCntVertex].BoneRef[1]);	//対応ボーンを設定
									}
									if (strcmp(&aMessage[0], "WEIGHT_VALUE") == 0)
									{
										fscanf(pFile, "%s", &aString[0]);
										fscanf(pFile, "%f", &m_Vertex[nCntVertex].WeightValue[0]);	//ウェイト値を設定
										fscanf(pFile, "%f", &m_Vertex[nCntVertex].WeightValue[1]);	//ウェイト値を設定
									}
									if (strcmp(&aMessage[0], "TEX") == 0)
									{
										fscanf(pFile, "%s", &aString[0]);
										fscanf(pFile, "%f", &m_Vertex[nCntVertex].Tex.x);	//テクスチャを設定
										fscanf(pFile, "%f", &m_Vertex[nCntVertex].Tex.y);	//テクスチャを設定
									}
									if (strcmp(&aMessage[0], "END_VERTEX") == 0)
									{
										nCntVertex++;
										break;
									}
								}
							}
						}
					}
					if (strcmp(&aMessage[0], "END_VERTEXSET") == 0)
					{
						break;
					}
				}


				// モーション情報読み込み-----------------------------------------------------
				while (1)
				{//「MOTIONSET」を探す
					fscanf(pFile, "%s", &aMessage[0]);
					if (strcmp(&aMessage[0], "MOTIONSET") == 0)
					{
						while (1)
						{//各種変数を探す
							fscanf(pFile, "%s", &aMessage[0]);
							if (strcmp(&aMessage[0], "LOOP") == 0)
							{
								fscanf(pFile, "%s", &aString[0]);
								fscanf(pFile, "%s", &aBool[0]);	//ループするかどうかを設定
								m_aInfo[nCntMotion].bLoop = (strcmp(&aBool[0], "1") == 0 ? true : false);			//bool型の書き方
							}
							if (strcmp(&aMessage[0], "NUM_KEY") == 0)
							{
								fscanf(pFile, "%s", &aString[0]);
								fscanf(pFile, "%d", &m_aInfo[nCntMotion].NumKey);	//キーの総数を設定
								break;
							}
						}

						// キー情報読み込み-----------------------------------------------------
						while (1)
						{//「KEYSET」を探す
							fscanf(pFile, "%s", &aMessage[0]);
							if (strcmp(&aMessage[0], "KEYSET") == 0)
							{
								while (1)
								{//「FRAME」を探す
									fscanf(pFile, "%s", &aMessage[0]);
									if (strcmp(&aMessage[0], "FRAME") == 0)
									{
										fscanf(pFile, "%s", &aString[0]);
										fscanf(pFile, "%d", &m_aInfo[nCntMotion].aKeyInfo[nCntKeySet].nFrame);	//キーフレームを設定
										break;
									}
								}

								while (1)
								{//「KEY」を探す
									fscanf(pFile, "%s", &aMessage[0]);
									if (strcmp(&aMessage[0], "KEY") == 0)
									{
										while (1)
										{//各種変数を探す
											fscanf(pFile, "%s", &aMessage[0]);
											if (strcmp(&aMessage[0], "POS") == 0)
											{
												fscanf(pFile, "%s", &aString[0]);
												fscanf(pFile, "%f", &m_aInfo[nCntMotion].aKeyInfo[nCntKeySet].aKey[nCntKey].fPosX);	//位置を設定
												fscanf(pFile, "%f", &m_aInfo[nCntMotion].aKeyInfo[nCntKeySet].aKey[nCntKey].fPosY);	//位置を設定
												fscanf(pFile, "%f", &m_aInfo[nCntMotion].aKeyInfo[nCntKeySet].aKey[nCntKey].fPosZ);	//位置を設定
											}
											if (strcmp(&aMessage[0], "ROT") == 0)
											{
												fscanf(pFile, "%s", &aString[0]);
												fscanf(pFile, "%f", &m_aInfo[nCntMotion].aKeyInfo[nCntKeySet].aKey[nCntKey].fRotX);	//位置を設定
												fscanf(pFile, "%f", &m_aInfo[nCntMotion].aKeyInfo[nCntKeySet].aKey[nCntKey].fRotY);	//位置を設定
												fscanf(pFile, "%f", &m_aInfo[nCntMotion].aKeyInfo[nCntKeySet].aKey[nCntKey].fRotZ);	//位置を設定
												break;
											}
										}
									}
									if (strcmp(&aMessage[0], "END_KEY") == 0)
									{
										nCntKey++;
										if (nCntKey >= nCntBone)
										{
											break;
										}
									}
								}
							}
							if (strcmp(&aMessage[0], "END_KEYSET") == 0)
							{
								nCntKey = 0;
								nCntKeySet++;
								if (nCntKeySet >= m_aInfo[nCntMotion].NumKey)
								{
									break;
								}
							}
						}
					}
					if (strcmp(&aMessage[0], "END_MOTIONSET") == 0)
					{
						nCntKeySet = 0;
						nCntMotion++;
					}
					else if (strcmp(&aMessage[0], "END_SCRIPT") == 0)
					{
						break;
					}
				}
			}
			if (strcmp(&aMessage[0], "END_SCRIPT") == 0)
			{
				break;
			}
		}
		fclose(pFile);
	}
	else
	{//ファイルが開けなかった場合
		printf("***ファイルを開けませんでした***\n");
	}
}