#include "ResultScene.h"
#include"DebugText.h"
#include"Shape.h"
#include <FBXObject3d.h>
#include<Input.h>
#include"TitleScene.h"
#include"SceneManager.h"
#include "../slide puzzle/Score.h"
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

	back = Sprite::Get()->SpriteCreate(L"Resources/back.png");

	Score::GetInstance()->SetIsScoreDraw(false);

	//オブジェクト生成
	gameoverGraph = Sprite::Get()->SpriteCreate(L"Resources/gameover.png");
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
	Sprite::Get()->Draw(back, Vec2(), static_cast<float>(window_width), static_cast<float>(window_height));
	Sprite::Get()->Draw(gameoverGraph, Vec2(), static_cast<float>(window_width), static_cast<float>(window_height));
	Score::GetInstance()->Draw(Vec2(static_cast<float>((window_width / 3)) + 90.0f, static_cast<float>(window_height / 2) - 70.0f));
	sceneChange_->Draw();
}

void ResultScene::ShadowDraw()
{

}


void ResultScene::Finalize()
{
	Texture::Get()->Delete();
}
