#pragma once

#include <string>
#include <vector>
#include <DirectXMath.h>
#include <DirectXTex.h>
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include<fbxsdk.h>
#include<Texture.h>
// ノード
struct Node
{
	// 名前
	std::string name;
	// ローカルスケール
	DirectX::XMVECTOR scaling = { 1,1,1,0 };
	// ローカル回転角
	DirectX::XMVECTOR rotation = { 0,0,0,0 };
	// ローカル移動
	DirectX::XMVECTOR translation = { 0,0,0,1 };
	// ローカル変形行列
	DirectX::XMMATRIX transform;
	// グローバル変形行列
	DirectX::XMMATRIX globalTransform;
	// 親ノード
	Node* parent = nullptr;
};

/// <summary>
/// FBXモデルクラス
/// </summary>
class Model
{
public:
	// フレンドクラス
	friend class FbxLoader;
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using TexMetadata = DirectX::TexMetadata;
	using ScratchImage = DirectX::ScratchImage;
	// std::を省略
	using string = std::string;
	template <class T> using vector = std::vector<T>;
public://定数
	//ボーンインデックスの最大数
	static const int MAX_BONE_INDICES = 4;

	//FBXシーン
	FbxScene* fbxScene = nullptr;
public: // サブクラス
	// 頂点データ構造体
	struct VertexPosNormalUvSkin
	{
		DirectX::XMFLOAT3 pos; // xyz座標
		DirectX::XMFLOAT3 normal; // 法線ベクトル
		DirectX::XMFLOAT2 uv;  // uv座標
		UINT boneIndex[MAX_BONE_INDICES];//ボーン　番号
		float boneWeight[MAX_BONE_INDICES];//ボーン　重み
	};
	//ボーン構造体
	struct Bone
	{
		//名前
		std::string name;
		//初期姿勢の逆行列
		DirectX::XMMATRIX invInitialPose;
		//クラスター(FBX側のボーン情報)
		FbxCluster* fbxCluster;
		//コンストラクタ
		Bone(const std::string& name)
		{
			this->name = name;
		}
	};
public:
	// バッファ生成
	void CreateBuffers(ID3D12Device* device);
	// 描画
	void Draw(ID3D12GraphicsCommandList* cmdList, bool shadowFlag);
	// モデルの変形行列取得
	const XMMATRIX& GetModelTransform() { return meshNode->globalTransform; }

	//getter
	FbxScene* GetFbxScene()const { return fbxScene; }

	//getter
	std::vector<Bone>& GetBones() { return bones; }
	//デスクリプタ
	~Model();

private:
	// モデル名
	std::string m_name;
	// ノード配列
	std::vector<Node> nodes;
	// メッシュを持つノード
	Node* meshNode = nullptr;
	// 頂点データ配列
	std::vector<VertexPosNormalUvSkin> vertices;
	// 頂点インデックス配列
	std::vector<unsigned short> indices;
	// アンビエント係数
	DirectX::XMFLOAT3 m_ambient = { 1,1,1 };
	// ディフューズ係数
	DirectX::XMFLOAT3 m_diffuse = { 1,1,1 };
	// テクスチャメタデータ
	DirectX::TexMetadata m_metadata = {};
	// スクラッチイメージ
	DirectX::ScratchImage m_scratchImg = {};
	// 頂点バッファ
	ComPtr<ID3D12Resource>m_vertBuff;
	// インデックスバッファ
	ComPtr<ID3D12Resource> m_indexBuff;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW m_vbView = {};
	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW m_ibView = {};
	//ボーン配列
	std::vector<Bone>bones;

	TextureData m_textureNum = {};
};

