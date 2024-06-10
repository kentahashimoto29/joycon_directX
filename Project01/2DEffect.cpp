//============================================
//
//	�G�t�F�N�g���� [2DEffect.cpp]
//	Author:sakamoto kai
//
//============================================
#include "2DEffect.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"

//�}�N����`
#define SMALLER (2.0f)

//�ÓI�����o�ϐ��錾
LPDIRECT3DTEXTURE9 C2DEffect::m_pTexture = NULL;

//====================================================================
//�R���X�g���N�^
//====================================================================
C2DEffect::C2DEffect(int nPriority) : CObject2D(nPriority)
{
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_fRadius = 10.0f;
	m_nLife = 10;
	m_nLifeMax = m_nLife;
	m_fDel = SMALLER;
	m_cTexName = "";
	m_nIdxTexture = -1;
}

//====================================================================
//�f�X�g���N�^
//====================================================================
C2DEffect::~C2DEffect()
{

}

//====================================================================
//��������
//====================================================================
C2DEffect* C2DEffect::Create()
{
	C2DEffect* pPlayer = NULL;

	if (pPlayer == NULL)
	{
		//�I�u�W�F�N�g2D�̐���
		pPlayer = new C2DEffect();
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
HRESULT C2DEffect::Init(void)
{
	CTexture* pTexture = CManager::GetInstance()->GetTexture();;
	m_nIdxTexture = pTexture->Regist("data\\TEXTURE\\Effect000.jpg");
	m_cTexName = "data\\TEXTURE\\Effect000.jpg";

	SetType(CObject::TYPE_EFFECT);

	CObject2D::Init();

	//���_�J���[�̐ݒ�
	SetColor(m_col);

	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void C2DEffect::Uninit(void)
{
	CObject2D::Uninit();
}

//====================================================================
//�X�V����
//====================================================================
void C2DEffect::Update(void)
{
	//�T�C�Y�̒���
	m_fRadius -= m_fDel;
	SetWidth(m_fRadius);
	SetHeight(m_fRadius);

	m_nLife--;

	//�ʒu�X�V
	SetPos(GetPos() += m_move);

	//�s�����x�̐ݒ�
	SetColorA((float)m_nLife / (float)m_nLifeMax);

	if (m_nLife <= 0 || m_fRadius <= 0.0f)
	{
		Uninit();
		return;
	}

	//���_���̍X�V
	CObject2D::Update();

	CTexture* pTexture = CManager::GetInstance()->GetTexture();;
	m_nIdxTexture = pTexture->Regist(m_cTexName);
}

//====================================================================
//�`�揈��
//====================================================================
void C2DEffect::Draw(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//a�u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	CObject2D::Draw();

	//a�u�����f�B���O��ʏ�ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}