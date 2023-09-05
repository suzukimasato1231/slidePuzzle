#include"Keybord.h"
void Keybord::Initialize()
{
	HRESULT result;
	//�L�[�{�[�h����
	result = DirectInput8Create(
		Window::Get()->GetInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_dinput, nullptr);

	//�L�[�{�[�h�f�o�C�X�̐���
	result = m_dinput->CreateDevice(GUID_SysKeyboard, &m_devkeyboard, NULL);
	//���̓f�[�^�`���̃Z�b�g
	result = m_devkeyboard->SetDataFormat(&c_dfDIKeyboard);//�W���`��
	//�r�����䃌�x���̃Z�b�g
	result = m_devkeyboard->SetCooperativeLevel(
		Window::Get()->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
}

void Keybord::Update()
{
	HRESULT result;
	// DirectX���t���[�������@��������
	for (int i = 0; i < 256; i++)
	{
		m_oldkey[i] = m_key[i];
	}
	//�L�[�{�[�h���̎擾�J�n
	result = m_devkeyboard->Acquire();
	result = m_devkeyboard->GetDeviceState(sizeof(m_key), m_key);
}

bool Keybord::PushKey(BYTE keyNumber)const
{
	// 0�łȂ���Ή����Ă���
	if (m_key[keyNumber]) {
		return true;
	}
	// �����Ă��Ȃ�
	return false;
}

bool Keybord::TriggerKey(BYTE keyNumber)const
{
	// �O��0�ŁA����0�łȂ���΃g���K�[
	if (!m_oldkey[keyNumber] && m_key[keyNumber]) {
		return true;
	}
	// �g���K�[�łȂ�
	return false;
}

