#pragma once

#include "Winbrew.h"
#include <d3d11.h>

#include "DX11GFX_Namespace.h"
#include "WindowsException.h"
#include "Graphics.h"

class Graphics::Renderer
{
public:
	void clearTarget(float r, float g, float b)
	{
		const float rgb[] = {r,g,b, 1.0f};
		context->ClearRenderTargetView(target, rgb);
	}

	void flip();

	void setDevice(ID3D11Device* device);
	void setContext (ID3D11DeviceContext* context);
	void setTarget (ID3D11RenderTargetView* target);
	void setSwapChain(IDXGISwapChain* swapcchain);
private:
	ID3D11Device* device = NULL;
	IDXGISwapChain* swapchain = NULL;
	ID3D11DeviceContext* context = NULL;
	ID3D11RenderTargetView* target = NULL;
};