#pragma once

#include <vector>

#include "Winbrew.h"
#include "EventHandler.h"
#include "Exceptions.h"

namespace DX11Win {
	extern unsigned int WindowsRegistered;

	class WindowClass
	{
	public:
		WindowClass(LPCWSTR name);
		~WindowClass();

		void usingThisObj();
		void stoppedUsing();

		bool noUsers();

		LPCWSTR getName();
		HINSTANCE getInstance();
	private:
		unsigned short usedBy = 0;
		LPCWSTR windowClassName;
		HINSTANCE hInstance;
	};

	class Window
	{
	public:
		Window(int w, int h, LPCWSTR name, LPCWSTR windowClassName);
		Window(int w, int h, LPCWSTR name, WindowClass* windowClass);
		~Window();
		void CloseWindow();

		static LRESULT CALLBACK MSG_HandlerSetup(HWND windowHandle, UINT msg, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK Static_MSG_Handler(HWND windowHandle, UINT msg, WPARAM wParam, LPARAM lParam);

		LRESULT MSG_Handler(HWND windowHandle, UINT msg, WPARAM wParam, LPARAM lParam);

		handlers::EventHandler* getHandler();
		handlers::EventHandler* setHandler(handlers::EventHandler* handler);

		std::string title;
	private:
		void SetupWindow(int w, int h, LPCWSTR name);

		WindowClass* thisWindowClass = NULL;
		handlers::EventHandler* handler = NULL;

		int w = 0;
		int h = 0;
		HWND windowHandle{};
	};

	class WindowException : public except::BaseException
	{
	public:
		WindowException(int line, const char* file, HRESULT hr);
		const char* what() const override;
		const char* getType() const override;
		static std::string translateErrorCode(HRESULT hr);
		HRESULT getErrorCode() const;
		std::string getErrorString() const;
	private:
		HRESULT hResult;
	};
}