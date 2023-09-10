#include"TitleScene.h"
#include<sstream>
#include<iomanip>
#include"Camera.h"
#include"Shape.h"
#include"Input.h"
#include"Texture.h"
#include"GameScene.h"
#include"SceneManager.h"
TitleScene::TitleScene()
{}
TitleScene::~TitleScene()
{

}

void TitleScene::Init()
{
	// ���C�g�O���[�v�N���X�쐬
	lightGroup = std::make_unique<LightGroup>();
	lightGroup->Initialize();
	// 3D�I�u�G�N�g�Ƀ��C�g���Z�b�g
	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightDir(0, XMVECTOR{ 0,-1,0,0 });
	lightGroup->SetShadowDir(Vec3(0, 1, 0));

	//�J�����ݒ�
	Camera::Get()->SetCamera(Vec3{ 0,30,-5 }, Vec3{ 0, -3, 0 }, Vec3{ 0, 1, 0 });

	FBXObject3d::SetLight(lightGroup.get());
	Object::SetLight(lightGroup.get());


	

}

void TitleScene::Update()
{
	lightGroup->Update();

	if (Input::Get()->KeybordTrigger(DIK_SPACE) || Input::Get()->ControllerDown(ButtonA))
	{
		BaseScene* scene = new GameScene();
		sceneManager_->SetNextScene(scene);
	}
}

void TitleScene::Draw()
{
}

void TitleScene::ShadowDraw()
{

}


void TitleScene::Finalize()
{}
