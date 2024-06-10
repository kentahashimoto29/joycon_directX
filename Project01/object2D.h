//============================================
//
//	�I�u�W�F�N�g2D [object2D.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _OBJECT2D_H_
#define _OBJECT2D_H_

#include "main.h"
#include "object.h"

//�I�u�W�F�N�g2D�N���X
class CObject2D : public CObject
{
public:
	CObject2D(int nPriority);
	~CObject2D();

	static CObject2D *Create(int nPriority = 6);

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);

	void SetVerTex(void);						//���_���W�̐ݒ�
	void SetAnim(D3DXVECTOR2 Tex);				//�e�N�X�`�����W�̐ݒ�
	void SetScroll(D3DXVECTOR2 Tex);			//�e�N�X�`�����W�̐ݒ�
	void SetColor(D3DXCOLOR col);				//���_�J���[�̐ݒ�
	void SetColorA(float A);					//���_�J���[�̐ݒ�
	void SetTexture(const char* name);
	const char *GetTexture(void);

	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	void SetWidth(float Width) { m_Width = Width; }
	float GetWidth(void) { return m_Width; }
	void SetHeight(float Height) { m_Height = Height; }
	float GetHeight(void) { return m_Height; }
	virtual void SetIdx(int Idx) { m_nIdxTexture = Idx; }
	virtual int GetIdx(void) { return m_nIdxTexture; }
	void SetAddDraw(bool Set) { m_AddDarw = Set; }
	void SetTargetTexture(LPDIRECT3DTEXTURE9 Texture) { m_pTargetTexture = Texture; }
	void SetMultiTarget(bool Set) { MultiTarget = Set; }
	void SetNULL(void);

protected:
	void SetVtxBuff(LPDIRECT3DVERTEXBUFFER9 Height) { m_pVtxBuff = Height; }
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void) { return m_pVtxBuff; }

private:
	int m_nIdxTexture;
	float m_Width;							//��
	float m_Height;							//����
	bool m_AddDarw;							//�`��̎��ɉ��Z���������邩�ǂ���
	D3DXCOLOR m_Color;						//�F
	D3DXVECTOR3 m_pos;						//���_�̈ʒu
	D3DXVECTOR3 m_rot;						//����
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		//���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DTEXTURE9 m_pTexture;			//�e�N�X�`���ւ̃|�C���^
	LPDIRECT3DTEXTURE9 m_pTargetTexture;	//�e�N�X�`���ւ̃|�C���^
	bool MultiTarget;						//�}���`�^�[�Q�b�g�����_�����O���I���ɂ��邩�ǂ���
};
#endif