#pragma once
#include<Object.h>
#include <array>
//パネルの種類
enum PanelStatus
{
	NONE,
	WIDTHSTRAIGHTLINE,    //横直線
	HEIGHTSTRAIGHTLINE,   //縦直線
	CROSS,                //十字
	CUR_LEFTUP,           //カーブ左と上
	CUR_LEFTDOWN,         //カーブ左と下
	CUR_RIGHTUP,          //カーブ右と上
	CUR_RIGHTDOWN,        //カーブ右と下
	DCUR_LEFTUP_RIGHTDOWN,//ダブルカーブ左と上＆右と下
	DCUR_LEFTDOWN_RIGHTUP,//ダブルカーブ左と下＆右と上
};
/// <summary>
/// パネル
/// </summary>
class Plate
{
public:
	struct StageBlockData
	{
		std::vector<Vec3> position;
		std::vector<PanelStatus> blockType;
	};

public:
	//コンストラクタ
	Plate();
	//ですコンストラクタ
	~Plate();
	/// <summary>
	/// 初期化
	/// </summary>
	void Init();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	//描画
	void Draw();

	int GetStage(int i, int j) { return stage[j][i]; }

	void SetKeyFlag(bool num) { keyFlag_ = num; }

private:

	void None();

	void BeforeMove();

	void Move();

	void Selection();

	void NullStageBlockNumber();

	int GetStageBlockNumber(int num);

	int GetSaveBlockNumber(int num);

private:
	ObjectData container;//3x4の入れ物
	ObjectData plateCross;//十字のプレート
	ObjectData plateDoubleTurn;//ダブルカーブ
	ObjectData plateLine;      //一直線
	ObjectData plateSingleTurn;//単一カーブ

	int stage[3][4] = {
		CUR_RIGHTDOWN,WIDTHSTRAIGHTLINE,WIDTHSTRAIGHTLINE,CUR_LEFTDOWN,
		 HEIGHTSTRAIGHTLINE,CROSS,NONE,HEIGHTSTRAIGHTLINE,
		CUR_RIGHTUP,WIDTHSTRAIGHTLINE,WIDTHSTRAIGHTLINE,CUR_LEFTUP,
	};

	// 関数の管理
	std::vector<std::function<void()>> func_;
	// 関数の番号
	size_t phase_ = 0;
	// キーフラグ
	bool keyFlag_ = false;
	// ステージ上のブロックのデータ
	StageBlockData blockData_;
	// ステージの位置の配列
	std::vector<Vec3> seaveStageBlockPosition_;
	// 動かすブロックの番号
	std::vector<int> moveBlockNumber;
	// 空のブロックの番号
	std::vector<int> nullBlockNumber;
};

