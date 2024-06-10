//========================================================================================
//
// フェード処理[fade.cpp]
// Author: 坂本　翔唯
//
//========================================================================================
#include "fade.h"
#include "renderer.h"

//静的メンバ変数宣言
CFade::FADE CFade::m_fade = FADE_NONE;
CScene::MODE CFade::m_modeNext = CScene::MODE_TITLE;
D3DXCOLOR CFade::m_colorFade = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

//====================================================================
//コンストラクタ
//====================================================================
CFade::CFade()
{
	m_Fade_OK = true;
}

//====================================================================
//デストラクタ
//====================================================================
CFade::~CFade()
{

}

//====================================================================
//フェード画面の初期化処理
//====================================================================
void CFade::Init(CScene::MODE modeNext)
{
	m_fade = FADE_IN;

	m_modeNext = modeNext;

	m_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	m_Fade_OK = true;

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuffFade,
		NULL);

	VERTEX_2D*pVtx;	//頂点ポインタを所得

	//頂点バッファをロックし、両店情報へのポインタを所得
	m_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = m_colorFade;
	pVtx[1].col = m_colorFade;
	pVtx[2].col = m_colorFade;
	pVtx[3].col = m_colorFade;

	//pVtx += 4;	//頂点データのポインタを４つ分進める

	//頂点バッファをアンロックする
	m_pVtxBuffFade->Unlock();

	//モードの設定
	CScene::SetMode(m_modeNext);
}

//====================================================================
//フェードの終了処理
//====================================================================
void CFade::Uninit(void)
{
	//頂点バッファの破棄
	if (m_pVtxBuffFade != NULL)
	{
		m_pVtxBuffFade->Release();
		m_pVtxBuffFade = NULL;
	}
}

//====================================================================
//フェード画面の更新処理
//====================================================================
void CFade::Update(void)
{
	if (m_fade != FADE_NONE)
	{
		if (m_fade == FADE_IN)
		{
			m_colorFade.a -= 0.02f;	//ポリゴンを透明にする
			if (m_colorFade.a <= 0.0f)
			{
				m_colorFade.a = 0.0f;
				m_fade = FADE_NONE;	//何もしていない状態	
			}
		}
		else if (m_fade == FADE_OUT)
		{
			m_colorFade.a += 0.02f;
			if (m_colorFade.a >= 1.0f)
			{
				m_colorFade.a = 1.0f;
				m_fade = FADE_IN;

				m_Fade_OK = true;
				//モードの設定(次の画面に移行)
				CScene::SetMode(m_modeNext);
			}
		}
		VERTEX_2D*pVtx;	//頂点ポインタを所得

						//頂点バッファをロックし、両店情報へのポインタを所得
		m_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

		//頂点カラーの設定
		pVtx[0].col = m_colorFade;
		pVtx[1].col = m_colorFade;
		pVtx[2].col = m_colorFade;
		pVtx[3].col = m_colorFade;

		//頂点バッファをアンロックする
		m_pVtxBuffFade->Unlock();
	}
}

//====================================================================
//フェード画面の描画処理
//====================================================================
void CFade::Draw(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuffFade, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, NULL);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//プリミティブの種類
		0,						//プリミティブ(ポリゴン)数
		2);
}

//====================================================================
//フェード画面の設定処理
//====================================================================
void CFade::SetFade(CScene::MODE modeNext)
{
	if (m_fade == FADE_NONE)
	{
		m_fade = FADE_OUT;

		m_modeNext = modeNext;

		m_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	}
}

//====================================================================
//フェード画面の所得処理
//====================================================================
void CFade::Fade_false(void)
{
	m_Fade_OK = false;
}

//====================================================================
//フェード画面の所得処理
//====================================================================
bool CFade::Fade_OK(void)
{
	return m_Fade_OK;
}