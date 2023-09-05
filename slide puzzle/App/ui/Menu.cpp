#include"Menu.h"
#include<Input.h>
#include <Easing.h>
#include<Audio.h>
Menu::Menu()
{}

Menu::~Menu()
{}

void Menu::Init()
{
	m_menuButton = Sprite::Get()->SpriteCreate(L"Resources/UI/menuButton.png");
	m_menuFream = Sprite::Get()->SpriteCreate(L"Resources/UI/menuFrame.png");
	m_menuFreamFish = Sprite::Get()->SpriteCreate(L"Resources/UI/menuFrameFish.png");
	m_menuRetry = Sprite::Get()->SpriteCreate(L"Resources/UI/menuRetry.png");
	m_menuVolum = Sprite::Get()->SpriteCreate(L"Resources/UI/menuVolume.png");
	m_menuStageSelect = Sprite::Get()->SpriteCreate(L"Resources/UI/menuStageSelect.png");
	m_menuFish = Sprite::Get()->SpriteCreate(L"Resources/UI/menuFish.png");

	m_menuAudioFream = Sprite::Get()->SpriteCreate(L"Resources/UI/menuAudioFrame.png");
	m_menuAudioVolum = Sprite::Get()->SpriteCreate(L"Resources/UI/menuAudioVolum.png");
	m_menuAudioLandmark = Sprite::Get()->SpriteCreate(L"Resources/UI/menuAudioLandmark.png");

	m_decisionSE = Audio::SoundLoadWave("Resources/sound/SE/menu.wav");
	m_selectSE = Audio::SoundLoadWave("Resources/sound/SE/menuSelect.wav");
}

void Menu::Update(bool& menuFlag, bool& changeScene, int& sceneNum)
{
	//���j���[���J������
	MenuOpen(menuFlag);
	//���j���[���菈��
	MenuDecision(menuFlag, sceneNum, changeScene);
	//���̐ݒ�
	AudioOpition();
	//�C�[�W���O����
	MenuEasing(menuFlag);
}

void Menu::Draw(const bool gameoverFlag)
{
	//���j���[
	Sprite::Get()->Draw(m_menuButton, Vec2(1230.5f, 110.0f), 96.0f, 32.0f, Vec2(0.5f, 0.5f));

	DrawMenuDraw(gameoverFlag);
	//���ʒ��߂̕`��
	DrawAudioOption();
}

void Menu::Reset()
{
	m_menuNum = {};
	m_startTime = {};
	m_audioFlag = false;
	m_menuEasingFlag = false;
}

void Menu::MenuOpen(bool& menuFlag)
{
	//���j���[���
	if (Input::Get()->ControllerDown(ButtonMenu) == false || menuFlag == true) { return; }
	menuFlag = true;
	m_menuNum = {};
	m_startTime = {};
	m_menuEasingFlag = true;
	Audio::Get()->SoundSEPlayWave(m_decisionSE);
}

void Menu::MenuDecision(bool& menuFlag, int& sceneNum, bool& changeScene)
{
	if (menuFlag == false || m_audioFlag == true) { return; }
	if (Input::Get()->ControllerDown(ButtonB) == true)
	{
		menuFlag = false;
		m_startTime = {};
		Audio::Get()->SoundSEPlayWave(m_decisionSE);
	}
	if (Input::Get()->ControllerDown(ButtonA) == true)
	{
		switch (m_menuNum)
		{
		case MENURETRY://���g���C
			sceneNum = 1;
			changeScene = true;
			break;
		case MENUVOLUM://���ʒ���
			m_audioFlag = true;
			break;
		case MENUSTAGESELECT://�X�e�[�W�Z���N�g
			sceneNum = 2;
			changeScene = true;
			break;
		default:
			assert(0);
			break;
		}
		Audio::Get()->SoundSEPlayWave(m_decisionSE);
	}
	if (Input::Get()->ControllerDown(LButtonDown) == true)
	{
		m_menuNum++;
		if (m_menuNum >= MENUMAX)
		{
			m_menuNum = MENURETRY;
		}
		Audio::Get()->SoundSEPlayWave(m_selectSE);
	}
	if (Input::Get()->ControllerDown(LButtonUp) == true)
	{
		m_menuNum--;
		if (m_menuNum < MENURETRY)
		{
			m_menuNum = MENUSTAGESELECT;
		}
		Audio::Get()->SoundSEPlayWave(m_selectSE);
	}
	//���j���[�I�����Ă�̂�ڗ������鏈��
	const float menuScaleSpeed = 0.01f;	//���j���[�X�P�[���X�s�[�h
	const float menuScaleMax = 1.1f;	//�X�P�[���ő�
	const float menuScaleMin = 0.9f;	//�X�P�[���ŏ�
	if (m_menuTimeFlag == false)
	{
		m_menuScale += menuScaleSpeed;
		if (m_menuScale >= menuScaleMax)
		{
			m_menuTimeFlag = true;
		}
	}
	else
	{
		m_menuScale -= menuScaleSpeed;
		if (m_menuScale <= menuScaleMin)
		{
			m_menuTimeFlag = false;
		}
	}
}

void Menu::AudioOpition()
{
	if (m_audioFlag == false) { return; }
	//���ʂ��ς��X�s�[�h
	const float volumSpeed = 0.001f;
	if (Input::Get()->ControllerPush(LButtonLeft) == true)
	{
		m_volum -= volumSpeed;
		if (m_volum <= c_volumMin)
		{
			m_volum = c_volumMin;
		}
		Audio::Get()->SoundSEPlayWave(m_selectSE);
	}
	if (Input::Get()->ControllerPush(LButtonRight) == true)
	{
		m_volum += volumSpeed;
		if (m_volum >= c_volumMax)
		{
			m_volum = c_volumMax;
		}
		Audio::Get()->SoundSEPlayWave(m_selectSE);
	}
	Audio::Get()->SetVolume(m_volum);
	if (Input::Get()->ControllerDown(ButtonB) == true)
	{
		m_audioFlag = false;
		Audio::Get()->SoundSEPlayWave(m_decisionSE);
	}
}

void Menu::MenuEasing(const bool menuFlag)
{
	//�C�[�W���O����
	if (m_menuEasingFlag == false) { return; }
	if (menuFlag == true)
	{//���j���[���o�Ă���Ƃ��̏���
		float t = min(m_startTime / c_startTimeMax, 1.0f);
		if (t <= 1.0f)
		{
			m_startTime += 1.0f;
		}
		m_menuPos = Easing::easeOut(c_start, c_end, t);
	}
	else
	{//���j���[�����܂��Ƃ��̏���
		float t = min(m_startTime / c_startTimeMax, 1.0f);
		m_startTime += 1.0f;
		if (t >= 1.0f)
		{
			m_menuEasingFlag = false;
		}
		m_menuPos = Easing::easeIn(c_end, c_start, t);
	}
}

void Menu::DrawMenuDraw(const bool gameoverFlag)
{
	if (m_menuEasingFlag == false || gameoverFlag == true || m_audioFlag == true) { return; }

	Sprite::Get()->Draw(m_menuFream, Vec2(m_menuPos.x + 26.5f, 300.5), 459.0, 547.0f, Vec2(0.5f, 0.5f));
	Sprite::Get()->Draw(m_menuFreamFish, Vec2(m_menuPos.x, 300.5), 512.0f, 549.0f, Vec2(0.5f, 0.5f));
	const float UIPosX = 30.0f + m_menuPos.x;			//��{�ʒuX���W
	const Vec2 menuAnchor = { 0.5f,0.5f };				//�A���J�[�|�C���g
	const Vec2 menuSize = { 359.0f,71.0f };				//���j���[�̑傫��
	const float menuPosY[3] = { 150.5f,280.5f,400.5f };	//���j���[�̈ʒuY
	if (m_menuNum == MENURETRY)
	{
		Sprite::Get()->Draw(m_menuRetry, Vec2(UIPosX, menuPosY[0]), menuSize.x * m_menuScale, menuSize.y * m_menuScale, menuAnchor);
	}
	else
	{
		Sprite::Get()->Draw(m_menuRetry, Vec2(UIPosX, menuPosY[0]), menuSize.x, menuSize.y, menuAnchor);
	}
	if (m_menuNum == MENUVOLUM)
	{
		Sprite::Get()->Draw(m_menuVolum, Vec2(UIPosX, menuPosY[1]), menuSize.x * m_menuScale, menuSize.y * m_menuScale, menuAnchor);
	}
	else
	{
		Sprite::Get()->Draw(m_menuVolum, Vec2(UIPosX, menuPosY[1]), menuSize.x, menuSize.y, menuAnchor);
	}
	if (m_menuNum == MENUSTAGESELECT)
	{
		Sprite::Get()->Draw(m_menuStageSelect, Vec2(UIPosX, menuPosY[2]), menuSize.x * m_menuScale, menuSize.y * m_menuScale, menuAnchor);
	}
	else
	{
		Sprite::Get()->Draw(m_menuStageSelect, Vec2(UIPosX, menuPosY[2]), menuSize.x, menuSize.y, menuAnchor);
	}
}

void Menu::DrawAudioOption()
{
	if (m_audioFlag == false) { return; }
	const Vec2 audioAnchor = { 0.5f,0.5f };		//�A���J�[�|�C���g
	

	Sprite::Get()->Draw(m_menuFream, Vec2(m_menuPos.x + 26.5f, 300.5f), 459.0f, 549.0f, audioAnchor);
	Sprite::Get()->Draw(m_menuFreamFish, Vec2(m_menuPos.x, 300.5f), 512.0f, 549.0f, audioAnchor);
	Sprite::Get()->Draw(m_menuAudioFream, Vec2(m_menuPos.x, 300.5f), 512.0, 547.0f, audioAnchor);
	Sprite::Get()->Draw(m_menuAudioVolum, Vec2(m_menuPos.x - 133.0f, 285.0f), 372.0f * m_volum * 10, 28.0f);
	Sprite::Get()->Draw(m_menuAudioLandmark, Vec2(m_menuPos.x - 133.0f + 372.0f * m_volum * 10, 295.0f), 12.0f, 68.0f, audioAnchor);

}
