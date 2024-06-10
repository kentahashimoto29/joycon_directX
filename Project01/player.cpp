//============================================
//
//	プレイヤーの処理 [player.cpp]
//	Author:sakamoto kai
//
//============================================
#include "player.h"
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
#include "enemy.h"
#include "useful.h"
#include "Aim.h"
#include "Shuriken.h"
#include "objGauge2D.h"

#define COLLISION_SIZE (D3DXVECTOR3(20.0f,40.0f,20.0f))		//横の当たり判定
#define PLAYER_SPEED (10.0f)		//プレイヤーの移動速度

namespace
{
	const float MAX_LIFE = 100.0f;			//体力最大値
	const float EVADION_MOVE = 40.0f;		//回避移動量
	const int EVADION_FRAME = 7;		//回避フレーム

}

//====================================================================
//コンストラクタ
//====================================================================
CPlayer::CPlayer(int nPriority) :CObject(nPriority)
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
	m_HeightCameraRot = 0.0f;
	m_fLife = MAX_LIFE;
	m_fLifeMax = MAX_LIFE;
	m_pLifeGauge = nullptr;

	m_pTgtEnemy = nullptr;
}

//====================================================================
//デストラクタ
//====================================================================
CPlayer::~CPlayer()
{

}

//====================================================================
//生成処理
//====================================================================
CPlayer* CPlayer::Create()
{
	CPlayer* pPlayer = NULL;

	if (pPlayer == NULL)
	{
		//プレイヤーの生成
		pPlayer = new CPlayer();
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
HRESULT CPlayer::Init(void)
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

	//体力ゲージ生成
	if (CScene::GetMode() == CScene::MODE_GAME)
	{
		LifeGaugeCreate();
	}

	//種類設定
	SetType(CObject::TYPE_PLAYER3D);

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
void CPlayer::MyObjCreate(void)
{

}

//====================================================================
//終了処理
//====================================================================
void CPlayer::Uninit(void)
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
void CPlayer::Update(void)
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
void CPlayer::TitleUpdate(void)
{
	//モーションの更新
	m_pMotion->Update();
}

//====================================================================
//ゲームでの更新処理
//====================================================================
void CPlayer::GameUpdate(void)
{
	//デバッグ表示の取得
	CDebugProc* pDebugProc = CManager::GetInstance()->GetDebugProc();

	//// 移動処理
	//Move();

	//// 向き移動処理
	//Rot();

	// 追尾処理
	Tracking();

	// 手裏剣処理
	Shuriken();
	Evasion();	//8,9キーでデバッグ

	// カメラ更新処理
	CameraPosUpdate();

	// 位置更新処理
	PosUpdate();

	// 向き更新処理
	RotUpdate();

	//敵を戦闘状態へ変更
	CollisionEnemy();

	//モーションの管理
	ActionState();

	//ライフゲージの位置管理
	if (m_pLifeGauge != nullptr)
	{
		m_pLifeGauge->SetGaugeWidth(m_fLifeMax, m_fLife);
	}

	//状態の管理
	StateManager();

	//モーションの更新
	m_pMotion->Update();


	CGame::GetAim()->GetPos();

	DebugKey();

	CManager::GetInstance()->GetDebugProc()->Print("[自分]位置 %f : %f : %f\n", m_pos.x, m_pos.y, m_pos.z);
	CManager::GetInstance()->GetDebugProc()->Print("[自分]向き %f : %f : %f\n",m_rot.x, m_rot.y, m_rot.z);
}

//====================================================================
//描画処理
//====================================================================
void CPlayer::Draw(void)
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
//移動処理
//====================================================================
void CPlayer::Move(void)
{
	//キーボードの取得
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputJoypad* pInputJoypad = CManager::GetInstance()->GetInputJoyPad();
	D3DXVECTOR3 CameraRot = CManager::GetInstance()->GetCamera()->GetRot();

	D3DXVECTOR3 NormarizeMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//キーボードの移動処理
	if (pInputKeyboard->GetPress(DIK_W) == true)
	{
		NormarizeMove.z += 1.0f * cosf(CameraRot.y) * PLAYER_SPEED;
		NormarizeMove.x += 1.0f * sinf(CameraRot.y) * PLAYER_SPEED;

	}
	if (pInputKeyboard->GetPress(DIK_S) == true)
	{
		NormarizeMove.z += -1.0f * cosf(CameraRot.y) * PLAYER_SPEED;
		NormarizeMove.x += -1.0f * sinf(CameraRot.y) * PLAYER_SPEED;
	}
	if (pInputKeyboard->GetPress(DIK_A) == true)
	{
		NormarizeMove.x += -1.0f * cosf(CameraRot.y) * PLAYER_SPEED;
		NormarizeMove.z -= -1.0f * sinf(CameraRot.y) * PLAYER_SPEED;

	}
	if (pInputKeyboard->GetPress(DIK_D) == true)
	{
		NormarizeMove.x += 1.0f * cosf(CameraRot.y) * PLAYER_SPEED;
		NormarizeMove.z -= 1.0f * sinf(CameraRot.y) * PLAYER_SPEED;
	}

	if (pInputKeyboard->GetPress(DIK_W) == false && pInputKeyboard->GetPress(DIK_A) == false && pInputKeyboard->GetPress(DIK_S) == false && pInputKeyboard->GetPress(DIK_D) == false)
	{
		//左スティックによる前後移動	
		m_move.z += pInputJoypad->Get_Stick_Left(0).y * cosf(CameraRot.y) * PLAYER_SPEED;
		m_move.x += pInputJoypad->Get_Stick_Left(0).y * sinf(CameraRot.y) * PLAYER_SPEED;

		//左スティックによる左右移動
		m_move.x += pInputJoypad->Get_Stick_Left(0).x * cosf(CameraRot.y) * PLAYER_SPEED;
		m_move.z -= pInputJoypad->Get_Stick_Left(0).x * sinf(CameraRot.y) * PLAYER_SPEED;
	}

	if (pInputKeyboard->GetPress(DIK_W) == true || pInputKeyboard->GetPress(DIK_A) == true || pInputKeyboard->GetPress(DIK_S) == true || pInputKeyboard->GetPress(DIK_D) == true)
	{
		float JunpPawer = NormarizeMove.y;
		NormarizeMove.y = 0.0f;

		D3DXVec3Normalize(&NormarizeMove, &NormarizeMove);

		NormarizeMove.x *= PLAYER_SPEED;
		NormarizeMove.y = JunpPawer;
		NormarizeMove.z *= PLAYER_SPEED;
	}

	m_move += NormarizeMove;

}

//====================================================================
//移動方向処理
//====================================================================
void CPlayer::Rot(void)
{
	//キーボードの取得
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputJoypad* pInputJoypad = CManager::GetInstance()->GetInputJoyPad();
	D3DXVECTOR3 CameraRot = CManager::GetInstance()->GetCamera()->GetRot();

	//移動方向に向きを合わせる処理
	float fRotMove, fRotDest;

	fRotMove = m_rot.y;
	fRotDest = CManager::GetInstance()->GetCamera()->GetRot().y;

	if (pInputKeyboard->GetPress(DIK_I) == true)
	{
		m_HeightCameraRot += 0.01f;
		if (m_HeightCameraRot > 0.75f)
		{
			m_HeightCameraRot = 0.75f;
		}
	}
	if (pInputKeyboard->GetPress(DIK_L) == true)
	{
		m_rot.y += 0.01f;
	}
	if (pInputKeyboard->GetPress(DIK_J) == true)
	{
		m_rot.y -= 0.01f;
	}
	if (pInputKeyboard->GetPress(DIK_K) == true)
	{
		m_HeightCameraRot -= 0.01f;
		if (m_HeightCameraRot < -0.75f)
		{
			m_HeightCameraRot = -0.75f;
		}
	}

	useful::NormalizeAngle(&m_rot);
}

//====================================================================
//追尾処理
//====================================================================
void CPlayer::Tracking(void)
{
	// ターゲットの有無を判定
	if (m_pTgtEnemy != nullptr)
	{
		// ターゲットの状態を確認
		if (m_pTgtEnemy->GetState() == CEnemy::STATE_DEATH)
		{
			// 死亡状態
			m_pTgtEnemy = nullptr;
		}
		else
		{
			// 目的向きを算出（敵の位置）
			m_rotDest.y = atan2f(m_pTgtEnemy->GetPos().z - m_pos.z, m_pos.x - m_pTgtEnemy->GetPos().x) + D3DX_PI * 0.5f;

			// 敵との距離の判定（遠い）
			if (useful::CollisionCircle(m_pos, m_pTgtEnemy->GetPos(), 200.0f) == false)
			{
				// 進め
				m_move.x = sinf(D3DX_PI + m_rot.y) * 10.0f;
				m_move.z = cosf(D3DX_PI + m_rot.y) * 10.0f;
			}
			// 敵との距離の判定（近い）
			else if (useful::CollisionCircle(m_pos, m_pTgtEnemy->GetPos(), 150.0f) == true)
			{
				// 戻れ
				m_move.x = -sinf(D3DX_PI + m_rot.y) * 10.0f;
				m_move.z = -cosf(D3DX_PI + m_rot.y) * 10.0f;
			}
		}
	}
}

//====================================================================
//モーションと状態の管理
//====================================================================
void CPlayer::ActionState(void)
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
void CPlayer::StateManager(void)
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
		if (m_nStateCount == 0)
		{
			m_State = STATE_NORMAL;
		}
		break;
	}

	if (m_nStateCount > 0)
	{
		m_nStateCount--;
	}
}

//====================================================================
//状態管理
//====================================================================
void CPlayer::CollisionEnemy(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		//オブジェクトを取得
		CObject* pObj = CObject::GetTop(nCntPriority);

		while (pObj != NULL)
		{
			CObject* pObjNext = pObj->GetNext();

			CObject::TYPE type = pObj->GetType();			//種類を取得

			if (type == TYPE_ENEMY3D)
			{//種類がブロックの時
				CEnemy* pEnemy = (CEnemy*)pObj;

				if (useful::CollisionCircle(m_pos, pEnemy->GetPos(), 200.0f) == true)
				{
					if (pEnemy->GetState() != CEnemy::STATE_DEATH)
					{
						pEnemy->SetState(CEnemy::STATE_BATTLE);
					}
				}

				// 敵ターゲットのテスト用（仮）
				if (useful::CollisionCircle(m_pos, pEnemy->GetPos(), 5000.0f) == true)
				{
					if (m_pTgtEnemy == nullptr && pEnemy->GetState() != CEnemy::STATE_DEATH)
					{
						m_pTgtEnemy = pEnemy;
					}
				}
			}

			pObj = pObjNext;
		}
	}
}

//====================================================================
//手裏剣
//====================================================================
void CPlayer::Shuriken(void)
{
	CInputMouse* pMouse = CManager::GetInstance()->GetInputMouse();

	if (pMouse->GetTrigger(pMouse->PUSH_RIGHT))
	{
		CShuriken::Create({ m_pos.x, m_pos.y + 50.0f, m_pos.z }, 1.5f, 200.0f);
	}
}

//====================================================================
//状態管理
//====================================================================
void CPlayer::LifeGaugeCreate(void)
{
	if (m_pLifeGauge == nullptr)
	{
		m_pLifeGauge = CObjGauge2D::Create();
		m_pLifeGauge->SetPos(D3DXVECTOR3(20.0f, 20.0f, 0.0f));
		m_pLifeGauge->SetWidth(360.0f);
		m_pLifeGauge->SetHeight(10.0f);
		m_pLifeGauge->SetColor(D3DXCOLOR(0.2f, 1.0f, 0.2f, 1.0f));
		m_pLifeGauge->SetGaugeWidth(m_fLifeMax, m_fLifeMax);
	}
}

//====================================================================
//カメラ位置更新処理
//====================================================================
void CPlayer::CameraPosUpdate(void)
{
	//カメラ位置の更新
	m_CameraPos.x = m_pos.x;
	m_CameraPos.z = m_pos.z;

	if (m_bJump == false)
	{
		m_CameraPos.y = m_pos.y;
	}
}

//====================================================================
//位置更新処理
//====================================================================
void CPlayer::PosUpdate(void)
{
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
}

//====================================================================
//向き更新処理
//====================================================================
void CPlayer::RotUpdate(void)
{
	// 角度の差分を算出
	D3DXVECTOR3 rotDiff = m_rotDest - m_rot;	

	// 正規化
	useful::NormalizeAngle(&rotDiff);			

	// 向きの更新処理
	m_rot += (rotDiff * 0.05f);
}

//====================================================================
//ダメージ処理
//====================================================================
void CPlayer::HitDamage(float Damage)
{
	if (m_State == STATE_NORMAL)
	{
		m_fLife -= Damage;
		if (m_fLife < 0.0f)
		{
			m_fLife = 0.0f;

		}
		else
		{
			m_pLifeGauge->SetGaugeWidth(m_fLifeMax, m_fLife);
			m_State = STATE_DAMAGE;
			m_nStateCount = 5;
		}
	}
}

//====================================================================
// デバッグボタン
//====================================================================
void CPlayer::DebugKey(void)
{
#ifdef _DEBUG

	CInputMouse* pMouse = CManager::GetInstance()->GetInputMouse();

	if (pMouse->GetTrigger(pMouse->PUSH_LEFT))
	{
		HitDamage(10.0f);
	}

#endif // !_DEBUG
}


//====================================================================
//ロード処理
//====================================================================
void CPlayer::LoadLevelData(const char* pFilename)
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
//ライフゲージ生成
// 回避
//====================================================================
void CPlayer::Evasion()
{
	D3DXVECTOR3 CameraRot = CManager::GetInstance()->GetCamera()->GetRot();
	CInputKeyboard* pKeyboard = CManager::GetInstance()->GetInputKeyboard();
	D3DXVECTOR3 move = INITVECTOR3;

	//左右判定
	if (m_nEvasionCount <= 0)
	{
		if (pKeyboard->GetTrigger(DIK_8))
		{
			m_nEvasionLR = -1;
			m_nEvasionCount = EVADION_FRAME;
		}
		else if (pKeyboard->GetTrigger(DIK_9))
		{
			m_nEvasionLR = 1;
			m_nEvasionCount = EVADION_FRAME;
		}
		else
		{
			m_nEvasionLR = 0;
		}
	}

	if (m_nEvasionCount > 0)
	{
		float fPi = D3DX_PI * static_cast<float>(m_nEvasionLR) * 0.25f;
		move.x = sinf(CameraRot.y + fPi) * EVADION_MOVE;
		move.z = cosf(CameraRot.y + fPi) * EVADION_MOVE;
		m_nEvasionCount--;
	}

	//急な移動量
	m_pos += move;
}
