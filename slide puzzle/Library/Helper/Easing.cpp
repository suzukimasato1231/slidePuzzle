#include"Easing.h"
const  Vec3 Easing::lerp(const Vec3 &start, const Vec3 &end, const float t)
{
	return start * (1.0f - t) + end * t;
}

const Vec3 Easing::easeIn(const Vec3 &start, const Vec3 &end, const float t)
{
	float y = t * t;
	return start * (1.0f - y) + end * y;

}

const Vec3 Easing::easeOut(const Vec3 &start, const Vec3 &end, const float t)
{
	float y = t * (2 - t);
	return start * (1.0f - y) + end * y;
}

const Vec3 Easing::easeInOut(const Vec3 &start, const Vec3 &end, const float t)
{
	float y = t * t * (3 - 2 * t);
	return start * (1.0f - y) + end * y;
}

EaseData::EaseData(int count)
{
	countNum_ = count;
}

void EaseData::Update()
{
	if (!endFlag_ && actFlag_)
	{
		timeRate_ = timer_ / countNum_;
		timer_++;

		if (timer_ > countNum_)
		{
			endFlag_ = true;
			timer_ = 0.0f;
		}
	}
}

void EaseData::Reset()
{
	timer_ = 0.0f;
	timeRate_ = 0.0f;
	endFlag_ = false;
}
