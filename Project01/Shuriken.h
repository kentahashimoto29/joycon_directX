//========================================================================================
//
//　手裏剣[Shuriken.h]
//	Author:Takeru Ogasawara
//
//========================================================================================
#ifndef _SHURIKEN_H_
#define _SHURIKEN_H_

// インクルード宣言
#include "main.h"
#include "objectX.h"

// 前方宣言
class CAim;

// 手裏剣クラス
class CShuriken : public CObjectX
{
public:

	CShuriken();
	~CShuriken();

	static CShuriken* Create(D3DXVECTOR3 pos, float fSpeed = 1.0f, float fLife = 20.0f);

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();

private:
	void Throw(void);	// 投げる
	void LifeDecrease(void);
	void Rot(void);

	void Aim(void);
	void Lockon(void);

	void CollsionEnemy(void);

	// 情報
	struct SInfo
	{
		D3DXVECTOR3 move;
		D3DXVECTOR3 diff;
		float fSpeed;
		float fLife;
		bool bUse;
	};

	// メンバ変数
	SInfo m_info;
};

#endif