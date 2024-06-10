//============================================
//
//	ボスの処理 [boss.cpp]
//	Author:sakamoto kai
//
//============================================
#include "boss.h"
#include "renderer.h"
#include "manager.h"
#include "game.h"
#include "texture.h"
#include "XModel.h"
#include "player.h"
#include "Particle.h"
#include "objGauge2D.h"
#include "number.h"
#include "input.h"
#include "CubeBlock.h"
#include "CubeDamage.h"
#include "CubeEffect.h"
#include "sound.h"
#include "CubeSpin.h"
#include "camera.h"
#include "fade.h"
#include "objmeshRing.h"
#include "object3D.h"
#include "enemy.h"
#include "effect.h"
#include "modelEffect.h"
#include "log.h"
#include "time.h"

//マクロ定義
#define BLOCK_WIGHT (300.0f)		//横幅
#define BLOCK_HEIGHT (300.0f)		//縦幅
#define WALL_UPEND (20.0f)			//上昇位置
#define BOSS_LIFE (2000.0f)			//ボスの体力
#define BOSS_LIFESPEED_PLUS (128.0f)//体力増の移動量
#define BOSS_LIFESPEED_DEL (64.0f)	//体力減の移動量
#define BULLET_INTERVAL_FORM1 (20)	//弾の発射感覚(進化前)
#define BULLET_INTERVAL_FORM2 (10)	//弾の発射感覚(進化後)
#define COLLISION_SIZE (D3DXVECTOR3(90.0f,90.0f,90.0f))		//当たり判定
#define BULLET_LIFE (600)			//弾の寿命
#define BLOCKRUN_TIME (600)			//ブロックランの時間
#define BLOCKRUN_SPEED (5.0f)		//ブロックランの速度
#define BLOCKWALL_HEIGHT (1100.0f)	//ブロックウォールの通る幅
#define BULLET_SPEED_FORM1 (0.01f)		//弾の速度(進化前)
#define BULLET_SPEED_FORM2 (0.015f)		//弾の速度(進化後)
#define BULLET_DAMAGE (50.0f)		//弾のダメージ量
#define RUSH_DAMAGE (90.0f)			//突進攻撃のダメージ
#define BLOCKRUN_DAMAGE (60.0f)		//ブロックランのダメージ
#define SPINPILLAR_DAMAGE (60.0f)	//回転柱のダメージ
#define COOLTIME_BULLET (240)		//弾攻撃のクールタイム
#define COOLTIME_RUSH (180)			//突進攻撃のクールタイム
#define COOLTIME_BLOCKRUN (300)		//ブロックランのクールタイム
#define COOLTIME_SPINPILLAR_FORM1 (30)	//回転柱のクールタイム
#define COOLTIME_SPINPILLAR_FORM2 (300)	//回転柱のクールタイム
#define COOLTIME_RAIN (240)			//雨攻撃のクールタイム
#define DEATH_EXPLOSION (32)			//死亡時の爆発キューブの総数(数値*6)
#define DEATH_EXPLOSION_SPEED (0.03f)	//死亡時の爆発キューブの速度

//====================================================================
//コンストラクタ
//====================================================================
CBoss::CBoss(int nPriority) : CObjectX(nPriority)
{
	SetSize(COLLISION_SIZE);
	m_nIdxXModel = NULL;			//マテリアルの数
	m_CollisionPos = INITVECTOR3;
	m_bCollision = false;
	m_MoveCount = 0.0f;
	m_fLife = BOSS_LIFE;
	m_fMoveLife = 0.0f;
	m_bDelLife = false;
	m_fLifeMax = m_fLife;
	m_State = STATE_NORMAL;
	m_Action = ACTION_EVENT;
	m_nStateCount = 0;
	m_ColorA = 1.0f;
	m_AttackCount = 0;
	m_AttackWave = 0;
	m_AttackCoolTime = 0;
	m_SpinCount = 0.0f;
	m_Scaling = 1.0f;
	m_nForm = 0;
	m_bRevivalColorSwitch = false;
	m_CubeSpin = nullptr;
	m_CubeSpinTitle = nullptr;
	m_nWarpOK = false;
	m_pRevivalFG = nullptr;
	m_fRevivalColorA = 1.0f;
	m_pFGDel = 0.05f;
	m_pShadow = nullptr;
	m_bDeathColorSwich = false;
	m_fDeathColor = 1.0f;
	m_fDeathExplojsionDis = 0.0f;
	m_nDebugText = 0;

	for (int nCnt = 0; nCnt < 5; nCnt++)
	{
		m_pLifeNumber[nCnt] = nullptr;
	}

	for (int nCnt = 0; nCnt < 9; nCnt++)
	{
		m_bBreak[nCnt] = false;
	}
}

//====================================================================
//デストラクタ
//====================================================================
CBoss::~CBoss()
{

}

//====================================================================
//生成処理
//====================================================================
CBoss* CBoss::Create(char* pModelName)
{
	CBoss* pBlock = NULL;

	if (pBlock == NULL)
	{
		//オブジェクト2Dの生成
		pBlock = new CBoss();
	}

	//オブジェクトの初期化処理
	if (FAILED(pBlock->Init(pModelName)))
	{//初期化処理が失敗した場合
		return NULL;
	}

	return pBlock;
}

//====================================================================
//初期化処理
//====================================================================
HRESULT CBoss::Init(char* pModelName)
{
	SetType(CObject::TYPE_BOSS);

	CObjectX::Init(pModelName);

	switch (CScene::GetMode())
	{
	case CScene::MODE_TITLE:
		if (m_CubeSpin == nullptr)
		{
			m_CubeSpin = CCubeSpin::Create(7);
			m_CubeSpin->SetPos(D3DXVECTOR3(500.0f, 300.0f, 1000.0f));
			m_CubeSpin->SetSpinDistance(150.0f);
			m_CubeSpin->SetSpinSpeedY(0.01f);
			m_CubeSpin->SetSpinSpeedX(0.01f);
		}
		if (m_CubeSpinTitle == nullptr)
		{
			m_CubeSpinTitle = CCubeSpin::Create(15);
			m_CubeSpinTitle->SetPos(D3DXVECTOR3(500.0f, 300.0f, 1000.0f));
			m_CubeSpinTitle->SetSpinDistance(250.0f);
			m_CubeSpinTitle->SetSpinSpeedY(-0.02f);
			m_CubeSpinTitle->SetSpinSpeedX(-0.02f);
		}
		break;

	case CScene::MODE_GAME:

		if (m_pRevivalFG == nullptr)
		{
			m_pRevivalFG = CObject2D::Create(6);
			m_pRevivalFG->SetPos(D3DXVECTOR3(640.0f, 360.0f, 0.0f));
			m_pRevivalFG->SetWidth(1280.0f);
			m_pRevivalFG->SetHeight(720.0f);
		}

		break;

	case CScene::MODE_RESULT:
		break;
	}


	return S_OK;
}

//====================================================================
//終了処理
//====================================================================
void CBoss::Uninit(void)
{
	CObjectX::Uninit();
}

//====================================================================
//更新処理
//====================================================================
void CBoss::Update(void)
{
	switch (CScene::GetMode())
	{
	case CScene::MODE_TITLE:
		TitleUpdate();
		break;

	case CScene::MODE_GAME:
		GameUpdate();
		break;

	case CScene::MODE_RESULT:
		break;
	}
}

//====================================================================
//タイトルでの更新処理
//====================================================================
void CBoss::TitleUpdate(void)
{
	D3DXVECTOR3 pos = GetPos();

	m_MoveCount += 0.04f;
	m_move.y = sinf(m_MoveCount) * 1.0f;

	if (m_CubeSpin != nullptr)
	{
		m_CubeSpin->SetPos(pos);
	}
	if (m_CubeSpinTitle != nullptr)
	{
		m_CubeSpinTitle->SetPos(pos);
	}

	//位置更新
	pos += m_move;

	SetPos(pos);

	//頂点情報の更新
	CObjectX::Update();
}

//====================================================================
//ゲームでの更新処理
//====================================================================
void CBoss::GameUpdate(void)
{
	D3DXVECTOR3 pos = GetPos();

	if (m_Action != ACTION_ATTACK  && m_Action != ACTION_EVENT)
	{
		m_MoveCount += 0.04f;

		m_move.y = sinf(m_MoveCount) * 2.0f;
	}

	switch (m_Action)
	{
	case CBoss::ACTION_NORMAL:
		if (m_AttackCoolTime > 0)
		{
			m_AttackCoolTime--;
		}
		else
		{
			AttackSelect();
		}
		break;

	case CBoss::ACTION_ATTACK:
		AttackUpdate(&pos);
		break;

	case CBoss::ACTION_WARP:
		//ワープ処理
		WarpUpdate(&pos);
		break;
	case CBoss::ACTION_REVIVAL:
		//蘇生処理
		AttackRevival(&pos);
		break;
	case CBoss::ACTION_DEATH:
		//死亡処理
		AttackDeath(&pos);
		break;

	case CBoss::ACTION_EVENT:
		break;

	default:
		break;
	}

	//影の当たり判定
	CollisionShadow();

#ifdef _DEBUG

	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_1) == true)
	{
		switch (m_nDebugText)
		{
		case 0:
			CLog::Create(CLog::TEXT_00);
			break;
		case 1:
			CLog::Create(CLog::TEXT_01);
			break;
		case 2:
			CLog::Create(CLog::TEXT_02);
			break;
		case 3:
			CLog::Create(CLog::TEXT_03);
			break;
		case 4:
			CLog::Create(CLog::TEXT_04);
			break;
		case 5:
			CLog::Create(CLog::TEXT_05);
			break;
		case 6:
			CLog::Create(CLog::TEXT_06);
			break;
		case 7:
			CLog::Create(CLog::TEXT_07);
			break;
		case 8:
			CLog::Create(CLog::TEXT_08);
			break;
		case 9:
			CLog::Create(CLog::TEXT_09);
			break;
		case 10:
			CLog::Create(CLog::TEXT_10);
			break;
		case 11:
			CLog::Create(CLog::TEXT_11);
			break;
		default:
			m_nDebugText = -1;
			break;
		}

		m_nDebugText++;
	}
	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_2) == true)
	{
		Warp(ATTACK_BULLET);
	}
	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_3) == true)
	{
		Warp(ATTACK_RUSH);
	}
	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_4) == true)
	{
		Warp(ATTACK_BLOCKRUN);
	}
	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_5) == true)
	{
		Warp(ATTACK_SPINPILLAR);
	}
	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_6) == true)
	{
		Warp(ATTACK_RAIN);
	}
	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_7) == true)
	{
		Warp(ATTACK_SPAWNENEMY);
	}
	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_8) == true)
	{
		Warp(ATTACK_2D_BLOCKWALL);
	}
	if (CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_9) == true)
	{
		Warp(ATTACK_DOWN_BREAK);
	}

#endif // _DEBUG

	if (m_nForm == 1)
	{
		m_SpinCount += 0.075f;

		if (m_CubeSpin != nullptr)
		{
			m_CubeSpin->SetPos(D3DXVECTOR3(pos.x, pos.y + 100.0f, pos.z));
		}
	}

	//位置更新
	pos += m_move;

	SetPos(pos);
	SetRot(m_rot);

	SetScaling(D3DXVECTOR3(m_Scaling, m_Scaling, m_Scaling));

	//ライフゲージの位置管理
	if (m_pLifeGauge != nullptr)
	{
		if (m_bDelLife == false)
		{
			m_fMoveLife += BOSS_LIFESPEED_PLUS;

			if (m_fMoveLife >= m_fLife)
			{
				m_fMoveLife = m_fLife;
				m_bDelLife = true;
			}
		}
		else
		{
			m_fMoveLife -= BOSS_LIFESPEED_DEL;

			if (m_fMoveLife <= m_fLife)
			{
				m_fMoveLife = m_fLife;
			}
		}

		m_pLifeGauge->SetGaugeWidth(m_fLifeMax, m_fMoveLife);
	}

	if (m_fRevivalColorA > 0.0f)
	{
		//前面ポリゴンを透明にしていく
		m_fRevivalColorA -= m_pFGDel;
		if (m_fRevivalColorA < 0.0f)
		{
			m_fRevivalColorA = 0.0f;
		}
		
		//前面ポリゴンの管理
		m_pRevivalFG->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fRevivalColorA));
	}

	//状態管理
	StateManager();

	//頂点情報の更新
	CObjectX::Update();
}

//====================================================================
//描画処理
//====================================================================
void CBoss::Draw(void)
{
	if (m_Action == ACTION_WARP)
	{
		CObjectX::Draw(m_ColorA);
	}
	else
	{
		CObjectX::Draw();
	}
}

//====================================================================
//状態管理
//====================================================================
void CBoss::LifeGaugeCreate(void)
{
	if (m_pLifeGaugeBG == nullptr)
	{
		m_pLifeGaugeBG = CObjGauge2D::Create();
		m_pLifeGaugeBG->SetPos(D3DXVECTOR3(240.0f, 20.0f, 0.0f));
		m_pLifeGaugeBG->SetWidth(800.0f);
		m_pLifeGaugeBG->SetHeight(10.0f);
		m_pLifeGaugeBG->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f));
		m_pLifeGaugeBG->SetGaugeWidth(m_fLifeMax, m_fLifeMax);
	}

	if (m_pLifeGauge == nullptr)
	{
		m_pLifeGauge = CObjGauge2D::Create();
		m_pLifeGauge->SetPos(D3DXVECTOR3(240.0f, 20.0f, 0.0f));
		m_pLifeGauge->SetWidth(800.0f);
		m_pLifeGauge->SetHeight(10.0f);
		m_pLifeGauge->SetColor(D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));
		m_pLifeGauge->SetGaugeWidth(m_fLifeMax, m_fMoveLife);
	}

	for (int nCnt = 0; nCnt < 5; nCnt++)
	{
		m_pLifeNumber[nCnt] = CNumber::Create();
		m_pLifeNumber[nCnt]->SetPos(D3DXVECTOR3(1000.0f + nCnt * 15.0f, 50.0f, 0.0f));
		m_pLifeNumber[nCnt]->SetWidth(20.0f);
		m_pLifeNumber[nCnt]->SetHeight(20.0f);
	}

	SetLifeUI();

	//影の生成
	if (m_pShadow == nullptr)
	{
		m_pShadow = CObject3D::Create();
		m_pShadow->SetPos(GetPos());
		m_pShadow->SetRot(D3DXVECTOR3(D3DX_PI * 0.5f, m_rot.y, 0.0f));
		m_pShadow->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		m_pShadow->SetTexture("data\\TEXTURE\\Effect000.jpg");
		m_pShadow->SetAddDorw(true);
	}
}

//====================================================================
//状態管理
//====================================================================
void CBoss::StateManager(void)
{
	switch (m_State)
	{
	case STATE_NORMAL:
		break;

	case STATE_DAMAGE:
		if (m_nStateCount <= 0)
		{
			m_State = STATE_NORMAL;
			m_nStateCount = 0;
			SetDefColor();
		}
		break;

	case STATE_DEATH:
		if (m_nStateCount <= 0)
		{
			m_nStateCount = 0;
		}
		break;
	case STATE_INVINCIBLE:
		break;
	case ACTION_EVENT:
		break;
	}

	if (m_nStateCount > 0)
	{
		m_nStateCount--;
	}
}

//====================================================================
//ワープ処理
//====================================================================
void CBoss::WarpUpdate(D3DXVECTOR3 *pos)
{
	if (m_nWarpOK == false)
	{
		if (m_nForm == 0)
		{
			m_ColorA -= 0.05f;
		}
		else if (m_nForm == 1)
		{
			m_ColorA -= 0.1f;
		}

		if (m_ColorA <= 0)
		{
			m_ColorA = 0;
			*pos = m_WarpPos;
			m_nWarpOK = true;
		}
	}
	else
	{
		if (m_nForm == 0)
		{
			m_ColorA += 0.025f;
		}
		else if (m_nForm == 1)
		{
			m_ColorA += 0.1f;
		}

		if (m_ColorA >= 1.0f)
		{
			m_ColorA = 1.0f;
			m_nWarpOK = false;
			SetDefColor();
			
			if (m_AttackPattern == ATTACK_NOT)
			{
				m_Action = ACTION_NORMAL;
			}
			else if (m_AttackPattern == ATTACK_REVIVAL)
			{
				m_Action = ACTION_REVIVAL;
			}
			else
			{
				m_Action = ACTION_ATTACK;

				//ゲームのSEを再生する
				CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_WARNING);
			}
		}
	}
}

//====================================================================
//ワープ位置指定処理
//====================================================================
void CBoss::Warp(ATTACK Pattern)
{
	if (m_AttackPatternOld == Pattern)
	{
		AttackSelect();
	}
	else
	{
		m_AttackPattern = Pattern;
		m_AttackPatternOld = Pattern;
		m_Action = ACTION_WARP;
		m_AttackCount = 0;
		m_AttackWave = 0;
		CPlayer* pPlayer = CGame::GetPlayer();
		int Rand;

		switch (m_AttackPattern)
		{
		case ATTACK_NOT:

			m_WarpPos.x = (float)(rand() % 1001) - 500.0f;
			m_WarpPos.y = (float)(rand() % 101) + 200.0f;
			m_WarpPos.z = (float)(rand() % 1001) - 500.0f;
			m_AttackCoolTime = 5;
			break;

		case ATTACK_BULLET:

			m_rot.y = 0.0f;
			m_WarpPos.x = sinf(m_rot.y) * 500.0f;
			m_WarpPos.y = 400.0f;
			m_WarpPos.z = cosf(m_rot.y) * 500.0f;

			break;

		case ATTACK_RUSH:

			Rand = rand() % 4;
			m_WarpPos.y = 400.0f;
			m_move = INITVECTOR3;

			switch (Rand)
			{
			case 0:
				m_rot.y = 0.0f;
				m_WarpPos.x = pPlayer->GetPos().x;
				m_WarpPos.z = 600.0f;
				break;
			case 1:
				m_rot.y = D3DX_PI;
				m_WarpPos.x = pPlayer->GetPos().x;
				m_WarpPos.z = -600.0f;
				break;
			case 2:
				m_rot.y = D3DX_PI * 0.5f;
				m_WarpPos.x = 600.0f;
				m_WarpPos.z = pPlayer->GetPos().z;
				break;
			case 3:
				m_rot.y = D3DX_PI * -0.5f;
				m_WarpPos.x = -600.0f;
				m_WarpPos.z = pPlayer->GetPos().z;
				break;
			}
			break;

		case ATTACK_BLOCKRUN:

			m_move = INITVECTOR3;

			m_rot.y = D3DX_PI;
			m_WarpPos.x = 0.0f;
			m_WarpPos.y = 550.0f;
			m_WarpPos.z = -500.0f;
			break;

		case ATTACK_SPINPILLAR:

			m_WarpPos.x = (float)(rand() % 1001) - 500.0f;
			m_WarpPos.y = 250.0f;
			m_WarpPos.z = (float)(rand() % 1001) - 500.0f;

			break;

		case ATTACK_RAIN:

			m_WarpPos.x = 0.0f;
			m_WarpPos.y = 500.0f;
			m_WarpPos.z = 0.0f;

			break;

		case ATTACK_SPAWNENEMY:

			m_rot.y = 0.0f;

			m_WarpPos.x = 0.0f;
			m_WarpPos.y = 500.0f;
			m_WarpPos.z = 500.0f;

			break;

		case ATTACK_2D_BLOCKWALL:

			m_rot.y = D3DX_PI * 0.5f;

			m_WarpPos.x = 500.0f;
			m_WarpPos.y = 400.0f;
			m_WarpPos.z = 0.0f;

			break;
		}
	}
}

//====================================================================
//攻撃更新処理
//====================================================================
void CBoss::AttackUpdate(D3DXVECTOR3* pos)
{
	SetMatColor(D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));

	switch (m_AttackPattern)
	{
	case ATTACK_BULLET:
		AttackBullet(pos);
		break;
	case ATTACK_RUSH:
		AttackRush(pos);
		break;
	case ATTACK_BLOCKRUN:
		AttackBlockRun(pos);
		break;
	case ATTACK_SPINPILLAR:
		AttackSpinPillar(pos);
		break;
	case ATTACK_RAIN:
		AttackRain(pos);
		break;
	case ATTACK_SPAWNENEMY:
		AttackSpawnEnemy(pos);
		break;
	case ATTACK_2D_BLOCKWALL:
		AttackBlockWall(pos);
		break;
	case ATTACK_DOWN_BREAK:
		AttackMapBreak(pos);
		break;
	}
}

//====================================================================
//攻撃選択処理
//====================================================================
void CBoss::AttackSelect(void)
{
	int RandAttack = rand() % 151;

	if (m_nForm == 0)
	{
		if (RandAttack <= 10)
		{
			Warp(ATTACK_BULLET);
		}
		else if (RandAttack <= 30)
		{
			Warp(ATTACK_RUSH);
		}
		else if (RandAttack <= 40)
		{
			Warp(ATTACK_BLOCKRUN);
		}
		else if (RandAttack <= 55)
		{
			Warp(ATTACK_SPAWNENEMY);
		}
		else if (RandAttack <= 85)
		{
			Warp(ATTACK_SPINPILLAR);
		}
		else if (RandAttack <= 100)
		{
			Warp(ATTACK_RAIN);
		}
		else if (RandAttack <= 150)
		{
			Warp(ATTACK_NOT);
		}
	}
	else
	{
		if (CCubeDamage::GetExplosionNum() >= 1 && CCubeDamage::GetExplosionNum() <= 16)
		{
			//柱攻撃だけは三回連続で選択するように設定
			Warp(ATTACK_SPINPILLAR);
		}
		else
		{
			if (RandAttack <= 10)
			{
				Warp(ATTACK_BULLET);
			}
			else if (RandAttack <= 20)
			{
				Warp(ATTACK_RUSH);
			}
			else if (RandAttack <= 40)
			{
				Warp(ATTACK_SPINPILLAR);
			}
			else if (RandAttack <= 60)
			{
				Warp(ATTACK_SPAWNENEMY);
			}
			else if (RandAttack <= 80)
			{
				Warp(ATTACK_2D_BLOCKWALL);
			}
			else if (RandAttack <= 100)
			{
				Warp(ATTACK_DOWN_BREAK);
			}
			else if (RandAttack <= 150)
			{
				Warp(ATTACK_NOT);
			}
		}
	}
}

//====================================================================
//攻撃(射撃)
//====================================================================
void CBoss::AttackBullet(D3DXVECTOR3* pos)
{
	CPlayer *pPlayer = CGame::GetPlayer();

	if (m_rot.y < D3DX_PI * 2.0f)
	{
		m_rot.y += 0.015f;

		pos->x = sinf(m_rot.y) * 500.0f;
		pos->z = cosf(m_rot.y) * 500.0f;

		m_MoveCount += 0.04f;
		m_move.y = sinf(m_MoveCount) * 2.0f;

		m_AttackCount++;
		if (m_nForm == 0)
		{
			if (m_AttackCount % BULLET_INTERVAL_FORM1 == 0)
			{
				//ゲームのSEを再生する
				CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_BOSS_BULLET);

				for (int nCnt = 0; nCnt < 8; nCnt++)
				{
					CCubeDamage* pCubeDamage = CCubeDamage::Create();
					pCubeDamage->SetPos(*pos);
					switch (nCnt)
					{
					case 0:
						pCubeDamage->SetMove(D3DXVECTOR3(
							(pPlayer->GetPos().x - pos->x) * BULLET_SPEED_FORM1,
							(pPlayer->GetPos().y - pos->y) * BULLET_SPEED_FORM1,
							(pPlayer->GetPos().z - pos->z) * BULLET_SPEED_FORM1));
						break;

					case 1:
						pCubeDamage->SetMove(D3DXVECTOR3(
							-(pPlayer->GetPos().x - pos->x) * BULLET_SPEED_FORM1,
							(pPlayer->GetPos().y - pos->y) * BULLET_SPEED_FORM1,
							(pPlayer->GetPos().z - pos->z) * BULLET_SPEED_FORM1));
						break;

					case 2:
						pCubeDamage->SetMove(D3DXVECTOR3(
							(pPlayer->GetPos().x - pos->x) * BULLET_SPEED_FORM1,
							-(pPlayer->GetPos().y - pos->y) * BULLET_SPEED_FORM1,
							(pPlayer->GetPos().z - pos->z) * BULLET_SPEED_FORM1));
						break;

					case 3:
						pCubeDamage->SetMove(D3DXVECTOR3(
							(pPlayer->GetPos().x - pos->x) * BULLET_SPEED_FORM1,
							(pPlayer->GetPos().y - pos->y) * BULLET_SPEED_FORM1,
							-(pPlayer->GetPos().z - pos->z) * BULLET_SPEED_FORM1));
						break;
					case 4:
						pCubeDamage->SetMove(D3DXVECTOR3(
							-(pPlayer->GetPos().x - pos->x) * BULLET_SPEED_FORM1,
							-(pPlayer->GetPos().y - pos->y) * BULLET_SPEED_FORM1,
							(pPlayer->GetPos().z - pos->z) * BULLET_SPEED_FORM1));
						break;

					case 5:
						pCubeDamage->SetMove(D3DXVECTOR3(
							(pPlayer->GetPos().x - pos->x) * BULLET_SPEED_FORM1,
							-(pPlayer->GetPos().y - pos->y) * BULLET_SPEED_FORM1,
							-(pPlayer->GetPos().z - pos->z) * BULLET_SPEED_FORM1));
						break;

					case 6:
						pCubeDamage->SetMove(D3DXVECTOR3(
							-(pPlayer->GetPos().x - pos->x) * BULLET_SPEED_FORM1,
							(pPlayer->GetPos().y - pos->y) * BULLET_SPEED_FORM1,
							-(pPlayer->GetPos().z - pos->z) * BULLET_SPEED_FORM1));
						break;

					case 7:
						pCubeDamage->SetMove(D3DXVECTOR3(
							-(pPlayer->GetPos().x - pos->x) * BULLET_SPEED_FORM1,
							-(pPlayer->GetPos().y - pos->y) * BULLET_SPEED_FORM1,
							-(pPlayer->GetPos().z - pos->z) * BULLET_SPEED_FORM1));
						break;
					}
					pCubeDamage->SetCubeType(CCubeDamage::CUBETYPE_BREAK);
					pCubeDamage->SetDamage(BULLET_DAMAGE);
					pCubeDamage->SetLife(BULLET_LIFE);
				}
			}
		}
		else if (m_nForm == 1)
		{
			if (m_AttackCount % BULLET_INTERVAL_FORM2 == 0)
			{
				//ゲームのSEを再生する
				CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_BOSS_BULLET);

				for (int nCnt = 0; nCnt < 8; nCnt++)
				{
					CCubeDamage* pCubeDamage = CCubeDamage::Create();
					pCubeDamage->SetPos(*pos);
					switch (nCnt)
					{
					case 0:
						pCubeDamage->SetMove(D3DXVECTOR3(
							(pPlayer->GetPos().x - pos->x) * BULLET_SPEED_FORM2,
							(pPlayer->GetPos().y - pos->y) * BULLET_SPEED_FORM2,
							(pPlayer->GetPos().z - pos->z) * BULLET_SPEED_FORM2));
						break;

					case 1:
						pCubeDamage->SetMove(D3DXVECTOR3(
							-(pPlayer->GetPos().x - pos->x) * BULLET_SPEED_FORM2,
							(pPlayer->GetPos().y - pos->y) * BULLET_SPEED_FORM2,
							(pPlayer->GetPos().z - pos->z) * BULLET_SPEED_FORM2));
						break;

					case 2:
						pCubeDamage->SetMove(D3DXVECTOR3(
							(pPlayer->GetPos().x - pos->x) * BULLET_SPEED_FORM2,
							-(pPlayer->GetPos().y - pos->y) * BULLET_SPEED_FORM2,
							(pPlayer->GetPos().z - pos->z) * BULLET_SPEED_FORM2));
						break;

					case 3:
						pCubeDamage->SetMove(D3DXVECTOR3(
							(pPlayer->GetPos().x - pos->x) * BULLET_SPEED_FORM2,
							(pPlayer->GetPos().y - pos->y) * BULLET_SPEED_FORM2,
							-(pPlayer->GetPos().z - pos->z) * BULLET_SPEED_FORM2));
						break;
					case 4:
						pCubeDamage->SetMove(D3DXVECTOR3(
							-(pPlayer->GetPos().x - pos->x) * BULLET_SPEED_FORM2,
							-(pPlayer->GetPos().y - pos->y) * BULLET_SPEED_FORM2,
							(pPlayer->GetPos().z - pos->z) * BULLET_SPEED_FORM2));
						break;

					case 5:
						pCubeDamage->SetMove(D3DXVECTOR3(
							(pPlayer->GetPos().x - pos->x) * BULLET_SPEED_FORM2,
							-(pPlayer->GetPos().y - pos->y) * BULLET_SPEED_FORM2,
							-(pPlayer->GetPos().z - pos->z) * BULLET_SPEED_FORM2));
						break;

					case 6:
						pCubeDamage->SetMove(D3DXVECTOR3(
							-(pPlayer->GetPos().x - pos->x) * BULLET_SPEED_FORM2,
							(pPlayer->GetPos().y - pos->y) * BULLET_SPEED_FORM2,
							-(pPlayer->GetPos().z - pos->z) * BULLET_SPEED_FORM2));
						break;

					case 7:
						pCubeDamage->SetMove(D3DXVECTOR3(
							-(pPlayer->GetPos().x - pos->x) * BULLET_SPEED_FORM2,
							-(pPlayer->GetPos().y - pos->y) * BULLET_SPEED_FORM2,
							-(pPlayer->GetPos().z - pos->z) * BULLET_SPEED_FORM2));
						break;
					}
					pCubeDamage->SetCubeType(CCubeDamage::CUBETYPE_BREAK);
					pCubeDamage->SetDamage(BULLET_DAMAGE);
					pCubeDamage->SetLife(BULLET_LIFE);
				}
			}
		}

	}
	else
	{
		Warp(ATTACK_NOT);
		SetDefColor();
		m_AttackCoolTime = COOLTIME_BULLET;
	}
}

//====================================================================
//攻撃(突進)
//====================================================================
void CBoss::AttackRush(D3DXVECTOR3* pos)
{
	CPlayer* pPlayer = CGame::GetPlayer();
	CCubeDamage* pCubeDamage = nullptr;

	switch (m_AttackWave)
	{
	case 0:
		//ゲームのSEを再生する
		CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_BOSS_RUSH);

		m_move.x = -sinf(m_rot.y) * 1.0f;
		m_move.y = -10.0f;
		m_move.z = -cosf(m_rot.y) * 1.0f;

		if (pos->y < pPlayer->GetPos().y)
		{
			m_AttackWave++;
		}

		break;

	case 1:
		m_AttackCount++;

		m_move.x = -sinf(m_rot.y) * 15.0f;
		m_move.y = 0.0f;
		m_move.z = -cosf(m_rot.y) * 15.0f;

		pCubeDamage = CCubeDamage::Create();
		pCubeDamage->SetPos(D3DXVECTOR3(
			pos->x - sinf(m_rot.y) * 100.0f,
			pos->y + 10.0f,
			pos->z - cosf(m_rot.y) * 100.0f));
		pCubeDamage->SetMove(D3DXVECTOR3(
			sinf(m_rot.y + D3DX_PI * 0.5f) * 15.0f,
			0.0f,
			cosf(m_rot.y + D3DX_PI * 0.5f) * 15.0f));
		if (m_nForm == 0)
		{
			pCubeDamage->SetSize(D3DXVECTOR3(10.0f, 50.0f, 10.0f));
			pCubeDamage->SetLife(20);
			pCubeDamage->SetCubeType(CCubeDamage::CUBETYPE_NORMAL);
		}
		else if (m_nForm == 1)
		{
			pCubeDamage->SetSize(D3DXVECTOR3(10.0f, 200.0f + sinf(m_AttackCount * 0.2f) * 100.0f, 10.0f));
			pCubeDamage->SetLife(50);
			pCubeDamage->SetCubeType(CCubeDamage::CUBETYPE_EXPLOSION);
			pCubeDamage->SetExplosionCount(30);
		}
		pCubeDamage->SetDamage(RUSH_DAMAGE);

		pCubeDamage = CCubeDamage::Create();
		pCubeDamage->SetPos(D3DXVECTOR3(
			pos->x - sinf(m_rot.y) * 100.0f,
			pos->y + 10.0f,
			pos->z - cosf(m_rot.y) * 100.0f));
		pCubeDamage->SetMove(D3DXVECTOR3(
			sinf(m_rot.y + D3DX_PI * -0.5f) * 15.0f,
			0.0f,
			cosf(m_rot.y + D3DX_PI * -0.5f) * 15.0f));
		if (m_nForm == 0)
		{
			pCubeDamage->SetSize(D3DXVECTOR3(10.0f, 50.0f, 10.0f));
			pCubeDamage->SetLife(20);
			pCubeDamage->SetCubeType(CCubeDamage::CUBETYPE_NORMAL);
		}
		else if (m_nForm == 1)
		{
			pCubeDamage->SetSize(D3DXVECTOR3(10.0f, 200.0f + sinf(m_AttackCount * 0.2f) * 100.0f, 10.0f));
			pCubeDamage->SetLife(100);
			pCubeDamage->SetCubeType(CCubeDamage::CUBETYPE_EXPLOSION);
			pCubeDamage->SetExplosionCount(30);
		}
		pCubeDamage->SetDamage(RUSH_DAMAGE);

		if (pos->x > 600.0f ||
			pos->x < -600.0f ||
			pos->z > 600.0f ||
			pos->z < -600.0f)
		{
			m_AttackWave++;
		}

		break;

	case 2:
		m_move.x = -sinf(m_rot.y) * 2.0f;
		m_move.y = 10.0f;
		m_move.z = -cosf(m_rot.y) * 2.0f;

		if (pos->y > 400.0f)
		{
			m_AttackWave++;
		}

		break;

	default:
		m_move = INITVECTOR3;
		Warp(ATTACK_NOT);
		SetDefColor();
		m_AttackCoolTime = COOLTIME_RUSH;
		break;
	}
}

//====================================================================
//攻撃(ブロックラン)
//====================================================================
void CBoss::AttackBlockRun(D3DXVECTOR3* pos)
{
	CPlayer* pPlayer = CGame::GetPlayer();
	CCubeBlock* pCubeBlock = nullptr;
	CCubeDamage* pCubeDamage = nullptr;
	int nRand = rand() % 4;

	m_MoveCount += 0.04f;
	m_move.y = sinf(m_MoveCount) * 2.0f;

	switch (m_AttackWave)
	{
	case 0:

		pCubeBlock = CCubeBlock::Create();
		pCubeBlock->SetPos(INITVECTOR3);
		pCubeBlock->SetSize(D3DXVECTOR3(500.0f, 10.0f, 500.0f));
		pCubeBlock->SetMove(D3DXVECTOR3(0.0f, 10.0f, 0.0f));
		pCubeBlock->SetUninitPos(D3DXVECTOR3(pCubeBlock->GetPos().x, 500.0f, pCubeBlock->GetPos().z));
		pCubeBlock->SetBoolLife(true);
		pCubeBlock->SetPlusMove(true);

		m_AttackWave++;
		m_AttackCount++;

		break;

	case 1:

		m_AttackCount++;

		if (m_AttackCount % 60 == 0)
		{
			pCubeBlock = CCubeBlock::Create();
			pCubeBlock->SetPos(D3DXVECTOR3(0.0f, 200.0f, -0.0f));
			pCubeBlock->SetSize(D3DXVECTOR3(500.0f, 50.0f, 500.0f));
			pCubeBlock->SetMove(D3DXVECTOR3(0.0f, 0.0f, BLOCKRUN_SPEED));
			pCubeBlock->SetUninitPos(D3DXVECTOR3(pCubeBlock->GetPos().x, pCubeBlock->GetPos().y, 1000.0f));
			pCubeBlock->SetBoolLife(true);
			pCubeBlock->SetPlusMove(true);
			pCubeBlock->SetColorA(1.0f);
			pCubeBlock->SetDel(0.005f);

			pCubeDamage = CCubeDamage::Create();
			pCubeDamage->SetPos(D3DXVECTOR3(0.0f, 120.0f, 0.0f));
			pCubeDamage->SetSize(D3DXVECTOR3(500.0f, 10.0f, 500.0f));
			pCubeDamage->SetLife(BLOCKRUN_TIME);
			pCubeDamage->SetCubeType(CCubeDamage::CUBETYPE_NORMAL);
			pCubeDamage->SetDamage(BLOCKRUN_DAMAGE);

			CLog::Create(CLog::TEXT_06);

			m_AttackWave++;
		}

		break;

	case 2:

		m_AttackCount++;

		if (m_AttackCount % 30 == 0)
		{
			pCubeBlock = CCubeBlock::Create();

			switch (nRand)
			{
			case 0:
				pCubeBlock->SetPos(D3DXVECTOR3(-125.0f, 200.0f, -500.0f));
				break;
			case 1:
				pCubeBlock->SetPos(D3DXVECTOR3(-375.0f, 200.0f, -500.0f));
				break;
			case 2:
				pCubeBlock->SetPos(D3DXVECTOR3(125.0f, 200.0f, -500.0f));
				break;
			case 3:
				pCubeBlock->SetPos(D3DXVECTOR3(375.0f, 200.0f, -500.0f));
				break;
			}
			if (m_nForm == 0)
			{
				pCubeBlock->SetSize(D3DXVECTOR3(125.0f, 50.0f, 125.0f));
			}
			else if (m_nForm == 1)
			{
				pCubeBlock->SetSize(D3DXVECTOR3(75.0f, 50.0f, 75.0f));
			}
			pCubeBlock->SetMove(D3DXVECTOR3(0.0f, 0.0f, BLOCKRUN_SPEED));
			pCubeBlock->SetUninitPos(D3DXVECTOR3(pCubeBlock->GetPos().x, pCubeBlock->GetPos().y, 625.0f));
			pCubeBlock->SetBoolLife(true);
			pCubeBlock->SetPlusMove(true);
			pCubeBlock->SetColorA(1.0f);
			pCubeBlock->SetDel(0.0040f);
		}

		if (m_AttackCount > BLOCKRUN_TIME)
		{
			m_AttackWave++;
		}

		break;

	default:
		m_move = INITVECTOR3;
		Warp(ATTACK_NOT);
		SetDefColor();
		m_AttackCoolTime = COOLTIME_BLOCKRUN;
		break;
	}
}

//====================================================================
//攻撃(柱の回転)
//====================================================================
void CBoss::AttackSpinPillar(D3DXVECTOR3* pos)
{
	CPlayer* pPlayer = CGame::GetPlayer();
	CCubeBlock* pCubeBlock = nullptr;
	CCubeDamage* pCubeDamage = nullptr;
	m_move = INITVECTOR3;

	switch (m_AttackWave)
	{
	case 0:

		if (m_AttackCount % 5 == 0)
		{
			m_Scaling += 0.02f;
		}
		else
		{
			m_Scaling -= 0.02f;
		}

		if (m_Scaling <= 0.5f)
		{
			m_AttackWave++;
			m_AttackCount = 0;
		}

		m_AttackCount++;
		break;

	case 1:
		m_Scaling += 0.1f;

		if (m_Scaling >= 1.0f)
		{
			m_Scaling = 1.0f;
			m_AttackWave++;

			//ゲームのSEを再生する
			CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_BOSS_BULLET);

			if (m_nForm == 0)
			{
				for (int nCnt = 0; nCnt < 4; nCnt++)
				{
					CCubeDamage* pCubeDamage = CCubeDamage::Create();
					pCubeDamage->SetUseSpin(true);
					pCubeDamage->SetSpinPos(*pos);
					pCubeDamage->SetSpinCount(D3DXVECTOR3(m_SpinCount + nCnt * D3DX_PI * 0.5f, m_SpinCount + nCnt * D3DX_PI * 0.5f, m_SpinCount + nCnt * D3DX_PI * 0.5f));
					pCubeDamage->SetSpinSpeedY(0.015f);
					pCubeDamage->SetSpinDistance(100.0f);
					pCubeDamage->SetCubeType(CCubeDamage::CUBETYPE_BREAK);
					pCubeDamage->SetLife(900);
					pCubeDamage->SetSpinDisMove(3.0f);
					pCubeDamage->SetSize(D3DXVECTOR3(20.0f, 5000.0f, 20.0f));
					pCubeDamage->SetDamage(SPINPILLAR_DAMAGE);
				}
			}
			else if (m_nForm == 1)
			{
				for (int nCnt = 0; nCnt < 8; nCnt++)
				{
					CCubeDamage* pCubeDamage = CCubeDamage::Create();
					pCubeDamage->SetUseSpin(true);
					pCubeDamage->SetSpinPos(*pos);
					pCubeDamage->SetSpinCount(D3DXVECTOR3(m_SpinCount + nCnt * D3DX_PI * 0.25f, m_SpinCount + nCnt * D3DX_PI * 0.25f, m_SpinCount + nCnt * D3DX_PI * 0.25f));
					pCubeDamage->SetSpinSpeedY(0.015f);
					pCubeDamage->SetSpinDistance(10.0f);
					pCubeDamage->SetCubeType(CCubeDamage::CUBETYPE_EXPLOSION);
					pCubeDamage->SetExplosionCount(150);
					pCubeDamage->SetLife(900);
					pCubeDamage->SetSpinDisMove(3.0f);
					pCubeDamage->SetSize(D3DXVECTOR3(20.0f, 5000.0f, 20.0f));
					pCubeDamage->SetDamage(SPINPILLAR_DAMAGE);
				}
			}
		}
		break;

	default:
		m_move = INITVECTOR3;
		Warp(ATTACK_NOT);
		SetDefColor();

		if (CCubeDamage::GetExplosionNum() > 16)
		{
			m_AttackCoolTime = COOLTIME_SPINPILLAR_FORM2;
		}
		else
		{
			m_AttackCoolTime = COOLTIME_SPINPILLAR_FORM1;
		}
		break;
	}
}

//====================================================================
//攻撃(雨)
//====================================================================
void CBoss::AttackRain(D3DXVECTOR3* pos)
{
	CPlayer* pPlayer = CGame::GetPlayer();
	CCubeDamage* pCubeDamage = nullptr;

	switch (m_AttackWave)
	{
	case 0:

		m_move = D3DXVECTOR3(0.0f, -20.0f, 0.0f);

		if (pos->y < 200.0f)
		{
			m_AttackWave++;

			CLog::Create(CLog::TEXT_08);

			//カメラを振動させる
			CManager::GetInstance()->GetCamera()->SetBib(true);
		}
		break;

	case 1:

		//ゲームのSEを再生する
		CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_BOSS_RAIN);

		m_move = D3DXVECTOR3(0.0f, 20.0f, 0.0f);

		if (pos->y >= 300.0f)
		{
			m_AttackWave++;
		}
		break;

	case 2:

		m_move = INITVECTOR3;

		if (m_AttackCount % 3 == 0)
		{
			CCubeDamage* pCubeDamage = CCubeDamage::Create();
			pCubeDamage->SetPos(D3DXVECTOR3(
			(float)(rand() % 1001) - 500.0f,
			1000.0f,
			(float)(rand() % 1001) - 500.0f
			));
			pCubeDamage->SetMove(D3DXVECTOR3(0.0f, -5.0f - (float)(rand() % 4), 0.0f));
			pCubeDamage->SetCubeType(CCubeDamage::CUBETYPE_BREAK);
			pCubeDamage->SetDamage(BULLET_DAMAGE);
			pCubeDamage->SetLife(BULLET_LIFE);
		}

		if (m_AttackCount > 30)
		{
			//カメラの振動を止める
			CManager::GetInstance()->GetCamera()->SetBib(false);
		}

		if (m_AttackCount > 300)
		{
			m_AttackWave++;
		}

		m_AttackCount++;
		break;

	default:
		m_move = INITVECTOR3;
		Warp(ATTACK_NOT);
		SetDefColor();
		m_AttackCoolTime = COOLTIME_RAIN;
		break;
	}
}

//====================================================================
//攻撃(敵の生成)
//====================================================================
void CBoss::AttackSpawnEnemy(D3DXVECTOR3* pos)
{
	CPlayer* pPlayer = CGame::GetPlayer();
	CEnemy* pEnemy[2] = { nullptr };
	CCubeDamage* pCubeDamage = nullptr;

	int nRand = rand() % 6;
	m_MoveCount += 0.04f;
	m_move.y = sinf(m_MoveCount) * 2.0f;

	switch (m_AttackWave)
	{
	case 0:

		if (m_AttackCount > 300)
		{
			CManager::GetInstance()->GetCamera()->SetBib(false);
			m_AttackWave++;
		}
		else if (m_AttackCount == 0)
		{
			CManager::GetInstance()->GetCamera()->SetBib(true);
		}

		m_AttackCount++;

		if (m_AttackCount % 5 == 0)
		{
			CEffect* pEffect = CEffect::Create();
			pEffect->SetTexName("data\\TEXTURE\\RingEffect.png");
			pEffect->SetPos(D3DXVECTOR3(*pos));
			pEffect->SetDel(-40.0f);
			pEffect->SetLife(60);
			pEffect->SetRadius(0.0f);
			pEffect->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		}

		if (m_nForm == 0)
		{
			if (m_AttackCount % 110 == 0)
			{
				switch (nRand)
				{
				case 0:
					pEnemy[0]->SetPos(D3DXVECTOR3(-400.0f, 500.0f, 400.0f));
					pEnemy[1]->SetPos(D3DXVECTOR3(400.0f, 500.0f, 400.0f));
					break;
				case 1:
					pEnemy[0]->SetPos(D3DXVECTOR3(400.0f, 500.0f, -400.0f));
					pEnemy[1]->SetPos(D3DXVECTOR3(400.0f, 500.0f, 400.0f));
					break;
				case 2:
					pEnemy[0]->SetPos(D3DXVECTOR3(400.0f, 500.0f, 400.0f));
					pEnemy[1]->SetPos(D3DXVECTOR3(-400.0f, 500.0f, 400.0f));
					break;
				case 3:
					pEnemy[0]->SetPos(D3DXVECTOR3(400.0f, 500.0f, 400.0f));
					pEnemy[1]->SetPos(D3DXVECTOR3(400.0f, 500.0f, -400.0f));
					break;
				case 4:
					pEnemy[0]->SetPos(D3DXVECTOR3(-400.0f, 500.0f, 400.0f));
					pEnemy[1]->SetPos(D3DXVECTOR3(400.0f, 500.0f, -400.0f));
					break;
				case 5:
					pEnemy[0]->SetPos(D3DXVECTOR3(400.0f, 500.0f, -400.0f));
					pEnemy[1]->SetPos(D3DXVECTOR3(-400.0f, 500.0f, 400.0f));
					break;
				}
			}
		}
		else if (m_nForm == 1)
		{
			if (m_AttackCount % 70 == 0)
			{
				switch (nRand)
				{
				case 0:
					pEnemy[0]->SetPos(D3DXVECTOR3(-400.0f, 500.0f, 400.0f));
					pEnemy[1]->SetPos(D3DXVECTOR3(400.0f, 500.0f, 400.0f));
					break;
				case 1:
					pEnemy[0]->SetPos(D3DXVECTOR3(400.0f, 500.0f, -400.0f));
					pEnemy[1]->SetPos(D3DXVECTOR3(400.0f, 500.0f, 400.0f));
					break;
				case 2:
					pEnemy[0]->SetPos(D3DXVECTOR3(400.0f, 500.0f, 400.0f));
					pEnemy[1]->SetPos(D3DXVECTOR3(-400.0f, 500.0f, 400.0f));
					break;
				case 3:
					pEnemy[0]->SetPos(D3DXVECTOR3(400.0f, 500.0f, 400.0f));
					pEnemy[1]->SetPos(D3DXVECTOR3(400.0f, 500.0f, -400.0f));
					break;
				case 4:
					pEnemy[0]->SetPos(D3DXVECTOR3(-400.0f, 500.0f, 400.0f));
					pEnemy[1]->SetPos(D3DXVECTOR3(400.0f, 500.0f, -400.0f));
					break;
				case 5:
					pEnemy[0]->SetPos(D3DXVECTOR3(400.0f, 500.0f, -400.0f));
					pEnemy[1]->SetPos(D3DXVECTOR3(-400.0f, 500.0f, 400.0f));
					break;
				}
			}
		}
		break;

	default:
		m_move = INITVECTOR3;
		Warp(ATTACK_NOT);
		SetDefColor();
		m_AttackCoolTime = COOLTIME_RAIN;
		break;
	}
}

//====================================================================
//[2D]攻撃(ブロックウォール)
//====================================================================
void CBoss::AttackBlockWall(D3DXVECTOR3* pos)
{
	CPlayer* pPlayer = CGame::GetPlayer();
	CCubeDamage* pCubeDamage = nullptr;
	CCubeBlock* pCubeBlockUp = nullptr;
	CCubeBlock* pCubeBlockDown = nullptr;
	CCubeBlock* pCubeBlockFoot = nullptr;
	int nRand = rand() % 3;

	switch (m_AttackWave)
	{
	case 0:

		m_move = INITVECTOR3;

		m_Scaling -= 0.01f;

		if (m_Scaling <= 0.0f)
		{
			m_Scaling = 1.0f;
			m_AttackWave++;
			m_fRevivalColorA = 1.0f;
			m_pFGDel = 0.1f;
			CLog::Create(CLog::TEXT_06);

			pCubeBlockFoot = CCubeBlock::Create();
			pCubeBlockFoot->SetPos(D3DXVECTOR3(300.0f, 300.0f, 0.0f));
			pCubeBlockFoot->SetSize(D3DXVECTOR3(75.0f, 10.0f, 50.0f));
			pCubeBlockFoot->SetLife(500);

			pCubeBlockFoot = CCubeBlock::Create();
			pCubeBlockFoot->SetPos(D3DXVECTOR3(-300.0f, 300.0f, 0.0f));
			pCubeBlockFoot->SetSize(D3DXVECTOR3(75.0f, 10.0f, 50.0f));
			pCubeBlockFoot->SetLife(500);

			CManager::GetInstance()->GetCamera()->SetCameraMode(CCamera::CAMERAMODE_SIDEVIEW);
		}
		break;

	case 1:
		//上下にふわふわする
		m_MoveCount += 0.04f;
		m_move.y = sinf(m_MoveCount) * 2.0f;

		if (m_AttackCount % 60 == 0)
		{
			pCubeBlockUp = CCubeBlock::Create();
			pCubeBlockDown = CCubeBlock::Create();

			switch (nRand)
			{
			case 0:
				pCubeBlockUp->SetPos(D3DXVECTOR3(500.0f, 1100.0f, 0.0f));
				pCubeBlockDown->SetPos(D3DXVECTOR3(500.0f, pCubeBlockUp->GetPos().y - BLOCKWALL_HEIGHT, 0.0f));
				break;
			case 1:
				pCubeBlockUp->SetPos(D3DXVECTOR3(500.0f, 900.0f, 0.0f));
				pCubeBlockDown->SetPos(D3DXVECTOR3(500.0f, pCubeBlockUp->GetPos().y - BLOCKWALL_HEIGHT, 0.0f));
				break;
			case 2:
				pCubeBlockUp->SetPos(D3DXVECTOR3(500.0f, 700.0f, 0.0f));
				pCubeBlockDown->SetPos(D3DXVECTOR3(500.0f, pCubeBlockUp->GetPos().y - BLOCKWALL_HEIGHT, 0.0f));
				break;
			}
			pCubeBlockUp->SetSize(D3DXVECTOR3(25.0f, 500.0f, 50.0f));
			pCubeBlockUp->SetMove(D3DXVECTOR3(-BLOCKRUN_SPEED, 0.0f, 0.0f));
			pCubeBlockUp->SetUninitPos(D3DXVECTOR3(-900.0f, pCubeBlockUp->GetPos().y, pCubeBlockDown->GetPos().z));
			pCubeBlockUp->SetBoolLife(true);
			pCubeBlockUp->SetPlusMove(false);
			pCubeBlockUp->SetColorA(1.0f);

			pCubeBlockDown->SetSize(D3DXVECTOR3(25.0f, 500.0f, 50.0f));
			pCubeBlockDown->SetMove(D3DXVECTOR3(-BLOCKRUN_SPEED, 0.0f, 0.0f));
			pCubeBlockDown->SetUninitPos(D3DXVECTOR3(-900.0f, pCubeBlockDown->GetPos().y, pCubeBlockDown->GetPos().z));
			pCubeBlockDown->SetBoolLife(true);
			pCubeBlockDown->SetPlusMove(false);
			pCubeBlockDown->SetColorA(1.0f);
		}

		if (m_AttackCount > 400)
		{
			m_AttackWave++;
		}
		m_AttackCount++;
		break;

	case 2:
		m_move = INITVECTOR3;

		m_Scaling -= 0.01f;

		if (m_Scaling <= 0.0f)
		{
			m_Scaling = 1.0f;
			m_AttackWave++;
			m_fRevivalColorA = 1.0f;

			CManager::GetInstance()->GetCamera()->SetCameraMode(CCamera::CAMERAMODE_FOLLOW);
		}
		break;

	case 3:
		//蘇生ポリゴン出現時間の設定
		if (m_fRevivalColorA <= 0.0f)
		{
			m_AttackWave++;
		}
		break;

	default:
		m_move = INITVECTOR3;
		Warp(ATTACK_NOT);
		SetDefColor();
		m_AttackCoolTime = COOLTIME_RAIN;
		break;
	}
}

//====================================================================
//[見下ろし]攻撃(マップ破壊)
//====================================================================
void CBoss::AttackMapBreak(D3DXVECTOR3* pos)
{
	CPlayer* pPlayer = CGame::GetPlayer();
	CCubeDamage* pCubeDamage = nullptr;
	CCubeBlock* pCubeBlock = nullptr;
	CCubeBlock* pCubeBlockFoot = nullptr;
	int nRandX = 0;
	int nRandZ = 0;

	switch (m_AttackWave)
	{
	case 0:

		m_move = INITVECTOR3;

		m_Scaling -= 0.01f;

		if (m_Scaling <= 0.0f)
		{
			m_Scaling = 1.0f;
			*pos = D3DXVECTOR3(0.0f, 10000.0f, 0.0f);
			m_AttackWave++;
			m_fRevivalColorA = 1.0f;
			m_pFGDel = 0.1f;
			CLog::Create(CLog::TEXT_07);

			CManager::GetInstance()->GetCamera()->SetCameraMode(CCamera::CAMERAMODE_DOWNVIEW);
		}
		break;

	case 1:
		//上下にふわふわする
		m_MoveCount += 0.04f;
		m_move.y = sinf(m_MoveCount) * 2.0f;

		if (m_AttackCount == 0)
		{
			CGame::GetCubeBlock()->Uninit();

			for (int nCntX = -1; nCntX < 2; nCntX++)
			{
				for (int nCntZ = -1; nCntZ < 2; nCntZ++)
				{
					pCubeBlock = CCubeBlock::Create();
					pCubeBlock->SetPos(D3DXVECTOR3(-333.0f * nCntX, 100.0f, -333.0f * nCntZ));
					pCubeBlock->SetSize(D3DXVECTOR3(166.5f, 10.0f, 166.5f));
					pCubeBlock->SetMove(D3DXVECTOR3(-10.0f * nCntX, 1.0f, -10.0f * nCntZ));
					pCubeBlock->SetLife(10);
				}
			}
		}

		CGame::GetPlayer()->SetMove(D3DXVECTOR3(0.0f, CGame::GetPlayer()->GetMove().y, 0.0f));

		if (m_AttackCount > 10)
		{
			m_AttackWave++;
		}
		m_AttackCount++;
		break;

	case 2:

		for (int nCntX = -1; nCntX < 2; nCntX++)
		{
			for (int nCntZ = -1; nCntZ < 2; nCntZ++)
			{
				pCubeBlock = CCubeBlock::Create();
				pCubeBlock->SetPos(D3DXVECTOR3((-333.0f - 100.0f) * nCntX, 100.0f, (-333.0f - 100.0f) * nCntZ));
				pCubeBlock->SetSize(D3DXVECTOR3(166.5f, 10.0f, 166.5f));
				pCubeBlock->SetLife(710);
			}
		}

		m_AttackWave++;
		break;

	case 3:

		if (m_AttackCount % 100 == 0)
		{
			//ランダム処理
			while (1)
			{
				nRandX = (rand() % 3) - 1;
				nRandZ = (rand() % 3) - 1;

				if (m_bBreak[((nRandX + 1) + ((nRandZ + 1) * 3))] == true || (nRandX + nRandZ) == 0)
				{
					continue;
				}
				else
				{
					m_bBreak[((nRandX + 1) + ((nRandZ + 1) * 3))] = true;
					break;
				}
			}

			*pos = D3DXVECTOR3((-333.0f - 100.0f) * nRandX, 3000.0f, (-333.0f - 100.0f) * nRandZ);
		}
		m_move = D3DXVECTOR3(0.0f, -30.0f, 0.0f);

		//ブロックとの当たり判定
		CollisionBlock(pos);

		if (m_AttackCount >= 500)
		{
			m_AttackWave++;
		}
		else
		{
			m_AttackCount++;
		}

		break;

	case 4:
		m_move = INITVECTOR3;

		m_Scaling -= 0.005f;

		if (m_Scaling <= 0.0f)
		{
			m_Scaling = 1.0f;
			m_AttackWave++;
			m_fRevivalColorA = 1.0f;

			CManager::GetInstance()->GetCamera()->SetCameraMode(CCamera::CAMERAMODE_FOLLOW);
		}
		break;

	case 5:
		//蘇生ポリゴン出現時間の設定
		if (m_fRevivalColorA <= 0.0f)
		{
			m_AttackWave++;
		}

		for (int nCntX = -1; nCntX < 2; nCntX++)
		{
			for (int nCntZ = -1; nCntZ < 2; nCntZ++)
			{
				pCubeBlock = CCubeBlock::Create();
				pCubeBlock->SetPos(D3DXVECTOR3((-333.0f - 100.0f) * nCntX, 100.0f, (-333.0f - 100.0f) * nCntZ));
				pCubeBlock->SetSize(D3DXVECTOR3(166.5f, 10.0f, 166.5f));
				pCubeBlock->SetMove(D3DXVECTOR3(10.0f * nCntX, 1.0f, 10.0f * nCntZ));
				pCubeBlock->SetLife(10);
			}
		}

		break;

	default:
		m_move = INITVECTOR3;
		Warp(ATTACK_NOT);
		SetDefColor();
		m_AttackCoolTime = COOLTIME_RAIN;
		CGame::SetStageBlock();

		for (int nCnt = 0; nCnt < 9; nCnt++)
		{
			m_bBreak[nCnt] = false;
		}
		break;
	}
}

//====================================================================
//攻撃(蘇生)
//====================================================================
void CBoss::AttackRevival(D3DXVECTOR3* pos)
{
	CPlayer* pPlayer = CGame::GetPlayer();
	CCubeDamage* pCubeDamage = nullptr;

	switch (m_AttackWave)
	{
	case 0:	//蘇生前点滅
		m_MoveCount += 0.1f;
		m_move.y = sinf(m_MoveCount) * 4.0f;

		m_AttackCount++;

		if (m_AttackCount % 10 == 0)
		{
			m_bRevivalColorSwitch = m_bRevivalColorSwitch ? false : true;

			if (m_bRevivalColorSwitch == true)
			{
				SetMatColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}
			else
			{
				SetDefColor();
			}
		}

		if (m_AttackCount > 100)
		{
			m_AttackCount = 0;
			m_Scaling = 1.0f;
			m_AttackWave++;
		}
		break;

	case 1:	//蘇生前テレポート
		m_MoveCount += 0.1f;
		m_move.y = sinf(m_MoveCount) * 4.0f;

		m_AttackCount++;

		if (m_AttackCount % 5 == 0)
		{
			m_bRevivalColorSwitch = m_bRevivalColorSwitch ? false : true;

			if (m_bRevivalColorSwitch == true)
			{
				SetMatColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}
			else
			{
				SetDefColor();
			}
		}

		if (m_AttackCount % 5 == 0)
		{
			for (int nCnt = 0; nCnt < 8; nCnt++)
			{
				//モデルエフェクトの生成
				CModelEffect* pMEffect = CModelEffect::Create("data\\MODEL\\boss.x");
				pMEffect->SetPos(*pos);
				pMEffect->SetRot(m_rot);
				pMEffect->SetColor(D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));

				CCubeDamage* pCubeDamage = CCubeDamage::Create();
				pCubeDamage->SetUseSpin(true);
				pCubeDamage->SetSpinPos(*pos);
				pCubeDamage->SetSpinCount(D3DXVECTOR3(m_SpinCount + nCnt * D3DX_PI * 0.25f, m_SpinCount + nCnt * D3DX_PI * 0.25f, m_SpinCount + nCnt * D3DX_PI * 0.25f));
				pCubeDamage->SetSpinSpeedY(0.03f);
				pCubeDamage->SetSpinDistance(10.0f);
				pCubeDamage->SetCubeType(CCubeDamage::CUBETYPE_EXPLOSION);
				pCubeDamage->SetExplosionCount(101);
				pCubeDamage->SetLife(450);
				pCubeDamage->SetSpinDisMove(9.0f);
				pCubeDamage->SetSize(D3DXVECTOR3(20.0f, 20.0f, 20.0f));
				pCubeDamage->SetDamage(SPINPILLAR_DAMAGE);
			}

			pos->x = (float)(rand() % 1001) - 500.0f;
			pos->y = (float)(rand() % 101) + 250.0f;
			pos->z = (float)(rand() % 1001) - 500.0f;
		}

		if (m_AttackCount > 120)
		{
			m_AttackCount = 0;
			m_AttackWave++;
		}
		break;

	case 2: //蘇生前原点移動
		m_move = INITVECTOR3;

		SetDefColor();

		if (m_AttackCount % 30 == 0)
		{
			pos->x = 0.0f;
			pos->y = 300.0f;
			pos->z = 0.0f;
		}

		m_AttackWave++;
		break;

	case 3:	//収縮からの蘇生処理
		m_move = INITVECTOR3;

		m_Scaling -= 0.01f;

		if (m_Scaling <= 0.0f)
		{
			m_Scaling = 1.0f;
			m_AttackWave++;
			m_nForm++;
			m_fRevivalColorA = 1.0f;
			m_pFGDel = 0.05f;

			if (m_CubeSpin == nullptr)
			{
				m_CubeSpin = CCubeSpin::Create(16);
				m_CubeSpin->SetPos(D3DXVECTOR3(pos->x, pos->y + 100.0f, pos->z));
				m_CubeSpin->SetSpinDistance(75.0f);
				m_CubeSpin->SetSpinSpeedY(0.2f);
				m_CubeSpin->SetDamage(50.0f);
			}

			m_fLife = BOSS_LIFE;
			m_bDelLife = false;
			SetLifeUI();
		}

		break;

	case 4:	//蘇生終了処理
		m_MoveCount += 0.03f;
		m_move.y = sinf(m_MoveCount) * 2.0f;

		m_AttackCount++;

		if (m_AttackCount > 100)
		{
			m_AttackWave++;
		}

		break;

	default:
		m_move = INITVECTOR3;
		Warp(ATTACK_NOT);
		SetDefColor();
		m_State = STATE_NORMAL;
		m_AttackCoolTime = 0;
		break;
	}
}

//====================================================================
//死亡時演出
//====================================================================
void CBoss::AttackDeath(D3DXVECTOR3* pos)
{
	switch (m_AttackWave)
	{
	case 0:	//
		//カメラのバイブレーションをオンにする
		CManager::GetInstance()->GetCamera()->SetBib(true);

		m_move = INITVECTOR3;

		if (m_AttackCount % 5 == 0)
		{
			m_bDeathColorSwich = m_bDeathColorSwich ? false : true;

			if (m_bDeathColorSwich == true)
			{
				SetMatColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}
			else
			{
				SetMatColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			}
		}

		if (m_bDeathColorSwich == true)
		{
			m_Scaling += 0.06f;
		}
		else
		{
			m_Scaling -= 0.06f;
		}

		if (m_AttackCount % 60 == 0)
		{
			m_fRevivalColorA = 1.0f;
			m_pFGDel = 0.05f;

			//ゲームのSEを再生する
			CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_DAMAGE_BOSS);
		}

		m_AttackCount++;

		if (m_AttackCount > 120)
		{
			m_Scaling = 1.0f;
			m_AttackWave++;
			m_AttackCount = 0;
			m_fDeathColor = 1.0f;
			SetMatColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

			//カメラのバイブレーションをオンにする
			CManager::GetInstance()->GetCamera()->SetBib(false);
		}
		break;

	case 1:	//
		m_move = INITVECTOR3;

		m_AttackCount++;

		m_Scaling += 0.005f;
		m_fDeathColor -= 0.005f;
		SetMatColor(D3DXCOLOR(m_fDeathColor, m_fDeathColor, m_fDeathColor, 1.0f));
		CGame::AddBGColorA(-0.005f);

		if (m_AttackCount > 200)
		{
			m_AttackCount = 0;
			m_AttackWave++;
		}
		break;

	case 2:	//
		m_move = INITVECTOR3;

		m_AttackCount++;

		m_Scaling -= 0.1f;

		if (m_AttackCount > 20)
		{
			m_AttackCount = 0;
			m_AttackWave++;
			m_fDeathExplojsionDis = 75.0f;
		}
		break;

	case 3:	//
		m_move = INITVECTOR3;

		m_AttackCount++;

		//ゲームのSEを再生する
		CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_DEATHEXPLOSION);

		DeathExplosion(pos, D3DXVECTOR3(DEATH_EXPLOSION_SPEED, 100.0f, 100.0f), 0);
		DeathExplosion(pos, D3DXVECTOR3(DEATH_EXPLOSION_SPEED, 100.0f, 100.0f), 1);
		DeathExplosion(pos, D3DXVECTOR3(DEATH_EXPLOSION_SPEED, 100.0f, 100.0f), 2);
		DeathExplosion(pos, D3DXVECTOR3(100.0f, DEATH_EXPLOSION_SPEED, 100.0f), 0);
		DeathExplosion(pos, D3DXVECTOR3(100.0f, DEATH_EXPLOSION_SPEED, 100.0f), 1);
		DeathExplosion(pos, D3DXVECTOR3(100.0f, DEATH_EXPLOSION_SPEED, 100.0f), 2);
		DeathExplosion(pos, D3DXVECTOR3(100.0f, 100.0f, DEATH_EXPLOSION_SPEED), 0);
		DeathExplosion(pos, D3DXVECTOR3(100.0f, 100.0f, DEATH_EXPLOSION_SPEED), 1);
		DeathExplosion(pos, D3DXVECTOR3(100.0f, 100.0f, DEATH_EXPLOSION_SPEED), 2);
		DeathExplosion(pos, D3DXVECTOR3(DEATH_EXPLOSION_SPEED, DEATH_EXPLOSION_SPEED, 100.0f), 0);
		DeathExplosion(pos, D3DXVECTOR3(DEATH_EXPLOSION_SPEED, DEATH_EXPLOSION_SPEED, 100.0f), 1);
		DeathExplosion(pos, D3DXVECTOR3(DEATH_EXPLOSION_SPEED, DEATH_EXPLOSION_SPEED, 100.0f), 2);
		DeathExplosion(pos, D3DXVECTOR3(100.0f, DEATH_EXPLOSION_SPEED, DEATH_EXPLOSION_SPEED), 0);
		DeathExplosion(pos, D3DXVECTOR3(100.0f, DEATH_EXPLOSION_SPEED, DEATH_EXPLOSION_SPEED), 1);
		DeathExplosion(pos, D3DXVECTOR3(100.0f, DEATH_EXPLOSION_SPEED, DEATH_EXPLOSION_SPEED), 2);
		DeathExplosion(pos, D3DXVECTOR3(DEATH_EXPLOSION_SPEED, 100.0f, DEATH_EXPLOSION_SPEED), 0);
		DeathExplosion(pos, D3DXVECTOR3(DEATH_EXPLOSION_SPEED, 100.0f, DEATH_EXPLOSION_SPEED), 1);
		DeathExplosion(pos, D3DXVECTOR3(DEATH_EXPLOSION_SPEED, 100.0f, DEATH_EXPLOSION_SPEED), 2);

		if (m_AttackCount > 0)
		{
			m_AttackCount = 0;
			m_AttackWave++;
		}
		break;

	case 4:	//
		m_move = INITVECTOR3;

		m_AttackCount++;

		if (m_AttackCount < 10)
		{
			m_Scaling += 0.3f;
		}

		if (m_AttackCount == 10)
		{
			SetMatColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
			m_Scaling = 0.0f;

			if (m_pShadow != nullptr)
			{
				m_pShadow->Uninit();
				m_pShadow = nullptr;
			}
			if (m_CubeSpin != nullptr)
			{
				m_CubeSpin->Uninit();
				m_CubeSpin = nullptr;
			}
		}

		if (m_AttackCount >= 70)
		{
			CGame::AddBGColorA(0.02f);
		}

		if (m_AttackCount > 120)
		{
			m_AttackCount = 0;
			m_AttackWave++;
		}
		break;

	default:
		m_move = INITVECTOR3;
		m_State = STATE_DEATH;
		CManager::GetInstance()->SetGameClear(true);
		CGame::SetGameEnd(true);
		break;
	}
}

//====================================================================
//爆発キューブの呼び出し
//====================================================================
void CBoss::DeathExplosion(D3DXVECTOR3* pos, D3DXVECTOR3 SpinMove, int Set)
{
	CCubeDamage* pCubeDamage = nullptr;

	for (int nCnt = 0; nCnt < DEATH_EXPLOSION; nCnt++)
	{
		pCubeDamage = CCubeDamage::Create();
		pCubeDamage->SetUseSpin(true);
		pCubeDamage->SetSpinPos(*pos);
		pCubeDamage->SetSpinCount(D3DXVECTOR3(m_SpinCount + nCnt * D3DX_PI * 2.0f * (1.0f / DEATH_EXPLOSION), m_SpinCount + nCnt * D3DX_PI * 2.0f * (1.0f / DEATH_EXPLOSION), m_SpinCount + nCnt * D3DX_PI * 2.0f * (1.0f / DEATH_EXPLOSION)));
		pCubeDamage->SetSpinSpeedX(-SpinMove.x);
		pCubeDamage->SetSpinSpeedY(-SpinMove.y);
		pCubeDamage->SetSpinSpeedZ(-SpinMove.z);
		pCubeDamage->SetSpinDistance(10.0f);
		pCubeDamage->SetCubeType(CCubeDamage::CUBETYPE_BREAK);
		pCubeDamage->SetExplosionCount(101);
		pCubeDamage->SetLife(120);
		pCubeDamage->SetSpinDisMove(40.0f);
		pCubeDamage->SetSize(D3DXVECTOR3(20.0f, 20.0f, 20.0f));
		pCubeDamage->SetDamage(0.0f);
		pCubeDamage->SetSpinSinCos(Set);
		pCubeDamage->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	}
}

//====================================================================
//ダメージ処理
//====================================================================
void CBoss::HitDamage(float Damage)
{
	if (m_State == STATE_NORMAL)
	{
		m_fLife -= Damage;
		if (m_fLife < 0.0f)
		{
			CManager::GetInstance()->GetCamera()->SetCameraMode(CCamera::CAMERAMODE_FOLLOW);

			CManager::GetInstance()->GetCamera()->SetAttention(false);
			CManager::GetInstance()->GetCamera()->SetBib(false);
			m_fLife = 0.0f;
			m_AttackCount = 0;
			m_AttackWave = 0;
			m_fRevivalColorA = 1.0f;
			m_pFGDel = 0.05f;

			if (m_nForm == 0)
			{
				CLog::Create(CLog::TEXT_02);
				m_State = STATE_INVINCIBLE;
				Warp(ATTACK_REVIVAL);

				//ゲームのSEを再生する
				CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_DEATHEXPLOSION);
			}
			else if (m_nForm == 1)
			{
				//ゲームのSEを再生する
				CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_GOGOGOGO);

				m_State = STATE_DEATH;
				m_Action = ACTION_DEATH;
				CGame::GetTime()->SetStopTime(true);
			}
		}
		else
		{
			//ゲームのSEを再生する
			CManager::GetInstance()->GetSound()->PlaySoundA(CSound::SOUND_LABEL_SE_DAMAGE_BOSS);

			m_pLifeGauge->SetGaugeWidth(m_fLifeMax, m_fLife);
			m_State = STATE_DAMAGE;
			m_nStateCount = 5;
			SetMatColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		}
		SetLifeUI();
	}
}

//====================================================================
//ライフUIの更新
//====================================================================
void CBoss::SetLifeUI(void)
{
	for (int nCnt = 0; nCnt < 5; nCnt++)
	{
		if (m_pLifeNumber[nCnt] == nullptr)
		{
			return;
		}
	}

	m_pLifeNumber[0]->SetNumber((int)m_fLife % 100000 / 10000);
	m_pLifeNumber[1]->SetNumber((int)m_fLife % 10000 / 1000);
	m_pLifeNumber[2]->SetNumber((int)m_fLife % 1000 / 100);
	m_pLifeNumber[3]->SetNumber((int)m_fLife % 100 / 10);
	m_pLifeNumber[4]->SetNumber((int)m_fLife % 10 / 1);
}

//====================================================================
//当たり判定処理
//====================================================================
bool CBoss::Collision(D3DXVECTOR3* pPos, D3DXVECTOR3 pPosOld, D3DXVECTOR3* pMove, float fHeight, float fWidth, bool* bJumpMove, bool* bHit, bool bX)
{
	bool bOn = false;

	float myWidth = GetWidth();
	float myHeight = GetHeight();
	D3DXVECTOR3 mtPos = GetPos();


	if (bX == false)
	{
		if (
			(pPos->y < mtPos.y + myHeight &&
				pPosOld.y >= mtPos.y + myHeight) &&
			pPos->x + fWidth > mtPos.x - myWidth &&
			pPos->x - fWidth < mtPos.x + myWidth
			)
		{//上からめり込んだ時
			bOn = true;
			pPos->y = mtPos.y + myHeight;		//ブロックのうえにたたせる
			pMove->y = 0.0f;					//移動量を０にする
			*bHit = true;
		}

		if (
			(pPos->y + fHeight > mtPos.y - myHeight &&
				pPosOld.y <= mtPos.y - myHeight) &&
			pPos->x + fWidth > mtPos.x - myWidth &&
			pPos->x - fWidth < mtPos.x + myWidth
			)
		{//下からめり込んだ時
			*bJumpMove = false;
			pPos->y = mtPos.y - myHeight - fHeight;		//ブロックの下で止める
			pMove->y = 0.0f;					//移動量を０にする
			*bHit = true;
		}
	}
	else
	{
		if (
			(pPos->x + fWidth > mtPos.x - myWidth &&
				pPosOld.x + fWidth <= mtPos.x - myWidth) &&
			pPos->y + fHeight > mtPos.y - myHeight &&
			pPos->y < mtPos.y + myHeight
			)
		{//左からめり込んだ時
			pPos->x = mtPos.x - myWidth - fWidth;		//ブロックの左で止める
			pMove->x = 0.0f;					//移動量を０にする
			*bHit = true;
		}

		if (
			(pPos->x - fWidth < mtPos.x + myWidth &&
				pPosOld.x - fWidth >= mtPos.x + myWidth) &&
			pPos->y + fHeight > mtPos.y - myHeight &&
			pPos->y < mtPos.y + myHeight
			)
		{//右からめり込んだ時
			pPos->x = mtPos.x + myWidth + fWidth;		//ブロックの右で止める
			pMove->x = 0.0f;					//移動量を０にする
			*bHit = true;
		}
	}

	return bOn;
}

//====================================================================
//キューブブロックとの当たり判定処理
//====================================================================
bool CBoss::CollisionBlock(D3DXVECTOR3* pos)
{
	for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
	{
		//オブジェクトを取得
		CObject* pObj = CObject::GetTop(nCntPriority);

		while (pObj != NULL)
		{
			CObject* pObjNext = pObj->GetNext();

			CObject::TYPE type = pObj->GetType();			//種類を取得

			if (m_State != STATE_DEATH)
			{
				if (type == TYPE_CUBEBLOCK)
				{//種類がブロックの時

					D3DXVECTOR3 ObjPos = pObj->GetPos();
					float ObjWidth = pObj->GetSize().x;

					if (CollisionCircle(*pos, ObjPos, COLLISION_SIZE.x + ObjWidth) == true)
					{
						//リングの生成
						CObjmeshRing* pRing = CObjmeshRing::Create();
						pRing->SetPos(D3DXVECTOR3(ObjPos.x, ObjPos.y + 15.0f, ObjPos.z));
						pRing->SetRadiusMove(5.0f);
						pRing->SetLife(300);
						pRing->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.5f));

						for (int nCntX = -1; nCntX <= 1; nCntX++)
						{
							for (int nCntZ = -1; nCntZ <= 1; nCntZ++)
							{
								CCubeEffect* pCEffect = CCubeEffect::Create();
								pCEffect->SetPos(D3DXVECTOR3(ObjPos.x + (111.0f * nCntX), ObjPos.y, ObjPos.z + (111.0f * nCntZ)));
								pCEffect->SetMove(D3DXVECTOR3((5.0f * nCntX), 20.0f, (5.0f * nCntZ)));
								pCEffect->SetSize(D3DXVECTOR3(55.5f, 10.0f, 55.5f));
								pCEffect->SetColor(D3DXCOLOR(0.5f, 0.5f, 0.0f, 0.5f));
								pCEffect->SetFall(true);
							}
						}

						pObj->Uninit();
					}
				}
			}

			pObj = pObjNext;
		}
	}

	return false;
}

//====================================================================
//影の当たり判定処理
//====================================================================
bool CBoss::CollisionShadow(void)
{
	if (m_pShadow != nullptr)
	{
		float BlockPosY = -10000.0f;
		bool bShadow = false;;

		for (int nCntPriority = 0; nCntPriority < PRIORITY_MAX; nCntPriority++)
		{
			//オブジェクトを取得
			CObject* pObj = CObject::GetTop(nCntPriority);

			while (pObj != NULL)
			{
				CObject* pObjNext = pObj->GetNext();

				CObject::TYPE type = pObj->GetType();			//種類を取得

				if (type == TYPE_CUBEBLOCK)
				{//種類がブロックの時
					CCubeBlock* pBlock = (CCubeBlock*)pObj;

					D3DXVECTOR3 MyPos = GetPos();
					D3DXVECTOR3 MySize = COLLISION_SIZE * 2.0f;
					D3DXVECTOR3 BlockPos = pBlock->GetPos();
					D3DXVECTOR3 BlockSize = pBlock->GetSize();
					D3DXVECTOR3 BlockMove = pBlock->GetMove();

					if (BlockPos.y + BlockSize.y > BlockPosY)
					{
						if (BlockPos.x + BlockSize.x > MyPos.x &&
							BlockPos.x - BlockSize.x < MyPos.x &&
							BlockPos.z + BlockSize.z > MyPos.z &&
							BlockPos.z - BlockSize.z < MyPos.z &&
							BlockPos.y - BlockSize.y < MyPos.y)
						{
							float ColorA = (MyPos.y - BlockPos.y) * 0.0005f;
							m_pShadow->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f - ColorA));
							m_pShadow->SetPos(D3DXVECTOR3(
								MyPos.x,
								BlockPos.y + BlockSize.y + BlockMove.y,
								MyPos.z));

							if (CManager::GetInstance()->GetCamera()->GetCameraMode() == CCamera::CAMERAMODE_DOWNVIEW)
							{
								m_pShadow->SetWidth(MySize.x * 2.5f);
								m_pShadow->SetHeight(MySize.z * 2.5f);
							}
							else
							{
								m_pShadow->SetWidth(MySize.x);
								m_pShadow->SetHeight(MySize.z);
							}

							BlockPosY = BlockPos.y + BlockSize.y;
							bShadow = true;
						}
					}
				}

				pObj = pObjNext;
			}
		}

		if (bShadow == false)
		{
			m_pShadow->SetWidth(0.0f);
			m_pShadow->SetHeight(0.0f);
		}
	}
	return false;
}

//====================================================================
//円の当たり判定
//====================================================================
bool CBoss::CollisionCircle(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2, float nRadiusOut)
{
	bool nHit = false;

	if (sqrtf((pos1.x - pos2.x) * (pos1.x - pos2.x)
		+ (pos1.y - pos2.y) * (pos1.y - pos2.y)) <= nRadiusOut)
	{//円の判定が当たった
		if (sqrtf((pos1.x - pos2.x) * (pos1.x - pos2.x)
			+ (pos1.z - pos2.z) * (pos1.z - pos2.z)) <= nRadiusOut)
		{//円の判定が当たった
			nHit = true;
		}
	}

	return nHit;
}