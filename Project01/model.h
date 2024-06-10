//============================================
//
//	���f���̏��� [model.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _MODEL_H_
#define _MODEL_H_

#include "object.h"

//�I�u�W�F�N�g�v���C���[�N���X
class CModel
{
public:
	CModel();
	~CModel();

	static CModel *Create(const char *pModelName);
	HRESULT Init(const char *pModelName);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetParent(CModel *pModel) { m_pParent = pModel; }
	D3DMATRIX GetMtxWorld(void) { return m_mtxWorld; }
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	D3DXVECTOR3 GetWorldPos(void) { return m_WorldPos; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	void SetStartPos(D3DXVECTOR3 pos) { m_Startpos = pos; }
	D3DXVECTOR3 GetStartPos(void) { return m_Startpos; }
	void SetStartRot(D3DXVECTOR3 rot) { m_Startrot = rot; }
	D3DXVECTOR3 GetStartRot(void) { return m_Startrot; }
	void SetColor(D3DCOLORVALUE Color) { m_Color = Color; }
	D3DCOLORVALUE GetColor(void) { return m_Color; }
	void SetColorChange(bool Set) { m_bColorChange = Set; }
	bool GetColorChange(void) { return m_bColorChange; }
	void SetDisp(bool Set) { m_bDisp = Set; }
	bool GetDisp(void) { return m_bDisp; }


private:
	LPDIRECT3DTEXTURE9 *m_pTexture;	//�e�N�X�`���ւ̃|�C���^
	LPD3DXMESH m_pMesh;				//���b�V��(���_���)�ւ̃|�C���^
	LPD3DXBUFFER m_pBuffMat;		//�}�e���A���ւ̃|�C���^
	DWORD m_dwNumMat;				//�}�e���A���̐�

	D3DXVECTOR3 m_Startpos;			//�I�t�Z�b�g�̈ʒu
	D3DXVECTOR3 m_Startrot;			//�I�t�Z�b�g�̈ʒu
	D3DXVECTOR3 m_pos;				//�ʒu
	D3DXVECTOR3 m_rot;				//����
	D3DXMATRIX m_mtxWorld;			//���[���h�}�g���b�N�X
	D3DXVECTOR3 m_WorldPos;			//���[���h���W
	CModel *m_pParent;				//�e���f���ւ̃|�C���^
	D3DCOLORVALUE m_Color;			//�F
	bool m_bColorChange;			//�F��ύX���邩�ǂ���
	bool m_bDisp;					//��ʂɕ\�����邩�ǂ���

};

#endif