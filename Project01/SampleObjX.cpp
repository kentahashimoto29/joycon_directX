//============================================
//
//	オブジェクトＸモデルのサンプル [SampleObjX.cpp]
//	Author:sakamoto kai
//
//============================================
#include "SampleObjX.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "XModel.h"

//マクロ定義
#define COLLISION_SIZE (D3DXVECTOR3(20.0f,20.0f,20.0f))		//当たり判定
//====================================================================
//コンストラクタ
//====================================================================
CSampleObjectX::CSampleObjectX(int nPriority) : CObjectX(nPriority)
{
	SetSize(COLLISION_SIZE);
	SetPos(INITVECTOR3);
	m_nIdxXModel = NULL;			//マテリアルの数
	m_CollisionPos = INITVECTOR3;
	m_bCollision = false;
	m_State = STATE_NORMAL;
	m_nStateCount = 0;
	m_Scaling = 1.0f;
	m_fColorA = 0.0f;
}

//====================================================================
//デストラクタ
//====================================================================
CSampleObjectX::~CSampleObjectX()
{

}

//====================================================================
//生成処理
//====================================================================
CSampleObjectX* CSampleObjectX::Create(char* pModelName)
{
	CSampleObjectX* pSample = NULL;

	if (pSample == NULL)
	{
		//オブジェクト2Dの生成
		pSample = new CSampleObjectX();
	}

	//オブジェクトの初期化処理
	if (FAILED(pSample->Init(pModelName)))
	{//初期化処理が失敗した場合
		return NULL;
	}

	return pSample;
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CSampleObjectX::Init(char* pModelName)
{
	SetType(CObject::TYPE_ENEMY3D);

	CObjectX::Init(pModelName);

	//モードごとに初期値を設定出来る
	switch (CScene::GetMode())
	{
	case CScene::MODE_TITLE:
		break;

	case CScene::MODE_GAME:
		break;

	case CScene::MODE_RESULT:
		break;
	}


	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CSampleObjectX::Uninit(void)
{
	CObjectX::Uninit();
}

//====================================================================
//更新処理
//====================================================================
void CSampleObjectX::Update(void)
{
	switch (CScene::GetMode())
	{
	case CScene::MODE_TITLE:
		TitleUpdate();
		break;

	case CScene::MODE_GAME:
		GameUpdate();
		break;

	case CScene::MODE_RESULT:
		break;
	}
}

//====================================================================
//タイトルでの更新処理
//====================================================================
void CSampleObjectX::TitleUpdate(void)
{
	D3DXVECTOR3 pos = GetPos();

	//位置更新
	pos += m_move;

	SetPos(pos);

	//頂点情報の更新
	CObjectX::Update();
}

//====================================================================
//ゲームでの更新処理
//====================================================================
void CSampleObjectX::GameUpdate(void)
{
	//更新前の位置を過去の位置とする
	m_posOld = m_pos;

	//位置更新
	CObjectX::SetPos(m_pos);
	CObjectX::SetRot(m_rot);

	//画面外判定
	if (m_pos.y < 0.0f)
	{

	}

	//大きさの設定
	SetScaling(D3DXVECTOR3(m_Scaling, m_Scaling, m_Scaling));

	//状態管理
	StateManager();

	//頂点情報の更新
	CObjectX::Update();
}

//====================================================================
//描画処理
//====================================================================
void CSampleObjectX::Draw(void)
{
	CObjectX::Draw();
}

//====================================================================
//状態管理
//====================================================================
void CSampleObjectX::StateManager(void)
{
	switch (m_State)
	{
	case STATE_NORMAL:
		break;
	case STATE_ACTION:
		break;
	}

	if (m_nStateCount > 0)
	{
		m_nStateCount--;
	}
}