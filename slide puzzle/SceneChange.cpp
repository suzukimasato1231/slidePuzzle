#include "SceneChange.h"
#include"Easing.h"

void BlackBox::Spin(TYPE type)
{
	//終了していれば処理を省略
	if (type == TYPE::FadeIn && inEnd_ || type == TYPE::FadeOut && outEnd_)
	{
		return;
	}

	float timeRate = 0.0f;
	int countNum = 60;
	timeRate = easeTimer_ / countNum;
	easeTimer_++;

	//縮小
	Vec2 scale = {};
	if (type == TYPE::FadeIn)
	{
		scale.x = Easing::easeIn(Vec3(64.0f, 64.0f, 64.0f), Vec3(0.0f, 0.0f, 0.0f), timeRate).x;
		scale.y = Easing::easeIn(Vec3(64.0f, 64.0f, 64.0f), Vec3(0.0f, 0.0f, 0.0f), timeRate).y;
	}
	else if (type == TYPE::FadeOut)
	{
		scale.x = Easing::easeOut(Vec3(0.0f, 0.0f, 0.0f), Vec3(64.0f, 64.0f, 64.0f), timeRate).x;
		scale.y = Easing::easeOut(Vec3(0.0f, 0.0f, 0.0f), Vec3(64.0f, 64.0f, 64.0f), timeRate).y;
	}

	//見えなくなったら終了
	if (type == TYPE::FadeIn && easeTimer_ > countNum)
	{
		scale = {};
		easeTimer_ = 0.0f;
		inEnd_ = true;
	}
	else if (type == TYPE::FadeOut && easeTimer_ > countNum)
	{
		scale = Vec2(64.0f, 64.0f);
		easeTimer_ = 0.0f;
		outEnd_ = true;
	}

	scale_ = scale;
}

SceneChange::SceneChange()
{
	type_ = TYPE::FadeIn;

	// アンカーポイント
	XMFLOAT2 anchorPoint = { 0.5f, 0.5f };
	// 色
	XMFLOAT4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

	for (int x = 0; x < static_cast<int>(SIZEMAX::Width); x++)
	{
		std::vector<std::unique_ptr<BlackBox>> tempX;
		for (int y = 0; y < static_cast<int>(SIZEMAX::Height); y++)
		{
			std::unique_ptr<BlackBox> tempY = std::make_unique<BlackBox>();
			tempY->sprite_ = Sprite::Get()->SpriteCreate(L"Resources/black.png");
			tempY->scale_ = Vec2(64.0f, 64.0f);
			tempY->pos_ = { static_cast<float>(20 + x * 40),static_cast<float>(20 + y * 40) };
			tempY->offsetTimer_ = (31 - x + y);

			tempX.push_back(std::move(tempY));
		}
		blackBox_.push_back(std::move(tempX));
	}
}

void SceneChange::Update()
{
	if (type_ == TYPE::Not)
	{
		return;
	}

	for (int x = 0; x < blackBox_.size(); x++)
	{
		for (int y = 0; y < blackBox_[x].size(); y++)
		{
			if (blackBox_[x][y]->GetOffsetTimer() <= 0)
			{
				blackBox_[x][y]->Spin(type_);
			}
			else
			{
				blackBox_[x][y]->AddOffsetTimer(-1);
			}

		}
	}

	//末端がtrueならそれを全体に適応
	if (blackBox_[0][17]->inEnd_)
	{
		inEndFlag_ = true;
	}
	if (blackBox_[0][17]->outEnd_)
	{
		outEndFlag_ = true;
	}
}

void SceneChange::Draw()
{
	if (type_ == TYPE::Not)
	{
		return;
	}

	for (int x = 0; x < blackBox_.size(); x++)
	{
		for (int y = 0; y < blackBox_[x].size(); y++)
		{
			Sprite::Get()->Draw(blackBox_[x][y]->sprite_, blackBox_[x][y]->pos_, blackBox_[x][y]->scale_.x, blackBox_[x][y]->scale_.y, Vec2(0.5f,0.5f));
		}
	}
}

void SceneChange::SceneChangeStart(const std::string& sceneName)
{
	if (type_ != TYPE::FadeOut)
	{
		outEndFlag_ = false;

		for (int x = 0; x < blackBox_.size(); x++)
		{
			for (int y = 0; y < blackBox_[x].size(); y++)
			{
				blackBox_[x][y]->scale_ = Vec2(0.0f, 0.0f);
				blackBox_[x][y]->offsetTimer_ = (31 - x + y);
			}
		}
	}
	type_ = TYPE::FadeOut;

	sceneName_ = sceneName;
}
