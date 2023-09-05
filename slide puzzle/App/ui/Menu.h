#include <Sprite.h>
#include<Audio.h>
/// <summary>
/// ���j���[�N���X
/// </summary>
class Menu
{
private:
	//���j���[
	enum MenuType
	{
		MENURETRY,		//���g���C
		MENUVOLUM,		//�{�����[��
		MENUSTAGESELECT,//�X�e�[�W�Z���N�g
		MENUMAX,		//�ő�
	};
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Menu();
	/// <summary>
	/// �f�X�R���X�g���N�^
	/// </summary>
	~Menu();
	/// <summary>
	/// ������
	/// </summary>
	void Init();
	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="menuFlag">���j���[�t���O</param>
	/// <param name="changeScene">�`�F���W�V�[���t���O</param>
	/// <param name="sceneNum">�`�F���W���鐔��</param>
	void Update(bool& menuFlag, bool& changeScene, int& sceneNum);
	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="gameoverFlag">�Q�[���I�[�o�[�t���O</param>
	void Draw(const bool gameoverFlag);
	/// <summary>
	/// ���Z�b�g
	/// </summary>
	void Reset();
private:
	/// <summary>
	/// ���j���[���J������
	/// </summary>
	void MenuOpen(bool& menuFlag);
	/// <summary>
	/// ���j���[�̌��菈��
	/// </summary>
	void MenuDecision(bool& menuFlag, int& sceneNum, bool& changeScene);
	/// <summary>
	/// ���̐ݒ�
	/// </summary>
	void AudioOpition();
	/// <summary>
	/// ���j���[���o��Ƃ��̃C�[�W���O����
	/// </summary>
	void MenuEasing(const bool menuFlag);
	/// <summary>
	/// ���j���[�̕`��
	/// </summary>
	void DrawMenuDraw(const bool gameoverFlag);
	/// <summary>
	/// ���ʒ��ߕ`��
	/// </summary>
	void DrawAudioOption();
private:
	//���j���[�֘A
	SpriteData m_menuButton;
	SpriteData m_menuFream;
	SpriteData m_menuFreamFish;
	SpriteData m_menuRetry;
	SpriteData m_menuVolum;
	SpriteData m_menuStageSelect;
	SpriteData m_menuFish;
	int m_menuNum = 0;
	//���j���[�̋���
	bool m_menuTimeFlag = false;
	float m_menuScale = 1.0f;
	//�C�[�W���O�i���j���[�j
	const Vec3 c_start = { 1500.0f,300.0f,0.0f };
	const Vec3 c_end = { 950.0f,300.0f,0.0f };
	Vec3 m_menuPos = { 0.0f,0.0f ,0.0f };
	bool m_menuEasingFlag = false;
	//�o�Ă���Ƃ�
	const float c_startTimeMax = 20.0f;
	float m_startTime = 0.0f;

	//���ݒ�
	SpriteData m_menuAudioFream;
	SpriteData m_menuAudioVolum;
	SpriteData m_menuAudioLandmark;
	//���ʒ����t���O
	bool m_audioFlag = false;

	const float c_volumMax = 0.10f;
	const float c_volumMin = 0.0f;
	float m_volum = 0.02f;

	//���f�[�^
	SoundData m_decisionSE = {};
	SoundData m_selectSE = {};

};