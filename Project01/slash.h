//============================================
//
//	斬撃処理 [slash.h]
//	Author:SakaiMinato
//
//============================================

#ifndef _SLASH_H_
#define _SLASH_H_

#include "main.h"
#include "object.h"

#include "object2D.h"

//マクロ定義
#define SLASH_2D_NUM (4)	// スラッシュの数

// オブジェクトプレイヤークラス
class CSlash : public CObject
{
public:

	// 状態
	enum STATE
	{
		STATE_NONE = 0,
		STATE_HIT,
		STATE_MAX
	};

	// 2Dの情報
	struct SInfo2D
	{
		D3DXVECTOR3 pos;	// 位置情報
		D3DXVECTOR2 size;	// 大きさ情報
		D3DXCOLOR color;	// 色情報

		CObject2D* pObj2d;	// 2Dオブジェクト
	};

	// 情報
	struct SInfo
	{
		D3DXVECTOR3 pos;		// 位置情報
		D3DXCOLOR color;		// 色情報

		D3DXVECTOR3 angle;		// 方向

		D3DXVECTOR2 sizeDef;	// デフォルトサイズ

		int nCnt;				// カウント
		bool AllAppear;			// 全表示の有無
		
		STATE state;					// 状態
		SInfo2D aInfo2D[SLASH_2D_NUM];	// 2Dの情報
	};

	CSlash(int nPriority = 6);
	~CSlash();

	static CSlash* Create(const char* p2dFileName);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:

	void InitObj2D(const char* p2dFileName);

	void EventCnt(void);

	void CollsionEnemy(void);

	void Set2dSize(D3DXVECTOR2 size, int nCnt);
	void Dec2dSize(D3DXVECTOR2 sizeDec, int nCnt);
	void Set2dAppear(void);

	SInfo m_info;	// 情報
};
#endif