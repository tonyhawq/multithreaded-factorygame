#pragma once
#include "DX11GFX_Namespace.h"

#include "Winbrew.h"
#include <d3d11.h>
#include <wrl.h>

#include "WindowsException.h"

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
	Microsoft::WRL::ComPtr<ID3D11Device> device;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapchain;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> target;
};

class Graphics::DX11GFX::DeviceRemovedException : public except::WindowException {
	using except::WindowException::WindowException;
public:
	const char* getType() const override;
};