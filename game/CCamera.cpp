#include "CCamera.h"
void CCamera::init(const int &w, const int &h)
{
	mDefFwd = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	mDefRight = XMVectorSet(1.0f, 0, 0.0f, 0.0f);
	mRight = mDefRight;
	mForward = mDefFwd;
	mMoveLR = 0.0f;
	mMoveBF = 0.0f;
	mYaw = 0.0f;
	mPitch = 0.0f;
	mPosition = XMVectorSet(0, 0.5, -5, 0);
	mTarget = XMVectorSet(0, 0, 0, 0);
	mUp = XMVectorSet(0, 1, 0, 0);
	XMStoreFloat4x4(&mView, XMMatrixLookAtLH(mPosition, mTarget, mUp));
	XMStoreFloat4x4(&mProjection, XMMatrixPerspectiveFovLH(XM_PIDIV4, (float)w / (float)h, 0.1f, 500.0f));

	D3D11_BUFFER_DESC cbd = { 0 };
	cbd.Usage = D3D11_USAGE_DEFAULT;
	cbd.ByteWidth = sizeof(CameraCBuffer);
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	CDirectX11::gDev->CreateBuffer(&cbd, 0, &mCBuffer);
	mCBufferData.mProj = XMMatrixTranspose(XMLoadFloat4x4(&mProjection));
}
void CCamera::update(const float &lr, const float &bf, const float &yaw, const float &pitch)
{
	mMoveBF = bf;
	mMoveLR = lr;
	mYaw = yaw;
	mPitch = pitch;
	XMMATRIX rotMat = XMMatrixRotationRollPitchYaw(mPitch, mYaw, 0);
	mTarget = XMVector3Normalize(XMVector3TransformCoord(mDefFwd, rotMat));
	XMMATRIX RotateYTempMatrix;
	RotateYTempMatrix = XMMatrixRotationY(mYaw);
	mRight = XMVector3TransformCoord(mDefRight, RotateYTempMatrix);
	mUp = XMVector3TransformCoord(mUp, RotateYTempMatrix);
	mForward = XMVector3TransformCoord(mDefFwd, RotateYTempMatrix);
	mPosition += mMoveLR*mRight;
	mPosition += mMoveBF*mForward;
	mMoveLR = 0.0f;
	mMoveBF = 0.0f;
	mTarget += mPosition;
	XMStoreFloat4x4(&mView, XMMatrixLookAtLH(mPosition, mTarget, mUp));
	mCBufferData.mView = XMMatrixTranspose(XMLoadFloat4x4(&mView));
	CDirectX11::gDevCon->UpdateSubresource(mCBuffer, 0, 0, &mCBufferData, 0, 0);
	CDirectX11::gDevCon->VSSetConstantBuffers(0, 1, &mCBuffer);
}
void CCamera::destroy()
{
	safeRelease(mCBuffer);
}