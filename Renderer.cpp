#include "Renderer.h"

void Graphics::Renderer::setDevice(ID3D11Device* device) {
	this->device = device;
}

void Graphics::Renderer::setContext(ID3D11DeviceContext* context) {
	this->context = context;
}

void Graphics::Renderer::setTarget(ID3D11RenderTargetView* target) {
	this->target = target;
}

void Graphics::Renderer::setSwapChain(IDXGISwapChain* swapcchain) {
	this->swapchain = swapcchain;
}

void Graphics::Renderer::flip() {
	this->swapchain->Present(0, DXGI_PRESENT_DO_NOT_WAIT);
}