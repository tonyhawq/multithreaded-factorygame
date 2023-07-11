#include "Game.h"

Game::Game(int w, int h, LPCWSTR name) {
	window = new DX11Win::Window(w, h, name, L"DX11 factorygame windowclass");
	Graphics::DX11GFX::Graphics gfx = window->getGraphics();
	renderer.setContext(gfx.getContext());
	renderer.setTarget(gfx.getRenderTarget());
}

void Game::destroyWindow(DX11Win::Window* win) {
	win->~Window();
	delete win;
	win = NULL;
}

int Game::run() {
	this->running = true;
	int exitcode = 0;
	std::ostringstream sstream;
	while (DX11Win::WindowsRegistered > 0)
	{
		sstream.clear();
		sstream << DX11Win::WindowsRegistered << '\n';
		OutputDebugStringA(sstream.str().c_str());
		std::optional<int> forceQuit = DX11Win::Window::processMessages();
		if (forceQuit.has_value())
		{
			return *forceQuit;
		}
		if (this->window->exit)
		{
			exitcode = this->window->exitcode;
			this->destroyWindow(this->window);
		}
		gTime::sleep(16);
		update();
	}
	return exitcode;
}

bool Game::isRunning() {
	return this->running;
}

void Game::quit() {
	this->running = false;
}

void Game::update() {
	const float t = timer.since();
	std::ostringstream stringstream;
	stringstream << "time elapsed " << t;
	window->setTitle(stringstream.str());
	const float r = sin(t);
	const float g = sin(t + 1.57);
	const float b = sin(t + 3.14);
	renderer.clearTarget(r, g, b);
	renderer.flip();
}

void Game::render() {

}