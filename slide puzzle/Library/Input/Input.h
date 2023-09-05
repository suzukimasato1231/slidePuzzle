#pragma once
#include"Controller.h"
#include "Keybord.h"
#include"Mouse.h"
#include"Singleton.h"
/// <summary>
/// ���̓N���X
/// </summary>
class Input :public Singleton<Input>
{
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	friend Singleton<Input>;
private://�����o�ϐ�
	//�L�[�{�[�h
	Keybord m_keybord;
	//�R���g���[���[
	Controller m_controller;
	//�}�E�X
	Mouse m_mouse;
public://�ÓI�����o�ϐ�
	//static Input *GetInstance();
public://�����o�֐�
	Input() = default;
	Input(const Input &) = delete;
	~Input() = default;
	Input &operator=(const Input &) = delete;
	//������
	void Initialize();
	//�X�V
	void Update();
public://�L�[�{�[�h
	//�����Ă鎞
	bool KeybordPush(BYTE keyNumber)const;
	//�������u��
	bool KeybordTrigger(BYTE keyNumber)const;
	//���L�[�̂����ꂩ�����������ǂ���
	bool KeybordInputArrow()const;
public://�Q�[���p�b�h

	//@brief �{�^����������Ă��邩�̔���֐�
	// @param[in] button ���肵�����{�^���̎��
	bool ControllerPush(ButtonKind button)const;

	// @brief �{�^���������ꂽ�u�Ԃ̔���֐�
	// @param[in] button ���肵�����{�^���̎��
	bool ControllerUp(ButtonKind button)const;

	// @brief �{�^���𗣂����u�Ԃ̔���֐�
	// @param[in] button ���肵�����{�^���̎��
	bool ControllerDown(ButtonKind button)const;
	//���X�e�B�b�N�p�x�擾
	float GetLeftAngle()const;

	float GetRightAngle()const;
	//�E�X�e�B�b�N���͂��Ă邩
	bool ConRightInput()const;

	bool ConLeftInputS()const;

	//���X�e�B�b�N����͂��Ă邩
	bool ConLeftInput()const;
public://�}�E�X
	bool MousePushLeft();

	/// <summary>
	/// �L�[�̒��{�^���������`�F�b�N
	/// </summary>
	/// <returns>������Ă��邩</returns>
	bool MousePushMiddle();

	/// <summary>
	/// �L�[�̍��{�^���g���K�[���`�F�b�N
	/// </summary>
	/// <returns>�g���K�[��</returns>
	bool MouseTriggerLeft();

	/// <summary>
	/// �L�[�̒��{�^���g���K�[���`�F�b�N
	/// </summary>
	/// <returns>�g���K�[��</returns>
	bool MouseTriggerMiddle();

	/// <summary>
	/// �}�E�X�ړ��ʂ��擾
	/// </summary>
	/// <returns>�}�E�X�ړ���</returns>
	Mouse::MouseMove GetMouseMove();
};