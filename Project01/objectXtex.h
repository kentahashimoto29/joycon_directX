//============================================
//
//	オブジェクトXファイル [objectXtex.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _OBJECTXTEX_H_
#define _OBJECTXTEX_H_

#include "main.h"
#include "object.h"

//オブジェクトXクラス
class CObjectXtex : public CObject
{
public:
	CObjectXtex(int nPriority = 7);
	~CObjectXtex();

	static CObjectXtex *Create(void);

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);
	void BindFile(LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD pdwNumMat);

	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	virtual int GetIdx(void) { return -1; }
	void SetWidth(float Width) { m_Width = Width; }
	float GetWidth(void) { return m_Width; }
	void SetHeight(float Height) { m_Height = Height; }
	float GetHeight(void) { return m_Height; }
	void SetMatColor(D3DXCOLOR SetColor) { m_MatColor = SetColor; }
	void SetTexOne(bool Set) { m_bTexOne = Set; }
	D3DXVECTOR3 GetSize(void);
	void SetMatChange(bool bUse) { m_bMatChange = bUse; }
	bool GetMatChange(void) { return m_bMatChange; }

protected:
	D3DXMATRIX m_mtxWorld;					//ワールドマトリックス

private:
	float m_Width;							//幅
	float m_Height;							//高さ
	LPD3DXMESH m_pMesh;						//メッシュ(頂点情報)へのポインタ
	LPD3DXBUFFER m_pBuffMat;				//マテリアルへのポインタ
	DWORD m_dwNumMat;						//マテリアルの数
	D3DXVECTOR3 m_pos;						//頂点の位置
	D3DXVECTOR3 m_rot;						//向き
	D3DXCOLOR m_MatColor;					//マテリアルの色
	bool m_bTexOne;							//テクスチャ変更を正面のみにするかどうか
	bool m_bMatChange;						//マテリアルの色を指定の色に変更するかどうか
};
#endif