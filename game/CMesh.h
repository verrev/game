#ifndef _CMESH_H_
#define _CMESH_H_

#include <vector>
#include <fstream>
#include <D3Dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")
#include "CDirectX11.h"
#include "C:\Users\Vootele\Desktop\fbx2vmf\fbx2vmf\FileStructure.h"

class CMesh
{
public:
	static ID3D11InputLayout *getLayout(ID3D10Blob *s, const VERTEX_LAYOUT &vl, UINT &inputLayoutSize);
	void init();
	void draw();
	void destroy();
public:
	//std::vector<Vertex1P> mVertices1P;
	//std::vector<Vertex1P1N> mVertices1P1N;
	std::vector<Vertex1P1N1UV> mVertices1P1N1UV;

	UINT mVerticeCount, mInputLayoutSize;
	ID3D11InputLayout *mVertexLayout;
	ID3D11VertexShader *mVShader;
	ID3D11PixelShader *mPShader;
private:
	ID3D11Buffer *mVBuffer;
};
#endif