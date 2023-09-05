#pragma once
#include"CollisionPrimitive.h"
#include"Vec.h"
#include<DirectXMath.h>
///<summary>
///当たり判定ヘルパークラス
///</summary>
class Collision
{
public:
	/// <summary>
	/// 点と三角形の最近接点を求める
	/// </summary>
	/// <param name="point">点</param>
	/// <param name="triangle">三角形</param>
	/// <param name="closest">最近接点（出力用）</param>
	static void ClosestPtPoint2Triangle(const DirectX::XMVECTOR& point, const Triangle& triangle, DirectX::XMVECTOR* closest);

	/// <summary>
	/// 球と球の当たり判定
	/// </summary>
	/// <param name="sphereA">球A</param>
	/// <param name="sphereB">球B</param>
	/// <param name="inter">交点（出力用）</param>
	/// <returns></returns>
	static bool CheckSphere2Sphere(const Sphere& sphereA, const Sphere& sphereB, DirectX::XMVECTOR* inter = nullptr);

	/// <summary>
	/// 球と平面の当たり判定
	/// </summary>
	/// <param name="sphere">球</param>
	/// <param name="plane">平面</param>
	/// <param name="inter">交点（平面上の最近接点）</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckSphere2Plane(const Sphere& sphere, const Plane& plane, DirectX::XMVECTOR* inter = nullptr);

	/// <summary>
	/// 球と法線付き三角形の当たり判定
	/// </summary>
	/// <param name="sphere">球</param>
	/// <param name="triangle">三角形</param>
	/// <param name="inter">交点（三角形上の最近接点）</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle, DirectX::XMVECTOR* inter = nullptr, DirectX::XMVECTOR* reject = nullptr);

	/// <summary>
	/// レイと平面の当たり判定
	/// </summary>
	/// <param name="lay">レイ</param>
	/// <param name="plane">平面</param>
	/// <param name="inter">距離（出力用）</param>
	/// <param name="inter">交点（出力用）</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckRay2Plane(const Ray& lay, const Plane& plane, float* distance = nullptr, DirectX::XMVECTOR* inter = nullptr);

	/// <summary>
	/// レイと法線付き三角形の当たり判定
	/// </summary>
	/// <param name="lay">レイ</param>
	/// <param name="triangle">三角形</param>
	/// <param name="inter">距離（出力用）</param>
	/// <param name="inter">交点（出力用）</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckRay2Triangle(const Ray& lay, const Triangle& triangle, float* distance = nullptr, DirectX::XMVECTOR* inter = nullptr);

	/// <summary>
	/// レイと球の当たり判定
	/// </summary>
	/// <param name="lay">レイ</param>
	/// <param name="sphere">球</param>
	/// <param name="sphere">距離（出力用）</param>
	/// <param name="inter">交点（出力用）</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckRay2Sphere(const Ray& lay, const Sphere& sphere, float* distance = nullptr, DirectX::XMVECTOR* inter = nullptr);

	/// <summary>
	/// 球と球
	/// </summary>
	/// <param name="sphere1">球</param>
	/// <param name="sphere2">球</param>
	/// <returns></returns>
	static bool CheckSphere2Sphere(const Sphere& sphere1, const Sphere& sphere2);

	/// <summary>
	/// 直方体と直方体
	/// </summary>
	/// <param name="box1"></param>
	/// <param name="box"></param>
	/// <returns></returns>
	static bool CheckBox2Box(const Box& box1, const Box& box);

	/// <summary>
	/// 球と直方体(AABB制限付き)
	/// </summary>
	/// <param name="sphere"></param>
	/// <param name="box"></param>
	/// <returns></returns>
	static bool CheckSphere2Box(const Sphere& sphere, const Box& box);

	/// <summary>
	/// 球とカプセル
	/// </summary>
	/// <param name="sphere">球</param>
	/// <param name="capsule">カプセル</param>
	/// <returns></returns>
	static bool CheckSphere2Capsule(const Sphere& sphere, const Capsule& capsule);

	/// <summary>
	/// カプセルとカプセル
	/// </summary>
	/// <param name="capsule1">カプセル</param>
	/// <param name="capsule2">カプセル</param>
	/// <returns></returns>
	static bool CheckCapsule2Capsule(const Capsule &capsule1, const Capsule& capsule2);

	//線分と線分の距離の２乗を求める
	static float sqDistanceSegmentSegment(const DirectX::XMVECTOR& p1, const DirectX::XMVECTOR& q1, const DirectX::XMVECTOR& p2, const DirectX::XMVECTOR& q2);

	//値を範囲内に収める[low,high]
	static float clamp(float x, float low, float high)
	{
		x = (x < low) ? low : x;
		x = (x > high) ? high : x;
		return x;
	}

	//2D当たり判定

	/// <summary>
	/// 円と円の当たり判定
	/// </summary>
	/// <param name="circle1">１つ目の円の中心位置</param>
	/// <param name="circle2">２つ目の円の中心位置</param>
	/// <param name="radius1">１つ目の円の大きさ</param>
	/// <param name="radius2">２つ目の円の大きさ</param>
	/// <returns></returns>
	static bool CircleCollision(const Vec2 &circle1, Vec2 &circle2, float radius1, float radius2);
	/// <summary>
	/// 矩形の当たり判定
	/// </summary>
	/// <param name="box1">１つめの矩形の位置</param>
	/// <param name="box2">２つ目の矩形の位置</param>
	/// <param name="size1">１つ目の矩形の大きさ</param>
	/// <param name="size2">２つ目の矩形の大きさ</param>
	/// <returns></returns>
	static bool BoxCollision(const Vec2 &box1, const Vec2 &box2, const Vec2 &size1, const Vec2 &size2);

};


class OBB
{
protected:
	Vec3 m_Pos;					// 位置
	Vec3 m_NormaDirect[3];		// 方向ベクトル
	float m_fLength[3];         // 各軸方向の長さ
public:
	//XMfloat3 Pos 位置, XMMATRIX rotM 回転行列、float xの半分の長さ、 float yの半分の長さ、 float zの半分の長さ、
	void Initilize(const Vec3 &pos, const Vec3 &rotM, const Vec3 &scale);

	void SetPos(const Vec3 &pos) { m_Pos = pos; }
	Vec3 GetDirect (int elem)const; // 指定軸番号の方向ベクトルを取得
	float GetLen_W (int elem)const; // 指定軸方向の長さを取得
	Vec3 GetPos_W();				// 位置を取得
};

class OBBCollision :OBB
{
public:
	static bool ColOBBs(OBB& obb1, OBB& obb2);

	static float LenSegOnSeparateAxis(Vec3& Sep, Vec3& e1, Vec3& e2, Vec3 e3 = { 0,0,0 });
};

