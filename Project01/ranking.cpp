//============================================
//
//	ランキングの処理 [ranking.cpp]
//	Author:sakamoto kai
//
//============================================
#include "manager.h"
#include "ranking.h"
#include "number.h"

//マクロ定義
#define SPACE_TIME (50.0f)		//ランキング同士の隙間
#define TIME_POS (D3DXVECTOR3(1000.0f, 200.0f, 0.0f))		//ランキングの位置
#define RANKING_SIZE (D3DXVECTOR3(35.0f, 35.0f, 0.0f))		//ランキングの大きさ
#define NUMBER_DISTANCE (25.0f)		//文字同士の幅
#define RANKING_DISTANCE (48.0f)	//ランキング同士の幅

//静的メンバ変数宣言

//====================================================================
//コンストラクタ
//====================================================================
CRanking::CRanking(int nPriority) : CObject(nPriority)
{
	for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		for (int nCntTime = 0; nCntTime < NUM_TIME; nCntTime++)
		{
			m_apObject[nCntTime][nCntRank] = nullptr;
			m_nNumber[nCntTime][nCntRank] = 0;
		}
	}
	m_nTime[0] = 80000;
	m_nTime[1] = 40000;
	m_nTime[2] = 20000;
	m_nTime[3] = 10000;
	m_nTime[4] = 5000;

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nCount = 0;
	m_StartTime = 0;
	m_StopTime = false;
	m_SetRanking = false;
	m_bTime = false;
}

//====================================================================
//デストラクタ
//====================================================================
CRanking::~CRanking()
{

}

//====================================================================
//生成処理
//====================================================================
CRanking *CRanking::Create(char* pModelName)
{
	CRanking *pMultiBG = nullptr;

	if (pMultiBG == nullptr)
	{
		//オブジェクト2Dの生成
		pMultiBG = new CRanking();
	}

	pMultiBG->SetTextName(pModelName);

	//オブジェクトの初期化処理
	if (FAILED(pMultiBG->Init()))
	{//初期化処理が失敗した場合
		return nullptr;
	}

	return pMultiBG;
}

//====================================================================
//ランキングの初期化
//====================================================================
HRESULT CRanking::Init(void)
{
	SetType(CObject::TYPE_TIME);

	SetPos(TIME_POS);

	for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		for (int nCntObject = 0; nCntObject < NUM_TIME; nCntObject++)
		{
			if (m_apObject[nCntObject][nCntRank] == nullptr)
			{
				//数字の生成
				m_apObject[nCntObject][nCntRank] = CNumber::Create();
				m_apObject[nCntObject][nCntRank]->SetNumber(0);
				m_apObject[nCntObject][nCntRank]->SetPos(D3DXVECTOR3(m_pos.x + (nCntObject * NUMBER_DISTANCE), m_pos.y + (nCntRank * RANKING_DISTANCE), m_pos.z));
				m_apObject[nCntObject][nCntRank]->SetWidth(RANKING_SIZE.x);
				m_apObject[nCntObject][nCntRank]->SetHeight(RANKING_SIZE.y);
			}
		}
	}

	LoadRanking();

	return S_OK;
}

//====================================================================
//ランキングの終了
//====================================================================
void CRanking::Uninit(void)
{

}

//====================================================================
//ランキングの終了
//====================================================================
void CRanking::SetNULL(void)
{
	for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		for (int nCntObject = 0; nCntObject < NUM_TIME; nCntObject++)
		{
			if (m_apObject[nCntObject][nCntRank] != nullptr)
			{
				m_apObject[nCntObject][nCntRank]->Uninit();
				m_apObject[nCntObject][nCntRank] = nullptr;
			}
		}
	}
}

//====================================================================
//ランキングの終了
//====================================================================
void CRanking::SetUseTime(bool Set)
{
	m_bTime = Set;

	if (m_bTime == true)
	{
		for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
		{
			for (int nCntObject = 0; nCntObject < NUM_TIME; nCntObject++)
			{
				if (m_apObject[nCntObject][nCntRank] != nullptr)
				{
					if (nCntObject < NUM_TIME - 4)
					{
						//数字の生成
						m_apObject[nCntObject][nCntRank]->SetPos(D3DXVECTOR3(m_pos.x + (nCntObject * NUMBER_DISTANCE - 4.0f), m_pos.y + (nCntRank * RANKING_DISTANCE), m_pos.z));
						m_apObject[nCntObject][nCntRank]->SetWidth(RANKING_SIZE.x);
						m_apObject[nCntObject][nCntRank]->SetHeight(RANKING_SIZE.y);
					}
					else if (nCntObject < NUM_TIME - 2)
					{
						//数字の生成
						m_apObject[nCntObject][nCntRank]->SetPos(D3DXVECTOR3(m_pos.x + (nCntObject * NUMBER_DISTANCE), m_pos.y + (nCntRank * RANKING_DISTANCE), m_pos.z));
						m_apObject[nCntObject][nCntRank]->SetWidth(RANKING_SIZE.x);
						m_apObject[nCntObject][nCntRank]->SetHeight(RANKING_SIZE.y);
					}
					else
					{
						//数字の生成
						m_apObject[nCntObject][nCntRank]->SetPos(D3DXVECTOR3(
							m_pos.x + (nCntObject * NUMBER_DISTANCE + RANKING_SIZE.x * 0.2f), 
							m_pos.y + (nCntRank * RANKING_DISTANCE + RANKING_SIZE.y * 0.2f), 
							m_pos.z));
						m_apObject[nCntObject][nCntRank]->SetWidth(RANKING_SIZE.x * 0.8f);
						m_apObject[nCntObject][nCntRank]->SetHeight(RANKING_SIZE.y * 0.8f);
					}
				}
			}
		}
	}
}

//====================================================================
//ランキングの更新
//====================================================================
void CRanking::Update(void)
{
	if (m_bTime == true)
	{
		for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
		{
			for (int nCntObject = 0; nCntObject < NUM_TIME; nCntObject++)
			{
				if (m_apObject[nCntObject][nCntRank] != nullptr)
				{
					if (nCntObject < NUM_TIME - 4)
					{
						//数字の生成
						m_apObject[nCntObject][nCntRank]->SetPos(D3DXVECTOR3(m_pos.x + (nCntObject * NUMBER_DISTANCE - 4.0f), m_pos.y + (nCntRank * RANKING_DISTANCE), m_pos.z));
					}
					else if (nCntObject < NUM_TIME - 2)
					{
						m_apObject[nCntObject][nCntRank]->SetPos(D3DXVECTOR3(m_pos.x + (nCntObject * NUMBER_DISTANCE), m_pos.y + (nCntRank * RANKING_DISTANCE), m_pos.z));
					}
					else
					{
						m_apObject[nCntObject][nCntRank]->SetPos(D3DXVECTOR3(
							m_pos.x + (nCntObject * NUMBER_DISTANCE + RANKING_SIZE.x * 0.2f),
							m_pos.y + (nCntRank * RANKING_DISTANCE + RANKING_SIZE.y * 0.2f),
							m_pos.z));
					}
				}
			}
		}
	}
	else
	{
		for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
		{
			for (int nCntObject = 0; nCntObject < NUM_TIME; nCntObject++)
			{
				if (m_apObject[nCntObject] != nullptr)
				{
					m_apObject[nCntObject][nCntRank]->SetPos(D3DXVECTOR3(m_pos.x + (nCntObject * NUMBER_DISTANCE), m_pos.y + (nCntRank * RANKING_DISTANCE), m_pos.z));
				}
			}
		}
	}

	for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		m_nNumber[0][nCntRank] = m_nTime[nCntRank] % 1000000 / 100000;
		m_nNumber[1][nCntRank] = m_nTime[nCntRank] % 100000 / 10000;
		m_nNumber[2][nCntRank] = m_nTime[nCntRank] % 10000 / 1000;
		m_nNumber[3][nCntRank] = m_nTime[nCntRank] % 1000 / 100;
		m_nNumber[4][nCntRank] = m_nTime[nCntRank] % 100 / 10;
		m_nNumber[5][nCntRank] = m_nTime[nCntRank] % 10 / 1;

		for (int nCntObject = 0; nCntObject < NUM_TIME; nCntObject++)
		{
			m_apObject[nCntObject][nCntRank]->SetNumber(m_nNumber[nCntObject][nCntRank]);
			//m_apObject[nCntObject][nCntRank]->SetNumber(nCntObject * 2 + nCntRank * 2);
		}
	}
}

//====================================================================
//ランキングの描画
//====================================================================
void CRanking::Draw(void)
{

}

//====================================================================
//ランキングの描画
//====================================================================
void CRanking::FloatSetTime(int Time)
{
	if (m_StopTime == false)
	{
		m_nTime[0] = Time - m_StartTime;
	}
}

//====================================================================
//ランキングの設定処理
//====================================================================
void CRanking::SetRanking(int nRanKing)
{
	int nCount1;
	int nCount2;
	int nData;

	if (nRanKing < m_nTime[MAX_RANK - 1])
	{
		m_nTime[MAX_RANK - 1] = nRanKing;

		for (nCount1 = 0; nCount1 < MAX_RANK; nCount1++)
		{
			for (nCount2 = nCount1; nCount2 < MAX_RANK; nCount2++)
			{
				if (m_nTime[nCount1] > m_nTime[nCount2])
				{
					nData = m_nTime[nCount1];
					m_nTime[nCount1] = m_nTime[nCount2];
					m_nTime[nCount2] = nData;
				}
			}
		}
	}

	//if (nRanKing > m_nTime[MAX_RANK - 1])
	//{
	//	m_nTime[MAX_RANK - 1] = nRanKing;

	//	for (nCount1 = 0; nCount1 < MAX_RANK; nCount1++)
	//	{
	//		for (nCount2 = nCount1; nCount2 < MAX_RANK; nCount2++)
	//		{
	//			if (m_nTime[nCount1] < m_nTime[nCount2])
	//			{
	//				nData = m_nTime[nCount1];
	//				m_nTime[nCount1] = m_nTime[nCount2];
	//				m_nTime[nCount2] = nData;
	//			}
	//		}
	//	}
	//}
}

//====================================================================
//保存処理
//====================================================================
void CRanking::SaveRanking(void)
{
	FILE *pFile; //ファイルポインタを宣言

	//ファイルを開く
	pFile = fopen(m_cTextName, "w");
	
	if (pFile != nullptr)
	{//ファイルが開けた場合

		for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
		{
			fprintf(pFile, "%d\n", m_nTime[nCnt]);
		}

		fclose(pFile);
	}
	else
	{//ファイルが開けなかった場合
		printf("***ファイルを開けませんでした***\n");
	}
}

//====================================================================
//マップを全て埋めて生成
//====================================================================
void CRanking::LoadRanking(void)
{
	FILE *pFile; //ファイルポインタを宣言

	//ファイルを開く
	pFile = fopen(m_cTextName, "r");

	if (pFile != nullptr)
	{//ファイルが開けた場合

		for (int nCnt = 0; nCnt < MAX_RANK; nCnt++)
		{
			fscanf(pFile, "%d", &m_nTime[nCnt]);
		}
		fclose(pFile);
	}
	else
	{//ファイルが開けなかった場合
		printf("***ファイルを開けませんでした***\n");
	}
}