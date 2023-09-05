#pragma once
#include<Windows.h>
#include<vector>
#include<string>
#include<d3d12.h>
#include<dxgi1_6.h>
#include"Singleton.h"
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
extern const int window_width;
extern const int window_height;
/// <summary>
/// �E�B���h�E�N���X
/// </summary>
class Window :public Singleton<Window>
{
	friend Singleton<Window>;
public://�����o�֐�
	void CreateGameWindow();

	//���b�Z�[�W����
	bool ProcessMessage();

	//�Q�[���E�B���h�E�̔j��
	void TerminateGameWindow();

	//Hwnd�l��
	HWND GetHwnd()const { return m_hwnd; }

	//w.hInstance�l��
	HINSTANCE GetInstance()const { return m_w.hInstance; }

private://�����o�ϐ�
	// �E�B���h�E�I�u�W�F�N�g�̐���
	HWND m_hwnd = nullptr;
	WNDCLASSEX m_w{};//�E�B���h�E�N���X�̐ݒ�


};