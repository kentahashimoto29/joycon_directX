//============================================
//
//	カメラの処理 [camera.cpp]
//	Author:sakamoto kai
//
//============================================
#include "camera.h"
#include "manager.h"
#include "game.h"
#include "renderer.h"
#include "input.h"
#include "debugproc.h"
#include "player.h"
#include "boss.h"
#include "sound.h"

//マクロ定義
#define CAMERA_DISTANCE (550.0f)							//視点と注視点の距離
#define CAMERA_DISTANCE_EVENT (300.0f)						//イベント時の視点と注視点の距離
#define MODEL_DISTANCE (10.0f)								//モデルと注視点の距離
#define CAMERA_SPEED (9.0f)									//カメラの移動スピード
#define CAMERA_VR_SPEED (0.015f)							//カメラの視点スピード
#define CAMERA_PAD_VR_SPEED (0.015f)						//カメラのパッドの視点スピード
#define CAMERA_HOMING (0.2f)								//カメラの追従スピード
#define POS_HOMING (0.9f)									//位置への追従スピード
#define DOWNVIEW_POSV (D3DXVECTOR3(0.0f, 1500.0f, -200.0f))	//見下ろしの視点
#define DOWNVIEW_POSR (D3DXVECTOR3(0.0f, 0.0f, 0.0f))		//見下ろしの注視点
#define SIDEVIEW_POSV (D3DXVECTOR3(0.0f, 200.0f, -1000.0f))	//2Dの視点
#define SIDEVIEW_POSR (D3DXVECTOR3(0.0f, 200.0f, 0.0f))		//2Dの注視点

//====================================================================
//コンストラクタ
//====================================================================
CCamera::CCamera()
{
	m_bBib = false;
	m_fBibPowor = 0.0f;
	m_move = D3DXVECTOR3(10.0f, 0.0f, 0.0f);
	m_posV = INITVECTOR3;
	m_posR = INITVECTOR3;
	m_rot = INITVECTOR3;
	m_PlayerPos = INITVECTOR3;
	m_DelCameraPos = INITVECTOR3;
	ResetCamera();
	CameraMode = CAMERAMODE_FPS;
	
#ifdef _DEBUG
	CameraMode = CAMERAMODE_FOLLOW;
#endif

	m_FollowTime = 0;
	m_bFollowY = false;
	m_bAttention = false;
	m_fRotMove = INITVECTOR3;
}

//====================================================================
//デストラクタ
//====================================================================
CCamera::~CCamera()
{

}

//====================================================================
//カメラの初期化処理
//====================================================================
HRESULT CCamera::Init(void)
{
	return S_OK;
}

//====================================================================
//カメラの終了処理
//====================================================================
void CCamera::Uninit(void)
{

}

//====================================================================
//カメラの更新処理
//====================================================================
void CCamera::Update(void)
{
#ifdef _DEBUG
	//デバイスの取得
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	if (pInputKeyboard->GetTrigger(DIK_F3) == true)
	{
		if (CameraMode == CAMERAMODE_CONTROL)
		{
			CameraMode = CAMERAMODE_FPS;
		}
		else if (CameraMode == CAMERAMODE_FPS)
		{
			CameraMode = CAMERAMODE_CONTROL;
		}
		//else if (CameraMode == CAMERAMODE_FOLLOW)
		//{
		//	CameraMode = CAMERAMODE_DOWNVIEW;
		//}
		//else if (CameraMode == CAMERAMODE_DOWNVIEW)
		//{
		//	CameraMode = CAMERAMODE_SIDEVIEW;
		//}
		//else if (CameraMode == CAMERAMODE_SIDEVIEW)
		//{
		//	CameraMode = CAMERAMODE_CONTROL;
		//}
	}

#endif // _DEBUG

	switch (CScene::GetMode())
	{
	case CScene::MODE_TITLE:

		ControlCamera();

		//視点の情報を出力する
		m_posV.x = m_posR.x + sinf(m_rot.y) * -cosf(m_rot.x) * m_CameraDistance;
		m_posV.y = m_posR.y + sinf(-m_rot.x) * m_CameraDistance;
		m_posV.z = m_posR.z + cosf(m_rot.y) * -cosf(m_rot.x) * m_CameraDistance;

		break;

	case CScene::MODE_GAME:

		switch (CameraMode)
		{
		case CAMERAMODE_CONTROL:	//操作カメラの更新
			ControlCamera();
			break;
		case CAMERAMODE_FOLLOW:		//追従カメラの更新
			FollowCamera();
			break;
		case CAMERAMODE_DOWNVIEW:	//見下ろしカメラの更新
			DownviewCamera();
			break;
		case CAMERAMODE_SIDEVIEW:	//2Dカメラの更新
			SideviewCamera();
			break;
		case CAMERAMODE_EVENTBOSS:	//イベント中のボス注目カメラの更新
			EventBossCamera();
			break;
		case CAMERAMODE_FPS:		//一人称
			FPSCamera();
			break;
		}

		break;
	case CScene::MODE_RESULT:

		//視点の情報を出力する
		m_posV.x = m_posR.x + sinf(m_rot.y) * -cosf(m_rot.x) * m_CameraDistance;
		m_posV.y = m_posR.y + sinf(-m_rot.x) * m_CameraDistance;
		m_posV.z = m_posR.z + cosf(m_rot.y) * -cosf(m_rot.x) * m_CameraDistance;

		break;
	}
	//デバッグ表示の取得
	CDebugProc* pDebugProc = CManager::GetInstance()->GetDebugProc();
	pDebugProc->Print("%f:%f:%f\n", m_posV.x, m_posV.y, m_posV.z);
	pDebugProc->Print("%f:%f:%f\n", m_posR.x, m_posR.y, m_posR.z);
}

//====================================================================
//操作カメラの更新処理
//====================================================================
void CCamera::ControlCamera(void)
{
	//デバイスの取得
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputJoypad* pInputJoypad = CManager::GetInstance()->GetInputJoyPad();
	CInputMouse* pInputMouse = CManager::GetInstance()->GetInputMouse();

	if (m_rot.x <= D3DX_PI * 0.5f && m_rot.x >= -(D3DX_PI * 0.5f))
	{//入力
		m_rotOld.x = m_rot.x;

		//キーボード
		if (pInputKeyboard->GetPress(DIK_I) == true)
		{
			m_rot.x += CAMERA_VR_SPEED;
		}
		if (pInputKeyboard->GetPress(DIK_K) == true)
		{
			m_rot.x -= CAMERA_VR_SPEED;
		}
	}

	if (fabsf(m_rot.x) > fabsf(D3DX_PI * 0.5f))
	{//上限に達した時１フレーム前のrotにもどる
		m_rot.x = m_rotOld.x;
	}

	//キーボード
	if (pInputKeyboard->GetPress(DIK_J) == true)
	{
		m_rot.y -= CAMERA_VR_SPEED;

	}
	if (pInputKeyboard->GetPress(DIK_L) == true)
	{
		m_rot.y += CAMERA_VR_SPEED;
	}

	//一周した時の向きの補正
	if (m_rot.y > D3DX_PI * 1.0f)
	{
		m_rot.y -= D3DX_PI * 2.0f;
	}
	else if (m_rot.y < -D3DX_PI * 1.0f)
	{
		m_rot.y += D3DX_PI * 2.0f;
	}


	//キーボード
	//if (pInputKeyboard->GetPress(DIK_LSHIFT) == true)
	//{
	//	m_posV.y += CAMERA_SPEED;
	//	m_posR.y += CAMERA_SPEED;

	//}
	//if (pInputKeyboard->GetPress(DIK_LCONTROL) == true)
	//{
	//	m_posV.y -= CAMERA_SPEED;
	//	m_posR.y -= CAMERA_SPEED;
	//}
	if (pInputKeyboard->GetPress(DIK_RSHIFT) == true)
	{
		m_posV.y += CAMERA_SPEED;
		m_posR.y += CAMERA_SPEED;

	}
	if (pInputKeyboard->GetPress(DIK_RCONTROL) == true)
	{
		m_posV.y -= CAMERA_SPEED;
		m_posR.y -= CAMERA_SPEED;
	}

	//if (pInputKeyboard->GetTrigger(DIK_4) == true)
	//{
	//	m_CameraDistance -= 500.0f;
	//}

	//if (pInputKeyboard->GetTrigger(DIK_5) == true)
	//{
	//	m_CameraDistance += 500.0f;
	//}

	//キーボード
	if (pInputKeyboard->GetPress(DIK_T) == true)
	{
		m_posR.z += CAMERA_SPEED * cosf(m_rot.y);
		m_posR.x += CAMERA_SPEED * sinf(m_rot.y);

		m_posV.z += CAMERA_SPEED * cosf(m_rot.y);
		m_posV.x += CAMERA_SPEED * sinf(m_rot.y);
	}
	if (pInputKeyboard->GetPress(DIK_G) == true)
	{
		m_posR.z += -CAMERA_SPEED * cosf(m_rot.y);
		m_posR.x += -CAMERA_SPEED * sinf(m_rot.y);

		m_posV.z += -CAMERA_SPEED * cosf(m_rot.y);
		m_posV.x += -CAMERA_SPEED * sinf(m_rot.y);
	}
	if (pInputKeyboard->GetPress(DIK_F) == true)
	{
		m_posR.x += -CAMERA_SPEED * cosf(m_rot.y);
		m_posR.z -= -CAMERA_SPEED * sinf(m_rot.y);

		m_posV.x += -CAMERA_SPEED * cosf(m_rot.y);
		m_posV.z -= -CAMERA_SPEED * sinf(m_rot.y);
	}
	if (pInputKeyboard->GetPress(DIK_H) == true)
	{
		m_posR.x += CAMERA_SPEED * cosf(m_rot.y);
		m_posR.z -= CAMERA_SPEED * sinf(m_rot.y);

		m_posV.x += CAMERA_SPEED * cosf(m_rot.y);
		m_posV.z -= CAMERA_SPEED * sinf(m_rot.y);
	}

	if (CManager::GetInstance()->GetScene()->GetMode() == CScene::MODE_GAME)
	{
		//視点の情報を出力する
		m_posV.x = m_posR.x + sinf(m_rot.y) * -cosf(m_rot.x) * m_CameraDistance;
		m_posV.y = m_posR.y + sinf(-m_rot.x) * m_CameraDistance;
		m_posV.z = m_posR.z + cosf(m_rot.y) * -cosf(m_rot.x) * m_CameraDistance;
	}
}

//====================================================================
//追従カメラの更新処理
//====================================================================
void CCamera::FollowCamera(void)
{
	//デバイスの取得
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputJoypad* pInputJoypad = CManager::GetInstance()->GetInputJoyPad();
	CInputMouse* pInputMouse = CManager::GetInstance()->GetInputMouse();

	//プレイヤーの取得
	CPlayer* pPlayer = CGame::GetPlayer();
	//ボスの取得
	CBoss* pBoss = CGame::GetBoss();

	m_fRotMove = m_fRotMove * 0.5f;

	if (pPlayer->GetJump() == false)
	{
		if (m_rot.x <= D3DX_PI * 0.5f && m_rot.x >= -(D3DX_PI * 0.5f))
		{//入力
			m_rotOld.x = m_rot.x;

			//キーボード
			if (pInputKeyboard->GetPress(DIK_I) == true)
			{
				m_fRotMove.x += CAMERA_VR_SPEED;
			}
			if (pInputKeyboard->GetPress(DIK_K) == true)
			{
				m_fRotMove.x -= CAMERA_VR_SPEED;
			}

			//右スティックの上下視点移動入力
			m_fRotMove.x += pInputJoypad->Get_Stick_Right(0).y * CAMERA_PAD_VR_SPEED;

			m_fRotMove.x -= pInputMouse->GetMouseMove().y * CAMERA_VR_SPEED;

			m_rot.x += m_fRotMove.x;
		}
	}

	if (m_rot.x > D3DX_PI * 0.05f || m_rot.x < -(D3DX_PI * 0.45f))
	{//上限に達した時１フレーム前のrotにもどる
		m_rot.x = m_rotOld.x;
	}

	//キーボード
	if (pInputKeyboard->GetPress(DIK_J) == true)
	{
		m_fRotMove.y -= CAMERA_VR_SPEED;

	}
	if (pInputKeyboard->GetPress(DIK_L) == true)
	{
		m_fRotMove.y += CAMERA_VR_SPEED;
	}

	//右スティックの左右視点移動入力
	m_fRotMove.y += pInputJoypad->Get_Stick_Right(0).x * CAMERA_PAD_VR_SPEED;

	m_fRotMove.y += pInputMouse->GetMouseMove().x * CAMERA_VR_SPEED;

	m_rot.y += m_fRotMove.y;

	//一周した時の向きの補正
	if (m_rot.y > D3DX_PI * 1.0f)
	{
		m_rot.y -= D3DX_PI * 2.0f;
	}
	else if (m_rot.y < -D3DX_PI * 1.0f)
	{
		m_rot.y += D3DX_PI * 2.0f;
	}

	m_posRDest.x = m_posV.x + sinf(m_rot.y) * cosf(m_rot.x) * CAMERA_DISTANCE;
	m_posRDest.z = m_posV.z + cosf(m_rot.y) * cosf(m_rot.x) * CAMERA_DISTANCE;
	m_posRDest.y = m_posV.y + sinf(m_rot.x) * CAMERA_DISTANCE;

	m_posVDest.x = pPlayer->GetCameraPos().x + sinf(-pPlayer->GetRot().y) * MODEL_DISTANCE + sinf(m_rot.y) * -cosf(m_rot.x) * CAMERA_DISTANCE;
	m_posVDest.z = pPlayer->GetCameraPos().z + cosf(-pPlayer->GetRot().y) * MODEL_DISTANCE + cosf(m_rot.y) * -cosf(m_rot.x) * CAMERA_DISTANCE;
	m_posVDest.y = pPlayer->GetCameraPos().y + 175.0f + sinf(-m_rot.x) * CAMERA_DISTANCE;

	if (m_bBib == true)
	{
		m_fBibPowor += 0.4f;

		m_posR.x += (m_posRDest.x - m_posR.x) * CAMERA_HOMING;
		m_posR.z += (m_posRDest.z - m_posR.z) * CAMERA_HOMING;
		m_posV.x += (m_posVDest.x - m_posV.x) * CAMERA_HOMING * 5.0f + (int)(sin(D3DX_PI * m_fBibPowor) * 5.0f);
		m_posV.z += (m_posVDest.z - m_posV.z) * CAMERA_HOMING * 5.0f + (int)(sin(D3DX_PI * m_fBibPowor) * 5.0f);

		if (pPlayer->GetJump() == false)
		{
			m_posR.y += (m_posRDest.y - m_posR.y) * CAMERA_HOMING;
			m_posV.y += (m_posVDest.y - m_posV.y) * CAMERA_HOMING;
		}
	}
	else
	{
		m_posR.x += (m_posRDest.x - m_posR.x) * CAMERA_HOMING;
		m_posR.z += (m_posRDest.z - m_posR.z) * CAMERA_HOMING;
		m_posV.x += (m_posVDest.x - m_posV.x) * CAMERA_HOMING * 5.0f;
		m_posV.z += (m_posVDest.z - m_posV.z) * CAMERA_HOMING * 5.0f;

		if (pPlayer->GetJump() == false)
		{
			m_posR.y += (m_posRDest.y - m_posR.y) * CAMERA_HOMING;
			m_posV.y += (m_posVDest.y - m_posV.y) * CAMERA_HOMING;
		}
	}


	//注目の切り替え
	if (pInputKeyboard->GetTrigger(DIK_LSHIFT) == true ||
		pInputJoypad->GetTrigger(CInputJoypad::BUTTON_L, 0) == true)
	{
		//ゲームのSEを再生する
		CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_ATTENTION);

		m_bAttention = m_bAttention ? false : true;
	}

	if (m_bAttention == true)
	{
		m_FollowTime = 10;
	}

	////モデルが止まった時に正面を向く処理
	//float fRotMove, fRotDest, fRotDiff;

	//if (m_FollowTime > 0)
	//{
	//	m_FollowTime--;

	//	fRotMove = atan2f(sinf(m_rot.y), cosf(m_rot.y));	//現在の向き
	//	fRotDest = atan2f(pBoss->GetPos().x - pPlayer->GetPos().x, pBoss->GetPos().z - pPlayer->GetPos().z);	//目的の向き
	//	//fRotDest = atan2f(sinf(pPlayer->GetRot().y + D3DX_PI), cosf(pPlayer->GetRot().y + D3DX_PI));	//目的の向き
	//	fRotDiff = fRotDest - fRotMove;									 //差分

	//	if (fRotDiff > D3DX_PI * 1.0f)
	//	{
	//		fRotDiff -= D3DX_PI * 2.0f;
	//	}
	//	else if (fRotDiff < -D3DX_PI * 1.0f)
	//	{
	//		fRotDiff += D3DX_PI * 2.0f;
	//	}

	//	fRotMove += fRotDiff * 0.075f;

	//	if (fRotMove > D3DX_PI * 1.0f)
	//	{
	//		fRotMove -= D3DX_PI * 2.0f;
	//	}
	//	else if (fRotMove < -D3DX_PI * 1.0f)
	//	{
	//		fRotMove += D3DX_PI * 2.0f;
	//	}
	//	m_rot.y = fRotMove;
	//}
}

//====================================================================
//見下ろしカメラの更新処理
//====================================================================
void CCamera::DownviewCamera(void)
{
	m_rot.y = 0.0f;
	m_posV.x += (DOWNVIEW_POSV.x - m_posV.x) * CAMERA_HOMING;
	m_posV.y += (DOWNVIEW_POSV.y - m_posV.y) * CAMERA_HOMING;
	m_posV.z += (DOWNVIEW_POSV.z - m_posV.z) * CAMERA_HOMING;
	m_posR.x += (DOWNVIEW_POSR.x - m_posR.x) * CAMERA_HOMING;
	m_posR.y += (DOWNVIEW_POSR.y - m_posR.y) * CAMERA_HOMING;
	m_posR.z += (DOWNVIEW_POSR.z - m_posR.z) * CAMERA_HOMING;
}

//====================================================================
//2Dカメラの更新処理
//====================================================================
void CCamera::SideviewCamera(void)
{
	m_rot.y = 0.0f;
	m_posV.x += (SIDEVIEW_POSV.x - m_posV.x) * CAMERA_HOMING;
	m_posV.y += (SIDEVIEW_POSV.y - m_posV.y) * CAMERA_HOMING;
	m_posV.z += (SIDEVIEW_POSV.z - m_posV.z) * CAMERA_HOMING;
	m_posR.x += (SIDEVIEW_POSR.x - m_posR.x) * CAMERA_HOMING;
	m_posR.y += (SIDEVIEW_POSR.y - m_posR.y) * CAMERA_HOMING;
	m_posR.z += (SIDEVIEW_POSR.z - m_posR.z) * CAMERA_HOMING;
}

//====================================================================
//イベント中のボス注目カメラの更新処理
//====================================================================
void CCamera::EventBossCamera(void)
{
	//ボスの取得
	CBoss* pBoss = CGame::GetBoss();

	m_rot = INITVECTOR3;

	m_posRDest.x = m_posV.x + sinf(m_rot.y) * cosf(m_rot.x) * CAMERA_DISTANCE_EVENT;
	m_posRDest.z = m_posV.z + cosf(m_rot.y) * cosf(m_rot.x) * CAMERA_DISTANCE_EVENT;
	m_posRDest.y = m_posV.y + sinf(m_rot.x) * CAMERA_DISTANCE_EVENT;

	m_posVDest.x = CGame::GetEventPos().x - 50.0f + sinf(-pBoss->GetRot().y) * MODEL_DISTANCE + sinf(m_rot.y) * -cosf(m_rot.x) * CAMERA_DISTANCE_EVENT;
	m_posVDest.z = CGame::GetEventPos().z + cosf(-pBoss->GetRot().y) * MODEL_DISTANCE + cosf(m_rot.y) * -cosf(m_rot.x) * CAMERA_DISTANCE_EVENT;
	m_posVDest.y = CGame::GetEventPos().y + sinf(-m_rot.x) * CAMERA_DISTANCE_EVENT;

	if (m_bBib == true)
	{
		m_fBibPowor += 0.8f;

		//m_posR.y += (int)(sin(D3DX_PI * m_fBibPowor) * 10.0f);

		////視点の情報を出力する
		//m_posV.x = m_posR.x + sinf(m_rot.y) * -cosf(m_rot.x) * m_CameraDistance;
		//m_posV.y = m_posR.y + sinf(-m_rot.x) * m_CameraDistance + (int)(sin(D3DX_PI * m_fBibPowor) * 10.0f);
		//m_posV.z = m_posR.z + cosf(m_rot.y) * -cosf(m_rot.x) * m_CameraDistance;

		m_posR.x += (m_posRDest.x - m_posR.x) * CAMERA_HOMING;
		m_posR.y += (m_posRDest.y - m_posR.y) * CAMERA_HOMING * 5.0f;
		m_posR.z += (m_posRDest.z - m_posR.z) * CAMERA_HOMING;

		m_posV.x += (m_posVDest.x - m_posV.x) * CAMERA_HOMING * 5.0f + (int)(sin(D3DX_PI * m_fBibPowor) * 10.0f);
		m_posV.y += (m_posVDest.y - m_posV.y) * CAMERA_HOMING * 5.0f + (int)(sin(D3DX_PI * m_fBibPowor) * 10.0f);
		m_posV.z += (m_posVDest.z - m_posV.z) * CAMERA_HOMING * 5.0f + (int)(sin(D3DX_PI * m_fBibPowor) * 10.0f);
	}
	else
	{
		m_posR.x += (m_posRDest.x - m_posR.x) * CAMERA_HOMING * 0.3f;
		m_posR.y += (m_posRDest.y - m_posR.y) * CAMERA_HOMING * 0.3f;
		m_posR.z += (m_posRDest.z - m_posR.z) * CAMERA_HOMING * 0.3f;

		m_posV.x += (m_posVDest.x - m_posV.x) * CAMERA_HOMING * 0.3f;
		m_posV.y += (m_posVDest.y - m_posV.y) * CAMERA_HOMING * 0.3f;
		m_posV.z += (m_posVDest.z - m_posV.z) * CAMERA_HOMING * 0.3f;
	}
}

//====================================================================
//一人称カメラの更新処理
//====================================================================
void CCamera::FPSCamera(void)
{
	//プレイヤーの取得
	CPlayer* pPlayer = CGame::GetPlayer();

	m_posV.x = pPlayer->GetPos().x;
	m_posV.y = pPlayer->GetPos().y + 50.0f;
	m_posV.z = pPlayer->GetPos().z;
	m_rot.x = pPlayer->GetCameraHeightRot();
	m_rot.y = pPlayer->GetRot().y + D3DX_PI;

	//注視点の情報を出力する
	m_posR.x = m_posV.x + sinf(m_rot.y) * cosf(m_rot.x) * CAMERA_DISTANCE;
	m_posR.z = m_posV.z + cosf(m_rot.y) * cosf(m_rot.x) * CAMERA_DISTANCE;
	m_posR.y = m_posV.y + sinf(m_rot.x) * CAMERA_DISTANCE;
}

//====================================================================
//カメラの設定処理
//====================================================================
void CCamera::SetCamera(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	//プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
		D3DXToRadian(45.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		10.0f,
		12000.0f);

	//プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	//ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);

	//ビューマトリックスを作成
	D3DXMatrixLookAtLH(&m_mtxView,
		&m_posV,
		&m_posR,
		&m_vecU);

	//ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}

//====================================================================
//カメラの設定処理
//====================================================================
void CCamera::ResetCamera(void)
{
	m_posV = D3DXVECTOR3(0.0f, 100.0f, 0.0f);
	m_posR = D3DXVECTOR3(0.0f, 100.0f, 0.0f);
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_rotOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_CameraDistance = CAMERA_DISTANCE;
	m_EventPosY = 0.0f;

	if (CScene::MODE_GAME == CScene::GetMode())
	{
		m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	else
	{
		m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
}