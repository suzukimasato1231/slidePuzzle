#pragma once
#include"Menu.h"
/// <summary>
/// UIクラス
/// </summary>
class UI
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	UI();
	/// <summary>
	/// デスコンストラクタ
	/// </summary>
	~UI();
	/// <summary>
	/// 初期化
	/// </summary>
	void Init();
	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="fishNum">魚数</param>
	/// <param name="clearFlag">クリアフラグ</param>
	/// <param name="changeScene">チェンジフラグ</param>
	/// <param name="sceneNum">シーン数</param>
	void Update(const int fishNum, const bool clearFlag, bool& changeScene, int& sceneNum);
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="pLives">残機数</param>
	/// <param name="gameoverFlag">ゲームオーバーフラグ</param>
	void Draw(const int pLives, const  bool gameoverFlag);
	/// <summary>
	/// リセット
	/// </summary>
	void Reset();

	bool GetMenuFlag()const { return m_menuFlag; }
private:
	/// <summary>
	/// 表示する数字を決定
	/// </summary>
	void NumberDecision(const int fishNum);
	/// <summary>
	/// 数字強調処理
	/// </summary>
	void NumberEmphasis();
private:
	//ゲーム画面UI
	SpriteData m_boxGraph;

	SpriteData m_playerIcon;

	SpriteData m_fishGraph;

	SpriteData m_uiNumber[10];
	SpriteData m_uiSlash;

	//メニュー
	Menu m_menu;
	bool m_menuFlag = false;

	//演出(ゲーム画面)
	float m_fishPosPlas = 0.0f;
	const int c_stagingTimeMax = 20;
	const float c_stagingScaleMax = 1.5f;
	static const int numberNum = 4;
	float m_stagingScale[numberNum] = { 0.0f,0.0f,0.0f,0.0f };
	int m_stagingTime[numberNum] = {};
	bool m_stagingFlag[numberNum] = {};
	//壊した箱の数
	int m_breakNumber = 1, m_breakRemain = 1;
	//最大箱の数
	int m_breakNumberMax = 1, m_breakRemainMax = 1;
	//魚の数
	int m_fishNumber = 1, m_fishRemain = 1;
};

