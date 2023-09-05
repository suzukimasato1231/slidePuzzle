#pragma once
#include <d3dx12.h>
#include"Singleton.h"
#include<dxgi1_6.h>
#include "Pipeline.h"
//テクスチャデータ
struct TextureData
{
	D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	D3D12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	ComPtr<ID3D12Resource> texbuff;
	int s_texNum = 0;
};
/// <summary>
/// テクスチャクラス
/// </summary>
class Texture :public Singleton<Texture>
{
public:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	template <class T> using vector = std::vector<T>;
	friend Singleton<Texture>;
public:
	static ID3D12Device* dev;					//デバイス
	static const int textureMax = 2024;

	Texture();

	~Texture();
	//初期化
	void Init(ID3D12Device* dev);
	//画像読み込み
	TextureData LoadTexture(const wchar_t* filename);
	//OBJ画像読み込み
	TextureData OBJLoadTexture(const std::string& directoryPath, const std::string& filename);

	TextureData FbxLoadTexture(const DirectX::Image* img, CD3DX12_RESOURCE_DESC texresDesc);

	//影用の深度値保存
	void LoadShadowTexture(ID3D12Resource* texbuff);
	//被写界深度用
	void LoadCameraTexture(ID3D12Resource* texbuff);
	//ポストエフェクト保存
	void LoadPostEfectTexture(ID3D12Resource* texbuff);
	//desc獲得
	ID3D12DescriptorHeap* GetDescHeap();

	TextureData GetShadowTexture()const { return m_shadowData; }

	TextureData GetCameraDepth() const { return m_cameraDepth; }

	TextureData GetPostEfect() const { return m_postEfect; }
	//白画像
	TextureData GetWhite()const { return m_white; }
private:
	//定数バッファ用のデスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap >m_descHeap = { nullptr };

	//テクスチャデータの作成
	int m_texNum = 0;
	//白画像保存用
	TextureData m_white = {};
	//影保存用
	TextureData m_shadowData = {};
	//ポストエフェクト保存
	TextureData m_postEfect = {};
	//カメラ深度
	TextureData m_cameraDepth = {};
};