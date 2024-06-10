//============================================
//
//	�K�w�\���Ń��[�V�������s�����f���̃T���v�� [SampleLevelModel.cpp]
//	Author:sakamoto kai
//
//============================================
#include "SampleLevelModel.h"
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

#define COLLISION_SIZE (D3DXVECTOR3(20.0f,40.0f,20.0f))		//���̓����蔻��

//====================================================================
//�R���X�g���N�^
//====================================================================
CSampleLvModel::CSampleLvModel(int nPriority) :CObject(nPriority)
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
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CSampleLvModel::~CSampleLvModel()
{

}

//====================================================================
//��������
//====================================================================
CSampleLvModel* CSampleLvModel::Create()
{
	CSampleLvModel* pPlayer = NULL;

	if (pPlayer == NULL)
	{
		//�v���C���[�̐���
		pPlayer = new CSampleLvModel();
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
HRESULT CSampleLvModel::Init(void)
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

	//��ސݒ�
	SetType(CObject::TYPE_SAMPLE);

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
void CSampleLvModel::MyObjCreate(void)
{

}

//====================================================================
//�I������
//====================================================================
void CSampleLvModel::Uninit(void)
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
void CSampleLvModel::Update(void)
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
void CSampleLvModel::TitleUpdate(void)
{
	//���[�V�����̍X�V
	m_pMotion->Update();
}

//====================================================================
//�Q�[���ł̍X�V����
//====================================================================
void CSampleLvModel::GameUpdate(void)
{
	//�f�o�b�O�\���̎擾
	CDebugProc* pDebugProc = CManager::GetInstance()->GetDebugProc();

	//D3DXVECTOR3 ScreenCenter = D3DXVECTOR3(640.0f,360.0f,0.0f);
	//D3DXVECTOR3 ScreenPos[4] = { INITVECTOR3 };
	//D3DXVECTOR3 pos[4] = { INITVECTOR3 };

	//pos[0] = D3DXVECTOR3(GetPos().x - COLLISION_SIZE.x, GetPos().y, GetPos().z);
	//pos[1] = D3DXVECTOR3(GetPos().x - COLLISION_SIZE.x, GetPos().y + COLLISION_SIZE.y, GetPos().z);
	//pos[2] = D3DXVECTOR3(GetPos().x + COLLISION_SIZE.x, GetPos().y, GetPos().z);
	//pos[3] = D3DXVECTOR3(GetPos().x + COLLISION_SIZE.x, GetPos().y + COLLISION_SIZE.y, GetPos().z);

	////�r���[�|�[�g�̐ݒ�
	//D3DVIEWPORT9 viewport;
	//viewport.X = 0;
	//viewport.Y = 0;
	//viewport.Width = SCREEN_WIDTH;
	//viewport.Height = SCREEN_HEIGHT;
	//viewport.MaxZ = 0.0f;
	//viewport.MinZ = 0.0f;

	////�}�g���b�N�X�̎擾
	//D3DXMATRIX ViewMatrix = CManager::GetInstance()->GetCamera()->GetViewMatrix();
	//D3DXMATRIX ProjectionMatrix = CManager::GetInstance()->GetCamera()->GetProjectionMatrix();

	////D3DXVec3Project(&screenPosition, &pos, &viewport, &projectionMatrix, &viewMatrix, D3DXMatrixIdentity(&mtx));

	////3D�̍��W���X�N���[�����W�ɕϊ�
	//for (int nCnt = 0; nCnt < 4; nCnt++)
	//{
	//	D3DXVec3Project(&ScreenPos[nCnt], &pos[nCnt], &viewport, &ProjectionMatrix, &ViewMatrix, D3DXMatrixIdentity(&m_mtxWorld));

	//	C2DEffect* pEffect = C2DEffect::Create();
	//	pEffect->SetRadius(50.0f);
	//	pEffect->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	//	pEffect->SetPos(ScreenPos[nCnt]);
	//}

	////�p�x�ύX
	//CInputKeyboard* pKey = CManager::GetInstance()->GetInputKeyboard();

	//if (pKey->GetPress(DIK_RIGHT) == true)
	//{
	//	m_CollisionRot += 0.01f;
	//}

	//if (pKey->GetPress(DIK_LEFT) == true)
	//{
	//	m_CollisionRot -= 0.01f;
	//}

	////�f�o�b�O�p�̃G�t�F�N�g����
	//for (int nCnt = 0; nCnt < 10; nCnt++)
	//{
	//	C2DEffect* pEffect = C2DEffect::Create();
	//	pEffect->SetRadius(30.0f);
	//	pEffect->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	//	pEffect->SetPos(D3DXVECTOR3(
	//		ScreenCenter.x + sinf(m_CollisionRot) * 50.0f * nCnt,
	//		ScreenCenter.y + cosf(m_CollisionRot) * 50.0f * nCnt,
	//		0.0f));

	//	pEffect = C2DEffect::Create();
	//	pEffect->SetRadius(30.0f);
	//	pEffect->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	//	pEffect->SetPos(D3DXVECTOR3(
	//		ScreenCenter.x + sinf(m_CollisionRot) * -50.0f * nCnt,
	//		ScreenCenter.y + cosf(m_CollisionRot) * -50.0f * nCnt,
	//		0.0f));
	//}

	////�v���C���[�ƓG�̈ʒu�֌W�������O��
	//int Left = 0;
	//int	Right = 0;
	//for (int nCnt = 0; nCnt < 4; nCnt++)
	//{
	//	D3DXVECTOR3 vecToPos;
	//	D3DXVECTOR3 vecLine;
	//	float A;

	//	vecToPos.x = (ScreenPos[nCnt].x) - (ScreenCenter.x);
	//	vecToPos.y = (ScreenPos[nCnt].y) - (ScreenCenter.y);

	//	vecLine.x = sinf(m_CollisionRot);
	//	vecLine.y = cosf(m_CollisionRot);
	//	vecLine.z = 0.0f;

	//	A = (vecLine.y * vecToPos.x) - (vecLine.x * vecToPos.y);

	//	if (A < 0)
	//	{
	//		Right++;
	//	}
	//	else
	//	{
	//		Left++;
	//	}
	//}

	//pDebugProc->Print("�� : �E �� %d : %d\n", Left, Right);



	//���[�V�����̊Ǘ�
	ActionState();

	//��Ԃ̊Ǘ�
	StateManager();

	//���[�V�����̍X�V
	m_pMotion->Update();
}

//====================================================================
//���[�V�����Ə�Ԃ̊Ǘ�
//====================================================================
void CSampleLvModel::ActionState(void)
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
void CSampleLvModel::StateManager(void)
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
		break;
	}

	if (m_nStateCount > 0)
	{
		m_nStateCount--;
	}
}

//====================================================================
//�`�揈��
//====================================================================
void CSampleLvModel::Draw(void)
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
//���[�h����
//====================================================================
void CSampleLvModel::LoadLevelData(const char* pFilename)
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