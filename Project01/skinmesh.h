//============================================
//
//	3Dポリゴンでスキンメッシュを行う [skinmesh.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _SKINMESH_H_
#define _SKINMESH_H_

//マクロ定義
#define MAX_BONE (32)	//ボーン情報の最大数
#define MAX_VERTEX (256)	//ボーン情報の最大数
#define MAX_INFO (32)	//情報の最大数
#define MAX_PARTS (32)	//パーツの総数
#define MAX_KEY (16)	//キーの総数

#include "main.h"
#include "object.h"

class CCubeBlock;

//オブジェクト3Dクラス
class CSkinmesh : public CObject
{
public:
	CSkinmesh(int nPriority = 4);
	~CSkinmesh();

	//ボーン情報の構造体
	typedef struct
	{
		D3DXMATRIX m_mtxWorld;	//ワールドマトリックス
		int nParent;			//親子付け
		float fPosX;			//位置X
		float fPosY;			//位置Y
		float fPosZ;			//位置Z
		float fRotX;			//向きX
		float fRotY;			//向きY
		float fRotZ;			//向きZ
	}BONE;

	//頂点情報の構造体
	typedef struct
	{
		float fPosX;			//位置X
		float fPosY;			//位置Y
		float fPosZ;			//位置Z
		int BoneRef[2];			//どのボーンについていくか
		float WeightValue[2];	//ウェイト数
		D3DXVECTOR2 Tex;		//テクスチャ設定

	}VERTEXSET;

	//キーの構造体
	typedef struct
	{
		float fPosX;		//位置X
		float fPosY;		//位置Y
		float fPosZ;		//位置Z
		float fRotX;		//向きX
		float fRotY;		//向きY
		float fRotZ;		//向きZ
	}KEY;

	//キー情報の構造体
	typedef struct
	{
		int nFrame;					//再生フレーム
		KEY aKey[MAX_PARTS];		//各モデルのキー要素[パーツの総数]
	}KEY_INFO;

	//モーション情報の構造体
	typedef struct
	{
		bool bLoop;						//ループするかどうか
		int NumKey;						//各モデルのキー要素
		KEY_INFO aKeyInfo[MAX_KEY];		//キー情報[キーの総数]
	}INFO;

	static CSkinmesh* Create(void);

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	void SetNULL(void);
	virtual void Update(void);
	virtual void Draw(void);
	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);

	void SetVerPos(D3DXVECTOR3 Pos0, D3DXVECTOR3 Pos1, D3DXVECTOR3 Pos2, D3DXVECTOR3 Pos3);	//頂点座標の設定
	void SetVerTex(void);																	//頂点座標の設定
	void SetAnim(D3DXVECTOR2 Tex);															//テクスチャ座標の設定
	void SetColor(D3DXCOLOR col);															//頂点カラーの設定
	void SetColorA(float A) { m_Color.a = A; SetColor(m_Color); }							//頂点不透明度の設定
	void SetTexture(const char* name);
	D3DMATRIX GetBoneMtxWorld(int nNumber) { return m_Bone[nNumber].m_mtxWorld; }

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
	void SetAddDorw(bool Set) { m_AddDarw = Set; }
	bool GetAddDorw(void) { return m_AddDarw; }
	LPDIRECT3DVERTEXBUFFER9 GetVtxBuff(void) { return m_pVtxBuff; }
	virtual int GetIdx(void) { return m_nIdxTexture; }

	void LoadData(const char* pFilename);

protected:
	LPDIRECT3DTEXTURE9 m_pTexture;			//テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		//頂点バッファへのポインタ
	D3DXMATRIX m_mtxWorld;					//ワールドマトリックス

private:
	void WaitControl();

	CSkinmesh* m_pParent;				//親モデルへのポインタ
	INFO m_aInfo[MAX_INFO];				//モーションの情報[モーションの総数]
	int m_nIdxTexture;
	float m_Width;							//幅
	float m_Height;							//高さ
	bool m_AddDarw;							//加算合成
	D3DXVECTOR3 m_pos;						//現在の位置
	D3DXVECTOR3 m_posOld;					//過去の位置
	D3DXVECTOR3 m_rot;						//向き
	D3DXCOLOR m_Color;						//色
	BONE m_Bone[MAX_BONE];					//ボーン情報
	int m_nNumBone;							//ボーン数
	VERTEXSET m_Vertex[MAX_BONE];			//頂点情報
	int m_nNumVertex;						//頂点数
	CCubeBlock* m_pCubeBlock[MAX_BONE];
};
#endif