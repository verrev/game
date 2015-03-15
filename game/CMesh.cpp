#include "CMesh.h"
ID3D11InputLayout *CMesh::getLayout(ID3D10Blob *s, const VERTEX_LAYOUT &vl, UINT &inputLayoutSize)
{
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
void CMesh::createShaders(const VERTEX_LAYOUT &layout, const std::string &vsPath, const std::string &psPath)
{
	ID3D10Blob *sh;
	std::wstring ws;
	ws.assign(vsPath.begin(), vsPath.end());
	D3DReadFileToBlob(ws.c_str(), &sh);
	CDirectX11::gDev->CreateVertexShader(sh->GetBufferPointer(), sh->GetBufferSize(), nullptr, &mVShader);
	mVertexLayout = CMesh::getLayout(sh, layout, mInputLayoutSize);
	ws.clear();
	ws.assign(psPath.begin(), psPath.end());
	D3DReadFileToBlob(ws.c_str(), &sh);
	CDirectX11::gDev->CreatePixelShader(sh->GetBufferPointer(), sh->GetBufferSize(), nullptr, &mPShader);
	safeRelease(sh);
}
void CMesh::createVBuffer(void *vertices)
{
	D3D11_BUFFER_DESC vbd = { 0 };
	vbd.Usage = D3D11_USAGE_DEFAULT;
	vbd.ByteWidth = mInputLayoutSize * mVerticeCount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	D3D11_SUBRESOURCE_DATA vdata = { 0 };
	vdata.pSysMem = vertices;
	CDirectX11::gDev->CreateBuffer(&vbd, &vdata, &mVBuffer);
}
bool CMesh::init(std::istream &inFile)
{
	MeshHeader meshHeader;
	inFile.read((char*)&meshHeader, sizeof(MeshHeader));
	mVerticeCount = meshHeader.mVerticeCount;
	void *vertices = nullptr;

	std::vector<Vertex1P> vertices1P;
	std::vector<Vertex1P1N> vertices1P1N;
	std::vector<Vertex1P1N1UV> vertices1P1N1UV;

	if ((meshHeader.mVertexLayout & VERTEX_LAYOUT::POSITION)
		&& (meshHeader.mVertexLayout & VERTEX_LAYOUT::NORMAL)
		&& (meshHeader.mVertexLayout & VERTEX_LAYOUT::UVS)) {
		vertices1P1N1UV.resize(mVerticeCount);
		createShaders(meshHeader.mVertexLayout, "models/vs.cso", "models/ps.cso");
		inFile.read((char*)&vertices1P1N1UV[0], mVerticeCount * mInputLayoutSize);
		vertices = &vertices1P1N1UV[0];
	}
	else if ((meshHeader.mVertexLayout & VERTEX_LAYOUT::POSITION)
		&& (meshHeader.mVertexLayout & VERTEX_LAYOUT::NORMAL)) {
		vertices1P1N.resize(mVerticeCount);
		createShaders(meshHeader.mVertexLayout, "models/vs.cso", "models/ps.cso");
		inFile.read((char*)&vertices1P1N[0], mVerticeCount * mInputLayoutSize);
		vertices = &vertices1P1N[0];
	}
	else if ((meshHeader.mVertexLayout & VERTEX_LAYOUT::POSITION)) {
		vertices1P.resize(mVerticeCount);
		createShaders(meshHeader.mVertexLayout, "models/vs.cso", "models/ps.cso");
		inFile.read((char*)&vertices1P[0], mVerticeCount * mInputLayoutSize);
		vertices = &vertices1P[0]; 
	}
	createVBuffer(vertices);
	if (mVBuffer&&mVertexLayout&&mVShader&&mPShader&&mVBuffer)
		return 1;
	return 0;
}
void CMesh::draw()
{
	CDirectX11::gDevCon->IASetInputLayout(mVertexLayout);
	CDirectX11::gDevCon->VSSetShader(mVShader, 0, 0);
	CDirectX11::gDevCon->PSSetShader(mPShader, 0, 0);
	CDirectX11::gDevCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	UINT offset = 0;
	CDirectX11::gDevCon->IASetVertexBuffers(0, 1, &mVBuffer, &mInputLayoutSize, &offset);
	CDirectX11::gDevCon->Draw(mVerticeCount, 0);
}
void CMesh::destroy()
{
	safeRelease(mVBuffer);
	safeRelease(mVShader);
	safeRelease(mPShader);
	safeRelease(mVertexLayout);
}