//============================================
//
//	モーション [motion.cpp]
//	Author:sakamoto kai
//
//============================================
#include "motion.h"
#include "model.h"
#include "manager.h"
#include <stdio.h>

//====================================================================
//コンストラクタ
//====================================================================
CMotion::CMotion()
{
	m_ppModel = NULL;
	m_nType = 0;					//モーションの種類の設定
	m_bLoop = m_aInfo[m_nType].bLoop;	//ループするかどうかの設定
	m_nKey = 0;							//現在のキーNoを0にする
	m_nNextKey = m_nKey + 1;			//次のキーNoを1にする
	m_Counter = 0;						//モーションのカウンターを0にする
	m_bFinish = false;					//終了していない状態にする
}

//====================================================================
//デストラクタ
//====================================================================
CMotion::~CMotion()
{

}

//====================================================================
//モーションの設定処理
//====================================================================
void CMotion::Set(int nType, float BlendTime)
{
	m_nBlendType = m_nType;
	m_nBlendKey = m_nKey;
	m_nBlendCounter = 0;
	m_nBlendTime = BlendTime;
	m_nBlendTimeMax = m_nBlendTime;
	m_nType = nType;					//モーションの種類の設定
	m_bLoop = m_aInfo[m_nType].bLoop;	//ループするかどうかの設定
	m_nKey = 0;							//現在のキーNoを0にする
	m_nNextKey = m_nKey + 1;			//次のキーNoを1にする
	m_Counter = 0;						//モーションのカウンターを0にする
	m_bFinish = false;					//終了していない状態にする
}

//====================================================================
//モーションの設定処理
//====================================================================
void CMotion::SetStopPose(int nType, int nKey, float nCounter)
{
	m_nType = nType;					//モーションの種類の設定
	m_nKey = nKey;						//現在のキーNoを0にする
	m_nNextKey = m_nKey + 1;			//次のキーNoを1にする
	m_Counter = nCounter;				//モーションのカウンターを0にする

	//全モデル(パーツ)の更新
	for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{
		//キーの情報から位置・向きを算出
		float POSX = m_aInfo[m_nType].aKeyInfo[m_nNextKey].aKey[nCntModel].fPosX - m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].fPosX;
		float POSY = m_aInfo[m_nType].aKeyInfo[m_nNextKey].aKey[nCntModel].fPosY - m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].fPosY;
		float POSZ = m_aInfo[m_nType].aKeyInfo[m_nNextKey].aKey[nCntModel].fPosZ - m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].fPosZ;
		float ROTX = m_aInfo[m_nType].aKeyInfo[m_nNextKey].aKey[nCntModel].fRotX - m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].fRotX;
		float ROTY = m_aInfo[m_nType].aKeyInfo[m_nNextKey].aKey[nCntModel].fRotY - m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].fRotY;
		float ROTZ = m_aInfo[m_nType].aKeyInfo[m_nNextKey].aKey[nCntModel].fRotZ - m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].fRotZ;

		//パーツの位置・向きを設定
		m_ppModel[nCntModel]->SetPos(D3DXVECTOR3
		(
			m_ppModel[nCntModel]->GetStartPos().x + m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].fPosX + POSX * ((float)m_Counter / (float)m_aInfo[m_nType].aKeyInfo[m_nKey].nFrame),
			m_ppModel[nCntModel]->GetStartPos().y + m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].fPosY + POSY * ((float)m_Counter / (float)m_aInfo[m_nType].aKeyInfo[m_nKey].nFrame),
			m_ppModel[nCntModel]->GetStartPos().z + m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].fPosZ + POSZ * ((float)m_Counter / (float)m_aInfo[m_nType].aKeyInfo[m_nKey].nFrame)
		));

		m_ppModel[nCntModel]->SetRot(D3DXVECTOR3
		(
			m_ppModel[nCntModel]->GetStartRot().x + m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].fRotX + ROTX * ((float)m_Counter / (float)m_aInfo[m_nType].aKeyInfo[m_nKey].nFrame),
			m_ppModel[nCntModel]->GetStartRot().y + m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].fRotY + ROTY * ((float)m_Counter / (float)m_aInfo[m_nType].aKeyInfo[m_nKey].nFrame),
			m_ppModel[nCntModel]->GetStartRot().z + m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].fRotZ + ROTZ * ((float)m_Counter / (float)m_aInfo[m_nType].aKeyInfo[m_nKey].nFrame)
		));
	}
}

//====================================================================
//モーションの更新処理
//====================================================================
void CMotion::Update(void)
{
	if (m_nBlendTime > 0)
	{//モーションブレンドの実装
		for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
		{
			//キーの情報から位置・向きを算出
			float POSX = m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].fPosX - m_aInfo[m_nBlendType].aKeyInfo[m_nBlendKey].aKey[nCntModel].fPosX;
			float POSY = m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].fPosY - m_aInfo[m_nBlendType].aKeyInfo[m_nBlendKey].aKey[nCntModel].fPosY;
			float POSZ = m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].fPosZ - m_aInfo[m_nBlendType].aKeyInfo[m_nBlendKey].aKey[nCntModel].fPosZ;
			float ROTX = m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].fRotX - m_aInfo[m_nBlendType].aKeyInfo[m_nBlendKey].aKey[nCntModel].fRotX;
			float ROTY = m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].fRotY - m_aInfo[m_nBlendType].aKeyInfo[m_nBlendKey].aKey[nCntModel].fRotY;
			float ROTZ = m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].fRotZ - m_aInfo[m_nBlendType].aKeyInfo[m_nBlendKey].aKey[nCntModel].fRotZ;

			//パーツの位置・向きを設定
			m_ppModel[nCntModel]->SetPos(D3DXVECTOR3
			(
				m_ppModel[nCntModel]->GetStartPos().x + m_aInfo[m_nBlendType].aKeyInfo[m_nBlendKey].aKey[nCntModel].fPosX + POSX * ((float)m_nBlendCounter / (float)m_nBlendTimeMax),
				m_ppModel[nCntModel]->GetStartPos().y + m_aInfo[m_nBlendType].aKeyInfo[m_nBlendKey].aKey[nCntModel].fPosY + POSY * ((float)m_nBlendCounter / (float)m_nBlendTimeMax),
				m_ppModel[nCntModel]->GetStartPos().z + m_aInfo[m_nBlendType].aKeyInfo[m_nBlendKey].aKey[nCntModel].fPosZ + POSZ * ((float)m_nBlendCounter / (float)m_nBlendTimeMax)
			));

			m_ppModel[nCntModel]->SetRot(D3DXVECTOR3
			(
				m_ppModel[nCntModel]->GetStartRot().x + m_aInfo[m_nBlendType].aKeyInfo[m_nBlendKey].aKey[nCntModel].fRotX + ROTX * ((float)m_nBlendCounter / (float)m_nBlendTimeMax),
				m_ppModel[nCntModel]->GetStartRot().y + m_aInfo[m_nBlendType].aKeyInfo[m_nBlendKey].aKey[nCntModel].fRotY + ROTY * ((float)m_nBlendCounter / (float)m_nBlendTimeMax),
				m_ppModel[nCntModel]->GetStartRot().z + m_aInfo[m_nBlendType].aKeyInfo[m_nBlendKey].aKey[nCntModel].fRotZ + ROTZ * ((float)m_nBlendCounter / (float)m_nBlendTimeMax)
			));
		}
		m_nBlendCounter += CManager::GetInstance()->GetGameSpeed();;
		m_nBlendTime -= CManager::GetInstance()->GetGameSpeed();
	}
	else
	{
		m_nNumKey = m_aInfo[m_nType].NumKey;

		//全モデル(パーツ)の更新
		for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
		{
			//キーの情報から位置・向きを算出
			float POSX = m_aInfo[m_nType].aKeyInfo[m_nNextKey].aKey[nCntModel].fPosX - m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].fPosX;
			float POSY = m_aInfo[m_nType].aKeyInfo[m_nNextKey].aKey[nCntModel].fPosY - m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].fPosY;
			float POSZ = m_aInfo[m_nType].aKeyInfo[m_nNextKey].aKey[nCntModel].fPosZ - m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].fPosZ;
			float ROTX = m_aInfo[m_nType].aKeyInfo[m_nNextKey].aKey[nCntModel].fRotX - m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].fRotX;
			float ROTY = m_aInfo[m_nType].aKeyInfo[m_nNextKey].aKey[nCntModel].fRotY - m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].fRotY;
			float ROTZ = m_aInfo[m_nType].aKeyInfo[m_nNextKey].aKey[nCntModel].fRotZ - m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].fRotZ;

			//パーツの位置・向きを設定
			m_ppModel[nCntModel]->SetPos(D3DXVECTOR3
			(
				m_ppModel[nCntModel]->GetStartPos().x + m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].fPosX + POSX * (m_Counter / ((float)m_aInfo[m_nType].aKeyInfo[m_nKey].nFrame)),
				m_ppModel[nCntModel]->GetStartPos().y + m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].fPosY + POSY * (m_Counter / ((float)m_aInfo[m_nType].aKeyInfo[m_nKey].nFrame)),
				m_ppModel[nCntModel]->GetStartPos().z + m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].fPosZ + POSZ * (m_Counter / ((float)m_aInfo[m_nType].aKeyInfo[m_nKey].nFrame))
			));																																												  
																																															  
			m_ppModel[nCntModel]->SetRot(D3DXVECTOR3																																		  
			(																																												  
				m_ppModel[nCntModel]->GetStartRot().x + m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].fRotX + ROTX * (m_Counter / ((float)m_aInfo[m_nType].aKeyInfo[m_nKey].nFrame)),
				m_ppModel[nCntModel]->GetStartRot().y + m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].fRotY + ROTY * (m_Counter / ((float)m_aInfo[m_nType].aKeyInfo[m_nKey].nFrame)),
				m_ppModel[nCntModel]->GetStartRot().z + m_aInfo[m_nType].aKeyInfo[m_nKey].aKey[nCntModel].fRotZ + ROTZ * (m_Counter / ((float)m_aInfo[m_nType].aKeyInfo[m_nKey].nFrame))
			));
		}

		m_Counter += CManager::GetInstance()->GetGameSpeed();
		if (m_Counter >= m_aInfo[m_nType].aKeyInfo[m_nKey].nFrame)
		{
			m_Counter = 0;
			m_nKey++;
			m_nNextKey++;

			if (m_nNextKey >= m_nNumKey)
			{
				if (m_aInfo[m_nType].bLoop == true)
				{
					m_nNextKey = 0;
				}
				else
				{
					m_nNextKey = 0;
				}
			}
			if (m_nKey >= m_nNumKey)
			{
				if (m_aInfo[m_nType].bLoop == true)
				{
					m_nKey = 0;
				}
				else
				{
					m_nKey = 0;
				}
			}
		}
	}
}

//====================================================================
//モデルのセット処理
//====================================================================
void CMotion::SetModel(CModel **ppModel, int nNumModel)
{
	m_ppModel = ppModel;
	m_nNumModel = nNumModel;
}

//====================================================================
//モーションのロード処理
//====================================================================
void CMotion::LoadData(const char *pFilename)
{
	FILE *pFile; //ファイルポインタを宣言

	//ファイルを開く
	pFile = fopen(pFilename, "r");

	if (pFile != NULL)
	{//ファイルが開けた場合

		int nCntModel = 0;
		int nCntParts = 0;
		int nCntMotion = 0;
		int nCntKeySet = 0;
		int nCntKey = 0;

		char aString[128] = {};				//ゴミ箱
		char aMessage[128] = {};		//スタートメッセージ
		char aBool[128] = {};				//bool変換用メッセージ

		// 読み込み開始-----------------------------------------------------
		while (1)
		{//「SCRIPT」を探す
			fscanf(pFile, "%s", &aMessage[0]);
			if (strcmp(&aMessage[0], "SCRIPT") == 0)
			{
				// モーション情報読み込み-----------------------------------------------------
				while (1)
				{//「MOTIONSET」を探す
					fscanf(pFile, "%s", &aMessage[0]);
					if (strcmp(&aMessage[0], "MOTIONSET") == 0)
					{
						while (1)
						{//各種変数を探す
							fscanf(pFile, "%s", &aMessage[0]);
							if (strcmp(&aMessage[0], "LOOP") == 0)
							{
								fscanf(pFile, "%s", &aString[0]);
								fscanf(pFile, "%s", &aBool[0]);	//ループするかどうかを設定
								m_aInfo[nCntMotion].bLoop = (strcmp(&aBool[0], "1") == 0 ? true : false);			//bool型の書き方
							}
							if (strcmp(&aMessage[0], "NUM_KEY") == 0)
							{
								fscanf(pFile, "%s", &aString[0]);
								fscanf(pFile, "%d", &m_aInfo[nCntMotion].NumKey);	//キーの総数を設定
								break;
							}
						}

						// キー情報読み込み-----------------------------------------------------
						while (1)
						{//「KEYSET」を探す
							fscanf(pFile, "%s", &aMessage[0]);
							if (strcmp(&aMessage[0], "KEYSET") == 0)
							{
								while (1)
								{//「FRAME」を探す
									fscanf(pFile, "%s", &aMessage[0]);
									if (strcmp(&aMessage[0], "FRAME") == 0)
									{
										fscanf(pFile, "%s", &aString[0]);
										fscanf(pFile, "%d", &m_aInfo[nCntMotion].aKeyInfo[nCntKeySet].nFrame);	//キーフレームを設定
										break;
									}
								}

								while (1)
								{//「KEY」を探す
									fscanf(pFile, "%s", &aMessage[0]);
									if (strcmp(&aMessage[0], "KEY") == 0)
									{
										while (1)
										{//各種変数を探す
											fscanf(pFile, "%s", &aMessage[0]);
											if (strcmp(&aMessage[0], "POS") == 0)
											{
												fscanf(pFile, "%s", &aString[0]);
												fscanf(pFile, "%f", &m_aInfo[nCntMotion].aKeyInfo[nCntKeySet].aKey[nCntKey].fPosX);	//位置を設定
												fscanf(pFile, "%f", &m_aInfo[nCntMotion].aKeyInfo[nCntKeySet].aKey[nCntKey].fPosY);	//位置を設定
												fscanf(pFile, "%f", &m_aInfo[nCntMotion].aKeyInfo[nCntKeySet].aKey[nCntKey].fPosZ);	//位置を設定
											}
											if (strcmp(&aMessage[0], "ROT") == 0)
											{
												fscanf(pFile, "%s", &aString[0]);
												fscanf(pFile, "%f", &m_aInfo[nCntMotion].aKeyInfo[nCntKeySet].aKey[nCntKey].fRotX);	//位置を設定
												fscanf(pFile, "%f", &m_aInfo[nCntMotion].aKeyInfo[nCntKeySet].aKey[nCntKey].fRotY);	//位置を設定
												fscanf(pFile, "%f", &m_aInfo[nCntMotion].aKeyInfo[nCntKeySet].aKey[nCntKey].fRotZ);	//位置を設定
												break;
											}
										}
									}
									if (strcmp(&aMessage[0], "END_KEY") == 0)
									{
										nCntKey++;
										if (nCntKey >= m_nNumModel)
										{
											break;
										}
									}
								}
							}
							if (strcmp(&aMessage[0], "END_KEYSET") == 0)
							{
								nCntKey = 0;
								nCntKeySet++;
								if (nCntKeySet >= m_aInfo[nCntMotion].NumKey)
								{
									break;
								}
							}
						}
					}
					if (strcmp(&aMessage[0], "END_MOTIONSET") == 0)
					{
						nCntKeySet = 0;
						nCntMotion++;
					}
					else if (strcmp(&aMessage[0], "END_SCRIPT") == 0)
					{
						break;
					}
				}
			}
			if (strcmp(&aMessage[0], "END_SCRIPT") == 0)
			{
				break;
			}
		}
		fclose(pFile);
	}
	else
	{//ファイルが開けなかった場合
		printf("***ファイルを開けませんでした***\n");
	}
}