//========================================================================================
//
//　便利関数[useful.cpp]
//	Author:sakamoto kai
//
//========================================================================================
#include "useful.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"

//====================================================================
//
//====================================================================
bool useful::CollisionRectangle2D(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 Size1, D3DXVECTOR3 Size2, useful::COLLISION Shaft)
{
	bool Hit = false;

	switch (Shaft)
	{
	case useful::COLLISION_XY:
		if (pos1.x + Size1.x > pos2.x - Size2.x &&
			pos1.x - Size1.x < pos2.x + Size2.x &&
			pos1.y + Size1.y > pos2.y - Size2.y &&
			pos1.y - Size1.y < pos2.y + Size2.y)
		{
			Hit = true;
		}
		break;

	case useful::COLLISION_YZ:
		if (pos1.y + Size1.y < pos2.y - Size2.y &&
			pos1.y - Size1.y > pos2.y + Size2.y &&
			pos1.z + Size1.z < pos2.z - Size2.z &&
			pos1.z - Size1.z > pos2.z + Size2.z)
		{
			Hit = true;
		}
		break;

	case useful::COLLISION_ZX:
		if (pos1.z + Size1.z < pos2.z - Size2.z &&
			pos1.z - Size1.z > pos2.z + Size2.z &&
			pos1.x + Size1.x < pos2.x - Size2.x &&
			pos1.x - Size1.x > pos2.x + Size2.x)
		{
			Hit = true;
		}
		break;
	}

	return Hit;
}

//====================================================================
//カメラの前側に存在しているかどうか
//====================================================================
bool useful::CameraFront(D3DXVECTOR3 pos)
{
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	D3DXVECTOR3 CamPos = pCamera->GetPosV();
	float CamRot = pCamera->GetRot().y;

	D3DXVECTOR3 vecToPos;
	D3DXVECTOR3 vecLine;
	float A;

	vecToPos.x = (CamPos.x) - (pos.x);
	vecToPos.z = (CamPos.z) - (pos.z);

	vecLine.x = sinf(CamRot + D3DX_PI * 0.5f);
	vecLine.y = 0.0f;
	vecLine.z = cosf(CamRot + D3DX_PI * 0.5f);

	A = (vecLine.z * vecToPos.x) - (vecLine.x * vecToPos.z);

	if (A > 0)
	{
		return true;
	}
	return false;
}

//====================================================================
//斬撃の当たり判定
//====================================================================
bool useful::SlashCollision(D3DXVECTOR3 *EnemyPos, int Max, float SlashRot)
{
	//プレイヤーと敵の位置関係を示す外積
	int Left = 0;
	int	Right = 0;

	for (int nCnt = 0; nCnt < Max; nCnt++, EnemyPos++)
	{
		D3DXVECTOR3 vecToPos;
		D3DXVECTOR3 vecLine;
		float A;

		vecToPos.x = (EnemyPos->x) - (SCREEM_CENTER.x);
		vecToPos.y = (EnemyPos->y) - (SCREEM_CENTER.y);

		vecLine.x = sinf(SlashRot);
		vecLine.y = cosf(SlashRot);
		vecLine.z = 0.0f;

		A = (vecLine.y * vecToPos.x) - (vecLine.x * vecToPos.y);

		if (A < 0)
		{
			Right++;
		}
		else
		{
			Left++;
		}
	}

	//敵の点の間に攻撃時の線が入っていた場合
	if (Left > 0 && Right > 0)
	{
		return true;
	}

	return false;
}

//====================================================================
//ウィンドウ画面の当たり判定
//====================================================================
void useful::CollisionWindow(D3DXVECTOR3* pos)
{
	if (pos->x >= SCREEN_WIDTH)
	{
		pos->x = SCREEN_WIDTH;
	}
	else if (pos->x <= 0.0f)
	{
		pos->x = 0.0f;
	}

	if (pos->y >= SCREEN_HEIGHT)
	{
		pos->y = SCREEN_HEIGHT;
	}
	else if (pos->y <= 0.0f)
	{
		pos->y = 0.0f;
	}
}

//====================================================================
//ウィンドウ画面の当たり判定
//====================================================================
bool useful::CollisionWindow(D3DXVECTOR3 pos)
{
	bool bHit = false;

	if (pos.x >= SCREEN_WIDTH)
	{
		bHit = true;
	}
	else if (pos.x <= 0.0f)
	{
		bHit = true;
	}

	if (pos.y >= SCREEN_HEIGHT)
	{
		bHit = true;
	}
	else if (pos.y <= 0.0f)
	{
		bHit = true;
	}

	return bHit;
}

//====================================================================
//円の当たり判定
//====================================================================
bool useful::CollisionCircle(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float nRadiusOut)
{
	bool nHit = false;

	if (sqrtf((pos1.x - pos2.x) * (pos1.x - pos2.x)
		+ (pos1.y - pos2.y) * (pos1.y - pos2.y)) <= nRadiusOut)
	{//円の判定が当たった
		if (sqrtf((pos1.x - pos2.x) * (pos1.x - pos2.x)
			+ (pos1.z - pos2.z) * (pos1.z - pos2.z)) <= nRadiusOut)
		{//円の判定が当たった
			nHit = true;
		}
	}

	return nHit;
}

//====================================================================
// 矩形の当たり判定（酒井）
//====================================================================
bool useful::CollisionRectangle(D3DXVECTOR3 posMy, D3DXVECTOR3 sizeMy, D3DXVECTOR3 posPair, D3DXVECTOR3 sizePair)
{
	if (posMy.x + sizeMy.x >= posPair.x - sizePair.x &&
		posMy.x - sizeMy.x <= posPair.x + sizePair.x &&
		posMy.y + sizeMy.y >= posPair.y - sizePair.y &&
		posMy.y - sizeMy.y <= posPair.y + sizePair.y &&
		posMy.z + sizeMy.z >= posPair.z - sizePair.z &&
		posMy.z - sizeMy.z <= posPair.z + sizePair.z)
	{
		return true;
	}

	return false;
}

//====================================================================
// 角度の正規化（酒井）
//====================================================================
void useful::NormalizeAngle(D3DXVECTOR3* rot)
{
	// 目的の向きの補正
	if (rot->y > D3DX_PI)
	{
		rot->y += -D3DX_PI * 2;
	}
	else if (rot->y < -D3DX_PI)
	{
		rot->y += D3DX_PI * 2;
	}
}

//====================================================================
// 2D座標に変換 CieCv2D(出力先、変換座標)
//====================================================================
void useful::Vec3Project(D3DXVECTOR3 *pOut, D3DXVECTOR3 v)
{
	// 変数宣言
	D3DXMATRIX mtxProjection;
	D3DXMATRIX mtxView;
	D3DXMATRIX mtxWorld;
	D3DVIEWPORT9 Viewport;

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	pDevice->GetTransform(D3DTS_PROJECTION, &mtxProjection);
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);
	pDevice->GetViewport(&Viewport);

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	// 座標変換
	D3DXVec3Project(pOut, &v, &Viewport, &mtxProjection, &mtxView, NULL);
}
