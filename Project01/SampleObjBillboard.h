//============================================
//
//	オブジェクトビルボードのサンプル [SampleObjBillboard.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _SAMPLEOBJBILLBOARD_H_
#define _SAMPLEOBJBILLBOARD_H_

#include "main.h"
#include "objectBillboard.h"

//オブジェクトプレイヤークラス
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

	//取得処理・設定処理
	int GetSample(void) { return m_nSample; }
	void SetSample(int Sample) { m_nSample = Sample; }

private:
	int m_nSample;							//サンプル変数
};
#endif