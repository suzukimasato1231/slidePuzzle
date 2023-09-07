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

	const Vec2 basePos = { -9.0f,6.0f };
	const Vec2 varPos = { 6.1f,-6.1f };

	position = Vec3(basePos.x + 0 * varPos.x, 1.5f, basePos.y + 1 * varPos.y + varPos.y / 2);
	positionMemory = position;
}

void Player::Update(Plate* plate)
{
	DirectChange(plate);
	Move();
}

void Player::Draw()
{

	Object::Draw(pObject, position, Vec3(1.0f, 1.0f, 1.0f), rotation);
}

void Player::DirectChange(Plate* plate)
{
	bool hit = false;
	if (isPosFlag == false && isPosSecondFlag == false)
	{
		if (direction == UP)
		{
			position.z += 0.2f;
		}
		if (direction == DOWN)
		{
			position.z -= 0.2f;
		}
		if (direction == RIGHT)
		{
			position.x += 0.2f;
		}
		if (direction == LEFT)
		{
			position.x -= 0.2f;
		}
	}
	for (int j = 0; j < 3; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			//�����蔻��
			if (Collision::BoxCollision(
				Vec2(position.x, position.z), Vec2(basePos.x + i * varPos.x, basePos.y + j * -varPos.y),
				pSize / 2, sSize / 2))
			{
				switch (plate->GetStage(i, j))
				{
				case NONE:
					isDead = true;
					break;
				case WIDTHSTRAIGHTLINE://������
					LineInit();
					TurnLeft();
					TurnRight();
					break;
				case HEIGHTSTRAIGHTLINE://�c����
					LineInit();
					TurnUp();
					TurnDown();
					break;
				case CROSS:            //�\��
					LineInit();
					TurnLeft();
					TurnRight();
					TurnUp();
					TurnDown();
					break;
				case CUR_LEFTUP:       //�J�[�u���Ə�	
					TurnInit();
					TurnDownLeft();
					TurnRightUp();
					break;
				case CUR_LEFTDOWN:	   //�J�[�u���Ɖ�
					TurnInit();
					TurnRightDown();
					TurnUpLeft();
					break;
				case CUR_RIGHTUP:      //�J�[�u�E�Ə�
					TurnInit();
					TurnLeftUp();
					TurnDownRight();
					break;
				case CUR_RIGHTDOWN:    //�J�[�u�E�Ɖ�
					TurnInit();
					TurnLeftDown();
					TurnUpRight();
					break;
				case DCUR_LEFTUP_RIGHTDOWN://�_�u���J�[�u���Əさ�E�Ɖ�
					TurnInit();
					TurnDownLeft();
					TurnRightUp();
					TurnLeftDown();
					TurnUpRight();
					break;
				case DCUR_LEFTDOWN_RIGHTUP://�_�u���J�[�u���Ɖ����E�Ə�
					TurnInit();
					TurnLeftUp();
					TurnDownRight();
					TurnRightDown();
					TurnUpLeft();
					break;
				default:
					break;
				}
				hit = true;
			}
		}
	}
	//�^�C���O�ɂ������玀�S
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
		//�ړ��̏���
		position = Easing::lerp(posStart, posEnd, plateTime);
		//�v���C���[����]���鏈��
		if (isCurFlag == true)
		{
			rotation = Easing::lerp(turnStart, turnEnd, plateTime);
		}
		plateTime += turnSpeed;
		//�ړ��I�������
		if (plateTime >= 1.0f)
		{
			posStartSecond = position;
			turnStartSecond = rotation;
			isPosFlag = false;
			plateTime = 0.0f;
		}
	}
	else if (isPosSecondFlag == true)
	{
		//�ړ��̏���
		position = Easing::lerp(posStartSecond, posEndSecond, plateTime);
		//�v���C���[����]���鏈��
		if (isCurFlag == true)
		{
			rotation = Easing::lerp(turnStartSecond, turnEndSecond, plateTime);
		}
		plateTime += turnSpeed;
		//�ړ��I�������
		if (plateTime >= 1.0f)
		{
			isPosSecondFlag = false;
			isCurFlag = false;
			plateTime = 0.0f;
		}
	}
}

void Player::TurnInit()
{
	if (isPosFlag == true || isPosSecondFlag == true) { return; }
	posStart = position;
	turnStart = rotation;
	isCurFlag = true;
	isPosFlag = true, isPosSecondFlag = true;
	posEndSecond = {};
}

void Player::LineInit()
{
	if (isPosFlag == true || isPosSecondFlag == true) { return; }
	posStart = position;
	isPosFlag = true, isPosSecondFlag = true;
	posEndSecond = {};
}

void Player::TurnLeft()
{
	if (direction != LEFT) { return; }
	if (posEndSecond.x == 0.0f && posEndSecond.y == 0.0f && posEndSecond.z == 0.0f)
	{
		position = positionMemory;
		posEnd = Vec3(position.x - varPos.x / 2, position.y, position.z);
		posEndSecond = Vec3(position.x - varPos.x, position.y, position.z);
		positionMemory = posEndSecond;
	}
}

void Player::TurnRight()
{
	if (direction != RIGHT) { return; }
	if (posEndSecond.x == 0.0f && posEndSecond.y == 0.0f && posEndSecond.z == 0.0f)
	{
		position = positionMemory;
		posEnd = Vec3(position.x + varPos.x / 2, position.y, position.z);
		posEndSecond = Vec3(position.x + varPos.x, position.y, position.z);
		positionMemory = posEndSecond;
	}
}

void Player::TurnUp()
{
	if (direction != UP) { return; }
	if (posEndSecond.x == 0.0f && posEndSecond.y == 0.0f && posEndSecond.z == 0.0f)
	{
		position = positionMemory;
		posEnd = Vec3(position.x, position.y, position.z + varPos.y / 2);
		posEndSecond = Vec3(position.x, position.y, position.z + varPos.y);
		positionMemory = posEndSecond;
	}
}

void Player::TurnDown()
{
	if (direction != DOWN) { return; }
	if (posEndSecond.x == 0.0f && posEndSecond.y == 0.0f && posEndSecond.z == 0.0f)
	{
		position = positionMemory;
		posEnd = Vec3(position.x, position.y, position.z - varPos.y / 2);
		posEndSecond = Vec3(position.x, position.y, position.z - varPos.y);
		positionMemory = posEndSecond;
	}
}

void Player::TurnLeftDown()
{
	if (direction != LEFT) { return; }
	if (posEndSecond.x == 0.0f && posEndSecond.y == 0.0f && posEndSecond.z == 0.0f)
	{
		position = positionMemory;
		direction = DOWN;
		turnEnd = Vec3(rotation.x, rotation.y - 45.0f, rotation.z);
		turnEndSecond = Vec3(rotation.x, rotation.y - 90.0f, rotation.z);
		posEnd = Vec3(position.x - varPos.x / 4, position.y, position.z - varPos.y / 4);
		posEndSecond = Vec3(position.x - varPos.x / 2, position.y, position.z - varPos.y / 2);
		positionMemory = posEndSecond;
	}
}

void Player::TurnUpRight()
{
	if (direction != UP) { return; }
	if (posEndSecond.x == 0.0f && posEndSecond.y == 0.0f && posEndSecond.z == 0.0f)
	{
		direction = RIGHT;
		position = positionMemory;
		turnEnd = Vec3(rotation.x, rotation.y + 45.0f, rotation.z);
		turnEndSecond = Vec3(rotation.x, rotation.y + 90.0f, rotation.z);
		posEnd = Vec3(position.x + varPos.x / 4, position.y, position.z + varPos.y / 4);
		posEndSecond = Vec3(position.x + varPos.x / 2, position.y, position.z + varPos.y / 2);
		positionMemory = posEndSecond;
	}
}

void Player::TurnRightDown()
{
	if (direction != RIGHT) { return; }
	if (posEndSecond.x == 0.0f && posEndSecond.y == 0.0f && posEndSecond.z == 0.0f)
	{
		direction = DOWN;
		position = positionMemory;
		turnEnd = Vec3(rotation.x, rotation.y + 45.0f, rotation.z);
		turnEndSecond = Vec3(rotation.x, rotation.y + 90.0f, rotation.z);

		posEnd = Vec3(position.x + varPos.x / 4, position.y, position.z - varPos.y / 4);
		posEndSecond = Vec3(position.x + varPos.x / 2, position.y, position.z - varPos.y / 2);
		positionMemory = posEndSecond;
	}
}

void Player::TurnUpLeft()
{
	if (direction != UP) { return; }
	if (posEndSecond.x == 0.0f && posEndSecond.y == 0.0f && posEndSecond.z == 0.0f)
	{
		direction = LEFT;
		position = positionMemory;
		turnEnd = Vec3(rotation.x, rotation.y - 45.0f, rotation.z);
		turnEndSecond = Vec3(rotation.x, rotation.y - 90.0f, rotation.z);
		posEnd = Vec3(position.x - varPos.x / 4, position.y, position.z + varPos.y / 4);
		posEndSecond = Vec3(position.x - varPos.x / 2, position.y, position.z + varPos.y / 2);
		positionMemory = posEndSecond;
	}
}

void Player::TurnDownLeft()
{
	if (direction != DOWN) { return; }
	if (posEndSecond.x == 0.0f && posEndSecond.y == 0.0f && posEndSecond.z == 0.0f)
	{
		direction = LEFT;
		position = positionMemory;
		turnEnd = Vec3(rotation.x, rotation.y + 45.0f, rotation.z);
		turnEndSecond = Vec3(rotation.x, rotation.y + 90.0f, rotation.z);
		posEnd = Vec3(position.x - varPos.x / 4, position.y, position.z - varPos.y / 4);
		posEndSecond = Vec3(position.x - varPos.x / 2, position.y, position.z - varPos.y / 2);
		positionMemory = posEndSecond;
	}
}

void Player::TurnRightUp()
{
	if (direction != RIGHT) { return; }
	if (posEndSecond.x == 0.0f && posEndSecond.y == 0.0f && posEndSecond.z == 0.0f)
	{
		direction = UP;
		position = positionMemory;
		turnEnd = Vec3(rotation.x, rotation.y - 45.0f, rotation.z);
		turnEndSecond = Vec3(rotation.x, rotation.y - 90.0f, rotation.z);
		posEnd = Vec3(position.x + varPos.x / 4, position.y, position.z + varPos.y / 4);
		posEndSecond = Vec3(position.x + varPos.x / 2, position.y, position.z + varPos.y / 2);
		positionMemory = posEndSecond;
	}
}

void Player::TurnLeftUp()
{
	if (direction != LEFT) { return; }
	if (posEndSecond.x == 0.0f && posEndSecond.y == 0.0f && posEndSecond.z == 0.0f)
	{
		direction = UP;
		position = positionMemory;
		turnEnd = Vec3(rotation.x, rotation.y + 45.0f, rotation.z);
		turnEndSecond = Vec3(rotation.x, rotation.y + 90.0f, rotation.z);
		posEnd = Vec3(position.x - varPos.x / 4, position.y, position.z + varPos.y / 4);
		posEndSecond = Vec3(position.x - varPos.x / 2, position.y, position.z + varPos.y / 2);
		positionMemory = posEndSecond;
	}
}

void Player::TurnDownRight()
{
	if (direction != DOWN) { return; }
	if (posEndSecond.x == 0.0f && posEndSecond.y == 0.0f && posEndSecond.z == 0.0f)
	{
		direction = RIGHT;
		position = positionMemory;
		turnEnd = Vec3(rotation.x, rotation.y - 45.0f, rotation.z);
		turnEndSecond = Vec3(rotation.x, rotation.y - 90.0f, rotation.z);
		posEnd = Vec3(position.x + varPos.x / 4, position.y, position.z - varPos.y / 4);
		posEndSecond = Vec3(position.x + varPos.x / 2, position.y, position.z - varPos.y / 2);
		positionMemory = posEndSecond;
	}
}

