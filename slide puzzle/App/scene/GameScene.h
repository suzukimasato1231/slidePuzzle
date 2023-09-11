#pragma once
#include "DirectXBase.h"
#include"DebugText.h"
#include"ParticleManager.h"
#include<memory>
#include"BaseScene.h"
#include"Object.h"
#include<Audio.h>
#include"../slide puzzle/Player.h"
#include"../slide puzzle/plate.h"
extern const int window_width;
extern const int window_height;

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene:public BaseScene
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameScene();
	/// <summary>
	/// デスコンストラクタ
	/// </summary>
	~GameScene();
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
	std::unique_ptr<Audio> audio;
private://定義
	
	std::unique_ptr<Player>player;
	std::unique_ptr<Plate>plate;

	SpriteData back;
	int scene = 0;

	SpriteData gameoverGraph;

};