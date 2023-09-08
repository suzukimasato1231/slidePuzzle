#pragma once
#include"Vec.h"
/// <summary>
//イージング
//補間関数
//線形補間
/// </summary>
class Easing
{
public:
	/// <summary>
	/// 一定
	/// </summary>
	/// <param name="start">スタート位置</param>
	/// <param name="end">終了位置</param>
	/// <param name="t">経過時間</param>
	/// <returns></returns>
	static const Vec3 lerp(const Vec3 &start, const Vec3 &end, const float t);
	/// <summary>
	/// 徐々に速く
	/// </summary>
	/// <param name="start">スタート位置</param>
	/// <param name="end">終了位置</param>
	/// <param name="t">経過時間</param>
	/// <returns></returns>
	static const Vec3 easeIn(const Vec3 &start, const Vec3 &end, const float t);
	/// <summary>
	/// 徐々に遅く
	/// </summary>
	/// <param name="start">スタート位置</param>
	/// <param name="end">終了位置</param>
	/// <param name="t">経過時間</param>
	/// <returns></returns>
	static const Vec3 easeOut(const Vec3 &start, const Vec3 &end, const float t);
	/// <summary>
	/// 最初速く徐々に遅く
	/// </summary>
	/// <param name="start">スタート位置</param>
	/// <param name="end">終了位置</param>
	/// <param name="t">経過時間</param>
	/// <returns></returns>
	static const Vec3 easeInOut(const Vec3 &start, const Vec3 &end, const float t);
};

class EaseData
{
public:
	EaseData(int count = 0);
	~EaseData() = default;
	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();
	/// <summary>
	/// リセット
	/// </summary>
	void Reset();

public:
	/// <summary>
	/// 終了したか
	/// </summary>
	/// <returns></returns>
	bool GetEndFlag() { return endFlag_; }
	/// <summary>
	/// 行動しているか
	/// </summary>
	/// <returns></returns>
	bool GetActFlag() { return actFlag_; }
	/// <summary>
	/// 進行度の取得
	/// </summary>
	/// <returns>0.0 ~ 1.0</returns>
	float GetTimeRate() { return timeRate_; }
	/// <summary>
	/// フレームの設定
	/// </summary>
	/// <param name="count"></param>
	void SetCount(int count) { countNum_ = count; }
	/// <summary>
	/// 終了フラグの設定
	/// </summary>
	/// <param name="Flag"></param>
	void SetEndFlag(bool flag) { endFlag_ = flag; }
	/// <summary>
	/// 行動フラグの設定
	/// </summary>
	/// <param name="flag"></param>
	void SetActFlag(bool flag) { actFlag_ = flag; }
private:
	// イージングの進行度用
	float timer_ = 0.0f;
	float timeRate_ = 0.0f;
	int countNum_ = 0;
	bool endFlag_ = false;
	bool actFlag_ = true;
};