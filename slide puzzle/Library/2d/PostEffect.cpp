#include "PostEffect.h"
#include"ShaderManager.h"
using namespace DirectX;
extern const int window_width;
extern const int window_height;

const float PostEffect::clearColor[4] = { 0.2f,0.2f,0.4f,0.0f };//緑っぽい色
ID3D12Device* PostEffect::dev = nullptr;
PostEffect::PostEffect()
{}

void PostEffect::Initialize(ID3D12Device* dev)
{
	HRESULT result;

	this->dev = dev;
	assert(dev);

	CreatePipeline(dev);

	//頂点データ
	Sprite::VertexPosUv vertices[] = {
		{{-1.0f,-1.0f,0.0f},{0.0f,1.0f}},
		{{-1.0f,+1.0f,0.0f},{0.0f,0.0f}},
		{{+1.0f,-1.0f,0.0f},{1.0f,1.0f}},
		{{+1.0f,+1.0f,0.0f},{1.0f,0.0f}},
	};

	//頂点バッファ生成
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices)),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_sprite.vertBuff));
	assert(SUCCEEDED(result));
#ifdef _DEBUG
	m_sprite.vertBuff->SetName(L"SpriteVert");
#endif
	//頂点バッファへのデータ転送
	Sprite::VertexPosUv* vertMap = nullptr;
	result = m_sprite.vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		memcpy(vertMap, vertices, sizeof(vertices));
		m_sprite.vertBuff->Unmap(0, nullptr);
	}

	//頂点バッファビューの作成
	m_sprite.vbView.BufferLocation = m_sprite.vertBuff->GetGPUVirtualAddress();
	m_sprite.vbView.SizeInBytes = sizeof(Sprite::VertexPosUv) * 4;
	m_sprite.vbView.StrideInBytes = sizeof(Sprite::VertexPosUv);

	//定数バッファ
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(Sprite::ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_constBuff)
	);
	assert(SUCCEEDED(result));

#ifdef _DEBUG
	m_constBuff->SetName(L"SpriteConst");
#endif

	//テクスチャリソース設定
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		window_width,
		window_height,
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
	);

	//テクスチャバッファの生成

	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK,
			D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_R8G8B8A8_UNORM, clearColor),
		IID_PPV_ARGS(&m_texbuff));
	assert(SUCCEEDED(result));

#ifdef _DEBUG
	m_texbuff->SetName(L"SpriteBuff");
#endif

	{//テクスチャを赤クリア
		//画素数
		const UINT pixelCount = window_width * window_height;
		//画像1行分のデータサイズ
		const UINT rowPitch = sizeof(UINT) * window_width;
		//画像全体のデータサイズ
		const UINT depthPitch = rowPitch * window_height;
		//画像イメージ
		UINT* img = new UINT[pixelCount];
		for (int i = 0; unsigned(i) < pixelCount; i++) { img[i] = 0xff0000ff; }

		//テクスチャバッファにデータ転送
		result = m_texbuff->WriteToSubresource(0, nullptr,
			img, rowPitch, depthPitch);
		assert(SUCCEEDED(result));
		delete[] img;
	}

	//SRV用デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc = {};
	srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvDescHeapDesc.NumDescriptors = 2;
	//SRV用デスクリプタヒープを生成
	result = dev->CreateDescriptorHeap(&srvDescHeapDesc, IID_PPV_ARGS(&m_descHeapSRV));
	assert(SUCCEEDED(result));
#ifdef _DEBUG
	m_descHeapSRV->SetName(L"SpriteSRV");
#endif

	Texture::Get()->LoadPostEfectTexture(m_texbuff.Get());

#ifdef _DEBUG
	m_texbuff->SetName(L"SpriteBuff");
#endif

	//RTV用デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc{};
	rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescHeapDesc.NumDescriptors = 1;
	//RTV用デスクリプタヒープを生成
	result = dev->CreateDescriptorHeap(&rtvDescHeapDesc, IID_PPV_ARGS(&m_descHeapRTV));
	assert(SUCCEEDED(result));
#ifdef _DEBUG
	m_descHeapRTV->SetName(L"SpriteRTV");
#endif
	//デスクリプタヒープにRTV作成
	dev->CreateRenderTargetView(m_texbuff.Get(),
		nullptr,
		m_descHeapRTV->GetCPUDescriptorHandleForHeapStart());

	//深度バッファリソース設定
	CD3DX12_RESOURCE_DESC depthResDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			DXGI_FORMAT_R32_TYPELESS,
			window_width,
			window_height,
			1, 0,
			1, 0,
			D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

	//深度バッファの生成
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&depthResDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&CD3DX12_CLEAR_VALUE(DXGI_FORMAT_D32_FLOAT, 1.0f, 0),
		IID_PPV_ARGS(&m_depthBuff));

	assert(SUCCEEDED(result));
#ifdef _DEBUG
	m_depthBuff->SetName(L"SpriteDepth");
#endif
	//DSV用デスクリプタヒープ
	D3D12_DESCRIPTOR_HEAP_DESC DescHeapDesc{};
	DescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	DescHeapDesc.NumDescriptors = 1;
	//DSV用デスクリプタヒープを作成
	result = dev->CreateDescriptorHeap(&DescHeapDesc, IID_PPV_ARGS(&m_descHeapDSV));
	assert(SUCCEEDED(result));

	//デスクリプタヒープにDSV作成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;//深度値フォーマット
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dev->CreateDepthStencilView(m_depthBuff.Get(),
		&dsvDesc,
		m_descHeapDSV->GetCPUDescriptorHandleForHeapStart());
}

void PostEffect::CreatePipeline(ID3D12Device* dev)
{
	m_pipelineSet = Pipeline::DepthOfFieldPipeline;
	//SetPipeline(2);
}

void PostEffect::Draw(ID3D12GraphicsCommandList* cmdList,const Vec4 &color)
{

	DrawPost(m_sprite, { 0, 0 }, 500.0f, 500.0f, { 0.0f,0.0f }, color, false, false);

}

void PostEffect::PreDrawScene(ID3D12GraphicsCommandList* cmdList)
{
	//リソースバリアを変更
	cmdList->ResourceBarrier(1,
		&CD3DX12_RESOURCE_BARRIER::Transition(m_texbuff.Get(),
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			D3D12_RESOURCE_STATE_RENDER_TARGET));

	//レンダーターゲットビュー用ディスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvH =
		m_descHeapRTV->GetCPUDescriptorHandleForHeapStart();
	//深度ステンシルビュー用デスクリプタヒープのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH =
		m_descHeapDSV->GetCPUDescriptorHandleForHeapStart();
	//レンダーターゲットをセット
	cmdList->OMSetRenderTargets(1, &rtvH, false, &dsvH);

	//ビューポートの設定
	cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, static_cast<FLOAT>(window_width), static_cast<FLOAT>(window_height)));
	//シザリング短形の設定
	cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, static_cast<LONG>(window_width), static_cast<LONG>(window_height)));

	//全画面クリア
	cmdList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);
	//深度バッファのクリア
	cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

}
void PostEffect::PostDrawScene(ID3D12GraphicsCommandList* cmdList)
{
	//リソースバリアを変更
	cmdList->ResourceBarrier(1,
		&CD3DX12_RESOURCE_BARRIER::Transition(m_texbuff.Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
}

void PostEffect::UpdatePost(SpriteData& sprite, const Vec2 &position, float width, float height, const Vec2 &anchorpoint, const Vec4 &color, bool isFlipX, bool isFlipY)
{
	//定数バッファの転送
	Sprite::ConstBufferData* constMap = nullptr;
	HRESULT result = m_constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->mat = XMMatrixIdentity();
	constMap->color = color;
	m_constBuff->Unmap(0, nullptr);
}

void PostEffect::DrawPost(SpriteData& sprite, const Vec2 &position, float width, float height, const Vec2 &anchorpoint, const Vec4 &color, bool isFlipX, bool isFlipY)
{
	//パイプラインステートの設定
	Sprite::cmdList->SetPipelineState(m_pipelineSet.pipelinestate.Get());
	//ルートシグネチャの設定
	Sprite::cmdList->SetGraphicsRootSignature(m_pipelineSet.rootsignature.Get());
	//プリミティブ形状を設定
	Sprite::cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	UpdatePost(sprite, position, width, height, anchorpoint, color, isFlipX, isFlipY);
	//頂点バッファをセット
	Sprite::cmdList->IASetVertexBuffers(0, 1, &sprite.vbView);
	//定数バッファをセット
	Sprite::cmdList->SetGraphicsRootConstantBufferView(0, m_constBuff->GetGPUVirtualAddress());


	//テクスチャ用デスクリプタヒープの設定
	ID3D12DescriptorHeap* ppHeaps[] = { Texture::Get()->GetDescHeap()};
	Sprite::cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	//シェーダーリソースビューをセット
	
	Sprite::cmdList->SetGraphicsRootDescriptorTable(1, Texture::Get()->GetPostEfect().gpuDescHandleSRV);

	//ヒープの２番目にあるSRVをルートパラメータ１番に設定
	Sprite::cmdList->SetGraphicsRootDescriptorTable(2, Texture::Get()->GetCameraDepth().gpuDescHandleSRV);


	//ポリゴンの描画（４頂点で四角形）
	Sprite::cmdList->DrawInstanced(4, 1, 0, 0);
}

void PostEffect::SetPipeline(int num)
{
	switch (num)
	{
	case 0:
		m_pipelineSet = Pipeline::PostNormalCreateGraphicsPipeline(dev, ShaderManager::postNormalShader);
		break;
	case 1:
		m_pipelineSet = Pipeline::PostNormalCreateGraphicsPipeline(dev, ShaderManager::postTestShader);
		break;
	case 2:
		m_pipelineSet = Pipeline::DepthOfFieldPipeline;
		break;
	}
}


