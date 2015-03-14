#ifndef _CLIGHTMANAGER_H_
#define _CLIGHTMANAGER_H_
#include "CDirectX11.h"
#include <string>
#include <fstream>
#include <vector>
using namespace DirectX;

enum LIGHT_TYPE
{
	DIFFUSE
};
struct Light
{
	Light(const LIGHT_TYPE &type, const XMFLOAT4 &difCol, const XMFLOAT4 &ambCol, const XMFLOAT3 &difDir):
	mType(type),mDiffuseColor(difCol),mAmbientColor(ambCol),mDiffuseDir(difDir){}
	LIGHT_TYPE mType;
	XMFLOAT4 mDiffuseColor, mAmbientColor;
	XMFLOAT3 mDiffuseDir;
};
class CLightManager
{
public:
	bool init();
	void setLight(const UINT &id);
	void destroy();
private:
	std::vector<Light> mLights;
	ID3D11Buffer *mCBuffer;
};
#endif