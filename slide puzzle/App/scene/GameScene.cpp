#include "GameScene.h"
#include<sstream>
#include<iomanip>
#include "Input.h"
#include"FbxLoader.h"
#include"Shape.h"
#include"StageSelect.h"
#include"SceneManager.h"
#include"TitleScene.h"
GameScene::GameScene()
{}
GameScene::~GameScene()
{
}

void GameScene::Init()
{
	//Audio�N���X�쐬
	audio = std::make_unique<Audio>();
	//���C�g�O���[�v�N���X�쐬
	lightGroup = std::make_unique<LightGroup>();
	lightGroup->Initialize();
	// 3D�I�u�G�N�g�Ƀ��C�g���Z�b�g
	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightDir(0, XMVECTOR{ 0,-1,0,0 });
	lightGroup->SetShadowDir(Vec3(0, 1, 0));
	//���f�[�^�ǂݍ���
	//�J�����ʒu���Z�b�g
	Camera::Get()->SetCamera(Vec3{ 0,20,-1 }, Vec3{ 0, -3, 0 }, Vec3{ 0, 1, 0 });
	//�X�v���C�g�摜�ǂݍ���
	FBXObject3d::SetLight(lightGroup.get());
	Object::SetLight(lightGroup.get());

	player = std::make_unique<Player>();
	player->Init();
	plate = std::make_unique<Plate>();
	plate->Init();

	back = Sprite::Get()->SpriteCreate(L"Resources/back.png");

	//�I�u�W�F�N�g����
	gameoverGraph = Sprite::Get()->SpriteCreate(L"Resources/gameover.png");

	selectSound = Audio::Get()->SoundLoadWave("Resources/Sound/select.wav");

	// �V�[���J�ڂ̉��o�̏�����
	sceneChange_ = std::make_unique<SceneChange>();
}

void GameScene::Update()
{
	//���C�g�X�V
	lightGroup->Update();
	if (sceneChange_->GetinEndFlag())
	{
		if (Input::Get()->KeybordTrigger(DIK_SPACE) && player->GetDeadFlag() == true)
		{
			sceneChange_->SceneChangeStart("");
			Audio::Get()->SoundSEPlayWave(selectSound);
		}
		else
		{
			if (Input::Get()->KeybordTrigger(DIK_SPACE)) { plate->SetKeyFlag(true); }
			else if (Input::Get()->KeybordTrigger(DIK_UP)) { plate->AddSetSelectBlockNumber(-4);
			Audio::Get()->SoundSEPlayWave(selectSound);}
			else if (Input::Get()->KeybordTrigger(DIK_DOWN)) { plate->AddSetSelectBlockNumber(4); 
			Audio::Get()->SoundSEPlayWave(selectSound);}
			else if (Input::Get()->KeybordTrigger(DIK_LEFT)) { plate->AddSetSelectBlockNumber(-1);
			Audio::Get()->SoundSEPlayWave(selectSound);}
			else if (Input::Get()->KeybordTrigger(DIK_RIGHT)) { plate->AddSetSelectBlockNumber(1); 
			Audio::Get()->SoundSEPlayWave(selectSound);}
		}

		if (sceneChange_->GetOutEndFlag())
		{
			BaseScene* scene = new TitleScene();
			sceneManager_->SetNextScene(scene);
		}

		plate->SetEnd(player->GetDeadFlag());

		plate->Update();
		player->Update(plate.get());
	}

	sceneChange_->Update();
}

void GameScene::Draw()
{
	Sprite::Get()->Draw(back, Vec2(), static_cast<float>(window_width), static_cast<float>(window_height));

	player->Draw();
	plate->Draw();

	player->ScoreDraw();

	if (player->GetDeadFlag() == true)
	{
		Sprite::Get()->Draw(gameoverGraph, Vec2(), static_cast<float>(window_width), static_cast<float>(window_height));
	}

	sceneChange_->Draw();
}

void GameScene::ShadowDraw()
{

}

void GameScene::Finalize()
{
	Texture::Get()->Delete();
}

