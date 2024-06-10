//============================================
//
//	�Q�[����� [game.cpp]
//	Author:sakamoto kai
//
//============================================
#include "game.h"
#include "manager.h"
#include "fade.h"
#include "input.h"
#include "object.h"
#include "object2D.h"
#include "object3D.h"
#include "objectBillboard.h"
#include "objectX.h"
#include "objmeshField.h"
#include "objmeshWall.h"
#include "objmeshCylinder.h"
#include "objmeshDome.h"
#include "CubeBlock.h"
#include "CubeDamage.h"
#include "player.h"
#include "boss.h"
#include "Pause.h"
#include "camera.h"
#include "game.h"
#include "sound.h"
#include "time.h"
#include "debugproc.h"
#include "Xmodel.h"
#include "texture.h"
#include "numberBillboard.h"
#include "numberFall.h"
#include "enemy.h"
#include "effect.h"
#include "Edit.h"
#include "log.h"
#include "objGauge2D.h"
#include "skinmesh.h"
#include "renderer.h"
#include "SampleObj2D.h"
#include "SampleObj3D.h"
#include "SampleObjBillboard.h"
#include "SampleObjX.h"
#include "SampleLevelModel.h"

#include "Shuriken.h"
#include "aim.h"
#include "slash.h"
#include "MapModel.h"

#include "gameMode.h"

//�ÓI�����o�ϐ��錾
CTutorialUI *CGame::m_pTutorialUI = nullptr;
CEdit *CGame::m_pEdit = nullptr;
CPause *CGame::m_pPause = nullptr;
CScore *CGame::m_pScore = nullptr;
CTime *CGame::m_pTime = nullptr;
CObject2D* CGame::m_p2DSample = nullptr;
CObject2D* CGame::m_pEventBG[2] = {};
CObject2D *CGame::m_p2DUI_Attack = nullptr;		//�U����2DUI
CObject2D *CGame::m_p2DUI_Jump = nullptr;			//�W�����v��2DUI
CObject2D *CGame::m_p2DUI_Dodge = nullptr;			//�����2DUI
CObject2D* CGame::m_p2DUI_Attention = nullptr;		//���ڂ�2DUI
CObject2D* CGame::m_p2DUI_AttentionOK = nullptr;	//���ڂ�2DUI
CObject2D *CGame::m_p2DUITime = nullptr;	//���ڂ�2DUI
CObject3D* CGame::m_p3DSample = nullptr;
CObject3D* CGame::m_p3DEventBG = nullptr;			//�C�x���g����3D�w�i
CObject2D* CGame::m_p2DBossName = nullptr;
CObject2D* CGame::m_pPlayerName = nullptr;
CObject2D *CGame::m_pBossName = nullptr;
CObjectBillboard* CGame::m_pBillboardSample = nullptr;
CObjectX* CGame::m_pXModelSample = nullptr;
CObjmeshField* CGame::m_pMeshFieldSample = nullptr;
CObjmeshWall* CGame::m_pMeshWallSample = nullptr;
CObjmeshCylinder* CGame::m_pMeshCylinderSample = nullptr;
CObjmeshDome* CGame::m_pMeshDomeUp = nullptr;
CObjmeshField* CGame::m_pMeshField = nullptr;
CCubeBlock* CGame::m_pCubeBlock = nullptr;
CPlayer* CGame::m_pPlayer = nullptr;
CBoss*CGame::m_pBoss = nullptr;
bool CGame::m_bGameEnd = false;
bool CGame::m_bEvent = false;
bool CGame::m_bEventEnd = false;
bool CGame::m_Wireframe = false;
bool CGame::m_Slow = false;
int CGame::m_nEnemyNum = 0;
int CGame::m_nTutorialWave = 0;
int CGame::m_nEventCount = 0;
float CGame::m_EventHeight = 0.0f;
float CGame::m_NameColorA = 0.0f;
float CGame::m_BGColorA = 1.0f;
D3DXVECTOR3 CGame::m_EventPos = D3DXVECTOR3(0.0f, 300.0f, 0.0f);
D3DXVECTOR3 CGame::m_BGRot = INITVECTOR3;

CAim* CGame::m_pAim = nullptr;

//====================================================================
//�R���X�g���N�^
//====================================================================
CGame::CGame()
{
	m_nEnemyNum = 0;
	m_bGameEnd = false;
	m_bEvent = false;
	m_bEventEnd = false;
	m_EventHeight = 0.0f;
	m_nEventCount = 0;
	m_BGColorA = 1.0f;
	m_nTutorialWave = 0;
	CManager::GetInstance()->GetCamera()->SetBib(false);

#ifdef _DEBUG
	CManager::GetInstance()->GetCamera()->SetCameraMode(CCamera::CAMERAMODE_FPS);
	//CManager::GetInstance()->GetCamera()->SetCameraMode(CCamera::CAMERAMODE_CONTROL);
#else
	CManager::GetInstance()->GetCamera()->SetCameraMode(CCamera::CAMERAMODE_FPS);
#endif // DEBUG
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CGame::~CGame()
{

}

//====================================================================
//����������
//====================================================================
HRESULT CGame::Init(void)
{
	CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_BGM_TUTORIAL);
	CGame::GetTime()->SetStopTime(false);
	CCubeDamage::StaticReset();

	m_pMeshDomeUp = CObjmeshDome::Create();
	m_pMeshDomeUp->SetTexture("data\\TEXTURE\\rain_clown.jpg");

	m_pMeshField = CObjmeshField::Create();
	m_pMeshField->SetPos(INITVECTOR3);

	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		m_pEventBG[nCnt] = CObject2D::Create();
		m_pEventBG[nCnt]->SetPos(D3DXVECTOR3(640.0f, nCnt * 720.0f, 0.0f));
		m_pEventBG[nCnt]->SetWidth(1280.0f);
		m_pEventBG[nCnt]->SetHeight(0.0f);
		m_pEventBG[nCnt]->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
		m_pEventBG[nCnt]->SetTexture("data\\TEXTURE\\Test.jpg");
	}

	//�|�[�Y�̐���
	if (m_pPause == nullptr)
	{
		m_pPause = CPause::Create();
	}

	m_bGameEnd = false;
	CManager::GetInstance()->GetInstance()->SetStop(false);

	////�e��I�u�W�F�N�g�̐���------------------------------------
	//CObject2D* pCbject2D = CObject2D::Create();
	//pCbject2D->SetPos(D3DXVECTOR3(640.0, 360.0f, 0.0f));

	//CObject3D *pCbject3D = CObject3D::Create();
	//pCbject3D->SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//CObjectBillboard* Billboard = CObjectBillboard::Create();
	//Billboard->SetPos(D3DXVECTOR3(100.0f, 0.0f, 0.0f));

	//CObjectX* pObjectX = CObjectX::Create("data\\MODEL\\player00.x");
	//pObjectX->SetPos(D3DXVECTOR3(200.0f, 0.0f, 0.0f));

	//CObjmeshCube* pObjCube = CObjmeshCube::Create();
	//pObjCube->SetPos(D3DXVECTOR3(300.0f, 0.0f, 0.0f));

	//CObjmeshField *pObjField = CObjmeshField::Create();
	//pObjField->SetPos(D3DXVECTOR3(400.0f, 0.0f, 0.0f));

	//CObjmeshWall* pObjWall = CObjmeshWall::Create();
	//pObjWall->SetPos(D3DXVECTOR3(500.0f, 0.0f, 0.0f));

	//CObjmeshCylinder* pObjCylinder = CObjmeshCylinder::Create();
	//pObjCylinder->SetPos(D3DXVECTOR3(600.0f, 0.0f, 0.0f));


	////�e�I�u�W�F�N�g�̎q�N���X�̐���-----------------------------------------
	m_p2DSample = CSampleObj2D::Create(7);
	m_p2DSample->SetPos(D3DXVECTOR3(640.0f, 360.0f, 0.0f));
	m_p2DSample->SetWidth(1280.0f);
	m_p2DSample->SetHeight(720.0f);
	m_p2DSample->SetColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.5f));
	m_p2DSample->SetMultiTarget(true);

	//CSampleObj3D* pSampleObj3D = CSampleObj3D::Create();
	//pSampleObj3D->SetPos(D3DXVECTOR3(-100.0f, 0.0f, 0.0f));

	//CSampleObjBillboard* pSampleObjBillboard = CSampleObjBillboard::Create();
	//pSampleObjBillboard->SetPos(D3DXVECTOR3(-200.0f, 0.0f, 0.0f));

	//CSampleObjectX* pSampleObjX = CSampleObjectX::Create("data\\MODEL\\enemy.x");
	//pSampleObjX->SetPos(D3DXVECTOR3(-300.0f, 0.0f, 0.0f));

	//CEnemy* pEnemy = CEnemy::Create();
	//pEnemy->SetPos(D3DXVECTOR3(-500.0f, 0.0f, 0.0f));

	//CSampleLvModel* pSampleLvModel = CSampleLvModel::Create();
	//pSampleLvModel->SetPos(D3DXVECTOR3(-400.0f, 0.0f, 0.0f));

	m_pPlayer = CPlayer::Create();
	m_pPlayer->SetPos(D3DXVECTOR3(-1000.0f, 0.0f, 0.0f));

	// �Ə��̐���
	m_pAim = CAim::Create();

	//// �G�̐���
	//CEnemy::Create();

	LoadEnemy("data\\TXT\\Enemy.txt");
	LoadStageMapModel();

	// �T�����
	CModeGame::SetMode(CModeGame::MODE_GAME_SEARCH);

#if _DEBUG
	if (m_pEdit == nullptr)
	{
		m_pEdit = CEdit::Create();
	}
#endif
	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void CGame::Uninit(void)
{
	CModeGame* pModeGame = CModeGame::GetInstance();

	if (pModeGame != nullptr)
	{
		pModeGame->Uninit();
		pModeGame->NullInstance();
		delete pModeGame;
	}

	//�S�ẴI�u�W�F�N�g�̔j��
	CObject::ReleaseAll();

	if (m_pPause != nullptr)
	{
		delete m_pPause;
		m_pPause = nullptr;
	}

#if _DEBUG
	if (m_pEdit != nullptr)
	{
		m_pEdit->Uninit();
		delete m_pEdit;
		m_pEdit = nullptr;
	}
#endif
}

//====================================================================
//�I������
//====================================================================
void CGame::DeleteTutorial(void)
{

}

//====================================================================
//�X�V����
//====================================================================
void CGame::Update(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CInputJoypad* pInputJoypad = CManager::GetInstance()->GetInputJoyPad();

#if _DEBUG
	if (pInputKeyboard->GetTrigger(DIK_0) == true)
	{
		m_Wireframe = (m_Wireframe == true) ? false : true;

		if (m_Wireframe == true)
		{
			//�����_�[�X�e�[�g�̐ݒ�
			pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		}
		else
		{
			//�����_�[�X�e�[�g�̐ݒ�
			pDevice->SetRenderState(D3DRS_FILLMODE, 0);
		}
	}

	if (CManager::GetInstance()->GetEdit() == true)
	{
		CManager::GetInstance()->GetCamera()->SetCameraMode(CCamera::CAMERAMODE_CONTROL);
		m_pEdit->Update();
	}

	if (pInputKeyboard->GetTrigger(DIK_1) == true)
	{
		float Speed = CManager::GetInstance()->GetGameSpeed();

		Speed -= 0.91f;

		CManager::GetInstance()->SetGameSpeed(Speed);
	}

	if (pInputKeyboard->GetTrigger(DIK_2) == true)
	{
		float Speed = CManager::GetInstance()->GetGameSpeed();

		Speed += 0.91f;

		CManager::GetInstance()->SetGameSpeed(Speed);
	}

	if (CManager::GetInstance()->GetGameSpeed() <= 1.0f)
	{
		m_Slow = true;
	}
	else
	{
		m_Slow = false;
	}

	CDebugProc *pDebugProc = CManager::GetInstance()->GetDebugProc();
	pDebugProc->Print("�Q�[���X�s�[�h : %f\n", CManager::GetInstance()->GetGameSpeed());

#endif
	//�|�[�Y�̍X�V����
	m_pPause->Update();

	if (m_bGameEnd == true)
	{
		CFade::SetFade(CScene::MODE_RESULT);

		if (CManager::GetInstance()->GetGameClear() == true)
		{
			CManager::GetInstance()->SetEndTime(m_pTime->GetTimeNumber());
		}
	}

	// ���͔���i�����j& ���[�h����i�Q�[���j
	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_UP) == true ||
		pInputJoypad->Get_RStick_Trigger_Test(0))
	{
		// �a���̐���
		CSlash::Create(nullptr);

		// �a���̐���
		CModeGame::SetMode(CModeGame::MODE_GAME_SEARCH);
	}

	// ���͔���i�����j& ���[�h����i�Q�[���j
	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_DOWN) == true ||
		pInputJoypad->Get_RStick_Trigger_Test(0))
	{
		// �a���̐���
		CModeGame::SetMode(CModeGame::MODE_GAME_BATTLE);
	}

	// �G�̗L���𔻒�
	if (EnemyCheck() == false)
	{
		CFade::SetFade(CScene::MODE_RESULT);
	}

	if (CManager::GetInstance()->GetGameSpeed() <= 0.1f)
	{
		m_p2DSample->SetTargetTexture(CManager::GetInstance()->GetRenderer()->GetTextureMT());
		m_p2DSample->SetAppear(true);
	}
	else
	{
		m_p2DSample->SetAppear(false);
	}
}

//====================================================================
//UI�̕\����Ԑݒ�
//====================================================================
void CGame::DispUI(bool Set)
{
	if (m_p2DBossName != nullptr)
	{
		m_p2DBossName->SetAppear(Set);
	}
	if (m_p2DUI_Attention != nullptr)
	{
		m_p2DUI_Attention->SetAppear(Set);
	}
	if (m_p2DUI_Attack != nullptr)
	{
		m_p2DUI_Attack->SetAppear(Set);
	}
	if (m_p2DUI_AttentionOK != nullptr)
	{
		m_p2DUI_AttentionOK->SetAppear(Set);
	}
	if (m_p2DUI_Dodge != nullptr)
	{
		m_p2DUI_Dodge->SetAppear(Set);
	}
	if (m_p2DUI_Jump != nullptr)
	{
		m_p2DUI_Jump->SetAppear(Set);
	}
	if (m_pPlayerName != nullptr)
	{
		m_pPlayerName->SetAppear(Set);
	}
	if (m_pBossName != nullptr)
	{
		m_pBossName->SetAppear(Set);
	}
}

//====================================================================
//�X�e�[�W�̃u���b�N�z�u
//====================================================================
void CGame::SetStageBlock(void)
{
	m_pCubeBlock = CCubeBlock::Create();
	m_pCubeBlock->SetPos(D3DXVECTOR3(0.0f, 100.0f, 0.0f));
	m_pCubeBlock->SetSize(D3DXVECTOR3(500.0f, 10.0f, 500.0f));
	m_pCubeBlock->SetMove(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
}

//====================================================================
//�ǂݍ��݃u���b�N�z�u
//====================================================================
void CGame::LoadStageBlock(void)
{
	FILE* pFile; //�t�@�C���|�C���^��錾

	//�t�@�C�����J��
	pFile = fopen("", "r");

	if (GetMode() == MODE_GAME)
	{
		//�t�@�C�����J��
		pFile = fopen(DATA_BLOCK_NAME,"r");
	}

	if (pFile != nullptr)
	{//�t�@�C�����J�����ꍇ

		char Getoff[32] = {};
		char boolLife[32] = {};
		char aString[128] = {};			//�S�~��
		char aStartMessage[32] = {};	//�X�^�[�g���b�Z�[�W
		char aSetMessage[32] = {};		//�Z�b�g���b�Z�[�W
		char aEndMessage[32] = {};		//�I�����b�Z�[�W

		fscanf(pFile, "%s", &aStartMessage[0]);
		if (strcmp(&aStartMessage[0], "STARTSETSTAGE") == 0)
		{
			while (1)
			{
				fscanf(pFile, "%s", &aSetMessage[0]);
				if (strcmp(&aSetMessage[0], "STARTSETBLOCK") == 0)
				{
					D3DXVECTOR3 pos;
					D3DXVECTOR3 Size;

					fscanf(pFile, "%s", &aString[0]);
					fscanf(pFile, "%f", &pos.x);
					fscanf(pFile, "%f", &pos.y);
					fscanf(pFile, "%f", &pos.z);

					fscanf(pFile, "%s", &aString[0]);
					fscanf(pFile, "%f", &Size.x);
					fscanf(pFile, "%f", &Size.y);
					fscanf(pFile, "%f", &Size.z);

					CCubeBlock* pBlock = CCubeBlock::Create();
					pBlock->SetPos(pos);
					pBlock->SetSize(Size);

					fscanf(pFile, "%s", &aEndMessage[0]);
					if (strcmp(&aEndMessage[0], "ENDSETBLOCK") != 0)
					{
						break;
					}
				}
				else if (strcmp(&aSetMessage[0], "ENDSETSTAGE") == 0)
				{
					break;
				}
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
//�ǂݍ��݃}�b�v���f���z�u
//====================================================================
void CGame::LoadStageMapModel(void)
{
	FILE* pFile; //�t�@�C���|�C���^��錾

	//�t�@�C�����J��
	pFile = fopen("", "r");

	if (GetMode() == MODE_GAME)
	{
		//�t�@�C�����J��
		pFile = fopen(DATA_XMODEL_NAME, "r");
	}

	if (pFile != nullptr)
	{//�t�@�C�����J�����ꍇ

		char Getoff[32] = {};
		char boolLife[32] = {};
		char aString[128] = {};			//�S�~��
		char aStartMessage[32] = {};	//�X�^�[�g���b�Z�[�W
		char aSetMessage[32] = {};		//�Z�b�g���b�Z�[�W
		char aEndMessage[32] = {};		//�I�����b�Z�[�W
		char aBool[8] = {};				//bool�^���b�Z�[�W
		bool Loop = false;

		fscanf(pFile, "%s", &aStartMessage[0]);
		if (strcmp(&aStartMessage[0], "STARTSETSTAGE") == 0)
		{
			while (1)
			{
				fscanf(pFile, "%s", &aSetMessage[0]);
				if (strcmp(&aSetMessage[0], "STARTSETXMODEL") == 0)
				{
					char aModelName[64] = {};		//���f���̃p�X��
					D3DXVECTOR3 pos;
					D3DXVECTOR3 rot;

					fscanf(pFile, "%s", &aString[0]);
					fscanf(pFile, "%s", &aModelName[0]);

					fscanf(pFile, "%s", &aString[0]);
					fscanf(pFile, "%f", &pos.x);
					fscanf(pFile, "%f", &pos.y);
					fscanf(pFile, "%f", &pos.z);

					fscanf(pFile, "%s", &aString[0]);
					fscanf(pFile, "%f", &rot.x);
					fscanf(pFile, "%f", &rot.y);
					fscanf(pFile, "%f", &rot.z);

					fscanf(pFile, "%s", &aString[0]);
					fscanf(pFile, "%s", &aBool[0]);	//���[�v���邩�ǂ�����ݒ�
					Loop = (strcmp(&aBool[0], "1") == 0 ? true : false);			//bool�^�̏�����

					CMapModel* pModel = CMapModel::Create(&aModelName[0]);
					pModel->SetPos(pos);
					pModel->SetRot(rot);

					if (rot.y < 1.57f)
					{

					}
					else if (rot.y < 3.14f)
					{
						pModel->SwapSize();
					}
					else if (rot.y < 4.71f)
					{

					}
					else
					{
						pModel->SwapSize();
					}

					pModel->SetCollision(Loop);

					fscanf(pFile, "%s", &aEndMessage[0]);
					if (strcmp(&aEndMessage[0], "ENDSETXMODEL") != 0)
					{
						break;
					}
				}
				else if (strcmp(&aSetMessage[0], "ENDSETSTAGE") == 0)
				{
					break;
				}
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
//�ǂݍ��ݓG�z�u
//====================================================================
void CGame::LoadEnemy(const char* pFilename)
{
	FILE* pFile; //�t�@�C���|�C���^��錾

//�t�@�C�����J��
	pFile = fopen("", "r");

	if (GetMode() == MODE_GAME)
	{
		//�t�@�C�����J��
		pFile = fopen(pFilename, "r");
	}

	if (pFile != nullptr)
	{//�t�@�C�����J�����ꍇ

		char Getoff[32] = {};
		char boolLife[32] = {};
		char aString[128] = {};			//�S�~��
		char aMessage[128] = {};	//���b�Z�[�W
		char aBool[32] = {};			//bool�^���b�Z�[�W

		while (1)
		{
			fscanf(pFile, "%s", &aMessage[0]);
			if (strcmp(&aMessage[0], "START_ENEMYSET") == 0)
			{
				while (1)
				{
					fscanf(pFile, "%s", &aMessage[0]);
					if (strcmp(&aMessage[0], "ENEMYSET") == 0)
					{
						D3DXVECTOR3 pos[32];
						int nMax = 0;
						bool Loop = false;

						while (1)
						{
							fscanf(pFile, "%s", &aMessage[0]);
							if (strcmp(&aMessage[0], "POSSET") == 0)
							{
								while (1)
								{
									fscanf(pFile, "%s", &aMessage[0]);
									if (strcmp(&aMessage[0], "POS") == 0)
									{
										fscanf(pFile, "%s", &aString[0]);
										fscanf(pFile, "%f", &pos[nMax].x);
										fscanf(pFile, "%f", &pos[nMax].y);
										fscanf(pFile, "%f", &pos[nMax].z);

										nMax++;
									}
									if (strcmp(&aMessage[0], "ENDPOSSET") == 0)
									{
										break;
									}
								}
							}
							if (strcmp(&aMessage[0], "LOOP") == 0)
							{
								fscanf(pFile, "%s", &aString[0]);
								fscanf(pFile, "%s", &aBool[0]);	//���[�v���邩�ǂ�����ݒ�
								Loop = (strcmp(&aBool[0], "1") == 0 ? true : false);			//bool�^�̏�����
							}
							if (strcmp(&aMessage[0], "END_ENEMYSET") == 0)
							{
								CEnemy* pEnemy = CEnemy::Create();
								pEnemy->SetPos(pos[0]);
								for (int nCnt = 0; nCnt < nMax; nCnt++)
								{
									pEnemy->SetSeatchPos(pos[nCnt], nCnt);
								}
								if (nMax >= 2)
								{
									pEnemy->SetSeatchState(CEnemy::SEARCHSTATE_WALK);

									pEnemy->SetRot(D3DXVECTOR3(
										0.0f,
										atan2f(pos[1].z - pos[0].z, pos[0].x - pos[1].x) + D3DX_PI * 0.5f,
										0.0f));
								}
								else
								{
									pEnemy->SetSeatchState(CEnemy::SEARCHSTATE_WAIT);
								}
								pEnemy->SetSeatchLoop(Loop);

								break;
							}
						}
					}
					else if (strcmp(&aMessage[0], "END_ENEMYSET") == 0)
					{
						break;
					}
				}
			}

			if (strcmp(&aMessage[0], "END_ENEMYSET") == 0)
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
//�G�����邩���Ȃ����̔��f
//====================================================================
bool CGame::EnemyCheck(void)
{
	m_nEnemyNum = 0;

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		//�I�u�W�F�N�g���擾
		CObject* pObj = CObject::GetTop(nCntPriority);

		while (pObj != nullptr)
		{
			CObject* pObjNext = pObj->GetNext();

			CObject::TYPE type = pObj->GetType();			//��ނ��擾

			if (type == CObject::TYPE_ENEMY3D)
			{//��ނ��G�̎�
				m_nEnemyNum++;
			}

			pObj = pObjNext;
		}
	}

	if (m_nEnemyNum > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//====================================================================
//�G�����邩���Ȃ����̔��f
//====================================================================
void CGame::DeleteMap(void)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		//�I�u�W�F�N�g���擾
		CObject* pObj = CObject::GetTop(nCntPriority);

		while (pObj != nullptr)
		{
			CObject* pObjNext = pObj->GetNext();

			CObject::TYPE type = pObj->GetType();			//��ނ��擾

			if (type == CObject::TYPE_CUBEBLOCK ||
				type == CObject::TYPE_CUBEDAMEGE)
			{//��ނ��}�b�v�֘A�̎�
				pObj->Uninit();
			}

			pObj = pObjNext;
		}
	}
}

//====================================================================
//�`�揈��
//====================================================================
void CGame::Draw(void)
{

}