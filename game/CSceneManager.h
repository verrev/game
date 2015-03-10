#ifndef _CSCENEMANAGER_H_
#define _CSCENEMANAGER_H_
#include "CDirectX11.h"
#include "CInput.h"
#include "CTimer.h"
#include "CCamera.h"
#include "CModel.h"
#include <vector>

class CSceneManager
{
public:
	void init(const int &w, const int &h);
	void draw(const float &dt);
	void update(const float &dt);
	void destroy();
private:
	CCamera mCamera;
	std::vector<CModel*>mModels;
};
#endif