#pragma once
#include<Object.h>
#include <array>
#include <iostream>
#include <algorithm>
#include<Audio.h>
#include"Easing.h"

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

enum Crystal
{
	NOCRYSTALL,
	CRYSTALL,
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
		std::vector<Crystal>crytallFlag;
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


	void SetKeyFlag(bool num) { keyFlag_ = num; }

	void AddSetSelectBlockNumber(int num);

	void DeleteCrstal(int num);

	size_t GetPanelNum() { return blockData_.blockType.size(); }

	Vec3 GetPanelPos(int i) { return  blockData_.position[i]; }

	PanelStatus GetPanelStatus(int i) { return blockData_.blockType[i]; }

	Crystal GetCrystal(int i) { return blockData_.crytallFlag[i]; }

private:

	void None();

	void BeforeMove();

	void Move();

	void Selection();

	void SelectionMove();

	void NullStageBlockNumber();

	int GetStageBlockNumber(int num);

	int GetSaveBlockNumber(int num);

	bool LeftRight(int count, int number, int num, bool flag);

	bool UpDown(int count, int number, int num, bool flag);

	bool SelectionBlockCount();

	void CrystalRote();

	void CrystalCreate();

private:
	ObjectData container;//3x4の入れ物
	ObjectData plateCross;//十字のプレート
	ObjectData plateDoubleTurn;//ダブルカーブ
	ObjectData plateLine;      //一直線
	ObjectData plateSingleTurn;//単一カーブ
	ObjectData crystallObject;//クリスタル
	ObjectData landmarkObject;
	TextureData landmarkTex = {};
	Vec3 crystalRote = {};
	const Vec3 crystalRoteSpeed = {0.0f,1.0f,0.0f};
	SoundData decide = {};

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
	std::vector<int> moveBlockNumber_;
	// 空のブロックの番号
	std::vector<int> nullBlockNumber_;
	// 選択しているステージの位置の番号
	int selectionStageNumber_ = 4;
	// 選択しているブロックの番号
	int selectionBlockNumber_ = 0;
	// 選択しているブロックの番号を保存
	int saveSelectionBlockNumber_ = 0;
	// 選択しているブロックの位置
	Vec3 selectionBlockPosition_ = {};
	// イージングのデータ
	std::unique_ptr<EaseData> easeData_;
};

