//============================================
//
//	�藠��[Shuriken.h]
//	Author:Takeru Ogasawara
//
//============================================
#include "Shuriken.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "input.h"
#include "camera.h"

#include "game.h"
#include "aim.h"
#include "player.h"
#include "enemy.h"

#include "useful.h"

// �}�N����`
namespace
{
	char* MOTEL_FILE_PATH = "data\\MODEL\\player\\wepon\\syuriken1.x";	// ���f���̃p�X

	float Decrease = 1.0f;	// �����̌�����
	float ROTATE_SPEED = 2.0f;
}


//====================================================================
// �R���X�g���N�^
//====================================================================
CShuriken::CShuriken()
{
	// �l�̏�����
	ZeroMemory(&m_info, sizeof(m_info));
}

//====================================================================
// �f�X�g���N�^
//====================================================================
CShuriken::~CShuriken()
{

}

//====================================================================
// �������� : Create(�ʒu�A�����A����)
//====================================================================
CShuriken* CShuriken::Create(D3DXVECTOR3 pos, float fSpeed, float fLife)
{
	CShuriken* pShuriken = nullptr;

	if (pShuriken == nullptr)
	{
		// �C���X�^���X����
		pShuriken = new CShuriken;

		// ����������
		pShuriken->Init();

		// �ʒu�ݒ�
		pShuriken->SetPos(pos);

		// �����ݒ�
		pShuriken->m_info.fSpeed = fSpeed;

		// �����ݒ�
		pShuriken->m_info.fLife = fLife;
	}

	return pShuriken;
}

//====================================================================
// ����������
//====================================================================
HRESULT CShuriken::Init()
{
	// �p���N���X�̏�����
	CObjectX::Init(MOTEL_FILE_PATH);

	Test();

	return S_OK;
}

//====================================================================
// �I������
//====================================================================
void CShuriken::Uninit()
{
	// ���S�t���O�𗧂Ă�
	SetDeathFlag(true);
}

//====================================================================
// �X�V����
//====================================================================
void CShuriken::Update()
{
	// �p���N���X�̍X�V
	CObjectX::Update();

	CollsionEnemy();

	Throw();	// ������
}

//====================================================================
// �`�揈��
//====================================================================
void CShuriken::Draw()
{
	// �p���N���X�̕`��
	CObjectX::Draw();
}

//====================================================================
// ������
//====================================================================
void CShuriken::Throw(void)
{
	// ���̎擾
	CCamera* pCamera = CManager::GetInstance()->GetCamera();
	CDebugProc* pDebug = CManager::GetInstance()->GetDebugProc();
	CInputMouse* pMouse = CManager::GetInstance()->GetInputMouse();
	CPlayer* pPlayer = CGame::GetPlayer();
	CAim* pAim = CGame::GetAim();

	if (!pCamera || !pMouse || !pAim)
	{
		return;
	}

	// �g�����X�t�H�[���̎擾
	D3DXVECTOR3 pos = GetPos();

	// �J�[�\���������擾���A���̕����ւ̕����x�N�g�������߂�
	if (!m_info.bUse)
	{
		D3DXVECTOR3 posAim = pAim->GetPos();

		// �����x�N�g�����v�Z
		m_info.diff = posAim - pos;

		// �����x�N�g���𐳋K��
		D3DXVec3Normalize(&m_info.diff, &m_info.diff);

		m_info.bUse = true;
	}

	// �x�N�g���̕����֓�����
	if (m_info.bUse)
	{	
		// �ړ��ʂ����Z
		pos.x += m_info.move.x + m_info.diff.x * m_info.fSpeed * CManager::GetInstance()->GetGameSpeed();
		pos.y += m_info.move.y + m_info.diff.y * m_info.fSpeed * CManager::GetInstance()->GetGameSpeed();
		pos.z += m_info.move.z + m_info.diff.z * m_info.fSpeed * CManager::GetInstance()->GetGameSpeed();

		// �ʒu�̍X�V
		SetPos(pos);

		// �����Ɋւ��鏈��
		Rot();

		// ���������炷����
		LifeDecrease();
	}
}

//====================================================================
// �����̌���
//====================================================================
void CShuriken::LifeDecrease(void)
{
	// ���������炷
	m_info.fLife -= Decrease * CManager::GetInstance()->GetGameSpeed();

	if (m_info.fLife <= 0.0f)
	{
		// �I��
		Uninit();
	}
}

//====================================================================
// ����
//====================================================================
void CShuriken::Rot(void)
{
	// ���̎擾
	D3DXVECTOR3 rot = GetRot();

	// ��]
	rot.y += ROTATE_SPEED * CManager::GetInstance()->GetGameSpeed();

	// �����ݒ�
	SetRot(rot);
}

//====================================================================
// �G�C��
//====================================================================
void CShuriken::Aim(void)
{

}

//====================================================================
// ���b�N�I��
//====================================================================
void CShuriken::Lockon(void)
{
	// �ϐ��錾
	float fTempLength = 10000.0f;
	D3DXMATRIX mtxProjection;
	D3DXMATRIX mtxView;
	D3DXMATRIX mtxWorld;
	D3DXVECTOR3 EnemyPos = { };
	D3DXVECTOR3 ScreenPos;
	D3DVIEWPORT9 Viewport;

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	pDevice->GetTransform(D3DTS_PROJECTION, &mtxProjection);
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);
	pDevice->GetViewport(&Viewport);

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxWorld);

	CDebugProc* pDebug = CManager::GetInstance()->GetDebugProc();
	CPlayer* pPlayer = CGame::GetPlayer();

	if (pPlayer != nullptr)
	{
		// ���W�ϊ� (�ϊ����ʁA�ϊ�����3D���W�A�r���[�|�[�g�A�v���W�F�N�V�����s��A���[���h�s��)
		D3DXVec3Project(&ScreenPos, &EnemyPos, &Viewport, &mtxProjection, &mtxView, &mtxWorld);

		pDebug->Print("2D���W�@:%f %f %f\n", ScreenPos.x, ScreenPos.y, ScreenPos.z);
	}
}

//====================================================================
//= �G�Ƃ̓����蔻��
//====================================================================
void CShuriken::CollsionEnemy(void)
{
	CPlayer* pPlayer = CGame::GetPlayer();
	D3DXVECTOR3 posPlayer = pPlayer->GetPos();

	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		//�I�u�W�F�N�g���擾
		CObject* pObj = CObject::GetTop(nCntPriority);

		while (pObj != NULL)
		{
			// �I�u�W�F�N�g�����擾
			CObject* pObjNext = pObj->GetNext();	// ���̃I�u�W�F�N�g
			CObject::TYPE type = pObj->GetType();	// ���

			// ��ނ̔���i�G[3D]�j
			if (type == TYPE_ENEMY3D)
			{
				// �G�̏����擾
				D3DXVECTOR3 ObjPos = pObj->GetPos();	// �ʒu
				D3DXVECTOR3 ObjSize = pObj->GetSize();	// �傫��

				CEnemy* pEnemy = (CEnemy*)pObj;

				// �a���̓����蔻��
				if (useful::CollisionRectangle(GetPos(),GetSize(), ObjPos, ObjSize))
				{
					// �_���[�W����
					pEnemy->HitDamage(10.0f);

					// �I��
					Uninit();
				}
			}

			// ���̃I�u�W�F�N�g����
			pObj = pObjNext;
		}
	}
}