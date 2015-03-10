#include "CMesh.h"
ID3D11InputLayout *CMesh::getLayout(ID3D10Blob *s, const VERTEX_LAYOUT &vl, UINT &inputLayoutSize)
{
	D3D11_INPUT_ELEMENT_DESC ied[VERTEX_LAYOUT::MAX_LAYOUT_ITEMS];
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
void CMesh::init()
{
	mVerticeCount = mVertices1P1N1UV.size();

	D3D11_BUFFER_DESC vbd = { 0 };
	vbd.Usage = D3D11_USAGE_DEFAULT;
	vbd.ByteWidth = mInputLayoutSize * mVerticeCount;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	D3D11_SUBRESOURCE_DATA vdata = { 0 };
	vdata.pSysMem = &mVertices1P1N1UV[0];
	CDirectX11::gDev->CreateBuffer(&vbd, &vdata, &mVBuffer);

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