#pragma once

#include <DirectXMath.h>
#include "Vec.h"
/// <summary>
/// 平行光源
/// </summary>
class DirectionalLight
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
		Vec3 lightcolor;
		unsigned int active;
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
	// ライト色
	Vec3 m_lightcolor = { 1,1,1 };
	// 有効フラグ
	bool m_active = false;
};

