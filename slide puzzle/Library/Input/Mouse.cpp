#include"Mouse.h"
#include <cassert>



bool Mouse::Init()
{
	HRESULT result = S_FALSE;

	// DirectInputオブジェクトの生成	
	result = DirectInput8Create(Window::Get()->GetInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&m_dinput, nullptr);
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	// マウスデバイスの生成	
	result = m_dinput->CreateDevice(GUID_SysMouse, &m_devMouse, NULL);
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	// 入力データ形式のセット
	result = m_devMouse->SetDataFormat(&c_dfDIMouse2); // 標準形式
	if (FAILED(result)) {
		assert(0);
		return result;
	}

	// 排他制御レベルのセット
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
	result = m_devMouse->Acquire();	// マウス動作開始

	// 前回の入力を保存
	m_mouseStatePre = m_mouseState;

	// マウスの入力
	result = m_devMouse->GetDeviceState(sizeof(m_mouseState), &m_mouseState);
}

bool Mouse::PushMouseLeft()const
{
	// 0でなければ押している
	if (m_mouseState.rgbButtons[0]) {
		return true;
	}

	// 押していない
	return false;
}

bool  Mouse::PushMouseMiddle()const
{
	// 0でなければ押している
	if (m_mouseState.rgbButtons[2]) {
		return true;
	}

	// 押していない
	return false;
}

bool  Mouse::TriggerMouseLeft()const
{
	// 前回が0で、今回が0でなければトリガー
	if (!m_mouseStatePre.rgbButtons[0] && m_mouseState.rgbButtons[0]) {
		return true;
	}

	// トリガーでない
	return false;
}

bool  Mouse::TriggerMouseMiddle()const
{
	// 前回が0で、今回が0でなければトリガー
	if (!m_mouseStatePre.rgbButtons[2] && m_mouseState.rgbButtons[2]) {
		return true;
	}

	// トリガーでない
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
