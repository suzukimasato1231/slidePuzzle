#include"TitleScene.h"
#include<sstream>
#include<iomanip>
#include"Camera.h"
#include"Shape.h"
#include"Input.h"
#include"Texture.h"
#include"StageSelect.h"
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
	Camera::Get()->SetCamera(Vec3{ 0,0,-15 }, Vec3{ 0, -3, 0 }, Vec3{ 0, 1, 0 });

	FBXObject3d::SetLight(lightGroup.get());
	Object::SetLight(lightGroup.get());


	//�I�u�W�F�N�g����
	tortoise = Shape::CreateOBJ("sphere");
	//�X�v���C�g�쐬
	ui = Sprite::Get()->SpriteCreate(L"Resources/gutitubo.jpeg");

}

void TitleScene::Update()
{
	lightGroup->Update();

	if (Input::Get()->KeybordTrigger(DIK_SPACE) || Input::Get()->ControllerDown(ButtonA))
	{
		BaseScene* scene = new StageSelect();
		sceneManager_->SetNextScene(scene);
	}
}

void TitleScene::Draw()
{

	Vec3 position = {}, scale = { 1.0f,1.0f,1.0f }, angle = {};
	//�I�u�W�F�N�g�\��
	Object::Draw(tortoise, position, scale, angle);

	Vec2 uiPos = { 200.0f,150.0f };
	Sprite::Get()->Draw(ui, uiPos, 100.0f, 50.0f);

	DebugText::Get()->Print(10, 10, 4, "Title");
}

void TitleScene::ShadowDraw()
{

}


void TitleScene::Finalize()
{}
