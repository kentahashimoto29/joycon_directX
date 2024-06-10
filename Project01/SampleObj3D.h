//============================================
//
//	オブジェクト3Dのサンプル [SampleObj3D.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _SAMPLEOBJ3D_H_
#define _SAMPLEOBJ3D_H_

#include "main.h"
#include "object3D.h"

//オブジェクトプレイヤークラス
class CSampleObj3D : public CObject3D
{
public:

	CSampleObj3D(int nPriority = 4);
	~CSampleObj3D();

	static CSampleObj3D* Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	//取得処理・設定処理
	int GetSample(void) { return m_nSample; }
	void SetSample(int Sample) { m_nSample = Sample; }

private:
	int m_nSample;							//サンプル変数
};
#endif