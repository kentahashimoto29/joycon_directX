//============================================
//
//	オブジェクト2Dのサンプル [SampleObj2D.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _SAMPLEOBJ2D_H_
#define _SAMPLEOBJ2D_H_

#include "main.h"
#include "object2D.h"

//オブジェクトプレイヤークラス
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

	//取得処理・設定処理
	int GetSample(void) { return m_nSample; }
	void SetSample(int Sample) { m_nSample = Sample; }

private:
	int m_nSample;							//サンプル変数
};
#endif