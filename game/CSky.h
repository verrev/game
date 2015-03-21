#ifndef _CSKY_H_
#define _CSKY_H_
#include <string>
#include <fstream>
#include "CDirectX11.h"
#include "CModel.h"
#include <D3Dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")
#include "C:\Users\Vootele\Desktop\fbx2vmf\fbx2vmf\FileStructure.h"
// FIND A BETTER WAY?
#include "C:\Program Files (x86)\Microsoft DirectX SDK (June 2010)\Include\d3dx11.h"
#pragma comment(lib,"C:\\Program Files (x86)\\Microsoft DirectX SDK (June 2010)\\Lib\\x86\\d3dx11.lib")
class CSky
{
public:
	bool init(const std::string &fileName, const std::string &textureName);
	void draw(const XMVECTOR &camPos);
	void destroy();
private:
	ID3D11InputLayout *getLayout(ID3D10Blob *s, const VERTEX_LAYOUT &vl, UINT &inputLayoutSize);
	void createVBuffer(void *vertices);
	void createShaders(const VERTEX_LAYOUT &layout, const std::string &vsPath, const std::string &psPath);
	UINT mVerticeCount, mInputLayoutSize;
private:
	ID3D11ShaderResourceView *mTexture;
	ID3D11DepthStencilState *mDepthStencilState;
	ID3D11SamplerState *mState;
	ID3D11RasterizerState *mRasterizerState;
	ID3D11InputLayout *mVertexLayout;
	ID3D11VertexShader *mVShader;
	ID3D11PixelShader *mPShader;
	ID3D11Buffer *mVBuffer;
	ID3D11Buffer *mCBuffer;
	ModelCBuffer mCB;
};
#endif