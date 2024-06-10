//============================================
//
//	レンダラー[renderer.cpp]
//	Author:sakamoto kai
//
//============================================
#include "renderer.h"
#include "object.h"
#include "debugproc.h"
#include "manager.h"
#include "camera.h"
#include "fade.h"
#include "Pause.h"

//====================================================================
//コンストラクタ
//====================================================================
CRenderer::CRenderer()
{
	m_pD3D = NULL;					//Direct3Dオブジェクトへのポインタ
	m_pD3DDevice = NULL;			//Direct3Dデバイスへのポインタ
	m_bAfterimage = false;
}

//====================================================================
//デストラクタ
//====================================================================
CRenderer::~CRenderer()
{

}

//====================================================================
//初期化処理
//====================================================================
HRESULT CRenderer::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;			//ディスプレイモード
	D3DPRESENT_PARAMETERS d3dpp;	//プレゼンテーションパラメータ

	//Direct3Dオブジェクトの生成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == NULL)
	{
		return E_FAIL;
	}

	//現在のディスプレイモードを所得
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));	//パラメータのゼロクリア

	d3dpp.BackBufferWidth = SCREEN_WIDTH;							//ゲーム画面のサイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;							//ゲーム画面のサイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;							//バックバッファの形式
	d3dpp.BackBufferCount = 1;										//バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;						//ダブルバッファの切り替え(映像信号に同期)
	d3dpp.EnableAutoDepthStencil = TRUE;							//デプスバッファとシテンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;						//デプスバッファを16bitとして扱う
	d3dpp.Windowed = bWindow;										//ウインドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	//リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		//インターバル

	//Direct3dデバイスの作成
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&m_pD3DDevice)))
	{
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&m_pD3DDevice)))
		{
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&m_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}
	//レンダーステートの設定
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//サンプラーステート(テクスチャの拡縮補完)の設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	//テクスチャステージステート(テクスチャのアルファブレンド)の設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	LPDIRECT3DSURFACE9 pRenderDef, pZBuffDef;

	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		//レンダリングターゲット用テクスチャの生成
		m_pD3DDevice->CreateTexture(
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			1,
			D3DUSAGE_RENDERTARGET,
			D3DFMT_A8R8G8B8,
			D3DPOOL_DEFAULT,
			&m_apTextureMT[nCnt],
			NULL);

		//テクスチャレンダリング用インターフェイスの生成
		m_apTextureMT[nCnt]->GetSurfaceLevel(0, &m_apRenderMT[nCnt]);

		//テクスチャレンダリング用Zバッファの生成
		m_pD3DDevice->CreateDepthStencilSurface(
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			D3DFMT_D16,
			D3DMULTISAMPLE_NONE,
			0,
			TRUE,
			&m_pZBuffMT,
			NULL
		);

		//現在のレンダリングターゲットを取得(保存)
		m_pD3DDevice->GetRenderTarget(0, &pRenderDef);

		//現在のZバッファを取得(保存)
		m_pD3DDevice->GetDepthStencilSurface(&pZBuffDef);

		//現在のレンダリングターゲットを取得(保存)
		m_pD3DDevice->SetRenderTarget(0, m_apRenderMT[nCnt]);

		//現在のZバッファを取得(保存)
		m_pD3DDevice->SetDepthStencilSurface(m_pZBuffMT);

		//レンダリングターゲット用のテクスチャのクリア
		m_pD3DDevice->Clear(0, NULL,
			(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
			D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

		//現在のレンダリングターゲットを取得(保存)
		m_pD3DDevice->SetRenderTarget(0, pRenderDef);

		//現在のZバッファを取得(保存)
		m_pD3DDevice->SetDepthStencilSurface(pZBuffDef);
	}

	//テクスチャレンダリング用ビューポートの設定
	m_ViewportMT.X = 0;
	m_ViewportMT.Y = 0;
	m_ViewportMT.Width = SCREEN_WIDTH;
	m_ViewportMT.Height = SCREEN_HEIGHT;
	m_ViewportMT.MinZ = 0.0f;
	m_ViewportMT.MaxZ = 1.0f;

	//フィードバック用ポリゴンの生成
	if (FAILED(m_pD3DDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuffMT,
		NULL)))
	{
		return E_FAIL;
	}

	VERTEX_2D* pVtx;	//頂点ポインタを所得

	//頂点バッファをロックし、両店情報へのポインタを所得
	m_pVtxBuffMT->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定 
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(1280.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 720.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(1280.0f, 720.0f, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.99f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.99f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.99f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.99f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuffMT->Unlock();

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CRenderer::Uninit(void)
{
	//デバック表示用フォントの破棄
	if (m_pD3DDevice != NULL)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}

	//Direct3Dオブジェクトの破棄
	if (m_pD3D != NULL)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}

	for (int nCnt = 0; nCnt < 2; nCnt++)
	{
		//レンダリングターゲット用テクスチャの破棄
		if (m_apTextureMT[nCnt] != NULL)
		{
			m_apTextureMT[nCnt]->Release();
			m_apTextureMT[nCnt] = NULL;
		}

		//テクスチャレンダリング用インターフェイスの破棄
		if (m_apRenderMT[nCnt] != NULL)
		{
			m_apRenderMT[nCnt]->Release();
			m_apRenderMT[nCnt] = NULL;
		}
	}

	//テクスチャレンダリング用Zバッファの破棄
	if (m_pZBuffMT != NULL)
	{
		m_pZBuffMT->Release();
		m_pZBuffMT = NULL;
	}

	//フィードバック用ポリゴンの破棄
	if (m_pVtxBuffMT != NULL)
	{
		m_pVtxBuffMT->Release();
		m_pVtxBuffMT = NULL;
	}
}

//====================================================================
//更新処理
//====================================================================
void CRenderer::Update(void)
{
	//全てのオブジェクト2Dの更新処理
	CObject::UpdateAll();
}

//====================================================================
//描画処理
//====================================================================
void CRenderer::Draw(void)
{
	//デバッグ表示の取得
	CDebugProc* pDebugProc = CManager::GetInstance()->GetDebugProc();

	//カメラの取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();

	//フェードの取得
	CFade* pFade = CManager::GetInstance()->GetFade();

	if (m_bAfterimage == true)
	{
		LPDIRECT3DSURFACE9 pRenderWk;
		LPDIRECT3DTEXTURE9 pTextureWk;

		//画面クリア(バックバッファとZバッファのクリア)
		m_pD3DDevice->Clear(0, NULL,
			(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
			D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

		//-----------------------------------------------------------------------------------

		LPDIRECT3DSURFACE9 pRenderDef, pZBuffDef;
		D3DVIEWPORT9 viewportDef;
		D3DXMATRIX mtxViewDef, mtxProjectionDef;

		//現在のレンダリングターゲットを取得(保存)
		m_pD3DDevice->GetRenderTarget(0, &pRenderDef);

		//現在のZバッファを取得(保存)
		m_pD3DDevice->GetDepthStencilSurface(&pZBuffDef);

		//現在のビューポートを取得(保存)
		m_pD3DDevice->GetViewport(&viewportDef);

		CManager::GetInstance()->GetRenderer()->ChageTarget(pCamera->GetPosV(), pCamera->GetPosR(), pCamera->GetVecU());

		//レンダリングターゲット用のテクスチャのクリア
		m_pD3DDevice->Clear(0, NULL,
			(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
			D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

		//-----------------------------------------------------------------------------------

		//描画開始
		if (SUCCEEDED(m_pD3DDevice->BeginScene()))
		{//描画処理が成功した場合

			//カメラの設定
			pCamera->SetCamera();

			//全てのオブジェクト2Dの描画処理
			CObject::DrawAll();

			//-----------------------------------------------------------------------------------
			//頂点バッファをデータストリームに設定
			m_pD3DDevice->SetStreamSource(0, m_pVtxBuffMT, 0, sizeof(VERTEX_2D));

			//頂点フォーマットの設定
			m_pD3DDevice->SetFVF(FVF_VERTEX_2D);

			//テクスチャの設定
			m_pD3DDevice->SetTexture(0, m_apTextureMT[1]);

			//ポリゴンの描画
			m_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
				0,						//描画の開始位置
				2);						//プリミティブ(ポリゴン)数
			//-----------------------------------------------------------------------------------

			//フェードの更新処理
			pFade->Draw();

			//デバッグ表示の描画処理
			pDebugProc->Draw();

			//描画終了
			m_pD3DDevice->EndScene();
		}
		////バックバッファとフロントバッファの入れ替え
		//m_pD3DDevice->Present(NULL, NULL, NULL, NULL);

		//-----------------------------------------------------------------------------------

		//元のレンダリングターゲットに戻す
		m_pD3DDevice->SetRenderTarget(0, pRenderDef);

		//元のZバッファに戻す
		m_pD3DDevice->SetDepthStencilSurface(pZBuffDef);

		//元のビューポートに戻す
		m_pD3DDevice->SetViewport(&viewportDef);

		//-----------------------------------------------------------------------------------

		//描画開始
		if (SUCCEEDED(m_pD3DDevice->BeginScene()))
		{//描画処理が成功した場合

			//-----------------------------------------------------------------------------------

			//頂点バッファをデータストリームに設定
			m_pD3DDevice->SetStreamSource(0, m_pVtxBuffMT, 0, sizeof(VERTEX_2D));

			//頂点フォーマットの設定
			m_pD3DDevice->SetFVF(FVF_VERTEX_2D);

			//テクスチャの設定
			m_pD3DDevice->SetTexture(0, m_apTextureMT[0]);

			//ポリゴンの描画
			m_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
				0,						//描画の開始位置
				2);						//プリミティブ(ポリゴン)数
			//-----------------------------------------------------------------------------------

			//描画終了
			m_pD3DDevice->EndScene();
		}
		//バックバッファとフロントバッファの入れ替え
		m_pD3DDevice->Present(NULL, NULL, NULL, NULL);

		//テクスチャの[0]と[1]を入れ替える
		pTextureWk = m_apTextureMT[0];
		m_apTextureMT[0] = m_apTextureMT[1];
		m_apTextureMT[1] = pTextureWk;

		pRenderWk = m_apRenderMT[0];
		m_apRenderMT[0] = m_apRenderMT[1];
		m_apRenderMT[1] = pRenderWk;
	}
	else
	{
		//画面クリア(バックバッファとZバッファのクリア)
		m_pD3DDevice->Clear(0, NULL,
			(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
			D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

		//描画開始
		if (SUCCEEDED(m_pD3DDevice->BeginScene()))
		{//描画処理が成功した場合

			//カメラの設定
			pCamera->SetCamera();

			//全てのオブジェクト2Dの描画処理
			CObject::DrawAll();

			//フェードの更新処理
			pFade->Draw();

			//デバッグ表示の描画処理
			pDebugProc->Draw();

			//描画終了
			m_pD3DDevice->EndScene();
		}
		//バックバッファとフロントバッファの入れ替え
		m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
	}
}

//====================================================================
//レンダリングターゲットの変更
//====================================================================
void CRenderer::ChageTarget(D3DXVECTOR3 posV, D3DXVECTOR3 posR, D3DXVECTOR3 vecU)
{
	D3DXMATRIX mtxView, mtxProjection;
	float fAspect;

	//レンダリングターゲットを生成したテクスチャに設定
	m_pD3DDevice->SetRenderTarget(0, m_apRenderMT[0]);

	//Zバッファを生成したZバッファに設定
	m_pD3DDevice->SetDepthStencilSurface(m_pZBuffMT);

	//テクスチャレンダリング用のビューポートを設定
	m_pD3DDevice->SetViewport(&m_ViewportMT);

	//ビューマトリックスの初期化
	D3DXMatrixIdentity(&mtxView);

	//ビューマトリックスの作成
	D3DXMatrixLookAtLH(&mtxView,
		&posV,
		&posR,
		&vecU);

	//ビューマトリックスの設定
	m_pD3DDevice->SetTransform(D3DTS_WORLD, &mtxView);

	//プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&mtxProjection);

	//プロジェクションマトリックスの作成
	fAspect = (float)m_ViewportMT.Width / (float)m_ViewportMT.Height;
	D3DXMatrixPerspectiveFovLH(&mtxProjection,
		D3DXToRadian(45.0f),
		fAspect,
		10.0f,
		12000.0f);

	//プロジェクションマッピングの設定
	m_pD3DDevice->SetTransform(D3DTS_WORLD, &mtxProjection);
}

//====================================================================
//デバイスの取得
//====================================================================
LPDIRECT3DDEVICE9 CRenderer::GetDevice(void)
{
	return m_pD3DDevice;
}

////============================================
////
////	レンダラー[renderer.cpp]
////	Author:sakamoto kai
////
////============================================
//#include "renderer.h"
//#include "object.h"
//#include "debugproc.h"
//#include "manager.h"
//#include "camera.h"
//#include "fade.h"
//#include "Pause.h"
//
////====================================================================
////コンストラクタ
////====================================================================
//CRenderer::CRenderer()
//{
//	m_pD3D = NULL;					//Direct3Dオブジェクトへのポインタ
//	m_pD3DDevice = NULL;			//Direct3Dデバイスへのポインタ
//}
//
////====================================================================
////デストラクタ
////====================================================================
//CRenderer::~CRenderer()
//{
//
//}
//
////====================================================================
////初期化処理
////====================================================================
//HRESULT CRenderer::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
//{
//	D3DDISPLAYMODE d3ddm;			//ディスプレイモード
//	D3DPRESENT_PARAMETERS d3dpp;	//プレゼンテーションパラメータ
//
//									//Direct3Dオブジェクトの生成
//	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
//	if (m_pD3D == NULL)
//	{
//		return E_FAIL;
//	}
//
//	//現在のディスプレイモードを所得
//	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
//	{
//		return E_FAIL;
//	}
//
//	//デバイスのプレゼンテーションパラメータの設定
//	ZeroMemory(&d3dpp, sizeof(d3dpp));	//パラメータのゼロクリア
//
//	d3dpp.BackBufferWidth = SCREEN_WIDTH;							//ゲーム画面のサイズ(幅)
//	d3dpp.BackBufferHeight = SCREEN_HEIGHT;							//ゲーム画面のサイズ(高さ)
//	d3dpp.BackBufferFormat = d3ddm.Format;							//バックバッファの形式
//	d3dpp.BackBufferCount = 1;										//バックバッファの数
//	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;						//ダブルバッファの切り替え(映像信号に同期)
//	d3dpp.EnableAutoDepthStencil = TRUE;							//デプスバッファとシテンシルバッファを作成
//	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;						//デプスバッファを16bitとして扱う
//	d3dpp.Windowed = bWindow;										//ウインドウモード
//	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	//リフレッシュレート
//	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;		//インターバル
//
//	//Direct3dデバイスの作成
//	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
//		D3DDEVTYPE_HAL,
//		hWnd,
//		D3DCREATE_HARDWARE_VERTEXPROCESSING,
//		&d3dpp,
//		&m_pD3DDevice)))
//	{
//		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
//			D3DDEVTYPE_HAL,
//			hWnd,
//			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
//			&d3dpp,
//			&m_pD3DDevice)))
//		{
//			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
//				D3DDEVTYPE_REF,
//				hWnd,
//				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
//				&d3dpp,
//				&m_pD3DDevice)))
//			{
//				return E_FAIL;
//			}
//		}
//	}
//	//レンダーステートの設定
//	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
//	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
//	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
//	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
//
//	//サンプラーステート(テクスチャの拡縮補完)の設定
//	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
//	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
//	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
//	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
//
//	//テクスチャステージステート(テクスチャのアルファブレンド)の設定
//	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
//	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
//	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);
//
//	return S_OK;
//}
//
////====================================================================
////終了処理
////====================================================================
//void CRenderer::Uninit(void)
//{
//	//デバック表示用フォントの破棄
//	if (m_pD3DDevice != NULL)
//	{
//		m_pD3DDevice->Release();
//		m_pD3DDevice = NULL;
//	}
//
//	//Direct3Dオブジェクトの破棄
//	if (m_pD3D != NULL)
//	{
//		m_pD3D->Release();
//		m_pD3D = NULL;
//	}
//}
//
////====================================================================
////更新処理
////====================================================================
//void CRenderer::Update(void)
//{
//	//全てのオブジェクト2Dの更新処理
//	CObject::UpdateAll();
//}
//
////====================================================================
////描画処理
////====================================================================
//void CRenderer::Draw(void)
//{
//	//デバッグ表示の取得
//	CDebugProc *pDebugProc = CManager::GetInstance()->GetDebugProc();
//
//	//カメラの取得
//	CCamera *pCamera = CManager::GetInstance()->GetCamera();
//
//	//フェードの取得
//	CFade *pFade = CManager::GetInstance()->GetFade();
//
//	//画面クリア(バックバッファとZバッファのクリア)
//	m_pD3DDevice->Clear(0, NULL,
//		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
//		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);
//	//描画開始
//	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
//	{//描画処理が成功した場合
//
//		//カメラの設定
//		pCamera->SetCamera();
//
//		//全てのオブジェクト2Dの描画処理
//		CObject::DrawAll();
//
//		//フェードの更新処理
//		pFade->Draw();
//
//		//デバッグ表示の描画処理
//		pDebugProc->Draw();
//
//		//描画終了
//		m_pD3DDevice->EndScene();
//	}
//	//バックバッファとフロントバッファの入れ替え
//	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
//}
//
////====================================================================
////デバイスの取得
////====================================================================
//LPDIRECT3DDEVICE9 CRenderer::GetDevice(void)
//{
//	return m_pD3DDevice;
//}