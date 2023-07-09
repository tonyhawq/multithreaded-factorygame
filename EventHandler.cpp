#include "EventHandler.h"

LRESULT EventHandler::HandleMSG(HWND windowHandle, UINT msg, WPARAM wParam, LPARAM lParam) {

	return DefWindowProc(windowHandle, msg, wParam, lParam);
}