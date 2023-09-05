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
		//�ݒ�\����
		D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc{};
		descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//�V�F�[�_�[���猩����
		descHeapDesc.NumDescriptors = textureMax;//�萔�o�b�t�@�̐�
		//�f�X�N���v�^�q�[�v�̐���
		result = dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&m_descHeap));
	}


	TexMetadata metadata{};
	ScratchImage scratchImg{};
	std::string i;
	//WIC�e�N�X�`���̃��[�h
	result = LoadFromWICFile(
		filename,
		WIC_FLAGS_NONE,
		&metadata, scratchImg);

	const Image* img = scratchImg.GetImage(0, 0, 0);//���f�[�^���o

	//���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format,
		metadata.width,
		(UINT)metadata.height,
		(UINT16)metadata.arraySize,
		(UINT16)metadata.mipLevels);

	//�e�N�X�`���p�o�b�t�@�̐���
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,//�e�N�X�`���p�w��
		nullptr,
		IID_PPV_ARGS(&textureData.texbuff));
#ifdef _DEBUG
	textureData.texbuff->SetName(L"Texture");
#endif
	//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
	result = textureData.texbuff->WriteToSubresource(
		0,
		nullptr,//�S�̈�փR�s�[
		img->pixels,//���f�[�^�A�h���X
		(UINT)img->rowPitch,//1���C���T�C�Y
		(UINT)img->slicePitch//�S�T�C�Y
	);

	//�f�X�N���v�^�q�[�v�̐擪�n���h�����擾
	textureData.cpuDescHandleSRV = m_descHeap->GetCPUDescriptorHandleForHeapStart();
	//�n���h���̃A�h���X��i�߂�
	textureData.cpuDescHandleSRV.ptr += m_texNum * dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	D3D12_GPU_DESCRIPTOR_HANDLE gpuDescHandleStart = m_descHeap->GetGPUDescriptorHandleForHeapStart();
	UINT descHandleIncrementSize = dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	//1��SRV
	textureData.gpuDescHandleSRV = gpuDescHandleStart;
	textureData.gpuDescHandleSRV.ptr += descHandleIncrementSize * m_texNum;

	//�V�F�[�_���\�[�X�r���[�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};//�ݒ�\����
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//�QD�e�N�X�`��
	srvDesc.Texture2D.MipLevels = 1;

	//�q�[�v�̂Q�ԖڂɃV�F�[�_�[���\�[�X�r���[���쐬
	dev->CreateShaderResourceView(textureData.texbuff.Get(),//�r���[�Ɗ֘A�t����o�b�t�@
		&srvDesc,//�e�N�X�`���ݒ�\��
		textureData.cpuDescHandleSRV
	);
	m_texNum++;

	if (m_texNum == 1)
	{//���摜�ۑ�
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
		//�ݒ�\����
		D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc{};
		descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//�V�F�[�_�[���猩����
		descHeapDesc.NumDescriptors = textureMax;//�萔�o�b�t�@�̐�
		//�f�X�N���v�^�q�[�v�̐���
		result = dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&m_descHeap));
	}
	// WIC�e�N�X�`���̃��[�h
	TexMetadata metadata{};
	ScratchImage scratchImg{};

	//�t�@�C���p�X������
	std::string filepath = directoryPath + filename;

	//���j�R�[�h������ɕϊ�����
	wchar_t wfilepath[128];
	int iBufferSize = MultiByteToWideChar(CP_ACP, 0, filepath.c_str(), -1, wfilepath, _countof(wfilepath));

	result = LoadFromWICFile(
		wfilepath, WIC_FLAGS_NONE,
		&metadata, scratchImg
	);

	const Image* img = scratchImg.GetImage(0, 0, 0); // ���f�[�^���o

	// ���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format,
		metadata.width,
		(UINT)metadata.height,
		(UINT16)metadata.arraySize,
		(UINT16)metadata.mipLevels
	);
	// �e�N�X�`���p�o�b�t�@�̐���
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, // �e�N�X�`���p�w��
		nullptr,
		IID_PPV_ARGS(&textureData.texbuff));

	// �e�N�X�`���o�b�t�@�Ƀf�[�^�]��
	result = textureData.texbuff->WriteToSubresource(
		0,
		nullptr, // �S�̈�փR�s�[
		img->pixels,    // ���f�[�^�A�h���X
		(UINT)img->rowPitch,  // 1���C���T�C�Y
		(UINT)img->slicePitch // 1���T�C�Y
	);

	UINT descHandleIncrementSize = dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	// �V�F�[�_���\�[�X�r���[�쐬
	textureData.cpuDescHandleSRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_descHeap->GetCPUDescriptorHandleForHeapStart(), 0, descHandleIncrementSize);
	//�n���h���̃A�h���X��i�߂�
	textureData.cpuDescHandleSRV.ptr += m_texNum * descHandleIncrementSize;
	textureData.gpuDescHandleSRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(m_descHeap->GetGPUDescriptorHandleForHeapStart(), 0, descHandleIncrementSize);
	textureData.gpuDescHandleSRV.ptr += descHandleIncrementSize * m_texNum;

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // �ݒ�\����
	D3D12_RESOURCE_DESC resDesc = textureData.texbuff->GetDesc();

	srvDesc.Format = resDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = 1;

	dev->CreateShaderResourceView(textureData.texbuff.Get(), //�r���[�Ɗ֘A�t����o�b�t�@
		&srvDesc, //�e�N�X�`���ݒ���
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
	// �e�N�X�`���p�o�b�t�@�̐���
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, // �e�N�X�`���p�w��
		nullptr,
		IID_PPV_ARGS(&textureData.texbuff));

	// �e�N�X�`���o�b�t�@�Ƀf�[�^�]��
	result = textureData.texbuff->WriteToSubresource(
		0,
		nullptr, // �S�̈�փR�s�[
		img->pixels,    // ���f�[�^�A�h���X
		(UINT)img->rowPitch,  // 1���C���T�C�Y
		(UINT)img->slicePitch // 1���T�C�Y
	);

	UINT descHandleIncrementSize = dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	// �V�F�[�_���\�[�X�r���[�쐬
	textureData.cpuDescHandleSRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_descHeap->GetCPUDescriptorHandleForHeapStart(), 0, descHandleIncrementSize);
	//�n���h���̃A�h���X��i�߂�
	textureData.cpuDescHandleSRV.ptr += m_texNum * descHandleIncrementSize;
	textureData.gpuDescHandleSRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(m_descHeap->GetGPUDescriptorHandleForHeapStart(), 0, descHandleIncrementSize);
	textureData.gpuDescHandleSRV.ptr += descHandleIncrementSize * m_texNum;

	// �V�F�[�_���\�[�X�r���[(SRV)�쐬
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // �ݒ�\����
	D3D12_RESOURCE_DESC resDesc = textureData.texbuff->GetDesc();

	srvDesc.Format = resDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = 1;

	dev->CreateShaderResourceView(textureData.texbuff.Get(), //�r���[�Ɗ֘A�t����o�b�t�@
		&srvDesc, //�e�N�X�`���ݒ���
		textureData.cpuDescHandleSRV // �q�[�v�̐擪�A�h���X
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
	// �V�F�[�_���\�[�X�r���[�쐬
	textureData.cpuDescHandleSRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_descHeap->GetCPUDescriptorHandleForHeapStart(), 0, descHandleIncrementSize);
	//�n���h���̃A�h���X��i�߂�
	textureData.cpuDescHandleSRV.ptr += m_texNum * descHandleIncrementSize;
	textureData.gpuDescHandleSRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(m_descHeap->GetGPUDescriptorHandleForHeapStart(), 0, descHandleIncrementSize);
	textureData.gpuDescHandleSRV.ptr += descHandleIncrementSize * m_texNum;

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // �ݒ�\����
	srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = 1;

	dev->CreateShaderResourceView(textureData.texbuff.Get(), //�r���[�Ɗ֘A�t����o�b�t�@
		&srvDesc, //�e�N�X�`���ݒ���
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
	// �V�F�[�_���\�[�X�r���[�쐬
	textureData.cpuDescHandleSRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_descHeap->GetCPUDescriptorHandleForHeapStart(), 0, descHandleIncrementSize);
	//�n���h���̃A�h���X��i�߂�
	textureData.cpuDescHandleSRV.ptr += m_texNum * descHandleIncrementSize;
	textureData.gpuDescHandleSRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(m_descHeap->GetGPUDescriptorHandleForHeapStart(), 0, descHandleIncrementSize);
	textureData.gpuDescHandleSRV.ptr += descHandleIncrementSize * m_texNum;

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{}; // �ݒ�\����
	srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = 1;

	dev->CreateShaderResourceView(textureData.texbuff.Get(), //�r���[�Ɗ֘A�t����o�b�t�@
		&srvDesc, //�e�N�X�`���ݒ���
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
	// �V�F�[�_���\�[�X�r���[�쐬
	textureData.cpuDescHandleSRV = CD3DX12_CPU_DESCRIPTOR_HANDLE(m_descHeap->GetCPUDescriptorHandleForHeapStart(), 0, descHandleIncrementSize);
	textureData.cpuDescHandleSRV.ptr += m_texNum * descHandleIncrementSize;
	textureData.gpuDescHandleSRV = CD3DX12_GPU_DESCRIPTOR_HANDLE(m_descHeap->GetGPUDescriptorHandleForHeapStart(), 0, descHandleIncrementSize);
	textureData.gpuDescHandleSRV.ptr += descHandleIncrementSize * m_texNum;

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};//�ݒ�\����
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = 1;

	dev->CreateShaderResourceView(textureData.texbuff.Get(), //�r���[�Ɗ֘A�t����o�b�t�@
		&srvDesc, //�e�N�X�`���ݒ���
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

