//============================================
//
//	�K�w�\���Ń��[�V�������s�����f���̃T���v�� [SampleLevelModel.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _SAMPLELEVELMODEL_H_
#define _SAMPLELEVELMODEL_H_

#include "object.h"

//�O���錾
class CModel;
class CMotion;
class CObject3D;

//�I�u�W�F�N�g�v���C���[�N���X
class CSampleLvModel : public CObject
{
public:
	CSampleLvModel(int nPriority = 2);
	~CSampleLvModel();

	//�v���C���[�̃��[�V����
	enum ACTION_TYPE
	{
		ACTION_WAIT = 0,	//�ҋ@
		ACTION_MOVE,		//�ړ�
		ACTION_ATTACK1,		//�n��U���P
		ACTION_ATTACK2,		//�n��U���Q
		ACTION_ATTACK3,		//�n��U���R
		ACTION_SKYATTACK,	//�󒆍U��
		ACTION_JAMP,		//�W�����v
		ACTION_EVASION,		//���
		ACTION_TITLE,		//�^�C�g���̃|�[�Y
		ACTION_MAX,			//�ő�
	};

	//�v���C���[�̏��
	enum STATE
	{
		STATE_NORMAL = 0,	//�ʏ�
		STATE_DEATH,		//���S
		STATE_WAIT,			//�ҋ@
		STATE_DAMAGE,		//�_���[�W
		STATE_MAX,			//�ő�

	};

	D3DMATRIX GetMtxWorld(void) { return m_mtxWorld; }
	static CSampleLvModel* Create();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void TitleUpdate(void);
	void GameUpdate(void);
	void Draw(void);

	void LoadLevelData(const char* pFilename);
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	D3DXVECTOR3 GetMove(void) { return m_move; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	void SetWidth(float Width) { m_fWidth = Width; }
	float GetWidth(void) { return m_fWidth; }
	void SetHeight(float Height) { m_fHeight = Height; }
	float GetHeight(void) { return m_fHeight; }
	STATE GetState(void) { return m_State; }
	bool GetJump(void) { return m_bJump; }

private:
	void MyObjCreate(void);								//�������ێ�����I�u�W�F�N�g�̐���
	void ActionState(void);								//���[�V�����Ə�Ԃ̊Ǘ�
	void StateManager(void);							//��ԊǗ�

	ACTION_TYPE m_Action;
	ACTION_TYPE m_AtkAction;	//�U����ԋL�^�p�ϐ�
	D3DXVECTOR3 m_pos;			//�ʒu
	D3DXVECTOR3 m_posOld;		//�ߋ��̈ʒu
	D3DXVECTOR3 m_move;			//�ړ���
	D3DXVECTOR3 m_Objmove;		//�I�u�W�F�N�g����e�������ړ���
	D3DXVECTOR3 m_rot;			//����
	D3DXVECTOR3 m_AtkPos;		//�U���ʒu
	D3DXMATRIX m_mtxWorld;		//���[���h�}�g���b�N�X
	bool m_bJump;				//�W�����v���������ǂ���
	int m_nActionCount;			//�s���̃J�E���g
	bool m_nAttackHit;			//�U���������������ǂ���
	float m_fWidth;				//��
	float m_fHeight;			//����
	STATE m_State;				//���
	int m_nStateCount;			//��ԊǗ��p�J�E���g

	float m_CollisionRot;		//�����蔻��p�̌���

	//�K�w�\���ƃ��[�V�����̃|�C���^
	CModel* m_apModel[64];
	CMotion* m_pMotion;
	char* m_aModelName[64];
	int m_nNumModel;
};

#endif