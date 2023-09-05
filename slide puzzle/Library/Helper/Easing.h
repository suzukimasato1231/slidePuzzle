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