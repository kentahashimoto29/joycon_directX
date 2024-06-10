////============================================
////
////	アクションの処理 [action.cpp]
////	Author:sakamoto kai
////
////============================================
//
//#include "action.h"
//#include "object.h"
//#include "manager.h"
//#include "renderer.h"
//#include "model.h"
//#include "motion.h"
//#include "game.h"
//#include "object3D.h"
//#include "debugproc.h"
//#include "2DEffect.h"
//#include "camera.h"
//#include "input.h"
//#include "enemy.h"
//#include "useful.h"
//#include "Aim.h"
//#include "Shuriken.h"
//#include "objGauge2D.h"
//
//#define COLLISION_SIZE (D3DXVECTOR3(20.0f,40.0f,20.0f))		//横の当たり判定
//#define PLAYER_SPEED (10.0f)		//プレイヤーの移動速度
//
//namespace
//{
//	const float MAX_LIFE = 100.0f;			//体力最大値
//
//}
//
////====================================================================
////コンストラクタ
////====================================================================
//CAction::CAction()
//{
//	m_move = INITVECTOR3;
//	m_Objmove = INITVECTOR3;
//	m_rot = D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f);
//	m_bJump = false;
//	m_nActionCount = 0;
//	m_Action = ACTION_WAIT;
//	m_AtkAction = ACTION_WAIT;
//	m_nAttackHit = false;
//	m_State = STATE_NORMAL;
//	m_nStateCount = 0;
//	m_AtkPos = INITVECTOR3;
//	m_CollisionRot = 0.0f;
//	m_HeightCameraRot = 0.0f;
//
//	m_pTgtEnemy = nullptr;
//}
//
////====================================================================
////デストラクタ
////====================================================================
//CAction::~CAction()
//{
//
//}
//
////====================================================================
////生成処理
////====================================================================
//CAction* CAction::Create()
//{
//	CAction* pPlayer = NULL;
//
//	if (pPlayer == NULL)
//	{
//		//プレイヤーの生成
//		pPlayer = new CAction();
//	}
//
//	//オブジェクトの初期化処理
//	if (FAILED(pPlayer->Init()))
//	{//初期化処理が失敗した場合
//		return NULL;
//	}
//
//	return pPlayer;
//}
//
////====================================================================
////初期化処理
////====================================================================
//HRESULT CAction::Init(void)
//{
//	return S_OK;
//}
//
////====================================================================
////自分が保持するオブジェクトの生成
////====================================================================
//void CAction::MyObjCreate(void)
//{
//
//}
//
////====================================================================
////終了処理
////====================================================================
//void CAction::Uninit(void)
//{
//
//}
//
////====================================================================
////更新処理
////====================================================================
//void CAction::Update(void)
//{
//	switch (CScene::GetMode())
//	{
//	case CScene::MODE_TITLE:
//		TitleUpdate();
//		break;
//
//	case CScene::MODE_GAME:
//		GameUpdate();
//		break;
//
//	case CScene::MODE_RESULT:
//		break;
//	}
//}
//
////====================================================================
////タイトルでの更新処理
////====================================================================
//void CAction::TitleUpdate(void)
//{
//
//}
//
////====================================================================
////ゲームでの更新処理
////====================================================================
//void CAction::GameUpdate(void)
//{
//	//デバッグ表示の取得
//	CDebugProc* pDebugProc = CManager::GetInstance()->GetDebugProc();
//
//	//Move();
//
//	Rot();
//
//	Shuriken();
//
//	if (m_pTgtEnemy != nullptr)
//	{
//		if (m_pTgtEnemy->GetState() == CEnemy::STATE_DEATH)
//		{
//			m_pTgtEnemy = nullptr;
//		}
//		else
//		{
//			m_rotDest.y = atan2f(m_pTgtEnemy->GetPos().z - m_pos.z, m_pos.x - m_pTgtEnemy->GetPos().x) + D3DX_PI * 0.5f;
//
//			if (useful::CollisionCircle(m_pos, m_pTgtEnemy->GetPos(), 200.0f) == false)
//			{
//				m_move.x = sinf(D3DX_PI + m_rot.y) * 10.0f;
//				m_move.z = cosf(D3DX_PI + m_rot.y) * 10.0f;
//			}
//			else if (useful::CollisionCircle(m_pos, m_pTgtEnemy->GetPos(), 150.0f) == true)
//			{
//				m_move.x = -sinf(D3DX_PI + m_rot.y) * 10.0f;
//				m_move.z = -cosf(D3DX_PI + m_rot.y) * 10.0f;
//			}
//		}
//	}
//
//	//カメラ位置の更新
//	m_CameraPos.x = m_pos.x;
//	m_CameraPos.z = m_pos.z;
//	if (m_bJump == false)
//	{
//		m_CameraPos.y = m_pos.y;
//	}
//
//	//減衰係数
//	m_move.x = m_move.x * 0.5f;
//	if (m_move.x <= 0.0001f && m_move.x >= -0.0001f)
//	{
//		m_move.x = 0.0f;
//	}
//
//	m_move.z = m_move.z * 0.5f;
//	if (m_move.z <= 0.0001f && m_move.z >= -0.0001f)
//	{
//		m_move.z = 0.0f;
//	}
//
//	if (m_bJump == true)
//	{
//		m_Objmove.x = m_Objmove.x * 0.25f;
//		if (m_Objmove.x <= 0.0001f && m_Objmove.x >= -0.0001f)
//		{
//			m_Objmove.x = 0.0f;
//		}
//
//		m_Objmove.z = m_Objmove.z * 0.25f;
//		if (m_Objmove.z <= 0.0001f && m_Objmove.z >= -0.0001f)
//		{
//			m_Objmove.z = 0.0f;
//		}
//	}
//
//	//Y軸の位置更新
//	m_pos.y += m_move.y * CManager::GetInstance()->GetGameSpeed();
//	m_pos.y += m_Objmove.y * CManager::GetInstance()->GetGameSpeed();
//
//	//X軸の位置更新
//	m_pos.x += m_move.x * CManager::GetInstance()->GetGameSpeed();
//	m_pos.x += m_Objmove.x * CManager::GetInstance()->GetGameSpeed();
//
//	//Z軸の位置更新
//	m_pos.z += m_move.z * CManager::GetInstance()->GetGameSpeed();
//	m_pos.z += m_Objmove.z * CManager::GetInstance()->GetGameSpeed();
//
//	D3DXVECTOR3 rotDiff = m_rotDest - m_rot;	// 角度の差分を算出
//	useful::NormalizeAngle(&rotDiff);			// 正規化
//
//	// 向きの更新処理
//	m_rot += (rotDiff * 0.05f);
//
//	//状態の管理
//	StateManager();
//
//
//	CGame::GetAim()->GetPos();
//
//	DebugKey();
//
//	CManager::GetInstance()->GetDebugProc()->Print("[自分] %f : %f : %f\n", m_pos.x, m_pos.y, m_pos.z);
//}
//
////====================================================================
////移動処理
////====================================================================
//void CAction::Move(void)
//{
//	//キーボードの取得
//	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
//	CInputJoypad* pInputJoypad = CManager::GetInstance()->GetInputJoyPad();
//	D3DXVECTOR3 CameraRot = CManager::GetInstance()->GetCamera()->GetRot();
//
//	D3DXVECTOR3 NormarizeMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//
//	//キーボードの移動処理
//	if (pInputKeyboard->GetPress(DIK_W) == true)
//	{
//		NormarizeMove.z += 1.0f * cosf(CameraRot.y) * PLAYER_SPEED;
//		NormarizeMove.x += 1.0f * sinf(CameraRot.y) * PLAYER_SPEED;
//
//	}
//	if (pInputKeyboard->GetPress(DIK_S) == true)
//	{
//		NormarizeMove.z += -1.0f * cosf(CameraRot.y) * PLAYER_SPEED;
//		NormarizeMove.x += -1.0f * sinf(CameraRot.y) * PLAYER_SPEED;
//	}
//	if (pInputKeyboard->GetPress(DIK_A) == true)
//	{
//		NormarizeMove.x += -1.0f * cosf(CameraRot.y) * PLAYER_SPEED;
//		NormarizeMove.z -= -1.0f * sinf(CameraRot.y) * PLAYER_SPEED;
//
//	}
//	if (pInputKeyboard->GetPress(DIK_D) == true)
//	{
//		NormarizeMove.x += 1.0f * cosf(CameraRot.y) * PLAYER_SPEED;
//		NormarizeMove.z -= 1.0f * sinf(CameraRot.y) * PLAYER_SPEED;
//	}
//
//	if (pInputKeyboard->GetPress(DIK_W) == false && pInputKeyboard->GetPress(DIK_A) == false && pInputKeyboard->GetPress(DIK_S) == false && pInputKeyboard->GetPress(DIK_D) == false)
//	{
//		//左スティックによる前後移動	
//		m_move.z += pInputJoypad->Get_Stick_Left(0).y * cosf(CameraRot.y) * PLAYER_SPEED;
//		m_move.x += pInputJoypad->Get_Stick_Left(0).y * sinf(CameraRot.y) * PLAYER_SPEED;
//
//		//左スティックによる左右移動
//		m_move.x += pInputJoypad->Get_Stick_Left(0).x * cosf(CameraRot.y) * PLAYER_SPEED;
//		m_move.z -= pInputJoypad->Get_Stick_Left(0).x * sinf(CameraRot.y) * PLAYER_SPEED;
//	}
//
//	if (pInputKeyboard->GetPress(DIK_W) == true || pInputKeyboard->GetPress(DIK_A) == true || pInputKeyboard->GetPress(DIK_S) == true || pInputKeyboard->GetPress(DIK_D) == true)
//	{
//		float JunpPawer = NormarizeMove.y;
//		NormarizeMove.y = 0.0f;
//
//		D3DXVec3Normalize(&NormarizeMove, &NormarizeMove);
//
//		NormarizeMove.x *= PLAYER_SPEED;
//		NormarizeMove.y = JunpPawer;
//		NormarizeMove.z *= PLAYER_SPEED;
//	}
//
//	m_move += NormarizeMove;
//
//}
//
////====================================================================
////移動方向処理
////====================================================================
//void CAction::Rot(void)
//{
//	//キーボードの取得
//	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
//	CInputJoypad* pInputJoypad = CManager::GetInstance()->GetInputJoyPad();
//	D3DXVECTOR3 CameraRot = CManager::GetInstance()->GetCamera()->GetRot();
//
//	//移動方向に向きを合わせる処理
//	float fRotMove, fRotDest;
//
//	fRotMove = m_rot.y;
//	fRotDest = CManager::GetInstance()->GetCamera()->GetRot().y;
//
//	if (pInputKeyboard->GetPress(DIK_I) == true)
//	{
//		m_HeightCameraRot += 0.01f;
//		if (m_HeightCameraRot > 0.75f)
//		{
//			m_HeightCameraRot = 0.75f;
//		}
//	}
//	if (pInputKeyboard->GetPress(DIK_L) == true)
//	{
//		m_rot.y += 0.01f;
//	}
//	if (pInputKeyboard->GetPress(DIK_J) == true)
//	{
//		m_rot.y -= 0.01f;
//	}
//	if (pInputKeyboard->GetPress(DIK_K) == true)
//	{
//		m_HeightCameraRot -= 0.01f;
//		if (m_HeightCameraRot < -0.75f)
//		{
//			m_HeightCameraRot = -0.75f;
//		}
//	}
//}
////====================================================================
////状態管理
////====================================================================
//void CAction::StateManager(void)
//{
//	switch (m_State)
//	{
//	case STATE_NORMAL:
//		break;
//
//	case STATE_DEATH:
//		break;
//
//	case STATE_WAIT:
//		break;
//
//	case STATE_DAMAGE:
//		if (m_nStateCount == 0)
//		{
//			m_State = STATE_NORMAL;
//		}
//		break;
//	}
//
//	if (m_nStateCount > 0)
//	{
//		m_nStateCount--;
//	}
//}
//
////====================================================================
////手裏剣
////====================================================================
//void CAction::Shuriken(void)
//{
//	CInputMouse* pMouse = CManager::GetInstance()->GetInputMouse();
//
//	if (pMouse->GetTrigger(pMouse->PUSH_RIGHT))
//	{
//		CShuriken::Create({ m_pos.x, m_pos.y + 50.0f, m_pos.z }, 1.5f, 200.0f);
//	}
//}
//
////====================================================================
//// デバッグボタン
////====================================================================
//void CAction::DebugKey()
//{
//#ifdef _DEBUG
//
//
//#endif // !_DEBUG
//}
