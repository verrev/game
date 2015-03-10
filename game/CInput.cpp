#include "CInput.h"
IDirectInput8 *CInput::mDInput = nullptr;
IDirectInputDevice8 *CInput::mKeyboard = nullptr;
char CInput::mKeyboardState[256];
IDirectInputDevice8 *CInput::mMouse = nullptr;
DIMOUSESTATE2 CInput::mMouseState;
bool CInput::init(const HWND &window, const HINSTANCE &instance)
{
	ZeroMemory(&mKeyboardState, sizeof(mKeyboardState));
	ZeroMemory(&mMouseState, sizeof(mMouseState));
	mDInput = nullptr;

	DirectInput8Create(instance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&mDInput, 0);
	if (mDInput){
		mDInput->CreateDevice(GUID_SysKeyboard, &mKeyboard, 0);
		mKeyboard->SetDataFormat(&c_dfDIKeyboard);
		mKeyboard->SetCooperativeLevel(window, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
		mKeyboard->Acquire();

		mDInput->CreateDevice(GUID_SysMouse, &mMouse, 0);
		mMouse->SetDataFormat(&c_dfDIMouse);
		mMouse->SetCooperativeLevel(window, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
		mMouse->Acquire();
		return 1;
	}
	return 0;
}
void CInput::update()
{
	mKeyboard->GetDeviceState(sizeof(mKeyboardState), (void**)&mKeyboardState);
	mMouse->GetDeviceState(sizeof(DIMOUSESTATE), (void**)&mMouseState);
}
bool CInput::isKeyDown(const char &key)
{
	return (mKeyboardState[key] & 0x80) != 0;
}
bool CInput::isMouseButtonDown(const int &button)
{
	return (mMouseState.rgbButtons[button] & 0x80) != 0;
}
float CInput::getMouseX()
{
	return (float)mMouseState.lX;
}
float CInput::getMouseY()
{
	return (float)mMouseState.lY;
}
float CInput::getMouseZ()
{
	return (float)mMouseState.lZ;
}
void CInput::destroy()
{
	safeRelease(mDInput);
	mKeyboard->Unacquire();
	mMouse->Unacquire();
	safeRelease(mKeyboard);
	safeRelease(mMouse);
}