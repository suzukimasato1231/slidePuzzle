#pragma once
//�����蔻��v���~�e�B�u

#include<DirectXMath.h>
/// <summary>
/// ��
/// </summary>
struct Sphere
{
	//���S���W
	DirectX::XMVECTOR center = { 0,0,0,1 };
	//���a
	float radius = 1.0f;
};

struct Plane
{
	//�@���x�N�g��
	DirectX::XMVECTOR normal = { 0,1,0,0 };
	//���_�i0,0,0�j����̋���
	float distance = 0.0f;
};

/// <summary>
/// �@���t���O�p�`
/// </summary>
class Triangle
{
public:
	//���_���W�R��
	DirectX::XMVECTOR p0;
	DirectX::XMVECTOR p1;
	DirectX::XMVECTOR p2;
	//�@���x�N�g��
	DirectX::XMVECTOR normal;
};

/// <summary>
/// ���C�i�������j
/// </summary>
struct Ray
{
	//�n�_���W
	DirectX::XMVECTOR start = { 0,0,0,1 };
	//����
	DirectX::XMVECTOR dir = { 1.0,0,0 };
};

/// <summary>
/// ������
/// </summary>
struct Box
{
	//�ŏ��lx,y,z
	DirectX::XMVECTOR minPosition;
	//�ő�lx,y,z
	DirectX::XMVECTOR maxPosition;
};

/// <summary>
/// �J�v�Z��
/// </summary>
struct Capsule
{
	//�J�n���W
	DirectX::XMVECTOR startPosition;
	//�I�����W
	DirectX::XMVECTOR endPosition;
	//���a
	float radius;
};