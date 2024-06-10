//===================================
//
// ログヘッダー[log.h]
// Author 坂本翔唯
//
//===================================
#ifndef _LOG_H_
#define _LOG_H_

//***********************************
// インクルードファイル
//***********************************
class CObject2D;
class CPlayer;

//-----------------------------------
// クラス定義(サンプル)
//-----------------------------------
class CLog : public CObject/*親クラス*/
{
public:			// 誰でもアクセスできる

	CLog(int nPriority = 5);				// コンストラクタ
	~CLog();			// デストラクタ

	enum STATE
	{
		STATE_IN = 0,
		STATE_DOWN,
		STATE_OUT,
		STATE_MAX,
	};

	enum TEXT
	{
		TEXT_00 = 0,	//ボスがピンク色の時は近づきすぎるなよ
		TEXT_01,		//やつの頭上のリングには当たらないほうが良さそうだな
		TEXT_02,		//やったか！？
		TEXT_03,		//攻撃は地上で剣を振った方がダメージが高いぞ
		TEXT_04,		//赤とオレンジのブロックは触れるとダメージをくらうぞ
		TEXT_05,		//回避中は無敵状態となり素早く移動できるぞ
		TEXT_06,		//このままだと落とされるぞ
		TEXT_07,		//上から来るぞ！余波にも注意しろ！
		TEXT_08,		//冷静に影を見れば十分かわせるな
		TEXT_09,		//Lスティックで移動Rスティックで視点操作だ！
		TEXT_10,		//ジャンプで壁を乗り越えよう
		TEXT_11,		//敵だ！攻撃で倒そう！
		TYPE_MAX,
	};

	// メンバ関数
	HRESULT Init(void);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理

	void SetData(CLog::TEXT Type);		// 情報の設定処理
	void SetLogIdx(int nIdx) { m_nLogIdx = nIdx; }

	void SetMain(CPlayer *pMain) { m_pMain = pMain; }		// 自分の事を使用しているオブジェクトのポインタを設定する
	void DelCreateNumber(void) { m_CreateNumber--; }

	// 静的メンバ関数
	static CLog* Create(CLog::TEXT Type);		// 生成処理
	static void InitCreateMax(void) { m_CreateMax = -1; }		// 生成処理

private:		// 自分だけアクセスできる
	void StateManager(void);
	bool DelNumber(void);		//生成番号の修正

	D3DXVECTOR3 LogPos;			// ログ全体の位置
	int m_CreateNumber;			// 何番目の生成のログか
	static int m_CreateMax;		// 生成のログがいくつあるか
	STATE m_State;				// 状態
	int m_nLife;				// 寿命
	int m_nLogIdx;				// ログの使用番号
	float m_fLogStopPosY;		// ログが停止する位置
	TEXT m_Type;				// ログの種類
	CObject2D *m_pLogBG;		// ログの背景パーツ
	CObject2D *m_pLogPlayerBG;	// ログのプレイヤーIDパーツ
	CObject2D *m_pLogMessage;	// ログの内容パーツ
	CPlayer *m_pMain;			// 自分の事を使用しているオブジェクトのポインタ
};
#endif