#include "Plate.h"
#include<Shape.h>
#include<LoadCSV.h>
#include<random>

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
	crystallObject = Shape::CreateOBJ("crystal");
	landmarkObject = Shape::CreateSquare(1.0f, 1.0f, 1.0f);
	landmarkTex = Texture::Get()->LoadTexture(L"Resources/forcusGrid.png");
	const Vec3 basePos = { -9.0f,-0.2f,6.0f };
	const Vec2 varPos = { 6.1f,-6.1f };
	decide = Audio::Get()->SoundLoadWave("Resources/Sound/panelMove.wav");

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

	easeData_ = std::make_unique<EaseData>(5);

	func_.push_back([this]() { return None(); });
	func_.push_back([this]() { return BeforeMove(); });
	func_.push_back([this]() { return Move(); });
	func_.push_back([this]() { return Selection(); });
	func_.push_back([this]() { return SelectionMove(); });
}

void Plate::Update()
{
	func_[phase_]();

	CrystalRote();

	CrystalCreate();
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
				Vec3(0.5f, 0.5f, 0.5f), crystalRote);
		}
	}
	//入れ物
	Object::Draw(container, Vec3(), Vec3(3.0f, 3.0f, 3.0f), Vec3(0.0f, 90.0f, 0.0f));

	Object::Draw(landmarkObject, seaveStageBlockPosition_[selectionStageNumber_],
		Vec3(5.0f, 2.0f, 5.0f), Vec3(0.0f, 0.0f, 0.0f), landmarkTex);
}

void Plate::AddSetSelectBlockNumber(int num)
{
	saveSelectionBlockNumber_ = selectionStageNumber_;
	selectionStageNumber_ += num;

	if (num == -1 && (saveSelectionBlockNumber_ == 0 || saveSelectionBlockNumber_ == 4 || saveSelectionBlockNumber_ == 8))
	{
		selectionStageNumber_ += 4;
	}
	else if (num == 1 && (saveSelectionBlockNumber_ == 3 || saveSelectionBlockNumber_ == 7 || saveSelectionBlockNumber_ == 11))
	{
		selectionStageNumber_ -= 4;
	}

	if (selectionStageNumber_ < 0)
	{
		selectionStageNumber_ = static_cast<int>(seaveStageBlockPosition_.size() + static_cast<unsigned long long>(selectionStageNumber_));
	}
	else if (selectionStageNumber_ > seaveStageBlockPosition_.size() - 1)
	{
		selectionStageNumber_ = static_cast<int>(static_cast<unsigned long long>(selectionStageNumber_) - seaveStageBlockPosition_.size());
	}

	if (SelectionBlockCount())
	{
		//phase_ = 3;
	}
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
			Audio::Get()->SoundSEPlayWave(decide);
		}
	}

	keyFlag_ = false;
}

void Plate::BeforeMove()
{
	bool flag = false;

	if (LeftRight(4, GetStageBlockNumber(static_cast<int>(moveBlockNumber_[0])), 0, true))
	{
		flag = true;
	}
	else if (LeftRight(4, GetStageBlockNumber(static_cast<int>(moveBlockNumber_[0])), 3, false))
	{
		flag = true;
	}
	else if (UpDown(3, GetStageBlockNumber(static_cast<int>(moveBlockNumber_[0])), 0, true))
	{
		flag = true;
	}
	else if (UpDown(3, GetStageBlockNumber(static_cast<int>(moveBlockNumber_[0])), 0, false))
	{
		flag = true;
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
	tempPos.y += 5.0f;

	blockData_.position[selectionBlockNumber_] = Easing::easeOut(blockData_.position[selectionBlockNumber_], tempPos, easeData_->GetTimeRate());

	if (easeData_->GetEndFlag())
	{
		easeData_->Reset();
		phase_ = 4;
	}

	easeData_->Update();
}

void Plate::SelectionMove()
{
	Vec3 tempPos = seaveStageBlockPosition_[selectionStageNumber_];

	blockData_.position[selectionBlockNumber_] = Easing::easeIn(blockData_.position[selectionBlockNumber_], tempPos, easeData_->GetTimeRate());

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

bool Plate::LeftRight(int count, int number, int num, bool flag)
{
	int tempNumber = number;
	for (int i = 0; i < count; i++)
	{
		if (tempNumber == nullBlockNumber_[0])
		{
			if (flag)
			{
				for (int j = 1; j < GetStageBlockNumber(static_cast<int>(moveBlockNumber_[0])) - nullBlockNumber_[0]; j++)
				{
					moveBlockNumber_.push_back(GetSaveBlockNumber(GetStageBlockNumber(static_cast<int>(moveBlockNumber_[0])) - j));
					nullBlockNumber_.push_back(nullBlockNumber_[0] + j);
				}
			}
			else
			{
				for (int j = 1; j < nullBlockNumber_[0] - GetStageBlockNumber(static_cast<int>(moveBlockNumber_[0])); j++)
				{
					moveBlockNumber_.push_back(GetSaveBlockNumber(GetStageBlockNumber(static_cast<int>(moveBlockNumber_[0])) + j));
					nullBlockNumber_.push_back(nullBlockNumber_[0] - j);
				}
			}

			return true;
		}

		if (tempNumber == num ||
			tempNumber == num + 4 ||
			tempNumber == num + 8)
		{
			return false;
		}

		if (flag) { tempNumber--; }
		else { tempNumber++; }
	}

	return false;
}

bool Plate::UpDown(int count, int number, int num, bool flag)
{
	int tempNumber = number;
	for (int i = 0; i < count; i++)
	{
		if (tempNumber == nullBlockNumber_[0])
		{
			for (int j = 1; j < i; j++)
			{
				if (flag)
				{
					moveBlockNumber_.push_back(GetSaveBlockNumber(GetStageBlockNumber(static_cast<int>(moveBlockNumber_[0])) + j * 4));
					nullBlockNumber_.push_back(GetStageBlockNumber(static_cast<int>(moveBlockNumber_[0])) + (j * 4));
				}
				else
				{
					moveBlockNumber_.push_back(GetSaveBlockNumber(GetStageBlockNumber(static_cast<int>(moveBlockNumber_[0])) - j * 4));
					nullBlockNumber_.push_back(GetStageBlockNumber(static_cast<int>(moveBlockNumber_[0])) - (j * 4));
				}

			}
			return true;
		}

		if (flag) { tempNumber += 4; }
		else { tempNumber -= 4; }
	}

	return false;
}

bool Plate::SelectionBlockCount()
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

	if (GetStageBlockNumber(static_cast<int>(selectionBlockNumber_)) != saveSelectionBlockNumber_)
	{
		return true;
	}

	return false;
}

void Plate::CrystalRote()
{

	crystalRote += crystalRoteSpeed;
	if (crystalRote.y >= 360.0f)
	{
		crystalRote.y -= 360.0f;
	}
	else if (crystalRote.y <= -360.0f)
	{
		crystalRote.y += 360.0f;
	}
}

void Plate::CrystalCreate()
{
	//クリスタルが盤面にあるか探す
	bool flag = false;
	for (int i = 0; i < blockData_.blockType.size(); i++)
	{
		if (blockData_.crytallFlag[i] == CRYSTALL)
		{
			flag = true;
		}
	}
	//クリスタルが無かったらランダムで読み込む
	if (flag == true) { return; }

	static char* crystalPath = nullptr;
	int crystal[20][20] = {};
	int stageWidth = 0, stageHeight = 0;//ステージ最大


	//ステージごとにロード
	std::random_device seed_gen;
	std::mt19937 engine(seed_gen());
	std::uniform_int_distribution<>rand100(1, 3);

	if (rand100(engine) == 1)
	{
		crystalPath = (char*)"./Resources/stage/crystal1.csv";
	}
	else if (rand100(engine) == 2)
	{
		crystalPath = (char*)"./Resources/stage/crystal2.csv";
	}
	else if (rand100(engine) == 3)
	{
		crystalPath = (char*)"./Resources/stage/crystal3.csv";
	}
	else
	{
		crystalPath = (char*)"./Resources/stage/crystal3.csv";
	}
	

	LoadSize(stageWidth, stageHeight, crystalPath);
	LoadCSV(crystal, crystalPath, stageWidth, stageHeight);

	int width = 0, height = 0;
	for (int i = 0; i < blockData_.blockType.size(); i++)
	{
		blockData_.crytallFlag[i] = static_cast<Crystal>(crystal[height][width]);
		width++;
		if (width == stageWidth)
		{
			width = 0;
			height++;
		}
	}
}
