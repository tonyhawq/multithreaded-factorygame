#pragma once

#include "Window.h"
#include "Timing.h"

/// <summary>
/// Highest level class
/// </summary>
class Game
{
public:
	Game(int w, int h, LPCWSTR name);

	void destroyWindow(DX11Win::Window* win);

	int run();
	bool isRunning();
	void quit();

	void update();
	void render();

	DX11Win::Window* window = NULL;
	gTime::MarcoPolo timer;
private:
	bool running = false;
};