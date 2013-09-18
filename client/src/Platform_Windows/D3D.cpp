#include "D3D.h"

#include <dxgi.h>
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")

#include <ostream>
#include <iostream>
#include <ios>

D3D::D3D()	: swapChain(0), device(0), deviceContext(0), renderTargetView(0), rasterState(0), backBuffer(0)
{
	fullScreen = false;
}

D3D::~D3D()
{

}

bool setDXGI(unsigned int& _gpuMemory, char* _gpuDescription, int& _numerator, int& _denominator, unsigned int _screenWidth, unsigned int _screenHeight)
{
	HRESULT			result;
	IDXGIFactory*	factory;
	IDXGIAdapter*	adapter;
	IDXGIOutput*	adapterOutput;

	DXGI_MODE_DESC*		displayModeList;
	DXGI_ADAPTER_DESC	adapterDesc;
	unsigned int		numModes, stringLength;
	int error;
	// create directx interface
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if( FAILED(result) )
		return false;

	// create adapter for gpu interface
	result = factory->EnumAdapters(0, &adapter);
	if( FAILED(result) )
		return false;

	// enumerate all outputs(monitors)
	result = adapter->EnumOutputs(0, &adapterOutput);
	if( FAILED(result) )
		return false;
	
	// get number of modes (r8g8b8a8) supported by monitor
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if( FAILED(result) )
		return false;

	// create list for possible display modes for monitor/gpu combination
	displayModeList = new DXGI_MODE_DESC[numModes];
	if(!displayModeList)
		return false;

	// fill out list with display modes
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if( FAILED(result) )
		return false;

	// find matching refresh rate(numerator, denominator) for the active displaymode
	unsigned int i;
	for(i=0; i < numModes; i++)
	{
		if(displayModeList[i].Width == _screenWidth)
		{
			if(displayModeList[i].Height == _screenHeight)
			{
				_numerator = displayModeList[i].RefreshRate.Numerator;
				_denominator = displayModeList[i].RefreshRate.Denominator;
			}
		}
	}

	// get gpu description
	result = adapter->GetDesc(&adapterDesc);
	if( FAILED(result) )
		return false;

	_gpuMemory = (unsigned int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	error = wcstombs_s(&stringLength, _gpuDescription, 128, adapterDesc.Description, 128);
	if(error != 0)
		return false;

	// release DXGI
	delete [] displayModeList;
	displayModeList = NULL;

	adapterOutput->Release();
	adapterOutput = 0;

	adapter->Release();
	adapter = 0;

	factory->Release();
	factory = 0;

	return true;
}

bool D3D::setSwapChain(HWND _hWnd, unsigned int _numerator, unsigned int _denominator, bool _fullScreen)
{
	DXGI_SWAP_CHAIN_DESC	swapChainDesc;

	HRESULT					result;

	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	
	// set buffer information for swapchain description
	swapChainDesc.BufferCount					= 1;
	swapChainDesc.BufferDesc.Width				= screenWidth;
	swapChainDesc.BufferDesc.Height				= screenHeight;
	swapChainDesc.BufferDesc.Format				= DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.ScanlineOrdering	= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling			= DXGI_MODE_SCALING_UNSPECIFIED;

	if(vsyncEnabled)
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator		= _numerator;
		swapChainDesc.BufferDesc.RefreshRate.Denominator	= _denominator;
	}
	else
	{
		swapChainDesc.BufferDesc.RefreshRate.Numerator		= 0;
		swapChainDesc.BufferDesc.RefreshRate.Denominator	= 1;
	}

	// set usage of back buffer
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	// set handle
	swapChainDesc.OutputWindow = _hWnd;

	//no multisampling
	swapChainDesc.SampleDesc.Count	= 1;
	swapChainDesc.SampleDesc.Quality= 0;

	// set fullscreen/windowed
	if( _fullScreen )
		swapChainDesc.Windowed = false;
	else
		swapChainDesc.Windowed = true;

	// discard backbuffer after presenting
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	// no flags
	swapChainDesc.Flags = 0;

	// feature level
	featureLevel = D3D_FEATURE_LEVEL_11_0;
	
	// create swapchain, device & context
	result = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, &featureLevel, 1,
											D3D11_SDK_VERSION, &swapChainDesc, &swapChain, &device, NULL, &deviceContext);

	if( FAILED(result) )
		return false;

	// get pointer to backbuffer
	result = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
	if( FAILED(result) )
		return false;

	// create rendertargetview
	result = device->CreateRenderTargetView(backBuffer, NULL, &renderTargetView);
	if( FAILED(result) )
		return false;

	// release pointer to buffer
	backBuffer->Release();
	backBuffer = 0;
	
	return true;
}

bool D3D::setRasterState(D3D11_CULL_MODE _cullMode, D3D11_FILL_MODE _fillMode)
{
	HRESULT result;
	D3D11_RASTERIZER_DESC rasterDesc;

	rasterDesc.AntialiasedLineEnable	= false;
	rasterDesc.CullMode					= _cullMode;
	rasterDesc.DepthBias				= 0;
	rasterDesc.DepthBiasClamp			= 0.0f;
	rasterDesc.DepthClipEnable			= true;
	rasterDesc.FillMode					= _fillMode;
	rasterDesc.FrontCounterClockwise	= false;
	rasterDesc.MultisampleEnable		= false;
	rasterDesc.ScissorEnable			= false;
	rasterDesc.SlopeScaledDepthBias		= 0.0f;

	result = device->CreateRasterizerState(&rasterDesc, &rasterState);

	if( FAILED(result) )
		return false;

	deviceContext->RSSetState(rasterState);

	return true;
}

bool D3D::setViewPort(unsigned int _screenWidth, unsigned int _screenHeight)
{
	viewPort.Width		= (float)_screenWidth;
	viewPort.Height		= (float)_screenHeight;
	viewPort.MinDepth	= 0.0f;
	viewPort.MaxDepth	= 1.0f;
	viewPort.TopLeftX	= 0.0f;
	viewPort.TopLeftY	= 0.0f;

	deviceContext->RSSetViewports(1, &viewPort);

	return true;
}

bool D3D::initialize(HWND _hWnd)
{
	bool result;
	int numerator, denominator;
	screenWidth = 1280;
	screenHeight = 1024;

	// query DXGI for hardware setup information
	result = setDXGI(gpuMemory, gpuDescription, numerator, denominator, screenWidth, screenHeight);
	if(result == false)
		return false;

	// 
	result = setSwapChain(_hWnd, numerator, denominator, fullScreen);
	if(result == false)
		return false;

	// set new raster state
	result = setRasterState(D3D11_CULL_MODE::D3D11_CULL_NONE, D3D11_FILL_MODE::D3D11_FILL_SOLID);
	if( FAILED(result) )
		return false;

	setViewPort(screenWidth, screenHeight);

	return true;
}

void D3D::shutDown()
{

}