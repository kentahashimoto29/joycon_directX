//============================================
//
//	2Dエディット [2DUI_edit.cpp]
//	Author:sakamoto kai
//
//============================================
#include "2DUI_edit.h"
#include "2DUI.h"
#include "object.h"
#include "object2D.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "debugproc.h"
#include "Effect.h"
#include "debugproc.h"
#include "texture.h"
#include "useful.h"

//マクロ定義
#define NUM_OBJECT (8)						//マップに出すオブジェクトの種類数
#define START_OK ("STARTSETUI")				//スタートメッセージがあるかどうかの確認
#define END_SET_OK ("ENDSETUI")				//エンドメッセージがあるかどうかの確認
#define UI_SET ("STARTSET")					//ブロックのセットメッセージ
#define DELETE_WIGHT (45.0f)				//削除処理の横の削除範囲
#define DELETE_HEIGHT (45.0f)				//削除処理の縦の削除範囲
#define SIDE_PRESSCOUNT (30)				//横に素早く移動するまでのカウント
#define MOVE_POS (10.0f)					//移動距離

// 静的メンバ変数宣言
C2DUIEdit::UIINFO C2DUIEdit::m_aTextureInfo[UI_LABEL_MAX] =
{
	{ "data\\TEXTURE\\RankAP.png","サンプル00" },
	{ "data\\TEXTURE\\RankA.png","サンプル01" },
	{ "data\\TEXTURE\\RankAM.png","サンプル02" },
	{ "data\\TEXTURE\\RankBP.png","サンプル03" },
	{ "data\\TEXTURE\\RankB.png","サンプル04" },
	{ "data\\TEXTURE\\RankBM.png","サンプル05" },
	{ "data\\TEXTURE\\RankC.png","サンプル06" },
};
CObject2D* C2DUIEdit::m_pEditUI = NULL;			//ブロックへのポインタ

//====================================================================
//コンストラクタ
//====================================================================
C2DUIEdit::C2DUIEdit()
{
	m_EditPos = INITVECTOR3;
	m_EditRot = INITVECTOR3;
	m_EditSize = D3DXVECTOR3(100.0f, 100.0f, 0.0f);
	ObjectType = 0;
	m_nSideDistance = 10.0f;
	m_nSideCount = 0;
	m_cTextureName = m_aTextureInfo[ObjectType].pFilename;
}

//====================================================================
//デストラクタ
//====================================================================
C2DUIEdit::~C2DUIEdit()
{

}

//====================================================================
//生成処理
//====================================================================
C2DUIEdit* C2DUIEdit::Create()
{
	C2DUIEdit* pMap = NULL;

	if (pMap == NULL)
	{
		//オブジェクト2Dの生成
		pMap = new C2DUIEdit();
	}

	m_pEditUI = CObject2D::Create();

	// テクスチャの読み込み処理
	m_pEditUI->SetTexture(C2DUIEdit::m_aTextureInfo[0].pFilename);

	return pMap;
}

//====================================================================
//終了処理
//====================================================================
void C2DUIEdit::Uninit(void)
{

}


//====================================================================
//更新処理
//====================================================================
void C2DUIEdit::Update(void)
{
	C2DUI* pEditUI = NULL;

	//キーボードの取得
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	//エディットの位置を動かす----------------------------------------------------------
	if (pInputKeyboard->GetTrigger(DIK_W) == true)
	{
		m_EditPos.y -= MOVE_POS;
	}
	if (pInputKeyboard->GetTrigger(DIK_S) == true)
	{
		m_EditPos.y += MOVE_POS;
	}

	if (pInputKeyboard->GetTrigger(DIK_A) == true)
	{
		m_EditPos.x -= MOVE_POS;
	}
	if (pInputKeyboard->GetTrigger(DIK_D) == true)
	{
		m_EditPos.x += MOVE_POS;
	}

	//長押し移動----------------------------------------------------------------------
	if (pInputKeyboard->GetPress(DIK_W) == true)
	{
		if (m_nSideCount > SIDE_PRESSCOUNT && m_nSideCount % 2 == 0)
		{
			m_EditPos.y -= MOVE_POS * 2.0f;
		}
		m_nSideCount++;
	}
	else if (pInputKeyboard->GetPress(DIK_S) == true)
	{
		if (m_nSideCount > SIDE_PRESSCOUNT && m_nSideCount % 2 == 0)
		{
			m_EditPos.y += MOVE_POS * 2.0f;
		}
		m_nSideCount++;
	}
	else if (pInputKeyboard->GetPress(DIK_A) == true)
	{
		if (m_nSideCount > SIDE_PRESSCOUNT && m_nSideCount % 2 == 0)
		{
			m_EditPos.x -= MOVE_POS * 2.0f;
		}
		m_nSideCount++;
	}
	else if (pInputKeyboard->GetPress(DIK_D) == true)
	{
		if (m_nSideCount > SIDE_PRESSCOUNT && m_nSideCount % 2 == 0)
		{
			m_EditPos.x += MOVE_POS * 2.0f;
		}
		m_nSideCount++;
	}

	if (pInputKeyboard->GetPress(DIK_A) == false &&
		pInputKeyboard->GetPress(DIK_D) == false &&
		pInputKeyboard->GetPress(DIK_W) == false &&
		pInputKeyboard->GetPress(DIK_S) == false)
	{
		m_nSideCount = 0;
	}

	//オブジェクト切り替え----------------------------------------------------------
	if (pInputKeyboard->GetTrigger(DIK_0) == true)
	{
		ObjectType++;

		if (ObjectType >= UI_LABEL_MAX)
		{
			ObjectType = 0;
		}

		m_cTextureName = m_aTextureInfo[ObjectType].pFilename;

		// テクスチャの読み込み処理
		m_pEditUI->SetTexture(m_cTextureName);
	}

	//向きと位置の切り替え----------------------------------------------------------
	if (pInputKeyboard->GetTrigger(DIK_9) == true)
	{
		m_EditRot.y += 1.57f;

		if (m_EditRot.y >= 6.0f)
		{
			m_EditRot.y = 0;
		}
	}

	//とげの移動範囲調整----------------------------------------------------------
	if (pInputKeyboard->GetTrigger(DIK_3) == true)
	{
		m_nSideDistance -= 5.0f;
	}
	if (pInputKeyboard->GetTrigger(DIK_4) == true)
	{
		m_nSideDistance += 5.0f;
	}

	//とげの移動範囲調整----------------------------------------------------------
	if (pInputKeyboard->GetTrigger(DIK_5) == true)
	{
		m_EditSize.x -= m_nSideDistance;
	}
	if (pInputKeyboard->GetTrigger(DIK_6) == true)
	{
		m_EditSize.x += m_nSideDistance;
	}
	if (pInputKeyboard->GetTrigger(DIK_7) == true)
	{
		m_EditSize.y -= m_nSideDistance;
	}
	if (pInputKeyboard->GetTrigger(DIK_8) == true)
	{
		m_EditSize.y += m_nSideDistance;
	}

	//位置更新----------------------------------------------------------
	m_pEditUI->SetPos(m_EditPos);
	m_pEditUI->SetRot(m_EditRot);
	m_pEditUI->SetWidth(m_EditSize.x);
	m_pEditUI->SetHeight(m_EditSize.y);

	//オブジェクト設置----------------------------------------------------------
	if (pInputKeyboard->GetTrigger(DIK_RETURN) == true)
	{
		pEditUI = C2DUI::Create();
		pEditUI->SetPos(m_EditPos);
		pEditUI->SetRot(m_EditRot);
		pEditUI->SetWidth(m_EditSize.x);
		pEditUI->SetHeight(m_EditSize.y);
		pEditUI->SetTexture(m_cTextureName);
	}

	if (pInputKeyboard->GetTrigger(DIK_BACKSPACE) == true)
	{
		DeleteObject(m_EditPos);
	}

	if (pInputKeyboard->GetTrigger(DIK_F9) == true)
	{
		SaveData();
	}

	DebugObject();
}

//====================================================================
//削除処理
//====================================================================
void C2DUIEdit::DeleteObject(D3DXVECTOR3 pos)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		//オブジェクトを取得
		CObject* pObj = CObject::GetTop(nCntPriority);

		while (pObj != NULL)
		{
			CObject* pObjNext = pObj->GetNext();

			CObject::TYPE type = pObj->GetType();				//種類を取得

			if (type == CObject::TYPE_2DUI)
			{//種類がブロックの時
				C2DUI* pUI = (C2DUI*)pObj;

				if (useful::CollisionRectangle2D(pos, pUI->GetPos(), D3DXVECTOR3(DELETE_WIGHT, DELETE_HEIGHT, 0.0f), D3DXVECTOR3(pUI->GetWidth() * 0.5f, pUI->GetHeight() * 0.5f, 0.0f),useful::COLLISION_XY) == true)
				{
					pUI->Uninit();
				}
			}

			pObj = pObjNext;
		}
	}
}

//====================================================================
//デバッグ表示
//====================================================================
void C2DUIEdit::DebugObject(void)
{
	CManager::GetInstance()->GetDebugProc()->Print("\n%s\n", "ーーーーーエディットモードーーーーー");

	CManager::GetInstance()->GetDebugProc()->Print("\n%s\n", "操作説明ーー");
	//操作説明
	CManager::GetInstance()->GetDebugProc()->Print("%s\n", "移動：[W],[A],[S],[D]");
	CManager::GetInstance()->GetDebugProc()->Print("%s\n", "テクスチャ変更：[０]");
	CManager::GetInstance()->GetDebugProc()->Print("%s\n", "サイズ変更値の下降上昇：[３：４]");
	CManager::GetInstance()->GetDebugProc()->Print("%s\n", "UIの横サイズの下降上昇：[５：６]");
	CManager::GetInstance()->GetDebugProc()->Print("%s\n", "UIの縦サイズの下降上昇：[７：８]");
	CManager::GetInstance()->GetDebugProc()->Print("%s\n", "オブジェクト回転：[９]");
	CManager::GetInstance()->GetDebugProc()->Print("%s\n", "セーブ処理：[F9]");

	CManager::GetInstance()->GetDebugProc()->Print("\n%s\n", "ーーエディット情報ーー");

	//オブジェクト
	CManager::GetInstance()->GetDebugProc()->Print("テクスチャ：[%s]\n", m_aTextureInfo[ObjectType].pCommentname);

	//位置・向き・大きさ
	CManager::GetInstance()->GetDebugProc()->Print("位置：[%f:%f]\n", m_EditPos.x, m_EditPos.y);
	CManager::GetInstance()->GetDebugProc()->Print("サイズ変更値：[%f]\n", m_nSideDistance);
	CManager::GetInstance()->GetDebugProc()->Print("サイズ：[%f:%f]\n", m_EditSize.x, m_EditSize.y);
	CManager::GetInstance()->GetDebugProc()->Print("向き：[%f]\n", m_EditRot.y);

	//とげの移動範囲
	CManager::GetInstance()->GetDebugProc()->Print("\n%s\n", "ーーーーーエディットモードーーーーー");
}

//====================================================================
//保存処理
//====================================================================
void C2DUIEdit::SaveData(void)
{
	FILE* pFile; //ファイルポインタを宣言

	//ファイルを開く
	pFile = fopen(LOADUI_NAME, "w");

	if (pFile != NULL)
	{//ファイルが開けた場合

	 //ステージをセーブする開始の合図
		fprintf(pFile, "%s\n\n", START_OK);

		for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
		{
			//オブジェクトを取得
			CObject* pObj = CObject::GetTop(nCntPriority);

			while (pObj != NULL)
			{
				CObject* pObjNext = pObj->GetNext();

				CObject::TYPE type = pObj->GetType();				//種類を取得

				if (type == CObject::TYPE_2DUI)
				{//種類がブロックの時
					C2DUI* pUI = (C2DUI*)pObj;

					fprintf(pFile, "%s\n", UI_SET);

					//位置の取得
					fprintf(pFile, "%f ", pUI->GetPos().x);
					fprintf(pFile, "%f ", pUI->GetPos().y);
					fprintf(pFile, "%f\n", pUI->GetPos().z);

					//向きの取得
					fprintf(pFile, "%f ", pUI->GetRot().x);
					fprintf(pFile, "%f ", pUI->GetRot().y);
					fprintf(pFile, "%f\n", pUI->GetRot().z);

					//大きさの取得
					fprintf(pFile, "%f ", pUI->GetWidth());
					fprintf(pFile, "%f\n", pUI->GetHeight());

					fprintf(pFile, "%s\n", pUI->GetTexture());

					fprintf(pFile, "%s\n\n", "ENDSET");
				}

				pObj = pObjNext;
			}
		}

		//ステージをセーブした終了の合図
		fprintf(pFile, "%s", END_SET_OK);

		fclose(pFile);
	}
	else
	{//ファイルが開けなかった場合
		printf("***ファイルを開けませんでした***\n");
	}
}

//====================================================================
//マップを全て埋めて生成
//====================================================================
void C2DUIEdit::LoadData(char* Name, D3DXVECTOR3 pos)
{
	FILE* pFile; //ファイルポインタを宣言

	//ファイルを開く
	pFile = fopen(Name, "r");

	if (pFile != NULL)
	{//ファイルが開けた場合
		D3DXVECTOR3 SetPos = INITVECTOR3;
		D3DXVECTOR3 SetRot = INITVECTOR3;
		D3DXVECTOR3 SetSize = INITVECTOR3;
		char TexName[128] = {};
		int nTex = 0;
		char aStartMessage[32] = {};	//スタートメッセージ
		char aSetMessage[32] = {};		//セットメッセージ

		fscanf(pFile, "%s", &aStartMessage[0]);
		if (strcmp(&aStartMessage[0], START_OK) == 0)
		{
			while (1)
			{
				fscanf(pFile, "%s", &aSetMessage[0]);
				if (strcmp(&aSetMessage[0], END_SET_OK) == 0)
				{
					break;
				}
				else if (strcmp(&aSetMessage[0], UI_SET) == 0)
				{
					fscanf(pFile, "%f", &SetPos.x);
					fscanf(pFile, "%f", &SetPos.y);
					fscanf(pFile, "%f", &SetPos.z);
					fscanf(pFile, "%f", &SetRot.x);
					fscanf(pFile, "%f", &SetRot.y);
					fscanf(pFile, "%f", &SetRot.z);
					fscanf(pFile, "%f", &SetSize.x);
					fscanf(pFile, "%f", &SetSize.y);
					fscanf(pFile, "%s", &TexName[0]);
					fscanf(pFile, "%s", &aSetMessage[0]);

					C2DUI* pUI = C2DUI::Create();
					pUI->SetPos(pos + SetPos);
					pUI->SetRot(SetRot);
					pUI->SetWidth(SetSize.x);
					pUI->SetHeight(SetSize.y);
					pUI->SetTexture(&TexName[0]);
				}
			}
		}
		fclose(pFile);
	}
	else
	{//ファイルが開けなかった場合
		printf("***ファイルを開けませんでした***\n");
	}
}