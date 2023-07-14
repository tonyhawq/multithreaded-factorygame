#include "Game.h"

int CALLBACK WinMain(
	_In_ HINSTANCE hInstance, 
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow)
{
	try
	{
		Game game = Game( 1280, 720, L"game" );
		int errorCode = game.run();
		return errorCode;
	}
	catch (const Graphics::DX11GFX::DeviceRemovedException* e)
	{
		MessageBoxA(NULL, e->what(), e->getType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const except::WindowException* e)
	{
		MessageBoxA(NULL, e->what(), e->getType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const except::BaseException* e)
	{
		MessageBoxA(NULL, e->what(), e->getType(), MB_OK | MB_ICONEXCLAMATION);
	}
	catch (const std::exception* e)
	{
		MessageBoxA(NULL, e->what(), "std::exception", MB_OK | MB_ICONEXCLAMATION);
	}
	catch (...)
	{
		MessageBoxA(NULL, "No details", "unknown exception", MB_OK | MB_ICONERROR);
	}
	return -1;
}