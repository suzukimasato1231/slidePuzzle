#pragma once
#include "fbxsdk.h"
#include "Model.h"
#include <d3d12.h>
#include <d3dx12.h>
#include <string>
/// <summary>
/// FBX設定クラス
/// </summary>
class FbxLoader
{
private: // エイリアス
	// std::を省略
	using string = std::string;

public: // 定数
	// モデル格納ルートパス
	static const string baseDirectory;
	// テクスチャがない場合の標準テクスチャファイル名
	static const string defaultTextureFileName;
public:
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static FbxLoader* GetInstance();
	/// <summary>
	/// FBXの行列をXMMatrixに変換
	/// </summary>
	/// <param name="dst">書き込み先</param>
	/// <param name="src">元となるFBX行列</param>
	static void ConvertMatrixFromFbx(DirectX::XMMATRIX* dst, const FbxAMatrix& src);
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="device">D3D12デバイス</param>
	void Initialize(ID3D12Device* device);

	/// <summary>
	/// 後始末
	/// </summary>
	void Finalize();

	/// <summary>
	/// ファイルからFBXモデル読込
	/// </summary>
	/// <param name="modelName">モデル名</param>
	Model *LoadModelFromFile(const string& modelName, const string &file = "");
private:
	// D3D12デバイス
	ID3D12Device* device = nullptr;
	// FBXマネージャ
	FbxManager* fbxManager = nullptr;
	// FBXインポータ
	FbxImporter* fbxImporter = nullptr;
	// コントロールポイントのデータ
	std::vector<std::vector<int>> controlPointsData_;

private:
	// privateなコンストラクタ
	FbxLoader() = default;
	// privateなデストラクタ
	~FbxLoader() = default;
	// コピーコンストラクタを禁止
	FbxLoader(const FbxLoader& obj) = delete;
	// コピー代入演算子を禁止
	void operator=(const FbxLoader& obj) = delete;

	/// <summary>
	/// 再帰的にノード構成を解析
	/// </summary>
	/// <param name="model">読み込み先モデルオブジェクト</param>
	/// <param name="fbxNode">解析対象のノード</param>
	/// <param name="parent">親ノード</param>
	void ParseNodeRecursive(Model* model, FbxNode* fbxNode, Node* parent = nullptr,const  string &file = "");

	/// <summary>
	/// メッシュ読み取り
	/// </summary>
	/// <param name="model">読み込み先モデルオブジェクト</param>
	/// <param name="fbxNode">解析対象のノード</param>
	void ParseMesh(Model* model, FbxNode* fbxNode, const string& file);

	// 面情報読み取り
	void ParseMeshFaces(Model* model, FbxMesh* fbxMesh);
	// マテリアル読み取り
	void ParseMaterial(Model* model, FbxNode* fbxNode, const string& file);
	// テクスチャ読み込み
	void LoadTexture(Model* model, const std::string& fullpath);
	//スキニング情報の読み取り
	void ParseSkin(Model* model, FbxMesh* fbxMesh);

	// ディレクトリを含んだファイルパスからファイル名を抽出する
	std::string ExtractFileName(const std::string& path);

};