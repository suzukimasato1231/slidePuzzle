#include "DebugText.h"

DebugText::DebugText()
{

}

DebugText::~DebugText()
{}

void DebugText::Initialize()
{
	//スプライト
	sprite = std::make_unique<Sprite>();

	if (sprite == nullptr)
	{
		assert(sprite);
	}
	sprite->Init();

	// 全てのスプライトデータについて
	for (int i = 0; i < _countof(m_sprites); i++)
	{
		// スプライトを生成する
		m_sprites[i] = sprite->SpriteCreate(L"Resources/debugfont.png");
	}
}
// 1文字列追加
void DebugText::Print(const float x, const  float y, const  float size,std::string text, ...)
{
	bool parsentFlag = false;
	va_list args;
	va_start(args, text);
	text.push_back(' ');
	for (size_t i = 0; text[i] != '\0'; ++i)
	{
		printf("%c:", text[i]);
		if (text[i] == '%')
		{
			parsentFlag = true;
		}
		if (parsentFlag == true)
		{
			int temp;
			double tempF = NULL;
			switch (text[i])
			{
			case 'd':
				temp = va_arg(args, int);
				text.replace((size_t)i - 1, 2, std::to_string(temp));
				parsentFlag = false;
				break;
			case 'f':
				tempF = va_arg(args, double);
				tempF = round(tempF * 100);
				tempF = tempF / 100;
				text.replace((size_t)i - 1, 2, std::to_string(tempF));
				parsentFlag = false;
				break;
			default:
				break;
			}
		}
	}
	va_end(args);

	// 全ての文字について
	for (size_t i = 0; text[i] != '\0'; i++)
	{
		// 最大文字数超過
		if (m_spriteIndex >= maxCharCount) {
			break;
		}

		// 1文字取り出す(※ASCIIコードでしか成り立たない)
		const unsigned char& character = text[i];

		int fontIndex = character - 32;
		if (character >= 0x7f) {
			fontIndex = 0;
		}

		int fontIndexY = fontIndex / fontLineCount;
		int fontIndexX = fontIndex % fontLineCount;



		// 座標計算
		m_sprites[m_spriteIndex].position = { x + fontWidth * size * i, y ,0 };

		m_sprites[m_spriteIndex].texLeftTop = { (float)fontIndexX * fontWidth,(float)fontIndexY * fontHeight };

		m_sprites[m_spriteIndex].texSize = { fontWidth,fontHeight };

		m_sprites[m_spriteIndex].size = { fontWidth * size, fontHeight * size };

		//頂点バッファ転送
		sprite->SpriteTransferVertexBuffer(m_sprites[m_spriteIndex]);

		//更新
		sprite->DebugUpdate(m_sprites[m_spriteIndex]);

		// 文字を１つ進める
		m_spriteIndex++;
	}
}

void DebugText::DrawAll()
{
	sprite->PreDraw();
	// 全ての文字のスプライトについて
	for (size_t i = 0; i < m_spriteIndex; i++)
	{
		// スプライト描画
		sprite->DebugDraw(m_sprites[i]);
	}
	m_spriteIndex = 0;
}

