
//============================================
//
//	�Q�[�����[�h [gameMode.h]
//	Author:Sakai Minato
//
//============================================
#ifndef _GAME_MODE_H_
#define _GAME_MODE_H_

#include "main.h"

// �O���錾
class CObject2D;

// �Q�[�����[�h�̊��N���X
class CModeGame
{
public:

	// �Q�[�����[�h�̎��
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

// �T�����[�h
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

// �퓬���[�h
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