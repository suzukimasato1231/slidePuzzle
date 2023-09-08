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
	void Update(Plate* plate);
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
	/// <summary>
	/// �J�[�u������
	/// </summary>
	void TurnInit();
	/// <summary>
	/// ����������
	/// </summary>
	void LineInit();
	/// <summary>
	/// �^�[����
	/// </summary>
	void TurnLeft();
	/// <summary>
	/// �^�[���E
	/// </summary>
	void TurnRight();
	/// <summary>
	/// �^�[����
	/// </summary>
	void TurnUp();
	/// <summary>
	/// �^�[����
	/// </summary>
	void TurnDown();
	/// <summary>
	/// �^�[���E������鍶��
	/// </summary>
	void TurnLeftDown();
	/// <summary>
	/// �^�[�����������E��ݒ�
	/// </summary>
	void TurnUpRight();
	/// <summary>
	/// �^�[�����������Esita�ݒ�
	/// </summary>
	void TurnRightDown();
	/// <summary>
	/// �^�[����������鍶��ݒ�
	/// </summary>
	void TurnUpLeft();
	/// <summary>
	/// �^�[���ォ����鍶���ݒ�
	/// </summary>
	void TurnDownLeft();
	/// <summary>
	/// �^�[�����������E��ݒ�
	/// </summary>
	void TurnRightUp();
	/// <summary>
	/// �^�[���E������鍶��ݒ�
	/// </summary>
	void TurnLeftUp();
	/// <summary>
	/// �^�[���ォ�����E���ݒ�
	/// </summary>
	void TurnDownRight();
private:
	ObjectData pObject;                         //�v���C���[�I�u�W�F�N�g
	Vec3 position = {};                         //�ʒu
	Vec3 positionMemory = {};
	Vec3 rotation = {};
	int direction = UP;                         //�i�ތ���

	//�v���[�g��b�ʒu
	const Vec2 basePos = { -9.0f,6.0f };
	const Vec2 varPos = { 6.1f,6.1f };
	Vec2 pSize = { 0.001f,0.001f }, sSize = { 6.1f,6.1f };

	bool isDead = false;                        //���񂾂�


	const float turnSpeed = 0.01f;
	float plateTime = 0.0f;
	//�p�l���ړ��p
	Vec3 posStart = {};
	Vec3 posEnd = {};
	bool isPosFlag = false;

	Vec3 posStartSecond = {};
	Vec3 posEndSecond = {};
	bool isPosSecondFlag = false;

	//�J�[�u�p�ϐ�
	Vec3 turnEnd = {};
	Vec3 turnStart = {};
	bool isCurFlag = false;

	Vec3 turnEndSecond = {};
	Vec3 turnStartSecond = {};
};

