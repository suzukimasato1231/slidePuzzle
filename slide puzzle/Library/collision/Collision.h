#pragma once
#include"CollisionPrimitive.h"
#include"Vec.h"
#include<DirectXMath.h>
///<summary>
///�����蔻��w���p�[�N���X
///</summary>
class Collision
{
public:
	/// <summary>
	/// �_�ƎO�p�`�̍ŋߐړ_�����߂�
	/// </summary>
	/// <param name="point">�_</param>
	/// <param name="triangle">�O�p�`</param>
	/// <param name="closest">�ŋߐړ_�i�o�͗p�j</param>
	static void ClosestPtPoint2Triangle(const DirectX::XMVECTOR& point, const Triangle& triangle, DirectX::XMVECTOR* closest);

	/// <summary>
	/// ���Ƌ��̓����蔻��
	/// </summary>
	/// <param name="sphereA">��A</param>
	/// <param name="sphereB">��B</param>
	/// <param name="inter">��_�i�o�͗p�j</param>
	/// <returns></returns>
	static bool CheckSphere2Sphere(const Sphere& sphereA, const Sphere& sphereB, DirectX::XMVECTOR* inter = nullptr);

	/// <summary>
	/// ���ƕ��ʂ̓����蔻��
	/// </summary>
	/// <param name="sphere">��</param>
	/// <param name="plane">����</param>
	/// <param name="inter">��_�i���ʏ�̍ŋߐړ_�j</param>
	/// <returns>�������Ă��邩�ۂ�</returns>
	static bool CheckSphere2Plane(const Sphere& sphere, const Plane& plane, DirectX::XMVECTOR* inter = nullptr);

	/// <summary>
	/// ���Ɩ@���t���O�p�`�̓����蔻��
	/// </summary>
	/// <param name="sphere">��</param>
	/// <param name="triangle">�O�p�`</param>
	/// <param name="inter">��_�i�O�p�`��̍ŋߐړ_�j</param>
	/// <returns>�������Ă��邩�ۂ�</returns>
	static bool CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle, DirectX::XMVECTOR* inter = nullptr, DirectX::XMVECTOR* reject = nullptr);

	/// <summary>
	/// ���C�ƕ��ʂ̓����蔻��
	/// </summary>
	/// <param name="lay">���C</param>
	/// <param name="plane">����</param>
	/// <param name="inter">�����i�o�͗p�j</param>
	/// <param name="inter">��_�i�o�͗p�j</param>
	/// <returns>�������Ă��邩�ۂ�</returns>
	static bool CheckRay2Plane(const Ray& lay, const Plane& plane, float* distance = nullptr, DirectX::XMVECTOR* inter = nullptr);

	/// <summary>
	/// ���C�Ɩ@���t���O�p�`�̓����蔻��
	/// </summary>
	/// <param name="lay">���C</param>
	/// <param name="triangle">�O�p�`</param>
	/// <param name="inter">�����i�o�͗p�j</param>
	/// <param name="inter">��_�i�o�͗p�j</param>
	/// <returns>�������Ă��邩�ۂ�</returns>
	static bool CheckRay2Triangle(const Ray& lay, const Triangle& triangle, float* distance = nullptr, DirectX::XMVECTOR* inter = nullptr);

	/// <summary>
	/// ���C�Ƌ��̓����蔻��
	/// </summary>
	/// <param name="lay">���C</param>
	/// <param name="sphere">��</param>
	/// <param name="sphere">�����i�o�͗p�j</param>
	/// <param name="inter">��_�i�o�͗p�j</param>
	/// <returns>�������Ă��邩�ۂ�</returns>
	static bool CheckRay2Sphere(const Ray& lay, const Sphere& sphere, float* distance = nullptr, DirectX::XMVECTOR* inter = nullptr);

	/// <summary>
	/// ���Ƌ�
	/// </summary>
	/// <param name="sphere1">��</param>
	/// <param name="sphere2">��</param>
	/// <returns></returns>
	static bool CheckSphere2Sphere(const Sphere& sphere1, const Sphere& sphere2);

	/// <summary>
	/// �����̂ƒ�����
	/// </summary>
	/// <param name="box1"></param>
	/// <param name="box"></param>
	/// <returns></returns>
	static bool CheckBox2Box(const Box& box1, const Box& box);

	/// <summary>
	/// ���ƒ�����(AABB�����t��)
	/// </summary>
	/// <param name="sphere"></param>
	/// <param name="box"></param>
	/// <returns></returns>
	static bool CheckSphere2Box(const Sphere& sphere, const Box& box);

	/// <summary>
	/// ���ƃJ�v�Z��
	/// </summary>
	/// <param name="sphere">��</param>
	/// <param name="capsule">�J�v�Z��</param>
	/// <returns></returns>
	static bool CheckSphere2Capsule(const Sphere& sphere, const Capsule& capsule);

	/// <summary>
	/// �J�v�Z���ƃJ�v�Z��
	/// </summary>
	/// <param name="capsule1">�J�v�Z��</param>
	/// <param name="capsule2">�J�v�Z��</param>
	/// <returns></returns>
	static bool CheckCapsule2Capsule(const Capsule &capsule1, const Capsule& capsule2);

	//�����Ɛ����̋����̂Q������߂�
	static float sqDistanceSegmentSegment(const DirectX::XMVECTOR& p1, const DirectX::XMVECTOR& q1, const DirectX::XMVECTOR& p2, const DirectX::XMVECTOR& q2);

	//�l��͈͓��Ɏ��߂�[low,high]
	static float clamp(float x, float low, float high)
	{
		x = (x < low) ? low : x;
		x = (x > high) ? high : x;
		return x;
	}

	//2D�����蔻��

	/// <summary>
	/// �~�Ɖ~�̓����蔻��
	/// </summary>
	/// <param name="circle1">�P�ڂ̉~�̒��S�ʒu</param>
	/// <param name="circle2">�Q�ڂ̉~�̒��S�ʒu</param>
	/// <param name="radius1">�P�ڂ̉~�̑傫��</param>
	/// <param name="radius2">�Q�ڂ̉~�̑傫��</param>
	/// <returns></returns>
	static bool CircleCollision(const Vec2 &circle1, Vec2 &circle2, float radius1, float radius2);
	/// <summary>
	/// ��`�̓����蔻��
	/// </summary>
	/// <param name="box1">�P�߂̋�`�̈ʒu</param>
	/// <param name="box2">�Q�ڂ̋�`�̈ʒu</param>
	/// <param name="size1">�P�ڂ̋�`�̑傫��</param>
	/// <param name="size2">�Q�ڂ̋�`�̑傫��</param>
	/// <returns></returns>
	static bool BoxCollision(const Vec2 &box1, const Vec2 &box2, const Vec2 &size1, const Vec2 &size2);

};


class OBB
{
protected:
	Vec3 m_Pos;					// �ʒu
	Vec3 m_NormaDirect[3];		// �����x�N�g��
	float m_fLength[3];         // �e�������̒���
public:
	//XMfloat3 Pos �ʒu, XMMATRIX rotM ��]�s��Afloat x�̔����̒����A float y�̔����̒����A float z�̔����̒����A
	void Initilize(const Vec3 &pos, const Vec3 &rotM, const Vec3 &scale);

	void SetPos(const Vec3 &pos) { m_Pos = pos; }
	Vec3 GetDirect (int elem)const; // �w�莲�ԍ��̕����x�N�g�����擾
	float GetLen_W (int elem)const; // �w�莲�����̒������擾
	Vec3 GetPos_W();				// �ʒu���擾
};

class OBBCollision :OBB
{
public:
	static bool ColOBBs(OBB& obb1, OBB& obb2);

	static float LenSegOnSeparateAxis(Vec3& Sep, Vec3& e1, Vec3& e2, Vec3 e3 = { 0,0,0 });
};

