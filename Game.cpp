#include "Game.h"

Game::Game(int w, int h, LPCWSTR name) : window(w, h, name, L"DX11 factorygame windowclass") {

}

void Game::run() {
	this->running = true;
}

bool Game::isRunning() {
	return this->running;
}

void Game::quit() {
	this->running = false;
}