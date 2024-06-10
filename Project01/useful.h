//========================================================================================
//
//�@�֗��֐�[useful.h]
//	Author:sakamoto kai
//
//========================================================================================
#ifndef _USEFUL_H_
#define _USEFUL_H_

#include "main.h"

// ���O���
namespace useful
{
	//�����蔻��̎g�p��
	enum COLLISION
	{
		COLLISION_XY = 0,
		COLLISION_YZ,
		COLLISION_ZX,
		COLLISION_MAX,
	};

	bool CollisionRectangle2D(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, D3DXVECTOR3 Size1, D3DXVECTOR3 Size2, useful::COLLISION Shaft);
	bool CameraFront(D3DXVECTOR3 pos);
	bool SlashCollision(D3DXVECTOR3* EnemyPos, int Max, float SlashRot);
	void CollisionWindow(D3DXVECTOR3* pos);
	bool CollisionWindow(D3DXVECTOR3 pos);
	bool CollisionCircle(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float nRadiusOut);	//�~�̓����蔻��
	bool CollisionRectangle(D3DXVECTOR3 posMy, D3DXVECTOR3 sizeMy, D3DXVECTOR3 posPair, D3DXVECTOR3 sizePair);
	void NormalizeAngle(D3DXVECTOR3* rot);
	void Vec3Project(D3DXVECTOR3* pOut, D3DXVECTOR3 v);	// 2D���W�ɕϊ�
}

#endif