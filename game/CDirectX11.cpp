#include "CDirectX11.h"
ID3D11Device *CDirectX11::gDev = 0;
ID3D11DeviceContext *CDirectX11::gDevCon = 0;
IDXGISwapChain *CDirectX11::gSwapChain = 0;
ID3D11RenderTargetView *CDirectX11::gBackBufferView = 0;
ID3D11DepthStencilView *CDirectX11::gDepthStencilView = 0;
//-------------------------------------------------------------
bool CDirectX11::init(const UINT &w, const UINT &h, bool isWindowed, const HWND &window)
{
	if (w == GetSystemMetrics(0)) isWindowed = 0;
	DXGI_MODE_DESC bd = { 0 };
	bd.Width = w;
	bd.Height = h;
	bd.RefreshRate.Numerator = 60;
	bd.RefreshRate.Denominator = 1;
	bd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bd.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	bd.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	DXGI_SWAP_CHAIN_DESC scd = { 0 };
	scd.BufferDesc = bd;
	scd.SampleDesc.Count = 1;
	scd.SampleDesc.Quality = 0;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.BufferCount = 1;
	scd.OutputWindow = window;
	scd.Windowed = isWindowed;
	scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	D3D11CreateDeviceAndSwapChain(0, D3D_DRIVER_TYPE_HARDWARE, 0, D3D11_CREATE_DEVICE_DEBUG | D3D11_CREATE_DEVICE_SINGLETHREADED | D3D11_CREATE_DEVICE_BGRA_SUPPORT, 0, 0, D3D11_SDK_VERSION, &scd, &gSwapChain, &gDev, 0, &gDevCon);

	ID3D11Texture2D* bb;
	gSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&bb);

	gDev->CreateRenderTargetView(bb, 0, &gBackBufferView);
	safeRelease(bb);

	D3D11_VIEWPORT vp = { 0 };
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	vp.Width = static_cast<FLOAT>(w);
	vp.Height = static_cast<FLOAT>(h);
	gDevCon->RSSetViewports(1, &vp);

	ID3D11Texture2D* dsBuffer;
	D3D11_TEXTURE2D_DESC depthStencilDesc;

	depthStencilDesc.Width = w;
	depthStencilDesc.Height = h;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	gDev->CreateTexture2D(&depthStencilDesc, 0, &dsBuffer);
	gDev->CreateDepthStencilView(dsBuffer, NULL, &gDepthStencilView);

	gDevCon->OMSetRenderTargets(1, &gBackBufferView, gDepthStencilView);

	safeRelease(dsBuffer);

	if (gDev&&gDevCon&&gSwapChain&&gBackBufferView&&gDepthStencilView) return 1;
	return 0;
}
void CDirectX11::destroy()
{
	gSwapChain->SetFullscreenState(0, 0);
	safeRelease(gDev);
	safeRelease(gDevCon);
	safeRelease(gSwapChain);
	safeRelease(gBackBufferView);
	safeRelease(gDepthStencilView);
}