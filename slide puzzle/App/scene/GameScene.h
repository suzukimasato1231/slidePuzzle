#pragma once
#include "DirectXBase.h"
#include"DebugText.h"
#include"ParticleManager.h"
#include<memory>
#include"BaseScene.h"
#include"Object.h"
#include<Audio.h>
#include"../slide puzzle/Player.h"
#include"../slide puzzle/plate.h"
#include"../slide puzzle/SceneChange.h"
extern const int window_width;
extern const int window_height;

/// <summary>
/// �Q�[���V�[��
/// </summary>
class GameScene:public BaseScene
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	GameScene();
	/// <summary>
	/// �f�X�R���X�g���N�^
	/// </summary>
	~GameScene();
	void Init()override;
	/// <summary>
	/// �X�V
	/// </summary>
	void Update()override;
	/// <summary>
	/// �`��
	/// </summary>
	void Draw()override;
	/// <summary>
	/// �e�`��
	/// </summary>
	void ShadowDraw()override;
	
	void Finalize()override;
private:
	std::unique_ptr<LightGroup>lightGroup;
	std::unique_ptr<Audio> audio;
private://��`
	// �V�[���`�F���W
	std::unique_ptr<SceneChange> sceneChange_;
	std::unique_ptr<Player>player;
	std::unique_ptr<Plate>plate;

	SpriteData back;
	int scene = 0;

	SpriteData gameoverGraph;
	SoundData selectSound;

};