//============================================
//
//	�a������ [slash.h]
//	Author:SakaiMinato
//
//============================================

#ifndef _SLASH_H_
#define _SLASH_H_

#include "main.h"
#include "object.h"

#include "object2D.h"

//�}�N����`
#define SLASH_2D_NUM (4)	// �X���b�V���̐�

// �I�u�W�F�N�g�v���C���[�N���X
class CSlash : public CObject
{
public:

	// ���
	enum STATE
	{
		STATE_NONE = 0,
		STATE_HIT,
		STATE_MAX
	};

	// 2D�̏��
	struct SInfo2D
	{
		D3DXVECTOR3 pos;	// �ʒu���
		D3DXVECTOR2 size;	// �傫�����
		D3DXCOLOR color;	// �F���

		CObject2D* pObj2d;	// 2D�I�u�W�F�N�g
	};

	// ���
	struct SInfo
	{
		D3DXVECTOR3 pos;		// �ʒu���
		D3DXCOLOR color;		// �F���

		D3DXVECTOR3 angle;		// ����

		D3DXVECTOR2 sizeDef;	// �f�t�H���g�T�C�Y

		int nCnt;				// �J�E���g
		bool AllAppear;			// �S�\���̗L��
		
		STATE state;					// ���
		SInfo2D aInfo2D[SLASH_2D_NUM];	// 2D�̏��
	};

	CSlash(int nPriority = 6);
	~CSlash();

	static CSlash* Create(const char* p2dFileName);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:

	void InitObj2D(const char* p2dFileName);

	void EventCnt(void);

	void CollsionEnemy(void);

	void Set2dSize(D3DXVECTOR2 size, int nCnt);
	void Dec2dSize(D3DXVECTOR2 sizeDec, int nCnt);
	void Set2dAppear(void);

	SInfo m_info;	// ���
};
#endif