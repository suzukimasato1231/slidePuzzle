#include "Plate.h"
#include<Shape.h>
#include<LoadCSV.h>
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
	crystallObject = Shape::CreateOBJ("sphere");

	const Vec3 basePos = { -9.0f,-0.2f,6.0f };
	const Vec2 varPos = { 6.1f,-6.1f };


	//ここからステージロード
	static char* filePath = nullptr;
	static char* crystalPath = nullptr;
	int stage[20][20] = {}, crystal[20][20] = {};
	int stageWidth = 0, stageHeight = 0;//ステージ最大

	//ステージごとにロード
	filePath = (char*)"./Resources/stage/stage1.csv";
	crystalPath = (char*)"./Resources/stage/crystal1.csv";


	LoadSize(stageWidth, stageHeight, filePath);
	LoadCSV(stage, filePath, stageWidth, stageHeight);
	LoadCSV(crystal, crystalPath, stageWidth, stageHeight);
	for (int j = 0; j < stageHeight; j++)
	{
		for (int i = 0; i < stageWidth; i++)
		{
			seaveStageBlockPosition_.push_back(Vec3(basePos.x + i * varPos.x, basePos.y, basePos.z + j * varPos.y));
			if (stage[j][i] == NONE) { continue; }

			blockData_.blockType.push_back(static_cast<PanelStatus>(stage[j][i]));
			blockData_.position.push_back(Vec3(basePos.x + i * varPos.x, basePos.y, basePos.z + j * varPos.y));
			
			blockData_.crytallFlag.push_back(static_cast<Crystal>(crystal[j][i]));
		}
	}

	easeData_ = std::make_unique<EaseData>(15);

	func_.push_back([this]() { return None(); });
	func_.push_back([this]() { return BeforeMove(); });
	func_.push_back([this]() { return Move(); });
	func_.push_back([this]() { return Selection(); });
}

void Plate::Update()
{
	func_[phase_]();
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
		if (blockData_.crytallFlag[i] == CRYSTALL)
		{
			Object::Draw(crystallObject, blockData_.position[i],
				Vec3(1.0f, 1.0f, 1.0f), Vec3(0.0f, 0.0f, 0.0f));
		}
	}

	//入れ物
	Object::Draw(container, Vec3(), Vec3(3.0f, 3.0f, 3.0f), Vec3(0.0f, 90.0f, 0.0f));
}

void Plate::AddSetSelectBlockNumber(int num)
{
	if (phase_ != 0) { return; }

	selectionStageNumber_ += num;
	if (selectionStageNumber_ < 0)
	{
		selectionStageNumber_ = static_cast<int>(seaveStageBlockPosition_.size() + static_cast<unsigned long long>(selectionStageNumber_));
	}
	else if (selectionStageNumber_ > seaveStageBlockPosition_.size() - 1)
	{
		selectionStageNumber_ = static_cast<int>(static_cast<unsigned long long>(selectionStageNumber_) - seaveStageBlockPosition_.size());
	}

	//phase_ = 3;
}

void Plate::DeleteCrstal(int num)
{
	blockData_.crytallFlag[num] = NOCRYSTALL;
}

void Plate::None()
{
	if (keyFlag_)
	{
		NullStageBlockNumber();

		int count = 0;
		bool flag = false;
		Vec3 tempPos = seaveStageBlockPosition_[selectionStageNumber_];
		for (int i = 0; i < blockData_.position.size(); i++)
		{
			if (blockData_.position[i].x == tempPos.x &&
				blockData_.position[i].y == tempPos.y &&
				blockData_.position[i].z == tempPos.z)
			{
				moveBlockNumber_.push_back(count);
				flag = true;
				break;
			}
			count++;
		}

		if (flag)
		{
			phase_ = 1;
			easeData_->Reset();
		}
	}

	keyFlag_ = false;
}

void Plate::BeforeMove()
{
	bool flag = false;
	size_t count = static_cast<size_t>(GetStageBlockNumber(static_cast<int>(moveBlockNumber_[0])));
	int number = 0;
	for (int i = 0; i < 4; i++)
	{
		if (count == nullBlockNumber_[0])
		{
			flag = true;

			for (int j = 1; j < GetStageBlockNumber(static_cast<int>(moveBlockNumber_[0])) - nullBlockNumber_[0]; j++)
			{
				moveBlockNumber_.push_back(GetSaveBlockNumber(GetStageBlockNumber(static_cast<int>(moveBlockNumber_[0])) - j));
				nullBlockNumber_.push_back(nullBlockNumber_[0] + j);
			}
		}

		if (count == 0 ||
			count == 4 ||
			count == 8 || flag)
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
		count = static_cast<size_t>(GetStageBlockNumber(static_cast<int>(moveBlockNumber_[0])));
		for (int i = 0; i < 4; i++)
		{
			if (count == nullBlockNumber_[0])
			{
				flag = true;

				for (int j = 1; j < nullBlockNumber_[0] - GetStageBlockNumber(static_cast<int>(moveBlockNumber_[0])); j++)
				{
					moveBlockNumber_.push_back(GetSaveBlockNumber(GetStageBlockNumber(static_cast<int>(moveBlockNumber_[0])) + j));
					nullBlockNumber_.push_back(nullBlockNumber_[0] - j);
				}
			}

			if (count == 3 ||
				count == 7 ||
				count == 11)
			{
				break;
			}

			count++;
		}
	}

	if (!flag)
	{
		number = 0;
		count = static_cast<size_t>(GetStageBlockNumber(static_cast<int>(moveBlockNumber_[0])));
		for (int i = 0; i < 3; i++)
		{
			if (count == nullBlockNumber_[0])
			{
				flag = true;
				moveBlockNumber_.push_back(GetSaveBlockNumber(GetStageBlockNumber(static_cast<int>(moveBlockNumber_[0])) + number * 4));
				nullBlockNumber_.push_back(GetStageBlockNumber(static_cast<int>(moveBlockNumber_[0])) + (number * 4));
			}

			if (i != 0)
			{
				number++;
			}
			count += 4;
		}
	}

	if (!flag)
	{
		number = 0;
		count = static_cast<size_t>(GetStageBlockNumber(static_cast<int>(moveBlockNumber_[0])));
		for (int i = 0; i < 3; i++)
		{
			if (count == nullBlockNumber_[0])
			{
				flag = true;
				moveBlockNumber_.push_back(GetSaveBlockNumber(GetStageBlockNumber(static_cast<int>(moveBlockNumber_[0])) - number * 4));
				nullBlockNumber_.push_back(GetStageBlockNumber(static_cast<int>(moveBlockNumber_[0])) - (number * 4));
			}

			if (i != 0)
			{
				number++;
			}
			count -= 4;
		}
	}


	if (!flag)
	{
		phase_ = 0;
		moveBlockNumber_.clear();
		nullBlockNumber_.clear();
		return;
	}

	phase_ = 2;
}

void Plate::Move()
{
	size_t count = nullBlockNumber_.size() - 1;
	for (int i = 0; i < moveBlockNumber_.size(); i++)
	{
		blockData_.position[moveBlockNumber_[i]] = Easing::easeOut(blockData_.position[moveBlockNumber_[i]], seaveStageBlockPosition_[nullBlockNumber_[count - i]], easeData_->GetTimeRate());
	}


	if (easeData_->GetEndFlag())
	{
		easeData_->Reset();
		moveBlockNumber_.clear();
		nullBlockNumber_.clear();
		phase_ = 0;
	}

	easeData_->Update();
}

void Plate::Selection()
{
	Vec3 tempPos = seaveStageBlockPosition_[selectionStageNumber_];

	selectionBlockPosition_ = tempPos;

	int count = 0;
	for (int i = 0; i < blockData_.position.size(); i++)
	{
		if (blockData_.position[i].x == tempPos.x &&
			blockData_.position[i].y == tempPos.y &&
			blockData_.position[i].z == tempPos.z)
		{
			selectionBlockNumber_ = count;
			break;
		}
		count++;
	}

	tempPos.y += 5.0f;

	blockData_.position[selectionBlockNumber_] = Easing::easeOut(blockData_.position[selectionBlockNumber_], tempPos, easeData_->GetTimeRate());

	if (easeData_->GetEndFlag())
	{
		easeData_->Reset();
		phase_ = 0;
	}

	easeData_->Update();
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
			nullBlockNumber_.push_back(i);
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

void Plate::SetMove()
{
	bool flag = false;
	size_t count = static_cast<size_t>(GetStageBlockNumber(static_cast<int>(moveBlockNumber_[0])));
	int number = 0;
	for (int i = 0; i < 4; i++)
	{
		for (int i = 0; i < 4; i++)
		{
			if (count == nullBlockNumber_[0])
			{
				flag = true;

				moveBlockNumber_.push_back(GetSaveBlockNumber(GetStageBlockNumber(static_cast<int>(moveBlockNumber_[0])) - number));
				nullBlockNumber_.push_back(nullBlockNumber_[0] + number);
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
	}
}
