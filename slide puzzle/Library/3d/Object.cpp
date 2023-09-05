#include "Object.h"
using namespace DirectX;
using namespace Microsoft::WRL;
using namespace std;

ID3D12Device* Object::dev = nullptr;
ID3D12GraphicsCommandList* Object::cmdList = nullptr;
LightGroup* Object::lightGroup = nullptr;
size_t Object::OBJNum = 0;
std::vector<Object::OBJBuffer*> Object::OBJbuffer;


Object::~Object()
{}

void  Object::Init(ID3D12Device* dev, ID3D12GraphicsCommandList* cmdList)
{
	Object::dev = dev;
	Object::cmdList = cmdList;
}

Object* Object::Create()
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	Object* object = new Object();
	if (object == nullptr) {
		return nullptr;
	}

	// ������
	return object;
}

void Object::Delete()
{
	//OBJ�f�[�^�폜
	for (int i = (int)OBJbuffer.size() - 1; i >= 0; i--)
	{
		delete OBJbuffer[i];
		OBJbuffer.erase(OBJbuffer.begin() + i);
	}
}

void Object::MatWord(const ObjectData& polygon, const Vec3& position, const Vec3& scale, const Vec3& rotation, const Vec2& uv)
{
	HRESULT result;

	//���[���h�ϊ��F//�X�P�[�����O//��]�s��XMMATRIX//���s�ړ��s��
	XMMATRIX matScale, matRot, matTrains;
	//���[���h�ϊ�
	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);//�傫��
	matTrains = XMMatrixTranslation(position.x, position.y, position.z);//���s�ړ��s����Čv�Z

	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));//Z���܂��ɂS�T�x��]
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));//X���܂��ɂS�T�x��]
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));//Y���܂��ɂS�T�x��]
	//���[���h�ϊ�
	XMMATRIX matWorld;
	matWorld = XMMatrixIdentity();//���[���h�s��͖��t���[�����Z�b�g
	matWorld *= matScale;//���[���h�s��ɃX�P�[�����O�𔽉f
	matWorld *= matRot;//���[���h�s��ɉ�]�𔽉f
	matWorld *= matTrains;//���[���h�s��ɕύX�ړ��𔽉f


	const XMMATRIX& matViewProjection = Camera::Get()->GetMatViewProjection();
	const Vec3& cameraPos = Camera::Get()->GetEye();
	//GPU��̃o�b�t�@�ɑΉ��������z���������擾
	ConstBufferDataB0* constMap = nullptr;
	result = Object::OBJbuffer[OBJNum]->constBuffB0->Map(0, nullptr, (void**)&constMap);
	//�s��̍���   ���[���h�ϊ��s�� �� �r���[�ϊ��s�� �� �ˉe�ϊ��s��
	constMap->viewproj = matViewProjection;
	if (polygon.parent == nullptr)
	{
		constMap->world = matWorld;
	}
	else
	{
		constMap->world = matWorld * polygon.parent->matWorld;
	}
	constMap->cameraPos = cameraPos;
	constMap->uv = uv;
	constMap->lightproj = lightGroup->GetLightMatProjection();
	Object::OBJbuffer[OBJNum]->constBuffB0->Unmap(0, nullptr);


	//�萔�o�b�t�@�փf�[�^�]��
	ConstBufferDataB1* constMap1 = nullptr;
	result = Object::OBJbuffer[OBJNum]->constBuffB1->Map(0, nullptr, (void**)&constMap1);
	constMap1->ambient = polygon.material.ambient;
	constMap1->diffuse = polygon.material.diffuse;
	constMap1->specular = polygon.material.specular;
	constMap1->alpha = polygon.material.alpha;
	Object::OBJbuffer[OBJNum]->constBuffB1->Unmap(0, nullptr);

}

void Object::InitDraw()
{
	OBJNum = 0;
}

void Object::OBJConstantBuffer()
{
	HRESULT result;
	Object::OBJbuffer.push_back(new Object::OBJBuffer);
	// �萔�o�b�t�@�̐���
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// �A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB0) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&Object::OBJbuffer[Object::OBJbuffer.size() - 1]->constBuffB0));


	// �萔�o�b�t�@�̐���
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// �A�b�v���[�h�\
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB1) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&Object::OBJbuffer[Object::OBJbuffer.size() - 1]->constBuffB1));
	//�萔�o�b�t�@�փf�[�^�]��
	ConstBufferDataB1* constMap1 = nullptr;
	result = Object::OBJbuffer[OBJNum]->constBuffB1->Map(0, nullptr, (void**)&constMap1);
	constMap1->ambient = Vec3(1.0f, 1.0f, 1.0f);
	constMap1->diffuse = Vec3();
	constMap1->specular = Vec3();
	constMap1->alpha = 1.0f;
	Object::OBJbuffer[OBJNum]->constBuffB1->Unmap(0, nullptr);
#ifdef _DEBUG
	Object::OBJbuffer[Object::OBJbuffer.size() - 1]->constBuffB0->SetName(L"ObjectB0");
	Object::OBJbuffer[Object::OBJbuffer.size() - 1]->constBuffB1->SetName(L"ObjectB1");
#endif

}

void Object::Draw(const ObjectData& polygon, const Vec3& position, const Vec3& scale, const Vec3& rotation, const TextureData& graph, const Vec2& uv)
{
	if (OBJNum >= Object::OBJbuffer.size())
	{
		//�萔�o�b�t�@
		OBJConstantBuffer();
	}
	if (Pipeline::SetPipeline(PipelineNormal))
	{
		//�v���~�e�B�u�`��̐ݒ�R�}���h�i�O�p�`���X�g�j
		cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		////�f�X�N���v�^�q�[�v���Z�b�g
		ID3D12DescriptorHeap* ppHeaps[] = { Texture::Get()->GetDescHeap() };
		cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
		if (Pipeline::OBJPipeline.pipelinestate == nullptr || Pipeline::OBJPipeline.rootsignature == nullptr)
		{
			assert(0);
		}

		cmdList->SetPipelineState(Pipeline::OBJPipeline.pipelinestate.Get());
		cmdList->SetGraphicsRootSignature(Pipeline::OBJPipeline.rootsignature.Get());
	}
	//�X�V

	MatWord(polygon, position, scale, rotation, uv);

	//���_�o�b�t�@�̐ݒ�R�}���h
	cmdList->IASetVertexBuffers(0, 1, &polygon.vbView);

	////�C���f�b�N�X�o�b�t�@�r���[�̃Z�b�g�R�}���h
	cmdList->IASetIndexBuffer(&polygon.ibView);

	//�q�[�v�̐擪�ɂ���CBV�����[�g�p�����[�^�O�Ԃɐݒ�
	cmdList->SetGraphicsRootConstantBufferView(0, Object::OBJbuffer[OBJNum]->constBuffB0->GetGPUVirtualAddress());
	cmdList->SetGraphicsRootConstantBufferView(1, Object::OBJbuffer[OBJNum]->constBuffB1->GetGPUVirtualAddress());

	if (graph.s_texNum > 0)
	{
		//�q�[�v�̂Q�Ԗڂɂ���SRV�����[�g�p�����[�^�P�Ԃɐݒ�
		cmdList->SetGraphicsRootDescriptorTable(2, graph.gpuDescHandleSRV);
	}
	else if (polygon.OBJTexture.s_texNum == 0)
	{
		//�q�[�v�̂Q�Ԗڂɂ���SRV�����[�g�p�����[�^�P�Ԃɐݒ�//���摜��ݒ�
		cmdList->SetGraphicsRootDescriptorTable(2, Texture::Get()->GetWhite().gpuDescHandleSRV);
	}
	else
	{
		//�q�[�v�̂Q�Ԗڂɂ���SRV�����[�g�p�����[�^�P�Ԃɐݒ�
		cmdList->SetGraphicsRootDescriptorTable(2, polygon.OBJTexture.gpuDescHandleSRV);
	}
	//���C�g�̕`��
	lightGroup->Draw(cmdList, 3);
	//�e�摜
	cmdList->SetGraphicsRootDescriptorTable(4, Texture::Get()->GetShadowTexture().gpuDescHandleSRV);


	//�`��R�}���h          //���_��				//�C���X�^���X��	//�J�n���_�ԍ�		//�C���X�^���X���Ƃ̉��Z�ԍ�
	cmdList->DrawIndexedInstanced((UINT)polygon.indicesNum, 1, 0, 0, 0);
	OBJNum++;
}

void Object::MatWordUVScroll(const ObjectData& polygon, PSR& psr, const Vec3& position, const Vec3& scale, const Vec3& rotation, const Vec2& uv)
{
	HRESULT result;
	if (psr.position.x != position.x || psr.position.y != position.y || psr.position.z != position.z
		|| psr.scale.x != scale.x || psr.scale.y != scale.y || psr.scale.z != scale.z
		|| psr.rotation.x != rotation.x || psr.rotation.y != rotation.y || psr.rotation.z != rotation.z)
	{
		//���[���h�ϊ��F//�X�P�[�����O//��]�s��XMMATRIX//���s�ړ��s��
		XMMATRIX matScale, matRot, matTrains;
		//���[���h�ϊ�
		matScale = XMMatrixScaling(scale.x, scale.y, scale.z);//�傫��
		matTrains = XMMatrixTranslation(position.x, position.y, position.z);//���s�ړ��s����Čv�Z
		matRot = XMMatrixIdentity();
		matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));//Z���܂��ɂS�T�x��]
		matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));//X���܂��ɂS�T�x��]
		matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));//Y���܂��ɂS�T�x��]

		psr.matWorld = XMMatrixIdentity();//���[���h�s��͖��t���[�����Z�b�g
		psr.matWorld *= matScale;//���[���h�s��ɃX�P�[�����O�𔽉f
		psr.matWorld *= matRot;//���[���h�s��ɉ�]�𔽉f
		psr.matWorld *= matTrains;//���[���h�s��ɕύX�ړ��𔽉f

		psr.position = position;
		psr.scale = scale;
		psr.rotation = rotation;
	}


	const XMMATRIX& matViewProjection = Camera::Get()->GetMatViewProjection();
	const Vec3& cameraPos = Camera::Get()->GetEye();
	//GPU��̃o�b�t�@�ɑΉ��������z���������擾
	ConstBufferDataB0* constMap = nullptr;
	result = Object::OBJbuffer[OBJNum]->constBuffB0->Map(0, nullptr, (void**)&constMap);
	//�s��̍���   ���[���h�ϊ��s�� �� �r���[�ϊ��s�� �� �ˉe�ϊ��s��
	constMap->viewproj = matViewProjection;
	if (psr.parent == nullptr)
	{
		constMap->world = psr.matWorld;
	}
	else
	{
		constMap->world = psr.matWorld * psr.parent->matWorld;
	}
	constMap->cameraPos = cameraPos;
	constMap->uv = uv;
	constMap->lightproj = lightGroup->GetLightMatProjection();
	Object::OBJbuffer[OBJNum]->constBuffB0->Unmap(0, nullptr);


	////�萔�o�b�t�@�փf�[�^�]��
	//ConstBufferDataB1* constMap1 = nullptr;
	//result = Object::OBJbuffer[OBJNum]->constBuffB1->Map(0, nullptr, (void**)&constMap1);
	//constMap1->ambient = polygon.material.ambient;
	//constMap1->diffuse = polygon.material.diffuse;
	//constMap1->specular = polygon.material.specular;
	//constMap1->alpha = polygon.material.alpha;
	//Object::OBJbuffer[OBJNum]->constBuffB1->Unmap(0, nullptr);
}

void Object::DrawUVScroll(const ObjectData& polygon, PSR& psr, const Vec3& position, const Vec3& scale, const Vec3& rotation, const Vec2& uv, const TextureData& graph)
{
	if (OBJNum >= Object::OBJbuffer.size())
	{
		//�萔�o�b�t�@
		OBJConstantBuffer();
	}
	if (Pipeline::SetPipeline(PipelineNoShadow))
	{
		//�v���~�e�B�u�`��̐ݒ�R�}���h�i�O�p�`���X�g�j
		cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//�f�X�N���v�^�q�[�v���Z�b�g
		ID3D12DescriptorHeap* ppHeaps[] = { Texture::Get()->GetDescHeap() };
		cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
		if (Pipeline::OBJPipeline.pipelinestate == nullptr || Pipeline::OBJPipeline.rootsignature == nullptr)
		{
			assert(0);
		}
		cmdList->SetPipelineState(Pipeline::NoShadowOBJPipeline.pipelinestate.Get());
		cmdList->SetGraphicsRootSignature(Pipeline::NoShadowOBJPipeline.rootsignature.Get());
	}
	//�X�V
	MatWordUVScroll(polygon, psr, position, scale, rotation, uv);

	//���_�o�b�t�@�̐ݒ�R�}���h
	cmdList->IASetVertexBuffers(0, 1, &polygon.vbView);

	////�C���f�b�N�X�o�b�t�@�r���[�̃Z�b�g�R�}���h
	cmdList->IASetIndexBuffer(&polygon.ibView);

	//�q�[�v�̐擪�ɂ���CBV�����[�g�p�����[�^�O�Ԃɐݒ�
	cmdList->SetGraphicsRootConstantBufferView(0, Object::OBJbuffer[OBJNum]->constBuffB0->GetGPUVirtualAddress());
	cmdList->SetGraphicsRootConstantBufferView(1, Object::OBJbuffer[OBJNum]->constBuffB1->GetGPUVirtualAddress());
	if (graph.s_texNum > 0)
	{
		//�q�[�v�̂Q�Ԗڂɂ���SRV�����[�g�p�����[�^�P�Ԃɐݒ�
		cmdList->SetGraphicsRootDescriptorTable(2, graph.gpuDescHandleSRV);
	}
	else if (polygon.OBJTexture.s_texNum == 0)
	{
		//�q�[�v�̂Q�Ԗڂɂ���SRV�����[�g�p�����[�^�P�Ԃɐݒ�//���摜��ݒ�
		cmdList->SetGraphicsRootDescriptorTable(2, Texture::Get()->GetWhite().gpuDescHandleSRV);
	}
	else
	{
		//�q�[�v�̂Q�Ԗڂɂ���SRV�����[�g�p�����[�^�P�Ԃɐݒ�
		cmdList->SetGraphicsRootDescriptorTable(2, polygon.OBJTexture.gpuDescHandleSRV);
	}
	//���C�g�̕`��
	lightGroup->Draw(cmdList, 3);

	//�`��R�}���h          //���_��				//�C���X�^���X��	//�J�n���_�ԍ�		//�C���X�^���X���Ƃ̉��Z�ԍ�
	cmdList->DrawIndexedInstanced((UINT)polygon.indicesNum, 1, 0, 0, 0);
	OBJNum++;
}

void Object::NoShadowMatWorld(const ObjectData& polygon, PSR& psr, const Vec3& position, const Vec3& scale, const Vec3& rotation, const Vec2& uv)
{
	HRESULT result;
	if (psr.position.x != position.x || psr.position.y != position.y || psr.position.z != position.z
		|| psr.scale.x != scale.x || psr.scale.y != scale.y || psr.scale.z != scale.z
		|| psr.rotation.x != rotation.x || psr.rotation.y != rotation.y || psr.rotation.z != rotation.z)
	{
		//���[���h�ϊ��F//�X�P�[�����O//��]�s��XMMATRIX//���s�ړ��s��
		XMMATRIX matScale, matRot, matTrains;
		//���[���h�ϊ�
		matScale = XMMatrixScaling(scale.x, scale.y, scale.z);//�傫��
		matTrains = XMMatrixTranslation(position.x, position.y, position.z);//���s�ړ��s����Čv�Z
		matRot = XMMatrixIdentity();
		matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));//Z���܂��ɂS�T�x��]
		matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));//X���܂��ɂS�T�x��]
		matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));//Y���܂��ɂS�T�x��]

		psr.matWorld = XMMatrixIdentity();//���[���h�s��͖��t���[�����Z�b�g
		psr.matWorld *= matScale;//���[���h�s��ɃX�P�[�����O�𔽉f
		psr.matWorld *= matRot;//���[���h�s��ɉ�]�𔽉f
		psr.matWorld *= matTrains;//���[���h�s��ɕύX�ړ��𔽉f

		psr.position = position;
		psr.scale = scale;
		psr.rotation = rotation;
	}


	const XMMATRIX& matViewProjection = Camera::Get()->GetMatViewProjection();
	const Vec3& cameraPos = Camera::Get()->GetEye();
	//GPU��̃o�b�t�@�ɑΉ��������z���������擾
	ConstBufferDataB0* constMap = nullptr;
	result = Object::OBJbuffer[OBJNum]->constBuffB0->Map(0, nullptr, (void**)&constMap);
	//�s��̍���   ���[���h�ϊ��s�� �� �r���[�ϊ��s�� �� �ˉe�ϊ��s��
	constMap->viewproj = matViewProjection;
	if (psr.parent == nullptr)
	{
		constMap->world = psr.matWorld;
	}
	else
	{
		constMap->world = psr.matWorld * psr.parent->matWorld;
	}
	constMap->cameraPos = cameraPos;
	constMap->uv = uv;
	Object::OBJbuffer[OBJNum]->constBuffB0->Unmap(0, nullptr);


	//�萔�o�b�t�@�փf�[�^�]��
	ConstBufferDataB1* constMap1 = nullptr;
	result = Object::OBJbuffer[OBJNum]->constBuffB1->Map(0, nullptr, (void**)&constMap1);
	constMap1->ambient = polygon.material.ambient;
	constMap1->diffuse = polygon.material.diffuse;
	constMap1->specular = polygon.material.specular;
	constMap1->alpha = polygon.material.alpha;
	Object::OBJbuffer[OBJNum]->constBuffB1->Unmap(0, nullptr);
}

void Object::NoShadowDraw(const ObjectData& polygon, PSR& psr, const Vec3& position, const Vec3& scale, const Vec3& rotation, const Vec2& uv, const TextureData& graph)
{
	if (OBJNum >= Object::OBJbuffer.size())
	{
		//�萔�o�b�t�@
		OBJConstantBuffer();
	}

	//�X�V
	NoShadowMatWorld(polygon, psr, position, scale, rotation, uv);
	if (Pipeline::SetPipeline(PipelineNoShadow))
	{
		//�v���~�e�B�u�`��̐ݒ�R�}���h�i�O�p�`���X�g�j
		cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//�f�X�N���v�^�q�[�v���Z�b�g
		ID3D12DescriptorHeap* ppHeaps[] = { Texture::Get()->GetDescHeap() };
		cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
		if (Pipeline::OBJPipeline.pipelinestate == nullptr || Pipeline::OBJPipeline.rootsignature == nullptr)
		{
			assert(0);
		}
		cmdList->SetPipelineState(Pipeline::NoShadowOBJPipeline.pipelinestate.Get());
		cmdList->SetGraphicsRootSignature(Pipeline::NoShadowOBJPipeline.rootsignature.Get());
	}
	//���_�o�b�t�@�̐ݒ�R�}���h
	cmdList->IASetVertexBuffers(0, 1, &polygon.vbView);

	////�C���f�b�N�X�o�b�t�@�r���[�̃Z�b�g�R�}���h
	cmdList->IASetIndexBuffer(&polygon.ibView);

	//�q�[�v�̐擪�ɂ���CBV�����[�g�p�����[�^�O�Ԃɐݒ�
	cmdList->SetGraphicsRootConstantBufferView(0, Object::OBJbuffer[OBJNum]->constBuffB0->GetGPUVirtualAddress());
	cmdList->SetGraphicsRootConstantBufferView(1, Object::OBJbuffer[OBJNum]->constBuffB1->GetGPUVirtualAddress());
	if (graph.s_texNum > 0)
	{
		//�q�[�v�̂Q�Ԗڂɂ���SRV�����[�g�p�����[�^�P�Ԃɐݒ�
		cmdList->SetGraphicsRootDescriptorTable(2, graph.gpuDescHandleSRV);
	}
	else if (polygon.OBJTexture.s_texNum == 0)
	{
		//�q�[�v�̂Q�Ԗڂɂ���SRV�����[�g�p�����[�^�P�Ԃɐݒ�//���摜��ݒ�
		cmdList->SetGraphicsRootDescriptorTable(2, Texture::Get()->GetWhite().gpuDescHandleSRV);
	}
	else
	{
		//�q�[�v�̂Q�Ԗڂɂ���SRV�����[�g�p�����[�^�P�Ԃɐݒ�
		cmdList->SetGraphicsRootDescriptorTable(2, polygon.OBJTexture.gpuDescHandleSRV);
	}
	//���C�g�̕`��
	lightGroup->Draw(cmdList, 3);

	//�`��R�}���h          //���_��				//�C���X�^���X��	//�J�n���_�ԍ�		//�C���X�^���X���Ƃ̉��Z�ԍ�
	cmdList->DrawIndexedInstanced((UINT)polygon.indicesNum, 1, 0, 0, 0);
	OBJNum++;
}
