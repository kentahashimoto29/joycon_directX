//============================================
//
//	敵の処理 [enemy.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "object.h"

//前方宣言
class CModel;
class CMotion;
class CObject3D;

//オブジェクトプレイヤークラス
class CEnemy : public CObject
{
public:
	CEnemy(int nPriority = 2);
	~CEnemy();

	//プレイヤーのモーション
	enum ACTION_TYPE
	{
		ACTION_WAIT = 0,	//待機
		ACTION_MOVE,		//移動
		ACTION_ATTACK1,		//地上攻撃
		ACTION_JAMP,		//ジャンプ
		ACTION_MAX,			//最大
	};

	//敵の状態
	enum STATE
	{
		STATE_NORMAL = 0,	//通常
		STATE_BATTLE,		//戦闘
		STATE_SEATCH,		//探索
		STATE_DEATH,		//死亡
		STATE_MAX,			//最大
	};

	//敵の戦闘状態
	enum BATTLESTATE
	{
		BATTLESTATE_NORMAL = 0,	//通常
		BATTLESTATE_DEATH,		//死亡
		BATTLESTATE_WAIT,		//待機
		BATTLESTATE_DAMAGE,		//ダメージ
		BATTLESTATE_WALK,		//歩行
		BATTLESTATE_MAX,		//最大
	};


	//敵の探索状態
	enum SEARCHSTATE
	{
		SEARCHSTATE_NORMAL = 0,	//通常
		SEARCHSTATE_WALK,		//歩行
		SEARCHSTATE_TRUN,		//方向転換
		SEARCHSTATE_WAIT,		//待機
		SEARCHSTATE_SEARCH,		//探索
		SEARCHSTATE_MAX,		//最大
	};

	D3DMATRIX GetMtxWorld(void) { return m_mtxWorld; }
	static CEnemy* Create();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void TitleUpdate(void);
	void GameUpdate(void);
	void Draw(void);

	void LoadLevelData(const char* pFilename);

	void HitDamage(float fDamage);

	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	D3DXVECTOR3 GetMove(void) { return m_move; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	void SetSize(D3DXVECTOR3 size) { m_size = size; }
	D3DXVECTOR3 GetSize(void) { return m_size; }
	BATTLESTATE GetBattleState(void) { return m_BState; }
	SEARCHSTATE GetSearchState(void) { return m_SState; }
	bool GetJump(void) { return m_bJump; }
	D3DXVECTOR3* GetScreenPos(void) { return &m_ScreenPos[0]; }

	void SetSeatchPos(D3DXVECTOR3 pos, int nCnt) { m_SeatchPos[nCnt] = pos; }
	void SetSeatchLoop(bool Loop) { m_bSeatchLoop = Loop; }
	void SetState(STATE State) { m_State = State; }
	STATE GetState(void) { return m_State; }
	void SetSeatchState(SEARCHSTATE State) { m_SState = State; }

private:

	void Death(void);

	void ScreenCollision();

	void MyObjCreate(void);								//自分が保持するオブジェクトの生成
	void ActionState(void);								//モーションと状態の管理
	void BattleStateManager(void);						//戦闘状態管理
	void SeatchStateManager(void);						//探索状態管理
	void CollisionPush(void);							//自分以外の敵を押し出す処理

	ACTION_TYPE m_Action;
	ACTION_TYPE m_AtkAction;	//攻撃状態記録用変数
	D3DXVECTOR3 m_pos;			//位置
	D3DXVECTOR3 m_posOld;		//過去の位置
	D3DXVECTOR3 m_move;			//移動量
	D3DXVECTOR3 m_Objmove;		//オブジェクトから影響される移動量
	D3DXVECTOR3 m_rot;			//向き
	D3DXVECTOR3 m_AtkPos;		//攻撃位置
	D3DXVECTOR3 m_SeatchPos[32];//探索位置
	int m_nSeatchCount;			//探索段階のカウント
	bool m_bSeatchLoop;			//探索がループするかどうか
	D3DXMATRIX m_mtxWorld;		//ワールドマトリックス
	bool m_bJump;				//ジャンプをしたかどうか
	int m_nActionCount;			//行動のカウント
	bool m_nAttackHit;			//攻撃が当たったかどうか
	D3DXVECTOR3 m_size;			//大きさ
	STATE m_State;				//状態
	BATTLESTATE m_BState;		//戦闘状態
	SEARCHSTATE m_SState;		//探索状態
	int m_nStateCount;			//状態管理用カウント

	float m_fLife;				//体力

	D3DXVECTOR3 m_ScreenPos[8];	//スクリーン座標
	float m_CollisionRot;		//当たり判定用の向き

	//階層構造とモーションのポインタ
	CModel* m_apModel[64];
	CMotion* m_pMotion;
	char* m_aModelName[64];
	int m_nNumModel;
};

#endif