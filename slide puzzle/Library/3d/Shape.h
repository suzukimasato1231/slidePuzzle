#pragma once
#include"Object.h"
/// <summary>
/// 図形モデル
/// </summary>
class Shape
{
public:
	Shape();
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="dev">デバイス</param>
	static void Init(ID3D12Device* dev);
public://図形モデル
	/// <summary>
	/// 三角錐
	/// </summary>
	/// <param name="width">横の長さ</param>
	/// <param name="height">縦の長さ</param>
	/// <returns></returns>
	static ObjectData CreateTriangularPyramid(const float width, const float height);
	/// <summary>
	/// 短形
	/// </summary>
	/// <param name="width">横の長さ</param>
	/// <param name="height">縦の長さ</param>
	/// <returns></returns>
	static ObjectData CreateRect(const float width, const float height);
	/// <summary>
	/// 円
	/// </summary>
	/// <param name="r">半径</param>
	/// <param name="num">分割数</param>
	/// <returns></returns>
	static ObjectData CreateCircle(const float r, const int num);
	/// <summary>
	/// 立方体
	/// </summary>
	/// <param name="width">横の長さ</param>
	/// <param name="height">縦の長さ</param>
	/// <param name="depth">深さ</param>
	/// <returns></returns>
	static ObjectData CreateSquare(const float width, const float height, const float depth);
	/// <summary>
	/// 円柱
	/// </summary>
	/// <param name="division">分割数</param>
	/// <param name="prizmHeight">高さ</param>
	/// <param name="radius">底面の半径</param>
	/// <returns></returns>
	static ObjectData CreateCylinder(int division, int prizmHeight, int radius);
public://OBJ関連
	/// <summary>
	/// OBJオブジェクト作成
	/// </summary>
	/// <param name="filename">filename</param>
	/// <param name="smoothing"></param>
	/// <returns></returns>
	static ObjectData CreateOBJ(const std::string &filename, bool smoothing = false, const std::string &filePath = "");
	//マテリアル読み込み
	static int LoadMaterial(const std::string& directoryPath, const std::string& filename, ObjectData& polygon);
private:
	/// <summary>
	/// エッジ平滑化データの追加
	/// </summary>
	/// <param name="indexPosition">座標インデックス</param>
	/// <param name="indexVertex">頂点インデックス</param>
	static void AddSmoothData(ObjectData& polygon, unsigned short indexPosition, unsigned short indexVertex);
	///<summary>
	///平滑化された頂点法線の計算
	/// </summary>
	static void CalculateSmoothedVertexNormals(ObjectData& polygon);
	/// <summary>
	/// 頂点データの数を取得
	/// </summary>
	/// <returns>頂点データの数</returns>
	static inline size_t GetVertexCount(ObjectData& polygon);
public:
	static ID3D12Device* dev;					//デバイス

	static void CreateModel(ObjectData& polygon);

	static void OBJCreateModel(ObjectData& polygon);
};