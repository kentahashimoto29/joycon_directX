//============================================
//
//	敵の処理 [enemy.cpp]
//	Author:sakamoto kai
//
//============================================
#include "enemy.h"
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
#include "player.h"
#include "useful.h"

#define COLLISION_SIZE (D3DXVECTOR3(10.0f,40.0f,10.0f))		//横の当たり判定
#define LIFE (30.0f)	// 体力

//====================================================================
//コンストラクタ
//====================================================================
CEnemy::CEnemy(int nPriority) :CObject(nPriority)
{
	SetSize(COLLISION_SIZE);
	m_pos = INITVECTOR3;
	m_move = INITVECTOR3;
	m_Objmove = INITVECTOR3;
	m_rot = D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f);
	m_bJump = false;
	m_nActionCount = 0;
	m_Action = ACTION_WAIT;
	m_AtkAction = ACTION_WAIT;
	m_nAttackHit = false;
	m_BState = BATTLESTATE_WALK;
	m_nStateCount = 0;
	m_AtkPos = INITVECTOR3;
	m_CollisionRot = 0.0f;
	m_nSeatchCount = 0;
	m_bSeatchLoop = false;
	m_State = STATE_SEATCH;
	m_fLife = LIFE;

	for (int nCnt = 0; nCnt < 32; nCnt++)
	{
		m_SeatchPos[nCnt] = D3DXVECTOR3(0.0f, -10000.0f, 0.0f);
	}

	for (int nCnt = 0; nCnt < 8; nCnt++)
	{
		m_ScreenPos[nCnt] = D3DXVECTOR3(-100.0f, -100.0f, 0.0f);
	}
}

//====================================================================
//デストラクタ
//====================================================================
CEnemy::~CEnemy()
{

}

//====================================================================
//生成処理
//====================================================================
CEnemy* CEnemy::Create()
{
	CEnemy* pPlayer = NULL;

	if (pPlayer == NULL)
	{
		//プレイヤーの生成
		pPlayer = new CEnemy();
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
HRESULT CEnemy::Init(void)
{
	//モデルの生成
	LoadLevelData("data\\TXT\\motion_ManBase.txt");

	//モーションの生成
	if (m_pMotion == NULL)
	{
		//モーションの生成
		m_pMotion = new CMotion;
	}

	//初期化処理
	m_pMotion->SetModel(&m_apModel[0], m_nNumModel);
	m_pMotion->LoadData("data\\TXT\\motion_ManBase.txt");

	//種類設定
	SetType(CObject::TYPE_ENEMY3D);

	switch (CScene::GetMode())
	{
	case CScene::MODE_TITLE:
		m_pMotion->Set(ACTION_WAIT, 0);
		break;

	case CScene::MODE_GAME:
		m_SState = SEARCHSTATE_WALK;
		break;

	case CScene::MODE_RESULT:
		break;
	}

	return S_OK;
}

//====================================================================
//自分が保持するオブジェクトの生成
//====================================================================
void CEnemy::MyObjCreate(void)
{

}

//====================================================================
//終了処理
//====================================================================
void CEnemy::Uninit(void)
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
void CEnemy::Update(void)
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
void CEnemy::TitleUpdate(void)
{
	//モーションの更新
	m_pMotion->Update();
}

//====================================================================
//ゲームでの更新処理
//====================================================================
void CEnemy::GameUpdate(void)
{
	//デバッグ表示の取得
	CDebugProc* pDebugProc = CManager::GetInstance()->GetDebugProc();

	////角度変更
	//CInputKeyboard* pKey = CManager::GetInstance()->GetInputKeyboard();

	//スクリーン座標への変換処理
	ScreenCollision();
//
//#ifdef _DEBUG
//
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
	//		SCREEM_CENTER.x + sinf(m_CollisionRot) * 50.0f * nCnt,
	//		SCREEM_CENTER.y + cosf(m_CollisionRot) * 50.0f * nCnt,
	//		0.0f));

	//	pEffect = C2DEffect::Create();
	//	pEffect->SetRadius(30.0f);
	//	pEffect->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	//	pEffect->SetPos(D3DXVECTOR3(
	//		SCREEM_CENTER.x + sinf(m_CollisionRot) * -50.0f * nCnt,
	//		SCREEM_CENTER.y + cosf(m_CollisionRot) * -50.0f * nCnt,
	//		0.0f));
	//}
//
//	//デバッグ用エフェクト
//	for (int nCnt = 0; nCnt < 8; nCnt++)
//	{
//		if (useful::CameraFront(GetPos()) == true)
//		{
//			C2DEffect* pEffect = C2DEffect::Create();
//			pEffect->SetRadius(50.0f);
//			pEffect->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
//			pEffect->SetPos(m_ScreenPos[nCnt]);
//		}
//	}
//
//#endif
//
//	//プレイヤーと敵の位置関係を示す外積
//	int Left = 0;
//	int	Right = 0;
//	for (int nCnt = 0; nCnt < 8; nCnt++)
//	{
//		D3DXVECTOR3 vecToPos;
//		D3DXVECTOR3 vecLine;
//		float A;
//
//		vecToPos.x = (m_ScreenPos[nCnt].x) - (SCREEM_CENTER.x);
//		vecToPos.y = (m_ScreenPos[nCnt].y) - (SCREEM_CENTER.y);
//
//		vecLine.x = sinf(m_CollisionRot);
//		vecLine.y = cosf(m_CollisionRot);
//		vecLine.z = 0.0f;
//
//		A = (vecLine.y * vecToPos.x) - (vecLine.x * vecToPos.y);
//
//		if (A < 0)
//		{
//			Right++;
//		}
//		else
//		{
//			Left++;
//		}
//	}
//
//	pDebugProc->Print("左 : 右 → %d : %d\n", Left, Right);

	//if (useful::SlashCollision(&m_ScreenPos[0], 8, m_CollisionRot) == true)
	//{
	//	pDebugProc->Print("TRUE\n");
	//}
	//else
	//{
	//	pDebugProc->Print("FALSE\n");
	//}

	//減衰係数
	m_move.x = m_move.x * 0.5f;
	if (m_move.x <= 0.0001f && m_move.x >= -0.0001f)
	{
		m_move.x = 0.0f;
	}

	m_move.z = m_move.z * 0.5f;
	if (m_move.z <= 0.0001f && m_move.z >= -0.0001f)
	{
		m_move.z = 0.0f;
	}

	if (m_bJump == true)
	{
		m_Objmove.x = m_Objmove.x * 0.25f;
		if (m_Objmove.x <= 0.0001f && m_Objmove.x >= -0.0001f)
		{
			m_Objmove.x = 0.0f;
		}

		m_Objmove.z = m_Objmove.z * 0.25f;
		if (m_Objmove.z <= 0.0001f && m_Objmove.z >= -0.0001f)
		{
			m_Objmove.z = 0.0f;
		}
	}

	//Y軸の位置更新
	m_pos.y += m_move.y * CManager::GetInstance()->GetGameSpeed();
	m_pos.y += m_Objmove.y * CManager::GetInstance()->GetGameSpeed();

	//X軸の位置更新
	m_pos.x += m_move.x * CManager::GetInstance()->GetGameSpeed();
	m_pos.x += m_Objmove.x * CManager::GetInstance()->GetGameSpeed();

	//Z軸の位置更新
	m_pos.z += m_move.z * CManager::GetInstance()->GetGameSpeed();
	m_pos.z += m_Objmove.z * CManager::GetInstance()->GetGameSpeed();

	//モーションの管理
	ActionState();

	//状態の管理
	switch (m_State)
	{
	case CEnemy::STATE_BATTLE:

		//戦闘状態の管理
		BattleStateManager();

		break;

	case CEnemy::STATE_SEATCH:

		//探索状態の管理
		SeatchStateManager();
		
		break;

	case CEnemy::STATE_DEATH:

		// 死亡処理
		Death();

		break;

	default:
		break;
	}

	if (m_pMotion != nullptr)
	{
		//モーションの更新
		m_pMotion->Update();
	}
}

//====================================================================
//モーションと状態の管理
//====================================================================
void CEnemy::ActionState(void)
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
//戦闘状態管理
//====================================================================
void CEnemy::BattleStateManager(void)
{
	CPlayer* pPlayer = CGame::GetPlayer();

	switch (m_BState)
	{
	case BATTLESTATE_NORMAL:
		break;

	case BATTLESTATE_DEATH:

		break;

	case BATTLESTATE_WAIT:
		break;

	case BATTLESTATE_DAMAGE:
		break;

	case BATTLESTATE_WALK:

		m_rot.y = atan2f(pPlayer->GetPos().z - m_pos.z, m_pos.x - pPlayer->GetPos().x) + D3DX_PI * 0.5f;

		if (useful::CollisionCircle(m_pos, pPlayer->GetPos(), 200.0f) == false)
		{
			m_move.x = sinf(D3DX_PI + m_rot.y) * 3.0f;
			m_move.z = cosf(D3DX_PI + m_rot.y) * 3.0f;
		}
		else if (useful::CollisionCircle(m_pos, pPlayer->GetPos(), 150.0f) == true)
		{
			m_move.x = -sinf(D3DX_PI + m_rot.y) * 3.0f;
			m_move.z = -cosf(D3DX_PI + m_rot.y) * 3.0f;
		}

		CollisionPush();

		break;
	}

	if (m_nStateCount > 0)
	{
		m_nStateCount--;
	}
}

//====================================================================
//探索状態管理
//====================================================================
void CEnemy::SeatchStateManager(void)
{
	float fAngle = 0.0f;

	switch (m_SState)
	{
	case SEARCHSTATE_NORMAL:
		break;

	case SEARCHSTATE_WALK:

		m_move.x = sinf(D3DX_PI + m_rot.y) * 3.0f;
		m_move.y = 0.0f;
		m_move.z = cosf(D3DX_PI + m_rot.y) * 3.0f;

		if (m_SeatchPos[m_nSeatchCount + 1].y <= -10000.0f)
		{
			if (useful::CollisionCircle(m_pos, m_SeatchPos[0], 5.0f) == true)
			{
				m_SState = SEARCHSTATE_TRUN;
				m_nSeatchCount = 0;
			}
		}
		else
		{
			if (useful::CollisionCircle(m_pos, m_SeatchPos[m_nSeatchCount + 1], 5.0f) == true)
			{
				m_SState = SEARCHSTATE_TRUN;
				m_nSeatchCount += 1;
			}
		}

		CManager::GetInstance()->GetDebugProc()->Print("[敵] %f : %f : %f\n", m_pos.x, m_pos.y, m_pos.z);

		break;

	case SEARCHSTATE_TRUN:

		if (m_SeatchPos[m_nSeatchCount + 1].y <= -10000.0f)
		{
			if (m_bSeatchLoop == true)
			{
				fAngle = atan2f(m_SeatchPos[0].z - m_pos.z, m_pos.x - m_SeatchPos[0].x);
				m_rot.y = fAngle + D3DX_PI * 0.5f;
				m_SState = SEARCHSTATE_WALK;
			}
			else
			{
				m_SState = SEARCHSTATE_WAIT;
			}

		}
		else
		{
			fAngle = atan2f(m_SeatchPos[m_nSeatchCount + 1].z - m_pos.z, m_pos.x - m_SeatchPos[m_nSeatchCount + 1].x);
			m_rot.y = fAngle + D3DX_PI * 0.5f;
			m_SState = SEARCHSTATE_WALK;
		}

		break;

	case SEARCHSTATE_WAIT:
		break;

	case SEARCHSTATE_SEARCH:
		break;
	}
}

//====================================================================
//自分以外の敵を押し出す処理
//====================================================================
void CEnemy::CollisionPush(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		//オブジェクトを取得
		CObject* pObj = CObject::GetTop(nCntPriority);

		while (pObj != NULL)
		{
			CObject* pObjNext = pObj->GetNext();

			CObject::TYPE type = pObj->GetType();			//種類を取得
			float fAngle = 0.0f;

			if (type == TYPE_ENEMY3D && pObj != this)
			{//種類がブロックの時
				CEnemy* pEnemy = (CEnemy*)pObj;

				if (useful::CollisionCircle(m_pos, pEnemy->GetPos(), 30.0f) == true)
				{
					fAngle = atan2f(pEnemy->GetPos().z - m_pos.z, m_pos.x - pEnemy->GetPos().x) + D3DX_PI * 0.5f;
					m_move.x = sinf(fAngle) * 0.2f;
					m_move.z = cosf(fAngle) * 0.2f;
				}
			}

			pObj = pObjNext;
		}
	}
}

//====================================================================
//描画処理
//====================================================================
void CEnemy::Draw(void)
{
	//LPDIRECT3DSURFACE9 pRenderDef, pZBuffDef;
	//D3DVIEWPORT9 viewportDef;
	//D3DXMATRIX mtxViewDef, mtxProjectionDef;

	////カメラの取得
	//CCamera* pCamera = CManager::GetInstance()->GetCamera();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	////現在のレンダリングターゲットを取得(保存)
	//pDevice->GetRenderTarget(0, &pRenderDef);

	////現在のZバッファを取得(保存)
	//pDevice->GetDepthStencilSurface(&pZBuffDef);

	////現在のビューポートを取得(保存)
	//pDevice->GetViewport(&viewportDef);

	//CManager::GetInstance()->GetRenderer()->ChageTarget(pCamera->GetPosV(), pCamera->GetPosR(), pCamera->GetVecU());

	////レンダリングターゲット用のテクスチャのクリア
	//pDevice->Clear(0, NULL,
	//	(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
	//	D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	////-----------------------------------------------------------------------------------

	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

	////ワールドマトリックスの初期化
	//D3DXMatrixIdentity(&m_mtxWorld);

	////向きを反映
	//D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);

	//D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	////位置を反映
	//D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);

	//D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	////ワールドマトリックスの設定
	//pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	////モデルの描画(全パーツ)
	//for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	//{
	//	m_apModel[nCntModel]->Draw();
	//}

	////-----------------------------------------------------------------------------------

	////元のレンダリングターゲットに戻す
	//pDevice->SetRenderTarget(0, pRenderDef);

	////元のZバッファに戻す
	//pDevice->SetDepthStencilSurface(pZBuffDef);

	////元のビューポートに戻す
	//pDevice->SetViewport(&viewportDef);

	////-----------------------------------------------------------------------------------

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
//スクリーン座標に変換する処理
//====================================================================
void CEnemy::ScreenCollision()
{
	//頂点設定
	D3DXVECTOR3 pos[8] = { INITVECTOR3 };

	pos[0] = D3DXVECTOR3(GetPos().x - COLLISION_SIZE.x, GetPos().y, GetPos().z + COLLISION_SIZE.z);
	pos[1] = D3DXVECTOR3(GetPos().x - COLLISION_SIZE.x, GetPos().y + COLLISION_SIZE.y, GetPos().z + COLLISION_SIZE.z);
	pos[2] = D3DXVECTOR3(GetPos().x + COLLISION_SIZE.x, GetPos().y, GetPos().z + COLLISION_SIZE.z);
	pos[3] = D3DXVECTOR3(GetPos().x + COLLISION_SIZE.x, GetPos().y + COLLISION_SIZE.y, GetPos().z + COLLISION_SIZE.z);
	pos[4] = D3DXVECTOR3(GetPos().x - COLLISION_SIZE.x, GetPos().y, GetPos().z - COLLISION_SIZE.z);
	pos[5] = D3DXVECTOR3(GetPos().x - COLLISION_SIZE.x, GetPos().y + COLLISION_SIZE.y, GetPos().z - COLLISION_SIZE.z);
	pos[6] = D3DXVECTOR3(GetPos().x + COLLISION_SIZE.x, GetPos().y, GetPos().z - COLLISION_SIZE.z);
	pos[7] = D3DXVECTOR3(GetPos().x + COLLISION_SIZE.x, GetPos().y + COLLISION_SIZE.y, GetPos().z - COLLISION_SIZE.z);

	//ビューポートの設定
	D3DVIEWPORT9 viewport;
	viewport.X = 0;
	viewport.Y = 0;
	viewport.Width = SCREEN_WIDTH;
	viewport.Height = SCREEN_HEIGHT;
	viewport.MaxZ = 0.0f;
	viewport.MinZ = 0.0f;

	//マトリックスの取得
	D3DXMATRIX ViewMatrix = CManager::GetInstance()->GetCamera()->GetViewMatrix();
	D3DXMATRIX ProjectionMatrix = CManager::GetInstance()->GetCamera()->GetProjectionMatrix();

	//D3DXVec3Project(&screenPosition, &pos, &viewport, &projectionMatrix, &viewMatrix, D3DXMatrixIdentity(&mtx));

	//3Dの座標をスクリーン座標に変換
	for (int nCnt = 0; nCnt < 8; nCnt++)
	{
		D3DXVec3Project(&m_ScreenPos[nCnt], &pos[nCnt], &viewport, &ProjectionMatrix, &ViewMatrix, D3DXMatrixIdentity(&m_mtxWorld));
	}
}

//====================================================================
//ロード処理
//====================================================================
void CEnemy::LoadLevelData(const char* pFilename)
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

//====================================================================
// 接触ダメージ処理
//====================================================================
void CEnemy::HitDamage(float fDamage)
{
	m_fLife -= fDamage;

	if (m_fLife <= 0)
	{
		// 状態設定
		m_State = STATE_DEATH;
	}
}

//====================================================================
// 死亡処理
//====================================================================
void CEnemy::Death(void)
{
	// 終了処理
	Uninit();
}