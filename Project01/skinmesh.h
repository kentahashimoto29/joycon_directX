//============================================
//
//	3D�|���S���ŃX�L�����b�V�����s�� [skinmesh.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _SKINMESH_H_
#define _SKINMESH_H_

//�}�N����`
#define MAX_BONE (32)	//�{�[�����̍ő吔
#define MAX_VERTEX (256)	//�{�[�����̍ő吔
#define MAX_INFO (32)	//���̍ő吔
#define MAX_PARTS (32)	//�p�[�c�̑���
#define MAX_KEY (16)	//�L�[�̑���

#include "main.h"
#include "object.h"

class CCubeBlock;

//�I�u�W�F�N�g3D�N���X
class CSkinmesh : public CObject
{
public:
	CSkinmesh(int nPriority = 4);
	~CSkinmesh();

	//�{�[�����̍\����
	typedef struct
	{
		D3DXMATRIX m_mtxWorld;	//���[���h�}�g���b�N�X
		int nParent;			//�e�q�t��
		float fPosX;			//�ʒuX
		float fPosY;			//�ʒuY
		float fPosZ;			//�ʒuZ
		float fRotX;			//����X
		float fRotY;			//����Y
		float fRotZ;			//����Z
	}BONE;

	//���_���̍\����
	typedef struct
	{
		float fPosX;			//�ʒuX
		float fPosY;			//�ʒuY
		float fPosZ;			//�ʒuZ
		int BoneRef[2];			//�ǂ̃{�[���ɂ��Ă�����
		float WeightValue[2];	//�E�F�C�g��
		D3DXVECTOR2 Tex;		//�e�N�X�`���ݒ�

	}VERTEXSET;

	//�L�[�̍\����
	typedef struct
	{
		float fPosX;		//�ʒuX
		float fPosY;		//�ʒuY
		float fPosZ;		//�ʒuZ
		float fRotX;		//����X
		float fRotY;		//����Y
		float fRotZ;		//����Z
	}KEY;

	//�L�[���̍\����
	typedef struct
	{
		int nFrame;					//�Đ��t���[��
		KEY aKey[MAX_PARTS];		//�e���f���̃L�[�v�f[�p�[�c�̑���]
	}KEY_INFO;

	//���[�V�������̍\����
	typedef struct
	{
		bool bLoop;						//���[�v���邩�ǂ���
		int NumKey;						//�e���f���̃L�[�v�f
		KEY_INFO aKeyInfo[MAX_KEY];		//�L�[���[�L�[�̑���]
	}INFO;

	static CSkinmesh* Create(void);

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
	void SetTexture(const char* name);
	D3DMATRIX GetBoneMtxWorld(int nNumber) { return m_Bone[nNumber].m_mtxWorld; }

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

	void LoadData(const char* pFilename);

protected:
	LPDIRECT3DTEXTURE9 m_pTexture;			//�e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		//���_�o�b�t�@�ւ̃|�C���^
	D3DXMATRIX m_mtxWorld;					//���[���h�}�g���b�N�X

private:
	void WaitControl();

	CSkinmesh* m_pParent;				//�e���f���ւ̃|�C���^
	INFO m_aInfo[MAX_INFO];				//���[�V�����̏��[���[�V�����̑���]
	int m_nIdxTexture;
	float m_Width;							//��
	float m_Height;							//����
	bool m_AddDarw;							//���Z����
	D3DXVECTOR3 m_pos;						//���݂̈ʒu
	D3DXVECTOR3 m_posOld;					//�ߋ��̈ʒu
	D3DXVECTOR3 m_rot;						//����
	D3DXCOLOR m_Color;						//�F
	BONE m_Bone[MAX_BONE];					//�{�[�����
	int m_nNumBone;							//�{�[����
	VERTEXSET m_Vertex[MAX_BONE];			//���_���
	int m_nNumVertex;						//���_��
	CCubeBlock* m_pCubeBlock[MAX_BONE];
};
#endif