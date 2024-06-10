//============================================
//
//	�����_���[[renderer.cpp]
//	Author:sakamoto kai
//
//============================================
#include "renderer.h"
#include "object.h"
#include "debugproc.h"
#include "manager.h"
#include "camera.h"
#include "fade.h"
#include "Pause.h"

//====================================================================
//�R���X�g���N�^
//====================================================================
CRenderer::CRenderer()
{
	m_pD3D = NULL;					//Direct3D�I�u�W�F�N�g�ւ̃|�C���^
	m_pD3DDevice = NULL;			//Direct3D�f�o�C�X�ւ̃|�C���^
	m_bAfterimage = false;
}

//====================================================================
//�f�X�g���N�^
//====================================================================
CRenderer::~CRenderer()
{

}

//====================================================================
//����������
//====================================================================
HRESULT CRenderer::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;			//�f�B�X�v���C���[�h
	D3DPRESENT_PARAMETERS d3dpp;	//�v���[���e�[�V�����p�����[�^

	//Direct3D�I�u�W�F�N�g�̐���
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == NULL)
	{
		return E_FAIL;
	}

	//���݂̃f�B�X�v���C���[�h������
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//�f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));	//�p�����[�^�̃[���N���A

	d3dpp.BackBufferWidth = SCREEN_WIDTH;							//�Q�[����ʂ̃T�C�Y(��)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;							//�Q�[����ʂ̃T�C�Y(����)
	d3dpp.BackBufferFormat = d3ddm.Format;							//�o�b�N�o�b�t�@�̌`��
	d3dpp.BackBufferCount = 1;										//�o�b�N�o�b�t�@�̐�
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;						//�_�u���o�b�t�@�̐؂�ւ�(�f���M���ɓ���)
	d3dpp.EnableAutoDepthStencil = TRUE;							//�f�v�X�o�b�t�@�ƃV�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;						//�f�v�X�o�b�t�@��16bit�Ƃ��Ĉ���
	d3dpp.Windowed = bWindow;										//�E�C���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	//���t���b�V�����[�g
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		//�C���^�[�o��

	//Direct3d�f�o�C�X�̍쐬
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&m_pD3DDevice)))
	{
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&m_pD3DDevice)))
		{
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&m_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}
	//�����_�[�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//�T���v���[�X�e�[�g(�e�N�X�`���̊g�k�⊮)�̐ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	//�e�N�X�`���X�e�[�W�X�e�[�g(�e�N�X�`���̃A���t�@�u�����h)�̐ݒ�
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	LPDIRECT3DSURFACE9 pRenderDef, pZBuffDef;

	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		//�����_�����O�^�[�Q�b�g�p�e�N�X�`���̐���
		m_pD3DDevice->CreateTexture(
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			1,
			D3DUSAGE_RENDERTARGET,
			D3DFMT_A8R8G8B8,
			D3DPOOL_DEFAULT,
			&m_apTextureMT[nCnt],
			NULL);

		//�e�N�X�`�������_�����O�p�C���^�[�t�F�C�X�̐���
		m_apTextureMT[nCnt]->GetSurfaceLevel(0, &m_apRenderMT[nCnt]);

		//�e�N�X�`�������_�����O�pZ�o�b�t�@�̐���
		m_pD3DDevice->CreateDepthStencilSurface(
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			D3DFMT_D16,
			D3DMULTISAMPLE_NONE,
			0,
			TRUE,
			&m_pZBuffMT,
			NULL
		);

		//���݂̃����_�����O�^�[�Q�b�g���擾(�ۑ�)
		m_pD3DDevice->GetRenderTarget(0, &pRenderDef);

		//���݂�Z�o�b�t�@���擾(�ۑ�)
		m_pD3DDevice->GetDepthStencilSurface(&pZBuffDef);

		//���݂̃����_�����O�^�[�Q�b�g���擾(�ۑ�)
		m_pD3DDevice->SetRenderTarget(0, m_apRenderMT[nCnt]);

		//���݂�Z�o�b�t�@���擾(�ۑ�)
		m_pD3DDevice->SetDepthStencilSurface(m_pZBuffMT);

		//�����_�����O�^�[�Q�b�g�p�̃e�N�X�`���̃N���A
		m_pD3DDevice->Clear(0, NULL,
			(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
			D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

		//���݂̃����_�����O�^�[�Q�b�g���擾(�ۑ�)
		m_pD3DDevice->SetRenderTarget(0, pRenderDef);

		//���݂�Z�o�b�t�@���擾(�ۑ�)
		m_pD3DDevice->SetDepthStencilSurface(pZBuffDef);
	}

	//�e�N�X�`�������_�����O�p�r���[�|�[�g�̐ݒ�
	m_ViewportMT.X = 0;
	m_ViewportMT.Y = 0;
	m_ViewportMT.Width = SCREEN_WIDTH;
	m_ViewportMT.Height = SCREEN_HEIGHT;
	m_ViewportMT.MinZ = 0.0f;
	m_ViewportMT.MaxZ = 1.0f;

	//�t�B�[�h�o�b�N�p�|���S���̐���
	if (FAILED(m_pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuffMT,
		NULL)))
	{
		return E_FAIL;
	}

	VERTEX_2D* pVtx;	//���_�|�C���^������

	//���_�o�b�t�@�����b�N���A���X���ւ̃|�C���^������
	m_pVtxBuffMT->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ� 
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(1280.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 720.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(1280.0f, 720.0f, 0.0f);

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.99f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.99f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.99f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.99f);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuffMT->Unlock();

	return S_OK;
}

//====================================================================
//�I������
//====================================================================
void CRenderer::Uninit(void)
{
	//�f�o�b�N�\���p�t�H���g�̔j��
	if (m_pD3DDevice != NULL)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}

	//Direct3D�I�u�W�F�N�g�̔j��
	if (m_pD3D != NULL)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}

	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		//�����_�����O�^�[�Q�b�g�p�e�N�X�`���̔j��
		if (m_apTextureMT[nCnt] != NULL)
		{
			m_apTextureMT[nCnt]->Release();
			m_apTextureMT[nCnt] = NULL;
		}

		//�e�N�X�`�������_�����O�p�C���^�[�t�F�C�X�̔j��
		if (m_apRenderMT[nCnt] != NULL)
		{
			m_apRenderMT[nCnt]->Release();
			m_apRenderMT[nCnt] = NULL;
		}
	}

	//�e�N�X�`�������_�����O�pZ�o�b�t�@�̔j��
	if (m_pZBuffMT != NULL)
	{
		m_pZBuffMT->Release();
		m_pZBuffMT = NULL;
	}

	//�t�B�[�h�o�b�N�p�|���S���̔j��
	if (m_pVtxBuffMT != NULL)
	{
		m_pVtxBuffMT->Release();
		m_pVtxBuffMT = NULL;
	}
}

//====================================================================
//�X�V����
//====================================================================
void CRenderer::Update(void)
{
	//�S�ẴI�u�W�F�N�g2D�̍X�V����
	CObject::UpdateAll();
}

//====================================================================
//�`�揈��
//====================================================================
void CRenderer::Draw(void)
{
	//�f�o�b�O�\���̎擾
	CDebugProc* pDebugProc = CManager::GetInstance()->GetDebugProc();

	//�J�����̎擾
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	//�t�F�[�h�̎擾
	CFade* pFade = CManager::GetInstance()->GetFade();

	if (m_bAfterimage == true)
	{
		LPDIRECT3DSURFACE9 pRenderWk;
		LPDIRECT3DTEXTURE9 pTextureWk;

		//��ʃN���A(�o�b�N�o�b�t�@��Z�o�b�t�@�̃N���A)
		m_pD3DDevice->Clear(0, NULL,
			(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
			D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

		//-----------------------------------------------------------------------------------

		LPDIRECT3DSURFACE9 pRenderDef, pZBuffDef;
		D3DVIEWPORT9 viewportDef;
		D3DXMATRIX mtxViewDef, mtxProjectionDef;

		//���݂̃����_�����O�^�[�Q�b�g���擾(�ۑ�)
		m_pD3DDevice->GetRenderTarget(0, &pRenderDef);

		//���݂�Z�o�b�t�@���擾(�ۑ�)
		m_pD3DDevice->GetDepthStencilSurface(&pZBuffDef);

		//���݂̃r���[�|�[�g���擾(�ۑ�)
		m_pD3DDevice->GetViewport(&viewportDef);

		CManager::GetInstance()->GetRenderer()->ChageTarget(pCamera->GetPosV(), pCamera->GetPosR(), pCamera->GetVecU());

		//�����_�����O�^�[�Q�b�g�p�̃e�N�X�`���̃N���A
		m_pD3DDevice->Clear(0, NULL,
			(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
			D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

		//-----------------------------------------------------------------------------------

		//�`��J�n
		if (SUCCEEDED(m_pD3DDevice->BeginScene()))
		{//�`�揈�������������ꍇ

			//�J�����̐ݒ�
			pCamera->SetCamera();

			//�S�ẴI�u�W�F�N�g2D�̕`�揈��
			CObject::DrawAll();

			//-----------------------------------------------------------------------------------
			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			m_pD3DDevice->SetStreamSource(0, m_pVtxBuffMT, 0, sizeof(VERTEX_2D));

			//���_�t�H�[�}�b�g�̐ݒ�
			m_pD3DDevice->SetFVF(FVF_VERTEX_2D);

			//�e�N�X�`���̐ݒ�
			m_pD3DDevice->SetTexture(0, m_apTextureMT[1]);

			//�|���S���̕`��
			m_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
				0,						//�`��̊J�n�ʒu
				2);						//�v���~�e�B�u(�|���S��)��
			//-----------------------------------------------------------------------------------

			//�t�F�[�h�̍X�V����
			pFade->Draw();

			//�f�o�b�O�\���̕`�揈��
			pDebugProc->Draw();

			//�`��I��
			m_pD3DDevice->EndScene();
		}
		////�o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
		//m_pD3DDevice->Present(NULL, NULL, NULL, NULL);

		//-----------------------------------------------------------------------------------

		//���̃����_�����O�^�[�Q�b�g�ɖ߂�
		m_pD3DDevice->SetRenderTarget(0, pRenderDef);

		//����Z�o�b�t�@�ɖ߂�
		m_pD3DDevice->SetDepthStencilSurface(pZBuffDef);

		//���̃r���[�|�[�g�ɖ߂�
		m_pD3DDevice->SetViewport(&viewportDef);

		//-----------------------------------------------------------------------------------

		//�`��J�n
		if (SUCCEEDED(m_pD3DDevice->BeginScene()))
		{//�`�揈�������������ꍇ

			//-----------------------------------------------------------------------------------

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			m_pD3DDevice->SetStreamSource(0, m_pVtxBuffMT, 0, sizeof(VERTEX_2D));

			//���_�t�H�[�}�b�g�̐ݒ�
			m_pD3DDevice->SetFVF(FVF_VERTEX_2D);

			//�e�N�X�`���̐ݒ�
			m_pD3DDevice->SetTexture(0, m_apTextureMT[0]);

			//�|���S���̕`��
			m_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//�v���~�e�B�u�̎��
				0,						//�`��̊J�n�ʒu
				2);						//�v���~�e�B�u(�|���S��)��
			//-----------------------------------------------------------------------------------

			//�`��I��
			m_pD3DDevice->EndScene();
		}
		//�o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
		m_pD3DDevice->Present(NULL, NULL, NULL, NULL);

		//�e�N�X�`����[0]��[1]�����ւ���
		pTextureWk = m_apTextureMT[0];
		m_apTextureMT[0] = m_apTextureMT[1];
		m_apTextureMT[1] = pTextureWk;

		pRenderWk = m_apRenderMT[0];
		m_apRenderMT[0] = m_apRenderMT[1];
		m_apRenderMT[1] = pRenderWk;
	}
	else
	{
		//��ʃN���A(�o�b�N�o�b�t�@��Z�o�b�t�@�̃N���A)
		m_pD3DDevice->Clear(0, NULL,
			(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
			D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

		//�`��J�n
		if (SUCCEEDED(m_pD3DDevice->BeginScene()))
		{//�`�揈�������������ꍇ

			//�J�����̐ݒ�
			pCamera->SetCamera();

			//�S�ẴI�u�W�F�N�g2D�̕`�揈��
			CObject::DrawAll();

			//�t�F�[�h�̍X�V����
			pFade->Draw();

			//�f�o�b�O�\���̕`�揈��
			pDebugProc->Draw();

			//�`��I��
			m_pD3DDevice->EndScene();
		}
		//�o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
		m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
	}
}

//====================================================================
//�����_�����O�^�[�Q�b�g�̕ύX
//====================================================================
void CRenderer::ChageTarget(D3DXVECTOR3 posV, D3DXVECTOR3 posR, D3DXVECTOR3 vecU)
{
	D3DXMATRIX mtxView, mtxProjection;
	float fAspect;

	//�����_�����O�^�[�Q�b�g�𐶐������e�N�X�`���ɐݒ�
	m_pD3DDevice->SetRenderTarget(0, m_apRenderMT[0]);

	//Z�o�b�t�@�𐶐�����Z�o�b�t�@�ɐݒ�
	m_pD3DDevice->SetDepthStencilSurface(m_pZBuffMT);

	//�e�N�X�`�������_�����O�p�̃r���[�|�[�g��ݒ�
	m_pD3DDevice->SetViewport(&m_ViewportMT);

	//�r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxView);

	//�r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&mtxView,
		&posV,
		&posR,
		&vecU);

	//�r���[�}�g���b�N�X�̐ݒ�
	m_pD3DDevice->SetTransform(D3DTS_WORLD, &mtxView);

	//�v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxProjection);

	//�v���W�F�N�V�����}�g���b�N�X�̍쐬
	fAspect = (float)m_ViewportMT.Width / (float)m_ViewportMT.Height;
	D3DXMatrixPerspectiveFovLH(&mtxProjection,
		D3DXToRadian(45.0f),
		fAspect,
		10.0f,
		12000.0f);

	//�v���W�F�N�V�����}�b�s���O�̐ݒ�
	m_pD3DDevice->SetTransform(D3DTS_WORLD, &mtxProjection);
}

//====================================================================
//�f�o�C�X�̎擾
//====================================================================
LPDIRECT3DDEVICE9 CRenderer::GetDevice(void)
{
	return m_pD3DDevice;
}

////============================================
////
////	�����_���[[renderer.cpp]
////	Author:sakamoto kai
////
////============================================
//#include "renderer.h"
//#include "object.h"
//#include "debugproc.h"
//#include "manager.h"
//#include "camera.h"
//#include "fade.h"
//#include "Pause.h"
//
////====================================================================
////�R���X�g���N�^
////====================================================================
//CRenderer::CRenderer()
//{
//	m_pD3D = NULL;					//Direct3D�I�u�W�F�N�g�ւ̃|�C���^
//	m_pD3DDevice = NULL;			//Direct3D�f�o�C�X�ւ̃|�C���^
//}
//
////====================================================================
////�f�X�g���N�^
////====================================================================
//CRenderer::~CRenderer()
//{
//
//}
//
////====================================================================
////����������
////====================================================================
//HRESULT CRenderer::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
//{
//	D3DDISPLAYMODE d3ddm;			//�f�B�X�v���C���[�h
//	D3DPRESENT_PARAMETERS d3dpp;	//�v���[���e�[�V�����p�����[�^
//
//									//Direct3D�I�u�W�F�N�g�̐���
//	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
//	if (m_pD3D == NULL)
//	{
//		return E_FAIL;
//	}
//
//	//���݂̃f�B�X�v���C���[�h������
//	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
//	{
//		return E_FAIL;
//	}
//
//	//�f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
//	ZeroMemory(&d3dpp, sizeof(d3dpp));	//�p�����[�^�̃[���N���A
//
//	d3dpp.BackBufferWidth = SCREEN_WIDTH;							//�Q�[����ʂ̃T�C�Y(��)
//	d3dpp.BackBufferHeight = SCREEN_HEIGHT;							//�Q�[����ʂ̃T�C�Y(����)
//	d3dpp.BackBufferFormat = d3ddm.Format;							//�o�b�N�o�b�t�@�̌`��
//	d3dpp.BackBufferCount = 1;										//�o�b�N�o�b�t�@�̐�
//	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;						//�_�u���o�b�t�@�̐؂�ւ�(�f���M���ɓ���)
//	d3dpp.EnableAutoDepthStencil = TRUE;							//�f�v�X�o�b�t�@�ƃV�e���V���o�b�t�@���쐬
//	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;						//�f�v�X�o�b�t�@��16bit�Ƃ��Ĉ���
//	d3dpp.Windowed = bWindow;										//�E�C���h�E���[�h
//	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	//���t���b�V�����[�g
//	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		//�C���^�[�o��
//
//	//Direct3d�f�o�C�X�̍쐬
//	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
//		D3DDEVTYPE_HAL,
//		hWnd,
//		D3DCREATE_HARDWARE_VERTEXPROCESSING,
//		&d3dpp,
//		&m_pD3DDevice)))
//	{
//		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
//			D3DDEVTYPE_HAL,
//			hWnd,
//			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
//			&d3dpp,
//			&m_pD3DDevice)))
//		{
//			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
//				D3DDEVTYPE_REF,
//				hWnd,
//				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
//				&d3dpp,
//				&m_pD3DDevice)))
//			{
//				return E_FAIL;
//			}
//		}
//	}
//	//�����_�[�X�e�[�g�̐ݒ�
//	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
//	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
//	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
//	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
//
//	//�T���v���[�X�e�[�g(�e�N�X�`���̊g�k�⊮)�̐ݒ�
//	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
//	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
//	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
//	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
//
//	//�e�N�X�`���X�e�[�W�X�e�[�g(�e�N�X�`���̃A���t�@�u�����h)�̐ݒ�
//	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
//	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
//	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
//
//	return S_OK;
//}
//
////====================================================================
////�I������
////====================================================================
//void CRenderer::Uninit(void)
//{
//	//�f�o�b�N�\���p�t�H���g�̔j��
//	if (m_pD3DDevice != NULL)
//	{
//		m_pD3DDevice->Release();
//		m_pD3DDevice = NULL;
//	}
//
//	//Direct3D�I�u�W�F�N�g�̔j��
//	if (m_pD3D != NULL)
//	{
//		m_pD3D->Release();
//		m_pD3D = NULL;
//	}
//}
//
////====================================================================
////�X�V����
////====================================================================
//void CRenderer::Update(void)
//{
//	//�S�ẴI�u�W�F�N�g2D�̍X�V����
//	CObject::UpdateAll();
//}
//
////====================================================================
////�`�揈��
////====================================================================
//void CRenderer::Draw(void)
//{
//	//�f�o�b�O�\���̎擾
//	CDebugProc *pDebugProc = CManager::GetInstance()->GetDebugProc();
//
//	//�J�����̎擾
//	CCamera *pCamera = CManager::GetInstance()->GetCamera();
//
//	//�t�F�[�h�̎擾
//	CFade *pFade = CManager::GetInstance()->GetFade();
//
//	//��ʃN���A(�o�b�N�o�b�t�@��Z�o�b�t�@�̃N���A)
//	m_pD3DDevice->Clear(0, NULL,
//		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
//		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
//	//�`��J�n
//	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
//	{//�`�揈�������������ꍇ
//
//		//�J�����̐ݒ�
//		pCamera->SetCamera();
//
//		//�S�ẴI�u�W�F�N�g2D�̕`�揈��
//		CObject::DrawAll();
//
//		//�t�F�[�h�̍X�V����
//		pFade->Draw();
//
//		//�f�o�b�O�\���̕`�揈��
//		pDebugProc->Draw();
//
//		//�`��I��
//		m_pD3DDevice->EndScene();
//	}
//	//�o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
//	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
//}
//
////====================================================================
////�f�o�C�X�̎擾
////====================================================================
//LPDIRECT3DDEVICE9 CRenderer::GetDevice(void)
//{
//	return m_pD3DDevice;
//}