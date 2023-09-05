#include "GameScene.h"
#include<sstream>
#include<iomanip>
#include "Input.h"
#include"FbxLoader.h"
#include"Shape.h"
#include"StageSelect.h"
#include"SceneManager.h"
#include"ResultScene.h"
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
	Camera::Get()->SetCamera(Vec3{ 0,0,-200 }, Vec3{ 0, 0, 0 }, Vec3{ 0, 1, 0 });
	//�X�v���C�g�摜�ǂݍ���
	FBXObject3d::SetLight(lightGroup.get());
	Object::SetLight(lightGroup.get());
}

void GameScene::Update()
{
	//���C�g�X�V
	lightGroup->Update();
	if (Input::Get()->KeybordTrigger(DIK_SPACE) || Input::Get()->ControllerDown(ButtonA))
	{
		BaseScene* scene = new ResultScene();
		sceneManager_->SetNextScene(scene);
	}
}

void GameScene::Draw()
{
	DebugText::Get()->Print(10, 10, 4, "GameScene");
}

void GameScene::ShadowDraw()
{
	
}

void GameScene::Finalize()
{

}

