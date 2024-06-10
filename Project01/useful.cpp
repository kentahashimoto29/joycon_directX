//========================================================================================
//
//�@�֗��֐�[useful.cpp]
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
//�J�����̑O���ɑ��݂��Ă��邩�ǂ���
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
//�a���̓����蔻��
//====================================================================
bool useful::SlashCollision(D3DXVECTOR3 *EnemyPos, int Max, float SlashRot)
{
	//�v���C���[�ƓG�̈ʒu�֌W�������O��
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

	//�G�̓_�̊ԂɍU�����̐��������Ă����ꍇ
	if (Left > 0 && Right > 0)
	{
		return true;
	}

	return false;
}

//====================================================================
//�E�B���h�E��ʂ̓����蔻��
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
//�E�B���h�E��ʂ̓����蔻��
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
//�~�̓����蔻��
//====================================================================
bool useful::CollisionCircle(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float nRadiusOut)
{
	bool nHit = false;

	if (sqrtf((pos1.x - pos2.x) * (pos1.x - pos2.x)
		+ (pos1.y - pos2.y) * (pos1.y - pos2.y)) <= nRadiusOut)
	{//�~�̔��肪��������
		if (sqrtf((pos1.x - pos2.x) * (pos1.x - pos2.x)
			+ (pos1.z - pos2.z) * (pos1.z - pos2.z)) <= nRadiusOut)
		{//�~�̔��肪��������
			nHit = true;
		}
	}

	return nHit;
}

//====================================================================
// ��`�̓����蔻��i����j
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
// �p�x�̐��K���i����j
//====================================================================
void useful::NormalizeAngle(D3DXVECTOR3* rot)
{
	// �ړI�̌����̕␳
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
// 2D���W�ɕϊ� CieCv2D(�o�͐�A�ϊ����W)
//====================================================================
void useful::Vec3Project(D3DXVECTOR3 *pOut, D3DXVECTOR3 v)
{
	// �ϐ��錾
	D3DXMATRIX mtxProjection;
	D3DXMATRIX mtxView;
	D3DXMATRIX mtxWorld;
	D3DVIEWPORT9 Viewport;

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	pDevice->GetTransform(D3DTS_PROJECTION, &mtxProjection);
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);
	pDevice->GetViewport(&Viewport);

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	// ���W�ϊ�
	D3DXVec3Project(pOut, &v, &Viewport, &mtxProjection, &mtxView, NULL);
}
