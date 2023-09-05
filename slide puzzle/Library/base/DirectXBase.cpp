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
	//�f�o�b�N���C���[���I��
	ComPtr<ID3D12Debug1> debugController;
	ComPtr<ID3D12DebugDevice> mDebugDevice;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
	}
#endif
	//�A�_�v�^�̗�
	Adapter();

	//�R�}���h���X�g
	CommandListInitilize();

	//�[�x�ݒ�
	DepthBuffer();

	//�X���b�v�`�F�[��
	Swapchain();

	//�t�F���X�̐���
	result = m_dev->CreateFence(m_fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence));


	// imgui������
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
	//DXGI�t�@�N�g���[�̐���
	result = CreateDXGIFactory1(IID_PPV_ARGS(&m_dxgiFactory));

	//�A�_�v�^�[�̗񋓗p
	std::vector<ComPtr<IDXGIAdapter1>>adapters;
	//�����ɓ���̖��O�����A�_�v�^�[�I�u�W�F�N�g������
	ComPtr<IDXGIAdapter1>tmpAdapter = nullptr;
	//�Ή����x���̔z��
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
		//�̗p�����A�_�v�^�[�Ńf�o�C�X�𐶐�
		result = D3D12CreateDevice(tmpAdapter.Get(), levels[i], IID_PPV_ARGS(&m_dev));
		if (result == S_OK)
		{
			//�f�o�C�X�𐶐��ł������_�Ń��[�v�𔲂���
			featureLevel = levels[i];
			break;
		}
	}

	//�����ɓ���̖��O�����A�_�v�^�[�I�u�W�F�N�g������
	for (int i = 0;
		m_dxgiFactory->EnumAdapters1(i, &tmpAdapter) !=
		DXGI_ERROR_NOT_FOUND;
		i++)
	{
		adapters.push_back(tmpAdapter);//���I�z��ɒǉ�����
	}

	for (int i = 0; i < adapters.size(); i++)
	{
		DXGI_ADAPTER_DESC1 adesc;
		adapters[i]->GetDesc1(&adesc);//�A�_�v�^�[�̏����擾
		//�\�t�g�E�F�A�f�o�C�X�����
		if (adesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
		{
			continue;
		}
		std::wstring strDesc = adesc.Description;//�A�_�v�^�[��
		//Intel UHD Graphics�i�I���{�[�h�O���t�B�b�N�j�����
		if (strDesc.find(L"Intel") == std::wstring::npos)
		{
			tmpAdapter = adapters[i];//�̗p
			break;
		}
	}
}

void DirectXBase::CommandListInitilize()
{
	HRESULT result;
	//�R�}���h�A���P�[�^�𐶐�
	result = m_dev->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(&m_cmdAllocator));

	//�R�}���h���X�g�𐶐�
	result = m_dev->CreateCommandList(0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		m_cmdAllocator.Get(), nullptr,
		IID_PPV_ARGS(&m_cmdList));

	//�W���ݒ�ŃR�}���h�L���[�𐶐�
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc{};
	//�W���ݒ�ŃR�}���h�L���[�𐶐�
	m_dev->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&m_cmdQueue));
}

void DirectXBase::Swapchain()
{
	HRESULT result;
	//�e��ݒ�����ăX���b�v�`�F�[���𐶐�
	DXGI_SWAP_CHAIN_DESC1 swapchainDesc{};
	//�e��ݒ�����ăX���b�v�`�F�[���𐶐�
	swapchainDesc.Width = 1280;
	swapchainDesc.Height = 720;
	swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//�F��������
	swapchainDesc.SampleDesc.Count = 1;//�}���`�T���v���ɂ��Ȃ�
	swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;//�o�b�N�o�b�t�@�[�p
	swapchainDesc.BufferCount = 2;//�o�b�t�@�����Q�ɐݒ�
	swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;//�t���b�v��͔j��
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

	//�e��ݒ�����ăf�X�N���v�^�q�[�v�𐶐�
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;//�����_�[�^�[�Q�b�g�r���[
	heapDesc.NumDescriptors = 2;//���\�̂Q��

	m_dev->CreateDescriptorHeap(&heapDesc,
		IID_PPV_ARGS(&m_rtvHeaps));

	//���\��2���ɂ���
	backBuffers.resize(swapchainDesc.BufferCount);
	for (int i = 0; i < 2; i++)
	{
		//�X���b�v�`�F�[������o�b�t�@���擾
		result = m_swapchain->GetBuffer(i, IID_PPV_ARGS(&backBuffers[i]));
		//�����_�[�^�[�Q�b�g�r���[�̐���
		m_dev->CreateRenderTargetView(
			backBuffers[i].Get(),
			nullptr,
			CD3DX12_CPU_DESCRIPTOR_HANDLE(m_rtvHeaps->GetCPUDescriptorHandleForHeapStart(),//�q�[�v�̐擪�A�h���X
				i,//�f�X�N���v�^�ԍ�
				m_dev->GetDescriptorHandleIncrementSize(heapDesc.Type)
			));
	}
}

void DirectXBase::DepthBuffer()
{
	HRESULT result;
#pragma region	//�[�x�ݒ�
	//�[�x�o�b�t�@���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC depthResDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_D32_FLOAT,
		window_width,
		window_height,
		1, 0,
		1, 0,
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);
	//�[�x�o�b�t�@�̐���
	result = m_dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,//�[�x�l�������݂Ɏg�p
		&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0),
		IID_PPV_ARGS(&m_depthBuffer));


	//�[�x�r���[�p�f�X�N���v�^�q�[�v�̍쐬
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1;//�[�x�r���[�͂P��
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;//�f�u�X�X�e���V���r���[

	result = m_dev->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&m_dsvHeap));

	//�[�x�r���[�쐬
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;//�[�x�l�t�H�[�}�b�g
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	m_dev->CreateDepthStencilView(
		m_depthBuffer.Get(),
		&dsvDesc,
		m_dsvHeap->GetCPUDescriptorHandleForHeapStart());

}

bool DirectXBase::InitImgui()
{
	HRESULT result = S_FALSE;

	// �f�X�N���v�^�q�[�v�𐶐�
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	heapDesc.NumDescriptors = 1;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	result = m_dev->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&m_imguiHeap));
	if (FAILED(result)) {
		assert(0);
		return false;
	}

	// �X���b�v�`�F�[���̏����擾
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
	//�o�b�N�o�b�t�@�̔ԍ����擾�i�Q�Ȃ̂łO�Ԃ��P�ԁj
	UINT bbIndex = m_swapchain->GetCurrentBackBufferIndex();
#pragma region 1.���\�[�X�o���A�ŏ������݉\�ɕύX
	//�\����Ԃ���`���ԂɕύX
	m_cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(backBuffers[bbIndex].Get(),
		D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

#pragma region 2.�`���w��
	//�����_�[�^�[�Q�b�g�r���[�p�f�B�X�N���v�^�q�[�v�̃n���h�����擾
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvH = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_rtvHeaps->GetCPUDescriptorHandleForHeapStart(),//�q�[�v�̐擪�A�h���X
		bbIndex,//�f�X�N���v�^�ԍ�
		m_dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)
	);

	//�[�x�X�e���V���r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH = m_dsvHeap->GetCPUDescriptorHandleForHeapStart();
	m_cmdList->OMSetRenderTargets(1, &rtvH, false, &dsvH);


#pragma region	//3.��ʃN���A            R     G     B      A
	m_cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

#pragma region 4.�`��R�}���h�͂�������
	//�r���[�|�[�g�̐ݒ�R�}���h
	m_cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, (FLOAT)window_width, (FLOAT)window_height));
	//�V�U�[�Z�`�̐ݒ�R�}���h
	m_cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, (LONG)window_width, (LONG)window_height));
#ifdef _DEBUG
	// �o�ߎ��Ԍv��
	auto now = std::chrono::steady_clock::now();
	m_deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(now - lastUpdate).count() / 1000000.0f;
	m_frameRate = 1.0f / m_deltaTime;
	lastUpdate = now;
	// FPS,CPU�g�p���\��
	{
		static int count = 0;
		const float FPS_BASIS = 60.0f;
		// ��b�Ɉ�x�X�V
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

//���\�[�X�o���A��߂�
void DirectXBase::ResourceBarrier()
{
#ifdef _DEBUG
	// imgui�`��
	//ImGui::Render();
	//ID3D12DescriptorHeap* ppHeaps[] = { imguiHeap.Get() };
	//cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	//ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), cmdList.Get());
#endif
	//�`���Ԃ���\����ԂɕύX
	UINT bbIndex = m_swapchain->GetCurrentBackBufferIndex();
	m_cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(backBuffers[bbIndex].Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

	//���߂̃N���[�Y
	m_cmdList->Close();
	//�R�}���h���X�g�̎��s
	ID3D12CommandList* cmdLists[] = { m_cmdList.Get() };//�R�}���h���X�g�̔z��
	m_cmdQueue->ExecuteCommandLists(1, cmdLists);
	//�o�b�t�@���t���b�v(���\�̓��ւ�)
	m_swapchain->Present(1, 0);
	//�R�}���h���X�g�̎��s������҂�
	m_cmdQueue->Signal(m_fence.Get(), ++m_fenceVal);
	if (m_fence->GetCompletedValue() != m_fenceVal)
	{
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		m_fence->SetEventOnCompletion(m_fenceVal, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}
	FPS::Get()->run();
	m_cmdAllocator->Reset();//�L���[���N���A
	m_cmdList->Reset(m_cmdAllocator.Get(), nullptr);//�ĂуR�}���h���X�g�𒙂߂鏀��
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
	// �[�x�X�e���V���r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvH = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_dsvHeap->GetCPUDescriptorHandleForHeapStart());
	// �[�x�o�b�t�@�̃N���A
	m_cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}



