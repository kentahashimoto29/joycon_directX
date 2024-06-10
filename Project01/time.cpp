//============================================
//
//	タイムの処理 [time.cpp]
//	Author:sakamoto kai
//
//============================================
#include "time.h"
#include "number.h"

//マクロ定義
#define SPACE_TIME (15.0f)		//タイム同士の隙間
#define TIME_POS (D3DXVECTOR3(1168.0f, 480.0f, 0.0f))		//タイムの位置

//静的メンバ変数宣言
CNumber* CTime::m_apObject[NUM_TIME] = {};
D3DXVECTOR3 CTime::m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
int CTime::m_nTime = 0;
int CTime::m_nCount = 0;
int CTime::m_StartTime = 0;
int CTime::m_nNumber[NUM_TIME] = { 0 };
bool CTime::m_StopTime = false;

//====================================================================
//コンストラクタ
//====================================================================
CTime::CTime(int nPriority) : CObject(nPriority)
{

}

//====================================================================
//デストラクタ
//====================================================================
CTime::~CTime()
{

}

//====================================================================
//生成処理
//====================================================================
CTime* CTime::Create()
{
	CTime* pMultiBG = nullptr;

	if (pMultiBG == nullptr)
	{
		//オブジェクト2Dの生成
		pMultiBG = new CTime();
	}

	//オブジェクトの初期化処理
	if (FAILED(pMultiBG->Init()))
	{//初期化処理が失敗した場合
		return nullptr;
	}

	return pMultiBG;
}

//====================================================================
//タイムの初期化
//====================================================================
HRESULT CTime::Init(void)
{
	SetType(CObject::TYPE_TIME);

	SetPos(TIME_POS);

	SetTime(0);

	for (int nCntObject = 0; nCntObject < NUM_TIME; nCntObject++)
	{
		if (m_apObject[nCntObject] == nullptr)
		{
			//数字の生成
			m_apObject[nCntObject] = CNumber::Create();
			m_apObject[nCntObject]->SetNumber(0);

			if (nCntObject < 2)
			{
				m_apObject[nCntObject]->SetPos(D3DXVECTOR3(m_pos.x + (nCntObject * SPACE_TIME) - 3.0f, m_pos.y, m_pos.z));
				m_apObject[nCntObject]->SetWidth(20.0f);
				m_apObject[nCntObject]->SetHeight(20.0f);
			}
			else if (nCntObject < 4)
			{
				m_apObject[nCntObject]->SetPos(D3DXVECTOR3(m_pos.x + (nCntObject * SPACE_TIME), m_pos.y, m_pos.z));
				m_apObject[nCntObject]->SetWidth(20.0f);
				m_apObject[nCntObject]->SetHeight(20.0f);
			}
			else
			{
				m_apObject[nCntObject]->SetPos(D3DXVECTOR3(m_pos.x + (nCntObject * (SPACE_TIME - 2.0f)) + 8.0f, m_pos.y + 2.0f, m_pos.z));
				m_apObject[nCntObject]->SetWidth(18.0f);
				m_apObject[nCntObject]->SetHeight(18.0f);
			}

			m_apObject[nCntObject]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
		}
	}
	return S_OK;
}

//====================================================================
//タイムの終了
//====================================================================
void CTime::Uninit(void)
{
	SetDeathFlag(true);
}

//====================================================================
//所持ポインタの終了
//====================================================================
void CTime::SetNULL(void)
{
	for (int nCntObject = 0; nCntObject < NUM_TIME; nCntObject++)
	{
		m_apObject[nCntObject] = nullptr;
	}
}

//====================================================================
//タイムの更新
//====================================================================
void CTime::Update(void)
{
	int nTime = m_nTime / 1000;

	//分
	m_nNumber[0] = (nTime / 60) % 100 / 10;
	m_nNumber[1] = (nTime / 60) % 10 / 1;
	nTime %= 60;

	//秒
	m_nNumber[2] = (nTime) % 100 / 10;
	m_nNumber[3] = (nTime) % 10 / 1;

	//ミリ秒
	m_nNumber[4] = (m_nTime) % 1000 / 100;
	m_nNumber[5] = (m_nTime) % 100 / 10;

	//m_nNumber[0] = m_nTime % 10000000 / 1000000;
	//m_nNumber[1] = m_nTime % 1000000 / 100000;
	//m_nNumber[2] = m_nTime % 100000 / 10000;
	//m_nNumber[3] = m_nTime % 10000 / 1000;
	//m_nNumber[4] = m_nTime % 1000 / 100;
	//m_nNumber[5] = m_nTime % 100 / 10;

	for (int nCntObject = 0; nCntObject < NUM_TIME; nCntObject++)
	{
		m_apObject[nCntObject]->SetNumber(m_nNumber[nCntObject]);
	}
}

//====================================================================
//タイムの描画
//====================================================================
void CTime::Draw(void)
{

}

//====================================================================
//タイムの描画
//====================================================================
void CTime::FloatSetTime(int Time)
{
	if (m_StopTime == false)
	{
		m_nTime = Time - m_StartTime;
	}
}

//====================================================================
//タイムの描画
//====================================================================
void CTime::SetStartTime(int Time)
{
	m_StartTime = Time;

	for (int nCntObject = 0; nCntObject < NUM_TIME; nCntObject++)
	{
		if (m_apObject[nCntObject] != nullptr)
		{
			m_apObject[nCntObject]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
}

//====================================================================
//タイムの情報取得
//====================================================================
int CTime::GetTimeNumber(void)
{
	int nTime = m_nTime / 1000;
	int ReturnTime = 0;

	//分
	ReturnTime += ((nTime / 60) % 100 / 10) * 100000;
	ReturnTime += ((nTime / 60) % 10 / 1) * 10000;
	nTime %= 60;

	//秒
	ReturnTime += ((nTime) % 100 / 10) * 1000;
	ReturnTime += ((nTime) % 10 / 1) * 100;

	//ミリ秒
	ReturnTime += ((m_nTime) % 1000 / 100) * 10;
	ReturnTime += ((m_nTime) % 100 / 10);

	return ReturnTime;
}