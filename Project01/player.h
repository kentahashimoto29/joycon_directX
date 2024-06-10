//============================================
//
//	�v���C���[�̏��� [Player.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "object.h"

//�O���錾
class CModel;
class CMotion;
class CObject3D;
class CObject3D;
class CObjGauge2D;

class CEnemy;

//�I�u�W�F�N�g�v���C���[�N���X
class CPlayer : public CObject
{
public:
	CPlayer(int nPriority = 2);
	~CPlayer();

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
	static CPlayer* Create();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void TitleUpdate(void);
	void GameUpdate(void);
	void Draw(void);

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
	void SetCameraPos(D3DXVECTOR3 pos) { m_CameraPos = pos; }
	D3DXVECTOR3 GetCameraPos(void) { return m_CameraPos; }
	float GetCameraHeightRot(void) { return m_HeightCameraRot; }

	void HitDamage(float Damage);

private:
	void MyObjCreate(void);		//�������ێ�����I�u�W�F�N�g�̐���
	void ActionState(void);		//���[�V�����Ə�Ԃ̊Ǘ�
	void StateManager(void);	//��ԊǗ�
	void Move(void);			//�ړ�����
	void Rot(void);				//�ړ���������
	void Tracking(void);		//�ǔ�����
	void CollisionEnemy(void);	//�G�Ƃ̋�������
	void Shuriken(void);		//�藠���̏���
	void LifeGaugeCreate();		//���C�t�Q�[�W����
	void Evasion();				//���

	void CameraPosUpdate(void);	//�J�����ʒu�X�V����
	void PosUpdate(void);		//�ʒu�X�V����
	void RotUpdate(void);		//�����X�V����

	void DebugKey(void);		//�f�o�b�O�L�[

	void LoadLevelData(const char* pFilename);

	ACTION_TYPE m_Action;
	ACTION_TYPE m_AtkAction;	//�U����ԋL�^�p�ϐ�
	D3DXVECTOR3 m_pos;			//�ʒu
	D3DXVECTOR3 m_posOld;		//�ߋ��̈ʒu
	D3DXVECTOR3 m_move;			//�ړ���
	D3DXVECTOR3 m_Objmove;		//�I�u�W�F�N�g����e�������ړ���
	D3DXVECTOR3 m_rot;			//����
	D3DXVECTOR3 m_rotDest;		//�����̖ړI�n
	float m_HeightCameraRot;	//�J�����̍����̊p�x
	D3DXVECTOR3 m_AtkPos;		//�U���ʒu
	D3DXVECTOR3 m_CameraPos;	//�J�����ʒu�ʒu
	D3DXMATRIX m_mtxWorld;		//���[���h�}�g���b�N�X
	bool m_bJump;				//�W�����v���������ǂ���
	int m_nActionCount;			//�s���̃J�E���g
	bool m_nAttackHit;			//�U���������������ǂ���
	float m_fWidth;				//��
	float m_fHeight;			//����
	STATE m_State;				//���
	int m_nStateCount;			//��ԊǗ��p�J�E���g

	float m_CollisionRot;		//�����蔻��p�̌���

	CEnemy* m_pTgtEnemy;		//�^�[�Q�b�g�̓G�|�C���^

	float m_fLife;					//���C�t
	float m_fLifeMax;				//���C�t�̍ő�l
	CObjGauge2D* m_pLifeGauge;		//���C�t�Q�[�W�̃|�C���^

	int m_nEvasionCount;		//����J�E���g
	int m_nEvasionLR;			//������

	//�K�w�\���ƃ��[�V�����̃|�C���^
	CModel* m_apModel[64];
	CMotion* m_pMotion;
	char* m_aModelName[64];
	int m_nNumModel;
};

#endif