#pragma once

#include <memory>
#include <optional>

#include "Winbrew.h"
#include "EventHandler.h"
#include "Exceptions.h"
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
		Window(int w, int h, LPCWSTR name, LPCWSTR windowClassName);
		Window(int w, int h, LPCWSTR name, WindowClass* windowClass);
		~Window();
		void CloseWindow(int exitcode);

		void setTitle(std::string title);

		static std::optional<int> processMessages();
		static LRESULT CALLBACK MSG_HandlerSetup(HWND windowHandle, UINT msg, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK Static_MSG_Handler(HWND windowHandle, UINT msg, WPARAM wParam, LPARAM lParam);
		LRESULT MSG_Handler(HWND windowHandle, UINT msg, WPARAM wParam, LPARAM lParam);
		
		handlers::EventHandler* getOrMakeHandler();
		handlers::EventHandler* getHandler();
		handlers::EventHandler* setHandler(handlers::EventHandler* handler);

		std::string title;
		bool exit = false;
		int exitcode = 0;

		Graphics::DX11GFX::Graphics& getGraphics();
	private:
		void SetupWindow(int w, int h, LPCWSTR name);

		WindowClass* thisWindowClass = NULL;
		handlers::EventHandler* handler = NULL;
		std::unique_ptr<Graphics::DX11GFX::Graphics> gfx;

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