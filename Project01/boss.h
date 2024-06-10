//============================================
//
//	ボスの処理 [boss.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _BOSS_H_
#define _BOSS_H_

#include "main.h"
#include "objectX.h"

//前方宣言
class CObjGauge2D;
class CNumber;
class CCubeSpin;
class CObject2D;
class CObject3D;

//オブジェクトプレイヤークラス
class CBoss : public CObjectX
{
public:

	CBoss(int nPriority = 5);
	~CBoss();

	//ボスの状態
	enum STATE
	{
		STATE_NORMAL = 0,	//通常
		STATE_DAMAGE,		//被弾
		STATE_DEATH,		//死亡
		STATE_INVINCIBLE,	//無敵
		STATE_MAX,
	};

	//ボスの行動
	enum ACTION
	{
		ACTION_NORMAL = 0,	//通常
		ACTION_ATTACK,		//攻撃
		ACTION_WARP,		//瞬間移動
		ACTION_REVIVAL,		//復活
		ACTION_DEATH,		//死亡
		ACTION_EVENT,		//イベント時
		ACTION_MAX,
	};

	//ボスの攻撃
	enum ATTACK
	{
		ATTACK_NOT = 0,		//攻撃しない
		ATTACK_BULLET,		//弾
		ATTACK_RUSH,		//突進
		ATTACK_BLOCKRUN,	//ブロックラン
		ATTACK_SPINPILLAR,	//ブロックの柱
		ATTACK_RAIN,		//ブロックの雨
		ATTACK_SPAWNENEMY,	//敵の生成
		ATTACK_REVIVAL,		//復活
		ATTACK_2D_BLOCKWALL,//[2D]ブロックウォール
		ATTACK_DOWN_BREAK,	//[見下ろし]マップ破壊攻撃
		ATTACK_DEATH,		//死亡
		ATTACK_MAX,
	};

	static CBoss* Create(char* pModelName);

	HRESULT Init(char* pModelName);
	void Uninit(void);
	void Update(void);
	void TitleUpdate(void);
	void GameUpdate(void);
	void Draw(void);

	int GetIdx(void) { return m_nIdxTexture; }
	int GetIdxXModel(void) { return -1; }
	bool Collision(D3DXVECTOR3* pPos, D3DXVECTOR3 pPosOld, D3DXVECTOR3* pMove, float fHeight, float fWidth, bool* bJumpMove, bool* bHit, bool bX);
	void HitDamage(float Damage);
	void SetLifeUI(void);
	void SetAction(ACTION Action) { m_Action = Action; }
	ACTION GetAction(void) { return m_Action; }
	int GetBossForm(void) { return m_nForm; }
	STATE GetState(void) { return m_State; }
	void LifeGaugeCreate(void);				//ライフゲージ管理

private:
	void StateManager(void);				//状態管理
	void WarpUpdate(D3DXVECTOR3* pos);		//ワープ更新処理
	void Warp(ATTACK Pattern);				//ワープ位置指定処理
	void AttackUpdate(D3DXVECTOR3* pos);	//攻撃更新処理
	void AttackSelect(void);				//攻撃選択処理
	void AttackBullet(D3DXVECTOR3* pos);	//攻撃パターン射撃
	void AttackRush(D3DXVECTOR3* pos);		//攻撃パターン突進
	void AttackBlockRun(D3DXVECTOR3* pos);	//攻撃パターンブロックラン
	void AttackSpinPillar(D3DXVECTOR3* pos);//攻撃パターン回転する柱
	void AttackRain(D3DXVECTOR3* pos);		//攻撃パターン雨
	void AttackSpawnEnemy(D3DXVECTOR3* pos);//攻撃パターン敵生成
	void AttackBlockWall(D3DXVECTOR3* pos);	//攻撃パターンブロックウォール[2D]
	void AttackMapBreak(D3DXVECTOR3* pos);	//攻撃パターンマップブレイク[見下ろし]
	void AttackRevival(D3DXVECTOR3* pos);	//攻撃パターン復活
	void AttackDeath(D3DXVECTOR3* pos);		//攻撃パターン死亡演出
	void DeathExplosion(D3DXVECTOR3* pos, D3DXVECTOR3 SpinMove, int Set);	//死亡時の爆発
	bool CollisionBlock(D3DXVECTOR3* pos);	//オブジェクトとの当たり判定
	bool CollisionCircle(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float nRadiusOut);	//円の当たり判定
	bool CollisionShadow(void);	//オブジェクトとの当たり判定

	int m_nIdxXModel;				//Xモデルの番号
	D3DXVECTOR3 m_CollisionPos;		//当たり判定用の座標
	D3DXVECTOR3 m_WarpPos;			//ワープ先の位置
	bool m_bCollision;				//当たり判定用の座標
	int m_nIdxTexture;				//テクスチャの番号
	float m_MoveCount;				//待機中の浮遊用変数
	STATE m_State;					//状態
	ACTION m_Action;				//行動
	int m_nStateCount;				//状態管理用変数
	bool m_nWarpOK;					//ワープ前かワープ後か
	float m_ColorA;					//不透明度
	ATTACK m_AttackPatternOld;		//過去の攻撃パターン
	ATTACK m_AttackPattern;			//攻撃パターン
	int m_AttackCount;				//攻撃に使うカウント
	int m_AttackWave;				//攻撃段階
	int m_AttackCoolTime;			//攻撃に使うカウント
	float m_SpinCount;				//回転のカウント
	float m_Scaling;				//大きさ
	int m_nForm;					//形態
	bool m_bRevivalColorSwitch;		//蘇生中の色の変化

	D3DXVECTOR3 m_move;				//移動量	
	D3DXVECTOR3 m_rot;				//向き	
	float m_fLife;					//ボスのライフ
	float m_fMoveLife;				//演出用ライフ
	bool m_bDelLife;				//ライフが減る状態になったら
	float m_fLifeMax;				//ボスのライフの最大値
	CObjGauge2D* m_pLifeGaugeBG;	//ライフゲージのポインタ
	CObjGauge2D* m_pLifeGauge;		//ライフゲージのポインタ
	CNumber* m_pLifeNumber[5];		//ライフ用UI
	CCubeSpin* m_CubeSpin;			//回転用のキューブ
	CCubeSpin* m_CubeSpinTitle;		//回転用のキューブ(タイトル演出用)
	CObject2D* m_pRevivalFG;		//蘇生時の前面ポリゴン
	float m_pFGDel;					//前面ポリゴンの色減衰
	float m_fRevivalColorA;			//前面ポリゴンの不透明度
	bool m_bBreak[9];				//マップ破壊攻撃の時に破壊されている位置かどうかの判断をする変数
	CObject3D* m_pShadow;			//影
	bool m_bDeathColorSwich;		//死亡時の色変更
	float m_fDeathColor;			//死亡時の色
	float m_fDeathExplojsionDis;	//死亡時の爆発キューブの距離
	int m_nDebugText;
};
#endif