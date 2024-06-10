//========================================================================================
//
// フェード処理[fade.h]
// Author: 坂本　翔唯
//
//========================================================================================
#ifndef _FADE_H_
#define _FADE_H_

#include "main.h"
#include "manager.h"

//フェードのクラス宣言
class CFade
{
public:
	//フェードの状態
	typedef enum
	{
		FADE_NONE = 0,	//何もしていない状態
		FADE_IN,		//フェードイン状態
		FADE_OUT,		//フェードイン状態
		FADE_MAX
	}FADE;

	CFade();
	~CFade();

	void Init(CScene::MODE modeNext);
	void Uninit();
	void Update();
	void Draw(void);
	static void SetFade(CScene::MODE modeNext);
	static FADE GetFade(void) { return m_fade; }
	void Fade_false(void);
	bool Fade_OK(void);

protected:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffFade = NULL;		//頂点バッファへのポインタ
	static FADE m_fade;
	static CScene::MODE m_modeNext;
	static D3DXCOLOR m_colorFade;
	bool m_Fade_OK;
};

#endif
