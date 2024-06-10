//============================================
//
//	数字の処理 [NumberFall.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _NUMBERFALL_H_
#define _NUMBERFALL_H_

#include "main.h"
#include "numberBillboard.h"

//オブジェクトプレイヤークラス
class CNumberFall : public CNumberBillboard
{
public:

	CNumberFall(int nPriority = 4);
	~CNumberFall();

	static CNumberFall* Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetMove(D3DXVECTOR3 Move) { m_move = Move; }
	D3DXVECTOR3 GetMove(void) { return m_move; }

private:
	D3DXVECTOR3 m_move;
	float m_ColorA;
};
#endif