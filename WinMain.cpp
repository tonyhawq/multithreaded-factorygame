#include "Window.h"

int CALLBACK WinMain(
	_In_ HINSTANCE hInstance, 
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow)
{
	try {
		DX11Win::WindowClass cl = DX11Win::WindowClass(L"DX11 Factory game window class");
		DX11Win::Window window = DX11Win::Window(1280, 720, L"multithreaded factorygame", &cl);

		handlers::EventHandler* hnd = new handlers::EventHandler;
		window.setHandler(hnd);

		MSG msg = { 0 };
		BOOL gResult = -1;
		while (DX11Win::WindowsRegistered > 0)
		{
			while (gResult = GetMessage(&msg, NULL, 0, 0) > 0)
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
				if (window.getHandler()->keyPressed(VK_SPACE))
				{
					MessageBox(NULL, L"oh my GOODNESS!", L"ballz", MB_OK);
				}
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
	catch (const DX11Win::WindowException ex)
	{
		MessageBoxA(NULL, ex.what(), ex.getType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const except::BaseException ex)
	{
		MessageBoxA(NULL, ex.what(), ex.getType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception ex)
	{
		MessageBoxA(NULL, ex.what(), "standard except", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)
	{
		MessageBoxA(NULL, "No details", "unknown exception", MB_OK | MB_ICONEXCLAMATION);
	}
	return -1;
}