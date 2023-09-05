#pragma once
#include <DirectXMath.h>
#include "Vec.h"
/// <summary>
/// 丸影
/// </summary>
class CircleShadow
{
private: // エイリアス
	// DirectX::を省略
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public: // サブクラス

	// 定数バッファ用データ構造体
	struct ConstBufferData
	{
		XMVECTOR dir;
		Vec3 casterPos;
		float distanceCasterLight;
		Vec3 atten;
		float pad3;
		Vec2 factorAngleCos;
		unsigned int active;
		float pad4;
	};

public: // メンバ関数
	/// <summary>
	/// 方向をセット
	/// </summary>
	/// <param name="lightdir">方向</param>
	inline void SetDir(const XMVECTOR& dir) { m_dir = DirectX::XMVector3Normalize(dir); }

	/// <summary>
	/// ライト方向を取得
	/// </summary>
	/// <returns>ライト方向</returns>
	inline const XMVECTOR& GetDir() { return m_dir; }

	/// <summary>
	/// キャスター座標をセット
	/// </summary>
	/// <param name="lightpos">キャスター座標</param>
	inline void SetCasterPos(const Vec3& casterPos) { m_casterPos = casterPos; }

	/// <summary>
	/// キャスター座標を取得
	/// </summary>
	/// <returns>キャスター座標</returns>
	inline const Vec3& GetCasterPos() { return m_casterPos; }

	/// <summary>
	/// キャスターとライトの距離をセット
	/// </summary>
	/// <param name="lightpos">キャスターとライトの距離</param>
	inline void SetDistanceCasterLight(float distanceCasterLight) { m_distanceCasterLight = distanceCasterLight; }

	/// <summary>
	/// キャスターとライトの距離を取得
	/// </summary>
	/// <returns>キャスターとライトの距離</returns>
	inline float GetDistanceCasterLight() { return m_distanceCasterLight; }

	/// <summary>
	/// 距離減衰係数をセット
	/// </summary>
	/// <param name="lightatten">ライト距離減衰係数</param>
	inline void SetAtten(const Vec3& atten) { m_atten = atten; }

	/// <summary>
	/// 距離減衰係数を取得
	/// </summary>
	/// <returns>ライト距離減衰係数</returns>
	inline const Vec3& GetAtten() { return m_atten; }

	/// <summary>
	/// 減衰角度をセット
	/// </summary>
	/// <param name="lightFactorAngle">x:減衰開始角度 y:減衰終了角度[degree]</param>
	inline void SetFactorAngle(const Vec2& factorAngle) {
		m_factorAngleCos.x = cosf(DirectX::XMConvertToRadians(factorAngle.x));
		m_factorAngleCos.y = cosf(DirectX::XMConvertToRadians(factorAngle.y));
	}

	/// <summary>
	/// 減衰角度を取得
	/// </summary>
	/// <returns>減衰角度</returns>
	inline const Vec2& GetFactorAngleCos() { return m_factorAngleCos; }

	/// <summary>
	/// 有効フラグをセット
	/// </summary>
	/// <param name="active">有効フラグ</param>
	inline void SetActive(bool active) { m_active = active; }

	/// <summary>
	/// 有効チェック
	/// </summary>
	/// <returns>有効フラグ</returns>
	inline bool IsActive() { return m_active; }

private: // メンバ変数
	// 方向（単位ベクトル）
	XMVECTOR m_dir = { 1,0,0,0 };
	// キャスターとライトの距離
	float m_distanceCasterLight = 100.0f;
	// キャスター座標（ワールド座標系）
	Vec3 m_casterPos = { 0,0,0 };
	// 距離減衰係数
	Vec3 m_atten = { 0.5f, 0.6f, 0.0f };
	// 減衰角度
	Vec2 m_factorAngleCos = { 0.2f, 0.5f };
	// 有効フラグ
	bool m_active = false;
};

