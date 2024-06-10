//============================================
//
//	�G�̏��� [enemy.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "object.h"

//�O���錾
class CModel;
class CMotion;
class CObject3D;

//�I�u�W�F�N�g�v���C���[�N���X
class CEnemy : public CObject
{
public:
	CEnemy(int nPriority = 2);
	~CEnemy();

	//�v���C���[�̃��[�V����
	enum ACTION_TYPE
	{
		ACTION_WAIT = 0,	//�ҋ@
		ACTION_MOVE,		//�ړ�
		ACTION_ATTACK1,		//�n��U��
		ACTION_JAMP,		//�W�����v
		ACTION_MAX,			//�ő�
	};

	//�G�̏��
	enum STATE
	{
		STATE_NORMAL = 0,	//�ʏ�
		STATE_BATTLE,		//�퓬
		STATE_SEATCH,		//�T��
		STATE_DEATH,		//���S
		STATE_MAX,			//�ő�
	};

	//�G�̐퓬���
	enum BATTLESTATE
	{
		BATTLESTATE_NORMAL = 0,	//�ʏ�
		BATTLESTATE_DEATH,		//���S
		BATTLESTATE_WAIT,		//�ҋ@
		BATTLESTATE_DAMAGE,		//�_���[�W
		BATTLESTATE_WALK,		//���s
		BATTLESTATE_MAX,		//�ő�
	};


	//�G�̒T�����
	enum SEARCHSTATE
	{
		SEARCHSTATE_NORMAL = 0,	//�ʏ�
		SEARCHSTATE_WALK,		//���s
		SEARCHSTATE_TRUN,		//�����]��
		SEARCHSTATE_WAIT,		//�ҋ@
		SEARCHSTATE_SEARCH,		//�T��
		SEARCHSTATE_MAX,		//�ő�
	};

	D3DMATRIX GetMtxWorld(void) { return m_mtxWorld; }
	static CEnemy* Create();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void TitleUpdate(void);
	void GameUpdate(void);
	void Draw(void);

	void LoadLevelData(const char* pFilename);

	void HitDamage(float fDamage);

	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	D3DXVECTOR3 GetMove(void) { return m_move; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	void SetSize(D3DXVECTOR3 size) { m_size = size; }
	D3DXVECTOR3 GetSize(void) { return m_size; }
	BATTLESTATE GetBattleState(void) { return m_BState; }
	SEARCHSTATE GetSearchState(void) { return m_SState; }
	bool GetJump(void) { return m_bJump; }
	D3DXVECTOR3* GetScreenPos(void) { return &m_ScreenPos[0]; }

	void SetSeatchPos(D3DXVECTOR3 pos, int nCnt) { m_SeatchPos[nCnt] = pos; }
	void SetSeatchLoop(bool Loop) { m_bSeatchLoop = Loop; }
	void SetState(STATE State) { m_State = State; }
	STATE GetState(void) { return m_State; }
	void SetSeatchState(SEARCHSTATE State) { m_SState = State; }

private:

	void Death(void);

	void ScreenCollision();

	void MyObjCreate(void);								//�������ێ�����I�u�W�F�N�g�̐���
	void ActionState(void);								//���[�V�����Ə�Ԃ̊Ǘ�
	void BattleStateManager(void);						//�퓬��ԊǗ�
	void SeatchStateManager(void);						//�T����ԊǗ�
	void CollisionPush(void);							//�����ȊO�̓G�������o������

	ACTION_TYPE m_Action;
	ACTION_TYPE m_AtkAction;	//�U����ԋL�^�p�ϐ�
	D3DXVECTOR3 m_pos;			//�ʒu
	D3DXVECTOR3 m_posOld;		//�ߋ��̈ʒu
	D3DXVECTOR3 m_move;			//�ړ���
	D3DXVECTOR3 m_Objmove;		//�I�u�W�F�N�g����e�������ړ���
	D3DXVECTOR3 m_rot;			//����
	D3DXVECTOR3 m_AtkPos;		//�U���ʒu
	D3DXVECTOR3 m_SeatchPos[32];//�T���ʒu
	int m_nSeatchCount;			//�T���i�K�̃J�E���g
	bool m_bSeatchLoop;			//�T�������[�v���邩�ǂ���
	D3DXMATRIX m_mtxWorld;		//���[���h�}�g���b�N�X
	bool m_bJump;				//�W�����v���������ǂ���
	int m_nActionCount;			//�s���̃J�E���g
	bool m_nAttackHit;			//�U���������������ǂ���
	D3DXVECTOR3 m_size;			//�傫��
	STATE m_State;				//���
	BATTLESTATE m_BState;		//�퓬���
	SEARCHSTATE m_SState;		//�T�����
	int m_nStateCount;			//��ԊǗ��p�J�E���g

	float m_fLife;				//�̗�

	D3DXVECTOR3 m_ScreenPos[8];	//�X�N���[�����W
	float m_CollisionRot;		//�����蔻��p�̌���

	//�K�w�\���ƃ��[�V�����̃|�C���^
	CModel* m_apModel[64];
	CMotion* m_pMotion;
	char* m_aModelName[64];
	int m_nNumModel;
};

#endif