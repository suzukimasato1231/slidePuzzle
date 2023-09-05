#include "Window.h"

//�E�B���h�E�v���V�[�W��
LRESULT WindowProce(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	//�E�B���h�E���j�����ꂽ��Ă΂��
	if (msg == WM_DESTROY)
	{
		PostQuitMessage(0);//OS�ɑ΂��āu�������̃A�v���͏I���v�Ɠ`����
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

void Window::CreateGameWindow()
{
	// �E�B���h�E�T�C�Y{ X���W Y���W ���� �c�� }
	RECT wrc = { 0, 0, window_width, window_height };
	//�E�B���h�E�N���X�̐ݒ�
	m_w.cbSize = sizeof(WNDCLASSEX);
	m_w.lpfnWndProc = (WNDPROC)WindowProce; // �E�B���h�E�v���V�[�W����ݒ�
	m_w.lpszClassName = L"�y���M���u���[�N"; // �E�B���h�E�N���X��
	m_w.hInstance = GetModuleHandle(nullptr); // �E�B���h�E�n���h��
	m_w.hCursor = LoadCursor(NULL, IDC_ARROW); // �J�[�\���w��

	// �E�B���h�E�N���X��OS�ɓo�^
	RegisterClassEx(&m_w);

	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false); // �����ŃT�C�Y�␳

	// �E�B���h�E�I�u�W�F�N�g�̐���
	m_hwnd = CreateWindow(m_w.lpszClassName, // �N���X��
		L"�y���M���u���[�N",         // �^�C�g���o�[�̕���
		WS_OVERLAPPEDWINDOW,        // �W���I�ȃE�B���h�E�X�^�C��
		CW_USEDEFAULT,              // �\��X���W�iOS�ɔC����j
		CW_USEDEFAULT,              // �\��Y���W�iOS�ɔC����j
		wrc.right - wrc.left,       // �E�B���h�E����
		wrc.bottom - wrc.top,   // �E�B���h�E�c��
		nullptr,                // �e�E�B���h�E�n���h��
		nullptr,                // ���j���[�n���h��
		m_w.hInstance,            // �Ăяo���A�v���P�[�V�����n���h��
		nullptr);               // �I�v�V����

	// �E�B���h�E�\��
	ShowWindow(m_hwnd, SW_SHOW);
}


bool Window::ProcessMessage()
{
	MSG msg{};	// ���b�Z�[�W

	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) // ���b�Z�[�W������H
	{
		TranslateMessage(&msg); // �L�[���̓��b�Z�[�W�̏���
		DispatchMessage(&msg); // �E�B���h�E�v���V�[�W���Ƀ��b�Z�[�W�𑗂�
	}

	if (msg.message == WM_QUIT) // �I�����b�Z�[�W�������烋�[�v�𔲂���
	{
		return true;
	}
	return false;
}

void Window::TerminateGameWindow()
{
	// �E�B���h�E�N���X��o�^����
	UnregisterClass(m_w.lpszClassName, m_w.hInstance);

}

