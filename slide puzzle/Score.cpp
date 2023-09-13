#include "Score.h"
#include<Shape.h>

Score* Score::GetInstance()
{
	static Score instance;
	return &instance;
}

void Score::Initialize()
{
	scoreDraw = Shape::CreateRect(1.0f, 1.0f);
	backSprite = Sprite::Get()->SpriteCreate(L"Resources/scoreBack.png");
	comboSprite = Sprite::Get()->SpriteCreate(L"Resources/combo.png");
	numberGraph[0] = Sprite::Get()->SpriteCreate(L"Resources/number/0.png");
	numberGraph[1] = Sprite::Get()->SpriteCreate(L"Resources/number/1.png");
	numberGraph[2] = Sprite::Get()->SpriteCreate(L"Resources/number/2.png");
	numberGraph[3] = Sprite::Get()->SpriteCreate(L"Resources/number/3.png");
	numberGraph[4] = Sprite::Get()->SpriteCreate(L"Resources/number/4.png");
	numberGraph[5] = Sprite::Get()->SpriteCreate(L"Resources/number/5.png");
	numberGraph[6] = Sprite::Get()->SpriteCreate(L"Resources/number/6.png");
	numberGraph[7] = Sprite::Get()->SpriteCreate(L"Resources/number/7.png");
	numberGraph[8] = Sprite::Get()->SpriteCreate(L"Resources/number/8.png");
	numberGraph[9] = Sprite::Get()->SpriteCreate(L"Resources/number/9.png");

	comboGraph[0] = Sprite::Get()->SpriteCreate(L"Resources/number/combo0.png");
	comboGraph[1] = Sprite::Get()->SpriteCreate(L"Resources/number/combo1.png");
	comboGraph[2] = Sprite::Get()->SpriteCreate(L"Resources/number/combo2.png");
	comboGraph[3] = Sprite::Get()->SpriteCreate(L"Resources/number/combo3.png");
	comboGraph[4] = Sprite::Get()->SpriteCreate(L"Resources/number/combo4.png");
	comboGraph[5] = Sprite::Get()->SpriteCreate(L"Resources/number/combo5.png");
	comboGraph[6] = Sprite::Get()->SpriteCreate(L"Resources/number/combo6.png");
	comboGraph[7] = Sprite::Get()->SpriteCreate(L"Resources/number/combo7.png");
	comboGraph[8] = Sprite::Get()->SpriteCreate(L"Resources/number/combo8.png");
	comboGraph[9] = Sprite::Get()->SpriteCreate(L"Resources/number/combo9.png");
	comboGraph[10] = Sprite::Get()->SpriteCreate(L"Resources/number/plus.png");
}

void Score::Finalize()
{
}

void Score::Update()
{
	//コンボ
	if (comboTime > 0)
	{
		comboTime--;
	}
	if (comboTime == 0) { pointPlas = 10; }
	//ポイント
	if (pointPlas > 100) { pointPlas = 100; }
	if (pointNum > 99999) { pointNum = 99999; }
	//スコア
	if (scoreDrawTime > 0)
	{
		scoreDrawTime--;
	}
	else { isScoreDraw = false; }
}

void Score::Draw(Vec2 pos)
{
	//3D
	if (isScoreDraw == true)
	{
		int pointPlasNum[3] = {};
		Object::Draw(scoreDraw, scorePos + Vec3(0.0f, 0.0f, 3.0f),
			Vec3(5.5f, 1.1f, 1.1f), Vec3(90.0f, 0.0f, 0.0f), comboSprite.texNumber);
		Object::Draw(scoreDraw, scorePos + Vec3(-2.0f, 0.0f, 1.5f),
			Vec3(1.0f, 1.0f, 1.0f), Vec3(90.0f, 0.0f, 0.0f), comboGraph[10].texNumber);
		int point = 0;
		if (pointPlas != 100) { point = pointPlas - 10; }
		else { point = pointPlas; }
		pointPlasNum[0] = point % 10;
		pointPlasNum[1] = point % 100 / 10;
		pointPlasNum[2] = point % 1000 / 100;
		Vec3 plasPos = { 1.0f,0.0f,1.5f };
		for (int i = 0; i < 3; i++)
		{
			if (i == 2 && point != 100) { break; }
			Object::Draw(scoreDraw, scorePos + plasPos,
				Vec3(1.0f, 1.0f, 1.0f), Vec3(90.0f, 0.0f, 0.0f), comboGraph[pointPlasNum[i]].texNumber);
			plasPos += Vec3(-1.0f, 0.0f, 0.0f);
		}
	}
	int pointNumber[5] = {};
	pointNumber[0] = pointNum % 10;
	pointNumber[1] = pointNum % 100 / 10;
	pointNumber[2] = pointNum % 1000 / 100;
	pointNumber[3] = pointNum % 10000 / 1000;
	pointNumber[4] = pointNum % 100000 / 10000;
	//2D描画
	Sprite::Get()->Draw(backSprite, pos, 250, 80);
	Vec2 posPlas = { 40.0f,0.0f };
	int j = 0;
	pos.x += 15.0f;
	pos.y += 10.0f;
	for (int i = 4; i > -1; i--)
	{
		Sprite::Get()->Draw(numberGraph[pointNumber[i]], pos + posPlas * static_cast<float>(j), 60, 60);
		j++;
	}
}

void Score::Reset()
{
	comboTime = 0;
	pointNum = 0;
	scoreDrawTime = 0;
	pointPlas = 10;
	isScoreDraw = false;
}

void Score::CrstalGet(Vec3 position)
{
	pointNum += pointPlas;
	pointPlas += 10;
	comboTime = comboTimeMax;
	isScoreDraw = true;
	scorePos = position;
	scoreDrawTime = scoreDrawTimeMax;
}
