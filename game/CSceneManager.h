#ifndef _CSCENEMANAGER_H_
#define _CSCENEMANAGER_H_
#include "CDirectX11.h"
#include "CInput.h"
#include "CTimer.h"
#include "CCamera.h"
#include "CModel.h"
#include <vector>
#include "CLightManager.h"
#include "CSky.h"

class CSceneManager
{
public:
	void init(const int &w, const int &h);
	void draw(const float &dt);
	void update(const float &dt);
	void destroy();
private:
	CCamera mCamera;
	CLightManager mLightManager;
	std::vector<CModel*>mModels;
	CSky mSky;
};
#endif