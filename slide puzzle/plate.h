#pragma once
#include<Object.h>
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
/// <summary>
/// �p�l��
/// </summary>
class Plate
{
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

	int GetStage(int i, int j) { return stage[j][i]; }
private:
	ObjectData container;//3x4�̓��ꕨ
	ObjectData plateCross;//�\���̃v���[�g
	ObjectData plateDoubleTurn;//�_�u���J�[�u
	ObjectData plateLine;      //�꒼��
	ObjectData plateSingleTurn;//�P��J�[�u

	int stage[3][4] = {
		CUR_RIGHTDOWN,WIDTHSTRAIGHTLINE,WIDTHSTRAIGHTLINE,CUR_LEFTDOWN,
		 HEIGHTSTRAIGHTLINE,CROSS,NONE,HEIGHTSTRAIGHTLINE,
		CUR_RIGHTUP,WIDTHSTRAIGHTLINE,WIDTHSTRAIGHTLINE,CUR_LEFTUP,
	};
};

