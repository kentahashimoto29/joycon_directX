//========================================================================================
//
//　ジョイコンの処理[joycon.h]
//	Author:sakamoto kai
//
//========================================================================================
#ifndef _JOYCON_H_
#define _JOYCON_H_

#include "main.h"
#include "hidapi\hidapi.h"
#include <stdint.h>


//ジョイコンのクラス宣言
class CJoycon
{
public:
	CJoycon();
	~CJoycon();

	//===============================
	// サブコマンド
	//===============================
	typedef enum
	{
		SUBCMD_LIGHT_SET = 0x30,        //プレイヤーライトの設定
		SUBCMD_LIGHT_GET = 0x31,        //プレイヤーライトの取得
		SUBCMD_LIGHT_HOME = 0x38,       //HOMEライトの設定
		SUBCMD_IMU = 0x40,              //6軸センサーの有効化
		SUBCMD_IMU_SENSI_SET = 0x41,    //6軸センサーの感度設定
		SUBCMD_IMU_REGI_WRITE = 0x42,   //6軸センサーのレジスター書込
		SUBCMD_IMU_REGI_READ = 0x43,    //6軸センサーのレジスター読込
		SUBCMD_VIB = 0x48,              //振動の有効化
		SUBCMD_MAX
	}SUBCOMMAND;

	//===============================
	// ボタン(左右それぞれ)(buff[1])
	//===============================
	typedef enum
	{
		Y = 0x01,   //1
		X = 0x02,   //2
		B = 0x04,   //4
		A = 0x08,   //8
		SR = 0x10,  //16
		SL = 0x20,  //32
		//R = 0x40,   //64
		//ZR = 0x80,  //128
	}JOYCON_BUTTON;

	//===============================
	// 特殊ボタン (左右合一ボタン)(buff[2])
	//===============================
	typedef enum
	{
		MINUS = 0x01,   //1
		PLUS = 0x02,   //2
		STICK_L = 0x04,   //4
		STICK_R = 0x08,   //8
		HOME = 0x10,  //16
		PHOTO = 0x20,  //32
		LR = 0x40,   //64
		ZLZR = 0x80,  //128
	}JOYCON_BUTTON_SP;

	//===============================
	//スティック
	//===============================
	typedef enum
	{
		JOYSTICK_U = 0,
		JOYSTICK_UR,
		JOYSTICK_R,
		JOYSTICK_DR,
		JOYSTICK_D,
		JOYSTICK_DL,
		JOYSTICK_L,
		JOYSTICK_UL,
		JOYSTICK_NUT,
		JOYSTICK_MAX,
	}JOYCON_STICK;

	void SendSubcommand(hid_device* dev, uint8_t command, uint8_t data[], int len, int* globalCount);
	void enable_sensors(hid_device* handle);
	hid_device* open_joycon();

	void Sensor_Update();

	void Update();

protected:
	float accel_correction[2];
	float gyro_radian[2];

	D3DXVECTOR3 inverse_vector;

	int globalCount = 0;
	hid_device* dev;
	uint8_t buff[0x40];
	size_t size;
};

#endif
