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
	return !usedBy;
}

DX11Win::Window::Window(int w, int h, LPCWSTR name, LPCWSTR windowClassName) {
	this->thisWindowClass = new WindowClass(windowClassName);
	SetupWindow(w, h, name);
}

DX11Win::Window::Window(int w, int h, LPCWSTR name, WindowClass* windowClass) {
	this->thisWindowClass = windowClass;
	SetupWindow(w, h, name);
}

void DX11Win::Window::SetupWindow(int w, int h, LPCWSTR name) {
	DX11Win::WindowsRegistered++;
	// get adjusted window size based on actual client window size
	RECT windowRect{};
	windowRect.left = 0;
	windowRect.right = w;
	windowRect.top = 0;
	windowRect.bottom = h;
	AdjustWindowRect(&windowRect, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE);
	this->windowHandle = CreateWindow(
		this->thisWindowClass->getName(),
		name,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		// get width and height from adjusted window
		windowRect.right - windowRect.left,
		windowRect.bottom = windowRect.top,
		NULL,
		NULL,
		this->thisWindowClass->getInstance(),
		this
	);
	ShowWindow(this->windowHandle, SW_SHOW);
	this->w = w;
	this->h = h;
	this->thisWindowClass->usingThisObj();
}

DX11Win::Window::~Window() {
	DX11Win::WindowsRegistered--;
	this->thisWindowClass->stoppedUsing();
	if (this->thisWindowClass->noUsers())
	{
		delete this->thisWindowClass;
	}
	DestroyWindow(this->windowHandle);
}

void DX11Win::Window::CloseWindow() {
	Window::~Window();
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
	if (handler)
	{
		return handler->HandleMSG(windowHandle, msg, wParam, lParam);
	}
	switch(msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		this->CloseWindow();
		return 0;
		break;
	}
	return DefWindowProc(windowHandle, msg, wParam, lParam);
}

EventHandler* DX11Win::Window::getHandler()
{
	return handler;
}

DX11Win::WindowException::WindowException(int line, const char* file, HRESULT hr) 
	: except::BaseException(line, file) {}

const char* DX11Win::WindowException::what() const {
	std::ostringstream stringstream;
	stringstream << this->getType() << '\n'
		<< "Error code: " << this->getErrorCode() << '\n'
		<< "Error Desc: " << this->getErrorString() << '\n'
		<< getOrigin();
	written = stringstream.str();
	return written.c_str();
}

const char* DX11Win::WindowException::getType() const {
	return "Window Exception";
}

std::string DX11Win::WindowException::translateErrorCode(HRESULT hr) {
	char* messageBuffer = NULL;
	DWORD msgLength = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		hr,
		0,
		reinterpret_cast<LPWSTR>(&messageBuffer),
		0,
		NULL
	);
	if (msgLength == 0)
	{
		return "Something went wrong during formatting";
	}
	std::string errorString = messageBuffer;
	free(messageBuffer);
	return errorString;
}

HRESULT DX11Win::WindowException::getErrorCode() const {
	return hResult;
}

std::string DX11Win::WindowException::getErrorString() const {
	return written;
}