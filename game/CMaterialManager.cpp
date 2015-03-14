#include "CMaterialManager.h"
bool CMaterialManager::init(const std::string &fileName)
{
	std::ifstream inFile(fileName, std::ios::binary);
	if (inFile.good() && inFile.is_open()){
		MaterialHeader mh;
		inFile.read((char*)&mh, sizeof(MaterialHeader));
		mMaterials.resize(mh.mMaterialCount);
		inFile.read((char*)&mMaterials[0], sizeof(Material) * mh.mMaterialCount);
		inFile.close();
	}
	D3D11_BUFFER_DESC cbd = { 0 };
	cbd.Usage = D3D11_USAGE_DEFAULT;
	cbd.ByteWidth = sizeof(Material);
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	CDirectX11::gDev->CreateBuffer(&cbd, 0, &mCBuffer);

	if (mMaterials.size()>0&&mCBuffer) return 1;
	return 0;
}
void CMaterialManager::setMaterial(const UINT &id)
{
	// set textures here!
	if (id < mMaterials.size()){
		CDirectX11::gDevCon->UpdateSubresource(mCBuffer, 0, 0, &mMaterials[id], 0, 0);
		CDirectX11::gDevCon->VSSetConstantBuffers(2, 1, &mCBuffer);  // 0 - cam 1 - model 2 - material 3 - light
	}
}
void CMaterialManager::destroy()
{
	// release textures here!
	safeRelease(mCBuffer);
}
