//============================================
//
//	キューブスピン [CubeSpin.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _CUBESPIN_H_
#define _CUBESPIN_H_

#include "main.h"
#include "objmeshCube.h"

class CObject3D;
class CCubeDamage;

//オブジェクトメッシュフィールドクラス
class CCubeSpin : public CObject
{
public:
	CCubeSpin(int NumCube, int nPriority = 4);
	~CCubeSpin();

	static CCubeSpin* Create(int NumCube);

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetDamage(float Damage) { m_fDamage = Damage; }
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	void SetSpinDistance(float Dis) { m_fSpinDistance = Dis; }
	float GetSpinDistance(void) { return m_fSpinDistance; }
	void SetSpinDisMove(float Move) { m_fSpinDisMove = Move; }
	float GetSpinDisMove(void) { return m_fSpinDisMove; }
	void SetSpinSpeedX(float Speed) { m_fSpinSpeed.x = Speed; }
	void SetSpinSpeedY(float Speed) { m_fSpinSpeed.y = Speed; }
	void SetSpinSpeedZ(float Speed) { m_fSpinSpeed.z = Speed; }
	D3DXVECTOR3 GetSpinSpeed(void) { return m_fSpinSpeed; }
protected:

private:
	int m_nNumCube;				//キューブの数
	float m_fDamage;			//ダメージ量
	D3DXVECTOR3 m_pos;			//位置
	D3DXVECTOR3 m_move;			//移動量
	int m_nLife;				//寿命
	float m_fSpinDistance;		//回転の中心からの距離
	float m_fSpinDisMove;		//回転の中心からの距離の移動量
	D3DXVECTOR3 m_fSpinSpeed;	//回転速度
	CCubeDamage* m_apCubeDamage[128];	//キューブダメージのポインタ
};
#endif