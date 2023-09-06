#include "Plate.h"
#include<Shape.h>
Plate::Plate()
{
}

Plate::~Plate()
{
}

void Plate::Init()
{
	container = Shape::CreateOBJ("3x4case", false, "plate/");      //3x4�̓��ꕨ
	plateCross = Shape::CreateOBJ("plateCross", false, "plate/");     //�\���̃v���[�g
	plateDoubleTurn = Shape::CreateOBJ("plateDoubleTurn", false, "plate/");//�_�u���J�[�u
	plateLine = Shape::CreateOBJ("plateLine", false, "plate/");      //�꒼��
	plateSingleTurn = Shape::CreateOBJ("plateSingleTurn", false, "plate/");//�P��J�[�u
}

void Plate::Update()
{
}

void Plate::Draw()
{
	const Vec3 basePos = { -9.0f,-0.2f,6.0f };
	const Vec2 varPos = { 6.1f,-6.1f };
	for (int j = 0; j < 3; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			switch (stage[j][i])
			{
			case NONE:
				break;
			case WIDTHSTRAIGHTLINE://������
				Object::Draw(plateLine, Vec3(basePos.x + i * varPos.x, basePos.y, basePos.z + j * varPos.y),
					Vec3(3.0f, 3.0f, 3.0f), Vec3(0.0f, 0.0f, 0.0f));
				break;
			case HEIGHTSTRAIGHTLINE://�c����
				Object::Draw(plateLine, Vec3(basePos.x + i * varPos.x, basePos.y, basePos.z + j * varPos.y),
					Vec3(3.0f, 3.0f, 3.0f), Vec3(0.0f, 90.0f, 0.0f));
				break;
			case CROSS:            //�\��
				Object::Draw(plateCross, Vec3(basePos.x + i * varPos.x, basePos.y, basePos.z + j * varPos.y),
					Vec3(3.0f, 3.0f, 3.0f), Vec3(0.0f, 0.0f, 0.0f));
				break;
			case CUR_LEFTUP:       //�J�[�u���Ə�
				Object::Draw(plateSingleTurn, Vec3(basePos.x + i * varPos.x, basePos.y, basePos.z + j * varPos.y),
					Vec3(3.0f, 3.0f, 3.0f), Vec3(0.0f, 270.0f, 0.0f));
				break;
			case CUR_LEFTDOWN:     //�J�[�u���Ɖ�
				Object::Draw(plateSingleTurn, Vec3(basePos.x + i * varPos.x, basePos.y, basePos.z + j * varPos.y),
					Vec3(3.0f, 3.0f, 3.0f), Vec3(0.0f, 180.0f, 0.0f));
				break;
			case CUR_RIGHTUP:          //�J�[�u�E�Ə�
				Object::Draw(plateSingleTurn, Vec3(basePos.x + i * varPos.x, basePos.y, basePos.z + j * varPos.y),
					Vec3(3.0f, 3.0f, 3.0f), Vec3(0.0f, 0.0f, 0.0f));
				break;
			case CUR_RIGHTDOWN:        //�J�[�u�E�Ɖ�
				Object::Draw(plateSingleTurn, Vec3(basePos.x + i * varPos.x, basePos.y, basePos.z + j * varPos.y),
					Vec3(3.0f, 3.0f, 3.0f), Vec3(0.0f, 90.0f, 0.0f));
				break;
			case DCUR_LEFTUP_RIGHTDOWN://�_�u���J�[�u���Əさ�E�Ɖ�
				Object::Draw(plateDoubleTurn, Vec3(basePos.x + i * varPos.x, basePos.y, basePos.z + j * varPos.y),
					Vec3(3.0f, 3.0f, 3.0f), Vec3(0.0f, 0.0f, 0.0f));
				break;
			case DCUR_LEFTDOWN_RIGHTUP://�_�u���J�[�u���Ɖ����E�Ə�
				Object::Draw(plateDoubleTurn, Vec3(basePos.x + i * varPos.x, basePos.y, basePos.z + j * varPos.y),
					Vec3(3.0f, 3.0f, 3.0f), Vec3(0.0f, 180.0f, 0.0f));
				break;
			default:
				break;
			}
		}
	}
	//���ꕨ
	Object::Draw(container, Vec3(), Vec3(3.0f, 3.0f, 3.0f), Vec3(0.0f, 90.0f, 0.0f));
}
