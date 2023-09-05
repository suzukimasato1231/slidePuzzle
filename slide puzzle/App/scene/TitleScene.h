#pragma once
#include "DirectXBase.h"
#include"DebugText.h"
#include"Collision.h"
#include"Audio.h"
#include"FBXObject3d.h"
#include"Object.h"
#include"Sprite.h"
#include<memory>
#include"BaseScene.h"
extern const int window_width;
extern const int window_height;
/// <summary>
/// タイトルシーン
/// </summary>
class TitleScene :public BaseScene
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	TitleScene();
	/// <summary>
	/// デスコンストラクタ
	/// </summary>
	~TitleScene();

	void Init()override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;
	/// <summary>
	/// 描画
	/// </summary>
	void Draw()override;
	/// <summary>
	/// 影描画
	/// </summary>
	void ShadowDraw()override;

	void Finalize()override;
private:
	std::unique_ptr<LightGroup>lightGroup;
private://定義


	ObjectData tortoise;

	SpriteData ui;
};


