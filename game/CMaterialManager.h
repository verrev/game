#ifndef _CMATERIALMANAGER_H_
#define _CMATERIALMANAGER_H_
#include "CDirectX11.h"
#include <string>
#include <fstream>
#include <vector>
#include "C:\Users\Vootele\Desktop\fbx2vmf\fbx2vmf\FileStructure.h"

class CMaterialManager
{
public:
	bool init(const std::string &fileName);
	void setMaterial(const UINT &id);
	void destroy();
private:
	std::vector<Material> mMaterials;
	ID3D11Buffer *mCBuffer;
};
#endif