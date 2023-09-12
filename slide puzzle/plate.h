#pragma once
#include<Object.h>
#include <array>
#include <iostream>
#include <algorithm>
#include<Audio.h>
#include"Easing.h"

//�p�l���̎��
enum PanelStatus
{
	NONE,
	WIDTHSTRAIGHTLINE,    //������
	HEIGHTSTRAIGHTLINE,   //�c����
	CROSS,                //�\��
	CUR_LEFTUP,           //�J�[�u���Ə�
	CUR_LEFTDOWN,         //�J�[�u���Ɖ�
	CUR_RIGHTUP,          //�J�[�u�E�Ə�
	CUR_RIGHTDOWN,        //�J�[�u�E�Ɖ�
	DCUR_LEFTUP_RIGHTDOWN,//�_�u���J�[�u���Əさ�E�Ɖ�
	DCUR_LEFTDOWN_RIGHTUP,//�_�u���J�[�u���Ɖ����E�Ə�
};

enum Crystal
{
	NOCRYSTALL,
	CRYSTALL,
};

/// <summary>
/// �p�l��
/// </summary>
class Plate
{
public:
	struct StageBlockData
	{
		std::vector<Vec3> position;
		std::vector<PanelStatus> blockType;
		std::vector<Crystal>crytallFlag;
	};

public:
	//�R���X�g���N�^
	Plate();
	//�ł��R���X�g���N�^
	~Plate();
	/// <summary>
	/// ������
	/// </summary>
	void Init();
	/// <summary>
	/// �X�V
	/// </summary>
	void Update();
	//�`��
	void Draw();


	void SetKeyFlag(bool num) { keyFlag_ = num; }

	void AddSetSelectBlockNumber(int num);

	void DeleteCrstal(int num);

	size_t GetPanelNum() { return blockData_.blockType.size(); }

	Vec3 GetPanelPos(int i) { return  blockData_.position[i]; }

	PanelStatus GetPanelStatus(int i) { return blockData_.blockType[i]; }

	Crystal GetCrystal(int i) { return blockData_.crytallFlag[i]; }

private:

	void None();

	void BeforeMove();

	void Move();

	void Selection();

	void SelectionMove();

	void NullStageBlockNumber();

	int GetStageBlockNumber(int num);

	int GetSaveBlockNumber(int num);

	bool LeftRight(int count, int number, int num, bool flag);

	bool UpDown(int count, int number, int num, bool flag);

	bool SelectionBlockCount();

	void CrystalRote();

	void CrystalCreate();

private:
	ObjectData container;//3x4�̓��ꕨ
	ObjectData plateCross;//�\���̃v���[�g
	ObjectData plateDoubleTurn;//�_�u���J�[�u
	ObjectData plateLine;      //�꒼��
	ObjectData plateSingleTurn;//�P��J�[�u
	ObjectData crystallObject;//�N���X�^��
	ObjectData landmarkObject;
	TextureData landmarkTex = {};
	Vec3 crystalRote = {};
	const Vec3 crystalRoteSpeed = {0.0f,1.0f,0.0f};
	SoundData decide = {};

	// �֐��̊Ǘ�
	std::vector<std::function<void()>> func_;
	// �֐��̔ԍ�
	size_t phase_ = 0;
	// �L�[�t���O
	bool keyFlag_ = false;
	// �X�e�[�W��̃u���b�N�̃f�[�^
	StageBlockData blockData_;
	// �X�e�[�W�̈ʒu�̔z��
	std::vector<Vec3> seaveStageBlockPosition_;
	// �������u���b�N�̔ԍ�
	std::vector<int> moveBlockNumber_;
	// ��̃u���b�N�̔ԍ�
	std::vector<int> nullBlockNumber_;
	// �I�����Ă���X�e�[�W�̈ʒu�̔ԍ�
	int selectionStageNumber_ = 4;
	// �I�����Ă���u���b�N�̔ԍ�
	int selectionBlockNumber_ = 0;
	// �I�����Ă���u���b�N�̔ԍ���ۑ�
	int saveSelectionBlockNumber_ = 0;
	// �I�����Ă���u���b�N�̈ʒu
	Vec3 selectionBlockPosition_ = {};
	// �C�[�W���O�̃f�[�^
	std::unique_ptr<EaseData> easeData_;
};

