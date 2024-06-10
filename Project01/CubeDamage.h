//============================================
//
//	キューブダメージ [CubeDamage.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _CUBEDAMAGE_H_
#define _CUBEDAMAGE_H_

#include "main.h"
#include "objmeshCube.h"

class CObject3D;

//オブジェクトメッシュフィールドクラス
class CCubeDamage : public CObjmeshCube
{
public:
	CCubeDamage(int nPriority = 5);
	~CCubeDamage();

	//キューブの種類
	enum CUBETYPE
	{
		CUBETYPE_EXPLOSION = 0,
		CUBETYPE_NORMAL,
		CUBETYPE_BREAK,
		CUBETYPE_MAX,
	};

	static CCubeDamage* Create(void);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void UninitExplosion(void);
	bool CollisionDamageBlock(D3DXVECTOR3 pPos, D3DXVECTOR3 Size, float* Damage);
	bool CollisionShadow(void);	//オブジェクトとの当たり判定
	void SetCubeType(CUBETYPE Set);
	CUBETYPE GetCubeType(void) { return m_CubeType; }
	void SetDamage(float Damage) { m_fDamage = Damage; }
	void SetSpinCount(D3DXVECTOR3 Cnt) { m_fSpinCount = Cnt; }
	D3DXVECTOR3 GetSpinCount(void) { return m_fSpinCount; }
	void SetSpinSpeedX(float Speed) { m_fSpinSpeed.x = Speed; }
	void SetSpinSpeedY(float Speed) { m_fSpinSpeed.y = Speed; }
	void SetSpinSpeedZ(float Speed) { m_fSpinSpeed.z = Speed; }
	D3DXVECTOR3 GetSpinSpeed(void) { return m_fSpinSpeed; }
	void SetUseSpin(bool Set) { m_bSpin = Set; }
	bool GetUseSpin(void) { return m_bSpin; }
	void SetSpinPos(D3DXVECTOR3 Pos) { SpinPos = Pos; }
	D3DXVECTOR3 GetSpinPos(void) { return SpinPos; }
	void SetMove(D3DXVECTOR3 Move) { m_Move = Move; }
	D3DXVECTOR3 GetMove(void) { return m_Move; }
	void SetSpinDistance(float Dis) { m_fSpinDistance = Dis; }
	float GetSpinDistance(void) { return m_fSpinDistance; }
	void SetSpinDisMove(float Move) { m_fSpinDisMove = Move; }
	float GetSpinDisMove(void) { return m_fSpinDisMove; }
	static void SetExplosionCount(int Count) { m_nExplosionCount = Count; }
	static int GetExplosionNum(void) { return m_nExplosionNum; }
	void SetLife(int Life) { m_nLife = Life; }
	int GetLife(void) { return m_nLife; }
	void SetSpinSinCos(int Set) { SpinSinCos = Set; }
	bool CollisionBlock(void);	//オブジェクトとの当たり判定
	static void StaticReset(void);
protected:

private:
	void ExplosionSet(void);

	float m_fDamage;				//ダメージ量
	D3DXVECTOR3 m_posOld;			//過去の位置
	D3DXVECTOR3 m_pos;				//位置
	D3DXVECTOR3 m_Move;				//移動量
	D3DXVECTOR3 SpinPos;			//回転の中心位置
	float m_fSpinDistance;			//回転の中心からの距離
	float m_fSpinDisMove;			//回転の中心からの距離の移動量
	D3DXVECTOR3 m_fSpinCount;		//回転系の動きの時に使用
	D3DXVECTOR3 m_fSpinSpeed;		//回転速度
	D3DXVECTOR3 m_fBOOLSpin;		//回転を加えているか否か
	bool m_bSpin;					//回転系の動きの時に使用
	CUBETYPE m_CubeType;			//ブロックの種類
	CObject3D* m_pShadow;			//影
	static int m_nExplosionCount;	//爆発までのカウント
	static int m_nExplosionNum;		//爆発キューブの個数
	int m_nExplosionIdx;			//爆発キューブの番号
	int m_nLife;					//キューブの寿命
	int SpinSinCos;					//スピンの種類
};
#endif