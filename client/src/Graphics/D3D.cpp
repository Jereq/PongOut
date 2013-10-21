#include "D3D.h"

#include <d3d11.h>
#include <dxgi.h>
#include <D3DX11.h>


D3D::D3D()	:	swapChain(0), device(0), deviceContext(0), 
				renderTargetView(0), rasterState(0), backBuffer(0), 
				noDepthStencilState(0)
{
	fullScreen = false;
}

D3D::~D3D()
{

}

ErrorCode setDXGI(unsigned int& _gpuMemory, char* _gpuDescription, int& _numerator, int& _denominator, unsigned int _screenWidth, unsigned int _screenHeight)
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
		return ErrorCode::WGFX_CREATE_DXGI_FACTORY_FAIL;

	// create adapter for gpu interface
	result = factory->EnumAdapters(0, &adapter);
	if( FAILED(result) )
		return ErrorCode::WGFX_GET_ADAPTER_FAIL;

	// enumerate all outputs(monitors)
	result = adapter->EnumOutputs(0, &adapterOutput);
	if( FAILED(result) )
		return ErrorCode::WGFX_ENUMERATE_ADAPTERS_FAIL;
	
	// get number of modes (r8g8b8a8) supported by monitor
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL);
	if( FAILED(result) )
		return ErrorCode::WGFX_CREATE_DISPLAY_MODE_LIST_FAIL;

	// create list for possible display modes for monitor/gpu combination
	displayModeList = new DXGI_MODE_DESC[numModes];
	if(!displayModeList)
		return ErrorCode::WGFX_GET_DISPLAY_MODE_DESC_FAIL;

	// fill out list with display modes
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if( FAILED(result) )
		return ErrorCode::WGFX_FILL_DISPLAY_MODE_LIST_FAIL;

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
		return ErrorCode::WGFX_GET_GPU_DESCRIPTION_FAIL;

	_gpuMemory = (unsigned int)(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	error = wcstombs_s(&stringLength, _gpuDescription, 128, adapterDesc.Description, 128);
	if(error != 0)
		return ErrorCode::WGFX_GET_GPU_DESCRIPTION_FAIL;

	// release DXGI
	delete [] displayModeList;
	displayModeList = NULL;

	adapterOutput->Release();
	adapterOutput = 0;

	adapter->Release();
	adapter = 0;

	factory->Release();
	factory = 0;

	return ErrorCode::WGFX_SET_DXGI_OK;
}

ErrorCode D3D::setSwapChain(HWND _hWnd, unsigned int _numerator, unsigned int _denominator, bool _fullScreen)
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
		return ErrorCode::WGFX_CREATE_SWAPCHAIN_FAIL;

	// get pointer to backbuffer
	result = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
	if( FAILED(result) )
		return ErrorCode::WGFX_GET_SWAPCHAIN_BUFFER_FAIL;

	// create rendertargetview
	result = device->CreateRenderTargetView(backBuffer, NULL, &renderTargetView);
	if( FAILED(result) )
		return ErrorCode::WGFX_CREATE_RENDERTARGET_FAIL;

	// release pointer to buffer
	backBuffer->Release();
	backBuffer = 0;
	
	return ErrorCode::WGFX_OK;
}

ErrorCode D3D::setRasterState(D3D11_CULL_MODE _cullMode, D3D11_FILL_MODE _fillMode)
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
		return ErrorCode::WGFX_CREATE_RASTERIZER_FAIL;

	deviceContext->RSSetState(rasterState);

	return ErrorCode::WGFX_SET_SWAPCHAIN_OK;
}

ErrorCode D3D::setViewPort(unsigned int _screenWidth, unsigned int _screenHeight)
{
	viewPort.Width		= (float)_screenWidth;
	viewPort.Height		= (float)_screenHeight;
	viewPort.MinDepth	= 0.0f;
	viewPort.MaxDepth	= 1.0f;
	viewPort.TopLeftX	= 0.0f;
	viewPort.TopLeftY	= 0.0f;

	deviceContext->RSSetViewports(1, &viewPort);

	return ErrorCode::WGFX_SETVIEWPORT_OK;
}

ErrorCode D3D::setDepthBuffer()
{
	HRESULT result;
	D3D11_TEXTURE2D_DESC depthBufferDesc;

	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));
	depthBufferDesc.Width = screenWidth;
	depthBufferDesc.Height = screenHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	result = device->CreateTexture2D(&depthBufferDesc, NULL, &depthStencilBuffer);
	if( FAILED(result) )
		return ErrorCode::WGFX_CREATE_DEPTHBUFFER_FAIL;

	return ErrorCode::WGFX_SET_DEPTHBUFFER_OK;
}

ErrorCode D3D::setDepthStencil()
{
	HRESULT result;
	D3D11_DEPTH_STENCIL_DESC noDepthDesc;
	ZeroMemory(&noDepthDesc, sizeof(noDepthDesc));

	noDepthDesc.DepthEnable						= true;
	noDepthDesc.DepthWriteMask					= D3D11_DEPTH_WRITE_MASK_ALL;
	noDepthDesc.DepthFunc						= D3D11_COMPARISON_LESS;
	noDepthDesc.StencilEnable					= true;
	noDepthDesc.StencilReadMask					= 0xFF;
	noDepthDesc.StencilWriteMask				= 0xFF;
	noDepthDesc.FrontFace.StencilFailOp			= D3D11_STENCIL_OP_KEEP;
	noDepthDesc.FrontFace.StencilDepthFailOp	= D3D11_STENCIL_OP_INCR;
	noDepthDesc.FrontFace.StencilPassOp			= D3D11_STENCIL_OP_KEEP;
	noDepthDesc.FrontFace.StencilFunc			= D3D11_COMPARISON_ALWAYS;
	noDepthDesc.BackFace.StencilFailOp			= D3D11_STENCIL_OP_KEEP;
	noDepthDesc.BackFace.StencilDepthFailOp		= D3D11_STENCIL_OP_DECR;
	noDepthDesc.BackFace.StencilPassOp			= D3D11_STENCIL_OP_KEEP;
	noDepthDesc.BackFace.StencilFunc			= D3D11_COMPARISON_ALWAYS;

	result = device->CreateDepthStencilState(&noDepthDesc, &noDepthStencilState);
	if( FAILED(result) )
		return ErrorCode::WGFX_CREATE_DEPTHSTENCILSTATE_FAIL;

	deviceContext->OMSetDepthStencilState(noDepthStencilState, 1);

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	depthStencilViewDesc.Format				= DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension		= D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice	= 0;

	result = device->CreateDepthStencilView(depthStencilBuffer, &depthStencilViewDesc, &depthStencilView);
	if( FAILED(result) )
		return ErrorCode::WGFX_CREATE_DEPTHSTENCILVIEW_FAIL;

	deviceContext->OMSetRenderTargets(1, &renderTargetView, depthStencilView);

	return ErrorCode::WGFX_SET_DEPTHSTENCIL_OK;
}

ErrorCode D3D::initialize(HWND _hWnd)
{
	ErrorCode result;
	int numerator, denominator;
	screenWidth = 1920;
	screenHeight = 1080;

	// query DXGI for hardware setup information
	ErrorCode dxgi_result = setDXGI(gpuMemory, gpuDescription, numerator, denominator, screenWidth, screenHeight);
	if(dxgi_result != ErrorCode::WGFX_SET_DXGI_OK)
		return ErrorCode::WGFX_SETUP_DXGI_FAIL;

	// 
	result = setSwapChain(_hWnd, numerator, denominator, fullScreen);
	if(result != ErrorCode::WGFX_SET_SWAPCHAIN_OK)
		return result;

	// set new raster state
	result = setRasterState(D3D11_CULL_NONE, D3D11_FILL_SOLID);
	if(result != ErrorCode::WGFX_SET_SWAPCHAIN_OK)
		return result;

	// set viewport by screensize
	setViewPort(screenWidth, screenHeight);

	// set depth buffer
	result = setDepthBuffer();
	if(result != ErrorCode::WGFX_SET_DEPTHBUFFER_OK)
		return result;

	// set depth stencil
	result = setDepthStencil();
	if(result != ErrorCode::WGFX_SET_DEPTHSTENCIL_OK)
		return result;

	return ErrorCode::WGFX_D3D_INIT_OK;
}

void D3D::shutDown()
{

}

void D3D::beginScene(float _r, float _g, float _b, float _a)
{
	float color[4] = {_r, _g, _b, _a };
	deviceContext->ClearRenderTargetView(renderTargetView, color);
	deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void D3D::endScene()
{
	if(vsyncEnabled)
		swapChain->Present(1,0);
	else
		swapChain->Present(0,0);
}