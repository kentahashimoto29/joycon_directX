//============================================
//
//	レンダラー[renderer.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _RENSERER_H_
#define _RENSERER_H_

#include "main.h"

//レンダラークラス
class CRenderer
{
public:
	CRenderer();	//コンストラクタ
	~CRenderer();	//デストラクタ

	//レンダラーの処理
	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow); 	//返り値がHRESULTなのは頂点バッファの生成などに失敗したとき確認のため
	void Uninit(void);
	void Update(void);
	void Draw(void);
	LPDIRECT3DDEVICE9 GetDevice(void);

	void ChageTarget(D3DXVECTOR3 posV, D3DXVECTOR3 posR, D3DXVECTOR3 vecU);
	LPDIRECT3DTEXTURE9 GetTextureMT(void) { return m_apTextureMT[0]; }
	void SetAfterImage(bool Set) { m_bAfterimage = Set; }
	bool GetAfterImage(void) { return m_bAfterimage; }

private:

	LPDIRECT3D9 m_pD3D;					//Direct3Dオブジェクトへのポインタ
	LPDIRECT3DDEVICE9 m_pD3DDevice;		//Direct3Dデバイスへのポインタ

	LPDIRECT3DTEXTURE9 m_apTextureMT[2];	//レンダリングターゲット用テクスチャ
	LPDIRECT3DSURFACE9 m_apRenderMT[2];		//テクスチャレンダリング用インターフェイス
	LPDIRECT3DSURFACE9 m_pZBuffMT;			//テクスチャレンダリング用Zバッファ
	D3DVIEWPORT9 m_ViewportMT;				//テクスチャレンダリング用ビューポート

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffMT;	//フィードバック用ポリゴンの頂点バッファ
	bool m_bAfterimage;						//残像のオンオフ
};

#endif

////============================================
////
////	レンダラー[renderer.h]
////	Author:sakamoto kai
////
////============================================
//#ifndef _RENSERER_H_
//#define _RENSERER_H_
//
//#include "main.h"
//
////レンダラークラス
//class CRenderer
//{
//public:
//	CRenderer();	//コンストラクタ
//	~CRenderer();	//デストラクタ
//
//	//レンダラーの処理
//	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow); 	//返り値がHRESULTなのは頂点バッファの生成などに失敗したとき確認のため
//	void Uninit(void);
//	void Update(void);
//	void Draw(void);
//	LPDIRECT3DDEVICE9 GetDevice(void);
//
//private:
//
//	LPDIRECT3D9 m_pD3D;					//Direct3Dオブジェクトへのポインタ
//	LPDIRECT3DDEVICE9 m_pD3DDevice;		//Direct3Dデバイスへのポインタ
//};
//
//#endif