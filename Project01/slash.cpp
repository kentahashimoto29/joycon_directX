//=============================================
//=
//=	�a�� [slash.cpp]
//=	Author:sakamoto kai
//=
//=============================================

#include "main.h"
#include "slash.h"

#include "useful.h"

#include "input.h"
#include "debugproc.h"

#include "game.h"
#include "aim.h"

#include "enemy.h"

#include "player.h"

// �萔��`
const float LENGTH = 200.0f;		// ����

const D3DXVECTOR2 SIZE_DEFAULT =	// �傫���̃f�t�H���g
		D3DXVECTOR2(15.0f, LENGTH * 0.5f);
const D3DXVECTOR2 SIZE_DEST_2D = 	// �傫���̌�����
		D3DXVECTOR2(2.0f, 0.0f);

const D3DXCOLOR HIT_RED = 	// �q�b�g���̃|���S���F
		D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

const int SCREEN_POS_ENEMY_NUM = 8;	// �G�̃X�N���[�����W�̐��i���j

const int EVENT_SPWAN_SLASH_DIV = 2;	// ���̃X���b�V���R�}�܂ł̏o���t���[��
const int EVENT_DEST_START = 30;		// �傫���̌����J�n�̃t���[��
const int EVENT_END = 60;				// �I���̃t���[��

//---------------------------------------------------------
//- �R���X�g���N�^
//---------------------------------------------------------
CSlash::CSlash(int nPriority) :CObject(nPriority)
{
	// �a�����̏�����
	ZeroMemory(&m_info,sizeof(m_info));
}

//---------------------------------------------------------
//- �f�X�g���N�^
//---------------------------------------------------------
CSlash::~CSlash()
{

}

//---------------------------------------------------------
//- ��������
//---------------------------------------------------------
CSlash* CSlash::Create(const char* p2dFileName)
{
	// ����
	CSlash* pInstance = new CSlash();

	// ����������
	if (FAILED(pInstance->Init()))
	{//���������������s�����ꍇ
		return nullptr;
	}

	// 2D�I�u�W�F�N�g�̏�����
	pInstance->InitObj2D(p2dFileName);

	return pInstance;
}

//---------------------------------------------------------
//- ����������
//---------------------------------------------------------
HRESULT CSlash::Init(void)
{
	//�f�o�C�X�̎擾
	CInputJoypad* pInputJoypad = CManager::GetInstance()->GetInputJoyPad();

	//// �Ə����擾
	//CAim* pAim = CGame::GetAim();

	//if (pAim != nullptr)
	//{
	//	// �ʒu���擾
	//	D3DXVECTOR3 posAim = pAim->GetPos();

	//	// �J�[�\���ʒu���璆�S�ւ̕������Z�o
	//	m_info.angle.z = atan2f(SCREEM_CENTER.x - posAim.x, SCREEM_CENTER.y - posAim.y) + (D3DX_PI);
	//}

	if (pInputJoypad != nullptr)
	{
		// �R���g���[���[�̍��X�e�B�b�N�����擾�i1�Ԗڂ̃R���g���[���[�j
		D3DXVECTOR3 rotStick = pInputJoypad->Get_Stick_Right(0);
		D3DXVECTOR3 pi = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		
		// �x���@�Z�o
		pi.y = atan2f(-rotStick.x, rotStick.y);

		// �J�[�\���ʒu���璆�S�ւ̕������Z�o
		m_info.angle.z = pi.y;
	}

	// �l�����i��)
	m_info.pos = D3DXVECTOR3(						// �ʒu
		sinf(m_info.angle.z) * LENGTH, 
		cosf(m_info.angle.z) * LENGTH, 
		0.0f);	
	m_info.sizeDef = SIZE_DEFAULT;					// �f�t�H���g�T�C�Y
	m_info.color = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);	// �F

	return S_OK;
}

//---------------------------------------------------------
//- �I������
//---------------------------------------------------------
void CSlash::Uninit(void)
{
	// 2D�̏I������
	for (int nCnt = 0; nCnt < SLASH_2D_NUM; nCnt++)
	{
		if (m_info.aInfo2D[nCnt].pObj2d != nullptr)
		{
			// �I������
			m_info.aInfo2D[nCnt].pObj2d->Uninit();
			m_info.aInfo2D[nCnt].pObj2d = nullptr;
		}
	}

	// ���S�t���O���z
	SetDeathFlag(true);
}

//---------------------------------------------------------
//- �X�V����
//---------------------------------------------------------
void CSlash::Update(void)
{
	// �J�E���g���Ƃ̃C�x���g
	EventCnt();

	// ��Ԕ���i�ʏ�j
	if (m_info.state == STATE_NONE)
	{
		// �G�Ƃ̓����蔻��
		CollsionEnemy();
	}
}

//---------------------------------------------------------
//- �`�揈��
//---------------------------------------------------------
void CSlash::Draw(void)
{

}

//---------------------------------------------------------
//- 2D�I�u�W�F�N�g�̏���������
//---------------------------------------------------------
void CSlash::InitObj2D(const char* p2dFileName)
{
	for (int nCnt = 0; nCnt < SLASH_2D_NUM; nCnt++)
	{
		// ��������
		m_info.aInfo2D[nCnt].pObj2d = CObject2D::Create();

		if (m_info.aInfo2D[nCnt].pObj2d != nullptr)
		{
			// �Q�L�L���v�Z���i���j
			float fLength = LENGTH + ((nCnt + 0.5f) * -(LENGTH * 0.5f));

			// �l����i���j
			m_info.aInfo2D[nCnt].pObj2d->SetPos(
					SCREEM_CENTER + D3DXVECTOR3(sinf(m_info.angle.z) * fLength, 
					cosf(m_info.angle.z) * fLength, 
					0.0f));
			Set2dSize(m_info.sizeDef,nCnt);
			m_info.aInfo2D[nCnt].color = m_info.color;
			m_info.aInfo2D[nCnt].pObj2d->SetRot(m_info.angle);
			m_info.aInfo2D[nCnt].pObj2d->SetAppear(false);

			//// �e�N�X�`���ݒ菈��
			//m_pObj2d->SetTexture(p2dFileName);
		}
	}
}

//---------------------------------------------------------
//- �J�E���g���Ƃ̃C�x���g
//---------------------------------------------------------
void CSlash::EventCnt(void)
{
	m_info.nCnt++;

	// �`��ݒ�C�x���g
	if (m_info.nCnt % EVENT_SPWAN_SLASH_DIV == 0)
	{
		if (m_info.AllAppear == false)
		{
			// �`��ݒ�
			Set2dAppear();
		}
	}
	else if (m_info.nCnt >= EVENT_DEST_START)
	{
		for (int nCnt = 0; nCnt < SLASH_2D_NUM; nCnt++)
		{
			// 2D�̌�������
			Dec2dSize(SIZE_DEST_2D, nCnt);
		}
	}

	// �I���C�x���g
	if (m_info.nCnt % EVENT_END == 0)
	{
		// �I������
		Uninit();
	}
}

//---------------------------------------------------------
//- �G�Ƃ̓����蔻��
//---------------------------------------------------------
void CSlash::CollsionEnemy(void)
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
				float ObjWidth = pObj->GetSize().x;		// �T�C�Y

				CEnemy* pEnemy = (CEnemy*)pObj;

				// �^�[�Q�b�g�Ƃ̋����𔻒�
				float fLengthTgtSquared = ((ObjPos.x - posPlayer.x) * (ObjPos.x - posPlayer.x) +
										   (ObjPos.y - posPlayer.y) * (ObjPos.y - posPlayer.y) +
										   (ObjPos.z - posPlayer.z) * (ObjPos.z - posPlayer.z));

				// ������Ԃ�
				float fLength = std::sqrt(fLengthTgtSquared);

				// �a���̓����蔻��
				if (useful::SlashCollision(pObj->GetScreenPos(), SCREEN_POS_ENEMY_NUM, m_info.angle.z) &&
					fLength <= 200.0f)
				{
					// �F��ύX�i�ԁj
					m_info.color = HIT_RED;
					m_info.state = STATE_HIT;

					for (int nCnt = 0; nCnt < SLASH_2D_NUM; nCnt++)
					{
						// 2D�̐F��ύX�i�ԁj
						m_info.aInfo2D[nCnt].pObj2d->SetColor(m_info.aInfo2D[nCnt].color = m_info.color);
					}

					// �_���[�W����
					pEnemy->HitDamage(10.0f);
				}
			}

			// ���̃I�u�W�F�N�g����
			pObj = pObjNext;
		}
	}
}

//---------------------------------------------------------
//- 2D�̃T�C�Y�ݒ菈��
//---------------------------------------------------------
void CSlash::Set2dSize(D3DXVECTOR2 size, int nCnt)
{
	m_info.aInfo2D[nCnt].size = size;
	m_info.aInfo2D[nCnt].pObj2d->SetWidth(m_info.aInfo2D[nCnt].size.x);
	m_info.aInfo2D[nCnt].pObj2d->SetHeight(m_info.aInfo2D[nCnt].size.y);
}

//---------------------------------------------------------
//- 2D�̃T�C�Y��������
//---------------------------------------------------------
void CSlash::Dec2dSize(D3DXVECTOR2 sizeDec, int nCnt)
{
	m_info.aInfo2D[nCnt].size -= sizeDec;
	m_info.aInfo2D[nCnt].pObj2d->SetWidth(m_info.aInfo2D[nCnt].size.x);
	m_info.aInfo2D[nCnt].pObj2d->SetHeight(m_info.aInfo2D[nCnt].size.y);
}

//---------------------------------------------------------
//- �`��L���̐ݒ菈��
//---------------------------------------------------------
void CSlash::Set2dAppear(void)
{
	// �a��2D�̍ő吔
	for (int nCnt = 0; nCnt < SLASH_2D_NUM; nCnt++)
	{
		// �`��̗L���𔻒�
		if (m_info.aInfo2D[nCnt].pObj2d->GetAppear() == false)
		{
			// �`��
			m_info.aInfo2D[nCnt].pObj2d->SetAppear(true);

			// �ő吔
			if (nCnt == SLASH_2D_NUM - 1)
			{
				// �S�`��ݒ���I��
				m_info.AllAppear = true;
			}

			break;
		}
	}
}

