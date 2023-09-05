#pragma once
#include "Sprite.h"
#include"Singleton.h"
#include"Sprite.h"
#include"Texture.h"
/// <summary>
/// ポストエフェクト
/// </summary>
class PostEffect : public Singleton<PostEffect>
{
	template<class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
private:
	friend Singleton<PostEffect>;
public:

	//画面クリアカラー
	static const float clearColor[4];
	static ID3D12Device* dev;
	/// <summary>
	/// コンストラクタ
	/// </summary>
	PostEffect();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(ID3D12Device* dev);

	//パイプライン生成
	void CreatePipeline(ID3D12Device* dev);

	/// <summary>
	/// 描画コマンドの発行
	/// </summary>
	/// <param name="cmdList"></param>
	void Draw(ID3D12GraphicsCommandList* cmdList,const Vec4 &color = { 1.0f,1.0f,1.0f,1.0f });
	/// <summary>
	/// シーン描画前処理
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	void PreDrawScene(ID3D12GraphicsCommandList* cmdList);
	/// <summary>
	/// シーン描画後処理
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	void PostDrawScene(ID3D12GraphicsCommandList* cmdList);

	void UpdatePost(SpriteData& sprite, const Vec2 &position, float width, float height, const Vec2 &anchorpoint, const Vec4 &color, bool isFlipX, bool isFlipY);
	/// <summary>
	/// ポストエフェクト描画
	/// </summary>
	/// <param name="sprite">スプライトデータ</param>
	/// <param name="position">座標</param>
	/// <param name="width">横幅</param>
	/// <param name="height">縦幅</param>
	/// <param name="anchorpoint">アンカーポイント</param>
	/// <param name="color">色</param>
	/// <param name="isFlipX">横反転</param>
	/// <param name="isFlipY">縦反転</param>
	void DrawPost(SpriteData& sprite, const Vec2 &position, float width, float height, const Vec2 &anchorpoint, const Vec4 &color, bool isFlipX, bool isFlipY);
	//パイプライン切り替え
	void SetPipeline(int num);

	ID3D12Resource* GetDepth() { return m_depthBuff.Get(); }
private:

	//テクスチャバッファ
	ComPtr<ID3D12Resource>m_texbuff;
	//SRV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap>m_descHeapSRV;
	//深度バッファ
	ComPtr<ID3D12Resource>m_depthBuff;
	//RTV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap>m_descHeapRTV;
	//DSV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap>m_descHeapDSV;

	//GPUリソースの生成1
	ComPtr<ID3D12Resource> m_constBuff;

	SpriteData m_sprite;

	//パイプラインセット
	Pipeline::PipelineSet m_pipelineSet;
};

