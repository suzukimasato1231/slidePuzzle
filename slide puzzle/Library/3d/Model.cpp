#include "Model.h"
#include<Texture.h>
Model::~Model()
{

}


void Model::CreateBuffers(ID3D12Device* device)
{
	HRESULT result;
	// 頂点データ全体のサイズ
	UINT sizeVB = static_cast<UINT>(sizeof(VertexPosNormalUvSkin) * vertices.size());
	// 頂点バッファ生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeVB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_vertBuff));

	// 頂点バッファへのデータ転送
	VertexPosNormalUvSkin* vertMap = nullptr;
	result = m_vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		std::copy(vertices.begin(), vertices.end(), vertMap);
		m_vertBuff->Unmap(0, nullptr);
	}

	// 頂点バッファビュー(VBV)の作成
	m_vbView.BufferLocation = m_vertBuff->GetGPUVirtualAddress();
	m_vbView.SizeInBytes = sizeVB;
	m_vbView.StrideInBytes = sizeof(vertices[0]);
	// 頂点インデックス全体のサイズ
	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * indices.size());
	// インデックスバッファ生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeIB),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_indexBuff));

	// インデックスバッファへのデータ転送
	unsigned short* indexMap = nullptr;
	result = m_indexBuff->Map(0, nullptr, (void**)&indexMap);
	if (SUCCEEDED(result)) {
		std::copy(indices.begin(), indices.end(), indexMap);
		m_indexBuff->Unmap(0, nullptr);
	}

	// インデックスバッファビュー(IBV)の作成
	m_ibView.BufferLocation = m_indexBuff->GetGPUVirtualAddress();
	m_ibView.Format = DXGI_FORMAT_R16_UINT;
	m_ibView.SizeInBytes = sizeIB;

	// テクスチャ画像データ
	const DirectX::Image* img = m_scratchImg.GetImage(0, 0, 0); // 生データ抽出
	assert(img);

	// リソース設定
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		m_metadata.format,
		m_metadata.width,
		(UINT)m_metadata.height,
		(UINT16)m_metadata.arraySize,
		(UINT16)m_metadata.mipLevels
	);

	m_textureNum = Texture::Get()->FbxLoadTexture(img, texresDesc);
}

void Model::Draw(ID3D12GraphicsCommandList* cmdList, bool shadowFlag)
{
	// 頂点バッファをセット(VBV)
	cmdList->IASetVertexBuffers(0, 1, &m_vbView);
	// インデックスバッファをセット(IBV)
	cmdList->IASetIndexBuffer(&m_ibView);
	if (shadowFlag)
	{
		// デスクリプタヒープのセット
		ID3D12DescriptorHeap* ppHeaps[] = { Texture::Get()->GetDescHeap() };
		cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
		// シェーダリソースビューをセット
		cmdList->SetGraphicsRootDescriptorTable(1, m_textureNum.gpuDescHandleSRV);
	}
	// 描画コマンド
	cmdList->DrawIndexedInstanced((UINT)indices.size(), 1, 0, 0, 0);
}

