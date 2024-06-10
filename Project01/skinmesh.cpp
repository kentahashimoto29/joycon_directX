//============================================
//
//	3D�|���S���ŃX�L�����b�V�����s�� [skinmesh.cpp]
//	Author:sakamoto kai
//
//============================================
#include "skinmesh.h"
#include "input.h"
#include "renderer.h"
#include "manager.h"
#include "game.h"
#include "texture.h"
#include "CubeBlock.h"

#define POLYDON_SIZE (10.0f)

//====================================================================
//�R���X�g���N�^
//====================================================================
CSkinmesh::CSkinmesh(int nPriority) :CObject(nPriority)
{
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
	m_Width = 50.0f;
	m_Height = 150.0f;
	m_pos = D3DXVECTOR3(0.0f, 100.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_AddDarw = false;
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CSkinmesh::~CSkinmesh()
{

}

//====================================================================
//��������
//====================================================================
CSkinmesh* CSkinmesh::Create(void)
{
	CSkinmesh* pObject3D = NULL;

	if (pObject3D == NULL)
	{
		//�I�u�W�F�N�g3D�̐���
		pObject3D = new CSkinmesh();
	}

	//�I�u�W�F�N�g�̏���������
	if (FAILED(pObject3D->Init()))
	{//���������������s�����ꍇ
		return NULL;
	}

	return pObject3D;
}

//====================================================================
//����������
//====================================================================
HRESULT CSkinmesh::Init(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X

	LoadData("data\\TXT\\skinmesh.txt");

	//���_�o�b�t�@�̐���
	if (FAILED(pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nNumVertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL)))
	{
		return E_FAIL;
	}

	VERTEX_3D* pVtx;	//���_�|�C���^������

	//���_�o�b�t�@�����b�N���A���X���ւ̃|�C���^������
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < m_nNumVertex; nCnt++)
	{

		//���_���W�̐ݒ� 
		pVtx[nCnt].pos.x = m_Vertex[nCnt].fPosX;
		pVtx[nCnt].pos.y = m_Vertex[nCnt].fPosY;
		pVtx[nCnt].pos.z = m_Vertex[nCnt].fPosZ;

		//�@���x�N�g���̐ݒ�
		pVtx[nCnt].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		//���_�J���[�̐ݒ�
		pVtx[nCnt].col = D3DXCOLOR(m_Vertex[nCnt].WeightValue[1], 0.0f, m_Vertex[nCnt].WeightValue[0], 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[nCnt].tex = m_Vertex[nCnt].Tex;
	}

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//�ʒu�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//�{�[���̏����ݒ�
	for (int nCnt = 0; nCnt < m_nNumBone; nCnt++)
	{
		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&m_Bone[nCnt].m_mtxWorld);

		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Bone[nCnt].fRotY, m_Bone[nCnt].fRotX, m_Bone[nCnt].fRotZ);
		D3DXMatrixMultiply(&m_Bone[nCnt].m_mtxWorld, &m_Bone[nCnt].m_mtxWorld, &mtxRot);

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, m_Bone[nCnt].fPosX, m_Bone[nCnt].fPosY, m_Bone[nCnt].fPosZ);
		D3DXMatrixMultiply(&m_Bone[nCnt].m_mtxWorld, &m_Bone[nCnt].m_mtxWorld, &mtxTrans);

		D3DXMATRIX mtxParent;

		if (m_Bone[nCnt].nParent != -1)
		{//�e�����݂���
			mtxParent = GetBoneMtxWorld(m_Bone[nCnt].nParent);
		}
		else
		{//�e�����݂��Ȃ�
			//����(�ŐV)�̃}�g���b�N�X���擾����
			mtxParent = m_mtxWorld;
		}

		//�Z�o�����}�g���N�X���������킹��
		D3DXMatrixMultiply(&m_Bone[nCnt].m_mtxWorld,
			&m_Bone[nCnt].m_mtxWorld,
			&mtxParent);

		////�Z�o�����}�g���N�X���������킹��
		//D3DXMatrixMultiply(&m_mtxWorld,
		//	&m_mtxWorld,
		//	&mtxParent);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &m_Bone[nCnt].m_mtxWorld);


		m_pCubeBlock[nCnt] = CCubeBlock::Create();
		m_pCubeBlock[nCnt]->SetPos(D3DXVECTOR3(m_Bone[nCnt].m_mtxWorld._41, m_Bone[nCnt].m_mtxWorld._42, m_Bone[nCnt].m_mtxWorld._43));
		m_pCubeBlock[nCnt]->SetRot(D3DXVECTOR3(m_Bone[nCnt].fRotX, m_Bone[nCnt].fRotY, m_Bone[nCnt].fRotZ));
		m_pCubeBlock[nCnt]->SetSize(D3DXVECTOR3(10.0f, 10.0f, 10.0f));
	}

	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void CSkinmesh::Uninit(void)
{
	SetDeathFlag(true);
}

//====================================================================
//�I������
//====================================================================
void CSkinmesh::SetNULL(void)
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
void CSkinmesh::Update(void)
{
	//SetVerTex();

	WaitControl();
}

//====================================================================
//�`�揈��
//====================================================================
void CSkinmesh::Draw(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X

	if (m_AddDarw == true)
	{
		//a�u�����f�B���O�����Z�����ɐݒ�
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//�ʒu�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);

	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	assert(GetIdx() != -1);		//�e�N�X�`���̔ԍ������Y�ꂽ�ꍇ�G���[��f��

								//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, pTexture->GetAddress(GetIdx()));

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
		0,										//�v���~�e�B�u(�|���S��)��
		m_nNumVertex - 2);

	////�|���S���̕`��
	//pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
	//	0,
	//	0,
	//	0,//�p�ӂ������_�̐�
	//	0,
	//	m_nNumVertex - 2);//�`�悷��v���~�e�B�u�̐�

	if (m_AddDarw == true)
	{
		//a�u�����f�B���O��ʏ�ɐݒ�
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
}

//====================================================================
//�X�L�����b�V���̃E�F�C�g�l���������_�𓮂���
//====================================================================
void CSkinmesh::WaitControl()
{
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	D3DXMATRIX mtxParent;

	if (pInputKeyboard->GetPress(DIK_Q) == true)
	{
		m_Bone[0].fRotZ += 0.02f;
	}
	if (pInputKeyboard->GetPress(DIK_E) == true)
	{
		m_Bone[0].fRotZ -= 0.02f;
	}

	for (int nCnt = 0; nCnt < m_nNumBone; nCnt++)
	{
		m_pCubeBlock[nCnt]->SetRot(D3DXVECTOR3(m_Bone[nCnt].fRotX, m_Bone[nCnt].fRotY, m_Bone[nCnt].fRotZ));
	}


	VERTEX_3D* pVtx;	//���_�|�C���^������

	//���_�o�b�t�@�����b�N���A���X���ւ̃|�C���^������
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntVtx = 0; nCntVtx < m_nNumVertex; nCntVtx++)
	{
		//���_���W�̐ݒ� 
		pVtx[nCntVtx].pos.x = m_Vertex[nCntVtx].fPosX;
		pVtx[nCntVtx].pos.y = m_Vertex[nCntVtx].fPosY;
		pVtx[nCntVtx].pos.z = m_Vertex[nCntVtx].fPosZ;
	}

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//====================================================================
//�e�N�X�`���̊��蓖��
//====================================================================
void CSkinmesh::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;
}

//====================================================================
//���_���W�̐ݒ�
//====================================================================
void CSkinmesh::SetVerPos(D3DXVECTOR3 Pos0, D3DXVECTOR3 Pos1, D3DXVECTOR3 Pos2, D3DXVECTOR3 Pos3)
{
	VERTEX_3D* pVtx;	//���_�|�C���^������

	//���_�o�b�t�@�����b�N���A���X���ւ̃|�C���^������
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ� 
	pVtx[0].pos = Pos0;
	pVtx[1].pos = Pos1;
	pVtx[2].pos = Pos2;
	pVtx[3].pos = Pos3;

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//====================================================================
//���_���W�̐ݒ�
//====================================================================
void CSkinmesh::SetVerTex(void)
{
	VERTEX_3D* pVtx;	//���_�|�C���^������

	//���_�o�b�t�@�����b�N���A���X���ւ̃|�C���^������
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ� 
	pVtx[0].pos = D3DXVECTOR3(-m_Width * 0.5f, +m_Height * 0.5f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(+m_Width * 0.5f, +m_Height * 0.5f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-m_Width * 0.5f, -m_Height * 0.5f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(+m_Width * 0.5f, -m_Height * 0.5f, 0.0f);

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//====================================================================
//�e�N�X�`�����W�̐ݒ�
//====================================================================
void CSkinmesh::SetAnim(D3DXVECTOR2 Tex)
{
	VERTEX_3D* pVtx;	//���_�|�C���^������

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
//���_�J���[�̐ݒ�
//====================================================================
void CSkinmesh::SetColor(D3DXCOLOR col)
{
	m_Color = col;

	VERTEX_3D* pVtx;	//���_�|�C���^������

					//���_�o�b�t�@�����b�N���A���X���ւ̃|�C���^������
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_�J���[�̐ݒ�
	pVtx[0].col = m_Color;
	pVtx[1].col = m_Color;
	pVtx[2].col = m_Color;
	pVtx[3].col = m_Color;

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//====================================================================
//���_�J���[�̐ݒ�
//====================================================================
void CSkinmesh::SetTexture(const char* name)
{
	CTexture* pTexture = CManager::GetInstance()->GetTexture();
	m_nIdxTexture = pTexture->Regist(name);
}

//====================================================================
//���[�V�����̃��[�h����
//====================================================================
void CSkinmesh::LoadData(const char* pFilename)
{
	FILE* pFile; //�t�@�C���|�C���^��錾

	//�t�@�C�����J��
	pFile = fopen(pFilename, "r");

	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ

		int nCntBone = 0;
		int nCntVertex = 0;
		int nCntModel = 0;
		int nCntParts = 0;
		int nCntMotion = 0;
		int nCntKeySet = 0;
		int nCntKey = 0;

		char aString[128] = {};				//�S�~��
		char aMessage[128] = {};			//�X�^�[�g���b�Z�[�W
		char aBool[128] = {};				//bool�ϊ��p���b�Z�[�W
		char aTextureName[128] = {};		//�e�N�X�`���̃p�X��

		// �ǂݍ��݊J�n-----------------------------------------------------
		while (1)
		{//�uSCRIPT�v��T��
			fscanf(pFile, "%s", &aMessage[0]);
			if (strcmp(&aMessage[0], "SCRIPT") == 0)
			{
				// �e�N�X�`�����ǂݍ���-----------------------------------------------------
				while (1)
				{//�uTEXTURE_FILENAME�v��T��
					fscanf(pFile, "%s", &aMessage[0]);
					if (strcmp(&aMessage[0], "TEXTURE_FILENAME") == 0)
					{
						fscanf(pFile, "%s", &aString[0]);
						fscanf(pFile, "%s", &aTextureName[0]);
						SetTexture(&aTextureName[0]);
						break;
					}
				}

				// �{�[�����ǂݍ���-----------------------------------------------------
				while (1)
				{//�uBONESET�v��T��
					fscanf(pFile, "%s", &aMessage[0]);
					if (strcmp(&aMessage[0], "BONESET") == 0)
					{
						while (1)
						{//�e��ϐ���T��
							fscanf(pFile, "%s", &aMessage[0]);
							if (strcmp(&aMessage[0], "NUM_BONE") == 0)
							{
								fscanf(pFile, "%s", &aString[0]);
								fscanf(pFile, "%d", &m_nNumBone);	//�{�[���̑�����ݒ�
								break;
							}
						}

						while (m_nNumBone != nCntBone)
						{//�e��ϐ���T��
							fscanf(pFile, "%s", &aMessage[0]);
							if (strcmp(&aMessage[0], "BONE") == 0)
							{
								while (1)
								{//�e��ϐ���T��
									fscanf(pFile, "%s", &aMessage[0]);
									if (strcmp(&aMessage[0], "PARENT") == 0)
									{
										fscanf(pFile, "%s", &aString[0]);
										fscanf(pFile, "%d", &m_Bone[nCntBone].nParent);	//�e��ݒ�
									}
									if (strcmp(&aMessage[0], "POS") == 0)
									{
										fscanf(pFile, "%s", &aString[0]);
										fscanf(pFile, "%f", &m_Bone[nCntBone].fPosX);	//�ʒu��ݒ�
										fscanf(pFile, "%f", &m_Bone[nCntBone].fPosY);	//�ʒu��ݒ�
										fscanf(pFile, "%f", &m_Bone[nCntBone].fPosZ);	//�ʒu��ݒ�
									}
									if (strcmp(&aMessage[0], "ROT") == 0)
									{
										fscanf(pFile, "%s", &aString[0]);
										fscanf(pFile, "%f", &m_Bone[nCntBone].fRotX);	//������ݒ�
										fscanf(pFile, "%f", &m_Bone[nCntBone].fRotY);	//������ݒ�
										fscanf(pFile, "%f", &m_Bone[nCntBone].fRotZ);	//������ݒ�
									}
									if (strcmp(&aMessage[0], "END_BONE") == 0)
									{
										nCntBone++;
										break;
									}
								}
							}
						}
					}
					if (strcmp(&aMessage[0], "END_BONESET") == 0)
					{
						break;
					}
				}

				// ���_���ǂݍ���-----------------------------------------------------
				while (1)
				{//�uVERTEXSET�v��T��
					fscanf(pFile, "%s", &aMessage[0]);
					if (strcmp(&aMessage[0], "VERTEXSET") == 0)
					{
						while (1)
						{//�e��ϐ���T��
							fscanf(pFile, "%s", &aMessage[0]);
							if (strcmp(&aMessage[0], "NUM_VERTEX") == 0)
							{
								fscanf(pFile, "%s", &aString[0]);
								fscanf(pFile, "%d", &m_nNumVertex);	//���_�̑�����ݒ�
								break;
							}
						}

						while (m_nNumVertex != nCntVertex)
						{//�e��ϐ���T��
							fscanf(pFile, "%s", &aMessage[0]);
							if (strcmp(&aMessage[0], "VERTEX") == 0)
							{
								while (1)
								{
									fscanf(pFile, "%s", &aMessage[0]);
									if (strcmp(&aMessage[0], "POS") == 0)
									{
										fscanf(pFile, "%s", &aString[0]);
										fscanf(pFile, "%f", &m_Vertex[nCntVertex].fPosX);	//�ʒu��ݒ�
										fscanf(pFile, "%f", &m_Vertex[nCntVertex].fPosY);	//�ʒu��ݒ�
										fscanf(pFile, "%f", &m_Vertex[nCntVertex].fPosZ);	//�ʒu��ݒ�
									}
									if (strcmp(&aMessage[0], "BONE_REF") == 0)
									{
										fscanf(pFile, "%s", &aString[0]);
										fscanf(pFile, "%d", &m_Vertex[nCntVertex].BoneRef[0]);	//�Ή��{�[����ݒ�
										fscanf(pFile, "%d", &m_Vertex[nCntVertex].BoneRef[1]);	//�Ή��{�[����ݒ�
									}
									if (strcmp(&aMessage[0], "WEIGHT_VALUE") == 0)
									{
										fscanf(pFile, "%s", &aString[0]);
										fscanf(pFile, "%f", &m_Vertex[nCntVertex].WeightValue[0]);	//�E�F�C�g�l��ݒ�
										fscanf(pFile, "%f", &m_Vertex[nCntVertex].WeightValue[1]);	//�E�F�C�g�l��ݒ�
									}
									if (strcmp(&aMessage[0], "TEX") == 0)
									{
										fscanf(pFile, "%s", &aString[0]);
										fscanf(pFile, "%f", &m_Vertex[nCntVertex].Tex.x);	//�e�N�X�`����ݒ�
										fscanf(pFile, "%f", &m_Vertex[nCntVertex].Tex.y);	//�e�N�X�`����ݒ�
									}
									if (strcmp(&aMessage[0], "END_VERTEX") == 0)
									{
										nCntVertex++;
										break;
									}
								}
							}
						}
					}
					if (strcmp(&aMessage[0], "END_VERTEXSET") == 0)
					{
						break;
					}
				}


				// ���[�V�������ǂݍ���-----------------------------------------------------
				while (1)
				{//�uMOTIONSET�v��T��
					fscanf(pFile, "%s", &aMessage[0]);
					if (strcmp(&aMessage[0], "MOTIONSET") == 0)
					{
						while (1)
						{//�e��ϐ���T��
							fscanf(pFile, "%s", &aMessage[0]);
							if (strcmp(&aMessage[0], "LOOP") == 0)
							{
								fscanf(pFile, "%s", &aString[0]);
								fscanf(pFile, "%s", &aBool[0]);	//���[�v���邩�ǂ�����ݒ�
								m_aInfo[nCntMotion].bLoop = (strcmp(&aBool[0], "1") == 0 ? true : false);			//bool�^�̏�����
							}
							if (strcmp(&aMessage[0], "NUM_KEY") == 0)
							{
								fscanf(pFile, "%s", &aString[0]);
								fscanf(pFile, "%d", &m_aInfo[nCntMotion].NumKey);	//�L�[�̑�����ݒ�
								break;
							}
						}

						// �L�[���ǂݍ���-----------------------------------------------------
						while (1)
						{//�uKEYSET�v��T��
							fscanf(pFile, "%s", &aMessage[0]);
							if (strcmp(&aMessage[0], "KEYSET") == 0)
							{
								while (1)
								{//�uFRAME�v��T��
									fscanf(pFile, "%s", &aMessage[0]);
									if (strcmp(&aMessage[0], "FRAME") == 0)
									{
										fscanf(pFile, "%s", &aString[0]);
										fscanf(pFile, "%d", &m_aInfo[nCntMotion].aKeyInfo[nCntKeySet].nFrame);	//�L�[�t���[����ݒ�
										break;
									}
								}

								while (1)
								{//�uKEY�v��T��
									fscanf(pFile, "%s", &aMessage[0]);
									if (strcmp(&aMessage[0], "KEY") == 0)
									{
										while (1)
										{//�e��ϐ���T��
											fscanf(pFile, "%s", &aMessage[0]);
											if (strcmp(&aMessage[0], "POS") == 0)
											{
												fscanf(pFile, "%s", &aString[0]);
												fscanf(pFile, "%f", &m_aInfo[nCntMotion].aKeyInfo[nCntKeySet].aKey[nCntKey].fPosX);	//�ʒu��ݒ�
												fscanf(pFile, "%f", &m_aInfo[nCntMotion].aKeyInfo[nCntKeySet].aKey[nCntKey].fPosY);	//�ʒu��ݒ�
												fscanf(pFile, "%f", &m_aInfo[nCntMotion].aKeyInfo[nCntKeySet].aKey[nCntKey].fPosZ);	//�ʒu��ݒ�
											}
											if (strcmp(&aMessage[0], "ROT") == 0)
											{
												fscanf(pFile, "%s", &aString[0]);
												fscanf(pFile, "%f", &m_aInfo[nCntMotion].aKeyInfo[nCntKeySet].aKey[nCntKey].fRotX);	//�ʒu��ݒ�
												fscanf(pFile, "%f", &m_aInfo[nCntMotion].aKeyInfo[nCntKeySet].aKey[nCntKey].fRotY);	//�ʒu��ݒ�
												fscanf(pFile, "%f", &m_aInfo[nCntMotion].aKeyInfo[nCntKeySet].aKey[nCntKey].fRotZ);	//�ʒu��ݒ�
												break;
											}
										}
									}
									if (strcmp(&aMessage[0], "END_KEY") == 0)
									{
										nCntKey++;
										if (nCntKey >= nCntBone)
										{
											break;
										}
									}
								}
							}
							if (strcmp(&aMessage[0], "END_KEYSET") == 0)
							{
								nCntKey = 0;
								nCntKeySet++;
								if (nCntKeySet >= m_aInfo[nCntMotion].NumKey)
								{
									break;
								}
							}
						}
					}
					if (strcmp(&aMessage[0], "END_MOTIONSET") == 0)
					{
						nCntKeySet = 0;
						nCntMotion++;
					}
					else if (strcmp(&aMessage[0], "END_SCRIPT") == 0)
					{
						break;
					}
				}
			}
			if (strcmp(&aMessage[0], "END_SCRIPT") == 0)
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