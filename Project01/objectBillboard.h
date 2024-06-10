//============================================
//
//	オブジェクトビルボード [objectBillboard.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _OBJECTBILLBOARD_H_
#define _OBJECTBILLBOARD_H_

#include "main.h"
#include "object.h"

//オブジェクトBillboardクラス
class CObjectBillboard : public CObject
{
public:
	CObjectBillboard(int nPriority = 7);
	~CObjectBillboard();

	static CObjectBillboard *Create(void);

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);

	void SetVerPos(D3DXVECTOR3 Pos0, D3DXVECTOR3 Pos1, D3DXVECTOR3 Pos2, D3DXVECTOR3 Pos3);	//頂点座標の設定
	void SetVerTex(void);																	//頂点座標の設定
	void SetAnim(D3DXVECTOR2 Tex);															//テクスチャ座標の設定
	void SetColor(D3DXCOLOR col);															//頂点カラーの設定
	void SetColorA(float A) { m_Color.a = A; SetColor(m_Color); }							//頂点不透明度の設定
	void SetTexture(const char *name);

	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	void SetPosOld(D3DXVECTOR3 pos) { m_posOld = pos; }
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	void SetWidth(float Width) { m_Width = Width; }
	float GetWidth(void) { return m_Width; }
	void SetHeight(float Height) { m_Height = Height; }
	float GetHeight(void) { return m_Height; }
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void) { return m_pVtxBuff; }
	virtual int GetIdx(void) { return m_nIdxTexture; }
	void SetDigit(int Digit) { m_Digit = Digit; }

protected:
	LPDIRECT3DTEXTURE9 m_pTexture;			//テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		//頂点バッファへのポインタ
	D3DXMATRIX m_mtxWorld;					//ワールドマトリックス

private:
	int m_nIdxTexture;
	float m_Width;							//幅
	float m_Height;							//高さ
	D3DXVECTOR3 m_pos;						//現在の位置
	D3DXVECTOR3 m_posOld;					//過去の位置
	D3DXVECTOR3 m_rot;						//向き
	D3DXCOLOR m_Color;						//色
	int m_Digit;							//表示する数字の桁数

};
#endif