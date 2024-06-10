//============================================
//
//	オブジェクトメッシュフィールド [objmeshField.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _OBJMESHFIELD_H_
#define _OBJMESHFIELD_H_

#include "main.h"
#include "object.h"

//オブジェクトメッシュフィールドクラス
class CObjmeshField : public CObject
{
public:
	CObjmeshField(int nPriority = 3);
	~CObjmeshField();

	static CObjmeshField* Create(void);

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

protected:
	LPDIRECT3DTEXTURE9 m_pTexture;			//テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		//頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 g_pIdxBuff;		//インデックスバッファへのポインタ
	D3DXMATRIX m_mtxWorld;					//ワールドマトリックス

private:
	int m_nIdxTexture;
	D3DXVECTOR3 m_pos;						//現在の位置
	D3DXVECTOR3 m_posOld;					//過去の位置
	D3DXVECTOR3 m_rot;						//向き
	D3DXCOLOR m_Color;						//色
};
#endif