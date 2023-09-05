#include "ResultScene.h"
#include"DebugText.h"
#include"Shape.h"
#include <FBXObject3d.h>
#include<Input.h>
#include"TitleScene.h"
#include"SceneManager.h"
ResultScene::ResultScene()
{}
ResultScene::~ResultScene()
{}
void ResultScene::Init()
{
	// ライトグループクラス作成
	lightGroup = std::make_unique<LightGroup>();
	lightGroup->Initialize();
	// 3Dオブエクトにライトをセット
	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightDir(0, XMVECTOR{ 0,-1,0,0 });
	lightGroup->SetShadowDir(Vec3(0, 1, 0));


	Camera::Get()->SetCamera(Vec3{ 0,30,-5 }, Vec3{ 0, -3, 0 }, Vec3{ 0, 1, 0 });
	FBXObject3d::SetLight(lightGroup.get());
	Object::SetLight(lightGroup.get());
}

void ResultScene::Update()
{

	if (Input::Get()->KeybordTrigger(DIK_SPACE))
	{
		BaseScene* scene = new TitleScene();
		sceneManager_->SetNextScene(scene);
	}

	lightGroup->Update();
}

void ResultScene::Draw()
{
	DebugText::Get()->Print(10, 10, 4, "Result");
}

void ResultScene::ShadowDraw()
{

}


void ResultScene::Finalize()
{}
