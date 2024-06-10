//============================================
//
//	�}�b�v���f�� [MapModel.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _MAPMODEL_H_
#define _MAPMODEL_H_

#include "main.h"
#include "objectX.h"

//�O���錾
class CObjGauge2D;
class CCubeBlock;

//�I�u�W�F�N�g�v���C���[�N���X
class CMapModel : public CObjectX
{
public:

	CMapModel(int nPriority = 3);
	~CMapModel();

	//�T���v���̏��
	enum STATE
	{
		STATE_NORMAL = 0,	//�ʏ�
		STATE_ACTION,		//�s��
		STATE_MAX,
	};

	static CMapModel* Create(char* pModelName);

	HRESULT Init(char* pModelName);
	void Uninit(void);
	void Update(void);
	void TitleUpdate(void);
	void GameUpdate(void);
	void Draw(void);
	void SetCollision(bool Set) { m_bCollision = Set; }
	bool GetCollision(void) { return m_bCollision; }
	void CreateBlock(void);		//�f�o�b�O�p�u���b�N�̐���
	void UpdateBlock(void);		//�f�o�b�O�p�u���b�N�̍X�V
	void SetEditIdx(int Idx) { m_EditIndex = Idx; }
	int GetEditIdx(void) { return m_EditIndex; }

private:
	void StateManager(void);	//��ԊǗ�

	D3DXVECTOR3 m_CollisionPos;		//�����蔻��p�̍��W
	bool m_bCollision;				//�����蔻��p�̍��W
	STATE m_State;					//���
	int m_nStateCount;				//��ԊǗ��p�ϐ�
	float m_Scaling;				//�傫��
	D3DXVECTOR3 m_posOld;			//�ߋ��̈ʒu	
	D3DXVECTOR3 m_move;				//�ړ���	
	float m_fColorA;				//�s�����x
	int m_EditIndex;				//�G�f�B�b�g���̃C���f�b�N�X

	CCubeBlock* m_pDebugBlock;		//�f�o�b�O�p�u���b�N
};
#endif