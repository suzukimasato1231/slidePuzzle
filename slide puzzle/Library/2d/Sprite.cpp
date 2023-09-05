#include "Sprite.h"
#include"ShaderManager.h"
#include<Texture.h>
ID3D12Device* Sprite::dev = nullptr;
ID3D12GraphicsCommandList* Sprite::cmdList = nullptr;

Sprite::Sprite()
{}
Sprite::~Sprite()
{
	for (int i = (int)constBuffer.size() - 1; i >= 0; i--)
	{
		delete constBuffer[i];
		constBuffer.erase(constBuffer.begin() + i);
	}
}

void Sprite::StaticInit(ID3D12Device* dev, ID3D12GraphicsCommandList* cmdList)
{
	Sprite::dev = dev;
	Sprite::cmdList = cmdList;
}

Sprite* Sprite::Create()
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	Sprite* sprite = new Sprite();
	if (sprite == nullptr) {
		return nullptr;
	}
	sprite->Init();
	// ������
	return sprite;
}

//������
void Sprite::SpriteAllInitialization(int widow_width, int window_height)
{
	//�X�v���C�g���ʃf�[�^
	spriteCommon = Sprite::SpriteCommonCreate(widow_width, window_height);
}


//�������牺�͕ς��Ȃ�
void Sprite::Init()
{
	SpriteAllInitialization(window_width, window_height);
}

SpriteData Sprite::SpriteCreate(const wchar_t* filename)
{
	HRESULT result = S_FALSE;

	//�V�����X�v���C�g�����
	SpriteData sprite{};

	//�e�N�X�`���ԍ����R�s�[
	sprite.texNumber = Texture::Get()->LoadTexture(filename);

	//���_�f�[�^
	VertexPosUv vertices[] =
	{
		{{0.0f,100.0f,0.0f},{0.0f,1.0f}},//����
		{{0.0f,0.0f,0.0f},{0.0f,0.0f}},//�E��
		{{100.0f,100.0f,0.0f},{1.0f,1.0f}},//�E��
		{{100.0f,0.0f,0.0f},{1.0f,0.0f}},//�E��
	};

	//���_�o�b�t�@����
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices)),
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&sprite.vertBuff));

	//���_�o�b�t�@�ւ̃f�[�^�]��
	VertexPosUv* vertMap = nullptr;
	result = sprite.vertBuff->Map(0, nullptr, (void**)&vertMap);
	memcpy(vertMap, vertices, sizeof(vertices));
	sprite.vertBuff->Unmap(0, nullptr);


	//���_�o�b�t�@�r���[�̍쐬
	sprite.vbView.BufferLocation = sprite.vertBuff->GetGPUVirtualAddress();
	sprite.vbView.SizeInBytes = sizeof(vertices);
	sprite.vbView.StrideInBytes = sizeof(vertices[0]);


	//�w��ԍ��̉摜���ǂݍ��ݍς݂Ȃ�
	if (sprite.texNumber.texbuff)
	{
		//�e�N�X�`�������擾
		D3D12_RESOURCE_DESC resDesc = sprite.texNumber.texbuff->GetDesc();
		//�X�v���C�g�̑傫�����摜�̉𑜓x�ɍ��킹��
		sprite.size = { (float)resDesc.Width,(float)resDesc.Height };
	}
	//���_�o�b�t�@�f�[�^�]��
	SpriteTransferVertexBuffer(sprite);


	constBuffer.push_back({ new ConstBuffer });
	//�萔�o�b�t�@�̐���
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuffer[constBuffer.size() - 1]->constBuff));

	//�萔�o�b�t�@�Ƀf�[�^�]��
	ConstBufferData* constMap = nullptr;
	result = constBuffer[constBuffer.size() - 1]->constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->color = Vec4(1, 1, 1, 1);//�F�w��
	//���s���e�s��
	constMap->mat = XMMatrixOrthographicOffCenterLH(
		0.0f, (float)window_width, (float)window_height, 0.0f, 0.0f, 1.0f);
	constBuffer[constBuffer.size() - 1]->constBuff->Unmap(0, nullptr);

	return sprite;
}

//�X�v���C�g�P�̒��_�o�b�t�@�̓]��
void Sprite::SpriteTransferVertexBuffer(const SpriteData& sprite)
{
	HRESULT result = S_FALSE;

	//���_�f�[�^
	VertexPosUv vertices[] = {
		{{},{0.0f,1.0f}},//����
		{{},{0.0f,0.0f}},//����
		{{},{1.0f,1.0f}},//�E��
		{{},{1.0f,0.0f}},//�E��
	};

	//�����@����@�E���@�E��
	enum { LB, LT, RB, RT };

	float left = (0.0f - sprite.anchorpoint.x) * sprite.size.x;
	float right = (1.0f - sprite.anchorpoint.x) * sprite.size.x;
	float top = (0.0f - sprite.anchorpoint.y) * sprite.size.y;
	float bottom = (1.0f - sprite.anchorpoint.y) * sprite.size.y;

	vertices[LB].pos = { left,bottom,0.0f };//����
	vertices[LT].pos = { left,top,0.0f };//����
	vertices[RB].pos = { right,bottom,0.0f };//�E��
	vertices[RT].pos = { right,top,0.0f };//�E��

	//uv�v�Z
	//�w��ԍ��̉摜���ǂݍ��ݍς݂Ȃ�
	if (sprite.texNumber.texbuff && !sprite.texSize.x == 0 && !sprite.texSize.y == 0)
	{
		//�e�N�X�`�����擾
		D3D12_RESOURCE_DESC resDesc = sprite.texNumber.texbuff->GetDesc();

		float tex_left = sprite.texLeftTop.x / resDesc.Width;
		float tex_right = (sprite.texLeftTop.x + sprite.texSize.x) / resDesc.Width;
		float tex_top = sprite.texLeftTop.y / resDesc.Height;
		float texbottom = (sprite.texLeftTop.y + sprite.texSize.y) / resDesc.Height;

		vertices[LB].uv = { tex_left,texbottom };
		vertices[LT].uv = { tex_left,tex_top };
		vertices[RB].uv = { tex_right,texbottom };
		vertices[RT].uv = { tex_right,tex_top };

	}
	//���_�o�b�t�@�ւ̃f�[�^�]��
	VertexPosUv* vertMap = nullptr;
	result = sprite.vertBuff->Map(0, nullptr, (void**)&vertMap);
	memcpy(vertMap, vertices, sizeof(vertices));
	sprite.vertBuff->Unmap(0, nullptr);
}

void Sprite::PreDraw()
{
	spriteNum = 0;
}

void Sprite::CreateConstBuffer()
{
	HRESULT result = S_FALSE;
	constBuffer.push_back({ new ConstBuffer });
	//�萔�o�b�t�@�̐���
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuffer[constBuffer.size() - 1]->constBuff));

#ifdef _DEBUG
	constBuffer[constBuffer.size() - 1]->constBuff->SetName(L"Sprite");
#endif
}

//���ʃf�[�^�𐶐�����֐����쐬
Sprite::SpriteCommon Sprite::SpriteCommonCreate(int window_width, int window_height)
{
	HRESULT result = S_FALSE;

	//�V���ȃX�v���C�g���ʃf�[�^�𐶐�
	SpriteCommon spriteCommon{};

	//�X�v���C�g�p�p�C�v���C������
	spriteCommon.pipelineSet = Pipeline::SpriteCreateGraphicsPipeline(dev, ShaderManager::spriteShader);

	//���s���e�̎ˉe�s�񐶐�
	spriteCommon.matProjection = XMMatrixOrthographicOffCenterLH(
		0.0f, (float)window_width, (float)window_height, 0.0f, 0.0f, 1.0f);

	//���������X�v���C�g���ʃf�[�^��Ԃ�
	return spriteCommon;
}

//�X�v���C�g���ʃO���t�B�b�N�R�}���h�̃Z�b�g
void Sprite::SpriteCommonBeginDraw()
{
	//�p�C�v���C���X�e�[�g�̐ݒ�
	cmdList->SetPipelineState(spriteCommon.pipelineSet.pipelinestate.Get());
	//���[�g�V�O�l�`���̐ݒ�
	cmdList->SetGraphicsRootSignature(spriteCommon.pipelineSet.rootsignature.Get());
	//�v���~�e�B�u�`���ݒ�
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//�e�N�X�`���p�f�X�N���v�^�q�[�v�̐ݒ�
	ID3D12DescriptorHeap* ppHeaps[] = { Texture::Get()->GetDescHeap() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
}
//�X�v���C�g�P�̍X�V
void Sprite::Update(SpriteData& sprite, const Vec2 &position, float width, float height, const Vec2 &anchorpoint, const Vec4 &color, bool isFlipX, bool isFlipY)
{

	if (sprite.size.x != width || sprite.size.y != height ||
		sprite.anchorpoint.x != anchorpoint.x || sprite.anchorpoint.y != anchorpoint.y ||
		isFlipX == true || isFlipY == true)
	{
		//�T�C�Y�����킹��
		sprite.size.x = width, sprite.size.y = height;
		sprite.anchorpoint = anchorpoint;
		//���_�f�[�^
		VertexPosUv vertices[] = {
				{{},{0.0f,1.0f}},//����
				{{},{0.0f,0.0f}},//����
				{{},{1.0f,1.0f}},//�E��
				{{},{1.0f,0.0f}},//�E��
		};
		//�����@����@�E���@�E��
		enum { LB, LT, RB, RT };
		float left = (0.0f - sprite.anchorpoint.x) * sprite.size.x;
		float right = (1.0f - sprite.anchorpoint.x) * sprite.size.x;
		float top = (0.0f - sprite.anchorpoint.y) * sprite.size.y;
		float bottom = (1.0f - sprite.anchorpoint.y) * sprite.size.y;

		if (isFlipX)
		{
			left = -left;
			right = -right;
		}

		if (isFlipY)
		{
			top = -top;
			bottom = -bottom;
		}

		vertices[LB].pos = { left,bottom,0.0f };//����
		vertices[LT].pos = { left,top,0.0f };//����
		vertices[RB].pos = { right,bottom,0.0f };//�E��
		vertices[RT].pos = { right,top,0.0f };//�E��
		//���_�o�b�t�@�ւ̃f�[�^�]��
		VertexPosUv* vertMap = nullptr;
		sprite.vertBuff->Map(0, nullptr, (void**)&vertMap);
		memcpy(vertMap, vertices, sizeof(vertices));
		sprite.vertBuff->Unmap(0, nullptr);
	}

	//���[���h�s��̍X�V
	sprite.matWorld = XMMatrixIdentity();
	//Z����]
	sprite.matWorld *= XMMatrixRotationZ(XMConvertToRadians(sprite.rotation));
	//���s�ړ�
	sprite.matWorld *= XMMatrixTranslation(position.x, position.y, 0);


	if (sprite.parent == nullptr)
	{
		//�萔�o�b�t�@�̓]��
		ConstBufferData* constMap = nullptr;
		HRESULT result = constBuffer[spriteNum]->constBuff->Map(0, nullptr, (void**)&constMap);
		constMap->mat = sprite.matWorld * spriteCommon.matProjection;
		constMap->color = color;
		constBuffer[spriteNum]->constBuff->Unmap(0, nullptr);
	}
	else
	{
		//�萔�o�b�t�@�̓]��
		ConstBufferData* constMap = nullptr;
		HRESULT result = constBuffer[spriteNum]->constBuff->Map(0, nullptr, (void**)&constMap);
		constMap->mat = sprite.matWorld * sprite.parent->matWorld * spriteCommon.matProjection;
		constMap->color = color;
		constBuffer[spriteNum]->constBuff->Unmap(0, nullptr);
	}

}

//�X�v���C�g�P�̕`��
void Sprite::Draw(SpriteData& sprite, const Vec2 &position, const float width, const float height, const Vec2 &anchorpoint, const Vec4 &color, const bool isFlipX, const bool isFlipY)
{
	SpriteCommonBeginDraw();

	if (constBuffer.size() <= spriteNum)
	{
		CreateConstBuffer();
	}

	Update(sprite, position, width, height, anchorpoint, color, isFlipX, isFlipY);
	//���_�o�b�t�@���Z�b�g
	cmdList->IASetVertexBuffers(0, 1, &sprite.vbView);
	//�萔�o�b�t�@���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, constBuffer[spriteNum]->constBuff->GetGPUVirtualAddress());

	//�V�F�[�_�[���\�[�X�r���[���Z�b�g
	cmdList->SetGraphicsRootDescriptorTable(1,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(
			Texture::Get()->GetDescHeap()->GetGPUDescriptorHandleForHeapStart(),
			sprite.texNumber.s_texNum,
			dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));

	//�|���S���̕`��i�S���_�Ŏl�p�`�j
	cmdList->DrawInstanced(4, 1, 0, 0);

	spriteNum++;
}


void Sprite::DebugUpdate(SpriteData& sprite)
{
	//���[���h�s��̍X�V
	sprite.matWorld = XMMatrixIdentity();
	//Z����]
	sprite.matWorld *= XMMatrixRotationZ(XMConvertToRadians(sprite.rotation));
	//���s�ړ�
	sprite.matWorld *= XMMatrixTranslation(sprite.position.x, sprite.position.y, 0);

	//�萔�o�b�t�@�̓]��
	ConstBufferData* constMap = nullptr;
	HRESULT result = constBuffer[spriteNum]->constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->mat = sprite.matWorld * spriteCommon.matProjection;
	constMap->color = { 1.0f,1.0f,1.0f,1.0f };
	constBuffer[spriteNum]->constBuff->Unmap(0, nullptr);
}

//�X�v���C�g�P�̕`��
void Sprite::DebugDraw(SpriteData& sprite)
{
	SpriteCommonBeginDraw();

	if (constBuffer.size() <= spriteNum)
	{
		CreateConstBuffer();
	}

	DebugUpdate(sprite);

	//���_�o�b�t�@���Z�b�g
	cmdList->IASetVertexBuffers(0, 1, &sprite.vbView);
	//�萔�o�b�t�@���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, constBuffer[spriteNum]->constBuff->GetGPUVirtualAddress());

	//�V�F�[�_�[���\�[�X�r���[���Z�b�g
	cmdList->SetGraphicsRootDescriptorTable(1,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(
			Texture::Get()->GetDescHeap()->GetGPUDescriptorHandleForHeapStart(),
			sprite.texNumber.s_texNum,
			dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));

	//�|���S���̕`��i�S���_�Ŏl�p�`�j
	cmdList->DrawInstanced(4, 1, 0, 0);
	spriteNum++;
}

void Sprite::PostEffectDraw(ID3D12DescriptorHeap* descHeap, SpriteData& sprite, const Vec2 &position, float width, float height, const Vec2 &anchorpoint, const Vec4 &color, bool isFlipX, bool isFlipY)
{
	//�p�C�v���C���X�e�[�g�̐ݒ�
	cmdList->SetPipelineState(spriteCommon.pipelineSet.pipelinestate.Get());
	//���[�g�V�O�l�`���̐ݒ�
	cmdList->SetGraphicsRootSignature(spriteCommon.pipelineSet.rootsignature.Get());
	//�v���~�e�B�u�`���ݒ�
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//�e�N�X�`���p�f�X�N���v�^�q�[�v�̐ݒ�
	ID3D12DescriptorHeap* ppHeaps[] = { descHeap };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	if (constBuffer.size() <= spriteNum)
	{
		CreateConstBuffer();
	}

	Update(sprite, position, width, height, anchorpoint, color, isFlipX, isFlipY);
	//���_�o�b�t�@���Z�b�g
	cmdList->IASetVertexBuffers(0, 1, &sprite.vbView);
	//�萔�o�b�t�@���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, constBuffer[spriteNum]->constBuff->GetGPUVirtualAddress());

	cmdList->SetGraphicsRootDescriptorTable(1, descHeap->GetGPUDescriptorHandleForHeapStart());
	//�|���S���̕`��i�S���_�Ŏl�p�`�j
	cmdList->DrawInstanced(4, 1, 0, 0);

	spriteNum++;
}