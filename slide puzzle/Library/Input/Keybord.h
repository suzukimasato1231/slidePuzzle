#pragma once
//�L�[�{�[�h����
#include<dinput.h>
#include<wrl.h>
#include <DirectXMath.h>
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#include"Window.h"
//#define DIRECTINPUT_VERSION 0x0800//DirectInput�̃o�[�W�����w��
/// <summary>
/// �L�[�{�[�h�N���X
/// </summary>
class Keybord
{
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
private://�����o�ϐ�
	//�S�L�[�̓��͏�Ԃ��擾
	BYTE m_key[256]={};
	//1�O�̓��͏�Ԃ��擾
	BYTE m_oldkey[256]={};
	//�L�[�{�[�h����
	ComPtr<IDirectInput8> m_dinput = nullptr;
	//�L�[�{�[�h�f�o�C�X�̐���
	ComPtr<IDirectInputDevice8> m_devkeyboard = nullptr;
public://�����o�֐�
	//������
	void Initialize();
	//�X�V
	void Update();

	//�������u��
	bool PushKey(BYTE keyNumber)const;
	//�������u��
	bool TriggerKey(BYTE keyNumber)const;
};