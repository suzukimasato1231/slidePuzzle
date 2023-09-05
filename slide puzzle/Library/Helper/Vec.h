#pragma once
#include<DirectXMath.h>
/// <summary>
///XMFLOAT2�̕⏕
/// </summary>
class Vec2 :public DirectX::XMFLOAT2
{
public:
	//�R���X�g���N�^
	Vec2();					//��x�N�g���Ƃ���
	Vec2(float x, float y);	//x�����Ay�����Az�������w�肵�Ă̐���
	//�P�����Z�q�I�[�o�[���[�h
	Vec2 operator+() const;
	Vec2 operator-() const;

	//������Z�q�I�[�o�[���[�h
	Vec2 &operator+=(const Vec2 &v);
	Vec2 &operator-=(const Vec2 &v);
	Vec2 &operator*=(float s);
	Vec2 &operator/=(float s);

};
//�Q�����Z�q�I�[�o�[���[�h
const Vec2 operator+(const Vec2 &v1, const Vec2 &v2);
const Vec2 operator-(const Vec2 &v1, const Vec2 &v2);
const Vec2 operator*(const Vec2 &v, float s);
const Vec2 operator*(float s, const Vec2 &v);
const Vec2 operator/(const Vec2 &v, float s);
/// <summary>
/// XMFLOAT3�̕⏕
/// </summary>
class Vec3 :public DirectX::XMFLOAT3
{
public:
	//�R���X�g���N�^
	Vec3();							//��x�N�g���Ƃ���
	Vec3(float x, float y, float z);	//x�����Ay�����Az�������w�肵�Ă̐���

	//�����o�֐�
	float length() const;				//�m�����i�����j�����߂�
	Vec3 &normalize();				//���K������
	float dot(const Vec3 &v)const;	//���ς����߂�
	Vec3 cross(const Vec3 &v)const;//�O�ς����߂�

	//�P�����Z�q�I�[�o�[���[�h
	Vec3 operator+() const;
	Vec3 operator-() const;

	//������Z�q�I�[�o�[���[�h
	Vec3 &operator+=(const Vec3 &v);
	Vec3 &operator-=(const Vec3 &v);
	Vec3 &operator*=(float s);
	Vec3 &operator/=(float s);

};
const bool operator==(const Vec3& v1, const Vec3& v2);
//�Q�����Z�q�I�[�o�[���[�h
const Vec3 operator+(const Vec3 &v1, const Vec3 &v2);
const Vec3 operator-(const Vec3 &v1, const Vec3 &v2);
const Vec3 operator*(const Vec3 &v, float s);
const Vec3 operator*(float s, const Vec3 &v);
const Vec3 operator/(const Vec3 &v, float s);
const Vec3 operator*(const Vec3& v1, const Vec3 &v2);
/// <summary>
/// XMFLOAT4�̕⏕
/// </summary>
class Vec4 :public DirectX::XMFLOAT4
{
public:
	//�R���X�g���N�^
	Vec4();							//��x�N�g���Ƃ���
	Vec4(float x, float y, float z, float w);	//x�����Ay�����Az�������w�肵�Ă̐���

	//�P�����Z�q�I�[�o�[���[�h
	Vec4 operator+() const;
	Vec4 operator-() const;

	//������Z�q�I�[�o�[���[�h
	Vec4 &operator+=(const Vec4 &v);
	Vec4 &operator-=(const Vec4 &v);
	Vec4 &operator*=(float s);
	Vec4 &operator/=(float s);
};
//�Q�����Z�q�I�[�o�[���[�h
const Vec4 operator+(const Vec4 &v1, const Vec4 &v2);
const Vec4 operator-(const Vec4 &v1, const Vec4 &v2);
const Vec4 operator*(const Vec4 &v, float s);
const Vec4 operator*(float s, const Vec4 &v);
const Vec4 operator/(const Vec4 &v, float s);