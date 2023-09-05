#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include<forward_list>
#include"Camera.h"
#include"Pipeline.h"
#include<Texture.h>
/// <summary>
/// パーティクルマネジャー
/// </summary>
class ParticleManager 
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMMATRIX = DirectX::XMMATRIX;
public: // サブクラス

	struct VertexPos
	{
		Vec3 pos;
		float scale;//スケール
		Vec4 color;
	};

	// 定数バッファ用データ構造体
	struct ConstBufferData
	{
		XMMATRIX mat;	// ３Ｄ変換行列
		XMMATRIX matBillboard;//ビルボード行列
	};

	struct Particle
	{
		//座標
		Vec3 position = {};
		//速度
		Vec3 velocity = {};
		//加速度
		Vec3 accel = {};
		//現在フレーム
		int frame = 0;
		//終了フレーム
		int num_frame = 0;
		//スケール
		float scale = 1.0f;
		//初期値
		float s_scale = 1.0f;
		//最終地
		float e_scale = 0.0f;

		//カラー
		Vec4 color = {};
		//初期値
		Vec4 s_color = {};
		//最終地
		Vec4 e_color = {};

	};
private: // 定数
	static const int vertexCount = 1024;//頂点数
public: // 静的メンバ関数
	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="window_width">画面幅</param>
	/// <param name="window_height">画面高さ</param>
	/// <returns>成否</returns>
	static bool StaticInitialize(ID3D12Device *device, ID3D12GraphicsCommandList *cmdList, int window_width, int window_height);

	/// <summary>
	/// 描画前処理
	/// </summary>
	/// <param name="cmdList">描画コマンドリスト</param>
	static void PreDraw(ID3D12GraphicsCommandList *cmdList);

	/// <summary>
	/// 描画後処理
	/// </summary>
	static void PostDraw();



	/// <summary>
	/// パーティクルの追加
	/// </summary>
	/// <param name = "life">生存時間</param>
	/// <param name = "position">初期座標</param>
	/// <param name = "velocity">速度</param>
	/// <param name = "accel">加速度</param>
	void Add(int life, const Vec3 &position, const Vec3 &velocity, const Vec3 &accel,
		float start_scale, float end_scale, const Vec4 &start_color, const Vec4 &end_color);

	void BreakBoxAdd(const Vec3 &Pos, float Vel, float start_scale, float end_scale, const Vec4 &start_color, const Vec4 &end_color);

	/// <summary>
	/// 爆発パーティクル
	/// </summary>
	/// <param name="Pos">座標</param>
	/// <param name="Vel">方向</param>
	/// <param name="start_scale">最初のサイズ</param>
	/// <param name="end_scale">最後のサイズ</param>
	/// <param name="start_color">最初の色</param>
	/// <param name="end_color">最後の色</param>
	void BombAdd(const Vec3& Pos, float Vel, float start_scale, float end_scale, const Vec4& start_color, const Vec4& end_color);

	void ParticleAdd2(const Vec3 &Pos, const Vec4 &start_color, const Vec4 & end_color);
private: // 静的メンバ変数
	// デバイス
	static ID3D12Device *device;

	// コマンドリスト
	static ID3D12GraphicsCommandList *cmdList;

	//ビルボード行列
	static XMMATRIX matBillboard;
	//Y軸回りビルボード行列
	static XMMATRIX matBillboardY;
	//パイプライン設定
	static Pipeline::PipelineSet PartclePipelineSet;

private:// 静的メンバ関数
	/// <summary>
	/// モデル作成
	/// </summary>
	void CreateModel();
	/// <summary>
	/// ビルボート行列の更新
	/// </summary>
	void MatBillboardUpdate();

public: // メンバ関数
	bool Initialize();
	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();


	/// <summary>
	/// 描画
	/// </summary>
	void Draw(TextureData &graph);

private: // メンバ変数

	//パーティクル配列
	std::forward_list<Particle>particles;

	// ローカルスケール
	Vec3 m_scale = { 1,1,1 };
	//ローカルカラー
	Vec4 m_color = { 1,1,1,1 };
	ComPtr<ID3D12Resource> m_vertBuff;
	ComPtr<ID3D12Resource>m_constBuff; // 定数バッファ

	D3D12_VERTEX_BUFFER_VIEW m_vbView{};
	ParticleManager::VertexPos m_vertices[vertexCount];
};

