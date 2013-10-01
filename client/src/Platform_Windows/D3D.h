#ifndef __D3D_H
#define __D3D_H

#include <D3DCommon.h>
#include <d3d11.h>
#include "ErrorCodeList.h"

class D3D
{
public:
	D3D();
	~D3D();
	 ErrorCode initialize(HWND _hWnd);
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

	ID3D11Texture2D*			depthStencilBuffer;
	ID3D11DepthStencilView*		depthStencilView;
	ID3D11DepthStencilState*	noDepthStencilState;

	unsigned int	screenWidth, screenHeight;
	unsigned int	gpuMemory;
	char			gpuDescription[128];
	bool			vsyncEnabled;
	bool			fullScreen;

	ErrorCode			setSwapChain(HWND _hWnd, unsigned int _numerator, unsigned int _denominator, bool _fullScreen);
	ErrorCode			setRasterState(D3D11_CULL_MODE _cullMode, D3D11_FILL_MODE _fillMode);
	ErrorCode			setViewPort(unsigned int _screenWidth, unsigned int _screenHeight);
	ErrorCode			setDepthBuffer();
	ErrorCode			setDepthStencil();

	void beginScene(float _r, float _g, float _b, float _a);
	void endScene();
};

#endif