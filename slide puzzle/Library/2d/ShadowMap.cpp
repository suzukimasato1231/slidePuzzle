#include "ShadowMap.h"
#include <d3dx12.h>
#include <d3dcompiler.h>
#include <cassert>
#include"Object.h"
ID3D12Device* ShadowMap::dev = nullptr;
ShadowMap::ShadowMap()
{}
ShadowMap::~ShadowMap()
{}
void ShadowMap::Init()
{
	ID3D12Device* dev = DirectXBase::Get()->GetDevice();
	HRESULT result;
#ifdef _DEBUG
	dev->SetName(L"shadow");
	CreateGraphicsPipelineState(dev);
#endif
	//頂点バッファ生成
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(VertexPosUv) * 4),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_vertBuff));
	assert(SUCCEEDED(result));
#ifdef _DEBUG
	m_vertBuff->SetName(L"ShadowVert");
#endif
	//頂点データ転送
	VertexPosUv vertices[4] = {};

	vertices[0].pos = { -1.0f, -1.0f , 0.0f };
	vertices[1].pos = { -1.0f, 1.0, 0.0f };
	vertices[2].pos = { 1.0, -1.0f , 0.0f };
	vertices[3].pos = { 1.0, 1.0, 0.0f };

	vertices[0].uv = { 0.0f, 1.0f };
	vertices[1].uv = { 0.0f, 0.0f };
	vertices[2].uv = { 1.0f, 1.0f };
	vertices[3].uv = { 1.0f, 0.0f };

	//頂点バッファへのデータ転送
	VertexPosUv* vertMap = nullptr;
	result = m_vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		memcpy(vertMap, vertices, sizeof(vertices));
		m_vertBuff->Unmap(0, nullptr);
	}

	//頂点バッファビューの作成
	m_vbView.BufferLocation = m_vertBuff->GetGPUVirtualAddress();
	m_vbView.SizeInBytes = sizeof(VertexPosUv) * 4;
	m_vbView.StrideInBytes = sizeof(VertexPosUv);

	//定数バッファの生成
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstantBuffer_b0) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_constBuff)
	);
	assert(SUCCEEDED(result));
#ifdef _DEBUG
	m_constBuff->SetName(L"shadowConst");
#endif
	//テクスチャリソース設定
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		window_width,
		window_height,
		1, 1, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
	);

	//テクスチャバッファの生成
	float color[] = { 1.0f,1.0f,1.0f,1.0f };
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM, color),
		IID_PPV_ARGS(&m_texbuff)
	);
	assert(SUCCEEDED(result));
#ifdef _DEBUG
	m_texbuff->SetName(L"shadowTex");
#endif
	//テクスチャを仮想生成
	const UINT pixelCount = window_width * window_height;
	//画像1行分のデータサイズ
	const UINT rowPitch = sizeof(UINT) * window_width;
	//画像全体のデータサイズ
	const UINT depthPitch = rowPitch * window_height;
	//画像イメージ
	UINT* img = new UINT[pixelCount];
	for (int i = 0; i < static_cast<int>(pixelCount); i++) { img[i] = 0xff0000ff; }

	//テクスチャバッファにデータ転送
	result = m_texbuff->WriteToSubresource(0, nullptr,
		img, rowPitch, depthPitch);
	assert(SUCCEEDED(result));
	delete[] img;


	//RTV用デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc{};
	rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescHeapDesc.NumDescriptors = 1;

	//RTV用デスクリプタヒープを生成
	result = dev->CreateDescriptorHeap(
		&rtvDescHeapDesc,
		IID_PPV_ARGS(&m_descHeapRTV)
	);
	assert(SUCCEEDED(result));
#ifdef _DEBUG
	m_descHeapRTV->SetName(L"ShadowRTV");
#endif
	dev->CreateRenderTargetView(
		m_texbuff.Get(),
		nullptr,
		m_descHeapRTV->GetCPUDescriptorHandleForHeapStart()
	);

	//深度バッファリソース設定
	CD3DX12_RESOURCE_DESC depthResDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			DXGI_FORMAT_R32_TYPELESS,
			c_texture_width,
			c_texture_height,
			1, 0,
			1, 0,
			D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL
		);

	//深度バッファの生成
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0),
		IID_PPV_ARGS(&m_depthbuff)
	);
	assert(SUCCEEDED(result));
#ifdef _DEBUG
	m_depthbuff->SetName(L"ShadowDepth");
#endif
	//DSV用デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC DescHeapDesc{};
	DescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	DescHeapDesc.NumDescriptors = 1;
	//DSV用デスクリプタヒープを作成
	result = dev->CreateDescriptorHeap(
		&DescHeapDesc,
		IID_PPV_ARGS(&m_descHeapDSV)
	);
	assert(SUCCEEDED(result));

	//デスクリプタヒープにDSV作成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;	//深度値フォーマット
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dev->CreateDepthStencilView(
		m_depthbuff.Get(),
		&dsvDesc,
		m_descHeapDSV->GetCPUDescriptorHandleForHeapStart()
	);

#ifdef _DEBUG
	m_descHeapDSV->SetName(L"ShadowDSV");
#endif
}

void ShadowMap::Draw(ID3D12GraphicsCommandList* cmdList)
{
#ifdef _DEBUG
	HRESULT result = S_OK;
	//定数バッファにデータ転送
	ConstantBuffer_b0 data = {};
	data.mat = XMMatrixIdentity();

	//GPU上のバッファに対応した仮想メモリを取得
	ConstantBuffer_b0* constMap = nullptr;
	void* dst = nullptr;
	result = m_constBuff->Map(0, nullptr, (void**)&dst);
	memcpy(dst, &data, sizeof(ConstantBuffer_b0));
	m_constBuff->Unmap(0, nullptr);

	//パイプラインステートの設定
	cmdList->SetPipelineState(m_pipelineState.Get());
	//ルートシグネチャの設定
	cmdList->SetGraphicsRootSignature(m_rootSignature.Get());
	//プリミティブ形状を設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//頂点バッファの設定
	cmdList->IASetVertexBuffers(0, 1, &m_vbView);

	ID3D12DescriptorHeap* ppHeaps[] = { Texture::Get()->GetDescHeap() };
	//デスクリプタヒープをセット
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);


	//定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(0, m_constBuff->GetGPUVirtualAddress());

	//SRV
	cmdList->SetGraphicsRootDescriptorTable(
		1, Texture::Get()->GetShadowTexture().gpuDescHandleSRV);
	//描画コマンド
	cmdList->DrawInstanced(4, 1, 0, 0);
#endif
}

void ShadowMap::PreDraw(ID3D12GraphicsCommandList* cmdList)
{
	//リソースバリアを変更
	cmdList->ResourceBarrier(
		1,
		&CD3DX12_RESOURCE_BARRIER::Transition(m_texbuff.Get(),
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			D3D12_RESOURCE_STATE_RENDER_TARGET)
	);

	//レンダーターゲットビュー用デスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvH =
		m_descHeapRTV->GetCPUDescriptorHandleForHeapStart();
	//深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH =
		m_descHeapDSV->GetCPUDescriptorHandleForHeapStart();
	//レンダーターゲットをセット
	cmdList->OMSetRenderTargets(1, &rtvH, false, &dsvH);

	//ビューポートの設定
	cmdList->RSSetViewports(
		1,
		&CD3DX12_VIEWPORT(0.0f, 0.0f, static_cast<FLOAT>(c_texture_width), static_cast<FLOAT>(c_texture_height))
	);
	//シザリング矩形の設定
	cmdList->RSSetScissorRects(
		1,
		&CD3DX12_RECT(0, 0, static_cast<LONG>(c_texture_width), static_cast<LONG>(c_texture_height))
	);

	//深度バッファのクリア
	cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void ShadowMap::PostDraw(ID3D12GraphicsCommandList* cmdList)
{
	cmdList->ResourceBarrier(
		1,
		&CD3DX12_RESOURCE_BARRIER::Transition(m_texbuff.Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE)
	);
}


void ShadowMap::CreateGraphicsPipelineState(ID3D12Device* dev)
{
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob;
	ComPtr<ID3DBlob> psBlob;
	ComPtr<ID3DBlob> errorBlob;

	//頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/Shaders/ShadowMap/ShadowMapVS.hlsl",	//シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,					//インクルード可能にする
		"main",											//エントリーポイント名
		"vs_5_0",												//シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,		//デバッグ用設定
		0,
		&vsBlob, &errorBlob);

	if (FAILED(result)) {
		//errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		//エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		assert(0);
	}

	//ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/Shaders/ShadowMap/ShadowMapPS.hlsl",				//シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,					//インクルード可能にする
		"main",											//エントリーポイント名
		"ps_5_0",											//シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,	//デバッグ用設定
		0,
		&psBlob, &errorBlob);
	if (FAILED(result)) {
		//errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		//エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		assert(0);
	}

	//頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ //xy座標(1行で書いたほうが見やすい)
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ //uv座標(1行で書いたほうが見やすい)
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	//グラフィックスパイプラインの流れを設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	//サンプルマスク
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; //標準設定
	//ラスタライザステート
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//デプスステンシルステート
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS; //常に上書きルール

	//レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	//RBGA全てのチャンネルを描画

	blenddesc.BlendEnable = true;

	//半透明合成
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	//共通設定
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	//ブレンドステートの設定
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	//深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	//頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	//図形の形状設定（三角形）
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 1;							//描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;	//0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1;							//1ピクセルにつき1回サンプリング

	//デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV = {};
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); //t0レジスタ

	//ルートパラメータ(定数バッファに送るやつの設定、最初の引数でregisterを選択)
	//分けておくと必要な情報だけ転送したりできてエコ
	CD3DX12_ROOT_PARAMETER rootparams[3] = {};
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[2].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);

	//スタティックサンプラー
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc =
		CD3DX12_STATIC_SAMPLER_DESC(0, D3D12_FILTER_MIN_MAG_MIP_POINT); //s0レジスタ

	//ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	//バージョン自動判定のシリアライズ
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc,
		D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));

	//ルートシグネチャの生成
	result = dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(),
		rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature));
	assert(SUCCEEDED(result));

	gpipeline.pRootSignature = m_rootSignature.Get();
	//グラフィックスパイプラインの生成
	result = dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&m_pipelineState));
	assert(SUCCEEDED(result));
}
