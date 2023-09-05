#include"Mouse.h"
#include <cassert>



bool Mouse::Init()
{
	HRESULT result = S_FALSE;

	// DirectInput�I�u�W�F�N�g�̐���	
	result = DirectInput8Create(Window::Get()->GetInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&m_dinput, nullptr);
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	// �}�E�X�f�o�C�X�̐���	
	result = m_dinput->CreateDevice(GUID_SysMouse, &m_devMouse, NULL);
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	// ���̓f�[�^�`���̃Z�b�g
	result = m_devMouse->SetDataFormat(&c_dfDIMouse2); // �W���`��
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	// �r�����䃌�x���̃Z�b�g
	result = m_devMouse->SetCooperativeLevel(Window::Get()->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	if (FAILED(result)) {
		assert(0);
		return result;
	}
	return true;
}

void Mouse::Update()
{
	HRESULT result;
	result = m_devMouse->Acquire();	// �}�E�X����J�n

	// �O��̓��͂�ۑ�
	m_mouseStatePre = m_mouseState;

	// �}�E�X�̓���
	result = m_devMouse->GetDeviceState(sizeof(m_mouseState), &m_mouseState);
}

bool Mouse::PushMouseLeft()const
{
	// 0�łȂ���Ή����Ă���
	if (m_mouseState.rgbButtons[0]) {
		return true;
	}

	// �����Ă��Ȃ�
	return false;
}

bool  Mouse::PushMouseMiddle()const
{
	// 0�łȂ���Ή����Ă���
	if (m_mouseState.rgbButtons[2]) {
		return true;
	}

	// �����Ă��Ȃ�
	return false;
}

bool  Mouse::TriggerMouseLeft()const
{
	// �O��0�ŁA����0�łȂ���΃g���K�[
	if (!m_mouseStatePre.rgbButtons[0] && m_mouseState.rgbButtons[0]) {
		return true;
	}

	// �g���K�[�łȂ�
	return false;
}

bool  Mouse::TriggerMouseMiddle()const
{
	// �O��0�ŁA����0�łȂ���΃g���K�[
	if (!m_mouseStatePre.rgbButtons[2] && m_mouseState.rgbButtons[2]) {
		return true;
	}

	// �g���K�[�łȂ�
	return false;
}

Mouse::MouseMove  Mouse::GetMouseMove()
{
	MouseMove tmp;
	tmp.lX = m_mouseState.lX;
	tmp.lY = m_mouseState.lY;
	tmp.lZ = m_mouseState.lZ;
	return tmp;
}
