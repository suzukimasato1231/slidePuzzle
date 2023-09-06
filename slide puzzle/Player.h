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
		LEFTUP,
		RIGHTUP,
		LEFTDOWN,
		RIGHTDOWN,
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
	void Update(Plate *plate);
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
private:
	ObjectData pObject;                         //プレイヤーオブジェクト

	const Vec3 firstPos = { -9.0f,0.0f,0.0f };  //最初の位置
	Vec3 position = firstPos;                   //位置
	const float FixedSpeed = 1.0f;              //固定スピード
	float speed = 0.05f;                         //スピード
	float moveAngle = 0.0f;                     //進む角度
	int direction = UP;                         //進む向き

	bool isDead = false;                        //死んだか
};

