#pragma once
#include"Object.h"
#include"Sprite.h"
#include<ParticleManager.h>
#include<memory>
#include<Audio.h>
#include <array>
#include <algorithm>
#include <functional>
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

	struct DecoyData
	{
		ObjectData object;
		Vec3 position;
		Vec3 rotation;
		bool flag = false;
		int timer = 0;
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
	/// �X�R�A�`��
	/// </summary>
	void ScoreDraw();
	/// <summary>
	/// ���S�t���O�擾
	/// </summary>
	/// <returns></returns>
	bool GetDeadFlag() { return isDead; }
private:
	/// <summary>
	/// �R���{
	/// </summary>
	void PointUpdate();
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
	/// <summary>
	/// ���񂾂Ƃ��̉�]���o
	/// </summary>
	void DeadRotation();
	/// <summary>
	/// ���R���g���[���ŉ���
	/// </summary>
	void Dash();

	void DecoyUpdate();

private:
	ObjectData pObject;                         //�v���C���[�I�u�W�F�N�g
	Vec3 position = {};                         //�ʒu
	Vec3 positionMemory = {};
	Vec3 rotation = {};
	int direction = UP;                         //�i�ތ���
	int keepDirection = UP;

	//�c��
	std::array<DecoyData, 2> decoyObject_;
	int decoyTimer_ = 0;

	//�v���[�g��b�ʒu
	const Vec2 basePos = { -9.0f,6.0f };
	const Vec2 varPos = { 6.1f,6.1f };
	Vec2 pSize = { 0.001f,0.001f }, sSize = { 6.1f,6.1f };

	bool isDead = false;                        //���񂾂�
	//���֘A
	SoundData crashSound;
	SoundData crystalSound;
	SoundData runSound;
	SoundData turboSound;
	bool turboSoundFlag = false;
	bool runSoundFlag = false;

	//�R���{�֘A
	SpriteData backSprite;
	SpriteData comboSprite;
	SpriteData numberGraph[10] = {};
	const int comboTimeMax = 250;//�R���{�ő�
	int comboTime = 0;           //�R���{����
	int pointNum = 0;            //���_
	int pointPlas = 10;          //���_���Z
	std::unique_ptr < ParticleManager> comboParticle = nullptr;
	TextureData particleGraph = {};

	//�X�R�A�`��
	ObjectData scoreDraw = {};
	SpriteData comboGraph[11] = {};
	Vec3 scorePos = {};
	bool isScoreDraw = false;
	const int scoreDrawTimeMax = 50;
	int scoreDrawTime = 0;


	//�N���X�^���X�s�[�h�֘A
	const float speedPlas = 0.0025f;  //�X�s�[�hUP��
	const float speedMax = 0.06f;     //�ő�X�s�[�h
	const float turnSpeedMin = 0.01f;//�ŏ��X�s�[�h��
	float turnSpeed = 0.01f;         //�X�s�[�h

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

