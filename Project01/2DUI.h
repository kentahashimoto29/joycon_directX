//============================================
//
//	2DUI [2DUI.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _2DUI_H_
#define _2DUI_H_

#include "main.h"
#include "object2D.h"

//�I�u�W�F�N�g�v���C���[�N���X
class C2DUI : public CObject2D
{
public:

	C2DUI(int nPriority = 6);
	~C2DUI();

	static C2DUI* Create(void);

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