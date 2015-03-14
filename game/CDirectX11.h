#ifndef _CDIRECTX11_H_
#define _CDIRECTX11_H_
#include <windows.h>
#include <d3d11.h>
#pragma comment(lib,"d3d11.lib")
#include <DirectXMath.h>
template<typename T>
void safeRelease(T obj)
{
	if (obj) obj->Release();
	obj = 0;
}
//-------------------------------------------------------------
class CDirectX11
{
public:
	static bool init(const UINT &w, const UINT &h, bool isWindowed, const HWND &window);
	static void destroy();
public:
	static ID3D11Device *gDev;
	static ID3D11DeviceContext *gDevCon;
	static IDXGISwapChain *gSwapChain;
	static ID3D11RenderTargetView *gBackBufferView;
	static ID3D11DepthStencilView *gDepthStencilView;
};
#endif