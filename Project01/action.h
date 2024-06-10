////============================================
////
////	アクションの処理 [action.h]
////	Author:sakamoto kai
////
////============================================
//#ifndef _ACTION_H_
//#define _ACTION_H_
//
//#include "object.h"
//
////前方宣言
//class CModel;
//class CMotion;
//class CObject3D;
//class CObject3D;
//class CObjGauge2D;
//
//class CEnemy;
//
////オブジェクトプレイヤークラス
//class CAction
//{
//public:
//
//	CAction();
//	~CAction();
//
//	//プレイヤーのモーション
//	enum ACTION_TYPE
//	{
//		ACTION_WAIT = 0,	//待機
//		ACTION_MOVE,		//移動
//		ACTION_ATTACK1,		//地上攻撃１
//		ACTION_ATTACK2,		//地上攻撃２
//		ACTION_ATTACK3,		//地上攻撃３
//		ACTION_SKYATTACK,	//空中攻撃
//		ACTION_JAMP,		//ジャンプ
//		ACTION_EVASION,		//回避
//		ACTION_TITLE,		//タイトルのポーズ
//		ACTION_MAX,			//最大
//	};
//
//	//プレイヤーの状態
//	enum STATE
//	{
//		STATE_NORMAL = 0,	//通常
//		STATE_DEATH,		//死亡
//		STATE_WAIT,			//待機
//		STATE_DAMAGE,		//ダメージ
//		STATE_MAX,			//最大
//	};
//
//	D3DMATRIX GetMtxWorld(void) { return m_mtxWorld; }
//	static CAction* Create();
//	HRESULT Init(void);
//	void Uninit(void);
//	void Update(void);
//	void TitleUpdate(void);
//	void GameUpdate(void);
//
//	void SetMove(D3DXVECTOR3 move) { m_move = move; }
//	D3DXVECTOR3 GetMove(void) { return m_move; }
//	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
//	D3DXVECTOR3 GetRot(void) { return m_rot; }
//	STATE GetState(void) { return m_State; }
//	bool GetJump(void) { return m_bJump; }
//	void SetCameraPos(D3DXVECTOR3 pos) { m_CameraPos = pos; }
//	D3DXVECTOR3 GetCameraPos(void) { return m_CameraPos; }
//	float GetCameraHeightRot(void) { return m_HeightCameraRot; }
//
//
//private:
//	void MyObjCreate(void);		//自分が保持するオブジェクトの生成
//	void StateManager(void);	//状態管理
//	void Move(void);			//移動処理
//	void Rot(void);				//移動方向処理
//	void Shuriken(void);		//手裏剣の処理
//	void DebugKey();
//
//	ACTION_TYPE m_Action;
//	ACTION_TYPE m_AtkAction;	//攻撃状態記録用変数
//	D3DXVECTOR3 m_move;			//移動量
//	D3DXVECTOR3 m_Objmove;		//オブジェクトから影響される移動量
//	D3DXVECTOR3 m_rot;			//向き
//	D3DXVECTOR3 m_rotDest;		//向きの目的地
//	float m_HeightCameraRot;	//カメラの高さの角度
//	D3DXVECTOR3 m_AtkPos;		//攻撃位置
//	D3DXVECTOR3 m_CameraPos;	//カメラ位置位置
//	D3DXMATRIX m_mtxWorld;		//ワールドマトリックス
//	bool m_bJump;				//ジャンプをしたかどうか
//	int m_nActionCount;			//行動のカウント
//	bool m_nAttackHit;			//攻撃が当たったかどうか
//	STATE m_State;				//状態
//	int m_nStateCount;			//状態管理用カウント
//
//	float m_CollisionRot;		//当たり判定用の向き
//
//	CEnemy* m_pTgtEnemy;		//ターゲットの敵ポインタ
//};
//
//#endif