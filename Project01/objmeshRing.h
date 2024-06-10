//============================================
//
//	オブジェクトメッシュリング[objmeshRing.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _OBJMESHRING_H_
#define _OBJMESHRING_H_

#include "main.h"
#include "object.h"

//オブジェクトメッシュフィールドクラス
class CObjmeshRing : public CObject
{
public:
	CObjmeshRing(int nPriority = 3);
	~CObjmeshRing();

	static CObjmeshRing* Create(void);

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);

	void SetVerPos(D3DXVECTOR3 Pos0, D3DXVECTOR3 Pos1, D3DXVECTOR3 Pos2, D3DXVECTOR3 Pos3);	//頂点座標の設定
	void SetAnim(D3DXVECTOR2 Tex);															//テクスチャ座標の設定
	void SetColor(D3DXCOLOR col);															//頂点カラーの設定
	void SetColorA(float A) { m_Color.a = A; SetColor(m_Color); }							//頂点不透明度の設定
	void SetTexture(const char* name);

	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	void SetPosOld(D3DXVECTOR3 pos) { m_posOld = pos; }
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void) { return m_pVtxBuff; }
	virtual int GetIdx(void) { return m_nIdxTexture; }

	void SetRadius(float Radius) { m_Radius = Radius; }
	void SetRadiusMove(float move) { m_RadiusMove = move; }
	void SetLife(int Life) { m_nlife = Life; }
	bool CollisionRing(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float nRadiusOut, float nRadiusIn, float MinY, float MaxY);

protected:
	LPDIRECT3DTEXTURE9 m_pTexture;			//テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		//頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 g_pIdxBuff;		//インデックスバッファへのポインタ
	D3DXMATRIX m_mtxWorld;					//ワールドマトリックス

private:
	int m_nIdxTexture;
	float m_Radius;							//半径
	float m_RadiusMove;						//半径移動量
	int m_nlife;							//寿命
	D3DXVECTOR3 m_pos;						//現在の位置
	D3DXVECTOR3 m_posOld;					//過去の位置
	D3DXVECTOR3 m_rot;						//向き
	D3DXCOLOR m_Color;						//色
};
#endif