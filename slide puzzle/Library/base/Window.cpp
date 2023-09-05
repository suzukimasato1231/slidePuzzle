#include "Window.h"

//ウィンドウプロシージャ
LRESULT WindowProce(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	//ウィンドウが破棄されたら呼ばれる
	if (msg == WM_DESTROY)
	{
		PostQuitMessage(0);//OSに対して「もうこのアプリは終わる」と伝える
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

void Window::CreateGameWindow()
{
	// ウィンドウサイズ{ X座標 Y座標 横幅 縦幅 }
	RECT wrc = { 0, 0, window_width, window_height };
	//ウィンドウクラスの設定
	m_w.cbSize = sizeof(WNDCLASSEX);
	m_w.lpfnWndProc = (WNDPROC)WindowProce; // ウィンドウプロシージャを設定
	m_w.lpszClassName = L"ペンギンブレーク"; // ウィンドウクラス名
	m_w.hInstance = GetModuleHandle(nullptr); // ウィンドウハンドル
	m_w.hCursor = LoadCursor(NULL, IDC_ARROW); // カーソル指定

	// ウィンドウクラスをOSに登録
	RegisterClassEx(&m_w);

	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false); // 自動でサイズ補正

	// ウィンドウオブジェクトの生成
	m_hwnd = CreateWindow(m_w.lpszClassName, // クラス名
		L"ペンギンブレーク",         // タイトルバーの文字
		WS_OVERLAPPEDWINDOW,        // 標準的なウィンドウスタイル
		CW_USEDEFAULT,              // 表示X座標（OSに任せる）
		CW_USEDEFAULT,              // 表示Y座標（OSに任せる）
		wrc.right - wrc.left,       // ウィンドウ横幅
		wrc.bottom - wrc.top,   // ウィンドウ縦幅
		nullptr,                // 親ウィンドウハンドル
		nullptr,                // メニューハンドル
		m_w.hInstance,            // 呼び出しアプリケーションハンドル
		nullptr);               // オプション

	// ウィンドウ表示
	ShowWindow(m_hwnd, SW_SHOW);
}


bool Window::ProcessMessage()
{
	MSG msg{};	// メッセージ

	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) // メッセージがある？
	{
		TranslateMessage(&msg); // キー入力メッセージの処理
		DispatchMessage(&msg); // ウィンドウプロシージャにメッセージを送る
	}

	if (msg.message == WM_QUIT) // 終了メッセージが来たらループを抜ける
	{
		return true;
	}
	return false;
}

void Window::TerminateGameWindow()
{
	// ウィンドウクラスを登録解除
	UnregisterClass(m_w.lpszClassName, m_w.hInstance);

}

