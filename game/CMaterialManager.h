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
	inline int getTextureCount() { return mTextures.size(); }
	void createTextures();
	void setMaterial(const UINT &id);
	void destroy();
private:
	std::vector<Material> mMaterials;
	std::vector<Texture> mTexturepaths;
	std::vector<ID3D11ShaderResourceView*> mTextures;
	ID3D11SamplerState* mState;
	ID3D11Buffer *mCBuffer;
};
#endif