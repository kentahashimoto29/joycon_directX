//============================================
//
//	エフェクト処理 [2DEffect.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "main.h"
#include "object2D.h"

//オブジェクトプレイヤークラス
class C2DEffect : public CObject2D
{
public:

	C2DEffect(int nPriority = 4);
	~C2DEffect();

	static C2DEffect* Create();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	D3DXVECTOR3 GetMove(void) { return m_move; }
	void SetRadius(float Radius) { m_fRadius = Radius; }
	float GetRadius(void) { return m_fDel; }
	void SetDel(float Del) { m_fDel = Del; }
	float GetDel(void) { return m_fDel; }
	D3DXCOLOR GetColer(void) { return m_col; }
	void SetLife(int Life) { m_nLife = Life; m_nLifeMax = Life; }
	int GetLife(void) { return m_nLife; }
	int GetIdx(void) { return m_nIdxTexture; }
	void SetTexName(char* Name) { m_cTexName = Name; }

private:
	int m_nIdxTexture;						//テクスチャの番号
	static LPDIRECT3DTEXTURE9 m_pTexture;	//共有テクスチャへのポインタ
	D3DXVECTOR3 m_move;						//移動量
	float m_fRadius;						//半径(大きさ)
	float m_fDel;							//半径を小さくしていく値
	D3DXCOLOR m_col;						//色
	int m_nLifeMax;							//寿命の最大値
	int m_nLife;							//寿命
	char* m_cTexName;
};
#endif