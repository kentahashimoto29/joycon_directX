//============================================
//
//	�G�f�B�b�g [Edit.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _EDIT_H_
#define _EDIT_H_

#include "main.h"
#include "object.h"

#define DATA_BLOCK_NAME ("data\\TXT\\STAGE\\Block.txt")
#define DATA_XMODEL_NAME ("data\\TXT\\STAGE\\XModel.txt")

//�O���錾
class CObjmeshCube;
class CMapModel;

//�}�b�v�N���X
class CEdit
{
public:
	//-------------------------------------------------------------------------
// �I�u�W�F�N�g�ꗗ
//-------------------------------------------------------------------------
	enum OBJ_HIT
	{
		OBJ_HIT_TYPE1 = 0,		// �T���v���P
		OBJ_HIT_TYPE2,			// �T���v���Q
		OBJ_HIT_TYPE3,			// �T���v���R
		OBJ_HIT_TYPE4,			// �T���v���S
		OBJ_HIT_TYPE5,			// �T���v���T
		OBJ_HIT_TYPE6,			// �T���v���U
		OBJ_HIT_TYPE7,			// �T���v���V
		OBJ_HIT_MAX				// ���̗񋓌^�̑���
	};

	struct OBJINFO
	{
		char* pFilename;	// �t�@�C����
		char* pCommentname;	// �R�����g��
	};

	CEdit();
	~CEdit();

	enum EDITTYPE
	{
		EDITTYPE_BLOCK = 0,
		EDITTYPE_MODELHIT,
		EDITTYPE_MODELNOT,
		EDITTYPE_ENEMY,
		EDITTYPE_MAX,
	};

	static CEdit* Create();
	void Uninit(void);
	void Update(void);

protected:

private:
	void Move(void);			//�ړ�����
	void CollisionBlock(void);	//�u���b�N�Ƃ̓����蔻��
	void CollisionXModel(void);	//X���f���Ƃ̓����蔻��
	void SaveData(void);		//�G�f�B�b�g���̕ۑ�
	void SaveBlock(void);		//�u���b�N�̕ۑ�
	void SaveXModel(void);		//X���f���̕ۑ�
	void UpdateBlock(void);		//�u���b�N�̍X�V
	void UpdateXModel(void);	//X���f���̍X�V
	void AppearCollision(void);	//�����蔻��̉���

	int m_nPressCount;			//�{�^�����̓J�E���g
	D3DXVECTOR3 m_EditPos;		//�ݒu�ʒu
	D3DXVECTOR3 m_EditRot;		//�ݒu�p�x
	D3DXVECTOR3 m_EditSize;		//�ݒu�T�C�Y
	float MoveWidth;			//�ړ���
	EDITTYPE m_EditType;		//�G�f�B�b�g�Ŕz�u����I�u�W�F�N�g�̎��
	CObjmeshCube* m_pEditBlock;	//�u���b�N�̃|�C���^
	CMapModel* m_pEditModel;	//�}�b�v���f���̃|�C���^
	int m_nModelNumber;			//�ړ���
	bool m_bModelCollision;		//�����蔻���t�^���邩�ǂ���
	bool m_bAppearCollision;		//�����蔻����������邩�ǂ���

	static OBJINFO m_aModelInfo[OBJ_HIT_MAX];	// �e�N�X�`���̏��
};
#endif