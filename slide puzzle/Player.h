#pragma once
#include"Object.h"

class Plate;
/// <summary>
/// �v���C���[�N���X
/// </summary>
class Player
{
private:
	/// <summary>
	/// �v���C���[���i�ތ���
	/// </summary>
	enum Direction
	{
		UP,
		DOWN,
		LEFT,
		RIGHT,
		LEFTUP,
		RIGHTUP,
		LEFTDOWN,
		RIGHTDOWN,
	};
public:
	//�R���X�g���N�^
	Player();
	//�f�X�R���X�g���N�^
	~Player();
	/// <summary>
	/// ������
	/// </summary>
	void Init();
	/// <summary>
	/// �X�V
	/// </summary>
	void Update(Plate *plate);
	/// <summary>
	/// �`��
	/// </summary>
	void Draw();
private:
	/// <summary>
	/// �����`�F���W
	/// </summary>
	void DirectChange(Plate* plate);
	/// <summary>
	/// �v���C���[�ړ�
	/// </summary>
	void Move();
private:
	ObjectData pObject;                         //�v���C���[�I�u�W�F�N�g

	const Vec3 firstPos = { -9.0f,0.0f,0.0f };  //�ŏ��̈ʒu
	Vec3 position = firstPos;                   //�ʒu
	const float FixedSpeed = 1.0f;              //�Œ�X�s�[�h
	float speed = 0.05f;                         //�X�s�[�h
	float moveAngle = 0.0f;                     //�i�ފp�x
	int direction = UP;                         //�i�ތ���

	bool isDead = false;                        //���񂾂�
};

