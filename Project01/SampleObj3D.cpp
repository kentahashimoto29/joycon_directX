//============================================
//
//	オブジェクト3Dのサンプル [SampleObj3D.cpp]
//	Author:sakamoto kai
//
//============================================
#include "SampleObj3D.h"
#include "renderer.h"
#include "manager.h"

//マクロ定義
#define NUMBER_WIGHT (100.0f)		//横幅
#define NUMBER_HEIGHT (100.0f)		//縦幅

//====================================================================
//コンストラクタ
//====================================================================
CSampleObj3D::CSampleObj3D(int nPriority) : CObject3D(nPriority)
{
	SetWidth(NUMBER_WIGHT);
	SetHeight(NUMBER_HEIGHT);
	m_nSample = 0;
}

//====================================================================
//デストラクタ
//====================================================================
CSampleObj3D::~CSampleObj3D()
{

}

//====================================================================
//生成処理
//====================================================================
CSampleObj3D* CSampleObj3D::Create(void)
{
	CSampleObj3D* pSample3D = NULL;

	if (pSample3D == NULL)
	{
		//オブジェクト2Dの生成
		pSample3D = new CSampleObj3D();
	}

	//オブジェクトの初期化処理
	if (FAILED(pSample3D->Init()))
	{//初期化処理が失敗した場合
		return NULL;
	}

	return pSample3D;
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CSampleObj3D::Init(void)
{
	CObject3D::Init();

	//テクスチャ設定
	SetTexture("data\\TEXTURE\\Number01.png");

	//新しくcppを作成した時は新しいTYPEを列挙に追加して指定すること
	SetType(CObject::TYPE_SAMPLE);

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CSampleObj3D::Uninit(void)
{
	CObject3D::Uninit();
}

//====================================================================
//更新処理
//====================================================================
void CSampleObj3D::Update(void)
{
	//頂点情報の更新
	CObject3D::Update();

	//----頂点情報を変更したい場合は以下のコードを使用すること---- -
	//VERTEX_3D * pVtx;	//頂点ポインタを所得

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

	////法線
	//pVtx[0].nor;
	//pVtx[1].nor;
	//pVtx[2].nor;
	//pVtx[3].nor;

	////頂点バッファをアンロックする
	//GetVtxBuff()->Unlock();
}

//====================================================================
//描画処理
//====================================================================
void CSampleObj3D::Draw(void)
{
	CObject3D::Draw();
}