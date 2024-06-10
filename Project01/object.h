//============================================
//
//	オブジェクト [object.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "main.h"

//マクロ定義
#define MAX_OBJECT (8102)	//オブジェクトの最大数
#define PRIORITY_MAX (8)	//描画順の最大数

class CObject
{
public:
	enum TYPE
	{
		TYPE_NONE = 0,
		TYPE_SAMPLE,
		TYPE_OBJECT2D,
		TYPE_PLAYER3D,
		TYPE_ENEMY3D,
		TYPE_EXPLOSION,
		TYPE_EFFECT,
		TYPE_PARTICLE,
		TYPE_TIME,
		TYPE_NUMBER,
		TYPE_BREAKBLOCK3D,
		TYPE_TUTORIALUI,
		TYPE_FG,
		TYPE_BLOCK,
		TYPE_MAPCHECKPOINT,
		TYPE_PLAYEREFFECT,
		TYPE_BOSS,
		TYPE_CUBEBLOCK,
		TYPE_CUBEDAMEGE,
		TYPE_CUBEEFFECT,
		TYPE_CUBESPIN,
		TYPE_LOG,
		TYPE_2DUI,
		TYPE_MAPMODEL,
		TYPE_MAX,
	};

	//当たり判定の使用軸
	enum COLLISION
	{
		COLLISION_X = 0,
		COLLISION_Y,
		COLLISION_Z,
		COLLISION_MAX,
	};
	
	CObject(int nPriority = 3);
	~CObject();

	static void ReleaseAll(void);
	static void UpdateAll(void);
	static void DrawAll(void);
	static void MultiTargetDraw(void);
	static void ResetObjectMap(void);
	static void ReleaseEnemy(void);
	static void DeleteBlock(void);

	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	static CObject *GetTop(int nPriority) { return m_pTop[nPriority]; }
	CObject *GetNext(void) { return m_pNext; }
	void SetDeathFlag(bool Set) { m_bDeath = Set; }

	void SetType(TYPE type) { m_type = type; }
	TYPE GetType(void) { return m_type; }
	virtual D3DXVECTOR3 GetPos(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }
	virtual D3DXVECTOR3 GetPosOld(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }
	virtual void HitDamage(float Damage) {}
	virtual void Hit(D3DXVECTOR3 pos, int Damage) {}
	virtual int GetLife(void) { return 0; }
	virtual float GetWidth(void) { return 0.0f; }
	virtual float GetHeight(void) { return 0.0f; }
	virtual D3DXVECTOR3 GetSize(void) { return INITVECTOR3; }
	static void SetLevelStop(bool Set) { m_bLevelStop = Set; }
	virtual void SetLevelUI(bool Set) { m_bLevelUI = Set; }
	void SetAppear(bool Set) { m_Appear = Set; }
	bool GetAppear(void) { return m_Appear; }
	virtual bool CollisionPlayer(D3DXVECTOR3* pPos, D3DXVECTOR3 pPosOld, D3DXVECTOR3* pMove, float fHeight, float fWidth, bool* bJumpMove, bool* bHit, bool b_Jamp) { return false; }
	virtual bool CollisionBlock(D3DXVECTOR3* pPos, D3DXVECTOR3 pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3* pObjMove, D3DXVECTOR3 Size, bool* bJump, COLLISION XYZ) { return false; }
	virtual bool CollisionBlockUp(D3DXVECTOR3* pPos, D3DXVECTOR3 pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3* pObjMove, D3DXVECTOR3 Size, bool* bJump, COLLISION XYZ) { return false; }
	virtual bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 pPosOld, D3DXVECTOR3 *pMove, float fHeight, float fWidth, bool *bJumpMove, bool *bHit, bool b_Jamp) { return false; }
	virtual bool CollisionDamageBlock(D3DXVECTOR3 pPos, D3DXVECTOR3 Size, float* Damage) { return false; }
	virtual void SetNULL(void) {}
	virtual D3DXVECTOR3* GetScreenPos(void) { return nullptr; }

protected:
	void Release(void);

private:

	static CObject *m_pTop[PRIORITY_MAX];	//先頭のオブジェクトへのポインタ
	static CObject *m_pCur[PRIORITY_MAX];	//最後尾のオブジェクトへのポインタ
	CObject *m_pPrev;						//前のオブジェクトへのポインタ
	CObject *m_pNext;						//次のオブジェクトへのポインタ
	bool m_bDeath;							//死亡フラグ
	static int m_nNumAll;					//オブジェクト総数
	int m_Priority;							//描画の優先順位
	static bool m_bLevelStop;				//レベルアップ時のポーズ
	bool m_bLevelUI;						//レベルアップ時のUI
	bool m_Appear;							//表示状態かどうか
	TYPE m_type;							//オブジェクトの種類
};

#endif