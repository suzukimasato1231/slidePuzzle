#pragma once
//キーボード入力
#include<dinput.h>
#include<wrl.h>
#include <DirectXMath.h>
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#include"Window.h"
//#define DIRECTINPUT_VERSION 0x0800//DirectInputのバージョン指定
/// <summary>
/// キーボードクラス
/// </summary>
class Keybord
{
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
private://メンバ変数
	//全キーの入力状態を取得
	BYTE m_key[256]={};
	//1つ前の入力状態を取得
	BYTE m_oldkey[256]={};
	//キーボード入力
	ComPtr<IDirectInput8> m_dinput = nullptr;
	//キーボードデバイスの生成
	ComPtr<IDirectInputDevice8> m_devkeyboard = nullptr;
public://メンバ関数
	//初期化
	void Initialize();
	//更新
	void Update();

	//押した瞬間
	bool PushKey(BYTE keyNumber)const;
	//離した瞬間
	bool TriggerKey(BYTE keyNumber)const;
};