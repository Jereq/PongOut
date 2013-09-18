#include "D3D.h"

#include <dxgi.h>
#pragma comment(lib, "dxgi.lib")

D3D::D3D()
{

}

D3D::~D3D()
{

}

bool D3D::initialize(HWND _hWnd)
{
	HRESULT			result;
	IDXGIFactory*	factory;
	IDXGIAdapter*	adapter;
	IDXGIOutput*	adapterOutput;

	DXGI_MODE_DESC*	displayModeList;

	unsigned int	numModes;
	
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
	/*result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList);
	if( FAILED(result) )
		return false;
	/*
	// 
	//displayModeList = new DXGI_MODE_DESC[numModes];
	*/
	return true;
}

void D3D::shutDown()
{

}