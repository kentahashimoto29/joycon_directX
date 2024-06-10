//============================================
//
//	タイトル画面 [title.cpp]
//	Author:sakamoto kai
//
//============================================
#include "title.h"
#include "manager.h"
#include "fade.h"
#include "input.h"
#include "object2D.h"
#include "texture.h"
#include "particle.h"
#include "sound.h"
#include "objmeshDome.h"
#include "player.h"
#include "boss.h"
#include "CubeBlock.h"
#include "2DUI_edit.h"

//静的メンバ変数宣言
CObject2D *CTitle::m_pTitle = NULL;
CObject2D *CTitle::m_pTitleButton = NULL;
CObject2D *CTitle::m_pTitleLogo[4] = {};
int CTitle::m_nSelect = 1;
D3DXVECTOR2 CTitle::m_Scroll = D3DXVECTOR2(0.0f,0.0f);
D3DXVECTOR2 CTitle::m_Tex = D3DXVECTOR2(0.0f, 0.0f);
CObjmeshDome* CTitle::m_pMeshDome = NULL;
CCubeBlock* CTitle::m_pCubeBlock = NULL;
CPlayer* CTitle::m_pPlayer = NULL;
CBoss* CTitle::m_pBoss = NULL;

//====================================================================
//コンストラクタ
//====================================================================
CTitle::CTitle()
{
	m_nSelect = 1;
	m_Tex = D3DXVECTOR2(0.0f, 0.0f);
}

//====================================================================
//デストラクタ
//====================================================================
CTitle::~CTitle()
{

}

//====================================================================
//初期化処理
//====================================================================
HRESULT CTitle::Init(void)
{
	CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_BGM_TITLE);

	CTexture *pTexture = CManager::GetInstance()->GetTexture();

	m_pTitle = CObject2D::Create();
	m_pTitle->SetPos(D3DXVECTOR3(640.0f, 360.0f, 0.0f));
	m_pTitle->SetWidth(1280.0f);
	m_pTitle->SetHeight(720.0f);
	m_pTitle->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_pTitle->SetIdx(pTexture->Regist("data\\TEXTURE\\TitleBG.png"));

	m_pMeshDome = CObjmeshDome::Create();
	m_pMeshDome->SetTexture("data\\TEXTURE\\SkyBG.jpg");
	m_pMeshDome->SetRot(D3DXVECTOR3(D3DX_PI * 0.55f, D3DX_PI * -0.89f, D3DX_PI));

	m_pPlayer = CPlayer::Create();
	m_pPlayer->SetPos(D3DXVECTOR3(-50.0f, 60.0f, -450.0f));
	m_pPlayer->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * -0.9f, 0.0f));

	m_pBoss = CBoss::Create("data\\MODEL\\boss.x");
	m_pBoss->SetPos(D3DXVECTOR3(500.0f, 300.0f, 1000.0f));
	m_pBoss->SetRot(D3DXVECTOR3(0.0f, D3DX_PI * 0.1f, 0.0f));

	m_pCubeBlock = CCubeBlock::Create();
	m_pCubeBlock->SetPos(D3DXVECTOR3(-50.0f, 40.0f, -450.0f));
	m_pCubeBlock->SetRot(D3DXVECTOR3(D3DX_PI * 0.98f, D3DX_PI * -0.9f, 0.0f));
	m_pCubeBlock->SetSize(D3DXVECTOR3(250.0f, 25.0f, 10000.0f));

	m_pTitleButton = CObject2D::Create();
	m_pTitleButton->SetPos(D3DXVECTOR3(640.0f, 670.0f, 0.0f));
	m_pTitleButton->SetWidth(600.0f);
	m_pTitleButton->SetHeight(350.0f);
	m_pTitleButton->SetIdx(pTexture->Regist("data\\TEXTURE\\TitleBotton.png"));

	//UIエディターの生成
	m_pUIEdit = C2DUIEdit::Create();
	m_pUIEdit->LoadData(LOADUI_NAME, INITVECTOR3);

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CTitle::Uninit(void)
{
	if (m_pUIEdit != NULL)
	{
		//エディットモードの終了処理
		m_pUIEdit->Uninit();

		delete m_pUIEdit;
		m_pUIEdit = NULL;
	}

	//全てのオブジェクトの破棄
	CObject::ReleaseAll();
}

//====================================================================
//更新処理
//====================================================================
void CTitle::Update(void)
{
	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_F2) == true)
	{
		m_bEdit = !m_bEdit;
	}

	//エディットモードのオンオフ
	if (m_bEdit)
	{
		//エディットモードの更新処理
		m_pUIEdit->Update();
	}
	else
	{
		m_Scroll.x += 0.01f;
		m_Scroll.y += 0.0f;
		m_pMeshDome->SetScroll(m_Scroll);

		//選択処理
		Select();
	}
}

//====================================================================
//描画処理
//====================================================================
void CTitle::Draw(void)
{

}

//====================================================================
//選択処理
//====================================================================
void CTitle::Select(void)
{
	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_RETURN) == true)
	{
		CManager::GetInstance()->SetUseJoyPad(false);
		CFade::SetFade(CScene::MODE_GAME);
		CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_ENTER_PUSH);
	}
	else if (CManager::GetInstance()->GetInputJoyPad()->GetTrigger(CInputJoypad::BUTTON_A, 0) == true)
	{
		CManager::GetInstance()->SetUseJoyPad(true);
		CFade::SetFade(CScene::MODE_GAME);
		CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_ENTER_PUSH);
	}
}

//====================================================================
//選択項目のカラー変更処理
//====================================================================
void CTitle::SetSelectColor(int nCnt)
{
	m_nSelect = nCnt;
}