//============================================
//
//	テクスチャの管理 [texture.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "main.h"

//マクロ定義
#define MAX_TEXTURE (512)

//テクスチャクラス
class CTexture
{
public:
	CTexture();
	~CTexture();

	HRESULT Load(void);
	void Unload(void);

	int Regist(const char *pFilename);
	void XModelRegist(int *nTexIdx, int nXModelIdx);
	LPDIRECT3DTEXTURE9 GetAddress(int nIdx);
	const char *GetTextureName(int Idx) { return &c_apTexturename[Idx][0]; }

private:
	LPDIRECT3DTEXTURE9 m_apTexture[MAX_TEXTURE];
	static int m_NumAll;
	static char c_apTexturename[MAX_TEXTURE][128];
};
#endif