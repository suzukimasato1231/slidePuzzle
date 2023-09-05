#include "UI.h"
#include"../stage/Stage.h"
#include"DebugText.h"
UI::UI()
{
}
UI::~UI()
{
}
void UI::Init()
{
	m_boxGraph = Sprite::Get()->SpriteCreate(L"Resources/UI/UIBox.png");
	m_playerIcon = Sprite::Get()->SpriteCreate(L"Resources/UI/penginIcon.png");
	m_fishGraph = Sprite::Get()->SpriteCreate(L"Resources/UI/fish.png");
	m_uiNumber[0] = Sprite::Get()->SpriteCreate(L"Resources/UI/UINumber1.png");
	m_uiNumber[1] = Sprite::Get()->SpriteCreate(L"Resources/UI/UINumber2.png");
	m_uiNumber[2] = Sprite::Get()->SpriteCreate(L"Resources/UI/UINumber3.png");
	m_uiNumber[3] = Sprite::Get()->SpriteCreate(L"Resources/UI/UINumber4.png");
	m_uiNumber[4] = Sprite::Get()->SpriteCreate(L"Resources/UI/UINumber5.png");
	m_uiNumber[5] = Sprite::Get()->SpriteCreate(L"Resources/UI/UINumber6.png");
	m_uiNumber[6] = Sprite::Get()->SpriteCreate(L"Resources/UI/UINumber7.png");
	m_uiNumber[7] = Sprite::Get()->SpriteCreate(L"Resources/UI/UINumber8.png");
	m_uiNumber[8] = Sprite::Get()->SpriteCreate(L"Resources/UI/UINumber9.png");
	m_uiNumber[9] = Sprite::Get()->SpriteCreate(L"Resources/UI/UINumber10.png");
	m_uiSlash = Sprite::Get()->SpriteCreate(L"Resources/UI/UISlash.png");

	m_menu.Init();
}

void UI::Update(const int fishNum, const bool clearFlag, bool& changeScene, int& sceneNum)
{
	//表示する数字を決定
	NumberDecision(fishNum);

	//数字強調処理
	NumberEmphasis();

	if (clearFlag == false)
	{
		m_menu.Update(m_menuFlag, changeScene, sceneNum);
	}
}


void UI::Draw(const int pLives, const  bool gameoverFlag)
{
	const float numberScale = 64.0f;			//数字の大きさ
	const Vec2 numAncorPoint = { 0.5f, 0.5f };	//数字のアンカーポイント
	//プレイヤー残機
	Sprite::Get()->Draw(m_playerIcon, Vec2(1132.0f, 42.0f), numberScale, numberScale, numAncorPoint);
	Sprite::Get()->Draw(m_uiNumber[pLives], Vec2(1228.0f, 42.0f), numberScale, numberScale, numAncorPoint);
	//箱
	Sprite::Get()->Draw(m_boxGraph, Vec2(45.0f, 135.0f), 70.0f, 70.0f, numAncorPoint);

	if (m_breakRemain >= 1)
	{
		Sprite::Get()->Draw(m_uiNumber[m_breakRemain], Vec2(92.0f, 127.0f), numberScale * m_stagingScale[1], numberScale * m_stagingScale[1], numAncorPoint);
	}
	Sprite::Get()->Draw(m_uiNumber[m_breakNumber], Vec2(128.0f, 127.0f), numberScale * m_stagingScale[0], numberScale * m_stagingScale[0], numAncorPoint);

	Sprite::Get()->Draw(m_uiSlash, Vec2(160.0f, 132.0f), numberScale, numberScale, numAncorPoint);

	if (m_breakRemainMax >= 1)
	{
		Sprite::Get()->Draw(m_uiNumber[m_breakRemainMax], Vec2(192.0f, 137.0f), numberScale, numberScale, numAncorPoint);
	}
	Sprite::Get()->Draw(m_uiNumber[m_breakNumberMax], Vec2(224.0f, 137.0f), numberScale, numberScale, numAncorPoint);
	//魚ゲット数	
	Sprite::Get()->Draw(m_fishGraph, Vec2(51.5f, 37.0f), 83.0f, 54.0f, numAncorPoint);
	if (m_fishRemain >= 1)
	{
		Sprite::Get()->Draw(m_uiNumber[m_fishRemain], Vec2(108.0f, 32.0f), numberScale * m_stagingScale[3], numberScale * m_stagingScale[3], numAncorPoint);
	}
	Sprite::Get()->Draw(m_uiNumber[m_fishNumber], Vec2(138.0f, 32.0f), numberScale * m_stagingScale[2], numberScale * m_stagingScale[2], numAncorPoint);

	m_menu.Draw(gameoverFlag);
}

void UI::Reset()
{
	m_menuFlag = false;
	m_menu.Reset();
}

void UI::NumberDecision(const int fishNum)
{
	//数字を求めるのに使う数字
	const int numberSolution = 10;
	//壊した箱の数	
	int breaknumber = Stage::Get()->GetBlockNum() % numberSolution;
	if (m_breakNumber != breaknumber)
	{
		m_breakNumber = breaknumber;
		m_stagingTime[0] = c_stagingTimeMax;
		m_stagingFlag[0] = true;
	}
	int breakremain = Stage::Get()->GetBlockNum() / numberSolution;
	if (m_breakRemain != breakremain)
	{
		m_breakRemain = breakremain;
		m_stagingTime[1] = c_stagingTimeMax;
		m_stagingFlag[1] = true;
	}
	//最大箱の数
	m_breakNumberMax = Stage::Get()->GetBlockMax() % numberSolution;
	m_breakRemainMax = Stage::Get()->GetBlockMax() / numberSolution;
	//魚数
	int fishnumber = fishNum % numberSolution;
	if (m_fishNumber != fishnumber)
	{
		m_fishNumber = fishnumber;
		m_stagingTime[2] = c_stagingTimeMax;
		m_stagingFlag[2] = true;
	}
	int fishremain = fishNum / numberSolution;
	if (m_fishRemain != fishremain)
	{
		m_fishRemain = fishremain;
		m_stagingTime[3] = c_stagingTimeMax;
		m_stagingFlag[3] = true;
	}
}

void UI::NumberEmphasis()
{
	for (size_t i = 0; i < numberNum; i++)
	{
		if (m_stagingFlag[i] == false) { continue; }
		//大きさが変わるスピード
		const float scaleSpeed = 0.1f;
		m_stagingTime[i]--;
		if (c_stagingTimeMax / 2 < m_stagingTime[i])
		{
			m_stagingScale[i] += scaleSpeed;
			if (m_stagingScale[i] >= c_stagingScaleMax)
			{
				m_stagingScale[i] = c_stagingScaleMax;
			}
		}
		else
		{
			m_stagingScale[i] -= scaleSpeed;
			if (m_stagingScale[i] <= 1.0f)
			{
				m_stagingScale[i] = 1.0f;
			}
		}
		if (m_stagingTime <= 0)
		{
			m_stagingFlag[i] = false;
			m_stagingScale[i] = 1.0f;
		}
	}
}
