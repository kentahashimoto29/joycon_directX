//============================================
//
//	�I�u�W�F�N�g�r���{�[�h�̃T���v�� [SampleObjBillboard.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _SAMPLEOBJBILLBOARD_H_
#define _SAMPLEOBJBILLBOARD_H_

#include "main.h"
#include "objectBillboard.h"

//�I�u�W�F�N�g�v���C���[�N���X
class CSampleObjBillboard : public CObjectBillboard
{
public:

	CSampleObjBillboard(int nPriority = 4);
	~CSampleObjBillboard();

	static CSampleObjBillboard* Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//�擾�����E�ݒ菈��
	int GetSample(void) { return m_nSample; }
	void SetSample(int Sample) { m_nSample = Sample; }

private:
	int m_nSample;							//�T���v���ϐ�
};
#endif