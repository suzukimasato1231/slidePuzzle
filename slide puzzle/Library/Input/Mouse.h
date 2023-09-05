#pragma once
#include<dinput.h>
#include<wrl.h>
#include"Window.h"
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
/// <summary>
/// �}�E�X�N���X
/// </summary>
class Mouse
{
public:
	struct MouseMove {
		LONG    lX;
		LONG    lY;
		LONG    lZ;
	};
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public://�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	/// <returns></returns>
	bool Init();
	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �L�[�̍��{�^���������`�F�b�N
	/// </summary>
	/// <returns>������Ă��邩</returns>
	bool PushMouseLeft()const;

	/// <summary>
	/// �L�[�̒��{�^���������`�F�b�N
	/// </summary>
	/// <returns>������Ă��邩</returns>
	bool PushMouseMiddle()const;

	/// <summary>
	/// �L�[�̍��{�^���g���K�[���`�F�b�N
	/// </summary>
	/// <returns>�g���K�[��</returns>
	bool TriggerMouseLeft()const;

	/// <summary>
	/// �L�[�̒��{�^���g���K�[���`�F�b�N
	/// </summary>
	/// <returns>�g���K�[��</returns>
	bool TriggerMouseMiddle()const;

	/// <summary>
	/// �}�E�X�ړ��ʂ��擾
	/// </summary>
	/// <returns>�}�E�X�ړ���</returns>
	MouseMove GetMouseMove();
private:
	ComPtr<IDirectInput8> m_dinput = nullptr;
	ComPtr<IDirectInputDevice8> m_devMouse;
	DIMOUSESTATE2 m_mouseState = {};
	DIMOUSESTATE2 m_mouseStatePre = {};
};