#include "CModel.h"
bool CModel::init(const std::string &fileName,const D3D11_FILL_MODE &fillMode)
{
	// load materials file name from model file 
	std::string materialFileName = fileName;
	materialFileName.replace(materialFileName.size() - 3, 3, "vma");
	mMaterialManager.init(materialFileName);
	// 
	std::ifstream inFile(fileName, std::ios::binary);
	if (inFile.good() && inFile.is_open()){
		D3D11_BUFFER_DESC cbd = { 0 };
		cbd.Usage = D3D11_USAGE_DEFAULT;
		cbd.ByteWidth = sizeof(ModelCBuffer);
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		CDirectX11::gDev->CreateBuffer(&cbd, 0, &mCBuffer);
		setWorldMatrix(XMMatrixIdentity());
		
		D3D11_RASTERIZER_DESC rDesc;
		ZeroMemory(&rDesc, sizeof(D3D11_RASTERIZER_DESC));
		rDesc.FillMode = fillMode;
		rDesc.CullMode = D3D11_CULL_BACK;
		CDirectX11::gDev->CreateRasterizerState(&rDesc, &mRasterizerState);

		ModelHeader mh;
		inFile.read((char*)&mh, sizeof(ModelHeader));

		for (UINT i = 0; i < mh.mMeshCount; ++i){
			mMeshes.push_back(new CMesh);
			mMeshes[mMeshes.size() - 1]->init(inFile,&mMaterialManager);
		}
		inFile.close();
	}
	if (mMeshes.size()>0 && mCBuffer)
		return 1;
	return 0;
}
void CModel::setWorldMatrix(const XMMATRIX &world)
{
	XMStoreFloat4x4(&mCB.mWorld, XMMatrixTranspose(world));
}
void CModel::draw()
{
	CDirectX11::gDevCon->UpdateSubresource(mCBuffer, 0, 0, &mCB, 0, 0);
	CDirectX11::gDevCon->VSSetConstantBuffers(1, 1, &mCBuffer);  // 0 - cam 1 - model 
	CDirectX11::gDevCon->RSSetState(mRasterizerState);
	for (auto mesh : mMeshes){
		mesh->draw();
	}
}
void CModel::destroy()
{
	for (auto m : mMeshes){
		m->destroy();
		delete m;
	}
	safeRelease(mCBuffer);
	safeRelease(mRasterizerState);
	mMaterialManager.destroy();
}