#include "CLightManager.h"
bool CLightManager::init()
{
	Light l(LIGHT_TYPE::DIFFUSE, XMFLOAT4(1.0, 0.2, 0.2, 0.2), XMFLOAT4(1.0, 0.2, 0.2, 0.2), XMFLOAT3(0.0, 0.0, 1.0));
	mLights.push_back(l); // load from scene or sth
	D3D11_BUFFER_DESC cbd = { 0 };
	cbd.Usage = D3D11_USAGE_DEFAULT;
	cbd.ByteWidth = sizeof(Light);
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	CDirectX11::gDev->CreateBuffer(&cbd, 0, &mCBuffer);
	if (mLights.size()&&mCBuffer) return 1;
	return 0;
}
void CLightManager::setLight(const UINT &id)
{
	if (id < mLights.size()){
		CDirectX11::gDevCon->UpdateSubresource(mCBuffer, 0, 0, &mLights[id], 0, 0);
		CDirectX11::gDevCon->VSSetConstantBuffers(3, 1, &mCBuffer); // 0 - cam 1 - model 2 - material 3 - light
	}
}
void CLightManager::destroy()
{
	safeRelease(mCBuffer);
}
