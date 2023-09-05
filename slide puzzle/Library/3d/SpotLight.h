#pragma once

#include <DirectXMath.h>
#include "Vec.h"
/// <summary>
/// スポットライト
/// </summary>
class SpotLight
{
private: // エイリアス
	// DirectX::を省略
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public: // サブクラス

	// 定数バッファ用データ構造体
	struct ConstBufferData
	{
		XMVECTOR lightv;
		Vec3 lightpos;
		float pad1;
		Vec3 lightcolor;
		float pad2;
		Vec3 lightatten;
		float pad3;
		Vec2 lightfactoranglecos;
		unsigned int active;
		float pad4;
	};

public: // メンバ関数
	/// <summary>
	/// ライト方向をセット
	/// </summary>
	/// <param name="lightdir">ライト方向</param>
	inline void SetLightDir(const XMVECTOR& lightdir) { m_lightdir = DirectX::XMVector3Normalize(lightdir); }

	/// <summary>
	/// ライト方向を取得
	/// </summary>
	/// <returns>ライト方向</returns>
	inline const XMVECTOR& GetLightDir() { return m_lightdir; }

	/// <summary>
	/// ライト座標をセット
	/// </summary>
	/// <param name="lightpos">ライト座標</param>
	inline void SetLightPos(const Vec3& lightpos) { m_lightpos = lightpos; }

	/// <summary>
	/// ライト座標を取得
	/// </summary>
	/// <returns>ライト座標</returns>
	inline const Vec3& GetLightPos() { return m_lightpos; }

	/// <summary>
	/// ライト色をセット
	/// </summary>
	/// <param name="lightcolor">ライト色</param>
	inline void SetLightColor(const Vec3& lightcolor) { m_lightcolor = lightcolor; }

	/// <summary>
	/// ライト色を取得
	/// </summary>
	/// <returns>ライト色</returns>
	inline const Vec3& GetLightColor() { return m_lightcolor; }

	/// <summary>
	/// ライト距離減衰係数をセット
	/// </summary>
	/// <param name="lightatten">ライト距離減衰係数</param>
	inline void SetLightAtten(const Vec3& lightAtten) { m_lightAtten = lightAtten; }

	/// <summary>
	/// ライト距離減衰係数を取得
	/// </summary>
	/// <returns>ライト距離減衰係数</returns>
	inline const Vec3& GetLightAtten() { return m_lightAtten; }

	/// <summary>
	/// ライト減衰角度をセット
	/// </summary>
	/// <param name="lightFactorAngle">x:減衰開始角度 y:減衰終了角度[degree]</param>
	inline void SetLightFactorAngle(const Vec2& lightFactorAngle) { 
		m_lightFactorAngleCos.x = cosf(DirectX::XMConvertToRadians(lightFactorAngle.x));
		m_lightFactorAngleCos.y = cosf(DirectX::XMConvertToRadians(lightFactorAngle.y));
	}

	/// <summary>
	/// ライト減衰角度を取得
	/// </summary>
	/// <returns>ライト距離減衰係数</returns>
	inline const Vec2& GetLightFactorAngleCos() { return m_lightFactorAngleCos; }

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
	// ライト方向（単位ベクトル）
	XMVECTOR m_lightdir = { 1,0,0,0 };
	// ライト座標（ワールド座標系）
	Vec3 m_lightpos = { 0,0,0 };
	// ライト色
	Vec3 m_lightcolor = { 1,1,1 };
	// ライト距離減衰係数
	Vec3 m_lightAtten = { 1.0f, 1.0f, 1.0f };
	// ライト減衰角度
	Vec2 m_lightFactorAngleCos = { 0.2f, 0.5f };
	// 有効フラグ
	bool m_active = false;
};

