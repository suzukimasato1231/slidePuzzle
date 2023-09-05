#include "DirectXBase.h"
#include <cassert>
#include"Fps.h"
#include"Window.h"
DirectXBase::DirectXBase()
{
}
void DirectXBase::Initilize()
{
	HRESULT result;
#ifdef _DEBUG
	//デバックレイヤーをオン
	ComPtr<ID3D12Debug1> debugController;
	ComPtr<ID3D12DebugDevice> mDebugDevice;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
	}
#endif
	//アダプタの列挙
	Adapter();

	//コマンドリスト
	CommandListInitilize();

	//深度設定
	DepthBuffer();

	//スワップチェーン
	Swapchain();

	//フェンスの生成
	result = m_dev->CreateFence(m_fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence));


	// imgui初期化
	if (!InitImgui()) {
		assert(0);
	}

	FPS::Get()->Start();
#ifdef _DEBUG
	/*ID3D12DebugDevice* debugInterface;
	if (SUCCEEDED(dev.Get()->QueryInterface(&debugInterface)))
	{
		debugInterface->ReportLiveDeviceObjects(D3D12_RLDO_DETAIL | D3D12_RLDO_IGNORE_INTERNAL);
		debugInterface->Release();
	}*/
#endif
}

void DirectXBase::Adapter()
{
	HRESULT result;
	//DXGIファクトリーの生成
	result = CreateDXGIFactory1(IID_PPV_ARGS(&m_dxgiFactory));

	//アダプターの列挙用
	std::vector<ComPtr<IDXGIAdapter1>>adapters;
	//ここに特定の名前を持つアダプターオブジェクトが入る
	ComPtr<IDXGIAdapter1>tmpAdapter = nullptr;
	//対応レベルの配列
	D3D_FEATURE_LEVEL levels[]
	{
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0
	};
	D3D_FEATURE_LEVEL featureLevel;
	for (int i = 0; i < _countof(levels); i++)
	{
		//採用したアダプターでデバイスを生成
		result = D3D12CreateDevice(tmpAdapter.Get(), levels[i], IID_PPV_ARGS(&m_dev));
		if (result == S_OK)
		{
			//デバイスを生成できた時点でループを抜ける
			featureLevel = levels[i];
			break;
		}
	}

	//ここに特定の名前を持つアダプターオブジェクトが入る
	for (int i = 0;
		m_dxgiFactory->EnumAdapters1(i, &tmpAdapter) !=
		DXGI_ERROR_NOT_FOUND;
		i++)
	{
		adapters.push_back(tmpAdapter);//動的配列に追加する
	}

	for (int i = 0; i < adapters.size(); i++)
	{
		DXGI_ADAPTER_DESC1 adesc;
		adapters[i]->GetDesc1(&adesc);//アダプターの情報を取得
		//ソフトウェアデバイスを回避
		if (adesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
		{
			continue;
		}
		std::wstring strDesc = adesc.Description;//アダプター名
		//Intel UHD Graphics（オンボードグラフィック）を回避
		if (strDesc.find(L"Intel") == std::wstring::npos)
		{
			tmpAdapter = adapters[i];//採用
			break;
		}
	}
}

void DirectXBase::CommandListInitilize()
{
	HRESULT result;
	//コマンドアロケータを生成
	result = m_dev->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&m_cmdAllocator));

	//コマンドリストを生成
	result = m_dev->CreateCommandList(0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		m_cmdAllocator.Get(), nullptr,
		IID_PPV_ARGS(&m_cmdList));

	//標準設定でコマンドキューを生成
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc{};
	//標準設定でコマンドキューを生成
	m_dev->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&m_cmdQueue));
}

void DirectXBase::Swapchain()
{
	HRESULT result;
	//各種設定をしてスワップチェーンを生成
	DXGI_SWAP_CHAIN_DESC1 swapchainDesc{};
	//各種設定をしてスワップチェーンを生成
	swapchainDesc.Width = 1280;
	swapchainDesc.Height = 720;
	swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//色情報を書式
	swapchainDesc.SampleDesc.Count = 1;//マルチサンプルにしない
	swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;//バックバッファー用
	swapchainDesc.BufferCount = 2;//バッファ数を２つに設定
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;//フリップ後は破棄
	swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	ComPtr<IDXGISwapChain1>swapchain1;

	m_dxgiFactory->CreateSwapChainForHwnd(
		m_cmdQueue.Get(),
		Window::Get()->GetHwnd(),
		&swapchainDesc,
		nullptr,
		nullptr,
		&swapchain1);

	swapchain1.As(&m_swapchain);

	//各種設定をしてデスクリプタヒープを生成
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;//レンダーターゲットビュー
	heapDesc.NumDescriptors = 2;//裏表の２つ

	m_dev->CreateDescriptorHeap(&heapDesc,
		IID_PPV_ARGS(&m_rtvHeaps));

	//裏表の2つ分について
	backBuffers.resize(swapchainDesc.BufferCount);
	for (int i = 0; i < 2; i++)
	{
		//スワップチェーンからバッファを取得
		result = m_swapchain->GetBuffer(i, IID_PPV_ARGS(&backBuffers[i]));
		//レンダーターゲットビューの生成
		m_dev->CreateRenderTargetView(
			backBuffers[i].Get(),
			nullptr,
			CD3DX12_CPU_DESCRIPTOR_HANDLE(m_rtvHeaps->GetCPUDescriptorHandleForHeapStart(),//ヒープの先頭アドレス
				i,//デスクリプタ番号
				m_dev->GetDescriptorHandleIncrementSize(heapDesc.Type)
			));
	}
}

void DirectXBase::DepthBuffer()
{
	HRESULT result;
#pragma region	//深度設定
	//深度バッファリソース設定
	CD3DX12_RESOURCE_DESC depthResDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_D32_FLOAT,
		window_width,
		window_height,
		1, 0,
		1, 0,
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);
	//深度バッファの生成
	result = m_dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,//深度値書き込みに使用
		&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0),
		IID_PPV_ARGS(&m_depthBuffer));


	//深度ビュー用デスクリプタヒープの作成
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1;//深度ビューは１つ
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;//デブスステンシルビュー

	result = m_dev->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&m_dsvHeap));

	//深度ビュー作成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;//深度値フォーマット
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	m_dev->CreateDepthStencilView(
		m_depthBuffer.Get(),
		&dsvDesc,
		m_dsvHeap->GetCPUDescriptorHandleForHeapStart());

}

bool DirectXBase::InitImgui()
{
	HRESULT result = S_FALSE;

	// デスクリプタヒープを生成
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	heapDesc.NumDescriptors = 1;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	result = m_dev->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&m_imguiHeap));
	if (FAILED(result)) {
		assert(0);
		return false;
	}

	// スワップチェーンの情報を取得
	DXGI_SWAP_CHAIN_DESC swcDesc = {};
	result = m_swapchain->GetDesc(&swcDesc);
	if (FAILED(result)) {
		assert(0);
		return false;
	}

	return true;
}


void DirectXBase::PreDraw()
{
	//バックバッファの番号を取得（２つなので０番か１番）
	UINT bbIndex = m_swapchain->GetCurrentBackBufferIndex();
#pragma region 1.リソースバリアで書き込み可能に変更
	//表示状態から描画状態に変更
	m_cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(backBuffers[bbIndex].Get(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

#pragma region 2.描画先指定
	//レンダーターゲットビュー用ディスクリプタヒープのハンドルを取得
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvH = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_rtvHeaps->GetCPUDescriptorHandleForHeapStart(),//ヒープの先頭アドレス
		bbIndex,//デスクリプタ番号
		m_dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)
	);

	//深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = m_dsvHeap->GetCPUDescriptorHandleForHeapStart();
	m_cmdList->OMSetRenderTargets(1, &rtvH, false, &dsvH);


#pragma region	//3.画面クリア            R     G     B      A
	m_cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

#pragma region 4.描画コマンドはここから
	//ビューポートの設定コマンド
	m_cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, (FLOAT)window_width, (FLOAT)window_height));
	//シザー短形の設定コマンド
	m_cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, (LONG)window_width, (LONG)window_height));
#ifdef _DEBUG
	// 経過時間計測
	auto now = std::chrono::steady_clock::now();
	m_deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(now - lastUpdate).count() / 1000000.0f;
	m_frameRate = 1.0f / m_deltaTime;
	lastUpdate = now;
	// FPS,CPU使用率表示
	{
		static int count = 0;
		const float FPS_BASIS = 60.0f;
		// 一秒に一度更新
		if (++count > FPS_BASIS) {
			count = 0;
			float cputime = m_deltaTime - m_commandWaitTime;
			char str[50];
			sprintf_s(str, "fps=%03.0f cpu usage=%06.2f%%", m_frameRate, cputime * FPS_BASIS * 100.0f);
			SetWindowTextA(Window::Get()->GetHwnd(), str);
		}
	}
#endif
}

//リソースバリアを戻す
void DirectXBase::ResourceBarrier()
{
#ifdef _DEBUG
	// imgui描画
	//ImGui::Render();
	//ID3D12DescriptorHeap* ppHeaps[] = { imguiHeap.Get() };
	//cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), cmdList.Get());
#endif
	//描画状態から表示状態に変更
	UINT bbIndex = m_swapchain->GetCurrentBackBufferIndex();
	m_cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(backBuffers[bbIndex].Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

	//命令のクローズ
	m_cmdList->Close();
	//コマンドリストの実行
	ID3D12CommandList* cmdLists[] = { m_cmdList.Get() };//コマンドリストの配列
	m_cmdQueue->ExecuteCommandLists(1, cmdLists);
	//バッファをフリップ(裏表の入替え)
	m_swapchain->Present(1, 0);
	//コマンドリストの実行完了を待つ
	m_cmdQueue->Signal(m_fence.Get(), ++m_fenceVal);
	if (m_fence->GetCompletedValue() != m_fenceVal)
	{
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		m_fence->SetEventOnCompletion(m_fenceVal, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}
	FPS::Get()->run();
	m_cmdAllocator->Reset();//キューをクリア
	m_cmdList->Reset(m_cmdAllocator.Get(), nullptr);//再びコマンドリストを貯める準備
}

ID3D12Device* DirectXBase::GetDevice()
{
	return m_dev.Get();
}

ID3D12GraphicsCommandList* DirectXBase::GetCmandList()
{
	return m_cmdList.Get();
}

void DirectXBase::ClearDepthBuffer()
{
	// 深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvH = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_dsvHeap->GetCPUDescriptorHandleForHeapStart());
	// 深度バッファのクリア
	m_cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}



