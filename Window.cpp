#include "Window.h"

unsigned int DX11Win::WindowsRegistered = 0;

DX11Win::WindowClass::WindowClass(LPCWSTR name)
{
	this->hInstance = GetModuleHandle(NULL);
	this->windowClassName = name;
	WNDCLASSEX wc = {};
	wc.cbSize = sizeof(wc);
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = Window::MSG_HandlerSetup;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = this->getInstance();
	wc.hIcon = NULL;
	wc.hCursor = NULL;
	wc.hbrBackground = NULL;
	wc.lpszClassName = this->getName();
	wc.hIconSm = NULL;
	// register the class for use
	RegisterClassEx(&wc);
}

DX11Win::WindowClass::~WindowClass()
{
	UnregisterClass(this->getName(), this->getInstance());
}

LPCWSTR DX11Win::WindowClass::getName() {
	return windowClassName;
}

HINSTANCE DX11Win::WindowClass::getInstance() {
	return hInstance;
}

void DX11Win::WindowClass::usingThisObj() {
	usedBy++;
}
void DX11Win::WindowClass::stoppedUsing() {
	if (usedBy > 0)
	{
		usedBy--;
	}
}

bool DX11Win::WindowClass::noUsers() {
	return usedBy == 0;
}

DX11Win::Window::Window(int w, int h, LPCWSTR name, LPCWSTR windowClassName, handlers::EventHandler* handler) {
	this->thisWindowClass = new WindowClass(windowClassName);
	SetupWindow(w, h, name, handler);
}

DX11Win::Window::Window(int w, int h, LPCWSTR name, WindowClass* windowClass, handlers::EventHandler* handler) {
	this->thisWindowClass = windowClass;
	SetupWindow(w, h, name, handler);
}

Graphics::DX11GFX::Graphics*& DX11Win::Window::getGraphics() {
	return gfx;
}

void DX11Win::Window::SetupWindow(int w, int h, LPCWSTR name, handlers::EventHandler* handler) {
	// get adjusted window size based on actual client window size
	this->handler = handler;
	RECT windowRect{ 0, 0, w, h };
	if (AdjustWindowRect(&windowRect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE) == 0)
	{
		throw except::WindowException(__LINE__, __FILE__, GetLastError());
	}
	this->windowHandle = CreateWindow(
		this->thisWindowClass->getName(),
		name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		// get width and height from adjusted window
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		NULL,
		NULL,
		this->thisWindowClass->getInstance(),
		this
	);
	if (this->windowHandle == NULL)
	{
		throw except::WindowException(__LINE__, __FILE__, GetLastError());
	}
	ShowWindow(this->windowHandle, SW_SHOW);
	this->w = w;
	this->h = h;
	this->thisWindowClass->usingThisObj();
	this->gfx = new Graphics::DX11GFX::Graphics(windowHandle);
	DX11Win::WindowsRegistered++;
}

DX11Win::Window::~Window() {
	DX11Win::WindowsRegistered--;
	this->thisWindowClass->stoppedUsing();
	if (this->thisWindowClass->noUsers())
	{
		this->thisWindowClass->~WindowClass();
	}
	if (this->gfx)
	{
		delete this->gfx;
		this->gfx = NULL;
	}
	DestroyWindow(this->windowHandle);
}

void DX11Win::Window::CloseWindow(int exitcode) {
	this->exit = true;
	this->exitcode = exitcode;
}

std::optional<int> DX11Win::Window::processMessages() {
	MSG msg{};
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		if (msg.message == WM_QUIT)
		{
			return msg.wParam;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return {};
}

LRESULT CALLBACK DX11Win::Window::MSG_HandlerSetup(HWND windowHandle, UINT msg, WPARAM wParam, LPARAM lParam) {
	// sent when a window is being created
	if (msg == WM_NCCREATE)
	{
		// lParam is a pointer to CREATESTRUCT
		// CREATESTRUCT has a variable which is set to [this] on line 23
		const CREATESTRUCTW* createdStruct = reinterpret_cast<CREATESTRUCTW*>(lParam);
		Window* ourself = static_cast<Window*>(createdStruct->lpCreateParams);
		// set data in the window to be the pointer to ourself
		SetWindowLongPtr(windowHandle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(ourself));
		// now set the window handler to the static message handler
		SetWindowLongPtr(windowHandle, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::Static_MSG_Handler));
		return ourself->MSG_Handler(windowHandle, msg, wParam, lParam);
	}
	return DefWindowProc(windowHandle, msg, wParam, lParam);
}

LRESULT CALLBACK DX11Win::Window::Static_MSG_Handler(HWND windowHandle, UINT msg, WPARAM wParam, LPARAM lParam) {
	Window* windowToUse = reinterpret_cast<Window*>(GetWindowLongPtr(windowHandle, GWLP_USERDATA));
	return windowToUse->MSG_Handler(windowHandle, msg, wParam, lParam);
}

LRESULT DX11Win::Window::MSG_Handler(HWND windowHandle, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (msg == WM_CLOSE)
	{
		this->CloseWindow(0);
		return DefWindowProc(windowHandle, msg, wParam, lParam);
	}
	if (handler)
	{
		return handler->HandleMSG(windowHandle, msg, wParam, lParam, this->w, this->h);
	}
	return DefWindowProc(windowHandle, msg, wParam, lParam);
}

void DX11Win::Window::setTitle(std::string title) {
	SetWindowTextA(this->windowHandle, title.c_str());
}

HWND* DX11Win::Window::getHandle() {
	return &(this->windowHandle);
}

handlers::EventHandler* DX11Win::Window::getHandler()
{
	return handler;
}