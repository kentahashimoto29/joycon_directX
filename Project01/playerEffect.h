//============================================
//
//	プレイヤーの処理 [playerEffect.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _PLAYEREFFECT_H_
#define _PLAYEREFFECT_H_

#include "object.h"

//前方宣言
class CModel;
class CMotion;

//オブジェクトプレイヤークラス
class CPlayerEffect : public CObject
{
public:
	CPlayerEffect(int nPriority = 5);
	~CPlayerEffect();

	//プレイヤーのモーション
	typedef enum
	{
		ACTION_WAIT = 0,
		ACTION_MOVE,
		ACTION_ATTACK,
		ACTION_JAMP,
		ACTION_MAX,

	}ACTION_TYPE;

	D3DMATRIX GetMtxWorld(void) { return m_mtxWorld; }
	static CPlayerEffect* Create();
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void LoadLevelData(const char* pFilename);
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	void SetColor(D3DCOLORVALUE Color) { m_Color = Color; }
	void SetDel(float fDel) { m_fDel = fDel; }
	void SetAllPose(int nType, int nKey, float nCounter);
	void SetPose(int nType, int nKey, float nCounter, int nModelNumber);

private:
	ACTION_TYPE m_Action;		//モーション
	D3DXVECTOR3 m_pos;			//位置
	D3DXVECTOR3 m_rot;			//向き
	D3DXMATRIX m_mtxWorld;		//ワールドマトリックス
	D3DCOLORVALUE m_Color;		//色
	float m_fDel;				//色の減衰

	CModel* m_apModel[64];
	CMotion* m_pMotion;
	char* m_aModelName[64];
	int m_nNumModel;
};

#endif