#pragma once
#include<Object.h>
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
};

