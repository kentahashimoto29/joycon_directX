//============================================
//
//	ソリューション[main.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _MAIN_H_
#define _MAIN_H_

//#include <windows.h>
//#include "d3dx9.h"						//描画処理に必要
//#define DIRECTINPUT_VERSION (0x0800)	//ビルド時の警告対処用マクロ
//#include "dinput.h"						//入力処理に必要
//#include "xaudio2.h"					//サウンド処理

//ライブラリのリンク
#pragma comment (lib,"d3d9.lib")		//描画処理に必要
#pragma comment (lib,"d3dx9.lib")		//[d3d9.lib]の拡張ライブラリ
#pragma comment (lib,"dxguid.lib")		//DirectXコンポーネント(部品)使用に必要
#pragma comment (lib,"winmm.lib")		//システム時刻所得に必要
#pragma comment (lib,"dinput8.lib")		//入力処理に必要

//マクロ定義
#define SCREEN_WIDTH (1280)					//ウインドウの幅
#define SCREEN_WIDTHCENTER (640)			//ウインドウの幅中央
#define SCREEN_HEIGHT (720)					//ウインドウの高さ
#define SCREEN_HEIGHTCENTER (360)			//ウインドウの高さ中央
#define MY_FVF  (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_SPECULAR)
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)					//頂点フォーマット
#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)		//頂点フォーマット
#define INITVECTOR3 (D3DXVECTOR3(0.0f,0.0f,0.0f))		//座標のデフォルト
#define SCREEM_CENTER (D3DXVECTOR3(640.0f,360.0f,0.0f))		//スクリーン座標の中心

//頂点構造体[2D]の構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;	//頂点座標
	float rhw;			//座標変換用係数(1.0fで固定)
	D3DCOLOR col;		//頂点カラー
	D3DXVECTOR2 tex;	//テクスチャ座標
}VERTEX_2D;

//頂点構造体[3D]の構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;	//頂点座標
	D3DXVECTOR3 nor;	//法線ベクトル
	D3DCOLOR col;		//頂点カラー
	D3DXVECTOR2 tex;	//テクスチャ座標
}VERTEX_3D;

int GetFps(void);

#endif