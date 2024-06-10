//============================================
//
//	数字の処理 [numberBillboard.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _NUMBERBILLBOARD_H_
#define _NUMBERBILLBOARD_H_

#include "main.h"
#include "objectBillboard.h"

//オブジェクトプレイヤークラス
class CNumberBillboard : public CObjectBillboard
{
public:

	CNumberBillboard(int nPriority = 4);
	~CNumberBillboard();

	static CNumberBillboard* Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	int GetNumber(void) { return m_nNumber; }
	void SetNumber(int number) { m_nNumber = number; }
	int GetIdx(void) { return m_nIdxTexture; }

private:
	int m_nIdxTexture;						//テクスチャの番号
	int m_nNumber;							//表示する数字
};
#endif