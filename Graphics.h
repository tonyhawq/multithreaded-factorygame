#pragma once
#include "DX11GFX_Namespace.h"

#include "Winbrew.h"
#include <d3d11.h>

class Graphics::DX11GFX::Graphics
{
public:
	Graphics(HWND windowHandle, D3D_DRIVER_TYPE drivertype = D3D_DRIVER_TYPE_HARDWARE);
	~Graphics();

	ID3D11Device* getDevice();
	IDXGISwapChain* getSwapchain();
	ID3D11DeviceContext* getContext();
	ID3D11RenderTargetView* getRenderTarget();
private:
	ID3D11Device* device = NULL;
	IDXGISwapChain* swapchain = NULL;
	ID3D11DeviceContext* context = NULL;
	ID3D11RenderTargetView* target = NULL;
};

