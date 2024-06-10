
//============================================
//
//	ゲームモード [gameMode.h]
//	Author:Sakai Minato
//
//============================================
#ifndef _GAME_MODE_H_
#define _GAME_MODE_H_

#include "main.h"

// 前方宣言
class CObject2D;

// ゲームモードの基底クラス
class CModeGame
{
public:

	// ゲームモードの種類
	enum MODE_GAME
	{
		MODE_GAME_NONE = 0,
		MODE_GAME_SEARCH,
		MODE_GAME_BATTLE,
		MODE_GAME_MAX
	};

	CModeGame();
	~CModeGame();

	virtual void Uninit(void) = 0;

	static void SetMode(MODE_GAME modeGame);
	MODE_GAME GetMode(void) { return m_modeGame; }

	static CModeGame* GetInstance(void) { return m_pInstance; }
	static void NullInstance(void) { m_pInstance = nullptr; }

private:

	MODE_GAME m_modeGame;

	static CModeGame* m_pInstance;

};

// 探索モード
class CModeSearch : public CModeGame
{
public:

	CModeSearch();
	~CModeSearch();

	static CModeSearch* Create(void);

	HRESULT Init(void);
	void Uninit(void) override;

private:

	CObject2D* m_pTest2d;

};

// 戦闘モード
class CModeBattle : public CModeGame
{
public:

	CModeBattle();
	~CModeBattle();

	static CModeBattle* Create(void);

	HRESULT Init(void);
	void Uninit(void) override;

private:

};

#endif