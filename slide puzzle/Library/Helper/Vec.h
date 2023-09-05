#pragma once
#include<DirectXMath.h>
/// <summary>
///XMFLOAT2の補助
/// </summary>
class Vec2 :public DirectX::XMFLOAT2
{
public:
	//コンストラクタ
	Vec2();					//零ベクトルとする
	Vec2(float x, float y);	//x成分、y成分、z成分を指定しての生成
	//単項演算子オーバーロード
	Vec2 operator+() const;
	Vec2 operator-() const;

	//代入演算子オーバーロード
	Vec2 &operator+=(const Vec2 &v);
	Vec2 &operator-=(const Vec2 &v);
	Vec2 &operator*=(float s);
	Vec2 &operator/=(float s);

};
//２項演算子オーバーロード
const Vec2 operator+(const Vec2 &v1, const Vec2 &v2);
const Vec2 operator-(const Vec2 &v1, const Vec2 &v2);
const Vec2 operator*(const Vec2 &v, float s);
const Vec2 operator*(float s, const Vec2 &v);
const Vec2 operator/(const Vec2 &v, float s);
/// <summary>
/// XMFLOAT3の補助
/// </summary>
class Vec3 :public DirectX::XMFLOAT3
{
public:
	//コンストラクタ
	Vec3();							//零ベクトルとする
	Vec3(float x, float y, float z);	//x成分、y成分、z成分を指定しての生成

	//メンバ関数
	float length() const;				//ノルム（長さ）を求める
	Vec3 &normalize();				//正規化する
	float dot(const Vec3 &v)const;	//内積を求める
	Vec3 cross(const Vec3 &v)const;//外積を求める

	//単項演算子オーバーロード
	Vec3 operator+() const;
	Vec3 operator-() const;

	//代入演算子オーバーロード
	Vec3 &operator+=(const Vec3 &v);
	Vec3 &operator-=(const Vec3 &v);
	Vec3 &operator*=(float s);
	Vec3 &operator/=(float s);

};
const bool operator==(const Vec3& v1, const Vec3& v2);
//２項演算子オーバーロード
const Vec3 operator+(const Vec3 &v1, const Vec3 &v2);
const Vec3 operator-(const Vec3 &v1, const Vec3 &v2);
const Vec3 operator*(const Vec3 &v, float s);
const Vec3 operator*(float s, const Vec3 &v);
const Vec3 operator/(const Vec3 &v, float s);
const Vec3 operator*(const Vec3& v1, const Vec3 &v2);
/// <summary>
/// XMFLOAT4の補助
/// </summary>
class Vec4 :public DirectX::XMFLOAT4
{
public:
	//コンストラクタ
	Vec4();							//零ベクトルとする
	Vec4(float x, float y, float z, float w);	//x成分、y成分、z成分を指定しての生成

	//単項演算子オーバーロード
	Vec4 operator+() const;
	Vec4 operator-() const;

	//代入演算子オーバーロード
	Vec4 &operator+=(const Vec4 &v);
	Vec4 &operator-=(const Vec4 &v);
	Vec4 &operator*=(float s);
	Vec4 &operator/=(float s);
};
//２項演算子オーバーロード
const Vec4 operator+(const Vec4 &v1, const Vec4 &v2);
const Vec4 operator-(const Vec4 &v1, const Vec4 &v2);
const Vec4 operator*(const Vec4 &v, float s);
const Vec4 operator*(float s, const Vec4 &v);
const Vec4 operator/(const Vec4 &v, float s);