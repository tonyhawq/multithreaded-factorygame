#include "Game.h"

Game::Game(int w, int h, LPCWSTR name) {
	window = new DX11Win::Window(w, h, name, L"DX11 factorygame windowclass");
}

void Game::destroyWindow(DX11Win::Window* win) {
	win->~Window();
	delete win;
	win = NULL;
}

int Game::run() {
	this->running = true;
	int exitcode = 0;
	while (DX11Win::WindowsRegistered > 0)
	{
		DX11Win::Window::processMessages();
		if (this->window->exit)
		{
			exitcode = this->window->exitcode;
			destroyWindow(this->window);
		}
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
}

void Game::render() {

}