//============================================
//
//	�I�u�W�F�N�g3D�̃T���v�� [SampleObj3D.cpp]
//	Author:sakamoto kai
//
//============================================
#include "SampleObj3D.h"
#include "renderer.h"
#include "manager.h"

//�}�N����`
#define NUMBER_WIGHT (100.0f)		//����
#define NUMBER_HEIGHT (100.0f)		//�c��

//====================================================================
//�R���X�g���N�^
//====================================================================
CSampleObj3D::CSampleObj3D(int nPriority) : CObject3D(nPriority)
{
	SetWidth(NUMBER_WIGHT);
	SetHeight(NUMBER_HEIGHT);
	m_nSample = 0;
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CSampleObj3D::~CSampleObj3D()
{

}

//====================================================================
//��������
//====================================================================
CSampleObj3D* CSampleObj3D::Create(void)
{
	CSampleObj3D* pSample3D = NULL;

	if (pSample3D == NULL)
	{
		//�I�u�W�F�N�g2D�̐���
		pSample3D = new CSampleObj3D();
	}

	//�I�u�W�F�N�g�̏���������
	if (FAILED(pSample3D->Init()))
	{//���������������s�����ꍇ
		return NULL;
	}

	return pSample3D;
}

//====================================================================
//����������
//====================================================================
HRESULT CSampleObj3D::Init(void)
{
	CObject3D::Init();

	//�e�N�X�`���ݒ�
	SetTexture("data\\TEXTURE\\Number01.png");

	//�V����cpp���쐬�������͐V����TYPE��񋓂ɒǉ����Ďw�肷�邱��
	SetType(CObject::TYPE_SAMPLE);

	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void CSampleObj3D::Uninit(void)
{
	CObject3D::Uninit();
}

//====================================================================
//�X�V����
//====================================================================
void CSampleObj3D::Update(void)
{
	//���_���̍X�V
	CObject3D::Update();

	//----���_����ύX�������ꍇ�͈ȉ��̃R�[�h���g�p���邱��---- -
	//VERTEX_3D * pVtx;	//���_�|�C���^������

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

	////�@��
	//pVtx[0].nor;
	//pVtx[1].nor;
	//pVtx[2].nor;
	//pVtx[3].nor;

	////���_�o�b�t�@���A�����b�N����
	//GetVtxBuff()->Unlock();
}

//====================================================================
//�`�揈��
//====================================================================
void CSampleObj3D::Draw(void)
{
	CObject3D::Draw();
}