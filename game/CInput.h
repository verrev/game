#ifndef _CINPUT_H_
#define _CINPUT_H_
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#include "CDirectX11.h"
class CInput
{
public:
	static bool init(const HWND &window, const HINSTANCE &instance);
	static void update();
	static bool isKeyDown(const char &key);
	static bool isMouseButtonDown(const int &button);
	static float getMouseX();
	static float getMouseY();
	static float getMouseZ();
	static void destroy();
private:
	static IDirectInput8 *mDInput;
	static IDirectInputDevice8 *mKeyboard;
	static char mKeyboardState[256];
	static IDirectInputDevice8 *mMouse;
	static DIMOUSESTATE2 mMouseState;
};
#endif