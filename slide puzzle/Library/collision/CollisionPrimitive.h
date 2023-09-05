#pragma once
//当たり判定プリミティブ

#include<DirectXMath.h>
/// <summary>
/// 球
/// </summary>
struct Sphere
{
	//中心座標
	DirectX::XMVECTOR center = { 0,0,0,1 };
	//半径
	float radius = 1.0f;
};

struct Plane
{
	//法線ベクトル
	DirectX::XMVECTOR normal = { 0,1,0,0 };
	//原点（0,0,0）からの距離
	float distance = 0.0f;
};

/// <summary>
/// 法線付き三角形
/// </summary>
class Triangle
{
public:
	//頂点座標３つ
	DirectX::XMVECTOR p0;
	DirectX::XMVECTOR p1;
	DirectX::XMVECTOR p2;
	//法線ベクトル
	DirectX::XMVECTOR normal;
};

/// <summary>
/// レイ（半直線）
/// </summary>
struct Ray
{
	//始点座標
	DirectX::XMVECTOR start = { 0,0,0,1 };
	//方向
	DirectX::XMVECTOR dir = { 1.0,0,0 };
};

/// <summary>
/// 立方体
/// </summary>
struct Box
{
	//最小値x,y,z
	DirectX::XMVECTOR minPosition;
	//最大値x,y,z
	DirectX::XMVECTOR maxPosition;
};

/// <summary>
/// カプセル
/// </summary>
struct Capsule
{
	//開始座標
	DirectX::XMVECTOR startPosition;
	//終了座標
	DirectX::XMVECTOR endPosition;
	//半径
	float radius;
};