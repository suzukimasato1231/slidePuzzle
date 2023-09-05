#pragma once
#include <dinput.h>
#include <DirectXMath.h>
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
enum ButtonKind
{
	LButtonUp,
	LButtonDown,
	LButtonLeft,
	LButtonRight,
	LButtonUpS,
	LButtonDownS,
	LButtonLeftS,
	LButtonRightS,
	RButtonUp,
	RButtonDown,
	RButtonLeft,
	RButtonRight,
	ButtonA,
	ButtonB,
	ButtonX,
	ButtonY,
	ButtonRB,
	ButtonLB,
	ButtonRT,
	ButtonLT,
	ButtonMenu,
	ButtonKindMax,
};

enum ButtonState
{
	ButtonStateNone,
	ButtonStateDown,
	ButtonStatePush,
	ButtonStateUp,
	ButtonStateMax,
};
/// <summary>
/// コントロールクラス
/// </summary>
class Controller
{
private:
	// ゲームパッドの入力情報取得
	DIJOYSTATE pad_data = {};
	DIJOYSTATE oldpad_data={};
	LPDIRECTINPUTDEVICE8 g_GamePadDevice = nullptr;
	LPDIRECTINPUT8 g_InputInterface = nullptr;
public://ゲームパッド
	/// <summary>
	/// デバイス入力の初期化
	/// </summary>
	/// <returns></returns>
	bool InitController();

	/// <summary>
	/// Input終了関数
	/// </summary>
	void ReleaseInput();

	/// <summary>
	/// デバイスの更新
	/// </summary>
	void UpdateInput();

	/// <summary>
	/// ボタンが押されているかどうか
	/// </summary>
	/// <param name="button">ボタンの種類</param>
	/// <returns></returns>
	bool IsButtonPush(ButtonKind button)const;

	/// <summary>
	/// ボタンが押された瞬間かどうか
	/// </summary>
	/// <param name="button">ボタンの種類</param>
	/// <returns></returns>
	bool IsButtonUp(ButtonKind button)const;

	/// <summary>
	/// ボタンが離れた瞬間
	/// </summary>
	/// <param name="button">ボタンの種類</param>
	/// <returns></returns>
	bool IsButtonDown(ButtonKind button)const;

	/// <summary>
	/// 左パッド角度取得
	/// </summary>
	/// <returns></returns>
	float GetLeftAngle() const;
	/// <summary>
	/// 右スティック角度取得
	/// </summary>
	/// <returns></returns>
	float GetRightAngle() const;

};