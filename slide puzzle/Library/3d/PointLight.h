#pragma once

#include <DirectXMath.h>
#include "Vec.h"
/// <summary>
/// 点光源
/// </summary>
class PointLight
{
private: // エイリアス
	// DirectX::を省略
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;
public: // サブクラス

	// 定数バッファ用データ構造体
	struct ConstBufferData
	{
		Vec3 lightpos;
		float pad1;
		Vec3 lightcolor;
		float pad2;
		Vec3 lightatten;
		unsigned int active;
	};

public: // メンバ関数
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
	// ライト座標（ワールド座標系）
	Vec3 m_lightpos = { 0.5f,1.0f,0 };
	// ライト色
	Vec3 m_lightcolor = { 1,1,1 };
	// ライト距離減衰係数
	Vec3 m_lightAtten = { 1.0f, 1.0f, 1.0f };
	// 有効フラグ
	bool m_active = false;
};

