#pragma once
#include<Windows.h>
#include<wrl.h>
#include <d3dx12.h>
#include <cstdlib>
#include "Window.h."
#include"Singleton.h"
#include <imgui_impl_win32.h>
#include <imgui_impl_dx12.h>
#include <chrono>
extern const int window_width;
extern const int window_height;
/// <summary>
/// DirectX12クラス
/// </summary>
class DirectXBase :public Singleton<DirectXBase>
{
public:
	friend Singleton<DirectXBase>;
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	
private://メンバ変数
	//ウィンドウ関連

	//DirectX3D関連
	ComPtr<ID3D12Device> m_dev;
	ComPtr<ID3D12GraphicsCommandList >m_cmdList;
	ComPtr<IDXGIFactory6> m_dxgiFactory;
	ComPtr<ID3D12CommandAllocator> m_cmdAllocator;
	ComPtr<ID3D12CommandQueue >m_cmdQueue;
	ComPtr<IDXGISwapChain4> m_swapchain;
	std::vector<ComPtr<ID3D12Resource>> backBuffers;
	ComPtr<ID3D12Resource> m_depthBuffer;
	ComPtr<ID3D12DescriptorHeap>m_rtvHeaps;
	//深度設定
	ComPtr<ID3D12DescriptorHeap> m_dsvHeap;
	//フェンスの生成
	ComPtr<ID3D12Fence> m_fence;
	UINT64 m_fenceVal = 0;

	ComPtr<ID3D12DescriptorHeap> m_imguiHeap;
	float m_deltaTime = 0.0f;
	float m_frameRate = 0.0f;
	float m_commandWaitTime = 0.0f;
	std::chrono::steady_clock::time_point lastUpdate;
private:
	//アダプタの列挙
	void Adapter();

	//コマンドリスト
	void CommandListInitilize();

	//スワップチェーン
	void Swapchain();
	//深度設定
	void DepthBuffer();
	//imgui初期化
	bool InitImgui();
	
public://メンバ関数
	DirectXBase();

	~DirectXBase()
	{

	}
	//初期化
	void Initilize();

	//描画前処理
	void PreDraw();

	//描画後処理
	void ResourceBarrier();

	ID3D12Device* GetDevice();

	ID3D12GraphicsCommandList* GetCmandList();

	ID3D12Resource* GetDeth() { return m_depthBuffer.Get(); };

	void ClearDepthBuffer();
	
};
