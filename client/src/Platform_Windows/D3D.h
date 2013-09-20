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

public:
	ID3D11Device*			device;
	ID3D11DeviceContext*	deviceContext;


	IDXGISwapChain*			swapChain;			//
	ID3D11Texture2D*		backBuffer;			//
	ID3D11RenderTargetView*	renderTargetView;	//
	D3D11_VIEWPORT			viewPort;			//
	ID3D11RasterizerState*	rasterState;		//
	D3D_FEATURE_LEVEL		featureLevel;		//

	ID3D11DepthStencilState*	depthStencilBuffer;
	ID3D11DepthStencilView*		depthStencilView;
	ID3D11DepthStencilState*	noDepthStencilState;

	unsigned int	screenWidth, screenHeight;
	unsigned int	gpuMemory;
	char			gpuDescription[128];
	bool			vsyncEnabled;
	bool			fullScreen;

	bool			setSwapChain(HWND _hWnd, unsigned int _numerator, unsigned int _denominator, bool _fullScreen);
	bool			setRasterState(D3D11_CULL_MODE _cullMode, D3D11_FILL_MODE _fillMode);
	bool			setViewPort(unsigned int _screenWidth, unsigned int _screenHeight);
	bool			setDepthStencil();
};

#endif