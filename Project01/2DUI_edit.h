//============================================
//
//	2Dエディット [2DUI_edit.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _2DUI_EDIT_H_
#define _2DUI_EDIT_H_

#include "main.h"
#include "object.h"

#define LOADUI_NAME ("data\\TXT\\Set2DUI.txt")	//スタートメッセージがあるかどうかの確認

//前方宣言
class C2DUI;
class CObject2D;

//マップクラス
class C2DUIEdit
{
public:
	//-------------------------------------------------------------------------
	// UI一覧
	//-------------------------------------------------------------------------
	enum UI_LABEL
	{
		UI_LABEL_SAMPLE1 = 0,		// サンプル１
		UI_LABEL_SAMPLE2,			// サンプル２
		UI_LABEL_SAMPLE3,			// サンプル３
		UI_LABEL_SAMPLE4,			// サンプル４
		UI_LABEL_SAMPLE5,			// サンプル５
		UI_LABEL_SAMPLE6,			// サンプル６
		UI_LABEL_SAMPLE7,			// サンプル７
		UI_LABEL_MAX				// この列挙型の総数
	};

	struct UIINFO
	{
		char* pFilename;	// ファイル名
		char* pCommentname;	// コメント名
	};

	C2DUIEdit();
	~C2DUIEdit();

	static C2DUIEdit* Create();
	void Uninit(void);
	void Update(void);

	void LoadData(char* Name, D3DXVECTOR3 pos);

	D3DXVECTOR3 GetEditPos(void) { return m_EditPos; }
protected:

private:
	void DeleteObject(D3DXVECTOR3 pos);
	void DebugObject(void);
	void SaveData(void);

	static  CObject2D* m_pEditUI;
	int ObjectType;
	D3DXVECTOR3 m_EditPos;		//座標
	D3DXVECTOR3 m_EditRot;		//向き
	D3DXVECTOR3 m_EditSize;		//向き
	char* m_cTextureName;		//テクスチャの名前
	int m_nSideCount;			//移動カウント
	float m_nSideDistance;		//サイズ変更値
	static UIINFO m_aTextureInfo[UI_LABEL_MAX];	// テクスチャの情報
};
#endif