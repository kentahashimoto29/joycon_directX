//=============================================
//=
//=	斬撃 [slash.cpp]
//=	Author:sakamoto kai
//=
//=============================================

#include "main.h"
#include "slash.h"

#include "useful.h"

#include "input.h"
#include "debugproc.h"

#include "game.h"
#include "aim.h"

#include "enemy.h"

#include "player.h"

// 定数定義
const float LENGTH = 200.0f;		// 長さ

const D3DXVECTOR2 SIZE_DEFAULT =	// 大きさのデフォルト
		D3DXVECTOR2(15.0f, LENGTH * 0.5f);
const D3DXVECTOR2 SIZE_DEST_2D = 	// 大きさの減少量
		D3DXVECTOR2(2.0f, 0.0f);

const D3DXCOLOR HIT_RED = 	// ヒット時のポリゴン色
		D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

const int SCREEN_POS_ENEMY_NUM = 8;	// 敵のスクリーン座標の数（仮）

const int EVENT_SPWAN_SLASH_DIV = 2;	// 次のスラッシュコマまでの出現フレーム
const int EVENT_DEST_START = 30;		// 大きさの減少開始のフレーム
const int EVENT_END = 60;				// 終了のフレーム

//---------------------------------------------------------
//- コンストラクタ
//---------------------------------------------------------
CSlash::CSlash(int nPriority) :CObject(nPriority)
{
	// 斬撃情報の初期化
	ZeroMemory(&m_info,sizeof(m_info));
}

//---------------------------------------------------------
//- デストラクタ
//---------------------------------------------------------
CSlash::~CSlash()
{

}

//---------------------------------------------------------
//- 生成処理
//---------------------------------------------------------
CSlash* CSlash::Create(const char* p2dFileName)
{
	// 生成
	CSlash* pInstance = new CSlash();

	// 初期化処理
	if (FAILED(pInstance->Init()))
	{//初期化処理が失敗した場合
		return nullptr;
	}

	// 2Dオブジェクトの初期化
	pInstance->InitObj2D(p2dFileName);

	return pInstance;
}

//---------------------------------------------------------
//- 初期化処理
//---------------------------------------------------------
HRESULT CSlash::Init(void)
{
	//デバイスの取得
	CInputJoypad* pInputJoypad = CManager::GetInstance()->GetInputJoyPad();

	//// 照準を取得
	//CAim* pAim = CGame::GetAim();

	//if (pAim != nullptr)
	//{
	//	// 位置を取得
	//	D3DXVECTOR3 posAim = pAim->GetPos();

	//	// カーソル位置から中心への方向を算出
	//	m_info.angle.z = atan2f(SCREEM_CENTER.x - posAim.x, SCREEM_CENTER.y - posAim.y) + (D3DX_PI);
	//}

	if (pInputJoypad != nullptr)
	{
		// コントローラーの左スティック情報を取得（1番目のコントローラー）
		D3DXVECTOR3 rotStick = pInputJoypad->Get_Stick_Right(0);
		D3DXVECTOR3 pi = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		
		// 度数法算出
		pi.y = atan2f(-rotStick.x, rotStick.y);

		// カーソル位置から中心への方向を算出
		m_info.angle.z = pi.y;
	}

	// 値を代入（仮)
	m_info.pos = D3DXVECTOR3(						// 位置
		sinf(m_info.angle.z) * LENGTH, 
		cosf(m_info.angle.z) * LENGTH, 
		0.0f);	
	m_info.sizeDef = SIZE_DEFAULT;					// デフォルトサイズ
	m_info.color = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);	// 色

	return S_OK;
}

//---------------------------------------------------------
//- 終了処理
//---------------------------------------------------------
void CSlash::Uninit(void)
{
	// 2Dの終了処理
	for (int nCnt = 0; nCnt < SLASH_2D_NUM; nCnt++)
	{
		if (m_info.aInfo2D[nCnt].pObj2d != nullptr)
		{
			// 終了処理
			m_info.aInfo2D[nCnt].pObj2d->Uninit();
			m_info.aInfo2D[nCnt].pObj2d = nullptr;
		}
	}

	// 死亡フラグ建築
	SetDeathFlag(true);
}

//---------------------------------------------------------
//- 更新処理
//---------------------------------------------------------
void CSlash::Update(void)
{
	// カウントごとのイベント
	EventCnt();

	// 状態判定（通常）
	if (m_info.state == STATE_NONE)
	{
		// 敵との当たり判定
		CollsionEnemy();
	}
}

//---------------------------------------------------------
//- 描画処理
//---------------------------------------------------------
void CSlash::Draw(void)
{

}

//---------------------------------------------------------
//- 2Dオブジェクトの初期化処理
//---------------------------------------------------------
void CSlash::InitObj2D(const char* p2dFileName)
{
	for (int nCnt = 0; nCnt < SLASH_2D_NUM; nCnt++)
	{
		// 生成処理
		m_info.aInfo2D[nCnt].pObj2d = CObject2D::Create();

		if (m_info.aInfo2D[nCnt].pObj2d != nullptr)
		{
			// ゲキキモ計算式（仮）
			float fLength = LENGTH + ((nCnt + 0.5f) * -(LENGTH * 0.5f));

			// 値代入（仮）
			m_info.aInfo2D[nCnt].pObj2d->SetPos(
					SCREEM_CENTER + D3DXVECTOR3(sinf(m_info.angle.z) * fLength, 
					cosf(m_info.angle.z) * fLength, 
					0.0f));
			Set2dSize(m_info.sizeDef,nCnt);
			m_info.aInfo2D[nCnt].color = m_info.color;
			m_info.aInfo2D[nCnt].pObj2d->SetRot(m_info.angle);
			m_info.aInfo2D[nCnt].pObj2d->SetAppear(false);

			//// テクスチャ設定処理
			//m_pObj2d->SetTexture(p2dFileName);
		}
	}
}

//---------------------------------------------------------
//- カウントごとのイベント
//---------------------------------------------------------
void CSlash::EventCnt(void)
{
	m_info.nCnt++;

	// 描画設定イベント
	if (m_info.nCnt % EVENT_SPWAN_SLASH_DIV == 0)
	{
		if (m_info.AllAppear == false)
		{
			// 描画設定
			Set2dAppear();
		}
	}
	else if (m_info.nCnt >= EVENT_DEST_START)
	{
		for (int nCnt = 0; nCnt < SLASH_2D_NUM; nCnt++)
		{
			// 2Dの減少処理
			Dec2dSize(SIZE_DEST_2D, nCnt);
		}
	}

	// 終了イベント
	if (m_info.nCnt % EVENT_END == 0)
	{
		// 終了処理
		Uninit();
	}
}

//---------------------------------------------------------
//- 敵との当たり判定
//---------------------------------------------------------
void CSlash::CollsionEnemy(void)
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
				float ObjWidth = pObj->GetSize().x;		// サイズ

				CEnemy* pEnemy = (CEnemy*)pObj;

				// ターゲットとの距離を判定
				float fLengthTgtSquared = ((ObjPos.x - posPlayer.x) * (ObjPos.x - posPlayer.x) +
										   (ObjPos.y - posPlayer.y) * (ObjPos.y - posPlayer.y) +
										   (ObjPos.z - posPlayer.z) * (ObjPos.z - posPlayer.z));

				// 距離を返す
				float fLength = std::sqrt(fLengthTgtSquared);

				// 斬撃の当たり判定
				if (useful::SlashCollision(pObj->GetScreenPos(), SCREEN_POS_ENEMY_NUM, m_info.angle.z) &&
					fLength <= 200.0f)
				{
					// 色を変更（赤）
					m_info.color = HIT_RED;
					m_info.state = STATE_HIT;

					for (int nCnt = 0; nCnt < SLASH_2D_NUM; nCnt++)
					{
						// 2Dの色を変更（赤）
						m_info.aInfo2D[nCnt].pObj2d->SetColor(m_info.aInfo2D[nCnt].color = m_info.color);
					}

					// ダメージ処理
					pEnemy->HitDamage(10.0f);
				}
			}

			// 次のオブジェクトを代入
			pObj = pObjNext;
		}
	}
}

//---------------------------------------------------------
//- 2Dのサイズ設定処理
//---------------------------------------------------------
void CSlash::Set2dSize(D3DXVECTOR2 size, int nCnt)
{
	m_info.aInfo2D[nCnt].size = size;
	m_info.aInfo2D[nCnt].pObj2d->SetWidth(m_info.aInfo2D[nCnt].size.x);
	m_info.aInfo2D[nCnt].pObj2d->SetHeight(m_info.aInfo2D[nCnt].size.y);
}

//---------------------------------------------------------
//- 2Dのサイズ減少処理
//---------------------------------------------------------
void CSlash::Dec2dSize(D3DXVECTOR2 sizeDec, int nCnt)
{
	m_info.aInfo2D[nCnt].size -= sizeDec;
	m_info.aInfo2D[nCnt].pObj2d->SetWidth(m_info.aInfo2D[nCnt].size.x);
	m_info.aInfo2D[nCnt].pObj2d->SetHeight(m_info.aInfo2D[nCnt].size.y);
}

//---------------------------------------------------------
//- 描画有無の設定処理
//---------------------------------------------------------
void CSlash::Set2dAppear(void)
{
	// 斬撃2Dの最大数
	for (int nCnt = 0; nCnt < SLASH_2D_NUM; nCnt++)
	{
		// 描画の有無を判定
		if (m_info.aInfo2D[nCnt].pObj2d->GetAppear() == false)
		{
			// 描画
			m_info.aInfo2D[nCnt].pObj2d->SetAppear(true);

			// 最大数
			if (nCnt == SLASH_2D_NUM - 1)
			{
				// 全描画設定をオン
				m_info.AllAppear = true;
			}

			break;
		}
	}
}

