#include "TestShader.h"

#include <D3DX10.h>
#include <D3DX11.h>
#include <D3DX11async.h>
#include <D3D11.h>
#include <fstream>

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
	
	result = initializeShader(_device, _hWnd, "../shaders/test.vs", "../shaders/test.ps");
	if(!result)
		return false;

	return true;
}

void OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, LPCSTR shaderFilename)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	std::ofstream fout;


	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
	bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("../build3/shaders/shader-error.txt");

	// Write out the error message.
	for(i=0; i<bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// Close the file.
	fout.close();

	// Release the error message.
	errorMessage->Release();
	errorMessage = 0;

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(hwnd, "Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);

	return;
}

bool TestShader::initializeShader(ID3D11Device* _device, HWND _hWnd, LPCSTR _vsFile, LPCSTR _psFile)
{
	HRESULT result;
	ID3D10Blob* errorMessage			= 0;
	ID3D10Blob* vertexShaderBuffer		= 0;
	ID3D10Blob* pixelShaderBuffer		= 0;
	ID3D10Blob*	geometryShaderBuffer	= 0;

	D3D11_INPUT_ELEMENT_DESC layoutDesc[2];
	unsigned int numElements;

	
	result = D3DX11CompileFromFile("../build3/shaders/test.vs", NULL, NULL, "VShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &vertexShaderBuffer, &errorMessage, NULL);
	if( FAILED(result) )
	{
		if(errorMessage)
			OutputShaderErrorMessage(errorMessage, _hWnd, _vsFile);
		return false;
	}
	result = _device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &vertexShader);
	if( FAILED(result) )
		return false;

	result = D3DX11CompileFromFile("../build3/shaders/test.ps", NULL, NULL, "PShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &pixelShaderBuffer, &errorMessage, NULL);
	if( FAILED(result) )
	{
		if(errorMessage)
			OutputShaderErrorMessage(errorMessage, _hWnd, _psFile);
		return false;
	}
	result = _device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &pixelShader);
	if( FAILED(result) )
		return false;

	result = D3DX11CompileFromFile("../build3/shaders/spriteGS.gs", NULL, NULL, "GS", "gs_4_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &geometryShaderBuffer, &errorMessage, NULL);
	if( FAILED(result) )
	{
		if(errorMessage)
			OutputShaderErrorMessage(errorMessage, _hWnd, "Geometry Shader");
		else
			MessageBox(_hWnd, "Geometry Shader", "Missing Shader File", MB_OK);

		return false;
	}
	
	layoutDesc[0].SemanticName			= "ANCHOR";
	layoutDesc[0].SemanticIndex			= 0;
	layoutDesc[0].Format				= DXGI_FORMAT_R32G32_FLOAT;
	layoutDesc[0].InputSlot				= 0;
	layoutDesc[0].AlignedByteOffset		= 0;
	layoutDesc[0].InputSlotClass		= D3D11_INPUT_PER_VERTEX_DATA;
	layoutDesc[0].InstanceDataStepRate	= 0;

	layoutDesc[1].SemanticName			= "DIMENSIONS";
	layoutDesc[1].SemanticIndex			= 0;
	layoutDesc[1].Format				= DXGI_FORMAT_R32G32_FLOAT;
	layoutDesc[1].InputSlot				= 0;
	layoutDesc[1].AlignedByteOffset		= D3D11_APPEND_ALIGNED_ELEMENT;
	layoutDesc[1].InputSlotClass		= D3D11_INPUT_PER_VERTEX_DATA;
	layoutDesc[1].InstanceDataStepRate	= 0;

	numElements = sizeof(layoutDesc) / sizeof(layoutDesc[0]);

	result = _device->CreateInputLayout(layoutDesc, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &layout);

	if( FAILED(result) )
		return false;

	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	geometryShaderBuffer->Release();
	geometryShaderBuffer = 0;

	return true;
}

void TestShader::shutDown()
{

}

//bool TestShader::initializeShader(ID3D11Device* _device, HWND _hWnd, LPCSTR _vsFile,  WCHAR* _psFile)
//{
//
//}