//============================================
//
//	�I�u�W�F�N�gX�t�@�C�� [objectXtex.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _OBJECTXTEX_H_
#define _OBJECTXTEX_H_

#include "main.h"
#include "object.h"

//�I�u�W�F�N�gX�N���X
class CObjectXtex : public CObject
{
public:
	CObjectXtex(int nPriority = 7);
	~CObjectXtex();

	static CObjectXtex *Create(void);

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);
	void BindFile(LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD pdwNumMat);

	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	virtual int GetIdx(void) { return -1; }
	void SetWidth(float Width) { m_Width = Width; }
	float GetWidth(void) { return m_Width; }
	void SetHeight(float Height) { m_Height = Height; }
	float GetHeight(void) { return m_Height; }
	void SetMatColor(D3DXCOLOR SetColor) { m_MatColor = SetColor; }
	void SetTexOne(bool Set) { m_bTexOne = Set; }
	D3DXVECTOR3 GetSize(void);
	void SetMatChange(bool bUse) { m_bMatChange = bUse; }
	bool GetMatChange(void) { return m_bMatChange; }

protected:
	D3DXMATRIX m_mtxWorld;					//���[���h�}�g���b�N�X

private:
	float m_Width;							//��
	float m_Height;							//����
	LPD3DXMESH m_pMesh;						//���b�V��(���_���)�ւ̃|�C���^
	LPD3DXBUFFER m_pBuffMat;				//�}�e���A���ւ̃|�C���^
	DWORD m_dwNumMat;						//�}�e���A���̐�
	D3DXVECTOR3 m_pos;						//���_�̈ʒu
	D3DXVECTOR3 m_rot;						//����
	D3DXCOLOR m_MatColor;					//�}�e���A���̐F
	bool m_bTexOne;							//�e�N�X�`���ύX�𐳖ʂ݂̂ɂ��邩�ǂ���
	bool m_bMatChange;						//�}�e���A���̐F���w��̐F�ɕύX���邩�ǂ���
};
#endif