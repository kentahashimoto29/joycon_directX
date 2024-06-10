//============================================
//
//	手裏剣[Shuriken.h]
//	Author:Takeru Ogasawara
//
//============================================
#include "Shuriken.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "input.h"
#include "camera.h"

#include "game.h"
#include "aim.h"
#include "player.h"
#include "enemy.h"

#include "useful.h"

// マクロ定義
namespace
{
	char* MOTEL_FILE_PATH = "data\\MODEL\\player\\wepon\\syuriken1.x";	// モデルのパス

	float Decrease = 1.0f;	// 寿命の減少量
	float ROTATE_SPEED = 2.0f;
}


//====================================================================
// コンストラクタ
//====================================================================
CShuriken::CShuriken()
{
	// 値の初期化
	ZeroMemory(&m_info, sizeof(m_info));
}

//====================================================================
// デストラクタ
//====================================================================
CShuriken::~CShuriken()
{

}

//====================================================================
// 生成処理 : Create(位置、速さ、寿命)
//====================================================================
CShuriken* CShuriken::Create(D3DXVECTOR3 pos, float fSpeed, float fLife)
{
	CShuriken* pShuriken = nullptr;

	if (pShuriken == nullptr)
	{
		// インスタンス生成
		pShuriken = new CShuriken;

		// 初期化処理
		pShuriken->Init();

		// 位置設定
		pShuriken->SetPos(pos);

		// 速さ設定
		pShuriken->m_info.fSpeed = fSpeed;

		// 寿命設定
		pShuriken->m_info.fLife = fLife;
	}

	return pShuriken;
}

//====================================================================
// 初期化処理
//====================================================================
HRESULT CShuriken::Init()
{
	// 継承クラスの初期化
	CObjectX::Init(MOTEL_FILE_PATH);

	Test();

	return S_OK;
}

//====================================================================
// 終了処理
//====================================================================
void CShuriken::Uninit()
{
	// 死亡フラグを立てる
	SetDeathFlag(true);
}

//====================================================================
// 更新処理
//====================================================================
void CShuriken::Update()
{
	// 継承クラスの更新
	CObjectX::Update();

	CollsionEnemy();

	Throw();	// 投げる
}

//====================================================================
// 描画処理
//====================================================================
void CShuriken::Draw()
{
	// 継承クラスの描画
	CObjectX::Draw();
}

//====================================================================
// 投げる
//====================================================================
void CShuriken::Throw(void)
{
	// 情報の取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	CDebugProc* pDebug = CManager::GetInstance()->GetDebugProc();
	CInputMouse* pMouse = CManager::GetInstance()->GetInputMouse();
	CPlayer* pPlayer = CGame::GetPlayer();
	CAim* pAim = CGame::GetAim();

	if (!pCamera || !pMouse || !pAim)
	{
		return;
	}

	// トランスフォームの取得
	D3DXVECTOR3 pos = GetPos();

	// カーソル方向を取得し、その方向への方向ベクトルを求める
	if (!m_info.bUse)
	{
		D3DXVECTOR3 posAim = pAim->GetPos();

		// 方向ベクトルを計算
		m_info.diff = posAim - pos;

		// 方向ベクトルを正規化
		D3DXVec3Normalize(&m_info.diff, &m_info.diff);

		m_info.bUse = true;
	}

	// ベクトルの方向へ投げる
	if (m_info.bUse)
	{	
		// 移動量を加算
		pos.x += m_info.move.x + m_info.diff.x * m_info.fSpeed * CManager::GetInstance()->GetGameSpeed();
		pos.y += m_info.move.y + m_info.diff.y * m_info.fSpeed * CManager::GetInstance()->GetGameSpeed();
		pos.z += m_info.move.z + m_info.diff.z * m_info.fSpeed * CManager::GetInstance()->GetGameSpeed();

		// 位置の更新
		SetPos(pos);

		// 向きに関する処理
		Rot();

		// 寿命を減らす処理
		LifeDecrease();
	}
}

//====================================================================
// 寿命の減少
//====================================================================
void CShuriken::LifeDecrease(void)
{
	// 寿命を減らす
	m_info.fLife -= Decrease * CManager::GetInstance()->GetGameSpeed();

	if (m_info.fLife <= 0.0f)
	{
		// 終了
		Uninit();
	}
}

//====================================================================
// 向き
//====================================================================
void CShuriken::Rot(void)
{
	// 情報の取得
	D3DXVECTOR3 rot = GetRot();

	// 回転
	rot.y += ROTATE_SPEED * CManager::GetInstance()->GetGameSpeed();

	// 向き設定
	SetRot(rot);
}

//====================================================================
// エイム
//====================================================================
void CShuriken::Aim(void)
{

}

//====================================================================
// ロックオン
//====================================================================
void CShuriken::Lockon(void)
{
	// 変数宣言
	float fTempLength = 10000.0f;
	D3DXMATRIX mtxProjection;
	D3DXMATRIX mtxView;
	D3DXMATRIX mtxWorld;
	D3DXVECTOR3 EnemyPos = { };
	D3DXVECTOR3 ScreenPos;
	D3DVIEWPORT9 Viewport;

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	pDevice->GetTransform(D3DTS_PROJECTION, &mtxProjection);
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);
	pDevice->GetViewport(&Viewport);

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	CDebugProc* pDebug = CManager::GetInstance()->GetDebugProc();
	CPlayer* pPlayer = CGame::GetPlayer();

	if (pPlayer != nullptr)
	{
		// 座標変換 (変換結果、変換する3D座標、ビューポート、プロジェクション行列、ワールド行列)
		D3DXVec3Project(&ScreenPos, &EnemyPos, &Viewport, &mtxProjection, &mtxView, &mtxWorld);

		pDebug->Print("2D座標　:%f %f %f\n", ScreenPos.x, ScreenPos.y, ScreenPos.z);
	}
}

//====================================================================
//= 敵との当たり判定
//====================================================================
void CShuriken::CollsionEnemy(void)
{
	CPlayer* pPlayer = CGame::GetPlayer();
	D3DXVECTOR3 posPlayer = pPlayer->GetPos();

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		//オブジェクトを取得
		CObject* pObj = CObject::GetTop(nCntPriority);

		while (pObj != NULL)
		{
			// オブジェクト情報を取得
			CObject* pObjNext = pObj->GetNext();	// 次のオブジェクト
			CObject::TYPE type = pObj->GetType();	// 種類

			// 種類の判定（敵[3D]）
			if (type == TYPE_ENEMY3D)
			{
				// 敵の情報を取得
				D3DXVECTOR3 ObjPos = pObj->GetPos();	// 位置
				D3DXVECTOR3 ObjSize = pObj->GetSize();	// 大きさ

				CEnemy* pEnemy = (CEnemy*)pObj;

				// 斬撃の当たり判定
				if (useful::CollisionRectangle(GetPos(),GetSize(), ObjPos, ObjSize))
				{
					// ダメージ処理
					pEnemy->HitDamage(10.0f);

					// 終了
					Uninit();
				}
			}

			// 次のオブジェクトを代入
			pObj = pObjNext;
		}
	}
}