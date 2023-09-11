#include "Player.h"
#include<Shape.h>
#include<Collision.h>
#include"plate.h"
#include"Easing.h"
Player::Player()
{
}

Player::~Player()
{
}

void Player::Init()
{
	pObject = Shape::CreateOBJ("car");

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

	const Vec2 basePos = { -9.0f,6.0f };
	const Vec2 varPos = { 6.1f,-6.1f };

	position = Vec3(basePos.x + 0 * varPos.x, 1.5f, basePos.y + 1 * varPos.y + varPos.y / 2);
	positionMemory = position;
}

void Player::Update(Plate* plate)
{
	DirectChange(plate);
	Move();
	CrstalGet(plate);
	PointUpdate();
}

void Player::Draw()
{

	Object::Draw(pObject, position, Vec3(1.0f, 1.0f, 1.0f), rotation);
}

void Player::ScoreDraw()
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
	Sprite::Get()->Draw(backSprite, Vec2(), 250, 80);
	Vec2 posPlas = { 40.0f,0.0f };
	int j = 0;
	for (int i = 4; i > -1; i--)
	{
		Sprite::Get()->Draw(numberGraph[pointNumber[i]], Vec2() + posPlas * static_cast<float>(j), 60, 60);
		j++;
	}
}


void Player::PointUpdate()
{
	//コンボ
	if (comboTime > 0) { comboTime--; }
	if (comboTime == 0) { pointPlas = 10; }
	//ポイント
	if (pointPlas > 100) { pointPlas = 100; }
	if (pointNum > 99999) { pointNum = 99999; }
	//スコア
	if (scoreDrawTime > 0) { scoreDrawTime--; }
	else { isScoreDraw = false; }
}

void Player::DirectChange(Plate* plate)
{
	bool hit = false;
	Vec2 colPos = { position.x,position.z };
	if (isPosFlag == false && isPosSecondFlag == false && isDead == false)
	{
		if (direction == UP)
		{
			colPos.y += 0.5f;
		}
		if (direction == DOWN)
		{
			colPos.y -= 0.5f;
		}
		if (direction == RIGHT)
		{
			colPos.x += 0.5f;
		}
		if (direction == LEFT)
		{
			colPos.x -= 0.5f;
		}
	}
	for (int i = 0; i < plate->GetPanelNum(); i++)
	{
		//当たり判定
		if (Collision::BoxCollision(
			colPos, Vec2(plate->GetPanelPos(i).x, plate->GetPanelPos(i).z),
			pSize / 2, sSize / 2) && isDead == false)
		{
			switch (plate->GetPanelStatus(i))
			{
			case NONE:
				isDead = true;
				break;
			case WIDTHSTRAIGHTLINE://横直線
				LineInit();
				TurnLeft(Vec2(plate->GetPanelPos(i).x, plate->GetPanelPos(i).z));
				TurnRight(Vec2(plate->GetPanelPos(i).x, plate->GetPanelPos(i).z));
				if (posEndSecond.x == 0.0f && posEndSecond.z == 0.0f && (direction == UP || direction == DOWN))
				{
					isDead = true;
				}
				break;
			case HEIGHTSTRAIGHTLINE://縦直線
				LineInit();
				TurnUp(Vec2(plate->GetPanelPos(i).x, plate->GetPanelPos(i).z));
				TurnDown(Vec2(plate->GetPanelPos(i).x, plate->GetPanelPos(i).z));
				if (posEndSecond.x == 0.0f && posEndSecond.z == 0.0f && (direction == LEFT || direction == RIGHT))
				{
					isDead = true;
				}
				break;
			case CROSS:            //十字
				LineInit();
				TurnLeft(Vec2(plate->GetPanelPos(i).x, plate->GetPanelPos(i).z));
				TurnRight(Vec2(plate->GetPanelPos(i).x, plate->GetPanelPos(i).z));
				TurnUp(Vec2(plate->GetPanelPos(i).x, plate->GetPanelPos(i).z));
				TurnDown(Vec2(plate->GetPanelPos(i).x, plate->GetPanelPos(i).z));
				break;
			case CUR_LEFTUP:       //カーブ左と上	
				TurnInit();
				TurnDownLeft(Vec2(plate->GetPanelPos(i).x, plate->GetPanelPos(i).z));
				TurnRightUp(Vec2(plate->GetPanelPos(i).x, plate->GetPanelPos(i).z));
				if (posEndSecond.x == 0.0f && posEndSecond.z == 0.0f && (direction == LEFT || direction == UP))
				{
					isDead = true;
				}
				break;
			case CUR_LEFTDOWN:	   //カーブ左と下
				TurnInit();
				TurnRightDown(Vec2(plate->GetPanelPos(i).x, plate->GetPanelPos(i).z));
				TurnUpLeft(Vec2(plate->GetPanelPos(i).x, plate->GetPanelPos(i).z));
				if (posEndSecond.x == 0.0f && posEndSecond.z == 0.0f && (direction == LEFT || direction == DOWN))
				{
					isDead = true;
				}
				break;
			case CUR_RIGHTUP:      //カーブ右と上
				TurnInit();
				TurnLeftUp(Vec2(plate->GetPanelPos(i).x, plate->GetPanelPos(i).z));
				TurnDownRight(Vec2(plate->GetPanelPos(i).x, plate->GetPanelPos(i).z));
				if (posEndSecond.x == 0.0f && posEndSecond.z == 0.0f && (direction == RIGHT || direction == UP))
				{
					isDead = true;
				}
				break;
			case CUR_RIGHTDOWN:    //カーブ右と下
				TurnInit();
				TurnLeftDown(Vec2(plate->GetPanelPos(i).x, plate->GetPanelPos(i).z));
				TurnUpRight(Vec2(plate->GetPanelPos(i).x, plate->GetPanelPos(i).z));
				if (posEndSecond.x == 0.0f && posEndSecond.z == 0.0f && (direction == RIGHT || direction == DOWN))
				{
					isDead = true;
				}
				break;
			case DCUR_LEFTUP_RIGHTDOWN://ダブルカーブ左と上＆右と下
				TurnInit();
				TurnDownLeft(Vec2(plate->GetPanelPos(i).x, plate->GetPanelPos(i).z));
				TurnRightUp(Vec2(plate->GetPanelPos(i).x, plate->GetPanelPos(i).z));
				TurnLeftDown(Vec2(plate->GetPanelPos(i).x, plate->GetPanelPos(i).z));
				TurnUpRight(Vec2(plate->GetPanelPos(i).x, plate->GetPanelPos(i).z));
				break;
			case DCUR_LEFTDOWN_RIGHTUP://ダブルカーブ左と下＆右と上
				TurnInit();
				TurnLeftUp(Vec2(plate->GetPanelPos(i).x, plate->GetPanelPos(i).z));
				TurnDownRight(Vec2(plate->GetPanelPos(i).x, plate->GetPanelPos(i).z));
				TurnRightDown(Vec2(plate->GetPanelPos(i).x, plate->GetPanelPos(i).z));
				TurnUpLeft(Vec2(plate->GetPanelPos(i).x, plate->GetPanelPos(i).z));
				break;
			default:
				break;
			}
			hit = true;

		}
	}
	//タイル外にいったら死亡
	if (hit == false)
	{
		isDead = true;
	}
}

void Player::Move()
{
	if (isDead == true) { return; }

	if (isPosFlag == true)
	{
		//移動の処理
		position = Easing::lerp(posStart, posEnd, plateTime);
		//プレイヤーが回転する処理
		if (isCurFlag == true)
		{
			rotation = Easing::lerp(turnStart, turnEnd, plateTime);
		}
		plateTime += turnSpeed;
		//移動終わったら
		if (plateTime >= 1.0f)
		{
			//posStartSecond = posEnd;
			turnStartSecond = rotation;
			isPosFlag = false;
			plateTime = 0.0f;
		}
	}
	else if (isPosSecondFlag == true)
	{
		//移動の処理
		position = Easing::lerp(posStartSecond, posEndSecond, plateTime);
		//プレイヤーが回転する処理
		if (isCurFlag == true)
		{
			rotation = Easing::lerp(turnStartSecond, turnEndSecond, plateTime);
		}
		plateTime += turnSpeed;
		//移動終わったら
		if (plateTime >= 1.0f)
		{
			isPosSecondFlag = false;
			isCurFlag = false;
			plateTime = 0.0f;
			rotation = turnEndSecond;
			position = posEndSecond;
		}
	}

	if (turnSpeed > speedMax)
	{
		turnSpeed = speedMax;
	}
}

void Player::CrstalGet(Plate* plate)
{
	const Vec2 crstalSize = { 5.0f,5.0f };
	for (int i = 0; i < plate->GetPanelNum(); i++)
	{
		//当たり判定
		if (plate->GetCrystal(i) == CRYSTALL && Collision::BoxCollision(
			Vec2(position.x, position.z), Vec2(plate->GetPanelPos(i).x, plate->GetPanelPos(i).z),
			pSize / 2, crstalSize / 2))
		{
			pointNum += pointPlas;
			pointPlas += 10;
			comboTime = comboTimeMax;
			turnSpeed += speedPlas;
			isScoreDraw = true;
			scorePos = position;
			scoreDrawTime = scoreDrawTimeMax;
			//クリスタルが消える処理
			plate->DeleteCrstal(i);
		}
	}
}

void Player::TurnInit()
{
	if (isPosFlag == true || isPosSecondFlag == true) { return; }
	turnStart = rotation;
	isCurFlag = true;
	isPosFlag = true, isPosSecondFlag = true;
	posEndSecond = {};
}

void Player::LineInit()
{
	if (isPosFlag == true || isPosSecondFlag == true) { return; }
	isPosFlag = true, isPosSecondFlag = true;
	posEndSecond = {};
}

void Player::TurnLeft(const Vec2 platePos)
{
	if (direction != LEFT) { return; }
	if (posEndSecond.x == 0.0f && posEndSecond.y == 0.0f && posEndSecond.z == 0.0f)
	{
		keepDirection = LEFT;
	}
	if (keepDirection == LEFT)
	{
		posStart = Vec3(platePos.x + varPos.x / 2 - 0.05f, positionMemory.y, platePos.y);
		posEnd = Vec3(platePos.x, position.y, platePos.y);
		posStartSecond = posEnd;
		posEndSecond = Vec3(platePos.x - varPos.x / 2, position.y, platePos.y);
		positionMemory = posEndSecond;
	}
}

void Player::TurnRight(const Vec2 platePos)
{
	if (direction != RIGHT) { return; }
	if (posEndSecond.x == 0.0f && posEndSecond.y == 0.0f && posEndSecond.z == 0.0f)
	{
		keepDirection = RIGHT;
	}
	if (keepDirection == RIGHT)
	{
		posStart = Vec3(platePos.x - varPos.x / 2 + 0.05f, positionMemory.y, platePos.y);
		posEnd = Vec3(platePos.x, position.y, platePos.y);
		posStartSecond = posEnd;
		posEndSecond = Vec3(platePos.x + varPos.x / 2, position.y, platePos.y);
		positionMemory = posEndSecond;
	}
}

void Player::TurnUp(const Vec2 platePos)
{
	if (direction != UP) { return; }
	if (posEndSecond.x == 0.0f && posEndSecond.y == 0.0f && posEndSecond.z == 0.0f)
	{
		keepDirection = UP;
	}
	if (keepDirection == UP)
	{
		posStart = Vec3(platePos.x, positionMemory.y, platePos.y - varPos.y / 2 + 0.05f);
		posEnd = Vec3(platePos.x, position.y, platePos.y);
		posStartSecond = posEnd;
		posEndSecond = Vec3(platePos.x, position.y, platePos.y + varPos.y / 2);
		positionMemory = posEndSecond;
	}
}

void Player::TurnDown(const Vec2 platePos)
{
	if (direction != DOWN) { return; }
	if (posEndSecond.x == 0.0f && posEndSecond.y == 0.0f && posEndSecond.z == 0.0f)
	{
		keepDirection = DOWN;
	}
	if (keepDirection == DOWN)
	{
		posStart = Vec3(platePos.x, positionMemory.y, platePos.y + varPos.y / 2 - 0.05f);
		posEnd = Vec3(platePos.x, position.y, platePos.y);
		posStartSecond = posEnd;
		posEndSecond = Vec3(platePos.x, position.y, platePos.y - varPos.y / 2);
		positionMemory = posEndSecond;
	}
}

void Player::TurnLeftDown(const Vec2 platePos)
{
	if (posEndSecond.x == 0.0f && posEndSecond.y == 0.0f && posEndSecond.z == 0.0f)
	{
		if (direction != LEFT) { return; }
		keepDirection = LEFT;
		direction = DOWN;
		turnEnd = Vec3(rotation.x, rotation.y - 45.0f, rotation.z);
		turnEndSecond = Vec3(rotation.x, rotation.y - 90.0f, rotation.z);
	}
	if (keepDirection == LEFT)
	{
		posStart = Vec3(platePos.x + varPos.x / 2 - 0.05f, positionMemory.y, platePos.y);
		posEnd = Vec3(platePos.x + varPos.x / 4, position.y, platePos.y - varPos.y / 8);
		posStartSecond = posEnd;
		posEndSecond = Vec3(platePos.x, position.y, platePos.y - varPos.y / 2);
		positionMemory = posEndSecond;
	}
}

void Player::TurnUpRight(const Vec2 platePos)
{
	if (posEndSecond.x == 0.0f && posEndSecond.y == 0.0f && posEndSecond.z == 0.0f)
	{
		if (direction != UP) { return; }
		keepDirection = UP;
		direction = RIGHT;
		turnEnd = Vec3(rotation.x, rotation.y + 45.0f, rotation.z);
		turnEndSecond = Vec3(rotation.x, rotation.y + 90.0f, rotation.z);
	}
	if (keepDirection == UP)
	{
		posStart = Vec3(platePos.x, positionMemory.y, platePos.y - varPos.y / 2 + 0.05f);
		posEnd = Vec3(platePos.x + varPos.x / 8, position.y, platePos.y - varPos.y / 4);
		posStartSecond = posEnd;
		posEndSecond = Vec3(platePos.x + varPos.x / 2, position.y, platePos.y);
		positionMemory = posEndSecond;
	}
}

void Player::TurnRightDown(const Vec2 platePos)
{
	if (posEndSecond.x == 0.0f && posEndSecond.y == 0.0f && posEndSecond.z == 0.0f)
	{
		if (direction != RIGHT) { return; }
		keepDirection = RIGHT;
		direction = DOWN;
		turnEnd = Vec3(rotation.x, rotation.y + 45.0f, rotation.z);
		turnEndSecond = Vec3(rotation.x, rotation.y + 90.0f, rotation.z);
	}
	if (keepDirection == RIGHT)
	{
		posStart = Vec3(platePos.x - varPos.x / 2 + 0.05f, positionMemory.y, platePos.y);
		posEnd = Vec3(platePos.x - varPos.x / 4, position.y, platePos.y - varPos.y / 8);
		posStartSecond = posEnd;
		posEndSecond = Vec3(platePos.x, position.y, platePos.y - varPos.y / 2);
		positionMemory = posEndSecond;
	}

}

void Player::TurnUpLeft(const Vec2 platePos)
{
	if (posEndSecond.x == 0.0f && posEndSecond.y == 0.0f && posEndSecond.z == 0.0f)
	{
		if (direction != UP) { return; }
		keepDirection = UP;
		direction = LEFT;
		turnEnd = Vec3(rotation.x, rotation.y - 45.0f, rotation.z);
		turnEndSecond = Vec3(rotation.x, rotation.y - 90.0f, rotation.z);
	}
	if (keepDirection == UP)
	{
		posStart = Vec3(platePos.x, positionMemory.y, platePos.y - varPos.y / 2 + 0.05f);
		posEnd = Vec3(platePos.x - varPos.x / 8, position.y, platePos.y - varPos.y / 4);
		posStartSecond = posEnd;
		posEndSecond = Vec3(platePos.x - varPos.x / 2, position.y, platePos.y);
		positionMemory = posEndSecond;
	}
}

void Player::TurnDownLeft(const Vec2 platePos)
{
	if (posEndSecond.x == 0.0f && posEndSecond.y == 0.0f && posEndSecond.z == 0.0f)
	{
		if (direction != DOWN) { return; }
		keepDirection = DOWN;
		direction = LEFT;
		turnEnd = Vec3(rotation.x, rotation.y + 45.0f, rotation.z);
		turnEndSecond = Vec3(rotation.x, rotation.y + 90.0f, rotation.z);
	}
	if (keepDirection == DOWN)
	{
		posStart = Vec3(platePos.x, positionMemory.y, platePos.y + varPos.y / 2 - 0.01f);
		posEnd = Vec3(platePos.x - varPos.x / 8, position.y, platePos.y + varPos.y / 4);
		posStartSecond = posEnd;
		posEndSecond = Vec3(platePos.x - varPos.x / 2, position.y, platePos.y);
		positionMemory = posEndSecond;
	}
}

void Player::TurnRightUp(const Vec2 platePos)
{
	if (posEndSecond.x == 0.0f && posEndSecond.y == 0.0f && posEndSecond.z == 0.0f)
	{
		if (direction != RIGHT) { return; }
		keepDirection = RIGHT;
		direction = UP;
		turnEnd = Vec3(rotation.x, rotation.y - 45.0f, rotation.z);
		turnEndSecond = Vec3(rotation.x, rotation.y - 90.0f, rotation.z);
	}
	if (keepDirection == RIGHT)
	{
		posStart = Vec3(platePos.x - varPos.x / 2 + 0.01f, positionMemory.y, platePos.y);
		posEnd = Vec3(platePos.x - varPos.x / 4, position.y, platePos.y + varPos.y / 8);
		posStartSecond = posEnd;
		posEndSecond = Vec3(platePos.x, position.y, platePos.y + varPos.y / 2);
		positionMemory = posEndSecond;
	}
}

void Player::TurnLeftUp(const Vec2 platePos)
{
	if (posEndSecond.x == 0.0f && posEndSecond.y == 0.0f && posEndSecond.z == 0.0f)
	{
		if (direction != LEFT) { return; }
		keepDirection = LEFT;
		direction = UP;
		turnEnd = Vec3(rotation.x, rotation.y + 45.0f, rotation.z);
		turnEndSecond = Vec3(rotation.x, rotation.y + 90.0f, rotation.z);
	}
	if (keepDirection == LEFT)
	{
		posStart = Vec3(platePos.x + varPos.x / 2 - 0.01f, positionMemory.y, platePos.y);
		posEnd = Vec3(platePos.x + varPos.x / 4, position.y, platePos.y + varPos.y / 8);
		posStartSecond = posEnd;
		posEndSecond = Vec3(platePos.x, position.y, platePos.y + varPos.y / 2);
		positionMemory = posEndSecond;
	}
}

void Player::TurnDownRight(const Vec2 platePos)
{
	if (posEndSecond.x == 0.0f && posEndSecond.y == 0.0f && posEndSecond.z == 0.0f)
	{
		if (direction != DOWN) { return; }
		keepDirection = DOWN;
		direction = RIGHT;
		turnEnd = Vec3(rotation.x, rotation.y - 45.0f, rotation.z);
		turnEndSecond = Vec3(rotation.x, rotation.y - 90.0f, rotation.z);
	}
	if (keepDirection == DOWN)
	{
		posStart = Vec3(platePos.x, positionMemory.y, platePos.y + varPos.y / 2 - 0.01f);
		posEnd = Vec3(platePos.x + varPos.x / 8, position.y, platePos.y + varPos.y / 4);
		posStartSecond = posEnd;
		posEndSecond = Vec3(platePos.x + varPos.x / 2, position.y, platePos.y);
		positionMemory = posEndSecond;
	}
}

