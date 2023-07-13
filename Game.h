#pragma once

#include "Window.h"
#include "Timing.h"
#include "Renderer.h"
#include <sstream>

/// <summary>
/// Highest level class
/// </summary>
class Game
{
public:
	Game(int w, int h, LPCWSTR name);

	void destroyWindow(DX11Win::Window** win);

	int run();
	bool isRunning();
	void quit();

	void update();
	void render();

	Graphics::Renderer renderer = Graphics::Renderer();
	DX11Win::Window* window = NULL;
	gTime::MarcoPolo timer;
	handlers::EventHandler* handler = NULL;
private:
	bool running = false;
};