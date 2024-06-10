//============================================
//
//	ランキングの処理 [ranking.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _RANKING_H_
#define _RANKING_H_

#include "main.h"
#include "object.h"

//マクロ定義
#define MAX_RANK (5)		//ランキングの数
#define NUM_TIME (6)		//ランキングの桁数

//前方宣言
class CObject;
class CNumber;

//多重背景クラス
class CRanking : public CObject
{
public:
	CRanking(int nPriority = 5);
	~CRanking();

	static CRanking *Create(char* pModelName);
	void SetTime(int Time, int nCnt) { m_nTime[nCnt] = Time; }
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void AddTime(int Time) { m_nTime[0] += Time; }
	void FloatSetTime(int Time);
	void SetStartTime(int Time) { m_StartTime = Time; }
	void SetStopTime(bool Stop) { m_StopTime = Stop; }
	void SetRanking(int nRanKing);
	void ChangeRanking(bool Set) { m_SetRanking = Set; };
	void SaveRanking(void);
	void LoadRanking(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetTextName(char* pModelName) { m_cTextName = pModelName; }
	void SetNULL(void);
	void SetUseTime(bool Set);

private:
	CNumber *m_apObject[NUM_TIME][MAX_RANK];
	D3DXVECTOR3 m_pos;
	int m_nCount;
	int m_StartTime;
	int m_nTime[MAX_RANK];
	int m_nNumber[NUM_TIME][MAX_RANK];
	bool m_StopTime;
	bool m_SetRanking;
	char* m_cTextName;
	bool m_bTime;
};

#endif