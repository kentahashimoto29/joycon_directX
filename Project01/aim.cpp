//============================================
//
//	�Ə� [aim.cpp]
//	Author:�ؓ������}�����ؓ�
//
//============================================
#include "aim.h"
#include "manager.h"
#include "input.h"
#include "useful.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "useful.h"
#include "camera.h"

// �}�N����`
namespace
{
	D3DXVECTOR2 AIM_SIZE = { 5.0f, 5.0f };	// �傫��
	float DISTANCE = 100.0f;				// �G�C���̋���
	float SENSITIVITY = 0.01f;				// ���x
}

//====================================================================
//= �R���X�g���N�^
//====================================================================
CAim::CAim(int nPriority) : CObjectBillboard(nPriority)
{
	// �l�̏�����
	ZeroMemory(&m_info, sizeof(m_info));
}

//====================================================================
//= �f�X�g���N�^
//====================================================================
CAim::~CAim()
{

}

//====================================================================
//= ��������
//====================================================================
CAim* CAim::Create()
{
	// �Ə��̐���
	CAim* pAim = new CAim();

	//����������
	if (FAILED(pAim->Init()))
	{//���������������s�����ꍇ
		return nullptr;
	}

	return pAim;
}

//====================================================================
//= ����������
//====================================================================
HRESULT CAim::Init()
{
	// �p���N���X�̏�����
	CObjectBillboard::Init();

	// �傫���ݒ�
	SetWidth(AIM_SIZE.x);
	SetHeight(AIM_SIZE.y);

	return S_OK;
}

//====================================================================
//= �I������
//====================================================================
void CAim::Uninit()
{

}

//====================================================================
//= �X�V����
//====================================================================
void CAim::Update()
{
	// �p���N���X�̍X�V
	CObjectBillboard::Update();

	// �ʒu�̐ݒ�i�h���b�N�j
	CursorMove();

	// ����
	//CursorLimit();

	// �F�̐ݒ�i�N���b�N�ŕύX�j
	CursorColor();

	// �f�o�b�N�\��
	CDebugProc* pDebug = CManager::GetInstance()->GetDebugProc();
	pDebug->Print("\n�Ə�\n");
	pDebug->Print("�ʒu:%f %f %f\n", GetPos().x, GetPos().y, GetPos().z);
	pDebug->Print("����:%f %f %f\n", GetRot().x, GetRot().y, GetRot().z);
}

//====================================================================
//= �`�揈��
//====================================================================
void CAim::Draw()
{
	// �p���N���X�̕`��
	CObjectBillboard::Draw();
}

//====================================================================
//= �J�[�\���ړ�
//====================================================================
void CAim::CursorMove()
{
	// �����擾
	CInputMouse* pMouse = CManager::GetInstance()->GetInputMouse();
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	if (!pCamera)
	{
		return;
	}

	if (pMouse != nullptr)
	{
		// �}�E�X�����擾
		D3DXVECTOR3 move = pMouse->GetMouseMove();	// �ړ���
		// �G�C�������擾
		D3DXVECTOR3 pos = GetPos();
		D3DXVECTOR3 rot = GetRot();
		// ���݂̌�����ۑ�
		m_info.rotOld = rot;

		// ���������Z
		rot.x += (-move.y) * SENSITIVITY;
		rot.y += move.x * SENSITIVITY;

		// �ړ��͈͂̐���
		Limit(&rot, &move);

		// �Ə��̈ʒu���v�Z
		pos.x = pCamera->GetPosV().x + sinf(pCamera->GetRot().y + rot.y) * cosf(pCamera->GetRot().x + rot.x) * DISTANCE;
		pos.z = pCamera->GetPosV().z + cosf(pCamera->GetRot().y + rot.y) * cosf(pCamera->GetRot().x + rot.x) * DISTANCE;
		pos.y = pCamera->GetPosV().y + sinf(pCamera->GetRot().x + rot.x) * DISTANCE;

		// �ʒu�ݒ�
		SetPos(pos);

		// �����ݒ�
		SetRot(rot);
	}
}

//====================================================================
//= �J�[�\���F
//====================================================================
void CAim::CursorColor()
{
	// �}�E�X���擾
	CInputMouse* pMouse = CManager::GetInstance()->GetInputMouse();

	if (pMouse != nullptr)
	{
		// �}�E�X���͂̔���i���N���b�N || �E�N���b�N�j
		if (pMouse->GetPress(pMouse->PUSH_LEFT) || pMouse->GetPress(pMouse->PUSH_RIGHT))
		{
			// �ΐF
			SetColor({ 0.0f, 1.0f, 0.0f, 1.0f });
		}
		else
		{
			// �f�t�H���g
			SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		}
	}
}

//====================================================================
//= ����1
//====================================================================
void CAim::CursorLimit()
{
	// �����擾
	CInputMouse* pMouse = CManager::GetInstance()->GetInputMouse();
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	if (!pCamera)
	{
		return;
	}

	// �ϐ��錾
	D3DXVECTOR3 ScreenPos = {};
	D3DXVECTOR3 AimPos = GetPos();
	D3DXVECTOR3 AimRot = GetRot();
	D3DXVECTOR3 rot = GetRot();

	// ���݂̌�����ۑ�
	m_info.rotOld = AimRot;

	// 2D���W�ϊ�
	useful::Vec3Project(&ScreenPos, AimPos);

	// �f�o�b�N�\��
	CDebugProc* pDebug = CManager::GetInstance()->GetDebugProc();
	pDebug->Print("2D�ʒu:%f %f %f\n", ScreenPos.x, ScreenPos.y, ScreenPos.z);
	pDebug->Print("�ߋ�����:%f %f %f\n", m_info.rotOld.x, m_info.rotOld.y, m_info.rotOld.z);


	if (useful::CollisionWindow(ScreenPos))
	{
		if (ScreenPos.x > SCREEN_WIDTH)
		{
			AimRot.y = m_info.rotOld.y - 0.01f;

		}
		else if (ScreenPos.x < 0.0f)
		{
			AimRot.y = m_info.rotOld.y + 0.01f;
		}
		if (ScreenPos.y > SCREEN_HEIGHT)
		{
			AimRot.x = m_info.rotOld.x + 0.01f;
		}
		else if (ScreenPos.y < 0.0f)
		{
			AimRot.x = m_info.rotOld.x - 0.01f;
		}

		// �Ə��̈ʒu���v�Z
		AimPos.x = pCamera->GetPosV().x + sinf(AimRot.y) * cosf(AimRot.x) * DISTANCE;
		AimPos.z = pCamera->GetPosV().z + cosf(AimRot.y) * cosf(AimRot.x) * DISTANCE;
		AimPos.y = pCamera->GetPosV().y + sinf(AimRot.x) * DISTANCE;

		// �ʒu�ݒ�
		SetPos(AimPos);

		// �����ݒ�
		SetRot(AimRot);

		pDebug->Print("���K��:%f %f %f\n", ScreenPos.x, ScreenPos.y, ScreenPos.z);
	}
}

//====================================================================
//= ����2
//====================================================================
void CAim::Limit(D3DXVECTOR3* rot, D3DXVECTOR3* move)
{
	// �ϐ��錾
	D3DXVECTOR3 ScreenPos = {};
	D3DXVECTOR3 AimPos = GetPos();
	D3DXVECTOR3 AimRot = GetRot();

	// 2D���W�ϊ�
	useful::Vec3Project(&ScreenPos, AimPos);

	if (useful::CollisionWindow(ScreenPos))
	{
		if (ScreenPos.x > SCREEN_WIDTH)
		{
			rot->y = AimRot.y - 0.01f;
			move->y = 0.0f;
		}
		else if (ScreenPos.x < 0.0f)
		{
			rot->y = AimRot.y + 0.01f;
			move->y = 0.0f;
		}
		if (ScreenPos.y > SCREEN_HEIGHT)
		{
			rot->x = AimRot.x + 0.01f;
			move->x = 0.0f;
		}
		else if (ScreenPos.y < 0.0f)
		{
			rot->x = AimRot.x - 0.01f;
			move->x = 0.0f;
		}
	}
}
