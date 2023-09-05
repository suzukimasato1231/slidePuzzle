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
	Camera::Get()->SetCamera(Vec3{ 0,0,-200 }, Vec3{ 0, 0, 0 }, Vec3{ 0, 1, 0 });
	//スプライト画像読み込み
	FBXObject3d::SetLight(lightGroup.get());
	Object::SetLight(lightGroup.get());
}

void GameScene::Update()
{
	//ライト更新
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

