//============================================
//
//	エディット [Edit.cpp]
//	Author:sakamoto kai
//
//============================================
#include "Edit.h"
#include "object.h"
#include "renderer.h"
#include "manager.h"
#include "game.h"
#include "input.h"
#include "objmeshCube.h"
#include "CubeBlock.h"
#include "debugproc.h"
#include "MapModel.h"
#include "XModel.h"

//マクロ定義

//静的メンバ変数宣言
CEdit::OBJINFO CEdit::m_aModelInfo[OBJ_HIT_MAX] =
{
	{ "data\\MODEL\\WallJump.x","サンプル00" },
	{ "data\\MODEL\\WallMove.x","サンプル01" },
	{ "data\\MODEL\\WoodBoaerd.x","サンプル02" },
	{ "data\\MODEL\\WoodFence.x","サンプル03" },
	{ "data\\MODEL\\MapSwitch.x","サンプル04" },
	{ "data\\MODEL\\MapPtiFloor.x","サンプル05" },
	{ "data\\MODEL\\player00.x","サンプル06" },
};

//====================================================================
//コンストラクタ
//====================================================================
CEdit::CEdit()
{
	m_nPressCount = 0;
	m_nModelNumber = 0;

	if (m_pEditBlock == nullptr)
	{
		m_pEditBlock = CObjmeshCube::Create();
		m_pEditBlock->SetSize(D3DXVECTOR3(100.0f, 10.0f, 100.0f));
		m_pEditBlock->SetAppear(false);
	}

	if (m_pEditModel == nullptr)
	{
		m_pEditModel = CMapModel::Create(m_aModelInfo[m_nModelNumber].pFilename);
		m_pEditModel->SetAppear(false);
		AppearCollision();
	}

	m_EditPos = INITVECTOR3;
	m_EditRot = INITVECTOR3;
	m_EditSize = D3DXVECTOR3(100.0f, 10.0f, 100.0f);
	m_EditType = EDITTYPE_MODELHIT;
	MoveWidth = 10.0f;
	m_bModelCollision = false;
	m_bAppearCollision = false;
}

//====================================================================
//デストラクタ
//====================================================================
CEdit::~CEdit()
{

}

//====================================================================
//生成処理
//====================================================================
CEdit* CEdit::Create()
{
	CEdit* pMap = NULL;

	if (pMap == NULL)
	{
		//オブジェクト2Dの生成
		pMap = new CEdit();
	}

	return pMap;
}

//====================================================================
//終了処理
//====================================================================
void CEdit::Uninit(void)
{

}

//====================================================================
//更新処理
//====================================================================
void CEdit::Update(void)
{
	//キーボードの取得
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	//移動処理
	Move();

	if (pInputKeyboard->GetTrigger(DIK_9) == true)
	{
		if (m_EditType == EDITTYPE_MODELHIT)
		{
			m_EditType = EDITTYPE_BLOCK;
			m_pEditBlock->SetAppear(true);
			m_pEditModel->SetAppear(false);
		}
		else if (m_EditType == EDITTYPE_BLOCK)
		{
			m_EditType = EDITTYPE_MODELHIT;
			m_pEditBlock->SetAppear(false);
			m_pEditModel->SetAppear(true);
		}
	}

	//デバッグ表示の取得
	CDebugProc* pDebugProc = CManager::GetInstance()->GetDebugProc();
	pDebugProc->Print("========== {エディットモード起動中} ==========\n");
	pDebugProc->Print("エディットの位置 [%f]:[%f]:[%f]\n", m_EditPos.x, m_EditPos.y, m_EditPos.z);
	pDebugProc->Print("変動の幅 [%f]\n", MoveWidth);
	pDebugProc->Print("-----[操作説明]-----\n");
	pDebugProc->Print("エディット位置の移動 : [W][A][S][D][LShift][LControl]\n");
	pDebugProc->Print("カメラ位置の移動     : [T][F][G][H][RShift][RControl]\n");
	pDebugProc->Print("視点の移動           : [I][J][K][L]\n");
	pDebugProc->Print("種類の切り替え       : [9]\n");
	pDebugProc->Print("設置情報の保存       : [F9]\n");

	switch (m_EditType)
	{
	case CEdit::EDITTYPE_BLOCK:

		UpdateBlock();

		break;
	case CEdit::EDITTYPE_MODELHIT:

		UpdateXModel();

		break;
	case CEdit::EDITTYPE_MODELNOT:
		break;
	case CEdit::EDITTYPE_ENEMY:
		break;
	case CEdit::EDITTYPE_MAX:
		break;
	default:
		break;
	}

	if (pInputKeyboard->GetTrigger(DIK_F9) == true)
	{
		SaveData();
	}

	pDebugProc->Print("========== {エディットモード起動中} ==========\n");
}

//====================================================================
//	ブロックの更新処理
//====================================================================
void CEdit::UpdateBlock(void)
{
	//キーボードの取得
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	m_pEditBlock->SetAppear(true);
	m_pEditBlock->SetPos(m_EditPos);

	m_pEditBlock->SetSize(m_EditSize);

	if (pInputKeyboard->GetTrigger(DIK_E) == true)
	{
		m_EditSize.x += MoveWidth;
	}

	if (pInputKeyboard->GetTrigger(DIK_Q) == true)
	{
		m_EditSize.x -= MoveWidth;
	}

	if (pInputKeyboard->GetTrigger(DIK_Y) == true)
	{
		m_EditSize.y += MoveWidth;
	}

	if (pInputKeyboard->GetTrigger(DIK_R) == true)
	{
		m_EditSize.y -= MoveWidth;
	}

	if (pInputKeyboard->GetTrigger(DIK_O) == true)
	{
		m_EditSize.z += MoveWidth;
	}

	if (pInputKeyboard->GetTrigger(DIK_U) == true)
	{
		m_EditSize.z -= MoveWidth;
	}

	if (pInputKeyboard->GetTrigger(DIK_RETURN) == true)
	{
		CCubeBlock* pBlock = CCubeBlock::Create();
		pBlock->SetPos(m_pEditBlock->GetPos());
		pBlock->SetSize(m_pEditBlock->GetSize());
		AppearCollision();
	}

	if (pInputKeyboard->GetTrigger(DIK_BACKSPACE) == true)
	{
		CollisionBlock();
	}

	//デバッグ表示の取得
	CDebugProc* pDebugProc = CManager::GetInstance()->GetDebugProc();
	pDebugProc->Print("ブロックのサイズ [%f]:[%f]:[%f]\n", m_pEditBlock->GetSize().x, m_pEditBlock->GetSize().y, m_pEditBlock->GetSize().z);
	pDebugProc->Print("サイズ変更キー : X軸 [Q- : E+] \n");
	pDebugProc->Print("サイズ変更キー : Y軸 [R- : Y+] \n");
	pDebugProc->Print("サイズ変更キー : Z軸 [U- : O+] \n");
}

//====================================================================
//	Xモデルの更新処理
//====================================================================
void CEdit::UpdateXModel(void)
{
	//各種情報の取得
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();
	CXModel* pXModel = CManager::GetInstance()->GetXModel();

	m_pEditModel->SetAppear(true);
	m_pEditModel->SetPos(m_EditPos);

	if (pInputKeyboard->GetTrigger(DIK_5) == true)
	{
		m_bModelCollision = m_pEditModel->GetCollision();

		m_bModelCollision = !m_bModelCollision;

		m_pEditModel->SetCollision(m_bModelCollision);
	}

	if (pInputKeyboard->GetTrigger(DIK_6) == true)
	{
		m_EditRot.y += D3DX_PI * 0.5f;
		m_pEditModel->SwapSize();

		if(m_EditRot.y >= D3DX_PI * 2.0f)
		{
			m_EditRot.y = 0.0f;
		}

		m_pEditModel->SetRot(m_EditRot);
	}

	if (pInputKeyboard->GetTrigger(DIK_7) == true)
	{
		m_nModelNumber--;

		if (m_nModelNumber < OBJ_HIT_TYPE1)
		{
			m_nModelNumber = OBJ_HIT_MAX - 1;
		}

		m_pEditModel->SetXModelIdx(pXModel->Regist(m_aModelInfo[m_nModelNumber].pFilename));
		m_pEditModel->Test();


		if (m_EditRot.y < 1.57f)
		{

		}
		else if (m_EditRot.y < 3.14f)
		{
			m_pEditModel->SwapSize();
		}
		else if (m_EditRot.y < 4.71f)
		{

		}
		else
		{
			m_pEditModel->SwapSize();
		}
	}

	if (pInputKeyboard->GetTrigger(DIK_8) == true)
	{
		m_nModelNumber++;

		if (m_nModelNumber >= OBJ_HIT_MAX)
		{
			m_nModelNumber = OBJ_HIT_TYPE1;
		}

		m_pEditModel->SetXModelIdx(pXModel->Regist(m_aModelInfo[m_nModelNumber].pFilename));
		m_pEditModel->Test();

		if (m_EditRot.y < 1.57f)
		{

		}
		else if (m_EditRot.y < 3.14f)
		{
			m_pEditModel->SwapSize();
		}
		else if (m_EditRot.y < 4.71f)
		{

		}
		else
		{
			m_pEditModel->SwapSize();
		}
	}

	if (pInputKeyboard->GetTrigger(DIK_RETURN) == true)
	{
		CMapModel* pModel = CMapModel::Create(m_aModelInfo[m_nModelNumber].pFilename);
		pModel->SetEditIdx(m_nModelNumber);
		pModel->SetPos(m_EditPos);
		pModel->SetRot(m_EditRot);

		if (m_EditRot.y < 1.57f)
		{

		}
		else if (m_EditRot.y < 3.14f)
		{
			pModel->SwapSize();
		}
		else if (m_EditRot.y < 4.71f)
		{

		}
		else
		{
			pModel->SwapSize();
		}

		pModel->SetCollision(m_bModelCollision);
		AppearCollision();
	}

	if (pInputKeyboard->GetTrigger(DIK_BACKSPACE) == true)
	{
		CollisionXModel();
	}

	m_pEditModel->UpdateBlock();

	//デバッグ表示の取得
	CDebugProc* pDebugProc = CManager::GetInstance()->GetDebugProc();
	pDebugProc->Print("モデルの当たり判定切り替え : [5]\n");
	pDebugProc->Print("向きの90度回転: [6]\n");
	pDebugProc->Print("モデル変更: [7 : 8]\n");
	if (m_bModelCollision == true)
	{
		pDebugProc->Print("モデルの当たり判定 : [ TRUE ]\n");
	}
	else
	{
		pDebugProc->Print("モデルの当たり判定 : [ FALSE ]\n");
	}
}

//====================================================================
//移動処理
//====================================================================
void CEdit::Move(void)
{
	//キーボードの取得
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();

	if (pInputKeyboard->GetPress(DIK_W) == true ||
		pInputKeyboard->GetPress(DIK_A) == true ||
		pInputKeyboard->GetPress(DIK_S) == true ||
		pInputKeyboard->GetPress(DIK_D) == true ||
		pInputKeyboard->GetPress(DIK_LSHIFT) == true ||
		pInputKeyboard->GetPress(DIK_LCONTROL) == true)
	{
		m_nPressCount++;
	}
	else
	{
		m_nPressCount = 0;
	}

	if (m_nPressCount >= 30 && m_nPressCount % 2 == 0)
	{
		if (pInputKeyboard->GetPress(DIK_W) == true)
		{
			m_EditPos.z += MoveWidth;
		}

		if (pInputKeyboard->GetPress(DIK_S) == true)
		{
			m_EditPos.z -= MoveWidth;
		}

		if (pInputKeyboard->GetPress(DIK_A) == true)
		{
			m_EditPos.x -= MoveWidth;
		}

		if (pInputKeyboard->GetPress(DIK_D) == true)
		{
			m_EditPos.x += MoveWidth;
		}

		if (pInputKeyboard->GetPress(DIK_LSHIFT) == true)
		{
			m_EditPos.y += MoveWidth;
		}

		if (pInputKeyboard->GetPress(DIK_LCONTROL) == true)
		{
			m_EditPos.y -= MoveWidth;
		}
	}
	else
	{
		if (pInputKeyboard->GetTrigger(DIK_W) == true)
		{
			m_EditPos.z += MoveWidth;
		}

		if (pInputKeyboard->GetTrigger(DIK_S) == true)
		{
			m_EditPos.z -= MoveWidth;
		}

		if (pInputKeyboard->GetTrigger(DIK_A) == true)
		{
			m_EditPos.x -= MoveWidth;
		}

		if (pInputKeyboard->GetTrigger(DIK_D) == true)
		{
			m_EditPos.x += MoveWidth;
		}

		if (pInputKeyboard->GetTrigger(DIK_LSHIFT) == true)
		{
			m_EditPos.y += MoveWidth;
		}

		if (pInputKeyboard->GetTrigger(DIK_LCONTROL) == true)
		{
			m_EditPos.y -= MoveWidth;
		}
	}

	if (pInputKeyboard->GetTrigger(DIK_UP) == true)
	{
		MoveWidth += 10.0f;
	}

	if (pInputKeyboard->GetTrigger(DIK_DOWN) == true)
	{
		MoveWidth -= 10.0f;
	}

	if (pInputKeyboard->GetTrigger(DIK_RIGHT) == true)
	{
		MoveWidth += 3.0f;
	}

	if (pInputKeyboard->GetTrigger(DIK_LEFT) == true)
	{
		MoveWidth -= 3.0f;
	}
}

//====================================================================
//ブロックの当たり判定処理
//====================================================================
void CEdit::CollisionBlock(void)
{
	if (m_pEditBlock != nullptr)
	{
		for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
		{
			//オブジェクトを取得
			CObject* pObj = CObject::GetTop(nCntPriority);

			while (pObj != NULL)
			{
				CObject* pObjNext = pObj->GetNext();

				CObject::TYPE type = pObj->GetType();			//種類を取得

				if (type == CObject::TYPE_CUBEBLOCK)
				{//種類がブロックの時
					CCubeBlock* pBlock = (CCubeBlock*)pObj;

					D3DXVECTOR3 MyPos = m_pEditBlock->GetPos();
					D3DXVECTOR3 MySize = m_pEditBlock->GetSize() * 2.0f;
					D3DXVECTOR3 BlockPos = pBlock->GetPos();
					D3DXVECTOR3 BlockSize = pBlock->GetSize();
					D3DXVECTOR3 BlockMove = pBlock->GetMove();

					if (BlockPos.x + BlockSize.x > MyPos.x &&
						BlockPos.x - BlockSize.x < MyPos.x &&
						BlockPos.z + BlockSize.z > MyPos.z &&
						BlockPos.z - BlockSize.z < MyPos.z &&
						BlockPos.y + BlockSize.y > MyPos.y &&
						BlockPos.y - BlockSize.y < MyPos.y)
					{
						pBlock->Uninit();
					}
				}
				pObj = pObjNext;
			}
		}
	}
}

//====================================================================
//ブロックの当たり判定処理
//====================================================================
void CEdit::CollisionXModel(void)
{
	if (m_pEditModel != nullptr)
	{
		for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
		{
			//オブジェクトを取得
			CObject* pObj = CObject::GetTop(nCntPriority);

			while (pObj != NULL)
			{
				CObject* pObjNext = pObj->GetNext();

				CObject::TYPE type = pObj->GetType();			//種類を取得

				if (type == CObject::TYPE_MAPMODEL && pObj != m_pEditModel)
				{//種類がブロックの時
					CMapModel* pModel = (CMapModel*)pObj;

					D3DXVECTOR3 MyPos = m_pEditModel->GetPos();
					D3DXVECTOR3 MySize = m_pEditModel->GetSize();
					D3DXVECTOR3 BlockPos = pModel->GetPos();
					D3DXVECTOR3 BlockSize = pModel->GetSize();

					if (MyPos.x + MySize.x > BlockPos.x &&
						MyPos.x - MySize.x < BlockPos.x &&
						MyPos.z + MySize.z > BlockPos.z &&
						MyPos.z - MySize.z < BlockPos.z &&
						MyPos.y + MySize.y > BlockPos.y &&
						MyPos.y - MySize.y < BlockPos.y)
					{
						pModel->Uninit();
					}
				}
				pObj = pObjNext;
			}
		}
	}
}

//====================================================================
//当たり判定の可視化
//====================================================================
void CEdit::AppearCollision(void)
{
	if (m_pEditModel != nullptr)
	{
		for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
		{
			//オブジェクトを取得
			CObject* pObj = CObject::GetTop(nCntPriority);

			while (pObj != NULL)
			{
				CObject* pObjNext = pObj->GetNext();

				CObject::TYPE type = pObj->GetType();			//種類を取得

				if (type == CObject::TYPE_MAPMODEL)
				{//種類がブロックの時
					CMapModel* pModel = (CMapModel*)pObj;

					pModel->CreateBlock();
				}
				pObj = pObjNext;
			}
		}
	}
}

//====================================================================
//保存処理
//====================================================================
void CEdit::SaveData(void)
{
	SaveBlock();
	SaveXModel();
}

//====================================================================
//ブロックの保存処理
//====================================================================
void CEdit::SaveBlock(void)
{
	FILE* pFile; //ファイルポインタを宣言

//ファイルを開く
	pFile = fopen(DATA_BLOCK_NAME, "w");

	if (pFile != NULL)
	{//ファイルが開けた場合

	 //ステージをセーブする開始の合図
		fprintf(pFile, "%s\n\n", "STARTSETSTAGE");

		for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
		{
			//オブジェクトを取得
			CObject* pObj = CObject::GetTop(nCntPriority);

			while (pObj != NULL)
			{
				CObject* pObjNext = pObj->GetNext();

				CObject::TYPE type = pObj->GetType();				//種類を取得

				if (type == CObject::TYPE_CUBEBLOCK)
				{//種類がブロックの時
					CCubeBlock* pBlock = (CCubeBlock*)pObj;

					fprintf(pFile, "%s\n", "STARTSETBLOCK");

					//ステージをセーブした終了の合図
					fprintf(pFile, "POS %f %f %f\n", pBlock->GetPos().x, pBlock->GetPos().y, pBlock->GetPos().z);
					fprintf(pFile, "SIZE %f %f %f\n", pBlock->GetSize().x, pBlock->GetSize().y, pBlock->GetSize().z);

					fprintf(pFile, "%s\n\n", "ENDSETBLOCK");
				}

				pObj = pObjNext;
			}
		}

		//ステージをセーブした終了の合図
		fprintf(pFile, "%s", "ENDSETSTAGE");

		fclose(pFile);
	}
	else
	{//ファイルが開けなかった場合
		printf("***ファイルを開けませんでした***\n");
	}
}

//====================================================================
//Xモデルの保存処理
//====================================================================
void CEdit::SaveXModel(void)
{
	FILE* pFile; //ファイルポインタを宣言

//ファイルを開く
	pFile = fopen(DATA_XMODEL_NAME, "w");

	if (pFile != NULL)
	{//ファイルが開けた場合

	 //ステージをセーブする開始の合図
		fprintf(pFile, "%s\n\n", "STARTSETSTAGE");

		for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
		{
			//オブジェクトを取得
			CObject* pObj = CObject::GetTop(nCntPriority);

			while (pObj != NULL)
			{
				CObject* pObjNext = pObj->GetNext();

				CObject::TYPE type = pObj->GetType();				//種類を取得

				if (type == CObject::TYPE_MAPMODEL && pObj != m_pEditModel)
				{//種類がブロックの時
					CMapModel* pModel = (CMapModel*)pObj;

					fprintf(pFile, "%s\n", "STARTSETXMODEL");

					//ステージをセーブした終了の合図
					fprintf(pFile, "NAME %s\n", m_aModelInfo[pModel->GetEditIdx()].pFilename);
					fprintf(pFile, "POS %f %f %f\n", pModel->GetPos().x, pModel->GetPos().y, pModel->GetPos().z);
					fprintf(pFile, "ROT %f %f %f\n", pModel->GetRot().x, pModel->GetRot().y, pModel->GetRot().z);
					if (pModel->GetCollision() == true)
					{
						fprintf(pFile, "COLLISION 1\n");
					}
					else
					{
						fprintf(pFile, "COLLISION 0\n");
					}

					fprintf(pFile, "%s\n\n", "ENDSETXMODEL");
				}

				pObj = pObjNext;
			}
		}

		//ステージをセーブした終了の合図
		fprintf(pFile, "%s", "ENDSETSTAGE");

		fclose(pFile);
	}
	else
	{//ファイルが開けなかった場合
		printf("***ファイルを開けませんでした***\n");
	}
}