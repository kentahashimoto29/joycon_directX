//============================================
//
//	�I�u�W�F�N�g�Q�[�W2D [objGauge2D.cpp]
//	Author:sakamoto kai
//
//============================================
#include "objGauge2D.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"

//====================================================================
//�R���X�g���N�^
//====================================================================
CObjGauge2D::CObjGauge2D(int nPriority) :CObject(nPriority)
{
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
	m_Width = 1.0f;
	m_Height = 1.0f;
	m_AddDarw = false;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Ratio = 1.0f;
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CObjGauge2D::~CObjGauge2D()
{

}

//====================================================================
//��������
//====================================================================
CObjGauge2D* CObjGauge2D::Create(int nPriority)
{
	CObjGauge2D* pObject2D = NULL;

	if (pObject2D == NULL)
	{
		//�I�u�W�F�N�g2D�̐���
		pObject2D = new CObjGauge2D(nPriority);
	}

	//�I�u�W�F�N�g�̏���������
	if (FAILED(pObject2D->Init()))
	{//���������������s�����ꍇ
		return NULL;
	}

	return pObject2D;
}

//====================================================================
//����������
//====================================================================
HRESULT CObjGauge2D::Init(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	SetTexture("data\\TEXTURE\\Test.jpg");

	SetType(TYPE_OBJECT2D);

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL)))
	{
		return E_FAIL;
	}

	VERTEX_2D* pVtx;	//���_�|�C���^������

	//���_�o�b�t�@�����b�N���A���X���ւ̃|�C���^������
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos.x = m_pos.x;
	pVtx[0].pos.y = m_pos.y;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = m_pos.x + m_Width * m_Ratio;
	pVtx[1].pos.y = m_pos.y;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = m_pos.x;
	pVtx[2].pos.y = m_pos.y + m_Height;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = m_pos.x + m_Width * m_Ratio;
	pVtx[3].pos.y = m_pos.y + m_Height;
	pVtx[3].pos.z = 0.0f;

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void CObjGauge2D::Uninit(void)
{
	SetDeathFlag(true);
}

//====================================================================
//�I������
//====================================================================
void CObjGauge2D::SetNULL(void)
{
	//���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//====================================================================
//�X�V����
//====================================================================
void CObjGauge2D::Update(void)
{
	SetVerTex();
}

//====================================================================
//�`�揈��
//====================================================================
void CObjGauge2D::Draw(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	if (m_AddDarw == true)
	{
		//a�u�����f�B���O�����Z�����ɐݒ�
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	////�e�N�X�`���̐ݒ�
	//pDevice->SetTexture(0, m_pTexture);

	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	assert(GetIdx() != -1);		//�e�N�X�`���̔ԍ������Y�ꂽ�ꍇ�G���[��f��

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, pTexture->GetAddress(GetIdx()));

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
		0,						//�`��̊J�n�ʒu
		2);						//�v���~�e�B�u(�|���S��)��

	if (m_AddDarw == true)
	{
		//a�u�����f�B���O��ʏ�ɐݒ�
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
}

//====================================================================
//�e�N�X�`���̊��蓖��
//====================================================================
void CObjGauge2D::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//====================================================================
//���_���W�̐ݒ�
//====================================================================
void CObjGauge2D::SetVerTex(void)
{
	VERTEX_2D* pVtx;	//���_�|�C���^������

	//���_�o�b�t�@�����b�N���A���X���ւ̃|�C���^������
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ� 
	pVtx[0].pos.x = m_pos.x;
	pVtx[0].pos.y = m_pos.y;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = m_pos.x + m_Width * m_Ratio;
	pVtx[1].pos.y = m_pos.y;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = m_pos.x;
	pVtx[2].pos.y = m_pos.y + m_Height;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = m_pos.x + m_Width * m_Ratio;
	pVtx[3].pos.y = m_pos.y + m_Height;
	pVtx[3].pos.z = 0.0f;

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//====================================================================
//�e�N�X�`�����W�̐ݒ�
//====================================================================
void CObjGauge2D::SetAnim(D3DXVECTOR2 Tex)
{
	VERTEX_2D* pVtx;	//���_�|�C���^������

	//���_�o�b�t�@�����b�N���A���X���ւ̃|�C���^������
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(Tex.x, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, Tex.y);
	pVtx[3].tex = D3DXVECTOR2(Tex.x, Tex.y);

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//====================================================================
//�e�N�X�`�����W�̐ݒ�
//====================================================================
void CObjGauge2D::SetScroll(D3DXVECTOR2 Tex)
{
	VERTEX_2D* pVtx;	//���_�|�C���^������

					//���_�o�b�t�@�����b�N���A���X���ւ̃|�C���^������
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(Tex.x, Tex.y);
	pVtx[1].tex = D3DXVECTOR2(Tex.x + 1.0f, Tex.y);
	pVtx[2].tex = D3DXVECTOR2(Tex.x, Tex.y + 1.0f);
	pVtx[3].tex = D3DXVECTOR2(Tex.x + 1.0f, Tex.y + 1.0f);

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//====================================================================
//���_�J���[�̐ݒ�
//====================================================================
void CObjGauge2D::SetColor(D3DXCOLOR col)
{
	VERTEX_2D* pVtx;	//���_�|�C���^������

	//���_�o�b�t�@�����b�N���A���X���ւ̃|�C���^������
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_�J���[�̐ݒ�
	pVtx[0].col =
		pVtx[1].col =
		pVtx[2].col =
		pVtx[3].col = col;

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//====================================================================
//���_�J���[�̐ݒ�
//====================================================================
void CObjGauge2D::SetTexture(const char* name)
{
	CTexture* pTexture = CManager::GetInstance()->GetTexture();
	m_nIdxTexture = pTexture->Regist(name);
}

//====================================================================
//���_�J���[�̐ݒ�
//====================================================================
void CObjGauge2D::SetGaugeWidth(float nCMax, float nCMeter)
{
	m_Ratio = (float)nCMeter / nCMax;
}