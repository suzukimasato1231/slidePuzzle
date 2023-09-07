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
	/*bool flag = false;
	size_t count = static_cast<size_t>(GetStageBlockCount(static_cast<int>(blockPhase_[0])));
	int number = 0;
	for (int i = 0; i < MapChip::GetInstance()->GetMapChipMaxX("map"); i++)
	{
		if (count == nullBlockPhase_[0])
		{
			flag = true;
			blockPhase_.push_back(GetSaveBlockCount(GetStageBlockCount(static_cast<int>(blockPhase_[0])) - number));
			nullBlockPhase_.push_back(nullBlockPhase_[0] + number);
		}

		if (count == 0 ||
			count == 3 ||
			count == 6)
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
		count = static_cast<size_t>(GetStageBlockCount(static_cast<int>(blockPhase_[0])));
		for (int i = 0; i < MapChip::GetInstance()->GetMapChipMaxX("map"); i++)
		{
			if (count == nullBlockPhase_[0])
			{
				flag = true;

				blockPhase_.push_back(GetSaveBlockCount(GetStageBlockCount(static_cast<int>(blockPhase_[0])) + number));
				nullBlockPhase_.push_back(nullBlockPhase_[0] - number);
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
		count = static_cast<size_t>(GetStageBlockCount(static_cast<int>(blockPhase_[0])));
		for (int i = 0; i < MapChip::GetInstance()->GetMapChipMaxY("map"); i++)
		{
			if (count == nullBlockPhase_[0])
			{
				flag = true;
				blockPhase_.push_back(GetSaveBlockCount(GetStageBlockCount(static_cast<int>(blockPhase_[0])) + number * 3));
				nullBlockPhase_.push_back(GetStageBlockCount(static_cast<int>(blockPhase_[0])) + (number * 3));
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
		count = static_cast<size_t>(GetStageBlockCount(static_cast<int>(blockPhase_[0])));
		for (int i = 0; i < MapChip::GetInstance()->GetMapChipMaxY("map"); i++)
		{
			if (count == nullBlockPhase_[0])
			{
				flag = true;
				blockPhase_.push_back(GetSaveBlockCount(GetStageBlockCount(static_cast<int>(blockPhase_[0])) - number * 3));
				nullBlockPhase_.push_back(GetStageBlockCount(static_cast<int>(blockPhase_[0])) - (number * 3));
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
		blockPhase_.clear();
		nullBlockPhase_.clear();
		return;
	}

	phase_ = 2;*/
}

void Plate::Move()
{
	/*size_t count = nullBlockPhase_.size() - 1;
	for (int i = 0; i < blockPhase_.size(); i++)
	{
		stageBlock_[blockPhase_[i]]->SetPosition(Ease::Action(EaseType::Out, EaseFunctionType::Quint, stageBlock_[blockPhase_[i]]->GetPosition(), savePosition_[nullBlockPhase_[count - i]], blockEaseData_->GetTimeRate()));
	}


	if (blockEaseData_->GetEndFlag())
	{
		blockEaseData_->Reset();
		blockPhase_.clear();
		nullBlockPhase_.clear();
		phase_ = 0;
	}

	blockEaseData_->Update();*/
}

void Plate::Selection()
{
}

void Plate::NullStageBlockNumber()
{
}

int Plate::GetStageBlockNumber(int num)
{
	int count = 0, temp = 0;

	/*for (auto& a : savePosition_)
	{
		if (a.x == stageBlock_[num]->GetPosition().x &&
			a.y == stageBlock_[num]->GetPosition().y &&
			a.z == stageBlock_[num]->GetPosition().z)
		{
			count = temp;
			break;
		}
		temp++;
	}*/

	return count;
}

int Plate::GetSaveBlockNumber(int num)
{
	int count = 0, temp = 0;

	/*for (auto& a : stageBlock_)
	{
		if (a->GetPosition().x == savePosition_[num].x &&
			a->GetPosition().y == savePosition_[num].y &&
			a->GetPosition().z == savePosition_[num].z)
		{
			count = temp;
			break;
		}
		temp++;
	}*/

	return count;
}
