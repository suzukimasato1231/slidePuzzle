#include"TitleScene.h"
#include<sstream>
#include<iomanip>
#include"Camera.h"
#include"Shape.h"
#include"Input.h"
#include"Texture.h"
#include"StageSelect.h"
#include"SceneManager.h"
#include"../slide puzzle/Player.h"
#include"../slide puzzle/plate.h"
TitleScene::TitleScene()
{}
TitleScene::~TitleScene()
{

}

void TitleScene::Init()
{
	// ライトグループクラス作成
	lightGroup = std::make_unique<LightGroup>();
	lightGroup->Initialize();
	// 3Dオブエクトにライトをセット
	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightDir(0, XMVECTOR{ 0,-1,0,0 });
	lightGroup->SetShadowDir(Vec3(0, 1, 0));

	//カメラ設定
	Camera::Get()->SetCamera(Vec3{ 0,20,-1 }, Vec3{ 0, -3, 0 }, Vec3{ 0, 1, 0 });

	FBXObject3d::SetLight(lightGroup.get());
	Object::SetLight(lightGroup.get());

	player = std::make_unique<Player>();
	player->Init();
	plate = std::make_unique<Plate>();
	plate->Init();

	back = Sprite::Get()->SpriteCreate(L"Resources/back.png");

	//オブジェクト生成
	titleGraph = Sprite::Get()->SpriteCreate(L"Resources/title.png");
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
	Sprite::Get()->Draw(back, Vec2(), static_cast<float>(window_width), static_cast<float>(window_height));

	player->Draw();
	plate->Draw();

	player->ScoreDraw();

	Sprite::Get()->Draw(titleGraph, Vec2(), static_cast<float>(window_width), static_cast<float>(window_height));
}

void TitleScene::ShadowDraw()
{

}


void TitleScene::Finalize()
{}
