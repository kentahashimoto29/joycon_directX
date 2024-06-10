//============================================
//
//	オブジェクトメッシュキューブ [objmeshCube.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _OBJMESHCUBE_H_
#define _OBJMESHCUBE_H_

#include "main.h"
#include "object.h"

//オブジェクトメッシュフィールドクラス
class CObjmeshCube : public CObject
{
public:
	CObjmeshCube(int nPriority = 3);
	~CObjmeshCube();

	static CObjmeshCube* Create(void);

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	void SetVerPos(D3DXVECTOR3 Pos0, D3DXVECTOR3 Pos1, D3DXVECTOR3 Pos2, D3DXVECTOR3 Pos3);	//頂点座標の設定
	void SetVerSize(void);																	//頂点位置の設定
	void SetAnim(D3DXVECTOR2 Tex);															//テクスチャ座標の設定
	void SetColor(D3DXCOLOR col);															//頂点カラーの設定
	void SetColorType(int Type);															//頂点カラーの設定
	D3DXCOLOR GetColor(void) { return m_Color; }											//頂点カラーの取得
	void SetColorA(float A) { m_Color.a = A; SetColor(m_Color); }							//頂点不透明度の設定
	void SetTexture(const char* name);

	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; m_posOld = pos; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	void SetUninitPos(D3DXVECTOR3 pos) { UninitPos = pos; }
	D3DXVECTOR3 GetUninitPos(void) { return UninitPos; }
	void SetPosOld(D3DXVECTOR3 pos) { m_posOld = pos; }
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	void SetSize(D3DXVECTOR3 Size) { m_Size = Size; SetVerSize(); }
	D3DXVECTOR3 GetSize(void) { return m_Size; }
	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	D3DXVECTOR3 GetMove(void) { return m_move; }
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void) { return m_pVtxBuff; }
	virtual int GetIdx(void) { return m_nIdxTexture; }
	void SetPlusMove(bool Set) { m_PlusMove = Set; }
	bool GetPlusMove(void) { return m_PlusMove; }
	void SetBoolLife(bool Set) { m_bLife = Set; }
	bool GetBoolLife(void) { return m_bLife; }
	void SetLife(int Life) { m_nLife = Life; }
	int GetLife(void) { return m_nLife; }
	void SetDel(float Del) { m_fDel = Del; }

protected:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		//頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 g_pIdxBuff;		//インデックスバッファへのポインタ
	D3DXMATRIX m_mtxWorld;					//ワールドマトリックス

private:
	int m_nIdxTexture;
	D3DXVECTOR3 m_pos;						//現在の位置
	D3DXVECTOR3 m_posOld;					//過去の位置
	D3DXVECTOR3 m_rot;						//向き
	D3DXVECTOR3 m_Size;						//大きさ
	D3DXVECTOR3 m_move;						//移動量
	D3DXCOLOR m_Color;						//色
	bool m_PlusMove;						//正の方向へ移動しているかどうか
	bool m_bLife;							//消えるキューブか否か
	D3DXVECTOR3 UninitPos;					//キューブの削除位置
	int m_nLife;							//キューブの寿命
	float m_fDel;							//キューブの色の減衰量
};
#endif