#ifndef __TESTSHADER_H
#define __TESTSHADER_H

#include <d3d11.h>
#include "ErrorCodeList.h"

class TestShader
{
public:
	TestShader();
	~TestShader();

	ErrorCode	initialize(ID3D11Device* _device, HWND _hWnd);
	void		shutDown();

	ErrorCode	draw(ID3D11DeviceContext* _deviceContext, ID3D11Buffer* _vbuffer, ID3D11Buffer* _iBuffer, ID3D11ShaderResourceView* _srView, int _indexCount);

public:
	ErrorCode	initializeShader(ID3D11Device* _device, HWND _hWnd, LPCSTR _vsFile,  LPCSTR _psFile);

	ErrorCode	setShaderParamaters(ID3D11DeviceContext* _deviceContext, ID3D11Buffer* _vBuffer, ID3D11Buffer* _iBuffer, ID3D11ShaderResourceView* _srView);
	void		drawShader(ID3D11DeviceContext* _deviceContext, int indexCount);

	ID3D11VertexShader*		vertexShader;
	ID3D11PixelShader*		pixelShader;
	ID3D11GeometryShader*	geometryShader;
	ID3D11InputLayout*		layout;
	ID3D11SamplerState*		sampleState;
	ID3D11Buffer*			positionBuffer;
	ID3D11BlendState*		blendState;
};

#endif