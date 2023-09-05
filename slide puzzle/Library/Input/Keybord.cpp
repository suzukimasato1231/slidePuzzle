#include"Keybord.h"
void Keybord::Initialize()
{
	HRESULT result;
	//キーボード入力
	result = DirectInput8Create(
		Window::Get()->GetInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_dinput, nullptr);

	//キーボードデバイスの生成
	result = m_dinput->CreateDevice(GUID_SysKeyboard, &m_devkeyboard, NULL);
	//入力データ形式のセット
	result = m_devkeyboard->SetDataFormat(&c_dfDIKeyboard);//標準形式
	//排他制御レベルのセット
	result = m_devkeyboard->SetCooperativeLevel(
		Window::Get()->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
}

void Keybord::Update()
{
	HRESULT result;
	// DirectX毎フレーム処理　ここから
	for (int i = 0; i < 256; i++)
	{
		m_oldkey[i] = m_key[i];
	}
	//キーボード情報の取得開始
	result = m_devkeyboard->Acquire();
	result = m_devkeyboard->GetDeviceState(sizeof(m_key), m_key);
}

bool Keybord::PushKey(BYTE keyNumber)const
{
	// 0でなければ押している
	if (m_key[keyNumber]) {
		return true;
	}
	// 押していない
	return false;
}

bool Keybord::TriggerKey(BYTE keyNumber)const
{
	// 前回が0で、今回が0でなければトリガー
	if (!m_oldkey[keyNumber] && m_key[keyNumber]) {
		return true;
	}
	// トリガーでない
	return false;
}

