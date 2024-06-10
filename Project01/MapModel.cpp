//============================================
//
//	マップモデル [MapModel.cpp]
//	Author:sakamoto kai
//
//============================================
#include "MapModel.h"
#include "renderer.h"
#include "manager.h"
#include "texture.h"
#include "XModel.h"
#include "CubeBlock.h"

//マクロ定義
//====================================================================
//コンストラクタ
//====================================================================
CMapModel::CMapModel(int nPriority) : CObjectX(nPriority)
{
	SetPos(INITVECTOR3);
	m_CollisionPos = INITVECTOR3;
	m_bCollision = true;
	m_State = STATE_NORMAL;
	m_nStateCount = 0;
	m_Scaling = 1.0f;
	m_fColorA = 0.0f;
	m_pDebugBlock = nullptr;		//デバッグ用ブロック
}

//====================================================================
//デストラクタ
//====================================================================
CMapModel::~CMapModel()
{

}

//====================================================================
//生成処理
//====================================================================
CMapModel* CMapModel::Create(char* pModelName)
{
	CMapModel* pSample = NULL;

	if (pSample == NULL)
	{
		//オブジェクト2Dの生成
		pSample = new CMapModel();
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
HRESULT CMapModel::Init(char* pModelName)
{
	SetType(CObject::TYPE_MAPMODEL);

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
void CMapModel::Uninit(void)
{
	CObjectX::Uninit();
}

//====================================================================
//更新処理
//====================================================================
void CMapModel::Update(void)
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
void CMapModel::TitleUpdate(void)
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
void CMapModel::GameUpdate(void)
{
	D3DXVECTOR3 pos = GetPos();

	//更新前の位置を過去の位置とする
	m_posOld = pos;

	//位置更新
	CObjectX::SetPos(pos);

	//画面外判定
	if (pos.y < 0.0f)
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
void CMapModel::Draw(void)
{
	CObjectX::Draw();
}

//====================================================================
//状態管理
//====================================================================
void CMapModel::StateManager(void)
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

//====================================================================
//ブロックの生成
//====================================================================
void CMapModel::CreateBlock(void)
{
	if (m_pDebugBlock == nullptr)
	{
		m_pDebugBlock = CCubeBlock::Create();
		m_pDebugBlock->SetPos(GetPos());
		m_pDebugBlock->SetSize(GetSize());

		if (m_bCollision == true)
		{
			m_pDebugBlock->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f));
		}
		else
		{
			m_pDebugBlock->SetColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.5f));
		}
	}
}

//====================================================================
//ブロックの更新
//====================================================================
void CMapModel::UpdateBlock(void)
{
	if (m_pDebugBlock != nullptr)
	{
		m_pDebugBlock->SetPos(GetPos());
		m_pDebugBlock->SetSize(GetSize());

		if (m_bCollision == true)
		{
			m_pDebugBlock->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f));
		}
		else
		{
			m_pDebugBlock->SetColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.5f));
		}
	}
}