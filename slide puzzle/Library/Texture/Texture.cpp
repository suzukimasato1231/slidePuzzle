#include "Texture.h"
#include <DirectXTex.h>
#include<dxgi1_6.h>
#include<DirectXMath.h>
using namespace DirectX;
ID3D12Device* Texture::dev = nullptr;

Texture::Texture()
{
}

Texture::~Texture()
{
}

void Texture::Init(ID3D12Device* dev)
{
	this->dev = dev;

	LoadTexture(L"Resources/white1x1.png");
}

TextureData Texture::LoadTexture(const wchar_t* filename)
{
	HRESULT result = S_FALSE;

	TextureData textureData = {};

	if (m_texNum == 0)
	{
		//設定構造体
		D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc{};
		descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダーから見える
		descHeapDesc.NumDescriptors = textureMax;//定数バッファの数
		//デスクリプタヒープの生成
		result = dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&m_descHeap));
	}


	TexMetadata metadata{};
	ScratchImage scratchImg{};
	std::string i;
	//WICテクスチャのロード
	result = LoadFromWICFile(
		filename,
		WIC_FLAGS_NONE,
		&metadata, scratchImg);

	const Image* img = scratchImg.GetImage(0, 0, 0);//生データ抽出

	//リソース設定
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format,
		metadata.width,
		(UINT)metadata.height,
		(UINT16)metadata.arraySize,
		(UINT16)metadata.mipLevels);

	//テクスチャ用バッファの生成
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,//テクスチャ用指定
		nullptr,
		IID_PPV_ARGS(&textureData.texbuff));
#ifdef _DEBUG
	textureData.texbuff->SetName(L"Texture");
#endif
	//テクスチャバッファにデータ転送
	result = textureData.texbuff->WriteToSubresource(
		0,
		nullptr,//全領域へコピー
		img->pixels,//元データアドレス
		(UINT)img->rowPitch,//1ラインサイズ
		(UINT)img->slicePitch//全サイズ
	);

	//デスクリプタヒープの先頭ハンドルを取得
	textureData.cpuDescHandleSRV = m_descHeap->GetCPUDescriptorHandleForHeapStart();
	//ハンドルのアドレスを進める
	textureData.cpuDescHandleSRV.ptr += m_texNum * dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandleStart = m_descHeap->GetGPUDescriptorHandleForHeapStart();
	UINT descHandleIncrementSize = dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	//1番SRV
	textureData.gpuDescHandleSRV = gpuDescHandleStart;
	textureData.gpuDescHandleSRV.ptr += descHandleIncrementSize * m_texNum;

	//シェーダリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};//設定構造体
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//２Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;

	//ヒープの２番目にシェーダーリソースビューを作成
	dev->CreateShaderResourceView(textureData.texbuff.Get(),//ビューと関連付けるバッファ
		&srvDesc,//テクスチャ設定構造
		textureData.cpuDescHandleSRV
	);
	m_texNum++;

	if (m_texNum == 1)
	{//白画像保存
		m_white = textureData;
	}
	textureData.s_texNum = m_texNum - 1;

	return textureData;
}

TextureData Texture::OBJLoadTexture(const std::string& directoryPath, const std::string& filename)
{
	HRESULT result = S_FALSE;
	TextureData textureData = {};
	if (m_texNum == 0)
	{
		//設定構造体
		D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc{};
		descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//シェーダーから見える
		descHeapDesc.NumDescriptors = textureMax;//定数バッファの数
		//デスクリプタヒープの生成
		result = dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&m_descHeap));
	}
	// WICテクスチャのロード
	TexMetadata metadata{};
	ScratchImage scratchImg{};

	//ファイルパスを結合
	std::string filepath = directoryPath + filename;

	//ユニコード文字列に変換する
	wchar_t wfilepath[128];
	int iBufferSize = MultiByteToWideChar(CP_ACP, 0, filepath.c_str(), -1, wfilepath, _countof(wfilepath));

	result = LoadFromWICFile(
		wfilepath, WIC_FLAGS_NONE,
		&metadata, scratchImg
	);

	const Image* img = scratchImg.GetImage(0, 0, 0); // 生データ抽出

	// リソース設定
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format,
		metadata.width,
		(UINT)metadata.height,
		(UINT16)metadata.arraySize,
		(UINT16)metadata.mipLevels
	);
	// テクスチャ用バッファの生成
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, // テクスチャ用指定
		nullptr,
		IID_PPV_ARGS(&textureData.texbuff));

	// テクスチャバッファにデータ転送
	result = textureData.texbuff->WriteToSubresource(
		0,
		nullptr, // 全領域へコピー
		img->pixels,    // 元データアドレス
		(UINT)img->rowPitch,  // 1ラインサイズ
		(UINT)img->slicePitch // 1枚サイズ
	);

	UINT descHandleIncrementSize = dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	// シェーダリソースビュー作成
	textureData.cpuDescHandleSRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_descHeap->GetCPUDescriptorHandleForHeapStart(), 0, descHandleIncrementSize);
	//ハンドルのアドレスを進める
	textureData.cpuDescHandleSRV.ptr += m_texNum * descHandleIncrementSize;
	textureData.gpuDescHandleSRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(m_descHeap->GetGPUDescriptorHandleForHeapStart(), 0, descHandleIncrementSize);
	textureData.gpuDescHandleSRV.ptr += descHandleIncrementSize * m_texNum;

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // 設定構造体
	D3D12_RESOURCE_DESC resDesc = textureData.texbuff->GetDesc();

	srvDesc.Format = resDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;

	dev->CreateShaderResourceView(textureData.texbuff.Get(), //ビューと関連付けるバッファ
		&srvDesc, //テクスチャ設定情報
		textureData.cpuDescHandleSRV
	);
	m_texNum++;

	textureData.s_texNum = m_texNum - 1;
	return textureData;
}

TextureData  Texture::FbxLoadTexture(const DirectX::Image* img, CD3DX12_RESOURCE_DESC texresDesc)
{
	HRESULT result = S_FALSE;
	TextureData textureData = {};
	// テクスチャ用バッファの生成
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, // テクスチャ用指定
		nullptr,
		IID_PPV_ARGS(&textureData.texbuff));

	// テクスチャバッファにデータ転送
	result = textureData.texbuff->WriteToSubresource(
		0,
		nullptr, // 全領域へコピー
		img->pixels,    // 元データアドレス
		(UINT)img->rowPitch,  // 1ラインサイズ
		(UINT)img->slicePitch // 1枚サイズ
	);

	UINT descHandleIncrementSize = dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	// シェーダリソースビュー作成
	textureData.cpuDescHandleSRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_descHeap->GetCPUDescriptorHandleForHeapStart(), 0, descHandleIncrementSize);
	//ハンドルのアドレスを進める
	textureData.cpuDescHandleSRV.ptr += m_texNum * descHandleIncrementSize;
	textureData.gpuDescHandleSRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(m_descHeap->GetGPUDescriptorHandleForHeapStart(), 0, descHandleIncrementSize);
	textureData.gpuDescHandleSRV.ptr += descHandleIncrementSize * m_texNum;

	// シェーダリソースビュー(SRV)作成
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // 設定構造体
	D3D12_RESOURCE_DESC resDesc = textureData.texbuff->GetDesc();

	srvDesc.Format = resDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;

	dev->CreateShaderResourceView(textureData.texbuff.Get(), //ビューと関連付けるバッファ
		&srvDesc, //テクスチャ設定情報
		textureData.cpuDescHandleSRV // ヒープの先頭アドレス
	);
	m_texNum++;
	textureData.s_texNum = m_texNum - 1;
	return textureData;
}

void Texture::LoadShadowTexture(ID3D12Resource* texbuff)
{
	TextureData textureData = {};
	textureData.texbuff = texbuff;
	UINT descHandleIncrementSize = dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	// シェーダリソースビュー作成
	textureData.cpuDescHandleSRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_descHeap->GetCPUDescriptorHandleForHeapStart(), 0, descHandleIncrementSize);
	//ハンドルのアドレスを進める
	textureData.cpuDescHandleSRV.ptr += m_texNum * descHandleIncrementSize;
	textureData.gpuDescHandleSRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(m_descHeap->GetGPUDescriptorHandleForHeapStart(), 0, descHandleIncrementSize);
	textureData.gpuDescHandleSRV.ptr += descHandleIncrementSize * m_texNum;

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // 設定構造体
	srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;

	dev->CreateShaderResourceView(textureData.texbuff.Get(), //ビューと関連付けるバッファ
		&srvDesc, //テクスチャ設定情報
		textureData.cpuDescHandleSRV
	);
	textureData.s_texNum = m_texNum;
	m_shadowData = textureData;

	m_texNum++;
}

void Texture::LoadCameraTexture(ID3D12Resource* texbuff)
{
	TextureData textureData = {};

	textureData.texbuff = texbuff;
	UINT descHandleIncrementSize = dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	// シェーダリソースビュー作成
	textureData.cpuDescHandleSRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_descHeap->GetCPUDescriptorHandleForHeapStart(), 0, descHandleIncrementSize);
	//ハンドルのアドレスを進める
	textureData.cpuDescHandleSRV.ptr += m_texNum * descHandleIncrementSize;
	textureData.gpuDescHandleSRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(m_descHeap->GetGPUDescriptorHandleForHeapStart(), 0, descHandleIncrementSize);
	textureData.gpuDescHandleSRV.ptr += descHandleIncrementSize * m_texNum;

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // 設定構造体
	srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;

	dev->CreateShaderResourceView(textureData.texbuff.Get(), //ビューと関連付けるバッファ
		&srvDesc, //テクスチャ設定情報
		textureData.cpuDescHandleSRV
	);
	textureData.s_texNum = m_texNum;
	m_cameraDepth = textureData;

	m_texNum++;
}

void Texture::LoadPostEfectTexture(ID3D12Resource* texbuff)
{
	TextureData textureData = {};
	textureData.texbuff = texbuff;
	UINT descHandleIncrementSize = dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	// シェーダリソースビュー作成
	textureData.cpuDescHandleSRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_descHeap->GetCPUDescriptorHandleForHeapStart(), 0, descHandleIncrementSize);
	textureData.cpuDescHandleSRV.ptr += m_texNum * descHandleIncrementSize;
	textureData.gpuDescHandleSRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(m_descHeap->GetGPUDescriptorHandleForHeapStart(), 0, descHandleIncrementSize);
	textureData.gpuDescHandleSRV.ptr += descHandleIncrementSize * m_texNum;

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};//設定構造体
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
	srvDesc.Texture2D.MipLevels = 1;

	dev->CreateShaderResourceView(textureData.texbuff.Get(), //ビューと関連付けるバッファ
		&srvDesc, //テクスチャ設定情報
		textureData.cpuDescHandleSRV
	);

	textureData.s_texNum = m_texNum;
	m_postEfect = textureData;
	m_texNum++;
}

ID3D12DescriptorHeap* Texture::GetDescHeap()
{
	return m_descHeap.Get();
}

