#include "Graphics.h"
#include "Exceptions.h"

#pragma comment(lib, "d3d11.lib")

Graphics::DX11GFX::Graphics::Graphics(HWND windowHandle, D3D_DRIVER_TYPE drivertype)
{
	DXGI_SWAP_CHAIN_DESC swpchndesc = {};
	swpchndesc.BufferCount = 1;
	swpchndesc.BufferDesc.Width = 0;
	swpchndesc.BufferDesc.Height = 0;
	swpchndesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swpchndesc.BufferDesc.RefreshRate.Numerator = 0;
	swpchndesc.BufferDesc.RefreshRate.Denominator = 0;
	swpchndesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swpchndesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swpchndesc.SampleDesc.Count = 1;
	swpchndesc.SampleDesc.Quality = 0;
	swpchndesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swpchndesc.OutputWindow = windowHandle;
	swpchndesc.Windowed = TRUE;
	swpchndesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swpchndesc.Flags = 0;
	HRESULT res = D3D11CreateDeviceAndSwapChain(
		NULL,
		drivertype,
		NULL,
		0,
		NULL,
		0,
		D3D11_SDK_VERSION,
		&swpchndesc,
		&swapchain,
		&device,
		NULL,
		&context
	);
	if (FAILED(res))
	{
		throw new except::WindowException(__LINE__, __FILE__, res);
	}
	ID3D11Resource* backBuffer = NULL;
	res = swapchain->GetBuffer(0, __uuidof(ID3D11Resource), (void**)&backBuffer);
	if (FAILED(res))
	{
		throw new except::WindowException(__LINE__, __FILE__, res);
	}
	res = device->CreateRenderTargetView(backBuffer, NULL, &this->target);
	if (FAILED(res))
	{
		throw new except::WindowException(__LINE__, __FILE__, res);
	}
	backBuffer->Release();
}

Graphics::DX11GFX::Graphics::~Graphics() {
	if (target)
	{
		target->Release();
	}
	if (context)
	{
		context->Release();
	}
	if (swapchain)
	{
		swapchain->Release();
	}
	if (device)
	{
		device->Release();
	}
}


ID3D11Device* Graphics::DX11GFX::Graphics::getDevice() { return this->device; }
IDXGISwapChain* Graphics::DX11GFX::Graphics::getSwapchain() { return this->swapchain; }
ID3D11DeviceContext* Graphics::DX11GFX::Graphics::getContext() { return this->context; }
ID3D11RenderTargetView* Graphics::DX11GFX::Graphics::getRenderTarget() { return this->target; }

const char* Graphics::DX11GFX::DeviceRemovedException::getType() const
{
	return "Device removed exception";
}