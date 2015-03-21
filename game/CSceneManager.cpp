#include "CSceneManager.h"
void CSceneManager::init(const int &w, const int &h)
{
	/*
	TODO: ADD FAILSAFES FOR RESOURCE LOADING (SHADERS, MODELS (KIND-OF ALREADY EXISTS)...)
	*/
	mCamera.init(w, h);
	mSky.init("models/box.vmf","models/skymap.dds");
	mLightManager.init();
	mLightManager.setLight(0); // modulate here

	mModels.push_back(new CModel);
	mModels[0]->init("models/HOUSE2.vmf");
	/*
	mModels.push_back(new CModel);
	mModels[1]->init("models/sphere.vmf");
	mModels.push_back(new CModel);
	mModels[2]->init("models/box.vmf");
	mModels.push_back(new CModel);
	mModels[3]->init("models/panamera.vmf");
	*/
}
void CSceneManager::draw(const float &dt)
{
	/*
	Make it so the rasterizer state is loaded from the file.
	*/

	update(dt);
	mSky.draw(mCamera.getPos());
	//mSky.draw(mCamera);
	/*
	for (int i = -5; i < 5; ++i){
		for (int j = -5; j < 5; ++j){ // this is a hack.
			mModels[0]->setWorldMatrix(XMMatrixIdentity()*XMMatrixRotationY(0)*XMMatrixRotationX(-XM_PIDIV2)*XMMatrixScaling(1, 1, 1)*XMMatrixTranslation(i, 0, j));
			mModels[0]->draw();
		}
	}
	mModels[1]->setWorldMatrix(XMMatrixIdentity()*XMMatrixRotationY(0)*XMMatrixRotationX(-XM_PIDIV2)*XMMatrixScaling(1, 1, 1)*XMMatrixTranslation(-3,1,0)); 
	mModels[2]->setWorldMatrix(XMMatrixIdentity()*XMMatrixRotationY(0)*XMMatrixRotationX(-XM_PIDIV2)*XMMatrixScaling(1, 1, 1)*XMMatrixTranslation(-1, 0, 0));
	mModels[3]->setWorldMatrix(XMMatrixIdentity()*XMMatrixRotationY(0)*XMMatrixRotationX(-0)*XMMatrixScaling(1, 1, 1)*XMMatrixTranslation(1, 0.2, 0));
	*/
	mModels[0]->setWorldMatrix(XMMatrixIdentity()*XMMatrixRotationY(0)*XMMatrixRotationX(-XM_PIDIV2)*XMMatrixScaling(0.2, 0.2, 0.2)*XMMatrixTranslation(0, -2, 3));

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
	mSky.destroy();
	mLightManager.destroy();
	for (UINT i = 0; i < mModels.size(); ++i){
		mModels[i]->destroy();
		delete mModels[i];
	}
}