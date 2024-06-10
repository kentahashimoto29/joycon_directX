//============================================
//
//	オブジェクト2Dのサンプル [SampleObj2D.cpp]
//	Author:sakamoto kai
//
//============================================
#include "SampleObj2D.h"
#include "renderer.h"
#include "manager.h"

//マクロ定義
#define NUMBER_WIGHT (100.0f)		//横幅
#define NUMBER_HEIGHT (100.0f)		//縦幅

//====================================================================
//コンストラクタ
//====================================================================
CSampleObj2D::CSampleObj2D(int nPriority) : CObject2D(nPriority)
{
	SetWidth(NUMBER_WIGHT);
	SetHeight(NUMBER_HEIGHT);
	m_nSample = 0;
}

//====================================================================
//デストラクタ
//====================================================================
CSampleObj2D::~CSampleObj2D()
{

}

//====================================================================
//生成処理
//====================================================================
CSampleObj2D* CSampleObj2D::Create(int nPriority)
{
	CSampleObj2D* pSample2D = NULL;

	if (pSample2D == NULL)
	{
		//オブジェクト2Dの生成
		pSample2D = new CSampleObj2D(nPriority);
	}

	//オブジェクトの初期化処理
	if (FAILED(pSample2D->Init()))
	{//初期化処理が失敗した場合
		return NULL;
	}

	return pSample2D;
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CSampleObj2D::Init(void)
{
	CObject2D::Init();

	//テクスチャ設定
	SetTexture("data\\TEXTURE\\Number01.png");

	//新しくcppを作成した時は新しいTYPEを列挙に追加して指定すること
	SetType(CObject::TYPE_SAMPLE);

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CSampleObj2D::Uninit(void)
{
	CObject2D::Uninit();
}

//====================================================================
//更新処理
//====================================================================
void CSampleObj2D::Update(void)
{
	//頂点情報の更新
	CObject2D::Update();


	//----頂点情報を変更したい場合は以下のコードを使用すること-----
	//VERTEX_2D* pVtx;	//頂点ポインタを所得

	////頂点バッファをロックし、両店情報へのポインタを所得
	//GetVtxBuff()->Lock(0, 0, (void**)&pVtx, 0);

	////位置
	//pVtx[0].pos;
	//pVtx[1].pos;
	//pVtx[2].pos;
	//pVtx[3].pos;

	////色
	//pVtx[0].col;
	//pVtx[1].col;
	//pVtx[2].col;
	//pVtx[3].col;

	////テクスチャ
	//pVtx[0].tex;
	//pVtx[1].tex;
	//pVtx[2].tex;
	//pVtx[3].tex;

	////頂点バッファをアンロックする
	//GetVtxBuff()->Unlock();
}

//====================================================================
//描画処理
//====================================================================
void CSampleObj2D::Draw(void)
{
	CObject2D::Draw();
}