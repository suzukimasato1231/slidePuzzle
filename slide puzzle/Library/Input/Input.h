#pragma once
#include"Controller.h"
#include "Keybord.h"
#include"Mouse.h"
#include"Singleton.h"
/// <summary>
/// 入力クラス
/// </summary>
class Input :public Singleton<Input>
{
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	friend Singleton<Input>;
private://メンバ変数
	//キーボード
	Keybord m_keybord;
	//コントローラー
	Controller m_controller;
	//マウス
	Mouse m_mouse;
public://静的メンバ変数
	//static Input *GetInstance();
public://メンバ関数
	Input() = default;
	Input(const Input &) = delete;
	~Input() = default;
	Input &operator=(const Input &) = delete;
	//初期化
	void Initialize();
	//更新
	void Update();
public://キーボード
	//押してる時
	bool KeybordPush(BYTE keyNumber)const;
	//押した瞬間
	bool KeybordTrigger(BYTE keyNumber)const;
	//矢印キーのいずれかを押したかどうか
	bool KeybordInputArrow()const;
public://ゲームパッド

	//@brief ボタンが押されているかの判定関数
	// @param[in] button 判定したいボタンの種類
	bool ControllerPush(ButtonKind button)const;

	// @brief ボタンが押された瞬間の判定関数
	// @param[in] button 判定したいボタンの種類
	bool ControllerUp(ButtonKind button)const;

	// @brief ボタンを離した瞬間の判定関数
	// @param[in] button 判定したいボタンの種類
	bool ControllerDown(ButtonKind button)const;
	//左スティック角度取得
	float GetLeftAngle()const;

	float GetRightAngle()const;
	//右スティック入力してるか
	bool ConRightInput()const;

	bool ConLeftInputS()const;

	//左スティックを入力してるか
	bool ConLeftInput()const;
public://マウス
	bool MousePushLeft();

	/// <summary>
	/// キーの中ボタン押下をチェック
	/// </summary>
	/// <returns>押されているか</returns>
	bool MousePushMiddle();

	/// <summary>
	/// キーの左ボタントリガーをチェック
	/// </summary>
	/// <returns>トリガーか</returns>
	bool MouseTriggerLeft();

	/// <summary>
	/// キーの中ボタントリガーをチェック
	/// </summary>
	/// <returns>トリガーか</returns>
	bool MouseTriggerMiddle();

	/// <summary>
	/// マウス移動量を取得
	/// </summary>
	/// <returns>マウス移動量</returns>
	Mouse::MouseMove GetMouseMove();
};