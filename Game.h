#pragma once

#include "Window.h"

/// <summary>
/// Highest level class
/// </summary>
class Game
{
public:
	Game(int w, int h, LPCWSTR name);

	void run();
	bool isRunning();
	void quit();

	void update();

	DX11Win::Window window;
private:
	bool running = false;
};