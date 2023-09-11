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
	//Audioクラス作成
	audio = std::make_unique<Audio>();
	//ライトグループクラス作成
	lightGroup = std::make_unique<LightGroup>();
	lightGroup->Initialize();
	// 3Dオブエクトにライトをセット
	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightDir(0, XMVECTOR{ 0,-1,0,0 });
	lightGroup->SetShadowDir(Vec3(0, 1, 0));
	//音データ読み込み
	//カメラ位置をセット
	Camera::Get()->SetCamera(Vec3{ 0,20,-1 }, Vec3{ 0, -3, 0 }, Vec3{ 0, 1, 0 });
	//スプライト画像読み込み
	FBXObject3d::SetLight(lightGroup.get());
	Object::SetLight(lightGroup.get());

	player = std::make_unique<Player>();
	player->Init();
	plate = std::make_unique<Plate>();
	plate->Init();

	back = Sprite::Get()->SpriteCreate(L"Resources/back.png");

	//オブジェクト生成
	gameoverGraph = Sprite::Get()->SpriteCreate(L"Resources/gameover.png");
}

void GameScene::Update()
{
	//ライト更新
	lightGroup->Update();

	if (Input::Get()->KeybordTrigger(DIK_SPACE) && player->GetDeadFlag() == true)
	{
		BaseScene* scene = new TitleScene();
		sceneManager_->SetNextScene(scene);
	}
	else
	{
		if (Input::Get()->KeybordTrigger(DIK_SPACE)) { plate->SetKeyFlag(true); }
		else if (Input::Get()->KeybordTrigger(DIK_UP)) { plate->AddSetSelectBlockNumber(-4); }
		else if (Input::Get()->KeybordTrigger(DIK_DOWN)) { plate->AddSetSelectBlockNumber(4); }
		else if (Input::Get()->KeybordTrigger(DIK_LEFT)) { plate->AddSetSelectBlockNumber(-1); }
		else if (Input::Get()->KeybordTrigger(DIK_RIGHT)) { plate->AddSetSelectBlockNumber(1); }
		plate->Update();
	}
	player->Update(plate.get());
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
}

void GameScene::ShadowDraw()
{

}

void GameScene::Finalize()
{

}

