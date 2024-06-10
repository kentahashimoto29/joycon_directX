//============================================
//
//	�I�u�W�F�N�g3D [object3D.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

#include "main.h"
#include "object.h"

//�I�u�W�F�N�g3D�N���X
class CObject3D : public CObject
{
public:
	CObject3D(int nPriority = 4);
	~CObject3D();

	static CObject3D *Create(void);

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	void SetNULL(void);
	virtual void Update(void);
	virtual void Draw(void);
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);

	void SetVerPos(D3DXVECTOR3 Pos0, D3DXVECTOR3 Pos1, D3DXVECTOR3 Pos2, D3DXVECTOR3 Pos3);	//���_���W�̐ݒ�
	void SetVerTex(void);																	//���_���W�̐ݒ�
	void SetAnim(D3DXVECTOR2 Tex);															//�e�N�X�`�����W�̐ݒ�
	void SetColor(D3DXCOLOR col);															//���_�J���[�̐ݒ�
	void SetColorA(float A) { m_Color.a = A; SetColor(m_Color); }							//���_�s�����x�̐ݒ�
	void SetTexture(const char *name);

	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	void SetPosOld(D3DXVECTOR3 pos) { m_posOld = pos; }
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	void SetWidth(float Width) { m_Width = Width; }
	float GetWidth(void) { return m_Width; }
	void SetHeight(float Height) { m_Height = Height; }
	float GetHeight(void) { return m_Height; }
	void SetAddDorw(bool Set) { m_AddDarw = Set; }
	bool GetAddDorw(void) { return m_AddDarw; }
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void) { return m_pVtxBuff; }
	virtual int GetIdx(void) { return m_nIdxTexture; }

protected:
	LPDIRECT3DTEXTURE9 m_pTexture;			//�e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		//���_�o�b�t�@�ւ̃|�C���^
	D3DXMATRIX m_mtxWorld;					//���[���h�}�g���b�N�X

private:
	int m_nIdxTexture;
	float m_Width;							//��
	float m_Height;							//����
	bool m_AddDarw;							//���Z����
	D3DXVECTOR3 m_pos;						//���݂̈ʒu
	D3DXVECTOR3 m_posOld;					//�ߋ��̈ʒu
	D3DXVECTOR3 m_rot;						//����
	D3DXCOLOR m_Color;						//�F

};
#endif