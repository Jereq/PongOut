#ifndef __TESTSHADER_H
#define __TESTSHADER_H

#include <d3d11.h>

class TestShader
{
public:
	TestShader();
	~TestShader();

	bool	initialize(ID3D11Device* _device, HWND _hWnd);
	void	shutDown();
	//bool			draw(ID3D11DeviceContext* _deviceContext, ID3D11ShaderResourceView* _resourceView);

private:
	bool	initializeShader(ID3D11Device* _device, HWND _hWnd, LPCSTR _vsFile,  LPCSTR _psFile);
	//bool	setShaderParamaters(ID3D11DeviceContext* _deviceContext, ID3D11ShaderResourceView* _resourceView);
	//void	drawShader(ID3D11DeviceContext* _deviceContext, int indexCount);

	ID3D11VertexShader*	vertexShader;
	ID3D11PixelShader*	pixelShader;
	ID3D11InputLayout*	layout;
	ID3D11SamplerState*	samplerState;
};

#endif