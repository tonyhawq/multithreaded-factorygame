#include "Window.h"

int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow)
{
	DX11Win::Window window = DX11Win::Window(1280, 720, L"multithreaded factorygame", L"DX11 Factory game window class");
	DX11Win::Window window2 = DX11Win::Window(1280, 720, L"multithreaded factorygame", L"DX11 Factory game window class 2");
	MSG msg = { 0 };
	BOOL gResult = -1;
	while (DX11Win::WindowsRegistered > 0)
	{
		while (gResult = GetMessage(&msg, NULL, 0, 0) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			std::ostringstream oss;
			oss << DX11Win::WindowsRegistered;
			OutputDebugStringA(oss.str().c_str());
		}
	}
	if (gResult == -1)
	{
		return -1;
	}
	else
	{
		return (int)msg.wParam;
	}
}