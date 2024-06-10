//============================================
//
//	タイトル画面 [title.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _TITLE_H_
#define _TITLE_H_

#include "main.h"
#include "manager.h"

//マクロ定義
#define MAX_SELECT (3)
#define MAX_TITLEUI (3)

//前方宣言
class CObject2D;
class CPlayer3D;
class CBreak_Block3D;
class CBlockUI;
class CTitleUI;
class CFallEffect;
class CObjmeshDome;
class CPlayer;
class CCubeBlock;
class CBoss;
class C2DUIEdit;

//シーンクラス
class CTitle : public CScene
{
public:
	CTitle();
	~CTitle();

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);
	static void SetSelectColor(int SelectCnt);

private:
	void Select(void);

	C2DUIEdit* m_pUIEdit;
	bool m_bEdit;

	static int m_nSelect;
	static D3DXVECTOR2 m_Scroll;
	static D3DXVECTOR2 m_Tex;
	static D3DXCOLOR m_Color;
	static CObject2D *m_pTitle;
	static CObject2D *m_pTitleLogo[4];
	static CObject2D* m_pTitleButton;
	static CObjmeshDome* m_pMeshDome;		//メッシュドーム
	static CPlayer* m_pPlayer;				//プレイヤーのポインタ
	static CCubeBlock* m_pCubeBlock;		//キューブブロック
	static CBoss* m_pBoss;					//ボス

};
#endif