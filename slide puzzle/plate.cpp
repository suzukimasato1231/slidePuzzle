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
	container = Shape::CreateOBJ("3x4case", false, "plate/");      //3x4�̓��ꕨ
	plateCross = Shape::CreateOBJ("plateCross", false, "plate/");     //�\���̃v���[�g
	plateDoubleTurn = Shape::CreateOBJ("plateDoubleTurn", false, "plate/");//�_�u���J�[�u
	plateLine = Shape::CreateOBJ("plateLine", false, "plate/");      //�꒼��
	plateSingleTurn = Shape::CreateOBJ("plateSingleTurn", false, "plate/");//�P��J�[�u
	crystallObject = Shape::CreateOBJ("sphere");

	const Vec3 basePos = { -9.0f,-0.2f,6.0f };
	const Vec2 varPos = { 6.1f,-6.1f };


	//��������X�e�[�W���[�h
	static char* filePath = nullptr;
	static char* crystalPath = nullptr;
	int stage[20][20] = {}, crystal[20][20] = {};
	int stageWidth = 0, stageHeight = 0;//�X�e�[�W�ő�

	//�X�e�[�W���ƂɃ��[�h
	filePath = (char*)"./Resources/stage/stage1.csv";
	crystalPath = (char*)"./Resources/stage/crystal1.csv";


	LoadSize(stageWidth, stageHeight, filePath);
	LoadCSV(stage, filePath, stageWidth, stageHeight);
	LoadCSV(crystal, crystalPath, stageWidth, stageHeight);
	for (int j = 0; j < stageHeight; j++)
	{
		for (int i = 0; i < stageWidth; i++)
		{
			blockData_.position.push_back(Vec3(basePos.x + i * varPos.x, basePos.y, basePos.z + j * varPos.y));
			seaveStageBlockPosition_.push_back(Vec3(basePos.x + i * varPos.x, basePos.y, basePos.z + j * varPos.y));
			blockData_.blockType.push_back(static_cast<PanelStatus>(stage[j][i]));
			blockData_.crytallFlag.push_back(static_cast<Crystal>(crystal[j][i]));
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
		case WIDTHSTRAIGHTLINE://������
			Object::Draw(plateLine, blockData_.position[i],
				Vec3(3.0f, 3.0f, 3.0f), Vec3(0.0f, 0.0f, 0.0f));
			break;
		case HEIGHTSTRAIGHTLINE://�c����
			Object::Draw(plateLine, blockData_.position[i],
				Vec3(3.0f, 3.0f, 3.0f), Vec3(0.0f, 90.0f, 0.0f));
			break;
		case CROSS:            //�\��
			Object::Draw(plateCross, blockData_.position[i],
				Vec3(3.0f, 3.0f, 3.0f), Vec3(0.0f, 0.0f, 0.0f));
			break;
		case CUR_LEFTUP:       //�J�[�u���Ə�
			Object::Draw(plateSingleTurn, blockData_.position[i],
				Vec3(3.0f, 3.0f, 3.0f), Vec3(0.0f, 270.0f, 0.0f));
			break;
		case CUR_LEFTDOWN:     //�J�[�u���Ɖ�
			Object::Draw(plateSingleTurn, blockData_.position[i],
				Vec3(3.0f, 3.0f, 3.0f), Vec3(0.0f, 180.0f, 0.0f));
			break;
		case CUR_RIGHTUP:          //�J�[�u�E�Ə�
			Object::Draw(plateSingleTurn, blockData_.position[i],
				Vec3(3.0f, 3.0f, 3.0f), Vec3(0.0f, 0.0f, 0.0f));
			break;
		case CUR_RIGHTDOWN:        //�J�[�u�E�Ɖ�
			Object::Draw(plateSingleTurn, blockData_.position[i],
				Vec3(3.0f, 3.0f, 3.0f), Vec3(0.0f, 90.0f, 0.0f));
			break;
		case DCUR_LEFTUP_RIGHTDOWN://�_�u���J�[�u���Əさ�E�Ɖ�
			Object::Draw(plateDoubleTurn, blockData_.position[i],
				Vec3(3.0f, 3.0f, 3.0f), Vec3(0.0f, 0.0f, 0.0f));
			break;
		case DCUR_LEFTDOWN_RIGHTUP://�_�u���J�[�u���Ɖ����E�Ə�
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

	//���ꕨ
	Object::Draw(container, Vec3(), Vec3(3.0f, 3.0f, 3.0f), Vec3(0.0f, 90.0f, 0.0f));
}

void Plate::AddSetSelectBlockNumber(int num)
{
	selectionBlockNumber_ += num;
	selectionBlockNumber_ = std::clamp(selectionBlockNumber_, 0, static_cast<int>(seaveStageBlockPosition_.size() - 1));
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
		Vec3 tempPos = {};
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

	if (!flag)
	{
		number = 0;
		count = static_cast<size_t>(GetStageBlockNumber(static_cast<int>(moveBlockNumber_[0])));
		for (int i = 0; i < 4; i++)
		{
			if (count == nullBlockNumber_[0])
			{
				flag = true;

				moveBlockNumber_.push_back(GetSaveBlockNumber(GetStageBlockNumber(static_cast<int>(moveBlockNumber_[0])) + number));
				nullBlockNumber_.push_back(nullBlockNumber_[0] - number);
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
		count = static_cast<size_t>(GetStageBlockNumber(static_cast<int>(moveBlockNumber_[0])));
		for (int i = 0; i < 3; i++)
		{
			if (count == nullBlockNumber_[0])
			{
				flag = true;
				moveBlockNumber_.push_back(GetSaveBlockNumber(GetStageBlockNumber(static_cast<int>(moveBlockNumber_[0])) + number * 3));
				nullBlockNumber_.push_back(GetSaveBlockNumber(static_cast<int>(moveBlockNumber_[0])) + (number * 3));
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
		count = static_cast<size_t>(GetStageBlockNumber(static_cast<int>(moveBlockNumber_[0])));
		for (int i = 0; i < 3; i++)
		{
			if (count == nullBlockNumber_[0])
			{
				flag = true;
				moveBlockNumber_.push_back(GetSaveBlockNumber(GetStageBlockNumber(static_cast<int>(moveBlockNumber_[0])) - number * 3));
				nullBlockNumber_.push_back(GetSaveBlockNumber(static_cast<int>(moveBlockNumber_[0])) - (number * 3));
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
	Vec3 tempPos = seaveStageBlockPosition_[selectionBlockNumber_];

	selectionBlockPosition_ = tempPos;
	//selectionBlockPosition_ = (Ease::Action(EaseType::Out, EaseFunctionType::Quint, block_->GetPosition(), tempPos, easeData_->GetTimeRate()));

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