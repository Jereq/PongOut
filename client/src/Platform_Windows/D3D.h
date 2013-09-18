#ifndef __D3D_H
#define __D3D_H

#include <D3DCommon.h>
#include <d3d11.h>

class D3D
{
public:
	D3D();
	~D3D();
	 bool initialize(HWND _hWnd);
	 void shutDown();

protected:

private:
	ID3D11Device*			device;
	ID3D11DeviceContext*	deviceContext;


	IDXGISwapChain*			swapChain;
	ID3D11RenderTargetView*	renderTargetView;
	ID3D11RasterizerState*	rasterizerState;

};

#endif