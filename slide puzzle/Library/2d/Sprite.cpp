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
	// 3Dオブジェクトのインスタンスを生成
	Sprite* sprite = new Sprite();
	if (sprite == nullptr) {
		return nullptr;
	}
	sprite->Init();
	// 初期化
	return sprite;
}

//初期化
void Sprite::SpriteAllInitialization(int widow_width, int window_height)
{
	//スプライト共通データ
	spriteCommon = Sprite::SpriteCommonCreate(widow_width, window_height);
}


//ここから下は変えない
void Sprite::Init()
{
	SpriteAllInitialization(window_width, window_height);
}

SpriteData Sprite::SpriteCreate(const wchar_t* filename)
{
	HRESULT result = S_FALSE;

	//新しいスプライトを作る
	SpriteData sprite{};

	//テクスチャ番号をコピー
	sprite.texNumber = Texture::Get()->LoadTexture(filename);

	//頂点データ
	VertexPosUv vertices[] =
	{
		{{0.0f,100.0f,0.0f},{0.0f,1.0f}},//左下
		{{0.0f,0.0f,0.0f},{0.0f,0.0f}},//右上
		{{100.0f,100.0f,0.0f},{1.0f,1.0f}},//右下
		{{100.0f,0.0f,0.0f},{1.0f,0.0f}},//右上
	};

	//頂点バッファ生成
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices)),
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&sprite.vertBuff));

	//頂点バッファへのデータ転送
	VertexPosUv* vertMap = nullptr;
	result = sprite.vertBuff->Map(0, nullptr, (void**)&vertMap);
	memcpy(vertMap, vertices, sizeof(vertices));
	sprite.vertBuff->Unmap(0, nullptr);


	//頂点バッファビューの作成
	sprite.vbView.BufferLocation = sprite.vertBuff->GetGPUVirtualAddress();
	sprite.vbView.SizeInBytes = sizeof(vertices);
	sprite.vbView.StrideInBytes = sizeof(vertices[0]);


	//指定番号の画像が読み込み済みなら
	if (sprite.texNumber.texbuff)
	{
		//テクスチャ情報を取得
		D3D12_RESOURCE_DESC resDesc = sprite.texNumber.texbuff->GetDesc();
		//スプライトの大きさを画像の解像度に合わせる
		sprite.size = { (float)resDesc.Width,(float)resDesc.Height };
	}
	//頂点バッファデータ転送
	SpriteTransferVertexBuffer(sprite);


	constBuffer.push_back({ new ConstBuffer });
	//定数バッファの生成
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&constBuffer[constBuffer.size() - 1]->constBuff));

	//定数バッファにデータ転送
	ConstBufferData* constMap = nullptr;
	result = constBuffer[constBuffer.size() - 1]->constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->color = Vec4(1, 1, 1, 1);//色指定
	//平行投影行列
	constMap->mat = XMMatrixOrthographicOffCenterLH(
		0.0f, (float)window_width, (float)window_height, 0.0f, 0.0f, 1.0f);
	constBuffer[constBuffer.size() - 1]->constBuff->Unmap(0, nullptr);

	return sprite;
}

//スプライト単体頂点バッファの転送
void Sprite::SpriteTransferVertexBuffer(const SpriteData& sprite)
{
	HRESULT result = S_FALSE;

	//頂点データ
	VertexPosUv vertices[] = {
		{{},{0.0f,1.0f}},//左下
		{{},{0.0f,0.0f}},//左上
		{{},{1.0f,1.0f}},//右下
		{{},{1.0f,0.0f}},//右上
	};

	//左下　左上　右下　右上
	enum { LB, LT, RB, RT };

	float left = (0.0f - sprite.anchorpoint.x) * sprite.size.x;
	float right = (1.0f - sprite.anchorpoint.x) * sprite.size.x;
	float top = (0.0f - sprite.anchorpoint.y) * sprite.size.y;
	float bottom = (1.0f - sprite.anchorpoint.y) * sprite.size.y;

	vertices[LB].pos = { left,bottom,0.0f };//左下
	vertices[LT].pos = { left,top,0.0f };//左上
	vertices[RB].pos = { right,bottom,0.0f };//右下
	vertices[RT].pos = { right,top,0.0f };//右上

	//uv計算
	//指定番号の画像が読み込み済みなら
	if (sprite.texNumber.texbuff && !sprite.texSize.x == 0 && !sprite.texSize.y == 0)
	{
		//テクスチャ情報取得
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
	//頂点バッファへのデータ転送
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
	//定数バッファの生成
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

//共通データを生成する関数を作成
Sprite::SpriteCommon Sprite::SpriteCommonCreate(int window_width, int window_height)
{
	HRESULT result = S_FALSE;

	//新たなスプライト共通データを生成
	SpriteCommon spriteCommon{};

	//スプライト用パイプライン生成
	spriteCommon.pipelineSet = Pipeline::SpriteCreateGraphicsPipeline(dev, ShaderManager::spriteShader);

	//平行投影の射影行列生成
	spriteCommon.matProjection = XMMatrixOrthographicOffCenterLH(
		0.0f, (float)window_width, (float)window_height, 0.0f, 0.0f, 1.0f);

	//生成したスプライト共通データを返す
	return spriteCommon;
}

//スプライト共通グラフィックコマンドのセット
void Sprite::SpriteCommonBeginDraw()
{
	//パイプラインステートの設定
	cmdList->SetPipelineState(spriteCommon.pipelineSet.pipelinestate.Get());
	//ルートシグネチャの設定
	cmdList->SetGraphicsRootSignature(spriteCommon.pipelineSet.rootsignature.Get());
	//プリミティブ形状を設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//テクスチャ用デスクリプタヒープの設定
	ID3D12DescriptorHeap* ppHeaps[] = { Texture::Get()->GetDescHeap() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
}
//スプライト単体更新
void Sprite::Update(SpriteData& sprite, const Vec2 &position, float width, float height, const Vec2 &anchorpoint, const Vec4 &color, bool isFlipX, bool isFlipY)
{

	if (sprite.size.x != width || sprite.size.y != height ||
		sprite.anchorpoint.x != anchorpoint.x || sprite.anchorpoint.y != anchorpoint.y ||
		isFlipX == true || isFlipY == true)
	{
		//サイズを合わせる
		sprite.size.x = width, sprite.size.y = height;
		sprite.anchorpoint = anchorpoint;
		//頂点データ
		VertexPosUv vertices[] = {
				{{},{0.0f,1.0f}},//左下
				{{},{0.0f,0.0f}},//左上
				{{},{1.0f,1.0f}},//右下
				{{},{1.0f,0.0f}},//右上
		};
		//左下　左上　右下　右上
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

		vertices[LB].pos = { left,bottom,0.0f };//左下
		vertices[LT].pos = { left,top,0.0f };//左上
		vertices[RB].pos = { right,bottom,0.0f };//右下
		vertices[RT].pos = { right,top,0.0f };//右上
		//頂点バッファへのデータ転送
		VertexPosUv* vertMap = nullptr;
		sprite.vertBuff->Map(0, nullptr, (void**)&vertMap);
		memcpy(vertMap, vertices, sizeof(vertices));
		sprite.vertBuff->Unmap(0, nullptr);
	}

	//ワールド行列の更新
	sprite.matWorld = XMMatrixIdentity();
	//Z軸回転
	sprite.matWorld *= XMMatrixRotationZ(XMConvertToRadians(sprite.rotation));
	//平行移動
	sprite.matWorld *= XMMatrixTranslation(position.x, position.y, 0);


	if (sprite.parent == nullptr)
	{
		//定数バッファの転送
		ConstBufferData* constMap = nullptr;
		HRESULT result = constBuffer[spriteNum]->constBuff->Map(0, nullptr, (void**)&constMap);
		constMap->mat = sprite.matWorld * spriteCommon.matProjection;
		constMap->color = color;
		constBuffer[spriteNum]->constBuff->Unmap(0, nullptr);
	}
	else
	{
		//定数バッファの転送
		ConstBufferData* constMap = nullptr;
		HRESULT result = constBuffer[spriteNum]->constBuff->Map(0, nullptr, (void**)&constMap);
		constMap->mat = sprite.matWorld * sprite.parent->matWorld * spriteCommon.matProjection;
		constMap->color = color;
		constBuffer[spriteNum]->constBuff->Unmap(0, nullptr);
	}

}

//スプライト単体描画
void Sprite::Draw(SpriteData& sprite, const Vec2 &position, const float width, const float height, const Vec2 &anchorpoint, const Vec4 &color, const bool isFlipX, const bool isFlipY)
{
	SpriteCommonBeginDraw();

	if (constBuffer.size() <= spriteNum)
	{
		CreateConstBuffer();
	}

	Update(sprite, position, width, height, anchorpoint, color, isFlipX, isFlipY);
	//頂点バッファをセット
	cmdList->IASetVertexBuffers(0, 1, &sprite.vbView);
	//定数バッファをセット
	cmdList->SetGraphicsRootConstantBufferView(0, constBuffer[spriteNum]->constBuff->GetGPUVirtualAddress());

	//シェーダーリソースビューをセット
	cmdList->SetGraphicsRootDescriptorTable(1,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(
			Texture::Get()->GetDescHeap()->GetGPUDescriptorHandleForHeapStart(),
			sprite.texNumber.s_texNum,
			dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));

	//ポリゴンの描画（４頂点で四角形）
	cmdList->DrawInstanced(4, 1, 0, 0);

	spriteNum++;
}


void Sprite::DebugUpdate(SpriteData& sprite)
{
	//ワールド行列の更新
	sprite.matWorld = XMMatrixIdentity();
	//Z軸回転
	sprite.matWorld *= XMMatrixRotationZ(XMConvertToRadians(sprite.rotation));
	//平行移動
	sprite.matWorld *= XMMatrixTranslation(sprite.position.x, sprite.position.y, 0);

	//定数バッファの転送
	ConstBufferData* constMap = nullptr;
	HRESULT result = constBuffer[spriteNum]->constBuff->Map(0, nullptr, (void**)&constMap);
	constMap->mat = sprite.matWorld * spriteCommon.matProjection;
	constMap->color = { 1.0f,1.0f,1.0f,1.0f };
	constBuffer[spriteNum]->constBuff->Unmap(0, nullptr);
}

//スプライト単体描画
void Sprite::DebugDraw(SpriteData& sprite)
{
	SpriteCommonBeginDraw();

	if (constBuffer.size() <= spriteNum)
	{
		CreateConstBuffer();
	}

	DebugUpdate(sprite);

	//頂点バッファをセット
	cmdList->IASetVertexBuffers(0, 1, &sprite.vbView);
	//定数バッファをセット
	cmdList->SetGraphicsRootConstantBufferView(0, constBuffer[spriteNum]->constBuff->GetGPUVirtualAddress());

	//シェーダーリソースビューをセット
	cmdList->SetGraphicsRootDescriptorTable(1,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(
			Texture::Get()->GetDescHeap()->GetGPUDescriptorHandleForHeapStart(),
			sprite.texNumber.s_texNum,
			dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));

	//ポリゴンの描画（４頂点で四角形）
	cmdList->DrawInstanced(4, 1, 0, 0);
	spriteNum++;
}

void Sprite::PostEffectDraw(ID3D12DescriptorHeap* descHeap, SpriteData& sprite, const Vec2 &position, float width, float height, const Vec2 &anchorpoint, const Vec4 &color, bool isFlipX, bool isFlipY)
{
	//パイプラインステートの設定
	cmdList->SetPipelineState(spriteCommon.pipelineSet.pipelinestate.Get());
	//ルートシグネチャの設定
	cmdList->SetGraphicsRootSignature(spriteCommon.pipelineSet.rootsignature.Get());
	//プリミティブ形状を設定
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//テクスチャ用デスクリプタヒープの設定
	ID3D12DescriptorHeap* ppHeaps[] = { descHeap };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	if (constBuffer.size() <= spriteNum)
	{
		CreateConstBuffer();
	}

	Update(sprite, position, width, height, anchorpoint, color, isFlipX, isFlipY);
	//頂点バッファをセット
	cmdList->IASetVertexBuffers(0, 1, &sprite.vbView);
	//定数バッファをセット
	cmdList->SetGraphicsRootConstantBufferView(0, constBuffer[spriteNum]->constBuff->GetGPUVirtualAddress());

	cmdList->SetGraphicsRootDescriptorTable(1, descHeap->GetGPUDescriptorHandleForHeapStart());
	//ポリゴンの描画（４頂点で四角形）
	cmdList->DrawInstanced(4, 1, 0, 0);

	spriteNum++;
}