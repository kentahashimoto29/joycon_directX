//============================================
//
//	�I�u�W�F�N�g2D�̃T���v�� [SampleObj2D.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _SAMPLEOBJ2D_H_
#define _SAMPLEOBJ2D_H_

#include "main.h"
#include "object2D.h"

//�I�u�W�F�N�g�v���C���[�N���X
class CSampleObj2D : public CObject2D
{
public:

	CSampleObj2D(int nPriority = 6);
	~CSampleObj2D();

	static CSampleObj2D* Create(int nPriority = 6);

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