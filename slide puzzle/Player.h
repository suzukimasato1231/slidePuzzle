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
	/// <summary>
	/// ���Z�b�g
	/// </summary>
	void Reset();
	/// <summary>
	/// ���S�t���O�擾
	/// </summary>
	/// <returns></returns>
	bool GetDeadFlag() { return isDead; }
	/// <summary>
	/// �N���X�^���l����
	/// </summary>
	/// <returns></returns>
	int GetCrstal() { return crstalNum; }
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
	/// �N���X�^���l��
	/// </summary>
	void CrstalGet(Plate* plate);
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
	void TurnLeft(const Vec2 platePos);
	/// <summary>
	/// �^�[���E
	/// </summary>
	void TurnRight(const Vec2 platePos);
	/// <summary>
	/// �^�[����
	/// </summary>
	void TurnUp(const Vec2 platePos);
	/// <summary>
	/// �^�[����
	/// </summary>
	void TurnDown(const Vec2 platePos);
	/// <summary>
	/// �^�[���E������鍶��
	/// </summary>
	void TurnLeftDown(const Vec2 platePos);
	/// <summary>
	/// �^�[�����������E��ݒ�
	/// </summary>
	void TurnUpRight(const Vec2 platePos);
	/// <summary>
	/// �^�[�����������Esita�ݒ�
	/// </summary>
	void TurnRightDown(const Vec2 platePos);
	/// <summary>
	/// �^�[����������鍶��ݒ�
	/// </summary>
	void TurnUpLeft(const Vec2 platePos);
	/// <summary>
	/// �^�[���ォ����鍶���ݒ�
	/// </summary>
	void TurnDownLeft(const Vec2 platePos);
	/// <summary>
	/// �^�[�����������E��ݒ�
	/// </summary>
	void TurnRightUp(const Vec2 platePos);
	/// <summary>
	/// �^�[���E������鍶��ݒ�
	/// </summary>
	void TurnLeftUp(const Vec2 platePos);
	/// <summary>
	/// �^�[���ォ�����E���ݒ�
	/// </summary>
	void TurnDownRight(const Vec2 platePos);
private:
	ObjectData pObject;                         //�v���C���[�I�u�W�F�N�g
	Vec3 position = {};                         //�ʒu
	Vec3 positionMemory = {};
	Vec3 rotation = {};
	int direction = UP;                         //�i�ތ���
	int keepDirection = UP;

	//�v���[�g��b�ʒu
	const Vec2 basePos = { -9.0f,6.0f };
	const Vec2 varPos = { 6.1f,6.1f };
	Vec2 pSize = { 0.001f,0.001f }, sSize = { 6.1f,6.1f };

	bool isDead = false;                        //���񂾂�


	//�N���X�^���֘A
	int crstalNum = 0;;//�l��������
	const float speedPlas = 0.004f;//�X�s�[�hUP��
	float turnSpeedMin = 0.01f;//�ŏ��X�s�[�h��
	float turnSpeed = 0.01f;//�X�s�[�h

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

