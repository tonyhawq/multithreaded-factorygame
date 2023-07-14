#pragma once

#include <memory>
#include <optional>

#include "Winbrew.h"
#include "EventHandler.h"
#include "Exceptions.h"
#include "WindowsException.h"
#include "Graphics.h"

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
		Window(int w, int h, LPCWSTR name, LPCWSTR windowClassName, handlers::EventHandler*& handler);
		Window(int w, int h, LPCWSTR name, WindowClass* windowClass, handlers::EventHandler*& handler);
		~Window();
		void CloseWindow(int exitcode);

		void setTitle(std::string title);

		static std::optional<int> processMessages();
		static LRESULT CALLBACK MSG_HandlerSetup(HWND windowHandle, UINT msg, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK Static_MSG_Handler(HWND windowHandle, UINT msg, WPARAM wParam, LPARAM lParam);
		LRESULT MSG_Handler(HWND windowHandle, UINT msg, WPARAM wParam, LPARAM lParam);
		
		handlers::EventHandler* getHandler();

		std::string title;
		bool exit = false;
		int exitcode = 0;

		HWND* getHandle();

		Graphics::DX11GFX::Graphics*& getGraphics();

		int getW() { return this->w; }
		int getH() { return this->h; }
		void getSize(int* w, int* h) { if (w) { *w = this->w; } if (h) { *h = this->h; } }
	private:
		void SetupWindow(int w, int h, LPCWSTR name, handlers::EventHandler* handler);

		WindowClass* thisWindowClass = NULL;
		handlers::EventHandler* handler = NULL;
		Graphics::DX11GFX::Graphics* gfx;

		int w = 0;
		int h = 0;
		HWND windowHandle{};
	};
}