//============================================
//
//	オブジェクトXファイル [objectX.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _OBJECTX_H_
#define _OBJECTX_H_

#include "main.h"
#include "object.h"

//オブジェクトXクラス
class CObjectX : public CObject
{
public:
	CObjectX(int nPriority = 7);
	~CObjectX();

	static CObjectX *Create(char *pModelName);

	virtual HRESULT Init(void);
	virtual HRESULT Init(char* pModelName);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);
	virtual void Draw(float fAlpha);
	void BindFile(LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD pdwNumMat);

	void SetVerTex(void);							//頂点座標の設定
	void SetAnim(D3DXVECTOR2 Tex);					//テクスチャ座標の設定
	void SetColor(D3DXCOLOR col);					//頂点カラーの設定

	void SetSize(D3DXVECTOR3 size) { m_size = size; }
	D3DXVECTOR3 GetSize(void) { return m_size; }

	void Test(void);
	void SwapSize(void);

	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	void SetPosOld(D3DXVECTOR3 pos) { m_posOld = pos; }
	D3DXVECTOR3 GetPosOld(void) { return m_posOld; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	void SetMatColor(D3DXCOLOR SetColor) { m_MatColor = SetColor; m_bUseColor = true; }
	void SetMatAlpha(float SetAlpha) { m_StateMat.MatD3D.Diffuse.a = SetAlpha; }
	void SetDefColor(void) { m_bUseColor = false; }
	void SetScaling(D3DXVECTOR3 Scaling) { m_Scaling = Scaling; }

	void SetXModelIdx(int Idx);
	int GetXModelIdx(void) { return m_IdxModelNumber; }
	void SetTextureIdx(int nCnt, int Idx);
	int GetTextureIdx(int nCnt) { return m_IdxTextureNumber[nCnt]; }

protected:
	D3DXMATRIX m_mtxWorld;					//ワールドマトリックス

private:
	int m_IdxModelNumber;					//モデルの番号
	int m_IdxTextureNumber[64];				//テクスチャの番号
	LPDIRECT3DTEXTURE9 *m_pTexture;			//テクスチャへのポインタ
	LPD3DXMESH m_pMesh;						//メッシュ(頂点情報)へのポインタ
	LPD3DXBUFFER m_pBuffMat;				//マテリアルへのポインタ
	DWORD m_dwNumMat;						//マテリアルの数
	D3DXVECTOR3 m_pos;						//頂点の位置
	D3DXVECTOR3 m_posOld;					//過去の位置
	D3DXVECTOR3 m_rot;						//向き
	D3DXVECTOR3 m_size;						//サイズ
	D3DXVECTOR3 m_Scaling;					//大きさ
	D3DXMATERIAL m_StateMat;				//状態管理時の色
	D3DXCOLOR m_MatColor;					//マテリアルの色
	bool m_bUseColor;						//色を変更しているかどうか
};
#endif