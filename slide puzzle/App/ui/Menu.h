#include <Sprite.h>
#include<Audio.h>
/// <summary>
/// メニュークラス
/// </summary>
class Menu
{
private:
	//メニュー
	enum MenuType
	{
		MENURETRY,		//リトライ
		MENUVOLUM,		//ボリューム
		MENUSTAGESELECT,//ステージセレクト
		MENUMAX,		//最大
	};
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Menu();
	/// <summary>
	/// デスコンストラクタ
	/// </summary>
	~Menu();
	/// <summary>
	/// 初期化
	/// </summary>
	void Init();
	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="menuFlag">メニューフラグ</param>
	/// <param name="changeScene">チェンジシーンフラグ</param>
	/// <param name="sceneNum">チェンジする数字</param>
	void Update(bool& menuFlag, bool& changeScene, int& sceneNum);
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="gameoverFlag">ゲームオーバーフラグ</param>
	void Draw(const bool gameoverFlag);
	/// <summary>
	/// リセット
	/// </summary>
	void Reset();
private:
	/// <summary>
	/// メニューを開く処理
	/// </summary>
	void MenuOpen(bool& menuFlag);
	/// <summary>
	/// メニューの決定処理
	/// </summary>
	void MenuDecision(bool& menuFlag, int& sceneNum, bool& changeScene);
	/// <summary>
	/// 音の設定
	/// </summary>
	void AudioOpition();
	/// <summary>
	/// メニューが出るときのイージング処理
	/// </summary>
	void MenuEasing(const bool menuFlag);
	/// <summary>
	/// メニューの描画
	/// </summary>
	void DrawMenuDraw(const bool gameoverFlag);
	/// <summary>
	/// 音量調節描画
	/// </summary>
	void DrawAudioOption();
private:
	//メニュー関連
	SpriteData m_menuButton;
	SpriteData m_menuFream;
	SpriteData m_menuFreamFish;
	SpriteData m_menuRetry;
	SpriteData m_menuVolum;
	SpriteData m_menuStageSelect;
	SpriteData m_menuFish;
	int m_menuNum = 0;
	//メニューの強調
	bool m_menuTimeFlag = false;
	float m_menuScale = 1.0f;
	//イージング（メニュー）
	const Vec3 c_start = { 1500.0f,300.0f,0.0f };
	const Vec3 c_end = { 950.0f,300.0f,0.0f };
	Vec3 m_menuPos = { 0.0f,0.0f ,0.0f };
	bool m_menuEasingFlag = false;
	//出てくるとき
	const float c_startTimeMax = 20.0f;
	float m_startTime = 0.0f;

	//音設定
	SpriteData m_menuAudioFream;
	SpriteData m_menuAudioVolum;
	SpriteData m_menuAudioLandmark;
	//音量調整フラグ
	bool m_audioFlag = false;

	const float c_volumMax = 0.10f;
	const float c_volumMin = 0.0f;
	float m_volum = 0.02f;

	//音データ
	SoundData m_decisionSE = {};
	SoundData m_selectSE = {};

};