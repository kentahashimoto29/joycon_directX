//============================================
//
//	モデル残像 [modelEffect.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _MODELEFFECT_H_
#define _MODELEFFECT_H_

#include "main.h"
#include "objectX.h"

//前方宣言

//オブジェクトプレイヤークラス
class CModelEffect : public CObjectX
{
public:

	CModelEffect(int nPriority = 3);
	~CModelEffect();

	static CModelEffect* Create(char* pModelName);

	HRESULT Init(char* pModelName);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	int GetIdx(void) { return m_nIdxTexture; }
	int GetIdxXModel(void) { return m_nIdxXModel; }
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	void SetColor(D3DXCOLOR color) { m_color = color; }
	D3DXCOLOR GetColor(void) { return m_color; }

private:

	int m_nIdxXModel;				//Xモデルの番号
	int m_nIdxTexture;				//テクスチャの番号
	D3DXVECTOR3 m_pos;				//位置	
	D3DXVECTOR3 m_rot;				//向き	
	D3DXCOLOR m_color;				//色
};
#endif