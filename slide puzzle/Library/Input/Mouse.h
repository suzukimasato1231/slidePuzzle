#pragma once
#include<dinput.h>
#include<wrl.h>
#include"Window.h"
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
/// <summary>
/// マウスクラス
/// </summary>
class Mouse
{
public:
	struct MouseMove {
		LONG    lX;
		LONG    lY;
		LONG    lZ;
	};
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public://メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns></returns>
	bool Init();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// キーの左ボタン押下をチェック
	/// </summary>
	/// <returns>押されているか</returns>
	bool PushMouseLeft()const;

	/// <summary>
	/// キーの中ボタン押下をチェック
	/// </summary>
	/// <returns>押されているか</returns>
	bool PushMouseMiddle()const;

	/// <summary>
	/// キーの左ボタントリガーをチェック
	/// </summary>
	/// <returns>トリガーか</returns>
	bool TriggerMouseLeft()const;

	/// <summary>
	/// キーの中ボタントリガーをチェック
	/// </summary>
	/// <returns>トリガーか</returns>
	bool TriggerMouseMiddle()const;

	/// <summary>
	/// マウス移動量を取得
	/// </summary>
	/// <returns>マウス移動量</returns>
	MouseMove GetMouseMove();
private:
	ComPtr<IDirectInput8> m_dinput = nullptr;
	ComPtr<IDirectInputDevice8> m_devMouse;
	DIMOUSESTATE2 m_mouseState = {};
	DIMOUSESTATE2 m_mouseStatePre = {};
};