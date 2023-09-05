#include "PostEffect.h"
#include"ShaderManager.h"
using namespace DirectX;
extern const int window_width;
extern const int window_height;

const float PostEffect::clearColor[4] = { 0.2f,0.2f,0.4f,0.0f };//�΂��ۂ��F
ID3D12Device* PostEffect::dev = nullptr;
PostEffect::PostEffect()
{}

void PostEffect::Initialize(ID3D12Device* dev)
{
	HRESULT result;

	this->dev = dev;
	assert(dev);

	CreatePipeline(dev);

	//���_�f�[�^
	Sprite::VertexPosUv vertices[] = {
		{{-1.0f,-1.0f,0.0f},{0.0f,1.0f}},
		{{-1.0f,+1.0f,0.0f},{0.0f,0.0f}},
		{{+1.0f,-1.0f,0.0f},{1.0f,1.0f}},
		{{+1.0f,+1.0f,0.0f},{1.0f,0.0f}},
	};

	//���_�o�b�t�@����
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
	//���_�o�b�t�@�ւ̃f�[�^�]��
	Sprite::VertexPosUv* vertMap = nullptr;
	result = m_sprite.vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		memcpy(vertMap, vertices, sizeof(vertices));
		m_sprite.vertBuff->Unmap(0, nullptr);
	}

	//���_�o�b�t�@�r���[�̍쐬
	m_sprite.vbView.BufferLocation = m_sprite.vertBuff->GetGPUVirtualAddress();
	m_sprite.vbView.SizeInBytes = sizeof(Sprite::VertexPosUv) * 4;
	m_sprite.vbView.StrideInBytes = sizeof(Sprite::VertexPosUv);

	//�萔�o�b�t�@
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

	//�e�N�X�`�����\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		window_width,
		window_height,
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
	);

	//�e�N�X�`���o�b�t�@�̐���

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

	{//�e�N�X�`����ԃN���A
		//��f��
		const UINT pixelCount = window_width * window_height;
		//�摜1�s���̃f�[�^�T�C�Y
		const UINT rowPitch = sizeof(UINT) * window_width;
		//�摜�S�̂̃f�[�^�T�C�Y
		const UINT depthPitch = rowPitch * window_height;
		//�摜�C���[�W
		UINT* img = new UINT[pixelCount];
		for (int i = 0; unsigned(i) < pixelCount; i++) { img[i] = 0xff0000ff; }

		//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
		result = m_texbuff->WriteToSubresource(0, nullptr,
			img, rowPitch, depthPitch);
		assert(SUCCEEDED(result));
		delete[] img;
	}

	//SRV�p�f�X�N���v�^�q�[�v�ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc = {};
	srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvDescHeapDesc.NumDescriptors = 2;
	//SRV�p�f�X�N���v�^�q�[�v�𐶐�
	result = dev->CreateDescriptorHeap(&srvDescHeapDesc, IID_PPV_ARGS(&m_descHeapSRV));
	assert(SUCCEEDED(result));
#ifdef _DEBUG
	m_descHeapSRV->SetName(L"SpriteSRV");
#endif

	Texture::Get()->LoadPostEfectTexture(m_texbuff.Get());

#ifdef _DEBUG
	m_texbuff->SetName(L"SpriteBuff");
#endif

	//RTV�p�f�X�N���v�^�q�[�v�ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescHeapDesc{};
	rtvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescHeapDesc.NumDescriptors = 1;
	//RTV�p�f�X�N���v�^�q�[�v�𐶐�
	result = dev->CreateDescriptorHeap(&rtvDescHeapDesc, IID_PPV_ARGS(&m_descHeapRTV));
	assert(SUCCEEDED(result));
#ifdef _DEBUG
	m_descHeapRTV->SetName(L"SpriteRTV");
#endif
	//�f�X�N���v�^�q�[�v��RTV�쐬
	dev->CreateRenderTargetView(m_texbuff.Get(),
		nullptr,
		m_descHeapRTV->GetCPUDescriptorHandleForHeapStart());

	//�[�x�o�b�t�@���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC depthResDesc =
		CD3DX12_RESOURCE_DESC::Tex2D(
			DXGI_FORMAT_R32_TYPELESS,
			window_width,
			window_height,
			1, 0,
			1, 0,
			D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);

	//�[�x�o�b�t�@�̐���
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
	//DSV�p�f�X�N���v�^�q�[�v
	D3D12_DESCRIPTOR_HEAP_DESC DescHeapDesc{};
	DescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	DescHeapDesc.NumDescriptors = 1;
	//DSV�p�f�X�N���v�^�q�[�v���쐬
	result = dev->CreateDescriptorHeap(&DescHeapDesc, IID_PPV_ARGS(&m_descHeapDSV));
	assert(SUCCEEDED(result));

	//�f�X�N���v�^�q�[�v��DSV�쐬
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;//�[�x�l�t�H�[�}�b�g
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
	//���\�[�X�o���A��ύX
	cmdList->ResourceBarrier(1,
		&CD3DX12_RESOURCE_BARRIER::Transition(m_texbuff.Get(),
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
			D3D12_RESOURCE_STATE_RENDER_TARGET));

	//�����_�[�^�[�Q�b�g�r���[�p�f�B�X�N���v�^�q�[�v�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE rtvH =
		m_descHeapRTV->GetCPUDescriptorHandleForHeapStart();
	//�[�x�X�e���V���r���[�p�f�X�N���v�^�q�[�v�̃n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE dsvH =
		m_descHeapDSV->GetCPUDescriptorHandleForHeapStart();
	//�����_�[�^�[�Q�b�g���Z�b�g
	cmdList->OMSetRenderTargets(1, &rtvH, false, &dsvH);

	//�r���[�|�[�g�̐ݒ�
	cmdList->RSSetViewports(1, &CD3DX12_VIEWPORT(0.0f, 0.0f, static_cast<FLOAT>(window_width), static_cast<FLOAT>(window_height)));
	//�V�U�����O�Z�`�̐ݒ�
	cmdList->RSSetScissorRects(1, &CD3DX12_RECT(0, 0, static_cast<LONG>(window_width), static_cast<LONG>(window_height)));

	//�S��ʃN���A
	cmdList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);
	//�[�x�o�b�t�@�̃N���A
	cmdList->ClearDepthStencilView(dsvH, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

}
void PostEffect::PostDrawScene(ID3D12GraphicsCommandList* cmdList)
{
	//���\�[�X�o���A��ύX
	cmdList->ResourceBarrier(1,
		&CD3DX12_RESOURCE_BARRIER::Transition(m_texbuff.Get(),
			D3D12_RESOURCE_STATE_RENDER_TARGET,
			D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));
}

void PostEffect::UpdatePost(SpriteData& sprite, const Vec2 &position, float width, float height, const Vec2 &anchorpoint, const Vec4 &color, bool isFlipX, bool isFlipY)
{
	//�萔�o�b�t�@�̓]��
	Sprite::ConstBufferData* constMap = nullptr;
	HRESULT result = m_constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->mat = XMMatrixIdentity();
	constMap->color = color;
	m_constBuff->Unmap(0, nullptr);
}

void PostEffect::DrawPost(SpriteData& sprite, const Vec2 &position, float width, float height, const Vec2 &anchorpoint, const Vec4 &color, bool isFlipX, bool isFlipY)
{
	//�p�C�v���C���X�e�[�g�̐ݒ�
	Sprite::cmdList->SetPipelineState(m_pipelineSet.pipelinestate.Get());
	//���[�g�V�O�l�`���̐ݒ�
	Sprite::cmdList->SetGraphicsRootSignature(m_pipelineSet.rootsignature.Get());
	//�v���~�e�B�u�`���ݒ�
	Sprite::cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	UpdatePost(sprite, position, width, height, anchorpoint, color, isFlipX, isFlipY);
	//���_�o�b�t�@���Z�b�g
	Sprite::cmdList->IASetVertexBuffers(0, 1, &sprite.vbView);
	//�萔�o�b�t�@���Z�b�g
	Sprite::cmdList->SetGraphicsRootConstantBufferView(0, m_constBuff->GetGPUVirtualAddress());


	//�e�N�X�`���p�f�X�N���v�^�q�[�v�̐ݒ�
	ID3D12DescriptorHeap* ppHeaps[] = { Texture::Get()->GetDescHeap()};
	Sprite::cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	//�V�F�[�_�[���\�[�X�r���[���Z�b�g
	
	Sprite::cmdList->SetGraphicsRootDescriptorTable(1, Texture::Get()->GetPostEfect().gpuDescHandleSRV);

	//�q�[�v�̂Q�Ԗڂɂ���SRV�����[�g�p�����[�^�P�Ԃɐݒ�
	Sprite::cmdList->SetGraphicsRootDescriptorTable(2, Texture::Get()->GetCameraDepth().gpuDescHandleSRV);


	//�|���S���̕`��i�S���_�Ŏl�p�`�j
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


