//============================================
//
//	リザルト画面 [result.cpp]
//	Author:sakamoto kai
//
//============================================
#include "result.h"
#include "manager.h"
#include "fade.h"
#include "input.h"
#include "object2D.h"
#include "texture.h"
#include "sound.h"
#include "camera.h"
#include "ranking.h"
#include "number.h"

//マクロ定義
#define START_OK ("STARTSETSTAGE")	//スタートメッセージがあるかどうかの確認
#define END_SET_OK ("ENDSETSTAGE")	//エンドメッセージがあるかどうかの確認
#define SCORE_POSX (300.0f)
#define SCORE_MOVEX (20.1f)
#define TIME_POS (D3DXVECTOR3(260.0f,73.0f,0.0f))
#define TIME_DISTANCE (25.0f)
#define LIFE_POS (D3DXVECTOR3(257.0f,115.0f,0.0f))
#define LIFE_DISTANCE (25.0f)

//静的メンバ変数宣言
CObject2D *CResult::m_pResult = NULL;
CRanking* CResult::m_pLifeRanking = NULL;
CRanking *CResult::m_pTimeRanking = NULL;
CObject2D *CResult::m_ClearText = NULL;
CObject2D *CResult::m_NormalText = NULL;
CObject2D* CResult::m_DeathText = NULL;
CObject2D *CResult::m_Rank = NULL;
CScore *CResult::m_DeathScore = NULL;
CNumber* CResult::m_apLife[6] = {};
CNumber* CResult::m_apTime[6] = {};
bool CResult::m_Appear = false;
int CResult::m_AddScoreCount = 0;
int CResult::m_LifeData = 0;
int CResult::m_TimeData = 0;
int CResult::m_RankData = 0;
CEdit *CResult::m_pEdit = NULL;

//====================================================================
//コンストラクタ
//====================================================================
CResult::CResult()
{
	m_AddScoreCount = 0;
	m_LifeData = 0;
	m_TimeData = 0;
	m_RankData = 0;
}

//====================================================================
//デストラクタ
//====================================================================
CResult::~CResult()
{

}

//====================================================================
//初期化処理
//====================================================================
HRESULT CResult::Init(void)
{
	CTexture *pTexture = CManager::GetInstance()->GetTexture();;
	CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_BGM_RESULT);

	m_LifeData = CManager::GetInstance()->GetEndScore();
	m_TimeData = CManager::GetInstance()->GetEndTime();

	//LoadBlock("data\\TXT\\Result02", D3DXVECTOR3(-440.0f - 700.0f, 400.0f, 200.0f));
	//LoadBlock("data\\TXT\\Result02", D3DXVECTOR3(-440.0f + 700.0f, 400.0f, 200.0f));

	m_pResult = CObject2D::Create();
	m_pResult->SetPos(D3DXVECTOR3(640.0f, 360.0f, 0.0f));
	m_pResult->SetWidth(1280.0f);
	m_pResult->SetHeight(720.0f);
	m_pResult->SetColor(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f));
	m_pResult->SetTexture("data\\TEXTURE\\shot01.png");

	m_ClearText = CObject2D::Create();
	m_ClearText->SetPos(D3DXVECTOR3(640.0f, 360.0f, 0.0f));
	m_ClearText->SetWidth(1280.0f);
	m_ClearText->SetHeight(720.0f);
	if (CManager::GetInstance()->GetGameClear() == true)
	{
		m_ClearText->SetTexture("data\\TEXTURE\\result.png");
	}
	else
	{
		m_ClearText->SetTexture("data\\TEXTURE\\result.png");
	}

	m_pLifeRanking = CRanking::Create("data\\TXT\\LifeRanking.txt");
	m_pLifeRanking->SetPos(D3DXVECTOR3(860.0f,447.0f,0.0f));
	m_pLifeRanking->SetRanking(m_LifeData);

	m_pTimeRanking = CRanking::Create("data\\TXT\\TimeRanking.txt");
	m_pTimeRanking->SetPos(D3DXVECTOR3(280.0f, 447.0f, 0.0f));
	m_pTimeRanking->SetRanking(m_TimeData);
	m_pTimeRanking->SetUseTime(true);

	for (int nCntObject = 0; nCntObject < 6; nCntObject++)
	{
		//数字の生成
		m_apLife[nCntObject] = CNumber::Create();
		m_apLife[nCntObject]->SetPos(D3DXVECTOR3(LIFE_POS.x + (nCntObject * LIFE_DISTANCE), LIFE_POS.y, LIFE_POS.z));
		m_apLife[nCntObject]->SetWidth(35.0f);
		m_apLife[nCntObject]->SetHeight(35.0f);
	}

	for (int nCntObject = 0; nCntObject < 6; nCntObject++)
	{
		//数字の生成
		m_apTime[nCntObject] = CNumber::Create();

		if (nCntObject < 2)
		{
			m_apTime[nCntObject]->SetPos(D3DXVECTOR3(TIME_POS.x + (nCntObject * TIME_DISTANCE) - 3.0f, TIME_POS.y, TIME_POS.z));
			m_apTime[nCntObject]->SetWidth(35.0f);
			m_apTime[nCntObject]->SetHeight(35.0f);
		}
		else if (nCntObject < 4)
		{
			m_apTime[nCntObject]->SetPos(D3DXVECTOR3(TIME_POS.x + (nCntObject * TIME_DISTANCE), TIME_POS.y, TIME_POS.z));
			m_apTime[nCntObject]->SetWidth(35.0f);
			m_apTime[nCntObject]->SetHeight(35.0f);
		}
		else
		{
			m_apTime[nCntObject]->SetPos(D3DXVECTOR3(TIME_POS.x + (nCntObject * (TIME_DISTANCE - 7.0f)) + 27.0f, TIME_POS.y + 5.0f, TIME_POS.z));
			m_apTime[nCntObject]->SetWidth(25.0f);
			m_apTime[nCntObject]->SetHeight(25.0f);
		}
	}

	m_apLife[0]->SetNumber(m_LifeData % 1000000 / 100000);
	m_apLife[1]->SetNumber(m_LifeData % 100000 / 10000);
	m_apLife[2]->SetNumber(m_LifeData % 10000 / 1000);
	m_apLife[3]->SetNumber(m_LifeData % 1000 / 100);
	m_apLife[4]->SetNumber(m_LifeData % 100 / 10);
	m_apLife[5]->SetNumber(m_LifeData % 10 / 1);

	m_apTime[0]->SetNumber(m_TimeData % 1000000 / 100000);
	m_apTime[1]->SetNumber(m_TimeData % 100000 / 10000);
	m_apTime[2]->SetNumber(m_TimeData % 10000 / 1000);
	m_apTime[3]->SetNumber(m_TimeData % 1000 / 100);
	m_apTime[4]->SetNumber(m_TimeData % 100 / 10);
	m_apTime[5]->SetNumber(m_TimeData % 10 / 1);

	if (m_LifeData > 250)
	{
		m_RankData += 1;
	}
	if (m_LifeData > 500)
	{
		m_RankData += 1;
	}
	if (m_LifeData > 2750)
	{
		m_RankData += 1;
	}
	if (m_LifeData > 1000)
	{
		m_RankData += 1;
	}
	if (m_LifeData > 1250)
	{
		m_RankData += 1;
	}
	if (m_LifeData > 1500)
	{
		m_RankData += 1;
	}

	if (m_TimeData > 10000)
	{
		m_RankData += 1;
	}
	if (m_TimeData > 20000)
	{
		m_RankData += 1;
	}
	if (m_TimeData > 30000)
	{
		m_RankData += 1;
	}
	if (m_TimeData > 40000)
	{
		m_RankData += 1;
	}
	if (m_TimeData > 50000)
	{
		m_RankData += 1;
	}
	if (m_TimeData > 60000)
	{
		m_RankData += 1;
	}

	m_Rank = CObject2D::Create();
	m_Rank->SetPos(D3DXVECTOR3(750.0f, 235.0f, 0.0f));
	m_Rank->SetWidth(150.0f);
	m_Rank->SetHeight(150.0f);

	if (m_RankData == 0)
	{
		m_Rank->SetTexture("data\\TEXTURE\\RankS.png");
	}
	else if(m_RankData <= 1)
	{
		m_Rank->SetTexture("data\\TEXTURE\\RankAP.png");
	}
	else if (m_RankData <= 2)
	{
		m_Rank->SetTexture("data\\TEXTURE\\RankA.png");
	}
	else if (m_RankData <= 4)
	{
		m_Rank->SetTexture("data\\TEXTURE\\RankAM.png");
	}
	else if (m_RankData <= 6)
	{
		m_Rank->SetTexture("data\\TEXTURE\\RankBP.png");
	}
	else if (m_RankData <= 8)
	{
		m_Rank->SetTexture("data\\TEXTURE\\RankB.png");
	}
	else if (m_RankData <= 10)
	{
		m_Rank->SetTexture("data\\TEXTURE\\RankBM.png");
	}
	else
	{
		m_Rank->SetTexture("data\\TEXTURE\\RankC.png");
	}

	//m_DeathText = CObject2D::Create();
	//m_DeathText->SetPos(D3DXVECTOR3(640.0f, 550.0f, 0.0f));
	//m_DeathText->SetWidth(700.0f);
	//m_DeathText->SetHeight(500.0f);
	//m_DeathText->SetTexture("data\\TEXTURE\\DeathCount.png");

	CManager::GetInstance()->GetCamera()->SetCameraPos(D3DXVECTOR3(3500.0f, 450.0f, 0.0f));

	//m_MaxConbo = CScore::Create();
	//m_MaxConbo->SetPos(D3DXVECTOR3(m_MaxConboPosX + 150.0f, 350.0f, 0.0f));
	//m_MaxConbo->SetScore(CManager::GetInstance()->GetEndCombo() * 1000);

	//m_MaxFall = CScore::Create();
	//m_MaxFall->SetPos(D3DXVECTOR3(m_MaxFallPosX + 150.0f, 450.0f, 0.0f));
	//m_MaxFall->SetScore((CManager::GetInstance()->GetEndFall() / 60) * 1000);

	//m_Depth = CScore::Create();
	//m_Depth->SetPos(D3DXVECTOR3(m_DepthPosX + 150.0f, 550.0f, 0.0f));
	//m_Depth->SetScore((int)(CManager::GetInstance()->GetEndDepth()));

	//m_TotalScore = CScore::Create();
	//m_TotalScore->SetPos(D3DXVECTOR3(m_TotalScorePosX + 150.0f, 670.0f, 0.0f));
	//m_TotalScore->SetScore(0);

	//m_MaxConboText = CObject2D::Create();
	//m_MaxConboText->SetPos(D3DXVECTOR3(m_MaxConboPosX, 350.0f, 0.0f));
	//m_MaxConboText->SetWidth(400.0f);
	//m_MaxConboText->SetHeight(150.0f);
	//m_MaxConboText->SetTexture("data\\TEXTURE\\ComboScore.png");

	//m_MaxFallText = CObject2D::Create();
	//m_MaxFallText->SetPos(D3DXVECTOR3(m_MaxFallPosX, 450.0f, 0.0f));
	//m_MaxFallText->SetWidth(400.0f);
	//m_MaxFallText->SetHeight(150.0f);
	//m_MaxFallText->SetTexture("data\\TEXTURE\\FallScore.png");

	//m_DepthText = CObject2D::Create();
	//m_DepthText->SetPos(D3DXVECTOR3(m_DepthPosX, 550.0f, 0.0f));
	//m_DepthText->SetWidth(400.0f);
	//m_DepthText->SetHeight(150.0f);
	//m_DepthText->SetTexture("data\\TEXTURE\\DepthScore.png");

	//m_TotalScoreText = CObject2D::Create();
	//m_TotalScoreText->SetPos(D3DXVECTOR3(m_TotalScorePosX, 670.0f, 0.0f));
	//m_TotalScoreText->SetWidth(400.0f);
	//m_TotalScoreText->SetHeight(150.0f);
	//m_TotalScoreText->SetTexture("data\\TEXTURE\\TotalScore.png");

	//m_RankText = CObject2D::Create();
	//m_RankText->SetPos(D3DXVECTOR3(RankingPosX, 670.0f, 0.0f));
	//m_RankText->SetWidth(220.0f);
	//m_RankText->SetHeight(450.0f);
	//m_RankText->SetTexture("data\\TEXTURE\\RANKING_TEXT.png");

	//m_ResultText = CObject2D::Create();
	//m_ResultText->SetPos(D3DXVECTOR3(640.0f, 100.0f, 0.0f));
	//m_ResultText->SetWidth(600.0f);
	//m_ResultText->SetHeight(420.0f);
	//m_ResultText->SetTexture("data\\TEXTURE\\RESULT_TEXT.png");

	//m_RankingText = CObject2D::Create();
	//m_RankingText->SetPos(D3DXVECTOR3(RankingPosX, 100.0f, 0.0f));
	//m_RankingText->SetWidth(700.0f);
	//m_RankingText->SetHeight(400.0f);
	//m_RankingText->SetTexture("data\\TEXTURE\\RANKING_TEXT01.png");

	//m_pResult->SetIdx(pTexture->Regist("data\\TEXTURE\\Result.png"));

	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CResult::Uninit(void)
{
	//全てのオブジェクトの破棄
	CObject::ReleaseAll();
}

//====================================================================
//更新処理
//====================================================================
void CResult::Update(void)
{
	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_RETURN) == true ||
		CManager::GetInstance()->GetInputJoyPad()->GetTrigger(CInputJoypad::BUTTON_A, 0) == true)
	{
		CFade::SetFade(CScene::MODE_TITLE);
		CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_ENTER_PUSH);
		m_pLifeRanking->SaveRanking();
		m_pTimeRanking->SaveRanking();
	}
}

//====================================================================
//描画処理
//====================================================================
void CResult::Draw(void)
{

}