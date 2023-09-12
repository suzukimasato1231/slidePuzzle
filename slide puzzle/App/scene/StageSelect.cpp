#include "StageSelect.h"
#include<sstream>
#include<iomanip>
#include "Input.h"
#include"Shape.h"
#include"DebugText.h"
#include"Object.h"
#include"SceneManager.h"
#include"GameScene.h"
StageSelect::StageSelect()
{}
StageSelect::~StageSelect()
{}

void StageSelect::Init()
{
	//ライトグループクラス作成
	lightGroup = std::make_unique<LightGroup>();
	lightGroup->Initialize();
	// 3Dオブエクトにライトをセット
	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightDir(0, XMVECTOR{ 0,-1,0,0 });
	lightGroup->SetShadowDir(Vec3(0, 1, 0));

	Camera::Get()->SetCamera(Vec3{ 0,20,-1 }, Vec3{ 0, -3, 0 }, Vec3{ 0, 1, 0 });
	FBXObject3d::SetLight(lightGroup.get());
	Object::SetLight(lightGroup.get());

	player = std::make_unique<Player>();
	player->Init();
	plate = std::make_unique<Plate>();
	plate->Init();

	back = Sprite::Get()->SpriteCreate(L"Resources/back.png");
	//オブジェクト生成
	startGraph = Sprite::Get()->SpriteCreate(L"Resources/start.png");

	selectSound = Audio::Get()->SoundLoadWave("Resources/Sound/select.wav");

	// シーン遷移の演出の初期化
	sceneChange_ = std::make_unique<SceneChange>();
}


void StageSelect::Update()
{
	//ライト更新
	lightGroup->Update();

	if ((Input::Get()->KeybordTrigger(DIK_SPACE) || Input::Get()->ControllerDown(ButtonA)) && sceneChange_->GetinEndFlag())
	{
		sceneChange_->SceneChangeStart("");
		Audio::Get()->SoundSEPlayWave(selectSound);
	}

	if (sceneChange_->GetOutEndFlag())
	{
		BaseScene* scene = new GameScene();
		sceneManager_->SetNextScene(scene);
	}

	sceneChange_->Update();
}

void StageSelect::Draw()
{
	Sprite::Get()->Draw(back, Vec2(), static_cast<float>(window_width), static_cast<float>(window_height));

	plate->Draw();

	Sprite::Get()->Draw(startGraph, Vec2(), static_cast<float>(window_width), static_cast<float>(window_height));

	sceneChange_->Draw();
}

void StageSelect::ShadowDraw()
{
	
}

void StageSelect::Finalize()
{
	Texture::Get()->Delete();
}