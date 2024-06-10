//============================================
//
//	�����_���[[renderer.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _RENSERER_H_
#define _RENSERER_H_

#include "main.h"

//�����_���[�N���X
class CRenderer
{
public:
	CRenderer();	//�R���X�g���N�^
	~CRenderer();	//�f�X�g���N�^

	//�����_���[�̏���
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow); 	//�Ԃ�l��HRESULT�Ȃ̂͒��_�o�b�t�@�̐����ȂǂɎ��s�����Ƃ��m�F�̂���
	void Uninit(void);
	void Update(void);
	void Draw(void);
	LPDIRECT3DDEVICE9 GetDevice(void);

	void ChageTarget(D3DXVECTOR3 posV, D3DXVECTOR3 posR, D3DXVECTOR3 vecU);
	LPDIRECT3DTEXTURE9 GetTextureMT(void) { return m_apTextureMT[0]; }
	void SetAfterImage(bool Set) { m_bAfterimage = Set; }
	bool GetAfterImage(void) { return m_bAfterimage; }

private:

	LPDIRECT3D9 m_pD3D;					//Direct3D�I�u�W�F�N�g�ւ̃|�C���^
	LPDIRECT3DDEVICE9 m_pD3DDevice;		//Direct3D�f�o�C�X�ւ̃|�C���^

	LPDIRECT3DTEXTURE9 m_apTextureMT[2];	//�����_�����O�^�[�Q�b�g�p�e�N�X�`��
	LPDIRECT3DSURFACE9 m_apRenderMT[2];		//�e�N�X�`�������_�����O�p�C���^�[�t�F�C�X
	LPDIRECT3DSURFACE9 m_pZBuffMT;			//�e�N�X�`�������_�����O�pZ�o�b�t�@
	D3DVIEWPORT9 m_ViewportMT;				//�e�N�X�`�������_�����O�p�r���[�|�[�g

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffMT;	//�t�B�[�h�o�b�N�p�|���S���̒��_�o�b�t�@
	bool m_bAfterimage;						//�c���̃I���I�t
};

#endif

////============================================
////
////	�����_���[[renderer.h]
////	Author:sakamoto kai
////
////============================================
//#ifndef _RENSERER_H_
//#define _RENSERER_H_
//
//#include "main.h"
//
////�����_���[�N���X
//class CRenderer
//{
//public:
//	CRenderer();	//�R���X�g���N�^
//	~CRenderer();	//�f�X�g���N�^
//
//	//�����_���[�̏���
//	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow); 	//�Ԃ�l��HRESULT�Ȃ̂͒��_�o�b�t�@�̐����ȂǂɎ��s�����Ƃ��m�F�̂���
//	void Uninit(void);
//	void Update(void);
//	void Draw(void);
//	LPDIRECT3DDEVICE9 GetDevice(void);
//
//private:
//
//	LPDIRECT3D9 m_pD3D;					//Direct3D�I�u�W�F�N�g�ւ̃|�C���^
//	LPDIRECT3DDEVICE9 m_pD3DDevice;		//Direct3D�f�o�C�X�ւ̃|�C���^
//};
//
//#endif