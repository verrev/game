#include <string>
#include <vld.h>
#include <Windows.h>
#include "CSceneManager.h"
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hInstance = hInstance;
	wc.lpfnWndProc = WindowProc;
	wc.lpszClassName = L"wc1";
	wc.style = CS_VREDRAW | CS_HREDRAW;
	RegisterClassEx(&wc);
	ShowCursor(0);

	int w = 800, h = w;
	//float w = GetSystemMetrics(0), h = GetSystemMetrics(1); 
	HWND gameWindow = 0;
	while (!gameWindow)
		gameWindow = CreateWindowEx(0, L"wc1", L"pong", WS_VISIBLE | WS_BORDER, 0, 0, w, h, 0, 0, hInstance, 0);

	if (CDirectX11::init(w, h, 1, gameWindow, D3D11_FILL_WIREFRAME)){ //D3D11_FILL_WIREFRAME // D3D11_FILL_SOLID
		CSceneManager m;
		m.init(w, h);
		CTimer::init();
		CInput::init(gameWindow, hInstance);

		MSG msg;
		while (1){
			while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)){
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			if (GetAsyncKeyState(VK_ESCAPE))
				break;
			CTimer::begin();

			float bgColor[4] = { (1.0f, 0.0f, 0.0f, 0.0f) };
			CDirectX11::gDevCon->ClearRenderTargetView(CDirectX11::gBackBufferView, bgColor);
			CDirectX11::gDevCon->ClearDepthStencilView(CDirectX11::gDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
			CInput::update();
			m.draw(CTimer::getDelta());
			CDirectX11::gSwapChain->Present(0, 0);

			CTimer::end();
			FrameStats fs = CTimer::getStats();
			std::wstring s = L"FPS: ";
			s.append(std::to_wstring((int)fs.mFPS));
			s.append(L" frametime: ");
			s.append(std::to_wstring(fs.mMsPerFrame));
			s.append(L" (ms)");
			SetWindowText(gameWindow, s.c_str());
		}
		CInput::destroy();
		m.destroy();
		CDirectX11::destroy();
	}
	return 0;
}