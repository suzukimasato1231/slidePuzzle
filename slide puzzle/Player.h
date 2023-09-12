#pragma once
#include"Object.h"
#include"Sprite.h"
#include<ParticleManager.h>
#include<memory>
class Plate;
/// <summary>
/// プレイヤークラス
/// </summary>
class Player
{
private:
	/// <summary>
	/// プレイヤーが進む向き
	/// </summary>
	enum Direction
	{
		UP,
		DOWN,
		LEFT,
		RIGHT,
	};
public:
	//コンストラクタ
	Player();
	//デスコンストラクタ
	~Player();
	/// <summary>
	/// 初期化
	/// </summary>
	void Init();
	/// <summary>
	/// 更新
	/// </summary>
	void Update(Plate* plate);
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();
	/// <summary>
	/// スコア描画
	/// </summary>
	void ScoreDraw();
	/// <summary>
	/// 死亡フラグ取得
	/// </summary>
	/// <returns></returns>
	bool GetDeadFlag() { return isDead; }
private:
	/// <summary>
	/// コンボ
	/// </summary>
	void PointUpdate();
	/// <summary>
	/// 方向チェンジ
	/// </summary>
	void DirectChange(Plate* plate);
	/// <summary>
	/// プレイヤー移動
	/// </summary>
	void Move();
	/// <summary>
	/// クリスタル獲得
	/// </summary>
	void CrstalGet(Plate* plate);
	/// <summary>
	/// カーブ初期化
	/// </summary>
	void TurnInit();
	/// <summary>
	/// 直線初期化
	/// </summary>
	void LineInit();
	/// <summary>
	/// ターン左
	/// </summary>
	void TurnLeft(const Vec2 platePos);
	/// <summary>
	/// ターン右
	/// </summary>
	void TurnRight(const Vec2 platePos);
	/// <summary>
	/// ターン上
	/// </summary>
	void TurnUp(const Vec2 platePos);
	/// <summary>
	/// ターン下
	/// </summary>
	void TurnDown(const Vec2 platePos);
	/// <summary>
	/// ターン右から入る左下
	/// </summary>
	void TurnLeftDown(const Vec2 platePos);
	/// <summary>
	/// ターン下から入る右上設定
	/// </summary>
	void TurnUpRight(const Vec2 platePos);
	/// <summary>
	/// ターン左から入る右sita設定
	/// </summary>
	void TurnRightDown(const Vec2 platePos);
	/// <summary>
	/// ターン下から入る左上設定
	/// </summary>
	void TurnUpLeft(const Vec2 platePos);
	/// <summary>
	/// ターン上から入る左下設定
	/// </summary>
	void TurnDownLeft(const Vec2 platePos);
	/// <summary>
	/// ターン左から入る右上設定
	/// </summary>
	void TurnRightUp(const Vec2 platePos);
	/// <summary>
	/// ターン右から入る左上設定
	/// </summary>
	void TurnLeftUp(const Vec2 platePos);
	/// <summary>
	/// ターン上から入る右下設定
	/// </summary>
	void TurnDownRight(const Vec2 platePos);
	/// <summary>
	/// 死んだときの回転演出
	/// </summary>
	void DeadRotation();
private:
	ObjectData pObject;                         //プレイヤーオブジェクト
	Vec3 position = {};                         //位置
	Vec3 positionMemory = {};
	Vec3 rotation = {};
	int direction = UP;                         //進む向き
	int keepDirection = UP;

	//プレート基礎位置
	const Vec2 basePos = { -9.0f,6.0f };
	const Vec2 varPos = { 6.1f,6.1f };
	Vec2 pSize = { 0.001f,0.001f }, sSize = { 6.1f,6.1f };

	bool isDead = false;                        //死んだか


	//コンボ関連
	SpriteData backSprite;
	SpriteData comboSprite;
	SpriteData numberGraph[10] = {};
	const int comboTimeMax = 250;//コンボ最大
	int comboTime = 0;           //コンボ時間
	int pointNum = 0;            //コンボ
	int pointPlas = 10;          //コンボ加算
	std::unique_ptr < ParticleManager> comboParticle = nullptr;
	TextureData particleGraph = {};

	//スコア描画
	ObjectData scoreDraw = {};
	SpriteData comboGraph[11] = {};
	Vec3 scorePos = {};
	bool isScoreDraw = false;
	const int scoreDrawTimeMax = 50;
	int scoreDrawTime = 0;


	//クリスタルスピード関連
	const float speedPlas = 0.004f;//スピードUP数
	const float speedMax = 0.1f;  //最大スピード
	const float turnSpeedMin = 0.01f;//最小スピード数
	float turnSpeed = 0.01f;      //スピード

	float plateTime = 0.0f;
	//パネル移動用
	Vec3 posStart = {};
	Vec3 posEnd = {};
	bool isPosFlag = false;

	Vec3 posStartSecond = {};
	Vec3 posEndSecond = {};
	bool isPosSecondFlag = false;

	//カーブ用変数
	Vec3 turnEnd = {};
	Vec3 turnStart = {};
	bool isCurFlag = false;

	Vec3 turnEndSecond = {};
	Vec3 turnStartSecond = {};
};

