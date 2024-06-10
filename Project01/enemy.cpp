//============================================
//
//	�G�̏��� [enemy.cpp]
//	Author:sakamoto kai
//
//============================================
#include "enemy.h"
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
#include "player.h"
#include "useful.h"

#define COLLISION_SIZE (D3DXVECTOR3(10.0f,40.0f,10.0f))		//���̓����蔻��
#define LIFE (30.0f)	// �̗�

//====================================================================
//�R���X�g���N�^
//====================================================================
CEnemy::CEnemy(int nPriority) :CObject(nPriority)
{
	SetSize(COLLISION_SIZE);
	m_pos = INITVECTOR3;
	m_move = INITVECTOR3;
	m_Objmove = INITVECTOR3;
	m_rot = D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f);
	m_bJump = false;
	m_nActionCount = 0;
	m_Action = ACTION_WAIT;
	m_AtkAction = ACTION_WAIT;
	m_nAttackHit = false;
	m_BState = BATTLESTATE_WALK;
	m_nStateCount = 0;
	m_AtkPos = INITVECTOR3;
	m_CollisionRot = 0.0f;
	m_nSeatchCount = 0;
	m_bSeatchLoop = false;
	m_State = STATE_SEATCH;
	m_fLife = LIFE;

	for (int nCnt = 0; nCnt < 32; nCnt++)
	{
		m_SeatchPos[nCnt] = D3DXVECTOR3(0.0f, -10000.0f, 0.0f);
	}

	for (int nCnt = 0; nCnt < 8; nCnt++)
	{
		m_ScreenPos[nCnt] = D3DXVECTOR3(-100.0f, -100.0f, 0.0f);
	}
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CEnemy::~CEnemy()
{

}

//====================================================================
//��������
//====================================================================
CEnemy* CEnemy::Create()
{
	CEnemy* pPlayer = NULL;

	if (pPlayer == NULL)
	{
		//�v���C���[�̐���
		pPlayer = new CEnemy();
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
HRESULT CEnemy::Init(void)
{
	//���f���̐���
	LoadLevelData("data\\TXT\\motion_ManBase.txt");

	//���[�V�����̐���
	if (m_pMotion == NULL)
	{
		//���[�V�����̐���
		m_pMotion = new CMotion;
	}

	//����������
	m_pMotion->SetModel(&m_apModel[0], m_nNumModel);
	m_pMotion->LoadData("data\\TXT\\motion_ManBase.txt");

	//��ސݒ�
	SetType(CObject::TYPE_ENEMY3D);

	switch (CScene::GetMode())
	{
	case CScene::MODE_TITLE:
		m_pMotion->Set(ACTION_WAIT, 0);
		break;

	case CScene::MODE_GAME:
		m_SState = SEARCHSTATE_WALK;
		break;

	case CScene::MODE_RESULT:
		break;
	}

	return S_OK;
}

//====================================================================
//�������ێ�����I�u�W�F�N�g�̐���
//====================================================================
void CEnemy::MyObjCreate(void)
{

}

//====================================================================
//�I������
//====================================================================
void CEnemy::Uninit(void)
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
void CEnemy::Update(void)
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
void CEnemy::TitleUpdate(void)
{
	//���[�V�����̍X�V
	m_pMotion->Update();
}

//====================================================================
//�Q�[���ł̍X�V����
//====================================================================
void CEnemy::GameUpdate(void)
{
	//�f�o�b�O�\���̎擾
	CDebugProc* pDebugProc = CManager::GetInstance()->GetDebugProc();

	////�p�x�ύX
	//CInputKeyboard* pKey = CManager::GetInstance()->GetInputKeyboard();

	//�X�N���[�����W�ւ̕ϊ�����
	ScreenCollision();
//
//#ifdef _DEBUG
//
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
	//		SCREEM_CENTER.x + sinf(m_CollisionRot) * 50.0f * nCnt,
	//		SCREEM_CENTER.y + cosf(m_CollisionRot) * 50.0f * nCnt,
	//		0.0f));

	//	pEffect = C2DEffect::Create();
	//	pEffect->SetRadius(30.0f);
	//	pEffect->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	//	pEffect->SetPos(D3DXVECTOR3(
	//		SCREEM_CENTER.x + sinf(m_CollisionRot) * -50.0f * nCnt,
	//		SCREEM_CENTER.y + cosf(m_CollisionRot) * -50.0f * nCnt,
	//		0.0f));
	//}
//
//	//�f�o�b�O�p�G�t�F�N�g
//	for (int nCnt = 0; nCnt < 8; nCnt++)
//	{
//		if (useful::CameraFront(GetPos()) == true)
//		{
//			C2DEffect* pEffect = C2DEffect::Create();
//			pEffect->SetRadius(50.0f);
//			pEffect->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
//			pEffect->SetPos(m_ScreenPos[nCnt]);
//		}
//	}
//
//#endif
//
//	//�v���C���[�ƓG�̈ʒu�֌W�������O��
//	int Left = 0;
//	int	Right = 0;
//	for (int nCnt = 0; nCnt < 8; nCnt++)
//	{
//		D3DXVECTOR3 vecToPos;
//		D3DXVECTOR3 vecLine;
//		float A;
//
//		vecToPos.x = (m_ScreenPos[nCnt].x) - (SCREEM_CENTER.x);
//		vecToPos.y = (m_ScreenPos[nCnt].y) - (SCREEM_CENTER.y);
//
//		vecLine.x = sinf(m_CollisionRot);
//		vecLine.y = cosf(m_CollisionRot);
//		vecLine.z = 0.0f;
//
//		A = (vecLine.y * vecToPos.x) - (vecLine.x * vecToPos.y);
//
//		if (A < 0)
//		{
//			Right++;
//		}
//		else
//		{
//			Left++;
//		}
//	}
//
//	pDebugProc->Print("�� : �E �� %d : %d\n", Left, Right);

	//if (useful::SlashCollision(&m_ScreenPos[0], 8, m_CollisionRot) == true)
	//{
	//	pDebugProc->Print("TRUE\n");
	//}
	//else
	//{
	//	pDebugProc->Print("FALSE\n");
	//}

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

	//���[�V�����̊Ǘ�
	ActionState();

	//��Ԃ̊Ǘ�
	switch (m_State)
	{
	case CEnemy::STATE_BATTLE:

		//�퓬��Ԃ̊Ǘ�
		BattleStateManager();

		break;

	case CEnemy::STATE_SEATCH:

		//�T����Ԃ̊Ǘ�
		SeatchStateManager();
		
		break;

	case CEnemy::STATE_DEATH:

		// ���S����
		Death();

		break;

	default:
		break;
	}

	if (m_pMotion != nullptr)
	{
		//���[�V�����̍X�V
		m_pMotion->Update();
	}
}

//====================================================================
//���[�V�����Ə�Ԃ̊Ǘ�
//====================================================================
void CEnemy::ActionState(void)
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
//�퓬��ԊǗ�
//====================================================================
void CEnemy::BattleStateManager(void)
{
	CPlayer* pPlayer = CGame::GetPlayer();

	switch (m_BState)
	{
	case BATTLESTATE_NORMAL:
		break;

	case BATTLESTATE_DEATH:

		break;

	case BATTLESTATE_WAIT:
		break;

	case BATTLESTATE_DAMAGE:
		break;

	case BATTLESTATE_WALK:

		m_rot.y = atan2f(pPlayer->GetPos().z - m_pos.z, m_pos.x - pPlayer->GetPos().x) + D3DX_PI * 0.5f;

		if (useful::CollisionCircle(m_pos, pPlayer->GetPos(), 200.0f) == false)
		{
			m_move.x = sinf(D3DX_PI + m_rot.y) * 3.0f;
			m_move.z = cosf(D3DX_PI + m_rot.y) * 3.0f;
		}
		else if (useful::CollisionCircle(m_pos, pPlayer->GetPos(), 150.0f) == true)
		{
			m_move.x = -sinf(D3DX_PI + m_rot.y) * 3.0f;
			m_move.z = -cosf(D3DX_PI + m_rot.y) * 3.0f;
		}

		CollisionPush();

		break;
	}

	if (m_nStateCount > 0)
	{
		m_nStateCount--;
	}
}

//====================================================================
//�T����ԊǗ�
//====================================================================
void CEnemy::SeatchStateManager(void)
{
	float fAngle = 0.0f;

	switch (m_SState)
	{
	case SEARCHSTATE_NORMAL:
		break;

	case SEARCHSTATE_WALK:

		m_move.x = sinf(D3DX_PI + m_rot.y) * 3.0f;
		m_move.y = 0.0f;
		m_move.z = cosf(D3DX_PI + m_rot.y) * 3.0f;

		if (m_SeatchPos[m_nSeatchCount + 1].y <= -10000.0f)
		{
			if (useful::CollisionCircle(m_pos, m_SeatchPos[0], 5.0f) == true)
			{
				m_SState = SEARCHSTATE_TRUN;
				m_nSeatchCount = 0;
			}
		}
		else
		{
			if (useful::CollisionCircle(m_pos, m_SeatchPos[m_nSeatchCount + 1], 5.0f) == true)
			{
				m_SState = SEARCHSTATE_TRUN;
				m_nSeatchCount += 1;
			}
		}

		CManager::GetInstance()->GetDebugProc()->Print("[�G] %f : %f : %f\n", m_pos.x, m_pos.y, m_pos.z);

		break;

	case SEARCHSTATE_TRUN:

		if (m_SeatchPos[m_nSeatchCount + 1].y <= -10000.0f)
		{
			if (m_bSeatchLoop == true)
			{
				fAngle = atan2f(m_SeatchPos[0].z - m_pos.z, m_pos.x - m_SeatchPos[0].x);
				m_rot.y = fAngle + D3DX_PI * 0.5f;
				m_SState = SEARCHSTATE_WALK;
			}
			else
			{
				m_SState = SEARCHSTATE_WAIT;
			}

		}
		else
		{
			fAngle = atan2f(m_SeatchPos[m_nSeatchCount + 1].z - m_pos.z, m_pos.x - m_SeatchPos[m_nSeatchCount + 1].x);
			m_rot.y = fAngle + D3DX_PI * 0.5f;
			m_SState = SEARCHSTATE_WALK;
		}

		break;

	case SEARCHSTATE_WAIT:
		break;

	case SEARCHSTATE_SEARCH:
		break;
	}
}

//====================================================================
//�����ȊO�̓G�������o������
//====================================================================
void CEnemy::CollisionPush(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		//�I�u�W�F�N�g���擾
		CObject* pObj = CObject::GetTop(nCntPriority);

		while (pObj != NULL)
		{
			CObject* pObjNext = pObj->GetNext();

			CObject::TYPE type = pObj->GetType();			//��ނ��擾
			float fAngle = 0.0f;

			if (type == TYPE_ENEMY3D && pObj != this)
			{//��ނ��u���b�N�̎�
				CEnemy* pEnemy = (CEnemy*)pObj;

				if (useful::CollisionCircle(m_pos, pEnemy->GetPos(), 30.0f) == true)
				{
					fAngle = atan2f(pEnemy->GetPos().z - m_pos.z, m_pos.x - pEnemy->GetPos().x) + D3DX_PI * 0.5f;
					m_move.x = sinf(fAngle) * 0.2f;
					m_move.z = cosf(fAngle) * 0.2f;
				}
			}

			pObj = pObjNext;
		}
	}
}

//====================================================================
//�`�揈��
//====================================================================
void CEnemy::Draw(void)
{
	//LPDIRECT3DSURFACE9 pRenderDef, pZBuffDef;
	//D3DVIEWPORT9 viewportDef;
	//D3DXMATRIX mtxViewDef, mtxProjectionDef;

	////�J�����̎擾
	//CCamera* pCamera = CManager::GetInstance()->GetCamera();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	////���݂̃����_�����O�^�[�Q�b�g���擾(�ۑ�)
	//pDevice->GetRenderTarget(0, &pRenderDef);

	////���݂�Z�o�b�t�@���擾(�ۑ�)
	//pDevice->GetDepthStencilSurface(&pZBuffDef);

	////���݂̃r���[�|�[�g���擾(�ۑ�)
	//pDevice->GetViewport(&viewportDef);

	//CManager::GetInstance()->GetRenderer()->ChageTarget(pCamera->GetPosV(), pCamera->GetPosR(), pCamera->GetVecU());

	////�����_�����O�^�[�Q�b�g�p�̃e�N�X�`���̃N���A
	//pDevice->Clear(0, NULL,
	//	(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
	//	D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	////-----------------------------------------------------------------------------------

	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X

	////���[���h�}�g���b�N�X�̏�����
	//D3DXMatrixIdentity(&m_mtxWorld);

	////�����𔽉f
	//D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);

	//D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	////�ʒu�𔽉f
	//D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);

	//D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	////���[���h�}�g���b�N�X�̐ݒ�
	//pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	////���f���̕`��(�S�p�[�c)
	//for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	//{
	//	m_apModel[nCntModel]->Draw();
	//}

	////-----------------------------------------------------------------------------------

	////���̃����_�����O�^�[�Q�b�g�ɖ߂�
	//pDevice->SetRenderTarget(0, pRenderDef);

	////����Z�o�b�t�@�ɖ߂�
	//pDevice->SetDepthStencilSurface(pZBuffDef);

	////���̃r���[�|�[�g�ɖ߂�
	//pDevice->SetViewport(&viewportDef);

	////-----------------------------------------------------------------------------------

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
//�X�N���[�����W�ɕϊ����鏈��
//====================================================================
void CEnemy::ScreenCollision()
{
	//���_�ݒ�
	D3DXVECTOR3 pos[8] = { INITVECTOR3 };

	pos[0] = D3DXVECTOR3(GetPos().x - COLLISION_SIZE.x, GetPos().y, GetPos().z + COLLISION_SIZE.z);
	pos[1] = D3DXVECTOR3(GetPos().x - COLLISION_SIZE.x, GetPos().y + COLLISION_SIZE.y, GetPos().z + COLLISION_SIZE.z);
	pos[2] = D3DXVECTOR3(GetPos().x + COLLISION_SIZE.x, GetPos().y, GetPos().z + COLLISION_SIZE.z);
	pos[3] = D3DXVECTOR3(GetPos().x + COLLISION_SIZE.x, GetPos().y + COLLISION_SIZE.y, GetPos().z + COLLISION_SIZE.z);
	pos[4] = D3DXVECTOR3(GetPos().x - COLLISION_SIZE.x, GetPos().y, GetPos().z - COLLISION_SIZE.z);
	pos[5] = D3DXVECTOR3(GetPos().x - COLLISION_SIZE.x, GetPos().y + COLLISION_SIZE.y, GetPos().z - COLLISION_SIZE.z);
	pos[6] = D3DXVECTOR3(GetPos().x + COLLISION_SIZE.x, GetPos().y, GetPos().z - COLLISION_SIZE.z);
	pos[7] = D3DXVECTOR3(GetPos().x + COLLISION_SIZE.x, GetPos().y + COLLISION_SIZE.y, GetPos().z - COLLISION_SIZE.z);

	//�r���[�|�[�g�̐ݒ�
	D3DVIEWPORT9 viewport;
	viewport.X = 0;
	viewport.Y = 0;
	viewport.Width = SCREEN_WIDTH;
	viewport.Height = SCREEN_HEIGHT;
	viewport.MaxZ = 0.0f;
	viewport.MinZ = 0.0f;

	//�}�g���b�N�X�̎擾
	D3DXMATRIX ViewMatrix = CManager::GetInstance()->GetCamera()->GetViewMatrix();
	D3DXMATRIX ProjectionMatrix = CManager::GetInstance()->GetCamera()->GetProjectionMatrix();

	//D3DXVec3Project(&screenPosition, &pos, &viewport, &projectionMatrix, &viewMatrix, D3DXMatrixIdentity(&mtx));

	//3D�̍��W���X�N���[�����W�ɕϊ�
	for (int nCnt = 0; nCnt < 8; nCnt++)
	{
		D3DXVec3Project(&m_ScreenPos[nCnt], &pos[nCnt], &viewport, &ProjectionMatrix, &ViewMatrix, D3DXMatrixIdentity(&m_mtxWorld));
	}
}

//====================================================================
//���[�h����
//====================================================================
void CEnemy::LoadLevelData(const char* pFilename)
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
// �ڐG�_���[�W����
//====================================================================
void CEnemy::HitDamage(float fDamage)
{
	m_fLife -= fDamage;

	if (m_fLife <= 0)
	{
		// ��Ԑݒ�
		m_State = STATE_DEATH;
	}
}

//====================================================================
// ���S����
//====================================================================
void CEnemy::Death(void)
{
	// �I������
	Uninit();
}