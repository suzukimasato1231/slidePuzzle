#pragma once
#include"Object.h"

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
private:
	/// <summary>
	/// 方向チェンジ
	/// </summary>
	void DirectChange(Plate* plate);
	/// <summary>
	/// プレイヤー移動
	/// </summary>
	void Move();
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
	void TurnLeft();
	/// <summary>
	/// ターン右
	/// </summary>
	void TurnRight();
	/// <summary>
	/// ターン上
	/// </summary>
	void TurnUp();
	/// <summary>
	/// ターン下
	/// </summary>
	void TurnDown();
	/// <summary>
	/// ターン右から入る左下
	/// </summary>
	void TurnLeftDown();
	/// <summary>
	/// ターン下から入る右上設定
	/// </summary>
	void TurnUpRight();
	/// <summary>
	/// ターン左から入る右sita設定
	/// </summary>
	void TurnRightDown();
	/// <summary>
	/// ターン下から入る左上設定
	/// </summary>
	void TurnUpLeft();
	/// <summary>
	/// ターン上から入る左下設定
	/// </summary>
	void TurnDownLeft();
	/// <summary>
	/// ターン左から入る右上設定
	/// </summary>
	void TurnRightUp();
	/// <summary>
	/// ターン右から入る左上設定
	/// </summary>
	void TurnLeftUp();
	/// <summary>
	/// ターン上から入る右下設定
	/// </summary>
	void TurnDownRight();
private:
	ObjectData pObject;                         //プレイヤーオブジェクト
	Vec3 position = {};                         //位置
	Vec3 positionMemory = {};
	Vec3 rotation = {};
	int direction = UP;                         //進む向き

	//プレート基礎位置
	const Vec2 basePos = { -9.0f,6.0f };
	const Vec2 varPos = { 6.1f,6.1f };
	Vec2 pSize = { 0.001f,0.001f }, sSize = { 6.1f,6.1f };

	bool isDead = false;                        //死んだか


	const float turnSpeed = 0.01f;
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

