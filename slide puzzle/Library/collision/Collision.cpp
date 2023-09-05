#include "Collision.h"
#include "../Helper/Vec.h"

using namespace DirectX;

void Collision::ClosestPtPoint2Triangle(const DirectX::XMVECTOR& point, const Triangle& triangle, DirectX::XMVECTOR* closest)
{
	// point��p0�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	XMVECTOR p0_p1 = triangle.p1 - triangle.p0;
	XMVECTOR p0_p2 = triangle.p2 - triangle.p0;
	XMVECTOR p0_pt = point - triangle.p0;

	float d1 = XMVector3Dot(p0_p1, p0_pt).m128_f32[0];
	float d2 = XMVector3Dot(p0_p2, p0_pt).m128_f32[0];

	if (d1 <= 0.0f && d2 <= 0.0f)
	{
		// p0���ŋߖT
		*closest = triangle.p0;
		return;
	}

	// point��p1�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	XMVECTOR p1_pt = point - triangle.p1;

	float d3 = XMVector3Dot(p0_p1, p1_pt).m128_f32[0];
	float d4 = XMVector3Dot(p0_p2, p1_pt).m128_f32[0];

	if (d3 >= 0.0f && d4 <= d3)
	{
		// p1���ŋߖT
		*closest = triangle.p1;
		return;
	}

	// point��p0_p1�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p0_p1��ɑ΂���ˉe��Ԃ�
	float vc = d1 * d4 - d3 * d2;
	if (vc <= 0.0f && d1 >= 0.0f && d3 <= 0.0f)
	{
		float v = d1 / (d1 - d3);
		*closest = triangle.p0 + v * p0_p1;
		return;
	}

	// point��p2�̊O���̒��_�̈�̒��ɂ��邩�ǂ����`�F�b�N
	XMVECTOR p2_pt = point - triangle.p2;

	float d5 = XMVector3Dot(p0_p1, p2_pt).m128_f32[0];
	float d6 = XMVector3Dot(p0_p2, p2_pt).m128_f32[0];
	if (d6 >= 0.0f && d5 <= d6)
	{
		*closest = triangle.p2;
		return;
	}

	// point��p0_p2�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p0_p2��ɑ΂���ˉe��Ԃ�
	float vb = d5 * d2 - d1 * d6;
	if (vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
	{
		float w = d2 / (d2 - d6);
		*closest = triangle.p0 + w * p0_p2;
		return;
	}

	// point��p1_p2�̕ӗ̈�̒��ɂ��邩�ǂ����`�F�b�N���A�����point��p1_p2��ɑ΂���ˉe��Ԃ�
	float va = d3 * d6 - d5 * d4;
	if (va <= 0.0f && (d4 - d3) >= 0.0f && (d5 - d6) >= 0.0f)
	{
		float w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
		*closest = triangle.p1 + w * (triangle.p2 - triangle.p1);
		return;
	}

	// point�͖ʗ̈�̒��ɂ���Bclosest���d�S���W��p���Čv�Z����
	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;
	*closest = triangle.p0 + p0_p1 * v + p0_p2 * w;
}

bool Collision::CheckSphere2Sphere(const Sphere& sphereA, const Sphere& sphereB, DirectX::XMVECTOR* inter)
{
	// ���S�_�̋����̂Q�� <= ���a�̘a�̂Q��@�Ȃ����
	float dist = XMVector3LengthSq(sphereA.center - sphereB.center).m128_f32[0];

	float radius2 = sphereA.radius + sphereB.radius;
	radius2 *= radius2;

	if (dist <= radius2) {
		if (inter) {
			// A�̔��a��0�̎����W��B�̒��S�@B�̔��a��0�̎����W��A�̒��S�@�ƂȂ�悤�⊮
			float t = sphereB.radius / (sphereA.radius + sphereB.radius);
			*inter = XMVectorLerp(sphereA.center, sphereB.center, t);
		}
		return true;
	}

	return false;
}

bool Collision::CheckSphere2Plane(const Sphere& sphere, const Plane& plane, DirectX::XMVECTOR* inter)
{
	// ���W�n�̌��_���狅�̒��S���W�ւ̋�������
	// ���ʂ̌��_���������Z���邱�ƂŁA���ʂƋ��̒��S�Ƃ̋������o��
	float dist = XMVector3Dot(sphere.center, plane.normal).m128_f32[0] - plane.distance;
	// �����̐�Βl�����a���傫����Γ������Ă��Ȃ�
	if (fabsf(dist) > sphere.radius)	return false;

	// �[����_���v�Z
	if (inter)
	{
		// ���ʏ�̍Đڋߓ_���A�^����_�Ƃ���
		*inter = -dist * plane.normal + sphere.center;                                                                                              +sphere.center;
	}

	return true;
}

bool Collision::CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle, DirectX::XMVECTOR* inter, DirectX::XMVECTOR* reject)
{
	XMVECTOR p;
	// ���̒��S�ɑ΂���ŋߐړ_�ł���O�p�`��ɂ���_p��������
	ClosestPtPoint2Triangle(sphere.center, triangle, &p);
	// �_p�Ƌ��̒��S�̍����x�N�g��
	XMVECTOR v = p - sphere.center;
	// �����̓������߂�
	//�i�����x�N�g�����m�̓��ς͎O�����̒藝�̃��[�g�����̎��ƈ�v����j
	float distanceSquare = XMVector3Dot(v, v).m128_f32[0];
	// ���ƎO�p�`�̋��������a�ȉ��Ȃ瓖�����Ă��Ȃ�
	if (distanceSquare > sphere.radius * sphere.radius)	return false;
	// �[����_���v�Z
	if (inter) {
		// �O�p�`��̍ŋߐړ_p���^����_�Ƃ���
		*inter = p;
	}
	// �����o���x�N�g�����v�Z
	if (reject) {
		float ds = XMVector3Dot(sphere.center, triangle.normal).m128_f32[0];
		float dt = XMVector3Dot(triangle.p0, triangle.normal).m128_f32[0];
		float rejectLen = dt - ds + sphere.radius;
		*reject = triangle.normal * rejectLen;
	}
	return true;
}

bool Collision::CheckRay2Plane(const Ray& lay, const Plane& plane, float* distance, DirectX::XMVECTOR* inter)
{
	const float epsilon = 1.0e-5f;	// �덷�z���p�̔����Ȓl

	float d1 = XMVector3Dot(plane.normal, lay.dir).m128_f32[0];
	// ���ʂɂ͓�����Ȃ�
	if (d1 > -epsilon) {
		return false;
	}

	float d2 = XMVector3Dot(plane.normal, lay.start).m128_f32[0];
	float t = (plane.distance - d2) / d1;

	if (t < 0) return false;

	// ��������������
	if (distance) {
		*distance = t;
	}

	// ��_���v�Z
	if (inter) {
		*inter = lay.start + t * lay.dir;
	}

	return true;
}

bool Collision::CheckRay2Triangle(const Ray& lay, const Triangle& triangle, float* distance, DirectX::XMVECTOR* inter)
{
	// �O�p�`������Ă��镽�ʂ��Z�o
	Plane plane;
	XMVECTOR interPlane;
	plane.normal = triangle.normal;
	plane.distance = XMVector3Dot(triangle.normal, triangle.p0).m128_f32[0];
	// ���C�ƕ��ʂ��������Ă��Ȃ���΁A�������Ă��Ȃ�	
	if (!CheckRay2Plane(lay, plane, distance, &interPlane)) {
		return false;
	}
	// ���C�ƕ��ʂ��������Ă����̂ŁA�����ƌ�_���������܂ꂽ

	// ���C�ƕ��ʂ̌�_���O�p�`�̓����ɂ��邩����
	const float epsilon = 1.0e-5f;	// �덷�z���p�̔����Ȓl
	XMVECTOR m;
	// ��p0_p1�ɂ���
	XMVECTOR pt_p0 = triangle.p0 - interPlane;
	XMVECTOR p0_p1 = triangle.p1 - triangle.p0;
	m = XMVector3Cross(pt_p0, p0_p1);
	// �ӂ̊O��
	if (XMVector3Dot(m, triangle.normal).m128_f32[0] < -epsilon) {
		return false;
	}

	// ��p1_p2�ɂ���
	XMVECTOR pt_p1 = triangle.p1 - interPlane;
	XMVECTOR p1_p2 = triangle.p2 - triangle.p1;
	m = XMVector3Cross(pt_p1, p1_p2);
	// �ӂ̊O��
	if (XMVector3Dot(m, triangle.normal).m128_f32[0] < -epsilon) {
		return false;
	}

	// ��p2_p0�ɂ���
	XMVECTOR pt_p2 = triangle.p2 - interPlane;
	XMVECTOR p2_p0 = triangle.p0 - triangle.p2;
	m = XMVector3Cross(pt_p2, p2_p0);
	// �ӂ̊O��
	if (XMVector3Dot(m, triangle.normal).m128_f32[0] < -epsilon) {
		return false;
	}

	if (inter) {
		*inter = interPlane;
	}

	// �����Ȃ̂ŁA�������Ă���
	return true;
}

bool Collision::CheckRay2Sphere(const Ray& lay, const Sphere& sphere, float* distance, DirectX::XMVECTOR* inter)
{
	XMVECTOR m = lay.start - sphere.center;
	float b = XMVector3Dot(m, lay.dir).m128_f32[0];
	float c = XMVector3Dot(m, m).m128_f32[0] - sphere.radius * sphere.radius;
	// lay�̎n�_��sphere�̊O���ɂ���(c > 0)�Alay��sphere���痣��Ă���������
	// �����Ă���ꍇ(b > 0)�A������Ȃ�
	if (c > 0.0f && b > 0.0f) {
		return false;
	}

	float discr = b * b - c;
	// ���̔��ʎ��̓��C�������O��Ă��邱�ƂɈ�v
	if (discr < 0.0f) {
		return false;
	}

	// ���C�͋��ƌ������Ă���B
	// ��������ŏ��̒lt���v�Z
	float t = -b - sqrtf(discr);
	// t�����ł���ꍇ�A���C�͋��̓�������J�n���Ă���̂�t���[���ɃN�����v
	if (t < 0) t = 0.0f;

	if (distance) {
		*distance = t;
	}

	if (inter) {
		*inter = lay.start + t * lay.dir;
	}

	return true;
}

//���Ƌ��̓����蔻��
bool Collision::CheckSphere2Sphere(const Sphere& sphere1, const Sphere& sphere2)
{
	//�Q�̋��̒��S���W�����ԃx�N�g�������߂�
	XMVECTOR vec = sphere1.center - sphere2.center;

	//�x�N�g���̒����̂Q��i�Q�_�Ԃ̋����̂Q��j�����߂�
	float sqLength = vec.m128_f32[0] * vec.m128_f32[0] + vec.m128_f32[1] * vec.m128_f32[1] + vec.m128_f32[2] * vec.m128_f32[2];

	//2�̋��̔��a�̍��v�����߂�
	float r = sphere1.radius + sphere2.radius;

	//�Q�_�Ԃ̋����̂Q�恃���a�̍��v�̂Q��Ȃ�Q�̋��͌������Ă���
	return  sqLength < r* r;
}

bool Collision::CheckBox2Box(const Box& box1, const Box& box2)
{
	//x���̔���
	if (box2.minPosition.m128_f32[0] <= box1.maxPosition.m128_f32[0] && box1.minPosition.m128_f32[0] <= box2.maxPosition.m128_f32[0])
	{//y��
		if (box2.minPosition.m128_f32[1] <= box1.maxPosition.m128_f32[1] && box1.minPosition.m128_f32[1] <= box2.maxPosition.m128_f32[1])
		{
			//z���̔���
			if (box2.minPosition.m128_f32[2] <= box1.maxPosition.m128_f32[2] && box1.minPosition.m128_f32[2] <= box2.maxPosition.m128_f32[2])
			{

				return 1;
			}
		}
	}
	return 0;
}


//���ƒ����́iAABB�j�̏Փ˔���
bool Collision::CheckSphere2Box(const Sphere& sphere, const Box& box)
{
	//�܂��A���̒��S���W��AABB�Ƃ̋����̂Q������߂�
	float sqDistance = 0.0f;
	float pos;

	//x�������̋����̂Q������Z
	pos = sphere.center.m128_f32[0];
	if (pos < box.minPosition.m128_f32[0])
	{
		sqDistance += (box.minPosition.m128_f32[0] - pos) * (box.minPosition.m128_f32[0] - pos);
	}
	else if (pos > box.maxPosition.m128_f32[0])
	{
		sqDistance += (box.maxPosition.m128_f32[0] - pos) * (box.maxPosition.m128_f32[0] - pos);
	}

	//y�������̋����̂Q������Z
	pos = sphere.center.m128_f32[1];
	if (pos < box.minPosition.m128_f32[1])
	{
		sqDistance += (box.minPosition.m128_f32[1] - pos) * (box.minPosition.m128_f32[1] - pos);
	}
	else if (pos > box.maxPosition.m128_f32[1])
	{
		sqDistance += (box.maxPosition.m128_f32[1] - pos) * (box.maxPosition.m128_f32[1] - pos);
	}
	pos = sphere.center.m128_f32[2];
	//z�������̋����̂Q������Z
	if (pos < box.minPosition.m128_f32[2])
	{
		sqDistance += (box.minPosition.m128_f32[2] - pos) * (box.minPosition.m128_f32[2] - pos);
	}
	else if (pos > box.maxPosition.m128_f32[2])
	{
		sqDistance += (box.maxPosition.m128_f32[2] - pos) * (box.maxPosition.m128_f32[2] - pos);
	}


	//��L�ŋ��߂��l�����a�̂Q��Ȃ�A�Փ˂��Ă���
	return sqDistance < sphere.radius* sphere.radius;
}

bool Collision::CheckSphere2Capsule(const Sphere& sphere, const Capsule& capsule)
{
	Vec3 SS = { sphere.center.m128_f32[0],sphere.center.m128_f32[1] ,sphere.center.m128_f32[2] };
	Vec3 CS = { capsule.startPosition.m128_f32[0], capsule.startPosition.m128_f32[1], capsule.startPosition.m128_f32[2] };
	Vec3 CE = { capsule.endPosition.m128_f32[0], capsule.endPosition.m128_f32[1], capsule.endPosition.m128_f32[2] };

	//�P�D�J�v�Z�����̐����̃X�^�[�g�ʒu����G���h�ʒu�ւ̃x�N�g�������
	Vec3 vStartToEnd = CS - CE;

	//2.1.�̃x�N�g����P�ʃx�N�g���inormalize�j���An�ɗp�ӂ���
	Vec3 n = vStartToEnd;
	n.normalize();

	//3.Ps->Pc�ւ̃x�N�g���ƂQ�D�ŋ��߂�n�Ƃ̓��ς��v�Z����
	//�i�����n�����{����΃x�N�g��Ps->Pn�ɂȂ邩�{���it�j�����܂�j

	float t = n.dot(Vec3(CS - SS));

	//4.Ps->Pn�x�N�g�������߂Ă����A�܂��APn�̍��W�����߂Ă���
	Vec3 vPsPn = n * t;
	Vec3 posPn = CS - vPsPn;

	//5.�䗦t/(Ps->Pe�̒���)�����߂�
	float lengthRate = t / vStartToEnd.length();

	//6.lengthRate<0,0<=lengthRate<=1,1<lengthRate�ŏꍇ����
	float distance;
	if (lengthRate < 0.0f)
	{
		distance = (CS - SS).length() - capsule.radius;
	}
	else if (lengthRate <= 1.0f)
	{
		distance = (posPn - SS).length() - capsule.radius;
	}
	else
	{
		distance = (CE - SS).length() - capsule.radius;
	}

	return distance < sphere.radius;
}

bool Collision::CheckCapsule2Capsule(const Capsule &capsule1, const Capsule& capsule2)
{
	//�����Ɛ����̋����𒲂ׂ�@���ʂ͋����̂Q��̒l�ł���
	float sqDistance = sqDistanceSegmentSegment(capsule1.startPosition, capsule1.endPosition, capsule2.startPosition, capsule2.endPosition);


	//�Q�̃J�v�Z���̔��a�̘a�����߂�
	float r = capsule1.radius + capsule2.radius;

	return sqDistance < r* r;
}

float Collision::sqDistanceSegmentSegment(const XMVECTOR& p1, const XMVECTOR& q1, const XMVECTOR& p2, const XMVECTOR& q2)
{
	Vec3 p1_ = { p1.m128_f32[0],p1.m128_f32[1],p1.m128_f32[2] };
	Vec3 p2_ = { p2.m128_f32[0],p2.m128_f32[1],p2.m128_f32[2] };
	Vec3 q1_ = { q1.m128_f32[0],q1.m128_f32[1],q1.m128_f32[2] };
	Vec3 q2_ = { q2.m128_f32[0],q2.m128_f32[1],q2.m128_f32[2] };

	Vec3 d1 = q1_ - p1_;//p1->q1�̃x�N�g��
	Vec3 d2 = q2_ - p2_;//p2->q2�̃x�N�g��

	Vec3 r = p1_ - p2_;

	float a = d1.dot(d1);//a = d1*d1
	float b = d1.dot(d2);//b = d1*d2
	float e = d2.dot(d2);//e = d2*d2

	float c = d1.dot(r);
	float f = d2.dot(r);

	float s = 0.0f;

	float t = 0.0f;

	float denominator = a * e - b * b;//����

	//s�̒l�����߂�
	if (denominator != 0.0f)
	{
		s = (b * f - c * e) / denominator;
		s = clamp(s, 0.0f, 1.0f);
	}
	else//2�̐��������s�ł���Ƃ�
	{
		s = 0.0f;
	}
	//t�̒l�����߂�
	//t = ((p1+d1*s)-p2)*d2/(d2*d2)
	t = (f + b * s) / e;

	//t��[0.0,1.0]�͈̔͊O�ł���΁As���Čv�Z����
	//s = ((p2+d2*t)-p1)*d1/(d1*d1) = (t*b-c)/a
	if (t < 0.0f)
	{
		t = 0.0f;
		s = clamp(-c / a, 0.0f, 1.0f);
	}
	else if (t > 1.0f)
	{
		s = clamp((b - c) / a, 0.0f, 1.0f);
		t = 1.0f;
	}
	//s��t�̒l�����肵���̂ŁA�e�������̍��Wc1��c2�����߂�
	Vec3 c1 = p1_ + d1 * s;
	Vec3 c2 = p2_ + d2 * t;

	//2�_��(c1��c2)�̋����̂Q������߂āA���ʂ�Ԃ�
	return (c1 - c2).dot(c1 - c2);
}

bool Collision::CircleCollision(const Vec2& circle1, Vec2& circle2, float radius1, float radius2)
{
	double disX = circle1.x - circle2.x;
	double disY = circle1.y - circle2.y;
	double d = disX * disX + disY * disY;
	double r = (radius1 + radius2) * (radius1 + radius2);
	return d < r;
}

bool Collision::BoxCollision(const Vec2 &box1, const Vec2 &box2, const Vec2 &size1, const Vec2 &size2)
{
	//x���̔���
	if (box2.x - size2.x <= box1.x + size1.x && box1.x - size1.x <= box2.x + size2.x)
	{//y��
		if (box2.y - size2.y <= box1.y + size1.y && box1.y - size1.y <= box2.y + size2.y)
		{
			return 1;
		}
	}
	return 0;
}



void OBB::Initilize(const Vec3 &pos, const Vec3 &rotation, const Vec3 &scale)
{
	//���肷��OBB�̏����R�s�[
	m_Pos.x = pos.x;
	m_Pos.y = pos.y;
	m_Pos.z = pos.z;


	XMMATRIX rotM = XMMatrixIdentity();
	rotM *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));//Z���܂��ɂS�T�x��]
	rotM *= XMMatrixRotationX(XMConvertToRadians(rotation.x));//X���܂��ɂS�T�x��]
	rotM *= XMMatrixRotationY(XMConvertToRadians(rotation.y));//Y���܂��ɂS�T�x��]

	m_NormaDirect[0].x = rotM.r[0].m128_f32[0];
	m_NormaDirect[0].y = rotM.r[0].m128_f32[1];
	m_NormaDirect[0].z = rotM.r[0].m128_f32[2];

	m_NormaDirect[1].x = rotM.r[1].m128_f32[0];
	m_NormaDirect[1].y = rotM.r[1].m128_f32[1];
	m_NormaDirect[1].z = rotM.r[1].m128_f32[2];

	m_NormaDirect[2].x = rotM.r[2].m128_f32[0];
	m_NormaDirect[2].y = rotM.r[2].m128_f32[1];
	m_NormaDirect[2].z = rotM.r[2].m128_f32[2];

	m_fLength[0] = scale.x / 2;
	m_fLength[1] = scale.y / 2;
	m_fLength[2] = scale.z / 2;
}


Vec3 OBB::GetDirect(int elem)const
{
	switch (elem)
	{
	case 0:
		return m_NormaDirect[0];
		break;
	case 1:
		return  m_NormaDirect[1];
		break;
	case 2:
		return  m_NormaDirect[2];
		break;
	default:
		return Vec3();
		break;
	}
}

float OBB::GetLen_W(int elem)const
{
	switch (elem)
	{
	case 0:
		return  m_fLength[0];
		break;
	case 1:
		return  m_fLength[1];
		break;
	case 2:
		return  m_fLength[2];
		break;
	default:
		return 0;
		break;
	}
}

//���W���擾
Vec3 OBB::GetPos_W()
{
	return m_Pos;
}

// OBB v.s. OBB
bool OBBCollision::ColOBBs(OBB& obb1, OBB& obb2)
{
	// �e�����x�N�g���̊m��
	Vec3 NAe1 = obb1.GetDirect(0), Ae1 = NAe1 * obb1.GetLen_W(0);
	Vec3 NAe2 = obb1.GetDirect(1), Ae2 = NAe2 * obb1.GetLen_W(1);
	Vec3 NAe3 = obb1.GetDirect(2), Ae3 = NAe3 * obb1.GetLen_W(2);
	Vec3 NBe1 = obb2.GetDirect(0), Be1 = NBe1 * obb2.GetLen_W(0);
	Vec3 NBe2 = obb2.GetDirect(1), Be2 = NBe2 * obb2.GetLen_W(1);
	Vec3 NBe3 = obb2.GetDirect(2), Be3 = NBe3 * obb2.GetLen_W(2);
	Vec3 Interval = obb1.GetPos_W() - obb2.GetPos_W();

	// ������ : Ae1
	float rA = Ae1.length();
	float rB = LenSegOnSeparateAxis(NAe1, Be1, Be2, Be3);
	float L = fabsf(Interval.dot(NAe1));
	if (L > rA + rB)
		return false; // �Փ˂��Ă��Ȃ�

	 // ������ : Ae2
	rA = Ae2.length();
	rB = LenSegOnSeparateAxis(NAe2, Be1, Be2, Be3);
	L = fabsf(Interval.dot(NAe2));
	if (L > rA + rB)
		return false;

	// ������ : Ae3
	rA = Ae3.length();
	rB = LenSegOnSeparateAxis(NAe3, Be1, Be2, Be3);
	L = fabsf(Interval.dot(NAe3));
	if (L > rA + rB)
		return false;

	// ������ : Be1
	rA = LenSegOnSeparateAxis(NBe1, Ae1, Ae2, Ae3);
	rB = Be1.length();
	L = fabsf(Interval.dot(NBe1));
	if (L > rA + rB)
		return false;

	// ������ : Be2
	rA = LenSegOnSeparateAxis(NBe2, Ae1, Ae2, Ae3);
	rB = Be2.length();
	L = fabsf(Interval.dot(NBe2));
	if (L > rA + rB)
		return false;

	// ������ : Be3
	rA = LenSegOnSeparateAxis(NBe3, Ae1, Ae2, Ae3);
	rB = Be3.length();
	L = fabsf(Interval.dot(NBe3));
	if (L > rA + rB)
		return false;

	// ������ : C11
	Vec3 Cross;
	Cross = NAe1.cross(NBe1);
	rA = LenSegOnSeparateAxis(Cross, Ae2, Ae3);
	rB = LenSegOnSeparateAxis(Cross, Be2, Be3);
	L = fabsf(Interval.dot(Cross));
	if (L > rA + rB)
		return false;

	// ������ : C12
	Cross = NAe1.cross(NBe2);
	rA = LenSegOnSeparateAxis(Cross, Ae2, Ae3);
	rB = LenSegOnSeparateAxis(Cross, Be1, Be3);
	L = fabsf(Interval.dot(Cross));
	if (L > rA + rB)
		return false;

	// ������ : C13
	Cross = NAe1.cross(NBe3);
	rA = LenSegOnSeparateAxis(Cross, Ae2, Ae3);
	rB = LenSegOnSeparateAxis(Cross, Be1, Be2);
	L = fabsf(Interval.dot(Cross));
	if (L > rA + rB)
		return false;

	// ������ : C21
	Cross = NAe2.cross(NBe1);
	rA = LenSegOnSeparateAxis(Cross, Ae1, Ae3);
	rB = LenSegOnSeparateAxis(Cross, Be2, Be3);
	L = fabsf(Interval.dot(Cross));
	if (L > rA + rB)
		return false;

	// ������ : C22
	Cross = NAe2.cross(NBe2);
	rA = LenSegOnSeparateAxis(Cross, Ae1, Ae3);
	rB = LenSegOnSeparateAxis(Cross, Be1, Be3);
	L = fabsf(Interval.dot(Cross));
	if (L > rA + rB)
		return false;

	// ������ : C23
	Cross = NAe2.cross(NBe3);
	rA = LenSegOnSeparateAxis(Cross, Ae1, Ae3);
	rB = LenSegOnSeparateAxis(Cross, Be1, Be2);
	L = fabsf(Interval.dot(Cross));
	if (L > rA + rB)
		return false;

	// ������ : C31
	Cross = NAe3.cross(NBe1);
	rA = LenSegOnSeparateAxis(Cross, Ae1, Ae2);
	rB = LenSegOnSeparateAxis(Cross, Be2, Be3);
	L = fabsf(Interval.dot(Cross));
	if (L > rA + rB)
		return false;

	// ������ : C32
	Cross = NAe3.cross(NBe2);
	rA = LenSegOnSeparateAxis(Cross, Ae1, Ae2);
	rB = LenSegOnSeparateAxis(Cross, Be1, Be3);
	L = fabsf(Interval.dot(Cross));
	if (L > rA + rB)
		return false;

	// ������ : C33
	Cross = NAe3.cross(NBe3);
	rA = LenSegOnSeparateAxis(Cross, Ae1, Ae2);
	rB = LenSegOnSeparateAxis(Cross, Be1, Be2);
	L = fabsf(Interval.dot(Cross));
	if (L > rA + rB)
		return false;

	// �������ʂ����݂��Ȃ��̂Łu�Փ˂��Ă���v
	return true;
}


// �������ɓ��e���ꂽ���������瓊�e���������Z�o
float OBBCollision::LenSegOnSeparateAxis(Vec3& Sep, Vec3& e1, Vec3& e2, Vec3 e3)
{
	// 3�̓��ς̐�Βl�̘a�œ��e���������v�Z
	// ������Sep�͕W��������Ă��邱��
	float r1 = fabsf(Sep.dot(e1));
	float r2 = fabsf(Sep.dot(e2));
	float r3 = &e3 ? (fabsf(Sep.dot(e3))) : 0;
	return r1 + r2 + r3;
}