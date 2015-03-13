#include "CSceneManager.h"
void CSceneManager::init(const int &w, const int &h)
{
	mCamera.init(w, h);
	mModels.push_back(new CModel);
	mModels[0]->init("models/panamera.vmf");
}
void CSceneManager::draw(const float &dt)
{
	update(dt);
	for (auto m : mModels){
		m->draw();
	}
}
void CSceneManager::update(const float &dt)
{
	float lr = 0, bf = 0;
	if (CInput::isKeyDown(DIK_W)) {
		bf = dt * 3.5f;
	}
	else if (CInput::isKeyDown(DIK_S)) {
		bf = -dt * 3.5f;
	}
	if (CInput::isKeyDown(DIK_A)) {
		lr = -dt * 3.5f;
	}
	else if (CInput::isKeyDown(DIK_D)) {
		lr = dt * 3.5f;
	}
	static float yaw = 0, pitch = 0;
	yaw += CInput::getMouseX() * dt * 0.8f;
	pitch += CInput::getMouseY() * dt * 0.8f;
	mCamera.update(lr, bf, yaw, pitch);
}
void CSceneManager::destroy()
{
	mCamera.destroy();
	for (UINT i = 0; i < mModels.size(); ++i){
		mModels[i]->destroy();
		delete mModels[i];
	}
}