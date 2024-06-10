//============================================
//
//	�v���C���[�̏��� [player.cpp]
//	Author:sakamoto kai
//
//============================================
#include "player.h"
#include "object.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "motion.h"
#include "game.h"
#include "object3D.h"
#include "debugproc.h"
#include "2DEffect.h"
#include "camera.h"
#include "input.h"
#include "enemy.h"
#include "useful.h"
#include "Aim.h"
#include "Shuriken.h"
#include "objGauge2D.h"

#define COLLISION_SIZE (D3DXVECTOR3(20.0f,40.0f,20.0f))		//���̓����蔻��
#define PLAYER_SPEED (10.0f)		//�v���C���[�̈ړ����x

namespace
{
	const float MAX_LIFE = 100.0f;			//�̗͍ő�l
	const float EVADION_MOVE = 40.0f;		//����ړ���
	const int EVADION_FRAME = 7;		//����t���[��

}

//====================================================================
//�R���X�g���N�^
//====================================================================
CPlayer::CPlayer(int nPriority) :CObject(nPriority)
{
	SetWidth(COLLISION_SIZE.x);
	SetHeight(COLLISION_SIZE.y);
	m_pos = INITVECTOR3;
	m_move = INITVECTOR3;
	m_Objmove = INITVECTOR3;
	m_rot = D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f);
	m_bJump = false;
	m_nActionCount = 0;
	m_Action = ACTION_WAIT;
	m_AtkAction = ACTION_WAIT;
	m_nAttackHit = false;
	m_State = STATE_NORMAL;
	m_nStateCount = 0;
	m_AtkPos = INITVECTOR3;
	m_CollisionRot = 0.0f;
	m_HeightCameraRot = 0.0f;
	m_fLife = MAX_LIFE;
	m_fLifeMax = MAX_LIFE;
	m_pLifeGauge = nullptr;

	m_pTgtEnemy = nullptr;
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CPlayer::~CPlayer()
{

}

//====================================================================
//��������
//====================================================================
CPlayer* CPlayer::Create()
{
	CPlayer* pPlayer = NULL;

	if (pPlayer == NULL)
	{
		//�v���C���[�̐���
		pPlayer = new CPlayer();
	}

	//�I�u�W�F�N�g�̏���������
	if (FAILED(pPlayer->Init()))
	{//���������������s�����ꍇ
		return NULL;
	}

	return pPlayer;
}

//====================================================================
//����������
//====================================================================
HRESULT CPlayer::Init(void)
{
	//���f���̐���
	LoadLevelData("data\\TXT\\motion_player.txt");

	//���[�V�����̐���
	if (m_pMotion == NULL)
	{
		//���[�V�����̐���
		m_pMotion = new CMotion;
	}

	//����������
	m_pMotion->SetModel(&m_apModel[0], m_nNumModel);
	m_pMotion->LoadData("data\\TXT\\motion_player.txt");

	//�̗̓Q�[�W����
	if (CScene::GetMode() == CScene::MODE_GAME)
	{
		LifeGaugeCreate();
	}

	//��ސݒ�
	SetType(CObject::TYPE_PLAYER3D);

	switch (CScene::GetMode())
	{
	case CScene::MODE_TITLE:
		m_pMotion->Set(ACTION_TITLE, 0);
		break;

	case CScene::MODE_GAME:
		break;

	case CScene::MODE_RESULT:
		break;
	}

	return S_OK;
}

//====================================================================
//�������ێ�����I�u�W�F�N�g�̐���
//====================================================================
void CPlayer::MyObjCreate(void)
{

}

//====================================================================
//�I������
//====================================================================
void CPlayer::Uninit(void)
{
	for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{
		m_apModel[nCntModel]->Uninit();
		delete m_apModel[nCntModel];
		m_apModel[nCntModel] = NULL;
	}

	//���[�V�����̏I������
	if (m_pMotion != NULL)
	{
		//���[�V�����̔j��
		delete m_pMotion;
		m_pMotion = NULL;
	}

	SetDeathFlag(true);
}

//====================================================================
//�X�V����
//====================================================================
void CPlayer::Update(void)
{
	switch (CScene::GetMode())
	{
	case CScene::MODE_TITLE:
		TitleUpdate();
		break;

	case CScene::MODE_GAME:
		GameUpdate();
		break;

	case CScene::MODE_RESULT:
		break;
	}
}

//====================================================================
//�^�C�g���ł̍X�V����
//====================================================================
void CPlayer::TitleUpdate(void)
{
	//���[�V�����̍X�V
	m_pMotion->Update();
}

//====================================================================
//�Q�[���ł̍X�V����
//====================================================================
void CPlayer::GameUpdate(void)
{
	//�f�o�b�O�\���̎擾
	CDebugProc* pDebugProc = CManager::GetInstance()->GetDebugProc();

	//// �ړ�����
	//Move();

	//// �����ړ�����
	//Rot();

	// �ǔ�����
	Tracking();

	// �藠������
	Shuriken();
	Evasion();	//8,9�L�[�Ńf�o�b�O

	// �J�����X�V����
	CameraPosUpdate();

	// �ʒu�X�V����
	PosUpdate();

	// �����X�V����
	RotUpdate();

	//�G��퓬��Ԃ֕ύX
	CollisionEnemy();

	//���[�V�����̊Ǘ�
	ActionState();

	//���C�t�Q�[�W�̈ʒu�Ǘ�
	if (m_pLifeGauge != nullptr)
	{
		m_pLifeGauge->SetGaugeWidth(m_fLifeMax, m_fLife);
	}

	//��Ԃ̊Ǘ�
	StateManager();

	//���[�V�����̍X�V
	m_pMotion->Update();


	CGame::GetAim()->GetPos();

	DebugKey();

	CManager::GetInstance()->GetDebugProc()->Print("[����]�ʒu %f : %f : %f\n", m_pos.x, m_pos.y, m_pos.z);
	CManager::GetInstance()->GetDebugProc()->Print("[����]���� %f : %f : %f\n",m_rot.x, m_rot.y, m_rot.z);
}

//====================================================================
//�`�揈��
//====================================================================
void CPlayer::Draw(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//���f���̕`��(�S�p�[�c)
	for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{
		m_apModel[nCntModel]->Draw();
	}
}

//====================================================================
//�ړ�����
//====================================================================
void CPlayer::Move(void)
{
	//�L�[�{�[�h�̎擾
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputJoypad* pInputJoypad = CManager::GetInstance()->GetInputJoyPad();
	D3DXVECTOR3 CameraRot = CManager::GetInstance()->GetCamera()->GetRot();

	D3DXVECTOR3 NormarizeMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//�L�[�{�[�h�̈ړ�����
	if (pInputKeyboard->GetPress(DIK_W) == true)
	{
		NormarizeMove.z += 1.0f * cosf(CameraRot.y) * PLAYER_SPEED;
		NormarizeMove.x += 1.0f * sinf(CameraRot.y) * PLAYER_SPEED;

	}
	if (pInputKeyboard->GetPress(DIK_S) == true)
	{
		NormarizeMove.z += -1.0f * cosf(CameraRot.y) * PLAYER_SPEED;
		NormarizeMove.x += -1.0f * sinf(CameraRot.y) * PLAYER_SPEED;
	}
	if (pInputKeyboard->GetPress(DIK_A) == true)
	{
		NormarizeMove.x += -1.0f * cosf(CameraRot.y) * PLAYER_SPEED;
		NormarizeMove.z -= -1.0f * sinf(CameraRot.y) * PLAYER_SPEED;

	}
	if (pInputKeyboard->GetPress(DIK_D) == true)
	{
		NormarizeMove.x += 1.0f * cosf(CameraRot.y) * PLAYER_SPEED;
		NormarizeMove.z -= 1.0f * sinf(CameraRot.y) * PLAYER_SPEED;
	}

	if (pInputKeyboard->GetPress(DIK_W) == false && pInputKeyboard->GetPress(DIK_A) == false && pInputKeyboard->GetPress(DIK_S) == false && pInputKeyboard->GetPress(DIK_D) == false)
	{
		//���X�e�B�b�N�ɂ��O��ړ�	
		m_move.z += pInputJoypad->Get_Stick_Left(0).y * cosf(CameraRot.y) * PLAYER_SPEED;
		m_move.x += pInputJoypad->Get_Stick_Left(0).y * sinf(CameraRot.y) * PLAYER_SPEED;

		//���X�e�B�b�N�ɂ�鍶�E�ړ�
		m_move.x += pInputJoypad->Get_Stick_Left(0).x * cosf(CameraRot.y) * PLAYER_SPEED;
		m_move.z -= pInputJoypad->Get_Stick_Left(0).x * sinf(CameraRot.y) * PLAYER_SPEED;
	}

	if (pInputKeyboard->GetPress(DIK_W) == true || pInputKeyboard->GetPress(DIK_A) == true || pInputKeyboard->GetPress(DIK_S) == true || pInputKeyboard->GetPress(DIK_D) == true)
	{
		float JunpPawer = NormarizeMove.y;
		NormarizeMove.y = 0.0f;

		D3DXVec3Normalize(&NormarizeMove, &NormarizeMove);

		NormarizeMove.x *= PLAYER_SPEED;
		NormarizeMove.y = JunpPawer;
		NormarizeMove.z *= PLAYER_SPEED;
	}

	m_move += NormarizeMove;

}

//====================================================================
//�ړ���������
//====================================================================
void CPlayer::Rot(void)
{
	//�L�[�{�[�h�̎擾
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputJoypad* pInputJoypad = CManager::GetInstance()->GetInputJoyPad();
	D3DXVECTOR3 CameraRot = CManager::GetInstance()->GetCamera()->GetRot();

	//�ړ������Ɍ��������킹�鏈��
	float fRotMove, fRotDest;

	fRotMove = m_rot.y;
	fRotDest = CManager::GetInstance()->GetCamera()->GetRot().y;

	if (pInputKeyboard->GetPress(DIK_I) == true)
	{
		m_HeightCameraRot += 0.01f;
		if (m_HeightCameraRot > 0.75f)
		{
			m_HeightCameraRot = 0.75f;
		}
	}
	if (pInputKeyboard->GetPress(DIK_L) == true)
	{
		m_rot.y += 0.01f;
	}
	if (pInputKeyboard->GetPress(DIK_J) == true)
	{
		m_rot.y -= 0.01f;
	}
	if (pInputKeyboard->GetPress(DIK_K) == true)
	{
		m_HeightCameraRot -= 0.01f;
		if (m_HeightCameraRot < -0.75f)
		{
			m_HeightCameraRot = -0.75f;
		}
	}

	useful::NormalizeAngle(&m_rot);
}

//====================================================================
//�ǔ�����
//====================================================================
void CPlayer::Tracking(void)
{
	// �^�[�Q�b�g�̗L���𔻒�
	if (m_pTgtEnemy != nullptr)
	{
		// �^�[�Q�b�g�̏�Ԃ��m�F
		if (m_pTgtEnemy->GetState() == CEnemy::STATE_DEATH)
		{
			// ���S���
			m_pTgtEnemy = nullptr;
		}
		else
		{
			// �ړI�������Z�o�i�G�̈ʒu�j
			m_rotDest.y = atan2f(m_pTgtEnemy->GetPos().z - m_pos.z, m_pos.x - m_pTgtEnemy->GetPos().x) + D3DX_PI * 0.5f;

			// �G�Ƃ̋����̔���i�����j
			if (useful::CollisionCircle(m_pos, m_pTgtEnemy->GetPos(), 200.0f) == false)
			{
				// �i��
				m_move.x = sinf(D3DX_PI + m_rot.y) * 10.0f;
				m_move.z = cosf(D3DX_PI + m_rot.y) * 10.0f;
			}
			// �G�Ƃ̋����̔���i�߂��j
			else if (useful::CollisionCircle(m_pos, m_pTgtEnemy->GetPos(), 150.0f) == true)
			{
				// �߂�
				m_move.x = -sinf(D3DX_PI + m_rot.y) * 10.0f;
				m_move.z = -cosf(D3DX_PI + m_rot.y) * 10.0f;
			}
		}
	}
}

//====================================================================
//���[�V�����Ə�Ԃ̊Ǘ�
//====================================================================
void CPlayer::ActionState(void)
{
	//�W�����v���[�V����
	if (m_bJump == true)
	{
		if (m_Action != ACTION_JAMP)
		{
			m_Action = ACTION_JAMP;
			m_pMotion->Set(ACTION_JAMP, 5);
		}
	}
	//�ړ����[�V����
	else if (m_move.x > 0.1f || m_move.x < -0.1f || m_move.z > 0.1f || m_move.z < -0.1f)
	{
		if (m_Action != ACTION_MOVE)
		{
			m_Action = ACTION_MOVE;
			m_pMotion->Set(ACTION_MOVE, 5);
		}
	}
	//�j���[�g�������[�V����
	else
	{
		if (m_Action != ACTION_WAIT)
		{
			m_Action = ACTION_WAIT;
			m_pMotion->Set(ACTION_WAIT, 5);
		}
	}
}

//====================================================================
//��ԊǗ�
//====================================================================
void CPlayer::StateManager(void)
{
	switch (m_State)
	{
	case STATE_NORMAL:
		break;

	case STATE_DEATH:
		break;

	case STATE_WAIT:
		break;

	case STATE_DAMAGE:
		if (m_nStateCount == 0)
		{
			m_State = STATE_NORMAL;
		}
		break;
	}

	if (m_nStateCount > 0)
	{
		m_nStateCount--;
	}
}

//====================================================================
//��ԊǗ�
//====================================================================
void CPlayer::CollisionEnemy(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		//�I�u�W�F�N�g���擾
		CObject* pObj = CObject::GetTop(nCntPriority);

		while (pObj != NULL)
		{
			CObject* pObjNext = pObj->GetNext();

			CObject::TYPE type = pObj->GetType();			//��ނ��擾

			if (type == TYPE_ENEMY3D)
			{//��ނ��u���b�N�̎�
				CEnemy* pEnemy = (CEnemy*)pObj;

				if (useful::CollisionCircle(m_pos, pEnemy->GetPos(), 200.0f) == true)
				{
					if (pEnemy->GetState() != CEnemy::STATE_DEATH)
					{
						pEnemy->SetState(CEnemy::STATE_BATTLE);
					}
				}

				// �G�^�[�Q�b�g�̃e�X�g�p�i���j
				if (useful::CollisionCircle(m_pos, pEnemy->GetPos(), 5000.0f) == true)
				{
					if (m_pTgtEnemy == nullptr && pEnemy->GetState() != CEnemy::STATE_DEATH)
					{
						m_pTgtEnemy = pEnemy;
					}
				}
			}

			pObj = pObjNext;
		}
	}
}

//====================================================================
//�藠��
//====================================================================
void CPlayer::Shuriken(void)
{
	CInputMouse* pMouse = CManager::GetInstance()->GetInputMouse();

	if (pMouse->GetTrigger(pMouse->PUSH_RIGHT))
	{
		CShuriken::Create({ m_pos.x, m_pos.y + 50.0f, m_pos.z }, 1.5f, 200.0f);
	}
}

//====================================================================
//��ԊǗ�
//====================================================================
void CPlayer::LifeGaugeCreate(void)
{
	if (m_pLifeGauge == nullptr)
	{
		m_pLifeGauge = CObjGauge2D::Create();
		m_pLifeGauge->SetPos(D3DXVECTOR3(20.0f, 20.0f, 0.0f));
		m_pLifeGauge->SetWidth(360.0f);
		m_pLifeGauge->SetHeight(10.0f);
		m_pLifeGauge->SetColor(D3DXCOLOR(0.2f, 1.0f, 0.2f, 1.0f));
		m_pLifeGauge->SetGaugeWidth(m_fLifeMax, m_fLifeMax);
	}
}

//====================================================================
//�J�����ʒu�X�V����
//====================================================================
void CPlayer::CameraPosUpdate(void)
{
	//�J�����ʒu�̍X�V
	m_CameraPos.x = m_pos.x;
	m_CameraPos.z = m_pos.z;

	if (m_bJump == false)
	{
		m_CameraPos.y = m_pos.y;
	}
}

//====================================================================
//�ʒu�X�V����
//====================================================================
void CPlayer::PosUpdate(void)
{
	//�����W��
	m_move.x = m_move.x * 0.5f;
	if (m_move.x <= 0.0001f && m_move.x >= -0.0001f)
	{
		m_move.x = 0.0f;
	}

	m_move.z = m_move.z * 0.5f;
	if (m_move.z <= 0.0001f && m_move.z >= -0.0001f)
	{
		m_move.z = 0.0f;
	}

	if (m_bJump == true)
	{
		m_Objmove.x = m_Objmove.x * 0.25f;
		if (m_Objmove.x <= 0.0001f && m_Objmove.x >= -0.0001f)
		{
			m_Objmove.x = 0.0f;
		}

		m_Objmove.z = m_Objmove.z * 0.25f;
		if (m_Objmove.z <= 0.0001f && m_Objmove.z >= -0.0001f)
		{
			m_Objmove.z = 0.0f;
		}
	}

	//Y���̈ʒu�X�V
	m_pos.y += m_move.y * CManager::GetInstance()->GetGameSpeed();
	m_pos.y += m_Objmove.y * CManager::GetInstance()->GetGameSpeed();

	//X���̈ʒu�X�V
	m_pos.x += m_move.x * CManager::GetInstance()->GetGameSpeed();
	m_pos.x += m_Objmove.x * CManager::GetInstance()->GetGameSpeed();

	//Z���̈ʒu�X�V
	m_pos.z += m_move.z * CManager::GetInstance()->GetGameSpeed();
	m_pos.z += m_Objmove.z * CManager::GetInstance()->GetGameSpeed();
}

//====================================================================
//�����X�V����
//====================================================================
void CPlayer::RotUpdate(void)
{
	// �p�x�̍������Z�o
	D3DXVECTOR3 rotDiff = m_rotDest - m_rot;	

	// ���K��
	useful::NormalizeAngle(&rotDiff);			

	// �����̍X�V����
	m_rot += (rotDiff * 0.05f);
}

//====================================================================
//�_���[�W����
//====================================================================
void CPlayer::HitDamage(float Damage)
{
	if (m_State == STATE_NORMAL)
	{
		m_fLife -= Damage;
		if (m_fLife < 0.0f)
		{
			m_fLife = 0.0f;

		}
		else
		{
			m_pLifeGauge->SetGaugeWidth(m_fLifeMax, m_fLife);
			m_State = STATE_DAMAGE;
			m_nStateCount = 5;
		}
	}
}

//====================================================================
// �f�o�b�O�{�^��
//====================================================================
void CPlayer::DebugKey(void)
{
#ifdef _DEBUG

	CInputMouse* pMouse = CManager::GetInstance()->GetInputMouse();

	if (pMouse->GetTrigger(pMouse->PUSH_LEFT))
	{
		HitDamage(10.0f);
	}

#endif // !_DEBUG
}


//====================================================================
//���[�h����
//====================================================================
void CPlayer::LoadLevelData(const char* pFilename)
{
	FILE* pFile; //�t�@�C���|�C���^��錾

	//�t�@�C�����J��
	pFile = fopen(pFilename, "r");

	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ

		int ModelParent = 0;
		D3DXVECTOR3 ModelPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 ModelRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		char ModelName[128] = {};
		int nCntModel = 0;
		int nCntParts = 0;
		int nCntMotion = 0;
		int nCntKeySet = 0;
		int nCntKey = 0;

		char aString[128] = {};				//�S�~��
		char aMessage[128] = {};			//�X�^�[�g�ƃG���h�̃��b�Z�[�W
		char aBool[128] = {};				//bool�ϊ��p���b�Z�[�W

		// �ǂݍ��݊J�n-----------------------------------------------------
		while (1)
		{//�uSCRIPT�v��T��
			fscanf(pFile, "%s", &aMessage[0]);
			if (strcmp(&aMessage[0], "SCRIPT") == 0)
			{
				// ���f�����ǂݍ���-----------------------------------------------------
				while (1)
				{//�uNUM_MODEL�v��T��
					fscanf(pFile, "%s", &aMessage[0]);
					if (strcmp(&aMessage[0], "NUM_MODEL") == 0)
					{
						fscanf(pFile, "%s", &aString[0]);
						fscanf(pFile, "%d", &m_nNumModel);		//���f�����̐ݒ�
						break;
					}
				}

				//���f���t�@�C���̓ǂݍ���
				while (1)
				{//�uMODEL_FILENAME�v��T��
					fscanf(pFile, "%s", &aMessage[0]);
					if (strcmp(&aMessage[0], "MODEL_FILENAME") == 0)
					{
						fscanf(pFile, "%s", &aString[0]);
						fscanf(pFile, "%s", &ModelName[0]);		//�ǂݍ��ރ��f���̃p�X���擾

						m_apModel[nCntModel] = CModel::Create(&ModelName[0]);
						nCntModel++;
					}
					if (nCntModel >= m_nNumModel)
					{
						nCntModel = 0;
						break;
					}
				}

				// �L�����N�^�[���ǂݍ���-----------------------------------------------------
				while (1)
				{//�uPARTSSET�v��T��
					fscanf(pFile, "%s", &aMessage[0]);
					if (strcmp(&aMessage[0], "PARTSSET") == 0)
					{
						while (1)
						{//�e��ϐ���T��
							fscanf(pFile, "%s", &aMessage[0]);
							if (strcmp(&aMessage[0], "INDEX") == 0)
							{
								fscanf(pFile, "%s", &aString[0]);
								fscanf(pFile, "%d", &nCntModel);	//�C���f�b�N�X��ݒ�
							}
							if (strcmp(&aMessage[0], "PARENT") == 0)
							{
								fscanf(pFile, "%s", &aString[0]);
								fscanf(pFile, "%d", &ModelParent);	//�e���f���̃C���f�b�N�X��ݒ�

								if (ModelParent == -1)
								{
									m_apModel[nCntModel]->SetParent(NULL);
								}
								else
								{
									m_apModel[nCntModel]->SetParent(m_apModel[ModelParent]);
								}
							}
							if (strcmp(&aMessage[0], "POS") == 0)
							{
								fscanf(pFile, "%s", &aString[0]);
								fscanf(pFile, "%f", &ModelPos.x);				//�ʒu(�I�t�Z�b�g)�̏����ݒ�
								fscanf(pFile, "%f", &ModelPos.y);				//�ʒu(�I�t�Z�b�g)�̏����ݒ�
								fscanf(pFile, "%f", &ModelPos.z);				//�ʒu(�I�t�Z�b�g)�̏����ݒ�

								m_apModel[nCntModel]->SetPos(ModelPos);
								m_apModel[nCntModel]->SetStartPos(ModelPos);
							}
							if (strcmp(&aMessage[0], "ROT") == 0)
							{
								fscanf(pFile, "%s", &aString[0]);
								fscanf(pFile, "%f", &ModelRot.x);				////�����̏����ݒ�
								fscanf(pFile, "%f", &ModelRot.y);				////�����̏����ݒ�
								fscanf(pFile, "%f", &ModelRot.z);				////�����̏����ݒ�

								m_apModel[nCntModel]->SetRot(ModelRot);
								m_apModel[nCntModel]->SetStartRot(ModelRot);
							}
							if (strcmp(&aMessage[0], "END_PARTSSET") == 0)
							{
								break;
							}
						}
						nCntModel++;
						if (nCntModel >= m_nNumModel)
						{
							break;
						}
					}
				}
			}
			if (strcmp(&aMessage[0], "END_SCRIPT") == 0)
			{
				break;
			}
		}
		fclose(pFile);
	}
	else
	{//�t�@�C�����J���Ȃ������ꍇ
		printf("***�t�@�C�����J���܂���ł���***\n");
	}
}

//====================================================================
//���C�t�Q�[�W����
// ���
//====================================================================
void CPlayer::Evasion()
{
	D3DXVECTOR3 CameraRot = CManager::GetInstance()->GetCamera()->GetRot();
	CInputKeyboard* pKeyboard = CManager::GetInstance()->GetInputKeyboard();
	D3DXVECTOR3 move = INITVECTOR3;

	//���E����
	if (m_nEvasionCount <= 0)
	{
		if (pKeyboard->GetTrigger(DIK_8))
		{
			m_nEvasionLR = -1;
			m_nEvasionCount = EVADION_FRAME;
		}
		else if (pKeyboard->GetTrigger(DIK_9))
		{
			m_nEvasionLR = 1;
			m_nEvasionCount = EVADION_FRAME;
		}
		else
		{
			m_nEvasionLR = 0;
		}
	}

	if (m_nEvasionCount > 0)
	{
		float fPi = D3DX_PI * static_cast<float>(m_nEvasionLR) * 0.25f;
		move.x = sinf(CameraRot.y + fPi) * EVADION_MOVE;
		move.z = cosf(CameraRot.y + fPi) * EVADION_MOVE;
		m_nEvasionCount--;
	}

	//�}�Ȉړ���
	m_pos += move;
}
