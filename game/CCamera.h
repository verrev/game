#ifndef _CCAMERA_H_
#define _CCAMERA_H_
#include "CDirectX11.h"
struct CameraCBuffer
{
	XMMATRIX mView;
	XMMATRIX mProj;
};
class CCamera
{
public:
	void init(const int &w, const int &h);
	void update(const float &lr, const float &bf, const float &yaw, const float &pitch);
	XMVECTOR getPos();
	void destroy();
private:
	ID3D11Buffer *mCBuffer;
	CameraCBuffer mCBufferData;
	XMFLOAT4X4 mView, mProjection;
	XMVECTOR mPosition, mTarget, mUp;
	XMVECTOR mDefFwd, mDefRight;
	XMVECTOR mRight, mForward;
	float mMoveLR, mMoveBF;
	float mYaw, mPitch;
};
#endif