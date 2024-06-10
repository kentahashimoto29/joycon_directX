//============================================
//
//	階層構造でモーションを行うモデルのサンプル [SampleLevelModel.cpp]
//	Author:sakamoto kai
//
//============================================
#include "SampleLevelModel.h"
#include "object.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "motion.h"
#include "game.h"
#include "object3D.h"
#include "debugproc.h"
#include "2DEffect.h"
#include "camera.h"
#include "input.h"

#define COLLISION_SIZE (D3DXVECTOR3(20.0f,40.0f,20.0f))		//横の当たり判定

//====================================================================
//コンストラクタ
//====================================================================
CSampleLvModel::CSampleLvModel(int nPriority) :CObject(nPriority)
{
	SetWidth(COLLISION_SIZE.x);
	SetHeight(COLLISION_SIZE.y);
	m_pos = INITVECTOR3;
	m_move = INITVECTOR3;
	m_Objmove = INITVECTOR3;
	m_rot = D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f);
	m_bJump = false;
	m_nActionCount = 0;
	m_Action = ACTION_WAIT;
	m_AtkAction = ACTION_WAIT;
	m_nAttackHit = false;
	m_State = STATE_NORMAL;
	m_nStateCount = 0;
	m_AtkPos = INITVECTOR3;
	m_CollisionRot = 0.0f;
}

//====================================================================
//デストラクタ
//====================================================================
CSampleLvModel::~CSampleLvModel()
{

}

//====================================================================
//生成処理
//====================================================================
CSampleLvModel* CSampleLvModel::Create()
{
	CSampleLvModel* pPlayer = NULL;

	if (pPlayer == NULL)
	{
		//プレイヤーの生成
		pPlayer = new CSampleLvModel();
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
HRESULT CSampleLvModel::Init(void)
{
	//モデルの生成
	LoadLevelData("data\\TXT\\motion_player.txt");

	//モーションの生成
	if (m_pMotion == NULL)
	{
		//モーションの生成
		m_pMotion = new CMotion;
	}

	//初期化処理
	m_pMotion->SetModel(&m_apModel[0], m_nNumModel);
	m_pMotion->LoadData("data\\TXT\\motion_player.txt");

	//種類設定
	SetType(CObject::TYPE_SAMPLE);

	switch (CScene::GetMode())
	{
	case CScene::MODE_TITLE:
		m_pMotion->Set(ACTION_TITLE, 0);
		break;

	case CScene::MODE_GAME:
		break;

	case CScene::MODE_RESULT:
		break;
	}

	return S_OK;
}

//====================================================================
//自分が保持するオブジェクトの生成
//====================================================================
void CSampleLvModel::MyObjCreate(void)
{

}

//====================================================================
//終了処理
//====================================================================
void CSampleLvModel::Uninit(void)
{
	for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{
		m_apModel[nCntModel]->Uninit();
		delete m_apModel[nCntModel];
		m_apModel[nCntModel] = NULL;
	}

	//モーションの終了処理
	if (m_pMotion != NULL)
	{
		//モーションの破棄
		delete m_pMotion;
		m_pMotion = NULL;
	}

	SetDeathFlag(true);
}

//====================================================================
//更新処理
//====================================================================
void CSampleLvModel::Update(void)
{
	switch (CScene::GetMode())
	{
	case CScene::MODE_TITLE:
		TitleUpdate();
		break;

	case CScene::MODE_GAME:
		GameUpdate();
		break;

	case CScene::MODE_RESULT:
		break;
	}
}

//====================================================================
//タイトルでの更新処理
//====================================================================
void CSampleLvModel::TitleUpdate(void)
{
	//モーションの更新
	m_pMotion->Update();
}

//====================================================================
//ゲームでの更新処理
//====================================================================
void CSampleLvModel::GameUpdate(void)
{
	//デバッグ表示の取得
	CDebugProc* pDebugProc = CManager::GetInstance()->GetDebugProc();

	//D3DXVECTOR3 ScreenCenter = D3DXVECTOR3(640.0f,360.0f,0.0f);
	//D3DXVECTOR3 ScreenPos[4] = { INITVECTOR3 };
	//D3DXVECTOR3 pos[4] = { INITVECTOR3 };

	//pos[0] = D3DXVECTOR3(GetPos().x - COLLISION_SIZE.x, GetPos().y, GetPos().z);
	//pos[1] = D3DXVECTOR3(GetPos().x - COLLISION_SIZE.x, GetPos().y + COLLISION_SIZE.y, GetPos().z);
	//pos[2] = D3DXVECTOR3(GetPos().x + COLLISION_SIZE.x, GetPos().y, GetPos().z);
	//pos[3] = D3DXVECTOR3(GetPos().x + COLLISION_SIZE.x, GetPos().y + COLLISION_SIZE.y, GetPos().z);

	////ビューポートの設定
	//D3DVIEWPORT9 viewport;
	//viewport.X = 0;
	//viewport.Y = 0;
	//viewport.Width = SCREEN_WIDTH;
	//viewport.Height = SCREEN_HEIGHT;
	//viewport.MaxZ = 0.0f;
	//viewport.MinZ = 0.0f;

	////マトリックスの取得
	//D3DXMATRIX ViewMatrix = CManager::GetInstance()->GetCamera()->GetViewMatrix();
	//D3DXMATRIX ProjectionMatrix = CManager::GetInstance()->GetCamera()->GetProjectionMatrix();

	////D3DXVec3Project(&screenPosition, &pos, &viewport, &projectionMatrix, &viewMatrix, D3DXMatrixIdentity(&mtx));

	////3Dの座標をスクリーン座標に変換
	//for (int nCnt = 0; nCnt < 4; nCnt++)
	//{
	//	D3DXVec3Project(&ScreenPos[nCnt], &pos[nCnt], &viewport, &ProjectionMatrix, &ViewMatrix, D3DXMatrixIdentity(&m_mtxWorld));

	//	C2DEffect* pEffect = C2DEffect::Create();
	//	pEffect->SetRadius(50.0f);
	//	pEffect->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	//	pEffect->SetPos(ScreenPos[nCnt]);
	//}

	////角度変更
	//CInputKeyboard* pKey = CManager::GetInstance()->GetInputKeyboard();

	//if (pKey->GetPress(DIK_RIGHT) == true)
	//{
	//	m_CollisionRot += 0.01f;
	//}

	//if (pKey->GetPress(DIK_LEFT) == true)
	//{
	//	m_CollisionRot -= 0.01f;
	//}

	////デバッグ用のエフェクト生成
	//for (int nCnt = 0; nCnt < 10; nCnt++)
	//{
	//	C2DEffect* pEffect = C2DEffect::Create();
	//	pEffect->SetRadius(30.0f);
	//	pEffect->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	//	pEffect->SetPos(D3DXVECTOR3(
	//		ScreenCenter.x + sinf(m_CollisionRot) * 50.0f * nCnt,
	//		ScreenCenter.y + cosf(m_CollisionRot) * 50.0f * nCnt,
	//		0.0f));

	//	pEffect = C2DEffect::Create();
	//	pEffect->SetRadius(30.0f);
	//	pEffect->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	//	pEffect->SetPos(D3DXVECTOR3(
	//		ScreenCenter.x + sinf(m_CollisionRot) * -50.0f * nCnt,
	//		ScreenCenter.y + cosf(m_CollisionRot) * -50.0f * nCnt,
	//		0.0f));
	//}

	////プレイヤーと敵の位置関係を示す外積
	//int Left = 0;
	//int	Right = 0;
	//for (int nCnt = 0; nCnt < 4; nCnt++)
	//{
	//	D3DXVECTOR3 vecToPos;
	//	D3DXVECTOR3 vecLine;
	//	float A;

	//	vecToPos.x = (ScreenPos[nCnt].x) - (ScreenCenter.x);
	//	vecToPos.y = (ScreenPos[nCnt].y) - (ScreenCenter.y);

	//	vecLine.x = sinf(m_CollisionRot);
	//	vecLine.y = cosf(m_CollisionRot);
	//	vecLine.z = 0.0f;

	//	A = (vecLine.y * vecToPos.x) - (vecLine.x * vecToPos.y);

	//	if (A < 0)
	//	{
	//		Right++;
	//	}
	//	else
	//	{
	//		Left++;
	//	}
	//}

	//pDebugProc->Print("左 : 右 → %d : %d\n", Left, Right);



	//モーションの管理
	ActionState();

	//状態の管理
	StateManager();

	//モーションの更新
	m_pMotion->Update();
}

//====================================================================
//モーションと状態の管理
//====================================================================
void CSampleLvModel::ActionState(void)
{
	//ジャンプモーション
	if (m_bJump == true)
	{
		if (m_Action != ACTION_JAMP)
		{
			m_Action = ACTION_JAMP;
			m_pMotion->Set(ACTION_JAMP, 5);
		}
	}
	//移動モーション
	else if (m_move.x > 0.1f || m_move.x < -0.1f || m_move.z > 0.1f || m_move.z < -0.1f)
	{
		if (m_Action != ACTION_MOVE)
		{
			m_Action = ACTION_MOVE;
			m_pMotion->Set(ACTION_MOVE, 5);
		}
	}
	//ニュートラルモーション
	else
	{
		if (m_Action != ACTION_WAIT)
		{
			m_Action = ACTION_WAIT;
			m_pMotion->Set(ACTION_WAIT, 5);
		}
	}
}

//====================================================================
//状態管理
//====================================================================
void CSampleLvModel::StateManager(void)
{
	switch (m_State)
	{
	case STATE_NORMAL:
		break;

	case STATE_DEATH:
		break;

	case STATE_WAIT:
		break;

	case STATE_DAMAGE:
		break;
	}

	if (m_nStateCount > 0)
	{
		m_nStateCount--;
	}
}

//====================================================================
//描画処理
//====================================================================
void CSampleLvModel::Draw(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//モデルの描画(全パーツ)
	for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{
		m_apModel[nCntModel]->Draw();
	}
}

//====================================================================
//ロード処理
//====================================================================
void CSampleLvModel::LoadLevelData(const char* pFilename)
{
	FILE* pFile; //ファイルポインタを宣言

	//ファイルを開く
	pFile = fopen(pFilename, "r");

	if (pFile != NULL)
	{//ファイルが開けた場合

		int ModelParent = 0;
		D3DXVECTOR3 ModelPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 ModelRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		char ModelName[128] = {};
		int nCntModel = 0;
		int nCntParts = 0;
		int nCntMotion = 0;
		int nCntKeySet = 0;
		int nCntKey = 0;

		char aString[128] = {};				//ゴミ箱
		char aMessage[128] = {};			//スタートとエンドのメッセージ
		char aBool[128] = {};				//bool変換用メッセージ

		// 読み込み開始-----------------------------------------------------
		while (1)
		{//「SCRIPT」を探す
			fscanf(pFile, "%s", &aMessage[0]);
			if (strcmp(&aMessage[0], "SCRIPT") == 0)
			{
				// モデル数読み込み-----------------------------------------------------
				while (1)
				{//「NUM_MODEL」を探す
					fscanf(pFile, "%s", &aMessage[0]);
					if (strcmp(&aMessage[0], "NUM_MODEL") == 0)
					{
						fscanf(pFile, "%s", &aString[0]);
						fscanf(pFile, "%d", &m_nNumModel);		//モデル数の設定
						break;
					}
				}

				//モデルファイルの読み込み
				while (1)
				{//「MODEL_FILENAME」を探す
					fscanf(pFile, "%s", &aMessage[0]);
					if (strcmp(&aMessage[0], "MODEL_FILENAME") == 0)
					{
						fscanf(pFile, "%s", &aString[0]);
						fscanf(pFile, "%s", &ModelName[0]);		//読み込むモデルのパスを取得

						m_apModel[nCntModel] = CModel::Create(&ModelName[0]);
						nCntModel++;
					}
					if (nCntModel >= m_nNumModel)
					{
						nCntModel = 0;
						break;
					}
				}

				// キャラクター情報読み込み-----------------------------------------------------
				while (1)
				{//「PARTSSET」を探す
					fscanf(pFile, "%s", &aMessage[0]);
					if (strcmp(&aMessage[0], "PARTSSET") == 0)
					{
						while (1)
						{//各種変数を探す
							fscanf(pFile, "%s", &aMessage[0]);
							if (strcmp(&aMessage[0], "INDEX") == 0)
							{
								fscanf(pFile, "%s", &aString[0]);
								fscanf(pFile, "%d", &nCntModel);	//インデックスを設定
							}
							if (strcmp(&aMessage[0], "PARENT") == 0)
							{
								fscanf(pFile, "%s", &aString[0]);
								fscanf(pFile, "%d", &ModelParent);	//親モデルのインデックスを設定

								if (ModelParent == -1)
								{
									m_apModel[nCntModel]->SetParent(NULL);
								}
								else
								{
									m_apModel[nCntModel]->SetParent(m_apModel[ModelParent]);
								}
							}
							if (strcmp(&aMessage[0], "POS") == 0)
							{
								fscanf(pFile, "%s", &aString[0]);
								fscanf(pFile, "%f", &ModelPos.x);				//位置(オフセット)の初期設定
								fscanf(pFile, "%f", &ModelPos.y);				//位置(オフセット)の初期設定
								fscanf(pFile, "%f", &ModelPos.z);				//位置(オフセット)の初期設定

								m_apModel[nCntModel]->SetPos(ModelPos);
								m_apModel[nCntModel]->SetStartPos(ModelPos);
							}
							if (strcmp(&aMessage[0], "ROT") == 0)
							{
								fscanf(pFile, "%s", &aString[0]);
								fscanf(pFile, "%f", &ModelRot.x);				////向きの初期設定
								fscanf(pFile, "%f", &ModelRot.y);				////向きの初期設定
								fscanf(pFile, "%f", &ModelRot.z);				////向きの初期設定

								m_apModel[nCntModel]->SetRot(ModelRot);
								m_apModel[nCntModel]->SetStartRot(ModelRot);
							}
							if (strcmp(&aMessage[0], "END_PARTSSET") == 0)
							{
								break;
							}
						}
						nCntModel++;
						if (nCntModel >= m_nNumModel)
						{
							break;
						}
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