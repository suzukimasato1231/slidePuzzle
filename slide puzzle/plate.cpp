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

	const Vec3 basePos = { -9.0f,-0.2f,6.0f };
	const Vec2 varPos = { 6.1f,-6.1f };

	for (int j = 0; j < 3; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			blockData_.position.push_back(Vec3(basePos.x + i * varPos.x, basePos.y, basePos.z + j * varPos.y));
			seaveStageBlockPosition_.push_back(Vec3(basePos.x + i * varPos.x, basePos.y, basePos.z + j * varPos.y));
			blockData_.blockType.push_back(static_cast<PanelStatus>(stage[j][i]));
		}
	}

	func_.push_back([this]() { return None(); });
	func_.push_back([this]() { return BeforeMove(); });
	func_.push_back([this]() { return Move(); });
	func_.push_back([this]() { return Selection(); });
}

void Plate::Update()
{
	//func_[phase_]();
}

void Plate::Draw()
{
	for (int i = 0; i < blockData_.blockType.size(); i++)
	{
		switch (blockData_.blockType[i])
		{
		case NONE:
			break;
		case WIDTHSTRAIGHTLINE://横直線
			Object::Draw(plateLine, blockData_.position[i],
				Vec3(3.0f, 3.0f, 3.0f), Vec3(0.0f, 0.0f, 0.0f));
			break;
		case HEIGHTSTRAIGHTLINE://縦直線
			Object::Draw(plateLine, blockData_.position[i],
				Vec3(3.0f, 3.0f, 3.0f), Vec3(0.0f, 90.0f, 0.0f));
			break;
		case CROSS:            //十字
			Object::Draw(plateCross, blockData_.position[i],
				Vec3(3.0f, 3.0f, 3.0f), Vec3(0.0f, 0.0f, 0.0f));
			break;
		case CUR_LEFTUP:       //カーブ左と上
			Object::Draw(plateSingleTurn, blockData_.position[i],
				Vec3(3.0f, 3.0f, 3.0f), Vec3(0.0f, 270.0f, 0.0f));
			break;
		case CUR_LEFTDOWN:     //カーブ左と下
			Object::Draw(plateSingleTurn, blockData_.position[i],
				Vec3(3.0f, 3.0f, 3.0f), Vec3(0.0f, 180.0f, 0.0f));
			break;
		case CUR_RIGHTUP:          //カーブ右と上
			Object::Draw(plateSingleTurn, blockData_.position[i],
				Vec3(3.0f, 3.0f, 3.0f), Vec3(0.0f, 0.0f, 0.0f));
			break;
		case CUR_RIGHTDOWN:        //カーブ右と下
			Object::Draw(plateSingleTurn, blockData_.position[i],
				Vec3(3.0f, 3.0f, 3.0f), Vec3(0.0f, 90.0f, 0.0f));
			break;
		case DCUR_LEFTUP_RIGHTDOWN://ダブルカーブ左と上＆右と下
			Object::Draw(plateDoubleTurn, blockData_.position[i],
				Vec3(3.0f, 3.0f, 3.0f), Vec3(0.0f, 0.0f, 0.0f));
			break;
		case DCUR_LEFTDOWN_RIGHTUP://ダブルカーブ左と下＆右と上
			Object::Draw(plateDoubleTurn, blockData_.position[i],
				Vec3(3.0f, 3.0f, 3.0f), Vec3(0.0f, 180.0f, 0.0f));
			break;
		default:
			break;
		}
	}

	//入れ物
	Object::Draw(container, Vec3(), Vec3(3.0f, 3.0f, 3.0f), Vec3(0.0f, 90.0f, 0.0f));
}

void Plate::None()
{
	if (keyFlag_)
	{
		NullStageBlockNumber();

		size_t count = 0;
		bool flag = false;
		Vec3 tempPos = {};
		for (int i = 0; i < blockData_.position.size(); i++)
		{
			if (blockData_.position[i].x == tempPos.x &&
				blockData_.position[i].y == tempPos.y &&
				blockData_.position[i].z == tempPos.z)
			{
				//blockPhase_.push_back(count);
				flag = true;
				break;
			}
			count++;
		}

		if (flag)
		{
			phase_ = 1;
		}
	}

	keyFlag_ = false;
}

void Plate::BeforeMove()
{
	bool flag = false;
	size_t count = static_cast<size_t>(GetStageBlockNumber(static_cast<int>(moveBlockNumber[0])));
	int number = 0;
	for (int i = 0; i < 4; i++)
	{
		if (count == nullBlockNumber[0])
		{
			flag = true;
			moveBlockNumber.push_back(GetSaveBlockNumber(GetStageBlockNumber(static_cast<int>(moveBlockNumber[0])) - number));
			nullBlockNumber.push_back(nullBlockNumber[0] + number);
		}

		if (count == 0 ||
			count == 4 ||
			count == 8)
		{
			break;
		}

		if (i != 0)
		{
			number++;
		}

		count--;
	}

	if (!flag)
	{
		number = 0;
		count = static_cast<size_t>(GetStageBlockNumber(static_cast<int>(moveBlockNumber[0])));
		for (int i = 0; i < 4; i++)
		{
			if (count == nullBlockNumber[0])
			{
				flag = true;

				moveBlockNumber.push_back(GetSaveBlockNumber(GetStageBlockNumber(static_cast<int>(moveBlockNumber[0])) + number));
				nullBlockNumber.push_back(nullBlockNumber[0] - number);
			}

			if (count == 2 ||
				count == 5 ||
				count == 8)
			{
				break;
			}

			if (i != 0)
			{
				number++;
			}

			count++;
		}
	}

	if (!flag)
	{
		number = 0;
		count = static_cast<size_t>(GetStageBlockNumber(static_cast<int>(moveBlockNumber[0])));
		for (int i = 0; i < 3; i++)
		{
			if (count == nullBlockNumber[0])
			{
				flag = true;
				moveBlockNumber.push_back(GetSaveBlockNumber(GetStageBlockNumber(static_cast<int>(moveBlockNumber[0])) + number * 3));
				nullBlockNumber.push_back(GetSaveBlockNumber(static_cast<int>(moveBlockNumber[0])) + (number * 3));
			}

			if (i != 0)
			{
				number++;
			}
			count += 3;
		}
	}

	if (!flag)
	{
		number = 0;
		count = static_cast<size_t>(GetStageBlockNumber(static_cast<int>(moveBlockNumber[0])));
		for (int i = 0; i < 3; i++)
		{
			if (count == nullBlockNumber[0])
			{
				flag = true;
				moveBlockNumber.push_back(GetSaveBlockNumber(GetStageBlockNumber(static_cast<int>(moveBlockNumber[0])) - number * 3));
				nullBlockNumber.push_back(GetSaveBlockNumber(static_cast<int>(moveBlockNumber[0])) - (number * 3));
			}

			if (i != 0)
			{
				number++;
			}
			count -= 3;
		}
	}


	if (!flag)
	{
		phase_ = 0;
		moveBlockNumber.clear();
		nullBlockNumber.clear();
		return;
	}

	phase_ = 2;
}

void Plate::Move()
{
	size_t count = nullBlockNumber.size() - 1;
	for (int i = 0; i < moveBlockNumber.size(); i++)
	{
		//blockData_.position[moveBlockNumber[i]]->SetPosition(Ease::Action(EaseType::Out, EaseFunctionType::Quint, stageBlock_[moveBlockNumber[i]]->GetPosition(), savePosition_[nullBlockNumber[count - i]], blockEaseData_->GetTimeRate()));
	}


	/*if (blockEaseData_->GetEndFlag())
	{
		blockEaseData_->Reset();
		moveBlockNumber.clear();
		nullBlockNumber.clear();
		phase_ = 0;
	}*/

	//blockEaseData_->Update();
}

void Plate::Selection()
{
}

void Plate::NullStageBlockNumber()
{
	for (int i = 0; i < seaveStageBlockPosition_.size(); i++)
	{
		bool flag = false;

		for (auto& block : blockData_.position)
		{
			if (block.x == seaveStageBlockPosition_[i].x &&
				block.y == seaveStageBlockPosition_[i].y &&
				block.z == seaveStageBlockPosition_[i].z)
			{
				flag = true;
			}
		}

		if (!flag)
		{
			nullBlockNumber.push_back(i);
			break;
		}
	}
}

int Plate::GetStageBlockNumber(int num)
{
	int count = 0, temp = 0;

	for (auto& a : seaveStageBlockPosition_)
	{
		if (a.x == blockData_.position[num].x &&
			a.y == blockData_.position[num].y &&
			a.z == blockData_.position[num].z)
		{
			count = temp;
			break;
		}
		temp++;
	}

	return count;
}

int Plate::GetSaveBlockNumber(int num)
{
	int count = 0, temp = 0;

	for (auto& a : blockData_.position)
	{
		if (a.x == seaveStageBlockPosition_[num].x &&
			a.y == seaveStageBlockPosition_[num].y &&
			a.z == seaveStageBlockPosition_[num].z)
		{
			count = temp;
			break;
		}
		temp++;
	}

	return count;
}
