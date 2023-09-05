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


	// ライトグループクラス作成
	lightGroup = std::make_unique<LightGroup>();
	lightGroup->Initialize();
	// 3Dオブエクトにライトをセット
	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightDir(0, XMVECTOR{ 0,0,1,0 });
	lightGroup->SetShadowDir(Vec3(0, 1, -1));
	

	FBXObject3d::SetLight(lightGroup.get());
	Object::SetLight(lightGroup.get());
}


void StageSelect::Update()
{
	
	//ライト更新
	lightGroup->Update();

	if (Input::Get()->KeybordTrigger(DIK_SPACE) || Input::Get()->ControllerDown(ButtonA))
	{
		BaseScene* scene = new GameScene();
		sceneManager_->SetNextScene(scene);
	}
}

void StageSelect::Draw()
{
	DebugText::Get()->Print(10, 10, 4, "Select");
}

void StageSelect::ShadowDraw()
{
	
}

void StageSelect::Finalize()
{}