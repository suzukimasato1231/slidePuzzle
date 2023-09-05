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
	// 3Dオブジェクトのインスタンスを生成
	Object* object = new Object();
	if (object == nullptr) {
		return nullptr;
	}

	// 初期化
	return object;
}

void Object::Delete()
{
	//OBJデータ削除
	for (int i = (int)OBJbuffer.size() - 1; i >= 0; i--)
	{
		delete OBJbuffer[i];
		OBJbuffer.erase(OBJbuffer.begin() + i);
	}
}

void Object::MatWord(const ObjectData& polygon, const Vec3& position, const Vec3& scale, const Vec3& rotation, const Vec2& uv)
{
	HRESULT result;

	//ワールド変換：//スケーリング//回転行列XMMATRIX//平行移動行列
	XMMATRIX matScale, matRot, matTrains;
	//ワールド変換
	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);//大きさ
	matTrains = XMMatrixTranslation(position.x, position.y, position.z);//平行移動行列を再計算

	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));//Z軸まわりに４５度回転
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));//X軸まわりに４５度回転
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));//Y軸まわりに４５度回転
	//ワールド変換
	XMMATRIX matWorld;
	matWorld = XMMatrixIdentity();//ワールド行列は毎フレームリセット
	matWorld *= matScale;//ワールド行列にスケーリングを反映
	matWorld *= matRot;//ワールド行列に回転を反映
	matWorld *= matTrains;//ワールド行列に変更移動を反映


	const XMMATRIX& matViewProjection = Camera::Get()->GetMatViewProjection();
	const Vec3& cameraPos = Camera::Get()->GetEye();
	//GPU上のバッファに対応した仮想メモリを取得
	ConstBufferDataB0* constMap = nullptr;
	result = Object::OBJbuffer[OBJNum]->constBuffB0->Map(0, nullptr, (void**)&constMap);
	//行列の合成   ワールド変換行列 ＊ ビュー変換行列 ＊ 射影変換行列
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


	//定数バッファへデータ転送
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
	// 定数バッファの生成
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB0) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&Object::OBJbuffer[Object::OBJbuffer.size() - 1]->constBuffB0));


	// 定数バッファの生成
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB1) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&Object::OBJbuffer[Object::OBJbuffer.size() - 1]->constBuffB1));
	//定数バッファへデータ転送
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
		//定数バッファ
		OBJConstantBuffer();
	}
	if (Pipeline::SetPipeline(PipelineNormal))
	{
		//プリミティブ形状の設定コマンド（三角形リスト）
		cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		////デスクリプタヒープをセット
		ID3D12DescriptorHeap* ppHeaps[] = { Texture::Get()->GetDescHeap() };
		cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
		if (Pipeline::OBJPipeline.pipelinestate == nullptr || Pipeline::OBJPipeline.rootsignature == nullptr)
		{
			assert(0);
		}

		cmdList->SetPipelineState(Pipeline::OBJPipeline.pipelinestate.Get());
		cmdList->SetGraphicsRootSignature(Pipeline::OBJPipeline.rootsignature.Get());
	}
	//更新

	MatWord(polygon, position, scale, rotation, uv);

	//頂点バッファの設定コマンド
	cmdList->IASetVertexBuffers(0, 1, &polygon.vbView);

	////インデックスバッファビューのセットコマンド
	cmdList->IASetIndexBuffer(&polygon.ibView);

	//ヒープの先頭にあるCBVをルートパラメータ０番に設定
	cmdList->SetGraphicsRootConstantBufferView(0, Object::OBJbuffer[OBJNum]->constBuffB0->GetGPUVirtualAddress());
	cmdList->SetGraphicsRootConstantBufferView(1, Object::OBJbuffer[OBJNum]->constBuffB1->GetGPUVirtualAddress());

	if (graph.s_texNum > 0)
	{
		//ヒープの２番目にあるSRVをルートパラメータ１番に設定
		cmdList->SetGraphicsRootDescriptorTable(2, graph.gpuDescHandleSRV);
	}
	else if (polygon.OBJTexture.s_texNum == 0)
	{
		//ヒープの２番目にあるSRVをルートパラメータ１番に設定//白画像を設定
		cmdList->SetGraphicsRootDescriptorTable(2, Texture::Get()->GetWhite().gpuDescHandleSRV);
	}
	else
	{
		//ヒープの２番目にあるSRVをルートパラメータ１番に設定
		cmdList->SetGraphicsRootDescriptorTable(2, polygon.OBJTexture.gpuDescHandleSRV);
	}
	//ライトの描画
	lightGroup->Draw(cmdList, 3);
	//影画像
	cmdList->SetGraphicsRootDescriptorTable(4, Texture::Get()->GetShadowTexture().gpuDescHandleSRV);


	//描画コマンド          //頂点数				//インスタンス数	//開始頂点番号		//インスタンスごとの加算番号
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
		//ワールド変換：//スケーリング//回転行列XMMATRIX//平行移動行列
		XMMATRIX matScale, matRot, matTrains;
		//ワールド変換
		matScale = XMMatrixScaling(scale.x, scale.y, scale.z);//大きさ
		matTrains = XMMatrixTranslation(position.x, position.y, position.z);//平行移動行列を再計算
		matRot = XMMatrixIdentity();
		matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));//Z軸まわりに４５度回転
		matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));//X軸まわりに４５度回転
		matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));//Y軸まわりに４５度回転

		psr.matWorld = XMMatrixIdentity();//ワールド行列は毎フレームリセット
		psr.matWorld *= matScale;//ワールド行列にスケーリングを反映
		psr.matWorld *= matRot;//ワールド行列に回転を反映
		psr.matWorld *= matTrains;//ワールド行列に変更移動を反映

		psr.position = position;
		psr.scale = scale;
		psr.rotation = rotation;
	}


	const XMMATRIX& matViewProjection = Camera::Get()->GetMatViewProjection();
	const Vec3& cameraPos = Camera::Get()->GetEye();
	//GPU上のバッファに対応した仮想メモリを取得
	ConstBufferDataB0* constMap = nullptr;
	result = Object::OBJbuffer[OBJNum]->constBuffB0->Map(0, nullptr, (void**)&constMap);
	//行列の合成   ワールド変換行列 ＊ ビュー変換行列 ＊ 射影変換行列
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


	////定数バッファへデータ転送
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
		//定数バッファ
		OBJConstantBuffer();
	}
	if (Pipeline::SetPipeline(PipelineNoShadow))
	{
		//プリミティブ形状の設定コマンド（三角形リスト）
		cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//デスクリプタヒープをセット
		ID3D12DescriptorHeap* ppHeaps[] = { Texture::Get()->GetDescHeap() };
		cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
		if (Pipeline::OBJPipeline.pipelinestate == nullptr || Pipeline::OBJPipeline.rootsignature == nullptr)
		{
			assert(0);
		}
		cmdList->SetPipelineState(Pipeline::NoShadowOBJPipeline.pipelinestate.Get());
		cmdList->SetGraphicsRootSignature(Pipeline::NoShadowOBJPipeline.rootsignature.Get());
	}
	//更新
	MatWordUVScroll(polygon, psr, position, scale, rotation, uv);

	//頂点バッファの設定コマンド
	cmdList->IASetVertexBuffers(0, 1, &polygon.vbView);

	////インデックスバッファビューのセットコマンド
	cmdList->IASetIndexBuffer(&polygon.ibView);

	//ヒープの先頭にあるCBVをルートパラメータ０番に設定
	cmdList->SetGraphicsRootConstantBufferView(0, Object::OBJbuffer[OBJNum]->constBuffB0->GetGPUVirtualAddress());
	cmdList->SetGraphicsRootConstantBufferView(1, Object::OBJbuffer[OBJNum]->constBuffB1->GetGPUVirtualAddress());
	if (graph.s_texNum > 0)
	{
		//ヒープの２番目にあるSRVをルートパラメータ１番に設定
		cmdList->SetGraphicsRootDescriptorTable(2, graph.gpuDescHandleSRV);
	}
	else if (polygon.OBJTexture.s_texNum == 0)
	{
		//ヒープの２番目にあるSRVをルートパラメータ１番に設定//白画像を設定
		cmdList->SetGraphicsRootDescriptorTable(2, Texture::Get()->GetWhite().gpuDescHandleSRV);
	}
	else
	{
		//ヒープの２番目にあるSRVをルートパラメータ１番に設定
		cmdList->SetGraphicsRootDescriptorTable(2, polygon.OBJTexture.gpuDescHandleSRV);
	}
	//ライトの描画
	lightGroup->Draw(cmdList, 3);

	//描画コマンド          //頂点数				//インスタンス数	//開始頂点番号		//インスタンスごとの加算番号
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
		//ワールド変換：//スケーリング//回転行列XMMATRIX//平行移動行列
		XMMATRIX matScale, matRot, matTrains;
		//ワールド変換
		matScale = XMMatrixScaling(scale.x, scale.y, scale.z);//大きさ
		matTrains = XMMatrixTranslation(position.x, position.y, position.z);//平行移動行列を再計算
		matRot = XMMatrixIdentity();
		matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));//Z軸まわりに４５度回転
		matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));//X軸まわりに４５度回転
		matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));//Y軸まわりに４５度回転

		psr.matWorld = XMMatrixIdentity();//ワールド行列は毎フレームリセット
		psr.matWorld *= matScale;//ワールド行列にスケーリングを反映
		psr.matWorld *= matRot;//ワールド行列に回転を反映
		psr.matWorld *= matTrains;//ワールド行列に変更移動を反映

		psr.position = position;
		psr.scale = scale;
		psr.rotation = rotation;
	}


	const XMMATRIX& matViewProjection = Camera::Get()->GetMatViewProjection();
	const Vec3& cameraPos = Camera::Get()->GetEye();
	//GPU上のバッファに対応した仮想メモリを取得
	ConstBufferDataB0* constMap = nullptr;
	result = Object::OBJbuffer[OBJNum]->constBuffB0->Map(0, nullptr, (void**)&constMap);
	//行列の合成   ワールド変換行列 ＊ ビュー変換行列 ＊ 射影変換行列
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


	//定数バッファへデータ転送
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
		//定数バッファ
		OBJConstantBuffer();
	}

	//更新
	NoShadowMatWorld(polygon, psr, position, scale, rotation, uv);
	if (Pipeline::SetPipeline(PipelineNoShadow))
	{
		//プリミティブ形状の設定コマンド（三角形リスト）
		cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//デスクリプタヒープをセット
		ID3D12DescriptorHeap* ppHeaps[] = { Texture::Get()->GetDescHeap() };
		cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
		if (Pipeline::OBJPipeline.pipelinestate == nullptr || Pipeline::OBJPipeline.rootsignature == nullptr)
		{
			assert(0);
		}
		cmdList->SetPipelineState(Pipeline::NoShadowOBJPipeline.pipelinestate.Get());
		cmdList->SetGraphicsRootSignature(Pipeline::NoShadowOBJPipeline.rootsignature.Get());
	}
	//頂点バッファの設定コマンド
	cmdList->IASetVertexBuffers(0, 1, &polygon.vbView);

	////インデックスバッファビューのセットコマンド
	cmdList->IASetIndexBuffer(&polygon.ibView);

	//ヒープの先頭にあるCBVをルートパラメータ０番に設定
	cmdList->SetGraphicsRootConstantBufferView(0, Object::OBJbuffer[OBJNum]->constBuffB0->GetGPUVirtualAddress());
	cmdList->SetGraphicsRootConstantBufferView(1, Object::OBJbuffer[OBJNum]->constBuffB1->GetGPUVirtualAddress());
	if (graph.s_texNum > 0)
	{
		//ヒープの２番目にあるSRVをルートパラメータ１番に設定
		cmdList->SetGraphicsRootDescriptorTable(2, graph.gpuDescHandleSRV);
	}
	else if (polygon.OBJTexture.s_texNum == 0)
	{
		//ヒープの２番目にあるSRVをルートパラメータ１番に設定//白画像を設定
		cmdList->SetGraphicsRootDescriptorTable(2, Texture::Get()->GetWhite().gpuDescHandleSRV);
	}
	else
	{
		//ヒープの２番目にあるSRVをルートパラメータ１番に設定
		cmdList->SetGraphicsRootDescriptorTable(2, polygon.OBJTexture.gpuDescHandleSRV);
	}
	//ライトの描画
	lightGroup->Draw(cmdList, 3);

	//描画コマンド          //頂点数				//インスタンス数	//開始頂点番号		//インスタンスごとの加算番号
	cmdList->DrawIndexedInstanced((UINT)polygon.indicesNum, 1, 0, 0, 0);
	OBJNum++;
}
