#include "CSky.h"
bool CSky::init(const std::string &fileName,const std::string &textureName)
{
	D3DX11_IMAGE_LOAD_INFO loadSMInfo;
	loadSMInfo.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
	ID3D11Texture2D* SMTexture = 0;
	std::wstring ws;
	ws.assign(textureName.begin(), textureName.end());
	D3DX11CreateTextureFromFile(CDirectX11::gDev, ws.c_str(), &loadSMInfo, 0, (ID3D11Resource**)&SMTexture, 0);
	D3D11_TEXTURE2D_DESC SMTextureDesc;
	SMTexture->GetDesc(&SMTextureDesc);
	D3D11_SHADER_RESOURCE_VIEW_DESC SMViewDesc;
	SMViewDesc.Format = SMTextureDesc.Format;
	SMViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	SMViewDesc.TextureCube.MipLevels = SMTextureDesc.MipLevels;
	SMViewDesc.TextureCube.MostDetailedMip = 0;
	CDirectX11::gDev->CreateShaderResourceView(SMTexture, &SMViewDesc, &mTexture);
	safeRelease(SMTexture);
	D3D11_RASTERIZER_DESC rDesc;
	ZeroMemory(&rDesc, sizeof(D3D11_RASTERIZER_DESC));
	rDesc.FillMode = D3D11_FILL_SOLID;
	rDesc.CullMode = D3D11_CULL_NONE;
	CDirectX11::gDev->CreateRasterizerState(&rDesc, &mRasterizerState);
	D3D11_DEPTH_STENCIL_DESC dssDesc;
	ZeroMemory(&dssDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dssDesc.DepthEnable = true;
	dssDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dssDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	CDirectX11::gDev->CreateDepthStencilState(&dssDesc, &mDepthStencilState);

	std::ifstream inFile(fileName, std::ios::binary);
	if (inFile.good() && inFile.is_open()){
		D3D11_BUFFER_DESC cbd = { 0 };
		cbd.Usage = D3D11_USAGE_DEFAULT;
		cbd.ByteWidth = sizeof(ModelCBuffer);
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		CDirectX11::gDev->CreateBuffer(&cbd, 0, &mCBuffer);
		ModelHeader mh;
		inFile.read((char*)&mh, sizeof(ModelHeader));
		MeshHeader meshHeader;
		inFile.read((char*)&meshHeader, sizeof(MeshHeader));
		// INPUT LAYOUT NOT CREATED!
		createShaders(meshHeader.mVertexLayout, "models/sky_vs.cso", "models/sky_ps.cso");
		mVerticeCount = meshHeader.mVerticeCount;
		std::vector<Vertex1P1N1UV> vertices; vertices.resize(meshHeader.mVerticeCount);
		inFile.read((char*)&vertices[0], mVerticeCount * mInputLayoutSize);
		inFile.close();
		createVBuffer(&vertices[0]);

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
	}
	return 1;
}
void CSky::draw(const XMVECTOR &camPos)
{
	XMStoreFloat4x4(&mCB.mWorld, XMMatrixIdentity() * XMMatrixScaling(1, 1, 1) * XMMatrixTranslation(XMVectorGetX(camPos), XMVectorGetY(camPos), XMVectorGetZ(camPos)));
	CDirectX11::gDevCon->UpdateSubresource(mCBuffer, 0, 0, &mCB, 0, 0);
	CDirectX11::gDevCon->VSSetConstantBuffers(1, 1, &mCBuffer);  // 0 - cam 1 - model 

	CDirectX11::gDevCon->IASetInputLayout(mVertexLayout);
	CDirectX11::gDevCon->VSSetShader(mVShader, 0, 0);
	CDirectX11::gDevCon->PSSetShader(mPShader, 0, 0);
	CDirectX11::gDevCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	CDirectX11::gDevCon->PSSetSamplers(0, 1, &mState);
	CDirectX11::gDevCon->PSSetShaderResources(0, 1, &mTexture);
	CDirectX11::gDevCon->OMSetDepthStencilState(mDepthStencilState, 0);
	CDirectX11::gDevCon->RSSetState(mRasterizerState);
	UINT offset = 0;
	CDirectX11::gDevCon->IASetVertexBuffers(0, 1, &mVBuffer, &mInputLayoutSize, &offset);
	CDirectX11::gDevCon->Draw(mVerticeCount, 0);
	CDirectX11::gDevCon->OMSetDepthStencilState(nullptr, 0);
}
void CSky::destroy()
{
	safeRelease(mTexture);
	safeRelease(mDepthStencilState);
	safeRelease(mRasterizerState);
	safeRelease(mVertexLayout);
	safeRelease(mVShader);
	safeRelease(mPShader);
	safeRelease(mCBuffer);
	safeRelease(mVBuffer);
	safeRelease(mState);
}
ID3D11InputLayout *CSky::getLayout(ID3D10Blob *s, const VERTEX_LAYOUT &vl, UINT &inputLayoutSize)
{
	// THIS IS REPLICATED FROM CMESH!!
	// IT NEEDS TO BE MOVED TO ANOTHER FREESTANDING CLASS AS A WRAPPER AROUND DIRECTX11
	D3D11_INPUT_ELEMENT_DESC ied[MAX_LAYOUT_ITEMS];
	UINT index = 0;
	UINT size = 0;

	if (vl & VERTEX_LAYOUT::POSITION){
		ied[index].SemanticName = "POSITION";
		ied[index].SemanticIndex = 0;
		ied[index].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		ied[index].InputSlot = 0;
		ied[index].AlignedByteOffset = size;
		ied[index].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		ied[index++].InstanceDataStepRate = 0;
		size += sizeof(XMFLOAT3);
	}
	if (vl & VERTEX_LAYOUT::NORMAL){
		ied[index].SemanticName = "NORMAL";
		ied[index].SemanticIndex = 0;
		ied[index].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		ied[index].InputSlot = 0;
		ied[index].AlignedByteOffset = size;
		ied[index].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		ied[index++].InstanceDataStepRate = 0;
		size += sizeof(XMFLOAT3);
	}
	if (vl & VERTEX_LAYOUT::UVS){
		ied[index].SemanticName = "TEXCOORD";
		ied[index].SemanticIndex = 0;
		ied[index].Format = DXGI_FORMAT_R32G32_FLOAT;
		ied[index].InputSlot = 0;
		ied[index].AlignedByteOffset = size;
		ied[index].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		ied[index++].InstanceDataStepRate = 0;
		size += sizeof(XMFLOAT2);
	}
	inputLayoutSize = size;
	ID3D11InputLayout *vrtxLyt;
	CDirectX11::gDev->CreateInputLayout(ied, index, s->GetBufferPointer(), s->GetBufferSize(), &vrtxLyt);
	return vrtxLyt;
}
void CSky::createVBuffer(void *vertices)
{
	// THIS IS REPLICATED FROM CMESH!!!
	// IT NEEDS TO BE REMOVED TO ANOTHER FREESTANDING CLASS!
	D3D11_BUFFER_DESC vbd = { 0 };
	vbd.Usage = D3D11_USAGE_DEFAULT;
	vbd.ByteWidth = mInputLayoutSize * mVerticeCount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	D3D11_SUBRESOURCE_DATA vdata = { 0 };
	vdata.pSysMem = vertices;
	CDirectX11::gDev->CreateBuffer(&vbd, &vdata, &mVBuffer);
}
void CSky::createShaders(const VERTEX_LAYOUT &layout, const std::string &vsPath, const std::string &psPath)
{
	// THIS IS REPLICATED FROM CMESH!!
	// IT NEEDS TO BE MOVED TO ANOTHER FREESTANDING CLASS AS A WRAPPER AROUND DIRECTX11
	ID3D10Blob *sh;
	std::wstring ws;
	ws.assign(vsPath.begin(), vsPath.end());
	D3DReadFileToBlob(ws.c_str(), &sh);
	CDirectX11::gDev->CreateVertexShader(sh->GetBufferPointer(), sh->GetBufferSize(), nullptr, &mVShader);
	mVertexLayout = getLayout(sh, layout, mInputLayoutSize);
	ws.clear();
	ws.assign(psPath.begin(), psPath.end());
	D3DReadFileToBlob(ws.c_str(), &sh);
	CDirectX11::gDev->CreatePixelShader(sh->GetBufferPointer(), sh->GetBufferSize(), nullptr, &mPShader);
	safeRelease(sh);
}