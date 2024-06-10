//========================================================================================
//
//　入力デバイスの処理[input.h]
//	Author:sakamoto kai
//
//========================================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"
#include "Xinput.h"

#pragma comment(lib,"xinput.lib")	//入力処理

//マクロ定義
#define NUM_KEY_MAX (256)	//キーの最大数
#define MAX_PLAYER (4)	//プレイヤーの人数

//入力のクラス宣言
class CInput
{
public:
	CInput();
	virtual ~CInput();

	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	virtual void Uninit();
	virtual void Update() = 0;

protected:
	static LPDIRECTINPUT8 m_pInput;
	LPDIRECTINPUTDEVICE8 pDevice;
};

//キーボードのクラス宣言
class CInputKeyboard :public CInput
{
public:
	CInputKeyboard();
	~CInputKeyboard();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	bool GetPress(int nKey);
	bool GetTrigger(int nKey);
	bool GetRerease(int nKey);

private:
	BYTE m_aKeyState[NUM_KEY_MAX];
	BYTE m_aKeyStateTrigger[NUM_KEY_MAX];
	BYTE m_aKeyStateRerease[NUM_KEY_MAX];

};

//ジョイパッドのクラス宣言
class CInputJoypad :public CInput
{
public:

	//列挙型定義
	typedef enum
	{
		BUTTON_UP = 0,	//十字キーの上ボタン
		BUTTON_DOWN,	//十字キーの下ボタン
		BUTTON_LEFT,	//十字キーの左ボタン
		BUTTON_RIGHT,	//十字キーの右ボタン
		BUTTON_START,	//スタートボタン
		BUTTON_BACK,	//バックボタン
		BUTTON_L_PUSH,	//左スティックの押込み
		BUTTON_R_PUSH,	//右スティックの押込み
		BUTTON_L,		//左ボタン
		BUTTON_R,		//右ボタン
		BUTTON_NAZO1,	//
		BUTTON_NAZO2,	//
		BUTTON_A,		//Aボタン
		BUTTON_B,		//Bボタン
		BUTTON_X,		//Xボタン
		BUTTON_Y,		//Yボタン
		BUTTON_MAX		//
	}JOYKEY;

	typedef enum
	{
		LSTICK_UP = 0,	//左スティックの上入力
		LSTICK_DOWN,	//左スティックの下入力
		LSTICK_RIGHT,	//左スティックの右入力
		LSTICK_LEFT,	//左スティックの左入力
		LSTICK_MAX,		//
	}JOYPAD_LSTICK;

	typedef enum
	{
		RSTICK_UP = 0,	//右スティックの上入力
		RSTICK_DOWN,	//右スティックの下入力
		RSTICK_RIGHT,	//右スティックの右入力
		RSTICK_LEFT,	//右スティックの左入力
		RSTICK_MAX,		//
	}JOYPAD_RSTICK;

	CInputJoypad();
	~CInputJoypad();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	bool GetPress(JOYKEY key, int nPlayer);
	bool GetTrigger(JOYKEY key, int nPlayer);
	bool GetRelease(JOYKEY key, int nPlayer);

	D3DXVECTOR3 Get_Stick_Left(int nPlayer);
	D3DXVECTOR3 Get_Stick_Right(int nPlayer);
	//float Get_Trigger_Left(int nPlayer);
	//float Get_Trigger_Right(int nPlayer);

	bool Get_LStick_Trigger(JOYPAD_LSTICK Type, int nPlayer);
	bool Get_RStick_Trigger(JOYPAD_RSTICK Type, int nPlayer);
	bool Get_RStick_Trigger_Test(int nPlayer);
	//void Get_Vibrtion(int nPlayer, int nLeftPower, int RightPoewr);
	//void Get_Vibrtion_false(int nPlayer);

private:
	DIJOYSTATE m_aKeyState; //全入力情報の保管
	DIJOYSTATE m_aKeyStateTrigger; //トリガー情報
	DIJOYSTATE m_aKeyStateRerease; //リリース情報
	DIJOYSTATE m_aKeyStateRepeat; //リピート情報
	BYTE m_aOldState[32]; //前回の入力情報を保存

	JOYPAD_LSTICK m_LStickInput;
	bool m_OutputDOK;
	bool m_OutputUOK;
	bool m_OutputLOK;
	bool m_OutputROK;
	bool m_OutputOK;

	XINPUT_STATE m_JoyKeyState[MAX_PLAYER];
	XINPUT_STATE m_JoyKeyStateTrigger[MAX_PLAYER];
	XINPUT_STATE m_JoyKeyStateRelease[MAX_PLAYER];
	XINPUT_VIBRATION m_vibration[MAX_PLAYER];
};

//マウスのクラス宣言
class CInputMouse :public CInput
{
public:

	//列挙型定義
	typedef enum
	{
		PUSH_LEFT = 0,
		PUSH_RIGHT,
		PUSH_WHEEL,
		PUSH_SIDE_1,
		PUSH_SIDE_2,
		PUSH_MAX,
	}MOUSE_PUSH;

	CInputMouse();
	~CInputMouse();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	bool GetPress(MOUSE_PUSH nKey);
	bool GetTrigger(MOUSE_PUSH nKey);
	bool GetRerease(MOUSE_PUSH nKey);
	D3DXVECTOR3 GetMouseMove(void);

private:
	LPDIRECTINPUTDEVICE8 m_pDevMouse = NULL;	//入力デバイスへのポインタ
	DIMOUSESTATE2 m_MouseState;					//全入力情報の保管
	DIMOUSESTATE2 m_MouseStateTrigger;			//トリガー情報
	DIMOUSESTATE2 m_MouseStateRerease;			//リリース情報
	DIMOUSESTATE2 m_MouseStateRepeat;			//リピート情報
	BYTE m_aOldState[8];						//前回の入力情報を保存

};

#endif
