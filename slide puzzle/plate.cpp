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
	container = Shape::CreateOBJ("3x4case", false, "plate/");      //3x4の入れ物
	plateCross = Shape::CreateOBJ("plateCross", false, "plate/");     //十字のプレート
	plateDoubleTurn = Shape::CreateOBJ("plateDoubleTurn", false, "plate/");//ダブルカーブ
	plateLine = Shape::CreateOBJ("plateLine", false, "plate/");      //一直線
	plateSingleTurn = Shape::CreateOBJ("plateSingleTurn", false, "plate/");//単一カーブ
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
			case WIDTHSTRAIGHTLINE://横直線
				Object::Draw(plateLine, Vec3(basePos.x + i * varPos.x, basePos.y, basePos.z + j * varPos.y),
					Vec3(3.0f, 3.0f, 3.0f), Vec3(0.0f, 0.0f, 0.0f));
				break;
			case HEIGHTSTRAIGHTLINE://縦直線
				Object::Draw(plateLine, Vec3(basePos.x + i * varPos.x, basePos.y, basePos.z + j * varPos.y),
					Vec3(3.0f, 3.0f, 3.0f), Vec3(0.0f, 90.0f, 0.0f));
				break;
			case CROSS:            //十字
				Object::Draw(plateCross, Vec3(basePos.x + i * varPos.x, basePos.y, basePos.z + j * varPos.y),
					Vec3(3.0f, 3.0f, 3.0f), Vec3(0.0f, 0.0f, 0.0f));
				break;
			case CUR_LEFTUP:       //カーブ左と上
				Object::Draw(plateSingleTurn, Vec3(basePos.x + i * varPos.x, basePos.y, basePos.z + j * varPos.y),
					Vec3(3.0f, 3.0f, 3.0f), Vec3(0.0f, 270.0f, 0.0f));
				break;
			case CUR_LEFTDOWN:     //カーブ左と下
				Object::Draw(plateSingleTurn, Vec3(basePos.x + i * varPos.x, basePos.y, basePos.z + j * varPos.y),
					Vec3(3.0f, 3.0f, 3.0f), Vec3(0.0f, 180.0f, 0.0f));
				break;
			case CUR_RIGHTUP:          //カーブ右と上
				Object::Draw(plateSingleTurn, Vec3(basePos.x + i * varPos.x, basePos.y, basePos.z + j * varPos.y),
					Vec3(3.0f, 3.0f, 3.0f), Vec3(0.0f, 0.0f, 0.0f));
				break;
			case CUR_RIGHTDOWN:        //カーブ右と下
				Object::Draw(plateSingleTurn, Vec3(basePos.x + i * varPos.x, basePos.y, basePos.z + j * varPos.y),
					Vec3(3.0f, 3.0f, 3.0f), Vec3(0.0f, 90.0f, 0.0f));
				break;
			case DCUR_LEFTUP_RIGHTDOWN://ダブルカーブ左と上＆右と下
				Object::Draw(plateDoubleTurn, Vec3(basePos.x + i * varPos.x, basePos.y, basePos.z + j * varPos.y),
					Vec3(3.0f, 3.0f, 3.0f), Vec3(0.0f, 0.0f, 0.0f));
				break;
			case DCUR_LEFTDOWN_RIGHTUP://ダブルカーブ左と下＆右と上
				Object::Draw(plateDoubleTurn, Vec3(basePos.x + i * varPos.x, basePos.y, basePos.z + j * varPos.y),
					Vec3(3.0f, 3.0f, 3.0f), Vec3(0.0f, 180.0f, 0.0f));
				break;
			default:
				break;
			}
		}
	}
	//入れ物
	Object::Draw(container, Vec3(), Vec3(3.0f, 3.0f, 3.0f), Vec3(0.0f, 90.0f, 0.0f));
}
