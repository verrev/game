#include "CModel.h"
bool CModel::init(const std::string &fileName)
{
	std::fstream inFile(fileName);
	if (inFile.good() && inFile.is_open()){
		D3D11_BUFFER_DESC cbd = { 0 };
		cbd.Usage = D3D11_USAGE_DEFAULT;
		cbd.ByteWidth = sizeof(ModelCBuffer);
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		CDirectX11::gDev->CreateBuffer(&cbd, 0, &mCBuffer);

		ModelHeader mh;
		inFile.read((char*)&mh, sizeof(ModelHeader));

		for (UINT i = 0; i < mh.mMeshCount; ++i){
			mMeshes.push_back(new CMesh);
			mMeshes[mMeshes.size() - 1]->init(inFile);
		}
		inFile.close();
	}
	if (mMeshes.size()>0 && mCBuffer)
		return 1;
	return 0;
}
void CModel::draw()
{
	static float y = 0; y += 0.001f;
	XMStoreFloat4x4(&mCB.mWorld, XMMatrixTranspose(XMMatrixIdentity()*XMMatrixRotationY(y)));
	CDirectX11::gDevCon->UpdateSubresource(mCBuffer, 0, 0, &mCB, 0, 0);
	CDirectX11::gDevCon->VSSetConstantBuffers(1, 1, &mCBuffer); // 0 - cam 1 - model
	for (auto mesh : mMeshes){
		mesh->draw();
	}
}
void CModel::destroy()
{
	for (UINT i = 0; i < mMeshes.size(); ++i){
		mMeshes[i]->destroy();
		delete mMeshes[i];
	}
	safeRelease(mCBuffer);
}