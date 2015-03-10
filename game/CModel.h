#ifndef _CMODEL_H_
#define _CMODEL_H_
#include <fstream>
#include <vector>
#include "CMesh.h"

struct ModelCBuffer
{
	XMFLOAT4X4 mWorld;
};
class CModel
{
public:
	bool init(const std::string &fileName);
	void draw();
	void destroy();
private:
	std::vector<CMesh*> mMeshes;
	ModelCBuffer mCB;
	ID3D11Buffer *mCBuffer;
};
#endif