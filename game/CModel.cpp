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

			MeshHeader meshHeader;
			inFile.read((char*)&meshHeader, sizeof(MeshHeader));

			auto m = mMeshes[mMeshes.size() - 1];
			
			m->mVertices1P1N1UV.resize(meshHeader.mVerticeCount);
			ID3D10Blob *sh;
			std::string s = "models/";
			s.append("vs.cso");
			std::wstring ws;
			ws.assign(s.begin(), s.end());
			D3DReadFileToBlob(ws.c_str(), &sh);
			CDirectX11::gDev->CreateVertexShader(sh->GetBufferPointer(), sh->GetBufferSize(), nullptr, &m->mVShader);
			m->mVertexLayout = CMesh::getLayout(sh, meshHeader.mVertexLayout, m->mInputLayoutSize);

			s = "models/";
			s.append("ps.cso");
			ws.clear();
			ws.assign(s.begin(), s.end());
			D3DReadFileToBlob(ws.c_str(), &sh);
			CDirectX11::gDev->CreatePixelShader(sh->GetBufferPointer(), sh->GetBufferSize(), nullptr, &m->mPShader);
			safeRelease(sh);

			inFile.read((char*)&m->mVertices1P1N1UV[0], m->mVertices1P1N1UV.size() * m->mInputLayoutSize);

			m->init();

		}
		inFile.close();
		return 1;
	}
	return 0;
}
void CModel::draw()
{
	XMStoreFloat4x4(&mCB.mWorld, XMMatrixTranspose(XMMatrixIdentity()));
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