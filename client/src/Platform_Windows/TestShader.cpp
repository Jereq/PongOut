#include "TestShader.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
//#pragma comment(
#include <d3d11.h>
#include <D3DX11.h>
#include <D3DX11async.h>
#include <D3DX10math.h>



TestShader::TestShader() :	vertexShader(0), pixelShader(0), layout(0),
							samplerState(0)
{
	
};

TestShader::~TestShader()
{

}

bool TestShader::initialize(ID3D11Device* _device, HWND _hWnd)
{
	bool result;
	
	result = initializeShader(_device, _hWnd, "../shader/test.vs", L"../shader/test.ps");
	if(!result)
		return false;

	return false;
}

bool TestShader::initializeShader(ID3D11Device* _device, HWND _hWnd, LPCSTR _vsFile, WCHAR* _psFile)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* vertexShaderBuffer;
	ID3D10Blob* pixelShaderBuffer;

	D3D11_INPUT_ELEMENT_DESC layoutDesc[2];
	unsigned int numElements;

	D3DXVECTOR2 vec = D3DXVECTOR2(0,0);
	result = D3DX11CompileFromFile(_vsFile, NULL, NULL, "TestVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &vertexShaderBuffer, &errorMessage, NULL);
	if( FAILED(result) )
		return false;

	return true;
}

void TestShader::shutDown()
{

}

//bool TestShader::initializeShader(ID3D11Device* _device, HWND _hWnd, LPCSTR _vsFile,  WCHAR* _psFile)
//{
//
//}