#include "Player.h"
#include<Shape.h>
#include<Collision.h>
#include"plate.h"
#include"Easing.h"
#include<random>
#include<Input.h>
#include "../slide puzzle/Score.h"
Player::Player()
{
}

Player::~Player()
{
}

void Player::Init()
{
	pObject = Shape::CreateOBJ("car");

	for (int i = 0; i < decoyObject_.size(); i++)
	{
		decoyObject_[i].object = Shape::CreateOBJ("car");
	}

	const Vec2 basePos = { -9.0f,6.0f };
	const Vec2 varPos = { 6.1f,-6.1f };

	position = Vec3(basePos.x + 0 * varPos.x, 1.5f, basePos.y + 1 * varPos.y + varPos.y / 2);
	positionMemory = position;

	comboParticle = std::make_unique<ParticleManager>();
	comboParticle->Initialize();

	particleGraph = Texture::Get()->LoadTexture(L"Resources/Paricle/kirakira.png");

	Score::GetInstance()->Reset();

	crashSound = Audio::Get()->SoundLoadWave("Resources/Sound/crash.wav");
	crystalSound = Audio::Get()->SoundLoadWave("Resources/Sound/crystal.wav");
	runSound = Audio::Get()->SoundLoadWave("Resources/Sound/move.wav");
	turboSound = Audio::Get()->SoundLoadWave("Resources/Sound/turbo.wav");
}

void Player::Update(Plate* plate)
{
	DirectChange(plate);
	Move();
	CrstalGet(plate);
	PointUpdate();
	DeadRotation();
	comboParticle->UpdateFollow(position);
}

void Player::Draw()
{
	for (int i = 0; i < decoyObject_.size(); i++)
	{
		if (decoyObject_[i].flag)
		{
			Object::Draw(decoyObject_[i].object, decoyObject_[i].position, Vec3(1.0f, 1.0f, 1.0f), decoyObject_[i].rotation);
		}
	}
	Object::Draw(pObject, position, Vec3(1.0f, 1.0f, 1.0f), rotation);
}

void Player::ScoreDraw()
{
	comboParticle->Draw(particleGraph);
	Score::GetInstance()->Draw();
}


void Player::PointUpdate()
{
	//コンボ
	if (Score::GetInstance()->GetComboTime() > 0)
	{
		comboParticle->BombAdd(Vec3(),
			0.0f, 0.5f, 0.0f,
			Vec4(0.3f, 0.3f, 0.0f, 0.3f), Vec4(0.0f, 0.0f, 0.0f, 0.0f));
	}

	Score::GetInstance()->Update();
}

void Player::DirectChange(Plate* plate)
{
	if (isDead == true) { return; }
	bool hit = false;
	Vec2 colPos = { position.x,position.z };
	if (isPosFlag == false && isPosSecondFlag == false)
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
	if (isPosFlag == false && isPosSecondFlag == false)
	{
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
					Audio::Get()->SoundRUNStop();
					Audio::Get()->SoundSEPlayWave(crashSound);
					break;
				case WIDTHSTRAIGHTLINE://横直線
					LineInit();
					TurnLeft(Vec2(plate->GetPanelPos(i).x, plate->GetPanelPos(i).z));
					TurnRight(Vec2(plate->GetPanelPos(i).x, plate->GetPanelPos(i).z));
					plateNumber = i;
					if (posEndSecond.x == 0.0f && posEndSecond.z == 0.0f && (direction == UP || direction == DOWN))
					{
						isDead = true;
						Audio::Get()->SoundRUNStop();
						Audio::Get()->SoundSEPlayWave(crashSound);

					}
					break;
				case HEIGHTSTRAIGHTLINE://縦直線
					LineInit();
					TurnUp(Vec2(plate->GetPanelPos(i).x, plate->GetPanelPos(i).z));
					TurnDown(Vec2(plate->GetPanelPos(i).x, plate->GetPanelPos(i).z));
					plateNumber = i;
					if (posEndSecond.x == 0.0f && posEndSecond.z == 0.0f && (direction == LEFT || direction == RIGHT))
					{
						isDead = true;
						Audio::Get()->SoundRUNStop();
						Audio::Get()->SoundSEPlayWave(crashSound);

					}
					break;
				case CROSS:            //十字
					LineInit();
					TurnLeft(Vec2(plate->GetPanelPos(i).x, plate->GetPanelPos(i).z));
					TurnRight(Vec2(plate->GetPanelPos(i).x, plate->GetPanelPos(i).z));
					TurnUp(Vec2(plate->GetPanelPos(i).x, plate->GetPanelPos(i).z));
					TurnDown(Vec2(plate->GetPanelPos(i).x, plate->GetPanelPos(i).z));
					plateNumber = i;
					break;
				case CUR_LEFTUP:       //カーブ左と上	
					TurnInit();
					TurnDownLeft(Vec2(plate->GetPanelPos(i).x, plate->GetPanelPos(i).z));
					TurnRightUp(Vec2(plate->GetPanelPos(i).x, plate->GetPanelPos(i).z));
					plateNumber = i;
					if (posEndSecond.x == 0.0f && posEndSecond.z == 0.0f && (direction == LEFT || direction == UP))
					{
						isDead = true;
						Audio::Get()->SoundRUNStop();
						Audio::Get()->SoundSEPlayWave(crashSound);

					}
					break;
				case CUR_LEFTDOWN:	   //カーブ左と下
					TurnInit();
					TurnRightDown(Vec2(plate->GetPanelPos(i).x, plate->GetPanelPos(i).z));
					TurnUpLeft(Vec2(plate->GetPanelPos(i).x, plate->GetPanelPos(i).z));
					plateNumber = i;
					if (posEndSecond.x == 0.0f && posEndSecond.z == 0.0f && (direction == LEFT || direction == DOWN))
					{
						isDead = true;
						Audio::Get()->SoundRUNStop();
						Audio::Get()->SoundSEPlayWave(crashSound);

					}
					break;
				case CUR_RIGHTUP:      //カーブ右と上
					TurnInit();
					TurnLeftUp(Vec2(plate->GetPanelPos(i).x, plate->GetPanelPos(i).z));
					TurnDownRight(Vec2(plate->GetPanelPos(i).x, plate->GetPanelPos(i).z));
					plateNumber = i;
					if (posEndSecond.x == 0.0f && posEndSecond.z == 0.0f && (direction == RIGHT || direction == UP))
					{
						isDead = true;
						Audio::Get()->SoundRUNStop();
						Audio::Get()->SoundSEPlayWave(crashSound);

					}
					break;
				case CUR_RIGHTDOWN:    //カーブ右と下
					TurnInit();
					TurnLeftDown(Vec2(plate->GetPanelPos(i).x, plate->GetPanelPos(i).z));
					TurnUpRight(Vec2(plate->GetPanelPos(i).x, plate->GetPanelPos(i).z));
					plateNumber = i;
					if (posEndSecond.x == 0.0f && posEndSecond.z == 0.0f && (direction == RIGHT || direction == DOWN))
					{
						isDead = true;
						Audio::Get()->SoundRUNStop();
						Audio::Get()->SoundSEPlayWave(crashSound);

					}
					break;
				case DCUR_LEFTUP_RIGHTDOWN://ダブルカーブ左と上＆右と下
					TurnInit();
					TurnDownLeft(Vec2(plate->GetPanelPos(i).x, plate->GetPanelPos(i).z));
					TurnRightUp(Vec2(plate->GetPanelPos(i).x, plate->GetPanelPos(i).z));
					TurnLeftDown(Vec2(plate->GetPanelPos(i).x, plate->GetPanelPos(i).z));
					TurnUpRight(Vec2(plate->GetPanelPos(i).x, plate->GetPanelPos(i).z));
					plateNumber = i;
					break;
				case DCUR_LEFTDOWN_RIGHTUP://ダブルカーブ左と下＆右と上
					TurnInit();
					TurnLeftUp(Vec2(plate->GetPanelPos(i).x, plate->GetPanelPos(i).z));
					TurnDownRight(Vec2(plate->GetPanelPos(i).x, plate->GetPanelPos(i).z));
					TurnRightDown(Vec2(plate->GetPanelPos(i).x, plate->GetPanelPos(i).z));
					TurnUpLeft(Vec2(plate->GetPanelPos(i).x, plate->GetPanelPos(i).z));
					plateNumber = i;
					break;
				default:
					break;
				}
				hit = true;

			}
		}
	}
	else
	{
		switch (plate->GetPanelStatus(plateNumber))
		{
		case NONE:
			isDead = true;
			break;
		case WIDTHSTRAIGHTLINE://横直線
			LineInit();
			TurnLeft(Vec2(plate->GetPanelPos(plateNumber).x, plate->GetPanelPos(plateNumber).z));
			TurnRight(Vec2(plate->GetPanelPos(plateNumber).x, plate->GetPanelPos(plateNumber).z));
			break;
		case HEIGHTSTRAIGHTLINE://縦直線
			LineInit();
			TurnUp(Vec2(plate->GetPanelPos(plateNumber).x, plate->GetPanelPos(plateNumber).z));
			TurnDown(Vec2(plate->GetPanelPos(plateNumber).x, plate->GetPanelPos(plateNumber).z));
			break;
		case CROSS:            //十字
			LineInit();
			TurnLeft(Vec2(plate->GetPanelPos(plateNumber).x, plate->GetPanelPos(plateNumber).z));
			TurnRight(Vec2(plate->GetPanelPos(plateNumber).x, plate->GetPanelPos(plateNumber).z));
			TurnUp(Vec2(plate->GetPanelPos(plateNumber).x, plate->GetPanelPos(plateNumber).z));
			TurnDown(Vec2(plate->GetPanelPos(plateNumber).x, plate->GetPanelPos(plateNumber).z));
			break;
		case CUR_LEFTUP:       //カーブ左と上	
			TurnInit();
			TurnDownLeft(Vec2(plate->GetPanelPos(plateNumber).x, plate->GetPanelPos(plateNumber).z));
			TurnRightUp(Vec2(plate->GetPanelPos(plateNumber).x, plate->GetPanelPos(plateNumber).z));
			break;
		case CUR_LEFTDOWN:	   //カーブ左と下
			TurnInit();
			TurnRightDown(Vec2(plate->GetPanelPos(plateNumber).x, plate->GetPanelPos(plateNumber).z));
			TurnUpLeft(Vec2(plate->GetPanelPos(plateNumber).x, plate->GetPanelPos(plateNumber).z));
			break;
		case CUR_RIGHTUP:      //カーブ右と上
			TurnInit();
			TurnLeftUp(Vec2(plate->GetPanelPos(plateNumber).x, plate->GetPanelPos(plateNumber).z));
			TurnDownRight(Vec2(plate->GetPanelPos(plateNumber).x, plate->GetPanelPos(plateNumber).z));
			break;
		case CUR_RIGHTDOWN:    //カーブ右と下
			TurnInit();
			TurnLeftDown(Vec2(plate->GetPanelPos(plateNumber).x, plate->GetPanelPos(plateNumber).z));
			TurnUpRight(Vec2(plate->GetPanelPos(plateNumber).x, plate->GetPanelPos(plateNumber).z));
			break;
		case DCUR_LEFTUP_RIGHTDOWN://ダブルカーブ左と上＆右と下
			TurnInit();
			TurnDownLeft(Vec2(plate->GetPanelPos(plateNumber).x, plate->GetPanelPos(plateNumber).z));
			TurnRightUp(Vec2(plate->GetPanelPos(plateNumber).x, plate->GetPanelPos(plateNumber).z));
			TurnLeftDown(Vec2(plate->GetPanelPos(plateNumber).x, plate->GetPanelPos(plateNumber).z));
			TurnUpRight(Vec2(plate->GetPanelPos(plateNumber).x, plate->GetPanelPos(plateNumber).z));
			break;
		case DCUR_LEFTDOWN_RIGHTUP://ダブルカーブ左と下＆右と上
			TurnInit();
			TurnLeftUp(Vec2(plate->GetPanelPos(plateNumber).x, plate->GetPanelPos(plateNumber).z));
			TurnDownRight(Vec2(plate->GetPanelPos(plateNumber).x, plate->GetPanelPos(plateNumber).z));
			TurnRightDown(Vec2(plate->GetPanelPos(plateNumber).x, plate->GetPanelPos(plateNumber).z));
			TurnUpLeft(Vec2(plate->GetPanelPos(plateNumber).x, plate->GetPanelPos(plateNumber).z));
			break;
		default:
			break;
		}
		hit = true;
	}
	//タイル外にいったら死亡
	if (hit == false)
	{
		isDead = true;
		Audio::Get()->SoundRUNStop();
		Audio::Get()->SoundSEPlayWave(crashSound);
	}
}

void Player::Move()
{
	if (isDead == true) { return; }

	Dash();

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
			Score::GetInstance()->CrstalGet(position);
			turnSpeed += speedPlas;
			Audio::Get()->SoundSEPlayWave(crystalSound);
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

void Player::DeadRotation()
{

	if (runSoundFlag == false)
	{
		runSoundFlag = true;
		Audio::Get()->SoundRUNPlayLoopWave(runSound);
	}

	if (isDead == false) { return; }

	rotation.z += 3.0f;
	if (rotation.z >= 180.0f)
	{
		rotation.z = 180.0f;
	}

}

void Player::Dash()
{

	if (Input::Get()->KeybordPush(DIK_LCONTROL) == true)
	{
		plateTime += 0.04f;
		if (turboSoundFlag == false)
		{
			Audio::Get()->SoundSEPlayWave(turboSound);
			turboSoundFlag = true;
		}
	}
	else
	{
		turboSoundFlag = false;
	}

}

void Player::DecoyUpdate()
{
	for (int i = 0; i < decoyObject_.size(); i++)
	{
		if (decoyObject_[i].flag)
		{
			if (decoyObject_[i].timer > 20)
			{
				decoyObject_[i].flag = false;
				decoyObject_[i].timer = 0;
			}
			decoyObject_[i].timer++;
		}
	}

	if (!turboSoundFlag) { return; }

	if (decoyTimer_ > 10)
	{
		for (int i = 0; i < decoyObject_.size(); i++)
		{
			if (!decoyObject_[i].flag)
			{
				decoyObject_[i].position = position;
				decoyObject_[i].rotation = rotation;
				decoyObject_[i].flag = true;
				break;
			}
		}
		decoyTimer_ = 0;
	}

	decoyTimer_++;
}

