//============================================
//
//	マップモデル [MapModel.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _MAPMODEL_H_
#define _MAPMODEL_H_

#include "main.h"
#include "objectX.h"

//前方宣言
class CObjGauge2D;
class CCubeBlock;

//オブジェクトプレイヤークラス
class CMapModel : public CObjectX
{
public:

	CMapModel(int nPriority = 3);
	~CMapModel();

	//サンプルの状態
	enum STATE
	{
		STATE_NORMAL = 0,	//通常
		STATE_ACTION,		//行動
		STATE_MAX,
	};

	static CMapModel* Create(char* pModelName);

	HRESULT Init(char* pModelName);
	void Uninit(void);
	void Update(void);
	void TitleUpdate(void);
	void GameUpdate(void);
	void Draw(void);
	void SetCollision(bool Set) { m_bCollision = Set; }
	bool GetCollision(void) { return m_bCollision; }
	void CreateBlock(void);		//デバッグ用ブロックの生成
	void UpdateBlock(void);		//デバッグ用ブロックの更新
	void SetEditIdx(int Idx) { m_EditIndex = Idx; }
	int GetEditIdx(void) { return m_EditIndex; }

private:
	void StateManager(void);	//状態管理

	D3DXVECTOR3 m_CollisionPos;		//当たり判定用の座標
	bool m_bCollision;				//当たり判定用の座標
	STATE m_State;					//状態
	int m_nStateCount;				//状態管理用変数
	float m_Scaling;				//大きさ
	D3DXVECTOR3 m_posOld;			//過去の位置	
	D3DXVECTOR3 m_move;				//移動量	
	float m_fColorA;				//不透明度
	int m_EditIndex;				//エディット時のインデックス

	CCubeBlock* m_pDebugBlock;		//デバッグ用ブロック
};
#endif