//============================================
//
//	照準 [aim.cpp]
//	Author:筋肉★小笠原★筋肉
//
//============================================
#include "aim.h"
#include "manager.h"
#include "input.h"
#include "useful.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "useful.h"
#include "camera.h"

// マクロ定義
namespace
{
	D3DXVECTOR2 AIM_SIZE = { 5.0f, 5.0f };	// 大きさ
	float DISTANCE = 100.0f;				// エイムの距離
	float SENSITIVITY = 0.01f;				// 感度
}

//====================================================================
//= コンストラクタ
//====================================================================
CAim::CAim(int nPriority) : CObjectBillboard(nPriority)
{
	// 値の初期化
	ZeroMemory(&m_info, sizeof(m_info));
}

//====================================================================
//= デストラクタ
//====================================================================
CAim::~CAim()
{

}

//====================================================================
//= 生成処理
//====================================================================
CAim* CAim::Create()
{
	// 照準の生成
	CAim* pAim = new CAim();

	//初期化処理
	if (FAILED(pAim->Init()))
	{//初期化処理が失敗した場合
		return nullptr;
	}

	return pAim;
}

//====================================================================
//= 初期化処理
//====================================================================
HRESULT CAim::Init()
{
	// 継承クラスの初期化
	CObjectBillboard::Init();

	// 大きさ設定
	SetWidth(AIM_SIZE.x);
	SetHeight(AIM_SIZE.y);

	return S_OK;
}

//====================================================================
//= 終了処理
//====================================================================
void CAim::Uninit()
{

}

//====================================================================
//= 更新処理
//====================================================================
void CAim::Update()
{
	// 継承クラスの更新
	CObjectBillboard::Update();

	// 位置の設定（ドラック）
	CursorMove();

	// 制限
	//CursorLimit();

	// 色の設定（クリックで変更）
	CursorColor();

	// デバック表示
	CDebugProc* pDebug = CManager::GetInstance()->GetDebugProc();
	pDebug->Print("\n照準\n");
	pDebug->Print("位置:%f %f %f\n", GetPos().x, GetPos().y, GetPos().z);
	pDebug->Print("向き:%f %f %f\n", GetRot().x, GetRot().y, GetRot().z);
}

//====================================================================
//= 描画処理
//====================================================================
void CAim::Draw()
{
	// 継承クラスの描画
	CObjectBillboard::Draw();
}

//====================================================================
//= カーソル移動
//====================================================================
void CAim::CursorMove()
{
	// 情報を取得
	CInputMouse* pMouse = CManager::GetInstance()->GetInputMouse();
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	if (!pCamera)
	{
		return;
	}

	if (pMouse != nullptr)
	{
		// マウス情報を取得
		D3DXVECTOR3 move = pMouse->GetMouseMove();	// 移動量
		// エイム情報を取得
		D3DXVECTOR3 pos = GetPos();
		D3DXVECTOR3 rot = GetRot();
		// 現在の向きを保存
		m_info.rotOld = rot;

		// 向きを加算
		rot.x += (-move.y) * SENSITIVITY;
		rot.y += move.x * SENSITIVITY;

		// 移動範囲の制限
		Limit(&rot, &move);

		// 照準の位置を計算
		pos.x = pCamera->GetPosV().x + sinf(pCamera->GetRot().y + rot.y) * cosf(pCamera->GetRot().x + rot.x) * DISTANCE;
		pos.z = pCamera->GetPosV().z + cosf(pCamera->GetRot().y + rot.y) * cosf(pCamera->GetRot().x + rot.x) * DISTANCE;
		pos.y = pCamera->GetPosV().y + sinf(pCamera->GetRot().x + rot.x) * DISTANCE;

		// 位置設定
		SetPos(pos);

		// 向き設定
		SetRot(rot);
	}
}

//====================================================================
//= カーソル色
//====================================================================
void CAim::CursorColor()
{
	// マウスを取得
	CInputMouse* pMouse = CManager::GetInstance()->GetInputMouse();

	if (pMouse != nullptr)
	{
		// マウス入力の判定（左クリック || 右クリック）
		if (pMouse->GetPress(pMouse->PUSH_LEFT) || pMouse->GetPress(pMouse->PUSH_RIGHT))
		{
			// 緑色
			SetColor({ 0.0f, 1.0f, 0.0f, 1.0f });
		}
		else
		{
			// デフォルト
			SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		}
	}
}

//====================================================================
//= 制限1
//====================================================================
void CAim::CursorLimit()
{
	// 情報を取得
	CInputMouse* pMouse = CManager::GetInstance()->GetInputMouse();
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	if (!pCamera)
	{
		return;
	}

	// 変数宣言
	D3DXVECTOR3 ScreenPos = {};
	D3DXVECTOR3 AimPos = GetPos();
	D3DXVECTOR3 AimRot = GetRot();
	D3DXVECTOR3 rot = GetRot();

	// 現在の向きを保存
	m_info.rotOld = AimRot;

	// 2D座標変換
	useful::Vec3Project(&ScreenPos, AimPos);

	// デバック表示
	CDebugProc* pDebug = CManager::GetInstance()->GetDebugProc();
	pDebug->Print("2D位置:%f %f %f\n", ScreenPos.x, ScreenPos.y, ScreenPos.z);
	pDebug->Print("過去向き:%f %f %f\n", m_info.rotOld.x, m_info.rotOld.y, m_info.rotOld.z);


	if (useful::CollisionWindow(ScreenPos))
	{
		if (ScreenPos.x > SCREEN_WIDTH)
		{
			AimRot.y = m_info.rotOld.y - 0.01f;

		}
		else if (ScreenPos.x < 0.0f)
		{
			AimRot.y = m_info.rotOld.y + 0.01f;
		}
		if (ScreenPos.y > SCREEN_HEIGHT)
		{
			AimRot.x = m_info.rotOld.x + 0.01f;
		}
		else if (ScreenPos.y < 0.0f)
		{
			AimRot.x = m_info.rotOld.x - 0.01f;
		}

		// 照準の位置を計算
		AimPos.x = pCamera->GetPosV().x + sinf(AimRot.y) * cosf(AimRot.x) * DISTANCE;
		AimPos.z = pCamera->GetPosV().z + cosf(AimRot.y) * cosf(AimRot.x) * DISTANCE;
		AimPos.y = pCamera->GetPosV().y + sinf(AimRot.x) * DISTANCE;

		// 位置設定
		SetPos(AimPos);

		// 向き設定
		SetRot(AimRot);

		pDebug->Print("正規化:%f %f %f\n", ScreenPos.x, ScreenPos.y, ScreenPos.z);
	}
}

//====================================================================
//= 制限2
//====================================================================
void CAim::Limit(D3DXVECTOR3* rot, D3DXVECTOR3* move)
{
	// 変数宣言
	D3DXVECTOR3 ScreenPos = {};
	D3DXVECTOR3 AimPos = GetPos();
	D3DXVECTOR3 AimRot = GetRot();

	// 2D座標変換
	useful::Vec3Project(&ScreenPos, AimPos);

	if (useful::CollisionWindow(ScreenPos))
	{
		if (ScreenPos.x > SCREEN_WIDTH)
		{
			rot->y = AimRot.y - 0.01f;
			move->y = 0.0f;
		}
		else if (ScreenPos.x < 0.0f)
		{
			rot->y = AimRot.y + 0.01f;
			move->y = 0.0f;
		}
		if (ScreenPos.y > SCREEN_HEIGHT)
		{
			rot->x = AimRot.x + 0.01f;
			move->x = 0.0f;
		}
		else if (ScreenPos.y < 0.0f)
		{
			rot->x = AimRot.x - 0.01f;
			move->x = 0.0f;
		}
	}
}
