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
	bool init(std::istream &inFile);
	void createShaders(const VERTEX_LAYOUT &layout, const std::string &vsPath, const std::string &psPath);
	void createVBuffer(void *vertices);
	void draw();
	void destroy();
private:
	UINT mVerticeCount, mInputLayoutSize;
	ID3D11InputLayout *mVertexLayout;
	ID3D11VertexShader *mVShader;
	ID3D11PixelShader *mPShader;
	ID3D11Buffer *mVBuffer;
};
#endif