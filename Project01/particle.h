//============================================
//
//	パーティクル処理 [particle.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "main.h"
#include "object.h"

//前方宣言
class CObject2D;

//パーティクルクラス
class CParticle : public CObject
{
public:
	CParticle();
	CParticle(D3DXVECTOR3 pos, D3DXCOLOR col, int nLife, float nSize, int nType);
	~CParticle();

	static CParticle *Create(D3DXVECTOR3 pos, D3DXCOLOR col, int nLife, float nSize, int nType);

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	D3DXVECTOR3 GetMove(void) { return m_move; }
	void SetRadius(float Size) { m_nSize = Size; }
	float GetRadius(void) { return m_nSize; }
	void SetColer(D3DXCOLOR Coler) { m_col = Coler; }
	D3DXCOLOR GetColer(void) { return m_col; }
	void SetLife(int Life) { m_nLife = Life; }
	int GetLife(void) { return m_nLife; }
	void SetParticleType(int Type) { m_nType = Type; }
	int GetParticleType(void) { return m_nType; }

protected:

private:
	D3DXVECTOR3 m_pos;	//発生位置
	D3DXVECTOR3 m_move;	//移動量
	D3DXCOLOR m_col;	//色
	int m_nLife;		//寿命(発生時間)
	float m_nSize;		//大きさ
	int m_nType;		//種類	

};

#endif