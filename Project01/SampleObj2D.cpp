//============================================
//
//	�I�u�W�F�N�g2D�̃T���v�� [SampleObj2D.cpp]
//	Author:sakamoto kai
//
//============================================
#include "SampleObj2D.h"
#include "renderer.h"
#include "manager.h"

//�}�N����`
#define NUMBER_WIGHT (100.0f)		//����
#define NUMBER_HEIGHT (100.0f)		//�c��

//====================================================================
//�R���X�g���N�^
//====================================================================
CSampleObj2D::CSampleObj2D(int nPriority) : CObject2D(nPriority)
{
	SetWidth(NUMBER_WIGHT);
	SetHeight(NUMBER_HEIGHT);
	m_nSample = 0;
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CSampleObj2D::~CSampleObj2D()
{

}

//====================================================================
//��������
//====================================================================
CSampleObj2D* CSampleObj2D::Create(int nPriority)
{
	CSampleObj2D* pSample2D = NULL;

	if (pSample2D == NULL)
	{
		//�I�u�W�F�N�g2D�̐���
		pSample2D = new CSampleObj2D(nPriority);
	}

	//�I�u�W�F�N�g�̏���������
	if (FAILED(pSample2D->Init()))
	{//���������������s�����ꍇ
		return NULL;
	}

	return pSample2D;
}

//====================================================================
//����������
//====================================================================
HRESULT CSampleObj2D::Init(void)
{
	CObject2D::Init();

	//�e�N�X�`���ݒ�
	SetTexture("data\\TEXTURE\\Number01.png");

	//�V����cpp���쐬�������͐V����TYPE��񋓂ɒǉ����Ďw�肷�邱��
	SetType(CObject::TYPE_SAMPLE);

	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void CSampleObj2D::Uninit(void)
{
	CObject2D::Uninit();
}

//====================================================================
//�X�V����
//====================================================================
void CSampleObj2D::Update(void)
{
	//���_���̍X�V
	CObject2D::Update();


	//----���_����ύX�������ꍇ�͈ȉ��̃R�[�h���g�p���邱��-----
	//VERTEX_2D* pVtx;	//���_�|�C���^������

	////���_�o�b�t�@�����b�N���A���X���ւ̃|�C���^������
	//GetVtxBuff()->Lock(0, 0, (void**)&pVtx, 0);

	////�ʒu
	//pVtx[0].pos;
	//pVtx[1].pos;
	//pVtx[2].pos;
	//pVtx[3].pos;

	////�F
	//pVtx[0].col;
	//pVtx[1].col;
	//pVtx[2].col;
	//pVtx[3].col;

	////�e�N�X�`��
	//pVtx[0].tex;
	//pVtx[1].tex;
	//pVtx[2].tex;
	//pVtx[3].tex;

	////���_�o�b�t�@���A�����b�N����
	//GetVtxBuff()->Unlock();
}

//====================================================================
//�`�揈��
//====================================================================
void CSampleObj2D::Draw(void)
{
	CObject2D::Draw();
}