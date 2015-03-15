#include "CMaterialManager.h"
// FIND A BETTER WAY?
#include "C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Include\d3dx11.h"
#pragma comment(lib,"C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Lib\\x86\\d3dx11.lib")

bool CMaterialManager::init(const std::string &fileName)
{
	std::ifstream inFile(fileName, std::ios::binary);
	if (inFile.good() && inFile.is_open()){
		MaterialHeader mh;
		inFile.read((char*)&mh, sizeof(MaterialHeader));
		mMaterials.resize(mh.mMaterialCount);
		inFile.read((char*)&mMaterials[0], sizeof(Material) * mh.mMaterialCount);
		TextureHeader th;
		inFile.read((char*)&th, sizeof(TextureHeader));
		mTexturepaths.resize(th.mTextureCount);
		if (mTexturepaths.size())
			inFile.read((char*)&mTexturepaths[0], sizeof(Texture) * th.mTextureCount);
		inFile.close();
		D3D11_BUFFER_DESC cbd = { 0 };
		cbd.Usage = D3D11_USAGE_DEFAULT;
		cbd.ByteWidth = sizeof(Material);
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		HRESULT hr = CDirectX11::gDev->CreateBuffer(&cbd, 0, &mCBuffer); 
		createTextures(); // EXPERIMENTAL!
	}
	if (mMaterials.size()>0&&mCBuffer) return 1;
	return 0;
}
void CMaterialManager::createTextures()
{
	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	CDirectX11::gDev->CreateSamplerState(&samplerDesc, &mState);

	mTextures.resize(mTexturepaths.size());
	for (UINT i = 0; i < mTextures.size(); ++i){
		std::string s = "models/" + std::string(mTexturepaths[i].mPath);
		std::wstring ws;
		ws.assign(s.begin(), s.end());
		D3DX11CreateShaderResourceViewFromFile(CDirectX11::gDev, ws.c_str(), nullptr, nullptr, &mTextures[i], nullptr);
	}
}
void CMaterialManager::setMaterial(const UINT &id)
{
	if (id < 0){ //reset the texs
		CDirectX11::gDevCon->PSSetSamplers(0, 1, &mState);
		CDirectX11::gDevCon->PSSetShaderResources(0, 1, nullptr);
		CDirectX11::gDevCon->UpdateSubresource(mCBuffer, 0, 0, nullptr, 0, 0);
		CDirectX11::gDevCon->PSSetConstantBuffers(0, 1, &mCBuffer);  
	}
	if (id < mTextures.size()){
		CDirectX11::gDevCon->PSSetSamplers(0, 1, &mState);
		CDirectX11::gDevCon->PSSetShaderResources(0, 1, &mTextures[id]);
	}
	if (id < mMaterials.size()){
		CDirectX11::gDevCon->UpdateSubresource(mCBuffer, 0, 0, &mMaterials[id], 0, 0);
		CDirectX11::gDevCon->PSSetConstantBuffers(0, 1, &mCBuffer);  // 0 - material 1 - light
	}
	
}
void CMaterialManager::destroy()
{
	safeRelease(mCBuffer);
	safeRelease(mState);
	for (auto t : mTextures){
		safeRelease(t);
	}
}
