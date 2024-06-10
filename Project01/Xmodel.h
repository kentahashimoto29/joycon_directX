//============================================
//
//	Xモデルの管理 [Xmodel.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _XMODEL_H_
#define _XMODEL_H_

#include "main.h"

//マクロ定義
#define MAX_MODEL (256)

//Xモデルクラス
class CXModel
{
public:
	typedef struct
	{
		LPD3DXBUFFER m_pBuffMat;
		DWORD m_dwNumMat;
		LPD3DXMESH m_pMesh;
	}XModel;

	CXModel();
	~CXModel();

	HRESULT Load(void);
	void Unload(void);

	int Regist(const char *pFilename);
	LPDIRECT3DTEXTURE9 GetAddress(int nIdx);
	void GetXModelInfo(LPD3DXBUFFER *pBuffMat, DWORD *dwNumMat, LPD3DXMESH *pMesh, int nIdx);
	XModel *GetXModel(int nIdx) { return &m_apXModel[nIdx]; }

private:
	XModel m_apXModel[MAX_MODEL];
	static int m_NumAll;
	static char c_apModelname[MAX_MODEL][128];
};
#endif