#include <Windows.h>

LRESULT CALLBACK WindowProcedure(HWND windowHandle, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(42);
		break;
	case WM_KEYDOWN:
		if (wParam == L'F')
		{
			SetWindowTextA(windowHandle, "Wow. You pressed F. GOOD FREAKING JOB!!!!!");
		}
		break;
	}
	return DefWindowProc(windowHandle, msg, wParam, lParam);
}

int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow)
{
	// create class
	const wchar_t className[] = L"d3d11_multithreaded_factorygame";
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WindowProcedure;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = NULL;
	wc.hCursor = NULL;
	wc.hbrBackground = NULL;
	wc.lpszClassName = className;
	wc.hIconSm = NULL;
	// register the class for use
	RegisterClassEx(&wc);
	// create the window
	HWND baseWindow = CreateWindowEx(
		0,
		className,
		// window name
		L"dx11 multithreaded factorygame",
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		// x, y
		200, 200,
		// w, h
		1280, 720,
		NULL,
		NULL,
		hInstance,
		NULL
	);
	// show the window
	ShowWindow(baseWindow, 5);
	MSG msg = { 0 };
	BOOL gResult;
	while (gResult = GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	if (gResult == -1)
	{
		return -1;
	}
	else
	{
		return msg.wParam;
	}
}