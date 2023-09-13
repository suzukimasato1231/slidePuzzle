#include <Object.h>
#include<memory>
#include"FBXObject3d.h"
#include<Audio.h>
#include"BaseScene.h"
#include<Sprite.h>
#include"../slide puzzle/SceneChange.h"
/// <summary>
/// シーン移動
/// </summary>
enum class ResultNext
{
	ResultNextStage,
	ResultSelect,
};

/// <summary>
/// リザルトシーン
/// </summary>
class ResultScene:public BaseScene
{
private:

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	ResultScene();
	/// <summary>
	/// デスコンストラクタ
	/// </summary>
	~ResultScene();
	/// <summary>
	/// ループ初期化
	/// </summary>
	void Init()override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="stageNum">ステージ</param>
	void Draw()override;
	/// <summary>
	/// 影描画
	/// </summary>
	void ShadowDraw()override;

	void Finalize()override;
private:
	std::unique_ptr<LightGroup> lightGroup;
private:
	// シーンチェンジ
	std::unique_ptr<SceneChange> sceneChange_;

	SpriteData back;
	SpriteData gameoverGraph;
};