#include"Controller.h"
static LPDIRECTINPUT8 g_InputInterface;							//!< DIRECTINPUT8�̃|�C���^
static LPDIRECTINPUTDEVICE8 g_GamePadDevice;					//!< DIRECTINPUTDEVICE8�̃|�C���^
static ButtonState g_ButtonStates[ButtonKind::ButtonKindMax];
static float leftAngle;
static float rightAngle;
// ���̓C���^�[�t�F�[�X�̍쐬
bool CreateInputInterface();

// �Q�[���p�b�h�f�o�C�X�̍쐬
bool CreateGamePadDevice();

// �Q�[���p�b�h�̍X�V
void UpdateGamePad();

//�Q�[���p�b�h�f�o�C�X�̍쐬-�f�o�C�X�񋓂̌��ʂ��󂯎��\����
struct DeviceEnumParameter
{
	LPDIRECTINPUTDEVICE8* GamePadDevice;
	int FindCount;
};

BOOL SetUpCooperativeLevel(LPDIRECTINPUTDEVICE8 device)
{
	return true;
}

BOOL StartGamePadControl()
{
	// �f�o�C�X����������ĂȂ�
	if (g_GamePadDevice == nullptr)
	{
		return false;
	}

	// ����J�n
	if (FAILED(g_GamePadDevice->Acquire()))
	{
		return false;
	}

	DIDEVCAPS cap;
	g_GamePadDevice->GetCapabilities(&cap);
	// �|�[�����O����
	if (cap.dwFlags & DIDC_POLLEDDATAFORMAT)
	{
		DWORD error = GetLastError();

		if (FAILED(g_GamePadDevice->Poll()))
		{
			return false;
		}
	}

	return true;
}

BOOL SetUpGamePadProperty(LPDIRECTINPUTDEVICE8 device)
{
	// �����[�h���Βl���[�h�Ƃ��Đݒ�
	DIPROPDWORD diprop;
	ZeroMemory(&diprop, sizeof(diprop));
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.diph.dwObj = 0;
	diprop.dwData = DIPROPAXISMODE_ABS;
	if (FAILED(device->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
	{
		return false;
	}

	// X���̒l�͈̔͐ݒ�
	DIPROPRANGE diprg;
	ZeroMemory(&diprg, sizeof(diprg));
	diprg.diph.dwSize = sizeof(diprg);
	diprg.diph.dwHeaderSize = sizeof(diprg.diph);
	diprg.diph.dwHow = DIPH_BYOFFSET;
	diprg.diph.dwObj = DIJOFS_X;
	diprg.lMin = -1000;
	diprg.lMax = 1000;
	if (FAILED(device->SetProperty(DIPROP_RANGE, &diprg.diph)))
	{
		return false;
	}

	// Y���̒l�͈̔͐ݒ�
	diprg.diph.dwObj = DIJOFS_Y;
	if (FAILED(device->SetProperty(DIPROP_RANGE, &diprg.diph)))
	{
		return false;
	}
	// Z���̒l�͈̔͐ݒ�
	diprg.diph.dwObj = DIJOFS_Z;
	if (FAILED(device->SetProperty(DIPROP_RANGE, &diprg.diph)))
	{
		return false;
	}

	//�E�X�e�B�b�N
	diprg.diph.dwObj = DIJOFS_RX;
	if (FAILED(device->SetProperty(DIPROP_RANGE, &diprg.diph)))
	{
		return false;
	}

	// Y���̒l�͈̔͐ݒ�
	diprg.diph.dwObj = DIJOFS_RY;
	if (FAILED(device->SetProperty(DIPROP_RANGE, &diprg.diph)))
	{
		return false;
	}

	diprg.diph.dwObj = DIJOFS_RZ;
	if (FAILED(device->SetProperty(DIPROP_RANGE, &diprg.diph)))
	{
		return false;
	}

	return true;
}

BOOL CALLBACK DeviceFindCallBack(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef)
{
	DeviceEnumParameter* parameter = (DeviceEnumParameter*)pvRef;
	LPDIRECTINPUTDEVICE8 device = nullptr;

	// ���ɔ������Ă���Ȃ�I��
	if (parameter->FindCount >= 1)
	{
		return DIENUM_STOP;
	}

	// �f�o�C�X����
	HRESULT hr = g_InputInterface->CreateDevice(
		lpddi->guidInstance,
		parameter->GamePadDevice,
		NULL);

	if (FAILED(hr))
	{
		return DIENUM_STOP;
	}

	// ���̓t�H�[�}�b�g�̎w��
	device = *parameter->GamePadDevice;
	hr = device->SetDataFormat(&c_dfDIJoystick);

	if (FAILED(hr))
	{
		return DIENUM_STOP;
	}

	// �v���p�e�B�̐ݒ�
	if (SetUpGamePadProperty(device) == false)
	{
		return DIENUM_STOP;
	}

	// �������x���̐ݒ�
	if (SetUpCooperativeLevel(device) == false)
	{
		return DIENUM_STOP;
	}

	// ���������J�E���g
	parameter->FindCount++;

	return DIENUM_CONTINUE;
}

bool Controller::InitController()
{
	// �C���^�[�t�F�[�X�쐬
	if (CreateInputInterface() == false)
	{
		return false;
	}

	// �f�o�C�X�쐬
	if (CreateGamePadDevice() == false)
	{
		ReleaseInput();
		return false;
	}

	// ���͏��̏�����
	for (int i = 0; i < ButtonKind::ButtonKindMax; i++)
	{
		g_ButtonStates[i] = ButtonState::ButtonStateNone;
	}

	return true;
}

void Controller::ReleaseInput()
{
	// �f�o�C�X�̉��
	if (g_GamePadDevice != nullptr)
	{
		// ������~
		g_GamePadDevice->Unacquire();
		g_GamePadDevice->Release();
		g_GamePadDevice = nullptr;
	}

	// �C���^�[�t�F�[�X�̉��
	if (g_InputInterface != nullptr)
	{
		g_InputInterface->Release();
		g_InputInterface = nullptr;
	}
}

void Controller::UpdateInput()
{
	UpdateGamePad();
}

bool Controller::IsButtonPush(ButtonKind button)const
{
	if (g_ButtonStates[button] == ButtonState::ButtonStatePush)
	{
		return true;
	}

	return false;
}

bool Controller::IsButtonUp(ButtonKind button)const
{
	if (g_ButtonStates[button] == ButtonState::ButtonStateUp)
	{
		return true;
	}

	return false;
}

bool Controller::IsButtonDown(ButtonKind button)const
{
	if (g_ButtonStates[button] == ButtonState::ButtonStateDown)
	{
		return true;
	}

	return false;
}

float Controller::GetLeftAngle()const
{
	return leftAngle;
}

float Controller::GetRightAngle()const
{
	return rightAngle;
}

bool CreateInputInterface()
{
	// �C���^�[�t�F�[�X�쐬
	HRESULT ret = DirectInput8Create(
		GetModuleHandle(nullptr),	// �C���X�^���X�n���h��
		DIRECTINPUT_VERSION,		// DirectInput�̃o�[�W����
		IID_IDirectInput8,			// �g�p����@�\
		(void**)&g_InputInterface,	// �쐬���ꂽ�C���^�[�t�F�[�X����p
		NULL						// NULL�Œ�
	);

	if (FAILED(ret))
	{
		return false;
	}

	return true;
}

bool CreateGamePadDevice()
{
	DeviceEnumParameter parameter;

	parameter.FindCount = 0;
	parameter.GamePadDevice = &g_GamePadDevice;

	// GAMEPAD�𒲂ׂ�
	g_InputInterface->EnumDevices(
		DI8DEVTYPE_GAMEPAD,			// ��������f�o�C�X�̎��
		DeviceFindCallBack,			// �������Ɏ��s����֐�
		&parameter,					// �֐��ɓn���l
		DIEDFL_ATTACHEDONLY			// �������@
	);

	// JOYSTICK�𒲂ׂ�
	g_InputInterface->EnumDevices(
		DI8DEVTYPE_JOYSTICK,
		DeviceFindCallBack,
		&parameter,
		DIEDFL_ATTACHEDONLY
	);

	// �ǂ���������邱�Ƃ��o���Ȃ������玸�s
	if (parameter.FindCount == 0)
	{
		return false;
	}

	int count = 0;
	// ����J�n
	while (StartGamePadControl() == false)
	{
		Sleep(100);
		count++;
		if (count >= 5)
		{
			break;
		}
	}

	return true;
}

void UpdateGamePad()
{
	if (g_GamePadDevice == nullptr)
	{
		return;
	}

	DIJOYSTATE pad_data={};

	// �f�o�C�X�擾
	HRESULT hr = g_GamePadDevice->GetDeviceState(sizeof(DIJOYSTATE), &pad_data);
	if (FAILED(hr))
	{
		// �ēx����J�n
		if (FAILED(g_GamePadDevice->Acquire()))
		{
			for (int i = 0; i < ButtonKind::ButtonKindMax; i++)
			{
				g_ButtonStates[i] = ButtonState::ButtonStateNone;
			}
			g_GamePadDevice->Poll();
		}
		return;
	}

	bool is_push[ButtonKind::ButtonKindMax] = {};
	// �X�e�B�b�N����
	int unresponsive_range = 300;
	//���X�e�B�b�N
	if (pad_data.lX < -unresponsive_range)
	{
		is_push[ButtonKind::LButtonLeft] = true;
	}
	else if (pad_data.lX > unresponsive_range)
	{
		is_push[ButtonKind::LButtonRight] = true;
	}

	if (pad_data.lY < -unresponsive_range)
	{
		is_push[ButtonKind::LButtonUp] = true;
	}
	else if (pad_data.lY > unresponsive_range)
	{
		is_push[ButtonKind::LButtonDown] = true;
	}
	//�E�X�e�B�b�N
	if (pad_data.lRx < -unresponsive_range)
	{
		is_push[ButtonKind::RButtonLeft] = true;
	}
	else if (pad_data.lRx > unresponsive_range)
	{
		is_push[ButtonKind::RButtonRight] = true;
	}
	//�E�X�e�B�b�N
	if (pad_data.lRx < -unresponsive_range)
	{
		is_push[ButtonKind::RButtonLeft] = true;
	}
	else if (pad_data.lRx > unresponsive_range)
	{
		is_push[ButtonKind::RButtonRight] = true;
	}

	if (pad_data.lRy < -unresponsive_range)
	{
		is_push[ButtonKind::RButtonUp] = true;
	}
	else if (pad_data.lRy > unresponsive_range)
	{
		is_push[ButtonKind::RButtonDown] = true;
	}
	//T
	if (pad_data.lZ > unresponsive_range)
	{
		is_push[ButtonKind::ButtonLT] = true;
	}
	else if (pad_data.lZ < -unresponsive_range)
	{
		is_push[ButtonKind::ButtonRT] = true;
	}
	unresponsive_range = 150;
	if (pad_data.lX < -unresponsive_range)
	{
		is_push[ButtonKind::LButtonLeftS] = true;
	}
	else if (pad_data.lX > unresponsive_range)
	{
		is_push[ButtonKind::LButtonRightS] = true;
	}

	if (pad_data.lY < -unresponsive_range)
	{
		is_push[ButtonKind::LButtonUpS] = true;
	}
	else if (pad_data.lY > unresponsive_range)
	{
		is_push[ButtonKind::LButtonDownS] = true;
	}

	//�p�x���擾
#define PI 3.141592653589793
	leftAngle = static_cast<float>(atan2(0 - pad_data.lX, 0 - pad_data.lY));
	rightAngle = static_cast<float>(atan2(0 - pad_data.lRx, 0 - pad_data.lRy));

	// �\���L�[����
	if (pad_data.rgdwPOV[0] != 0xFFFFFFFF)
	{
#define PI 3.141592653589793
		float rad = static_cast<float>(((pad_data.rgdwPOV[0] / 100.0f) * 180.0 / PI));
		float x = sinf(rad);
		float y = cosf(rad);

		if (x < -0.01f)
		{
			is_push[ButtonKind::LButtonLeft] = true;
		}
		else if (x > 0.01f)
		{
			is_push[ButtonKind::LButtonRight] = true;
		}

		if (y > 0.01f)
		{
			is_push[ButtonKind::LButtonUp] = true;
		}
		else if (y < -0.01f)
		{
			is_push[ButtonKind::LButtonDown] = true;
		}
	}

	// �{�^������
	for (int i = 0; i < 32; i++)
	{
		if (!(pad_data.rgbButtons[i] & 0x80))
		{
			continue;
		}

		switch (i)
		{
		case 0:
			is_push[ButtonKind::ButtonA] = true;
			break;
		case 1:
			is_push[ButtonKind::ButtonB] = true;
			break;
		case 2:
			is_push[ButtonKind::ButtonX] = true;
			break;
		case 3:
			is_push[ButtonKind::ButtonY] = true;
			break;
		case 4:
			is_push[ButtonKind::ButtonLB] = true;
			break;
		case 5:
			is_push[ButtonKind::ButtonRB] = true;
			break;
		case 7:
			is_push[ButtonKind::ButtonMenu] = true;
			break;
		}
	}

	// ���͏�񂩂�{�^���̏�Ԃ��X�V����
	for (int i = 0; i < ButtonKind::ButtonKindMax; i++)
	{
		if (is_push[i] == true)
		{
			if (g_ButtonStates[i] == ButtonState::ButtonStateNone)
			{
				g_ButtonStates[i] = ButtonState::ButtonStateDown;
			}
			else
			{
				g_ButtonStates[i] = ButtonState::ButtonStatePush;
			}
		}
		else
		{
			if (g_ButtonStates[i] == ButtonState::ButtonStatePush)
			{
				g_ButtonStates[i] = ButtonState::ButtonStateUp;
			}
			else
			{
				g_ButtonStates[i] = ButtonState::ButtonStateNone;
			}
		}
	}
}