#include "LightGroup.h"
#include <assert.h>
#include"Camera.h"
#include"Input.h"
using namespace DirectX;

/// <summary>
/// 静的メンバ変数の実体
/// </summary>
ID3D12Device* LightGroup::device = nullptr;

void LightGroup::StaticInitialize(ID3D12Device* device)
{
	// 再初期化チェック
	assert(!LightGroup::device);

	// nullptrチェック
	assert(device);

	LightGroup::device = device;
}

LightGroup* LightGroup::Create()
{
	// 3Dオブジェクトのインスタンスを生成
	LightGroup* instance = new LightGroup();

	// 初期化
	instance->Initialize();

	return instance;
}

void LightGroup::Initialize()
{
	// nullptrチェック
	assert(device);

	DefaultLightSetting();

	HRESULT result;
	// 定数バッファの生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&m_constBuff));
#ifdef _DEBUG
	m_constBuff->SetName(L"Light");
#endif
	if (FAILED(result)) {
		assert(0);
	}
	// 定数バッファへデータ転送
	TransferConstBuffer();

	m_shadowProjection = XMMatrixOrthographicOffCenterLH(
		-100.0f, 450.0f,
		-550.0f, 500.0f,
		-350.0f, 350.0f);//前端　奥端
}

void LightGroup::Update()
{
	////影用
	Vec3 target = Vec3(Camera::Get()->GetEye().x - 90, Camera::Get()->GetEye().y - 100, Camera::Get()->GetEye().z);
	Vec3 eye = Camera::Get()->GetTarget() + m_shadowDir.normalize() * Vec3(Camera::Get()->GetTarget() - Camera::Get()->GetEye()).length();
	Vec3 up = { 0,1,0 };


	m_matView = XMMatrixLookAtLH(
		XMLoadFloat3(&eye),
		XMLoadFloat3(&target),
		XMLoadFloat3(&up));

	m_lightMatViewProjection = m_matView * m_shadowProjection;


	// 値の更新があった時だけ定数バッファに転送する
	if (m_dirty) {
		TransferConstBuffer();
		m_dirty = false;
	}
}

void LightGroup::Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex)
{
	// 定数バッファビューをセット
	cmdList->SetGraphicsRootConstantBufferView(rootParameterIndex, m_constBuff->GetGPUVirtualAddress());
}

void LightGroup::TransferConstBuffer()
{
	HRESULT result;

	// 定数バッファへデータ転送
	ConstBufferData* constMap = nullptr;
	result = m_constBuff->Map(0, nullptr, (void**)&constMap);
	if (SUCCEEDED(result)) {
		// 環境光
		constMap->ambientColor = m_ambientColor;
		// 平行光源
		for (int i = 0; i < DirLightNum; i++) {
			// ライトが有効なら設定を転送
			if (m_dirLights[i].IsActive()) {
				constMap->dirLights[i].active = 1;
				constMap->dirLights[i].lightv = -m_dirLights[i].GetLightDir();
				constMap->dirLights[i].lightcolor = m_dirLights[i].GetLightColor();
			}
			// ライトが無効ならライト色を0に
			else {
				constMap->dirLights[i].active = 0;
			}
		}
		// 点光源
		for (int i = 0; i < PointLightNum; i++) {
			// ライトが有効なら設定を転送
			if (m_pointLights[i].IsActive()) {
				constMap->pointLights[i].active = 1;
				constMap->pointLights[i].lightpos = m_pointLights[i].GetLightPos();
				constMap->pointLights[i].lightcolor = m_pointLights[i].GetLightColor();
				constMap->pointLights[i].lightatten = m_pointLights[i].GetLightAtten();
			}
			// ライトが無効ならライト色を0に
			else {
				constMap->pointLights[i].active = 0;
			}
		}
		// スポットライト
		for (int i = 0; i < SpotLightNum; i++) {
			// ライトが有効なら設定を転送
			if (m_spotLights[i].IsActive()) {
				constMap->spotLights[i].active = 1;
				constMap->spotLights[i].lightv = -m_spotLights[i].GetLightDir();
				constMap->spotLights[i].lightpos = m_spotLights[i].GetLightPos();
				constMap->spotLights[i].lightcolor = m_spotLights[i].GetLightColor();
				constMap->spotLights[i].lightatten = m_spotLights[i].GetLightAtten();
				constMap->spotLights[i].lightfactoranglecos = m_spotLights[i].GetLightFactorAngleCos();
			}
			// ライトが無効ならライト色を0に
			else {
				constMap->spotLights[i].active = 0;
			}
		}
		// 丸影
		for (int i = 0; i < CircleShadowNum; i++) {
			// 有効なら設定を転送
			if (m_circleShadows[i].IsActive()) {
				constMap->circleShadows[i].active = 1;
				constMap->circleShadows[i].dir = -m_circleShadows[i].GetDir();
				constMap->circleShadows[i].casterPos = m_circleShadows[i].GetCasterPos();
				constMap->circleShadows[i].distanceCasterLight = m_circleShadows[i].GetDistanceCasterLight();
				constMap->circleShadows[i].atten = m_circleShadows[i].GetAtten();
				constMap->circleShadows[i].factorAngleCos = m_circleShadows[i].GetFactorAngleCos();
			}
			// 無効なら色を0に
			else {
				constMap->circleShadows[i].active = 0;
			}
		}
		m_constBuff->Unmap(0, nullptr);
	}
}

void LightGroup::DefaultLightSetting()
{
	m_dirLights[0].SetActive(true);
	m_dirLights[0].SetLightColor({ 1.0f, 1.0f, 1.0f });
	m_dirLights[0].SetLightDir({ 1.0f, -1.0f, 0.0f, 0 });
}

void LightGroup::SetAmbientColor(const Vec3& color)
{
	m_ambientColor = color;
	m_dirty = true;
}

void LightGroup::SetDirLightActive(int index, bool active)
{
	assert(0 <= index && index < DirLightNum);

	m_dirLights[index].SetActive(active);

}

void LightGroup::SetDirLightDir(int index, const XMVECTOR& lightdir)
{
	assert(0 <= index && index < DirLightNum);

	m_dirLights[index].SetLightDir(lightdir);
	m_dirty = true;
}

void LightGroup::SetDirLightColor(int index, const Vec3& lightcolor)
{
	assert(0 <= index && index < DirLightNum);

	m_dirLights[index].SetLightColor(lightcolor);
	m_dirty = true;
}

void LightGroup::SetPointLightActive(int index, bool active)
{
	assert(0 <= index && index < PointLightNum);

	m_pointLights[index].SetActive(active);
	m_dirty = true;
}

void LightGroup::SetPointLightPos(int index, const Vec3& lightpos)
{
	assert(0 <= index && index < PointLightNum);

	m_pointLights[index].SetLightPos(lightpos);
	m_dirty = true;
}

void LightGroup::SetPointLightColor(int index, const Vec3& lightcolor)
{
	assert(0 <= index && index < PointLightNum);

	m_pointLights[index].SetLightColor(lightcolor);
	m_dirty = true;
}

void LightGroup::SetPointLightAtten(int index, const Vec3& lightAtten)
{
	assert(0 <= index && index < PointLightNum);

	m_pointLights[index].SetLightAtten(lightAtten);
	m_dirty = true;
}

void LightGroup::SetSpotLightActive(int index, bool active)
{
	assert(0 <= index && index < SpotLightNum);

	m_spotLights[index].SetActive(active);
	m_dirty = true;
}

void LightGroup::SetSpotLightDir(int index, const XMVECTOR& lightdir)
{
	assert(0 <= index && index < SpotLightNum);

	m_spotLights[index].SetLightDir(lightdir);
	m_dirty = true;
}

void LightGroup::SetSpotLightPos(int index, const Vec3& lightpos)
{
	assert(0 <= index && index < SpotLightNum);

	m_spotLights[index].SetLightPos(lightpos);
	m_dirty = true;
}

void LightGroup::SetSpotLightColor(int index, const Vec3& lightcolor)
{
	assert(0 <= index && index < SpotLightNum);

	m_spotLights[index].SetLightColor(lightcolor);
	m_dirty = true;
}

void LightGroup::SetSpotLightAtten(int index, const Vec3& lightAtten)
{
	assert(0 <= index && index < SpotLightNum);

	m_spotLights[index].SetLightAtten(lightAtten);
	m_dirty = true;
}

void LightGroup::SetSpotLightFactorAngle(int index, const Vec2& lightFactorAngle)
{
	assert(0 <= index && index < SpotLightNum);

	m_spotLights[index].SetLightFactorAngle(lightFactorAngle);
	m_dirty = true;
}

void LightGroup::SetCircleShadowActive(int index, bool active)
{
	assert(0 <= index && index < CircleShadowNum);

	m_circleShadows[index].SetActive(active);
	m_dirty = true;
}

void LightGroup::SetCircleShadowCasterPos(int index, const Vec3& casterPos)
{
	assert(0 <= index && index < CircleShadowNum);

	m_circleShadows[index].SetCasterPos(casterPos);
	m_dirty = true;
}

void LightGroup::SetCircleShadowDir(int index, const XMVECTOR& lightdir)
{
	assert(0 <= index && index < CircleShadowNum);

	m_circleShadows[index].SetDir(lightdir);
	m_dirty = true;
}

void LightGroup::SetCircleShadowDistanceCasterLight(int index, float distanceCasterLight)
{
	assert(0 <= index && index < CircleShadowNum);

	m_circleShadows[index].SetDistanceCasterLight(distanceCasterLight);
	m_dirty = true;
}

void LightGroup::SetCircleShadowAtten(int index, const Vec3& lightAtten)
{
	assert(0 <= index && index < CircleShadowNum);

	m_circleShadows[index].SetAtten(lightAtten);
	m_dirty = true;
}

void LightGroup::SetCircleShadowFactorAngle(int index, const Vec2& lightFactorAngle)
{
	assert(0 <= index && index < CircleShadowNum);

	m_circleShadows[index].SetFactorAngle(lightFactorAngle);
	m_dirty = true;
}

void LightGroup::SetShadowDir(const Vec3 shadowDir)
{
	m_shadowDir = shadowDir;
}
