#include "Game.h"

Game::Game(int w, int h, LPCWSTR name) {
	this->handler = new handlers::EventHandler();
	this->window = new DX11Win::Window(w, h, name, L"DX11 factorygame windowclass", this->handler);
	Graphics::DX11GFX::Graphics* gfx = this->window->getGraphics();
	this->renderer.setContext(gfx->getContext());
	this->renderer.setTarget(gfx->getRenderTarget());
	this->renderer.setSwapChain(gfx->getSwapchain());
	this->renderer.setDevice(gfx->getDevice());
}

void Game::destroyWindow(DX11Win::Window** win) {
	delete (*win);
	(*win) = NULL;
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
			this->destroyWindow(&(this->window));
			continue;
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
	const float r = (float)handler->mouse.x / (float)window->getW();
	const float g = (float)handler->mouse.y / (float)window->getH();
	const float b = sin(t + 3.14);
	renderer.clearTarget(r, g, b);
	renderer.flip();
}

void Game::render() {

}