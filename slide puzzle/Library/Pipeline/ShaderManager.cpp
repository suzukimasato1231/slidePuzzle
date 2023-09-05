#include "ShaderManager.h"

ComPtr<ID3DBlob> LoadShader(const LPCWSTR &VshaderName, const LPCSTR &Vtarget, const LPCSTR &entryPoint)
{
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> Blob;
	ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト

	if (VshaderName != L"")
	{
		// 頂点シェーダの読み込みとコンパイル
		result = D3DCompileFromFile(VshaderName,
			nullptr,
			D3D_COMPILE_STANDARD_FILE_INCLUDE,
			entryPoint, Vtarget, D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
			0, &Blob, &errorBlob);
		if (FAILED(result)) {
			std::string errstr;
			errstr.resize(errorBlob->GetBufferSize());

			std::copy_n((char*)errorBlob->GetBufferPointer(),
				errorBlob->GetBufferSize(),
				errstr.begin());
			errstr += "\n";
			OutputDebugStringA(errstr.c_str());
			exit(1);
		}
	}
	return Blob;
}

Shader ShaderManager::spriteShader;
Shader ShaderManager::objShader;
Shader ShaderManager::toonObjShader;
Shader ShaderManager::normalMapShader;
Shader ShaderManager::particleShader;
Shader ShaderManager::postNormalShader;
Shader ShaderManager::postTestShader;
Shader ShaderManager::ShadowMapShader;
Shader ShaderManager::DepthOfFieldShader;
Shader ShaderManager::noShadowOBJShader;
void ShaderManager::LoadShaders()
{	
	//スプライト
	spriteShader.vsBlob = LoadShader(L"Resources/shaders/SpriteVS.hlsl", "vs_5_0");
	spriteShader.psBlob = LoadShader(L"Resources/shaders/SpritePS.hlsl", "ps_5_0");
	//OBJ
	objShader.vsBlob = LoadShader(L"Resources/shaders/Object/OBJVertexShader.hlsl", "vs_5_0");
	objShader.psBlob = LoadShader(L"Resources/shaders/Object/OBJPixelShader.hlsl", "ps_5_0");
	//影無しOBJ
	noShadowOBJShader.vsBlob = LoadShader(L"Resources/shaders/Object/NoShadowOBJVS.hlsl", "vs_5_0");
	noShadowOBJShader.psBlob = LoadShader(L"Resources/shaders/Object/NoShadowOBJPS.hlsl", "ps_5_0");
	//トゥーンシェーダー
	toonObjShader.vsBlob = LoadShader(L"Resources/shaders/Object/OBJToonVS.hlsl", "vs_5_0");
	toonObjShader.psBlob = LoadShader(L"Resources/shaders/Object/OBJToonPS.hlsl", "ps_5_0");
	//ノーマルマップ
	normalMapShader.vsBlob = LoadShader(L"Resources/shaders/Object/NormalMapVS.hlsl","vs_5_0");
	normalMapShader.psBlob = LoadShader(L"Resources/shaders/Object/NormalMapPS.hlsl", "ps_5_0");
	//パーティクル
	particleShader.vsBlob = LoadShader(L"Resources/shaders/ParticleVS.hlsl", "vs_5_0");
	particleShader.psBlob = LoadShader(L"Resources/shaders/ParticlePS.hlsl", "ps_5_0");
	particleShader.gsBlob = LoadShader(L"Resources/shaders/ParticleGS.hlsl", "gs_5_0");
	//ポストエフェクト通常
	postNormalShader.vsBlob = LoadShader(L"Resources/shaders/PostEffect/PostEffectNormalVS.hlsl", "vs_5_0");
	postNormalShader.psBlob = LoadShader(L"Resources/shaders/PostEffect/PostEffectNormalPS.hlsl", "ps_5_0");
	//ブルーム
	postTestShader.vsBlob = LoadShader(L"Resources/shaders/PostEffect/PostEffectTestVS.hlsl", "vs_5_0");
	postTestShader.psBlob = LoadShader(L"Resources/shaders/PostEffect/PostEffectTestPS.hlsl", "ps_5_0");
	//シャドウマップ
	ShadowMapShader.vsBlob = LoadShader(L"Resources/shaders/ShadowMapVS.hlsl", "vs_5_0");
	ShadowMapShader.psBlob = LoadShader(L"Resources/shaders/ShadowMapPS.hlsl", "ps_5_0");
	//被写界深度
	DepthOfFieldShader.vsBlob= LoadShader(L"Resources/shaders/PostEffect/DepthOfFieldVS.hlsl", "vs_5_0");;
	DepthOfFieldShader.psBlob = LoadShader(L"Resources/shaders/PostEffect/DepthOfFieldPS.hlsl", "ps_5_0");
}
