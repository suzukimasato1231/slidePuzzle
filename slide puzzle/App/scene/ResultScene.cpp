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

	// シーン遷移の演出の初期化
	sceneChange_ = std::make_unique<SceneChange>();
}

void ResultScene::Update()
{

	if (Input::Get()->KeybordTrigger(DIK_SPACE) && sceneChange_->GetinEndFlag())
	{
		sceneChange_->SceneChangeStart("");
	}

	if (sceneChange_->GetOutEndFlag())
	{
		BaseScene* scene = new TitleScene();
		sceneManager_->SetNextScene(scene);
	}

	lightGroup->Update();
	sceneChange_->Update();
}

void ResultScene::Draw()
{
	DebugText::Get()->Print(10, 10, 4, "Result");
	sceneChange_->Draw();
}

void ResultScene::ShadowDraw()
{

}


void ResultScene::Finalize()
{
	Texture::Get()->Delete();
}
