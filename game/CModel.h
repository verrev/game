#ifndef _CMODEL_H_
#define _CMODEL_H_
#include <fstream>
#include <vector>
#include "CMesh.h"
#include "CMaterialManager.h"

struct ModelCBuffer
{
	XMFLOAT4X4 mWorld;
};
class CModel
{
public:
	bool init(const std::string &fileName, const D3D11_FILL_MODE &fillMode = D3D11_FILL_SOLID);
	void draw();
	void setWorldMatrix(const XMMATRIX &world);
	void destroy();
private:
	std::vector<CMesh*> mMeshes;
	ModelCBuffer mCB;
	ID3D11Buffer *mCBuffer;
	ID3D11RasterizerState* mRasterizerState;
	CMaterialManager mMaterialManager;
};
#endif