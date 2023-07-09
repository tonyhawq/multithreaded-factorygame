#pragma once

#include "Winbrew.h"

class EventHandler {
public:
	LRESULT HandleMSG(HWND windowHandle, UINT msg, WPARAM wParam, LPARAM lParam);
private:
};