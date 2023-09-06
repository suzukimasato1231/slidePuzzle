#include "Player.h"
#include<Shape.h>
#include<Collision.h>
#include"plate.h"
Player::Player()
{
}

Player::~Player()
{
}

void Player::Init()
{

	pObject = Shape::CreateOBJ("sphere");

}

void Player::Update(Plate* plate)
{

	DirectChange(plate);

	Move();

}

void Player::Draw()
{

	Object::Draw(pObject, position, Vec3(1.0f, 1.0f, 1.0f), Vec3());
}

void Player::DirectChange(Plate* plate)
{
	//�v���[�g��b�ʒu
	const Vec2 basePos = { -9.0f,6.0f };
	const Vec2 varPos = { 6.1f,-6.1f };
	Vec2 pSize = { 0.01f,0.01f }, sSize = { 6.1f,6.1f };
	for (int j = 0; j < 3; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			//�����蔻��
			if (Collision::BoxCollision(
				Vec2(position.x, position.z), Vec2(basePos.x + i * varPos.x, basePos.y + j * varPos.y),
				pSize / 2, sSize / 2))
			{
				switch (plate->GetStage(i, j))
				{
				case NONE:
					isDead = true;
					break;
				case WIDTHSTRAIGHTLINE://������
					if (direction == UP || DOWN) { isDead = true; }
					if (direction == LEFTDOWN || direction == LEFTUP)
					{
						direction = LEFT;
						position.z = basePos.y + j * varPos.y;
					}
					if (direction == RIGHTDOWN || direction == RIGHTUP)
					{
						direction = RIGHT;
						position.z = basePos.y + j * varPos.y;
					}
					break;
				case HEIGHTSTRAIGHTLINE://�c����
					if (direction == LEFT || RIGHT) { isDead = true; }
					if (direction == LEFTDOWN || direction == RIGHTDOWN)
					{
						direction = DOWN;
						position.x = basePos.x + i * varPos.x;
					}
					if (direction == LEFTUP || direction == RIGHTUP)
					{
						direction = UP;
						position.x = basePos.x + i * varPos.x;
					}
					break;
				case CROSS:            //�\��
					break;
				case CUR_LEFTUP:       //�J�[�u���Ə�
					if (direction == LEFT || direction == UP) { isDead = true; }
					if (direction == DOWN)
					{
						direction = LEFTDOWN;
					}
					if (direction == RIGHT)
					{
						direction = RIGHTUP;
					}
					break;
				case CUR_LEFTDOWN:	       //�J�[�u���Ɖ�
					if (direction == LEFT || direction == DOWN) { isDead = true; }
					if (direction == UP)
					{
						direction = LEFTUP;
					}
					if (direction == RIGHT)
					{
						direction = RIGHTDOWN;
					}
					break;
				case CUR_RIGHTUP:          //�J�[�u�E�Ə�
					if (direction == RIGHT || direction == UP) { isDead = true; }
					if (direction == DOWN)
					{
						direction = RIGHTDOWN;
					}
					if (direction == LEFT)
					{
						direction = LEFTUP;
					}
					break;
				case CUR_RIGHTDOWN:        //�J�[�u�E�Ɖ�
					if (direction == RIGHT || direction == DOWN) { isDead = true; }
					if (direction == UP)
					{
						direction = RIGHTUP;
					}
					if (direction == LEFT)
					{
						direction = LEFTDOWN;
					}
					break;
				case DCUR_LEFTUP_RIGHTDOWN://�_�u���J�[�u���Əさ�E�Ɖ�
					break;
				case DCUR_LEFTDOWN_RIGHTUP://�_�u���J�[�u���Ɖ����E�Ə�
					break;
				default:
					break;
				}
			}
		}
	}
}

void Player::Move()
{

	if (direction == UP || direction == LEFTUP || direction == RIGHTUP)
	{
		position.z += speed;
	}
	if (direction == DOWN || direction == LEFTDOWN || direction == RIGHTDOWN)
	{
		position.z -= speed;
	}
	if (direction == LEFT || direction == LEFTDOWN || direction == LEFTUP)
	{
		position.x -= speed;
	}
	if (direction == RIGHT || direction == RIGHTDOWN || direction == RIGHTUP)
	{
		position.x += speed;
	}
}
