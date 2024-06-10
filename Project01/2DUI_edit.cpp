//============================================
//
//	2D�G�f�B�b�g [2DUI_edit.cpp]
//	Author:sakamoto kai
//
//============================================
#include "2DUI_edit.h"
#include "2DUI.h"
#include "object.h"
#include "object2D.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "debugproc.h"
#include "Effect.h"
#include "debugproc.h"
#include "texture.h"
#include "useful.h"

//�}�N����`
#define NUM_OBJECT (8)						//�}�b�v�ɏo���I�u�W�F�N�g�̎�ސ�
#define START_OK ("STARTSETUI")				//�X�^�[�g���b�Z�[�W�����邩�ǂ����̊m�F
#define END_SET_OK ("ENDSETUI")				//�G���h���b�Z�[�W�����邩�ǂ����̊m�F
#define UI_SET ("STARTSET")					//�u���b�N�̃Z�b�g���b�Z�[�W
#define DELETE_WIGHT (45.0f)				//�폜�����̉��̍폜�͈�
#define DELETE_HEIGHT (45.0f)				//�폜�����̏c�̍폜�͈�
#define SIDE_PRESSCOUNT (30)				//���ɑf�����ړ�����܂ł̃J�E���g
#define MOVE_POS (10.0f)					//�ړ�����

// �ÓI�����o�ϐ��錾
C2DUIEdit::UIINFO C2DUIEdit::m_aTextureInfo[UI_LABEL_MAX] =
{
	{ "data\\TEXTURE\\RankAP.png","�T���v��00" },
	{ "data\\TEXTURE\\RankA.png","�T���v��01" },
	{ "data\\TEXTURE\\RankAM.png","�T���v��02" },
	{ "data\\TEXTURE\\RankBP.png","�T���v��03" },
	{ "data\\TEXTURE\\RankB.png","�T���v��04" },
	{ "data\\TEXTURE\\RankBM.png","�T���v��05" },
	{ "data\\TEXTURE\\RankC.png","�T���v��06" },
};
CObject2D* C2DUIEdit::m_pEditUI = NULL;			//�u���b�N�ւ̃|�C���^

//====================================================================
//�R���X�g���N�^
//====================================================================
C2DUIEdit::C2DUIEdit()
{
	m_EditPos = INITVECTOR3;
	m_EditRot = INITVECTOR3;
	m_EditSize = D3DXVECTOR3(100.0f, 100.0f, 0.0f);
	ObjectType = 0;
	m_nSideDistance = 10.0f;
	m_nSideCount = 0;
	m_cTextureName = m_aTextureInfo[ObjectType].pFilename;
}

//====================================================================
//�f�X�g���N�^
//====================================================================
C2DUIEdit::~C2DUIEdit()
{

}

//====================================================================
//��������
//====================================================================
C2DUIEdit* C2DUIEdit::Create()
{
	C2DUIEdit* pMap = NULL;

	if (pMap == NULL)
	{
		//�I�u�W�F�N�g2D�̐���
		pMap = new C2DUIEdit();
	}

	m_pEditUI = CObject2D::Create();

	// �e�N�X�`���̓ǂݍ��ݏ���
	m_pEditUI->SetTexture(C2DUIEdit::m_aTextureInfo[0].pFilename);

	return pMap;
}

//====================================================================
//�I������
//====================================================================
void C2DUIEdit::Uninit(void)
{

}


//====================================================================
//�X�V����
//====================================================================
void C2DUIEdit::Update(void)
{
	C2DUI* pEditUI = NULL;

	//�L�[�{�[�h�̎擾
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	//�G�f�B�b�g�̈ʒu�𓮂���----------------------------------------------------------
	if (pInputKeyboard->GetTrigger(DIK_W) == true)
	{
		m_EditPos.y -= MOVE_POS;
	}
	if (pInputKeyboard->GetTrigger(DIK_S) == true)
	{
		m_EditPos.y += MOVE_POS;
	}

	if (pInputKeyboard->GetTrigger(DIK_A) == true)
	{
		m_EditPos.x -= MOVE_POS;
	}
	if (pInputKeyboard->GetTrigger(DIK_D) == true)
	{
		m_EditPos.x += MOVE_POS;
	}

	//�������ړ�----------------------------------------------------------------------
	if (pInputKeyboard->GetPress(DIK_W) == true)
	{
		if (m_nSideCount > SIDE_PRESSCOUNT && m_nSideCount % 2 == 0)
		{
			m_EditPos.y -= MOVE_POS * 2.0f;
		}
		m_nSideCount++;
	}
	else if (pInputKeyboard->GetPress(DIK_S) == true)
	{
		if (m_nSideCount > SIDE_PRESSCOUNT && m_nSideCount % 2 == 0)
		{
			m_EditPos.y += MOVE_POS * 2.0f;
		}
		m_nSideCount++;
	}
	else if (pInputKeyboard->GetPress(DIK_A) == true)
	{
		if (m_nSideCount > SIDE_PRESSCOUNT && m_nSideCount % 2 == 0)
		{
			m_EditPos.x -= MOVE_POS * 2.0f;
		}
		m_nSideCount++;
	}
	else if (pInputKeyboard->GetPress(DIK_D) == true)
	{
		if (m_nSideCount > SIDE_PRESSCOUNT && m_nSideCount % 2 == 0)
		{
			m_EditPos.x += MOVE_POS * 2.0f;
		}
		m_nSideCount++;
	}

	if (pInputKeyboard->GetPress(DIK_A) == false &&
		pInputKeyboard->GetPress(DIK_D) == false &&
		pInputKeyboard->GetPress(DIK_W) == false &&
		pInputKeyboard->GetPress(DIK_S) == false)
	{
		m_nSideCount = 0;
	}

	//�I�u�W�F�N�g�؂�ւ�----------------------------------------------------------
	if (pInputKeyboard->GetTrigger(DIK_0) == true)
	{
		ObjectType++;

		if (ObjectType >= UI_LABEL_MAX)
		{
			ObjectType = 0;
		}

		m_cTextureName = m_aTextureInfo[ObjectType].pFilename;

		// �e�N�X�`���̓ǂݍ��ݏ���
		m_pEditUI->SetTexture(m_cTextureName);
	}

	//�����ƈʒu�̐؂�ւ�----------------------------------------------------------
	if (pInputKeyboard->GetTrigger(DIK_9) == true)
	{
		m_EditRot.y += 1.57f;

		if (m_EditRot.y >= 6.0f)
		{
			m_EditRot.y = 0;
		}
	}

	//�Ƃ��̈ړ��͈͒���----------------------------------------------------------
	if (pInputKeyboard->GetTrigger(DIK_3) == true)
	{
		m_nSideDistance -= 5.0f;
	}
	if (pInputKeyboard->GetTrigger(DIK_4) == true)
	{
		m_nSideDistance += 5.0f;
	}

	//�Ƃ��̈ړ��͈͒���----------------------------------------------------------
	if (pInputKeyboard->GetTrigger(DIK_5) == true)
	{
		m_EditSize.x -= m_nSideDistance;
	}
	if (pInputKeyboard->GetTrigger(DIK_6) == true)
	{
		m_EditSize.x += m_nSideDistance;
	}
	if (pInputKeyboard->GetTrigger(DIK_7) == true)
	{
		m_EditSize.y -= m_nSideDistance;
	}
	if (pInputKeyboard->GetTrigger(DIK_8) == true)
	{
		m_EditSize.y += m_nSideDistance;
	}

	//�ʒu�X�V----------------------------------------------------------
	m_pEditUI->SetPos(m_EditPos);
	m_pEditUI->SetRot(m_EditRot);
	m_pEditUI->SetWidth(m_EditSize.x);
	m_pEditUI->SetHeight(m_EditSize.y);

	//�I�u�W�F�N�g�ݒu----------------------------------------------------------
	if (pInputKeyboard->GetTrigger(DIK_RETURN) == true)
	{
		pEditUI = C2DUI::Create();
		pEditUI->SetPos(m_EditPos);
		pEditUI->SetRot(m_EditRot);
		pEditUI->SetWidth(m_EditSize.x);
		pEditUI->SetHeight(m_EditSize.y);
		pEditUI->SetTexture(m_cTextureName);
	}

	if (pInputKeyboard->GetTrigger(DIK_BACKSPACE) == true)
	{
		DeleteObject(m_EditPos);
	}

	if (pInputKeyboard->GetTrigger(DIK_F9) == true)
	{
		SaveData();
	}

	DebugObject();
}

//====================================================================
//�폜����
//====================================================================
void C2DUIEdit::DeleteObject(D3DXVECTOR3 pos)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		//�I�u�W�F�N�g���擾
		CObject* pObj = CObject::GetTop(nCntPriority);

		while (pObj != NULL)
		{
			CObject* pObjNext = pObj->GetNext();

			CObject::TYPE type = pObj->GetType();				//��ނ��擾

			if (type == CObject::TYPE_2DUI)
			{//��ނ��u���b�N�̎�
				C2DUI* pUI = (C2DUI*)pObj;

				if (useful::CollisionRectangle2D(pos, pUI->GetPos(), D3DXVECTOR3(DELETE_WIGHT, DELETE_HEIGHT, 0.0f), D3DXVECTOR3(pUI->GetWidth() * 0.5f, pUI->GetHeight() * 0.5f, 0.0f),useful::COLLISION_XY) == true)
				{
					pUI->Uninit();
				}
			}

			pObj = pObjNext;
		}
	}
}

//====================================================================
//�f�o�b�O�\��
//====================================================================
void C2DUIEdit::DebugObject(void)
{
	CManager::GetInstance()->GetDebugProc()->Print("\n%s\n", "�[�[�[�[�[�G�f�B�b�g���[�h�[�[�[�[�[");

	CManager::GetInstance()->GetDebugProc()->Print("\n%s\n", "��������[�[");
	//�������
	CManager::GetInstance()->GetDebugProc()->Print("%s\n", "�ړ��F[W],[A],[S],[D]");
	CManager::GetInstance()->GetDebugProc()->Print("%s\n", "�e�N�X�`���ύX�F[�O]");
	CManager::GetInstance()->GetDebugProc()->Print("%s\n", "�T�C�Y�ύX�l�̉��~�㏸�F[�R�F�S]");
	CManager::GetInstance()->GetDebugProc()->Print("%s\n", "UI�̉��T�C�Y�̉��~�㏸�F[�T�F�U]");
	CManager::GetInstance()->GetDebugProc()->Print("%s\n", "UI�̏c�T�C�Y�̉��~�㏸�F[�V�F�W]");
	CManager::GetInstance()->GetDebugProc()->Print("%s\n", "�I�u�W�F�N�g��]�F[�X]");
	CManager::GetInstance()->GetDebugProc()->Print("%s\n", "�Z�[�u�����F[F9]");

	CManager::GetInstance()->GetDebugProc()->Print("\n%s\n", "�[�[�G�f�B�b�g���[�[");

	//�I�u�W�F�N�g
	CManager::GetInstance()->GetDebugProc()->Print("�e�N�X�`���F[%s]\n", m_aTextureInfo[ObjectType].pCommentname);

	//�ʒu�E�����E�傫��
	CManager::GetInstance()->GetDebugProc()->Print("�ʒu�F[%f:%f]\n", m_EditPos.x, m_EditPos.y);
	CManager::GetInstance()->GetDebugProc()->Print("�T�C�Y�ύX�l�F[%f]\n", m_nSideDistance);
	CManager::GetInstance()->GetDebugProc()->Print("�T�C�Y�F[%f:%f]\n", m_EditSize.x, m_EditSize.y);
	CManager::GetInstance()->GetDebugProc()->Print("�����F[%f]\n", m_EditRot.y);

	//�Ƃ��̈ړ��͈�
	CManager::GetInstance()->GetDebugProc()->Print("\n%s\n", "�[�[�[�[�[�G�f�B�b�g���[�h�[�[�[�[�[");
}

//====================================================================
//�ۑ�����
//====================================================================
void C2DUIEdit::SaveData(void)
{
	FILE* pFile; //�t�@�C���|�C���^��錾

	//�t�@�C�����J��
	pFile = fopen(LOADUI_NAME, "w");

	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ

	 //�X�e�[�W���Z�[�u����J�n�̍��}
		fprintf(pFile, "%s\n\n", START_OK);

		for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
		{
			//�I�u�W�F�N�g���擾
			CObject* pObj = CObject::GetTop(nCntPriority);

			while (pObj != NULL)
			{
				CObject* pObjNext = pObj->GetNext();

				CObject::TYPE type = pObj->GetType();				//��ނ��擾

				if (type == CObject::TYPE_2DUI)
				{//��ނ��u���b�N�̎�
					C2DUI* pUI = (C2DUI*)pObj;

					fprintf(pFile, "%s\n", UI_SET);

					//�ʒu�̎擾
					fprintf(pFile, "%f ", pUI->GetPos().x);
					fprintf(pFile, "%f ", pUI->GetPos().y);
					fprintf(pFile, "%f\n", pUI->GetPos().z);

					//�����̎擾
					fprintf(pFile, "%f ", pUI->GetRot().x);
					fprintf(pFile, "%f ", pUI->GetRot().y);
					fprintf(pFile, "%f\n", pUI->GetRot().z);

					//�傫���̎擾
					fprintf(pFile, "%f ", pUI->GetWidth());
					fprintf(pFile, "%f\n", pUI->GetHeight());

					fprintf(pFile, "%s\n", pUI->GetTexture());

					fprintf(pFile, "%s\n\n", "ENDSET");
				}

				pObj = pObjNext;
			}
		}

		//�X�e�[�W���Z�[�u�����I���̍��}
		fprintf(pFile, "%s", END_SET_OK);

		fclose(pFile);
	}
	else
	{//�t�@�C�����J���Ȃ������ꍇ
		printf("***�t�@�C�����J���܂���ł���***\n");
	}
}

//====================================================================
//�}�b�v��S�Ė��߂Đ���
//====================================================================
void C2DUIEdit::LoadData(char* Name, D3DXVECTOR3 pos)
{
	FILE* pFile; //�t�@�C���|�C���^��錾

	//�t�@�C�����J��
	pFile = fopen(Name, "r");

	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ
		D3DXVECTOR3 SetPos = INITVECTOR3;
		D3DXVECTOR3 SetRot = INITVECTOR3;
		D3DXVECTOR3 SetSize = INITVECTOR3;
		char TexName[128] = {};
		int nTex = 0;
		char aStartMessage[32] = {};	//�X�^�[�g���b�Z�[�W
		char aSetMessage[32] = {};		//�Z�b�g���b�Z�[�W

		fscanf(pFile, "%s", &aStartMessage[0]);
		if (strcmp(&aStartMessage[0], START_OK) == 0)
		{
			while (1)
			{
				fscanf(pFile, "%s", &aSetMessage[0]);
				if (strcmp(&aSetMessage[0], END_SET_OK) == 0)
				{
					break;
				}
				else if (strcmp(&aSetMessage[0], UI_SET) == 0)
				{
					fscanf(pFile, "%f", &SetPos.x);
					fscanf(pFile, "%f", &SetPos.y);
					fscanf(pFile, "%f", &SetPos.z);
					fscanf(pFile, "%f", &SetRot.x);
					fscanf(pFile, "%f", &SetRot.y);
					fscanf(pFile, "%f", &SetRot.z);
					fscanf(pFile, "%f", &SetSize.x);
					fscanf(pFile, "%f", &SetSize.y);
					fscanf(pFile, "%s", &TexName[0]);
					fscanf(pFile, "%s", &aSetMessage[0]);

					C2DUI* pUI = C2DUI::Create();
					pUI->SetPos(pos + SetPos);
					pUI->SetRot(SetRot);
					pUI->SetWidth(SetSize.x);
					pUI->SetHeight(SetSize.y);
					pUI->SetTexture(&TexName[0]);
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