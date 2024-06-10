//============================================
//
//	エディット [Edit.h]
//	Author:sakamoto kai
//
//============================================
#ifndef _EDIT_H_
#define _EDIT_H_

#include "main.h"
#include "object.h"

#define DATA_BLOCK_NAME ("data\\TXT\\STAGE\\Block.txt")
#define DATA_XMODEL_NAME ("data\\TXT\\STAGE\\XModel.txt")

//前方宣言
class CObjmeshCube;
class CMapModel;

//マップクラス
class CEdit
{
public:
	//-------------------------------------------------------------------------
// オブジェクト一覧
//-------------------------------------------------------------------------
	enum OBJ_HIT
	{
		OBJ_HIT_TYPE1 = 0,		// サンプル１
		OBJ_HIT_TYPE2,			// サンプル２
		OBJ_HIT_TYPE3,			// サンプル３
		OBJ_HIT_TYPE4,			// サンプル４
		OBJ_HIT_TYPE5,			// サンプル５
		OBJ_HIT_TYPE6,			// サンプル６
		OBJ_HIT_TYPE7,			// サンプル７
		OBJ_HIT_MAX				// この列挙型の総数
	};

	struct OBJINFO
	{
		char* pFilename;	// ファイル名
		char* pCommentname;	// コメント名
	};

	CEdit();
	~CEdit();

	enum EDITTYPE
	{
		EDITTYPE_BLOCK = 0,
		EDITTYPE_MODELHIT,
		EDITTYPE_MODELNOT,
		EDITTYPE_ENEMY,
		EDITTYPE_MAX,
	};

	static CEdit* Create();
	void Uninit(void);
	void Update(void);

protected:

private:
	void Move(void);			//移動処理
	void CollisionBlock(void);	//ブロックとの当たり判定
	void CollisionXModel(void);	//Xモデルとの当たり判定
	void SaveData(void);		//エディット情報の保存
	void SaveBlock(void);		//ブロックの保存
	void SaveXModel(void);		//Xモデルの保存
	void UpdateBlock(void);		//ブロックの更新
	void UpdateXModel(void);	//Xモデルの更新
	void AppearCollision(void);	//当たり判定の可視化

	int m_nPressCount;			//ボタン入力カウント
	D3DXVECTOR3 m_EditPos;		//設置位置
	D3DXVECTOR3 m_EditRot;		//設置角度
	D3DXVECTOR3 m_EditSize;		//設置サイズ
	float MoveWidth;			//移動幅
	EDITTYPE m_EditType;		//エディットで配置するオブジェクトの種類
	CObjmeshCube* m_pEditBlock;	//ブロックのポインタ
	CMapModel* m_pEditModel;	//マップモデルのポインタ
	int m_nModelNumber;			//移動幅
	bool m_bModelCollision;		//当たり判定を付与するかどうか
	bool m_bAppearCollision;		//当たり判定を可視化するかどうか

	static OBJINFO m_aModelInfo[OBJ_HIT_MAX];	// テクスチャの情報
};
#endif