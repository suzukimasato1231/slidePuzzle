#pragma once
#include"Object.h"
#include"Sprite.h"
#include <array>
#include <algorithm>
#include <functional>

class Score final
{
private:
	Score() = default;
	~Score() = default;

public:
	Score(const Score& r) = delete;
	Score& operator=(const Score& r) = delete;

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
	static Score* GetInstance();

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 終了
	/// </summary>
	void Finalize();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw(Vec2 pos = {});

	void Reset();

	void CrstalGet(Vec3 position);
	int GetComboTime() { return comboTime; }
	void SetIsScoreDraw(bool flag) { isScoreDraw = flag; }

private:
	SpriteData backSprite;
	SpriteData comboSprite;
	SpriteData numberGraph[10] = {};
	const int comboTimeMax = 250;//コンボ最大
	int comboTime = 0;           //コンボ時間
	int pointNum = 0;            //得点
	int pointPlas = 10;          //得点加算
	TextureData particleGraph = {};

	//スコア描画
	ObjectData scoreDraw = {};
	SpriteData comboGraph[11] = {};
	Vec3 scorePos = {};
	bool isScoreDraw = false;
	const int scoreDrawTimeMax = 50;
	int scoreDrawTime = 0;
};

