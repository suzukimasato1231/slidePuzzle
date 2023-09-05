#pragma once
#include "Sprite.h"
#include "Safe_delete.h"
#include <stdio.h>
#include <stdarg.h>
#include<memory>
extern const int window_width;
extern const int window_height;
/// <summary>
/// デバックテキスト
/// </summary>
class DebugText :public Singleton<DebugText>
{
	friend Singleton<DebugText>;
public:
	// デバッグテキスト用のテクスチャ番号を指定
	static const int maxCharCount = 256;	// 最大文字数
	static const int fontWidth = 9;			// フォント画像内1文字分の横幅
	static const int fontHeight = 18;		// フォント画像内1文字分の縦幅
	static const int fontLineCount = 14;	// フォント画像内1行分の文字数
	/// <summary>
	/// コンストラクタ
	/// </summary>
	DebugText();
	/// <summary>
	/// デスコンストラクタ
	/// </summary>
	~DebugText();
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	///  デバックテキスト
	/// </summary>
	/// <param name="x">X座標</param>
	/// <param name="y">Y座標</param>
	/// <param name="size">サイズ</param>
	/// <param name="text">表示する文字</param>
	/// <param name=""></param>
	void Print(const float x, const float y, const float size,std::string text, ...);
	/// <summary>
	/// デバックテキスト全描画
	/// </summary>
	void DrawAll();
private:
	std::unique_ptr<Sprite> sprite = nullptr;

	// スプライトデータの配列
	SpriteData m_sprites[maxCharCount] = {};
	// スプライトデータ配列の添え字番号
	int m_spriteIndex = 0;
};
