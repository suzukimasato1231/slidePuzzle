#include"Vec.h"
#include<cmath>
using namespace std;
/// <summary>
/// XMFLOAT2の補助
/// </summary>
Vec2::Vec2() :
	Vec2(0, 0)
{}

Vec2::Vec2(float x, float y) :
	XMFLOAT2{ x,y }
{}
//単項演算子オーバーロード
Vec2 Vec2::operator+() const {
	return *this;
}

Vec2 Vec2::operator-() const {
	return Vec2(-x, -y);
}
//代入演算子オーバーロード
Vec2& Vec2::operator+=(const Vec2& v) {
	x += v.x;
	y += v.y;
	return *this;
}

Vec2& Vec2::operator-=(const Vec2& v) {
	x -= v.x;
	y -= v.y;
	return *this;
}

Vec2& Vec2::operator*=(float s) {
	x *= s;
	y *= s;
	return *this;
}

Vec2& Vec2::operator/=(float s) {
	x /= s;
	y /= s;
	return *this;
}
//２項演算子オーバーロード
const Vec2 operator+(const Vec2& v1, const Vec2& v2) {
	Vec2 temp(v1);
	return  temp += v2;
}

const Vec2 operator-(const Vec2& v1, const Vec2& v2) {
	Vec2 temp(v1);
	return  temp -= v2;
}

const Vec2 operator*(const Vec2& v, float s) {
	Vec2 temp(v);
	return  temp *= s;
}

const Vec2 operator*(float s, const Vec2& v) {
	return v * s;
}

const Vec2 operator/(const Vec2& v, float s) {
	Vec2 temp(v);
	return  temp /= s;
}
/// <summary>
/// XMFLOAT3の補助
/// </summary>
Vec3::Vec3() :
	Vec3(0, 0, 0)
{}

Vec3::Vec3(float x, float y, float z) :
	XMFLOAT3{ x,y,z }
{}

float Vec3::length() const {
	return (float)sqrt(x * x + y * y + z * z);
}

Vec3& Vec3::normalize() {
	float len = length();
	if (len != 0) {
		return *this /= len;
	}
	return *this;
}

float Vec3::dot(const Vec3& v) const {
	return x * v.x + y * v.y + z * v.z;
}

Vec3 Vec3::cross(const Vec3& v) const {
	Vec3 temp;
	temp.x = this->y * v.z - this->z * v.y;
	temp.y = this->z * v.x - this->x * v.z;
	temp.z = this->x * v.y - this->y * v.x;
	return temp;
}
//単項演算子オーバーロード
Vec3 Vec3::operator+() const {
	return *this;
}

Vec3 Vec3::operator-() const {
	return Vec3(-x, -y, -z);
}
//代入演算子オーバーロード
Vec3& Vec3::operator+=(const Vec3& v) {
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

Vec3& Vec3::operator-=(const Vec3& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

Vec3& Vec3::operator*=(float s) {
	x *= s;
	y *= s;
	z *= s;
	return *this;
}

Vec3& Vec3::operator/=(float s) {
	x /= s;
	y /= s;
	z /= s;
	return *this;
}
const bool operator==(const Vec3& v1, const Vec3& v2)
{
	if (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z)
	{
		return true;
	}
	return false;
}
//２項演算子オーバーロード
const Vec3 operator+(const Vec3& v1, const Vec3& v2) {
	Vec3 temp(v1);
	return  temp += v2;
}

const Vec3 operator-(const Vec3& v1, const Vec3& v2) {
	Vec3 temp(v1);
	return  temp -= v2;
}

const Vec3 operator*(const Vec3& v, float s) {
	Vec3 temp(v);
	return  temp *= s;
}

const Vec3 operator*(float s, const Vec3& v) {
	return v * s;
}

const Vec3 operator/(const Vec3& v, float s) {
	Vec3 temp(v);
	return  temp /= s;
}

const Vec3 operator*(const Vec3& v1, const Vec3& v2)
{
	Vec3 temp = {};
	temp.x = v1.x * v2.x;
	temp.y = v1.y * v2.y;
	temp.z = v1.z * v2.z;
	return temp;
}

const  Vec3 lerp(const Vec3& start, const Vec3& end, const float t) {
	return start * (1.0f - t) + end * t;
}

const Vec3 easeIn(const Vec3& start, const Vec3& end, const float t) {
	float y = t * t;
	return start * (1.0f - y) + end * y;

}

const Vec3 easeOut(const Vec3& start, const Vec3& end, const float t) {
	float y = t * (2 - t);
	return start * (1.0f - y) + end * y;
}

const Vec3 easeInOut(const Vec3& start, const Vec3& end, const float t) {
	float y = t * t * (3 - 2 * t);
	return start * (1.0f - y) + end * y;
}


/// <summary>
/// XMFLOAT4の補助
/// </summary>
Vec4::Vec4() :
	Vec4(0, 0, 0, 0)
{}

Vec4::Vec4(float x, float y, float z, float w) :
	XMFLOAT4{ x,y ,z,w }
{}
//単項演算子オーバーロード
Vec4 Vec4::operator+() const {
	return *this;
}

Vec4 Vec4::operator-() const {
	return Vec4(-x, -y, -z, -w);
}
//代入演算子オーバーロード
Vec4& Vec4::operator+=(const Vec4& v) {
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
	return *this;
}

Vec4& Vec4::operator-=(const Vec4& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
	return *this;
}

Vec4& Vec4::operator*=(float s) {
	x *= s;
	y *= s;
	z *= s;
	w *= s;
	return *this;
}

Vec4& Vec4::operator/=(float s) {
	x /= s;
	y /= s;
	z /= s;
	w /= s;
	return *this;
}
//２項演算子オーバーロード
const Vec4 operator+(const Vec4& v1, const Vec4& v2)
{
	Vec4 temp(v1);
	return  temp += v2;
}

const Vec4 operator-(const Vec4& v1, const Vec4& v2) {
	Vec4 temp(v1);
	return  temp -= v2;
}

const Vec4 operator*(const Vec4& v, float s) {
	Vec4 temp(v);
	return  temp *= s;
}

const Vec4 operator*(float s, const Vec4& v) {
	return v * s;
}

const Vec4 operator/(const Vec4& v, float s) {
	Vec4 temp(v);
	return  temp /= s;
}