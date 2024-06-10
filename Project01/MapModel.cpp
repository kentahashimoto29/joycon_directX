//============================================
//
//	�}�b�v���f�� [MapModel.cpp]
//	Author:sakamoto kai
//
//============================================
#include "MapModel.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "XModel.h"
#include "CubeBlock.h"

//�}�N����`
//====================================================================
//�R���X�g���N�^
//====================================================================
CMapModel::CMapModel(int nPriority) : CObjectX(nPriority)
{
	SetPos(INITVECTOR3);
	m_CollisionPos = INITVECTOR3;
	m_bCollision = true;
	m_State = STATE_NORMAL;
	m_nStateCount = 0;
	m_Scaling = 1.0f;
	m_fColorA = 0.0f;
	m_pDebugBlock = nullptr;		//�f�o�b�O�p�u���b�N
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CMapModel::~CMapModel()
{

}

//====================================================================
//��������
//====================================================================
CMapModel* CMapModel::Create(char* pModelName)
{
	CMapModel* pSample = NULL;

	if (pSample == NULL)
	{
		//�I�u�W�F�N�g2D�̐���
		pSample = new CMapModel();
	}

	//�I�u�W�F�N�g�̏���������
	if (FAILED(pSample->Init(pModelName)))
	{//���������������s�����ꍇ
		return NULL;
	}

	return pSample;
}

//====================================================================
//����������
//====================================================================
HRESULT CMapModel::Init(char* pModelName)
{
	SetType(CObject::TYPE_MAPMODEL);

	CObjectX::Init(pModelName);

	//���[�h���Ƃɏ����l��ݒ�o����
	switch (CScene::GetMode())
	{
	case CScene::MODE_TITLE:
		break;

	case CScene::MODE_GAME:
		break;

	case CScene::MODE_RESULT:
		break;
	}


	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void CMapModel::Uninit(void)
{
	CObjectX::Uninit();
}

//====================================================================
//�X�V����
//====================================================================
void CMapModel::Update(void)
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
void CMapModel::TitleUpdate(void)
{
	D3DXVECTOR3 pos = GetPos();

	//�ʒu�X�V
	pos += m_move;

	SetPos(pos);

	//���_���̍X�V
	CObjectX::Update();
}

//====================================================================
//�Q�[���ł̍X�V����
//====================================================================
void CMapModel::GameUpdate(void)
{
	D3DXVECTOR3 pos = GetPos();

	//�X�V�O�̈ʒu���ߋ��̈ʒu�Ƃ���
	m_posOld = pos;

	//�ʒu�X�V
	CObjectX::SetPos(pos);

	//��ʊO����
	if (pos.y < 0.0f)
	{

	}

	//�傫���̐ݒ�
	SetScaling(D3DXVECTOR3(m_Scaling, m_Scaling, m_Scaling));

	//��ԊǗ�
	StateManager();

	//���_���̍X�V
	CObjectX::Update();
}

//====================================================================
//�`�揈��
//====================================================================
void CMapModel::Draw(void)
{
	CObjectX::Draw();
}

//====================================================================
//��ԊǗ�
//====================================================================
void CMapModel::StateManager(void)
{
	switch (m_State)
	{
	case STATE_NORMAL:
		break;
	case STATE_ACTION:
		break;
	}

	if (m_nStateCount > 0)
	{
		m_nStateCount--;
	}
}

//====================================================================
//�u���b�N�̐���
//====================================================================
void CMapModel::CreateBlock(void)
{
	if (m_pDebugBlock == nullptr)
	{
		m_pDebugBlock = CCubeBlock::Create();
		m_pDebugBlock->SetPos(GetPos());
		m_pDebugBlock->SetSize(GetSize());

		if (m_bCollision == true)
		{
			m_pDebugBlock->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f));
		}
		else
		{
			m_pDebugBlock->SetColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.5f));
		}
	}
}

//====================================================================
//�u���b�N�̍X�V
//====================================================================
void CMapModel::UpdateBlock(void)
{
	if (m_pDebugBlock != nullptr)
	{
		m_pDebugBlock->SetPos(GetPos());
		m_pDebugBlock->SetSize(GetSize());

		if (m_bCollision == true)
		{
			m_pDebugBlock->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f));
		}
		else
		{
			m_pDebugBlock->SetColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.5f));
		}
	}
}