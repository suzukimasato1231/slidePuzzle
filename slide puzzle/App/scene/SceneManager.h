#pragma once
#include "ShadowMap.h"
#include"BaseScene.h"
#include"Audio.h"
#include<Sprite.h>
extern const int window_width;
extern const int window_height;

class SceneManager :public Singleton<SceneManager>
{
private:
	friend Singleton<SceneManager>;
	/// <summary>
	/// �V�[���^�C�v
	/// </summary>
	enum Scene
	{
		Title,
		SelectScene,
		Game,
		Result,
	};
	/// <summary>
	/// �V�[���`�F���W��
	/// </summary>
	enum ChangeNumber
	{
		ChangeStand,
		ChangeFirst,
		ChangeEnd,
		ChangeEmpty,
	};
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	SceneManager();
	/// <summary>
	/// �f�X�R���X�g���N�^
	/// </summary>
	~SceneManager();
	/// <summary>
	/// ������
	/// </summary>
	void Initialize();
	/// <summary>
	/// �X�V
	/// </summary>
	void Update();
	/// <summary>
	/// �`��
	/// </summary>
	void Draw();
	/// <summary>
	/// �폜
	/// </summary>
	void Delete();

	//���̃V�[���\��
	void SetNextScene(BaseScene* nextScene) { nextScene_ = nextScene; }
private:
	/// <summary>
	/// �V�[���`�F���W�̏���
	/// </summary>
	void SceneChange();
private://��`
	//���̃V�[��
	BaseScene* scene_ = nullptr;
	//���̃V�[��
	BaseScene* nextScene_ = nullptr;

	ShadowMap  m_shadowMapFar;

	SoundData sound;
	SoundData moveSound;
};