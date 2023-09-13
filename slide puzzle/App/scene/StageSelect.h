#pragma once
#include "DirectXBase.h"
#include "Safe_delete.h"
#include"Collision.h"
#include"Audio.h"
#include"FBXObject3d.h"
#include"Object.h"
#include<memory>
#include"BaseScene.h"
#include"../slide puzzle/Player.h"
#include"../slide puzzle/plate.h"
#include"../slide puzzle/SceneChange.h"
extern const int window_width;
extern const int window_height;
/// <summary>
/// ステージセレクト
/// </summary>
class StageSelect:public BaseScene
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	StageSelect();
	/// <summary>
	/// デスコンストラクタ
	/// </summary>
	~StageSelect();
	/// <summary>
	/// 初期化ループ
	/// </summary>
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
	/// 影の描画
	/// </summary>
	void ShadowDraw()override;

	void Finalize()override;
public:
	std::unique_ptr<LightGroup>lightGroup;
private://定義
	std::unique_ptr<Player>player;
	std::unique_ptr<Plate>plate;
	SpriteData back;
	SpriteData titleGraph;
	SpriteData startGraph;
	SpriteData sousaGraph;
	SoundData selectSound;
	// シーンチェンジ
	std::unique_ptr<SceneChange> sceneChange_;
};