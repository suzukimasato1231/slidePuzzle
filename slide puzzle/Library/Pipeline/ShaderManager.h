#pragma once
#include <d3dcompiler.h>
#include <d3dx12.h>

using namespace Microsoft::WRL;
/// <summary>
/// シェーダークラス
/// </summary>
class Shader
{
public:
	ComPtr<ID3DBlob> vsBlob = nullptr; //頂点シェーダオブジェクト
	ComPtr<ID3DBlob> gsBlob = nullptr; //ジオメトリシェーダー
	ComPtr<ID3DBlob> psBlob = nullptr; //ピクセルシェーダオブジェクト
};

ComPtr<ID3DBlob> LoadShader(const LPCWSTR &VshaderName = L"", const LPCSTR &Vtarget = "", const LPCSTR &entryPoint = "main");

class ShaderManager
{
public:
	/// <summary>
	/// スプライト
	/// </summary>
	static Shader spriteShader;		//スプライト
	/// <summary>
	/// ポストエフェクト
	/// </summary>
	static Shader postNormalShader;	//ポストエフェクト（通常）
	static Shader postTestShader;	//ブルーム
	static Shader ShadowMapShader;	//影シェーダー
	static Shader DepthOfFieldShader;//被写界深度
	/// <summary>
	/// オブジェクト
	/// </summary>
	static Shader objShader;		//通常OBJ
	static Shader noShadowOBJShader;//影無しOBJ
	static Shader toonObjShader;	//トゥーンシェーダー
	static Shader normalMapShader;	//ノーマルマップ
	/// <summary>
	/// パーティクル
	/// </summary>
	static Shader particleShader;	//パーティクル

	static void LoadShaders();
};