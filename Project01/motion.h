//============================================
//
//	モーション [motion.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _MOTION_H_
#define _MOTION_H_

//マクロ定義
#define MAX_INFO (32)	//情報の最大数
#define MAX_PARTS (32)	//パーツの総数
#define MAX_KEY (16)	//キーの総数

//前方宣言
class CModel;

//マップクラス
class CMotion
{
public:
	CMotion();
	~CMotion();

	//キーの構造体
	typedef struct
	{
		float fPosX;		//位置X
		float fPosY;		//位置Y
		float fPosZ;		//位置Z
		float fRotX;		//向きX
		float fRotY;		//向きY
		float fRotZ;		//向きZ
	}KEY;

	//キー情報の構造体
	typedef struct
	{
		int nFrame;					//再生フレーム
		KEY aKey[MAX_PARTS];		//各モデルのキー要素[パーツの総数]
	}KEY_INFO;

	//モーション情報の構造体
	typedef struct
	{
		bool bLoop;						//ループするかどうか
		int NumKey;						//各モデルのキー要素
		KEY_INFO aKeyInfo[MAX_KEY];		//キー情報[キーの総数]
	}INFO;

	void Set(int nType, float BlendTime);
	void SetStopPose(int nType, int nKey, float nCounter);
	void Update(void);
	void SetModel(CModel **ppModel, int nNumModel);
	void LoadData(const char* pFilename);
	int GetType(void) { return m_nType; }
	int GetKey(void) { return m_nKey; }
	float GetCounter(void) { return m_Counter; }

private:
	INFO m_aInfo[MAX_INFO];		//モーションの情報[モーションの総数]
	int m_nNumAll;				//モデル(パーツ)の総数
	int m_nType;				//モーションの種類
	bool m_bLoop;				//ループするかどうか
	int m_nNumKey;				//キーの総数
	int m_nKey;					//現在のキーNo.
	int m_nNextKey;				//現在の次のキーNo.
	float m_Counter;			//モーションのカウンター
	int m_nBlendType;			//モーションブレンド用の種類
	int m_nBlendKey;			//モーションブレンド用のキーNo.
	float m_nBlendCounter;		//モーションブレンド用のカウンター
	float m_nBlendTime;			//モーションブレンドの時間
	float m_nBlendTimeMax;		//モーションブレンドの時間
	bool m_bFinish;				//終了したかどうか

	CModel **m_ppModel;			//モデルへのポインタ
	int m_nNumModel;			//モデルの総数
};
#endif