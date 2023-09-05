#pragma once
#include <DirectXMath.h>
#include <wrl.h>
#include"Vec.h"
#include"DirectXBase.h"
/// <summary>
/// シャドウマップ
/// </summary>
class ShadowMap
{
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMMATRIX = DirectX::XMMATRIX;
public:
	struct VertexPosUv
	{
		DirectX::XMFLOAT3 pos;			//xyz座標
		DirectX::XMFLOAT2 uv;			//uv座標
	};

	struct ConstantBuffer_b0
	{
		DirectX::XMMATRIX mat;			//変換行列
	};
private:
	static ID3D12Device* dev;
	//テクスチャバッファ
	ComPtr<ID3D12Resource> m_texbuff;
	//SRv用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> m_descHeapSRV;
	//深度バッファ
	ComPtr<ID3D12Resource> m_depthbuff;
	//SRv用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> m_depthHeapSRV;
	//RTV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> m_descHeapRTV;
	//DSV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> m_descHeapDSV;
	//グラフィックスパイプライン
	ComPtr<ID3D12PipelineState> m_pipelineState;
	//ルートシグネチャ
	ComPtr<ID3D12RootSignature> m_rootSignature;
	//頂点バッファ
	ComPtr<ID3D12Resource> m_vertBuff;
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW m_vbView{};
	//定数バッファ
	ComPtr<ID3D12Resource> m_constBuff;
	//画像横幅
	const int c_texture_width = 2500;	
	//画像縦幅
	const int c_texture_height = 2500;	
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	ShadowMap();
	/// <summary>
	/// デスコンストラクタ
	/// </summary>
	~ShadowMap();
	/// <summary>
	/// 初期化
	/// </summary>
	void Init();
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	void Draw(ID3D12GraphicsCommandList* cmdList);
	/// <summary>
	/// 描画前
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	void PreDraw(ID3D12GraphicsCommandList* cmdList);
	/// <summary>
	/// 描画後
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	void PostDraw(ID3D12GraphicsCommandList* cmdList);
	/// <summary>
	///深度バッファを取得
	/// </summary>
	/// <returns></returns>
	ID3D12Resource* GetTexbuff() { return m_depthbuff.Get(); }
private:
	/// <summary>
	/// パイプライン作成
	/// </summary>
	/// <param name="dev">デバイス</param>
	void CreateGraphicsPipelineState(ID3D12Device* dev);
};