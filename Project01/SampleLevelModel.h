//============================================
//
//	階層構造でモーションを行うモデルのサンプル [SampleLevelModel.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _SAMPLELEVELMODEL_H_
#define _SAMPLELEVELMODEL_H_

#include "object.h"

//前方宣言
class CModel;
class CMotion;
class CObject3D;

//オブジェクトプレイヤークラス
class CSampleLvModel : public CObject
{
public:
	CSampleLvModel(int nPriority = 2);
	~CSampleLvModel();

	//プレイヤーのモーション
	enum ACTION_TYPE
	{
		ACTION_WAIT = 0,	//待機
		ACTION_MOVE,		//移動
		ACTION_ATTACK1,		//地上攻撃１
		ACTION_ATTACK2,		//地上攻撃２
		ACTION_ATTACK3,		//地上攻撃３
		ACTION_SKYATTACK,	//空中攻撃
		ACTION_JAMP,		//ジャンプ
		ACTION_EVASION,		//回避
		ACTION_TITLE,		//タイトルのポーズ
		ACTION_MAX,			//最大
	};

	//プレイヤーの状態
	enum STATE
	{
		STATE_NORMAL = 0,	//通常
		STATE_DEATH,		//死亡
		STATE_WAIT,			//待機
		STATE_DAMAGE,		//ダメージ
		STATE_MAX,			//最大

	};

	D3DMATRIX GetMtxWorld(void) { return m_mtxWorld; }
	static CSampleLvModel* Create();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void TitleUpdate(void);
	void GameUpdate(void);
	void Draw(void);

	void LoadLevelData(const char* pFilename);
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	D3DXVECTOR3 GetMove(void) { return m_move; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	void SetWidth(float Width) { m_fWidth = Width; }
	float GetWidth(void) { return m_fWidth; }
	void SetHeight(float Height) { m_fHeight = Height; }
	float GetHeight(void) { return m_fHeight; }
	STATE GetState(void) { return m_State; }
	bool GetJump(void) { return m_bJump; }

private:
	void MyObjCreate(void);								//自分が保持するオブジェクトの生成
	void ActionState(void);								//モーションと状態の管理
	void StateManager(void);							//状態管理

	ACTION_TYPE m_Action;
	ACTION_TYPE m_AtkAction;	//攻撃状態記録用変数
	D3DXVECTOR3 m_pos;			//位置
	D3DXVECTOR3 m_posOld;		//過去の位置
	D3DXVECTOR3 m_move;			//移動量
	D3DXVECTOR3 m_Objmove;		//オブジェクトから影響される移動量
	D3DXVECTOR3 m_rot;			//向き
	D3DXVECTOR3 m_AtkPos;		//攻撃位置
	D3DXMATRIX m_mtxWorld;		//ワールドマトリックス
	bool m_bJump;				//ジャンプをしたかどうか
	int m_nActionCount;			//行動のカウント
	bool m_nAttackHit;			//攻撃が当たったかどうか
	float m_fWidth;				//幅
	float m_fHeight;			//高さ
	STATE m_State;				//状態
	int m_nStateCount;			//状態管理用カウント

	float m_CollisionRot;		//当たり判定用の向き

	//階層構造とモーションのポインタ
	CModel* m_apModel[64];
	CMotion* m_pMotion;
	char* m_aModelName[64];
	int m_nNumModel;
};

#endif