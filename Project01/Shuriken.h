//========================================================================================
//
//�@�藠��[Shuriken.h]
//	Author:Takeru Ogasawara
//
//========================================================================================
#ifndef _SHURIKEN_H_
#define _SHURIKEN_H_

// �C���N���[�h�錾
#include "main.h"
#include "objectX.h"

// �O���錾
class CAim;

// �藠���N���X
class CShuriken : public CObjectX
{
public:

	CShuriken();
	~CShuriken();

	static CShuriken* Create(D3DXVECTOR3 pos, float fSpeed = 1.0f, float fLife = 20.0f);

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

private:
	void Throw(void);	// ������
	void LifeDecrease(void);
	void Rot(void);

	void Aim(void);
	void Lockon(void);

	void CollsionEnemy(void);

	// ���
	struct SInfo
	{
		D3DXVECTOR3 move;
		D3DXVECTOR3 diff;
		float fSpeed;
		float fLife;
		bool bUse;
	};

	// �����o�ϐ�
	SInfo m_info;
};

#endif