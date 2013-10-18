#include "TestShader.h"

#include <D3DX10.h>
#include <D3DX11.h>
#include <D3DX11async.h>
#include <fstream>
#include "DXAssetInstancing.h"
#include <CoreSystem/ICoreSystem.h>

TestShader::TestShader() :	vertexShader(0), pixelShader(0), layout(0),
	sampleState(0), positionBuffer(0)
{	
}

TestShader::~TestShader()
{

}

ErrorCode TestShader::initialize(ID3D11Device* _device, HWND _hWnd)
{
	ErrorCode result;
	
	result = initializeShader(_device, _hWnd, "shaders/test.vs", "shaders/test.ps");
	
	return result;
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
	fout.open("../src/Platform_Windows/shaders/shader-error.txt");

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

ErrorCode TestShader::initializeShader(ID3D11Device* _device, HWND _hWnd, LPCSTR _vsFile, LPCSTR _psFile)
{
	HRESULT result;
	ID3D10Blob* errorMessage			= 0;
	ID3D10Blob* vertexShaderBuffer		= 0;
	ID3D10Blob* pixelShaderBuffer		= 0;
	ID3D10Blob*	geometryShaderBuffer	= 0;

	D3D11_INPUT_ELEMENT_DESC layoutDesc[2];
	unsigned int numElements;

	boost::filesystem::path rootDir = ICoreSystem::getInstance().lock()->getRootDir();
	
	result = D3DX11CompileFromFile((rootDir / _vsFile).string().c_str(), NULL, NULL, "VShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &vertexShaderBuffer, &errorMessage, NULL);
	if( FAILED(result) )
	{
		if(errorMessage)
			OutputShaderErrorMessage(errorMessage, _hWnd, _vsFile);
		return ErrorCode::WGFX_COMPILE_VS_FAIL;
	}
	result = _device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &vertexShader);
	if( FAILED(result) )
		return ErrorCode::WGFX_CREATE_VS_FAIL;

	result = D3DX11CompileFromFile((rootDir / _psFile).string().c_str(), NULL, NULL, "PShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &pixelShaderBuffer, &errorMessage, NULL);
	if( FAILED(result) )
	{
		if(errorMessage)
			OutputShaderErrorMessage(errorMessage, _hWnd, _psFile);
		return ErrorCode::WGFX_COMPILE_PS_FAIL;
	}
	result = _device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &pixelShader);
	if( FAILED(result) )
		return ErrorCode::WGFX_CREATE_PS_FAIL;

	result = D3DX11CompileFromFile((rootDir / "shaders/spriteGS.gs").string().c_str(), NULL, NULL, "GS", "gs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, NULL, &geometryShaderBuffer, &errorMessage, NULL);
	if( FAILED(result) )
	{
		if(errorMessage)
			OutputShaderErrorMessage(errorMessage, _hWnd, "Geometry Shader");
		else
			MessageBox(_hWnd, "Geometry Shader", "Missing Shader File", MB_OK);

		return ErrorCode::WGFX_COMPILE_GS_FAIL;
	}
	result = _device->CreateGeometryShader(geometryShaderBuffer->GetBufferPointer(), geometryShaderBuffer->GetBufferSize(), NULL, &geometryShader);
	if( FAILED(result) )
		return ErrorCode::WGFX_CREATE_GS_FAIL;

	layoutDesc[0].SemanticName			= "ANCHOR";
	layoutDesc[0].SemanticIndex			= 0;
	layoutDesc[0].Format				= DXGI_FORMAT_R32G32B32_FLOAT;
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
		return ErrorCode::WGFX_CREATE_INPUT_LAYOUT_FAIL;

	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;

	geometryShaderBuffer->Release();
	geometryShaderBuffer = 0;

	result = DXCREATE::createConstantBuffer(positionBuffer, _device);
	if( FAILED(result) )
		return ErrorCode::WGFX_CREATE_CBUFFER_FAIL;

	result = DXCREATE::createSampleState(sampleState, _device);
	if( FAILED(result) )
		return ErrorCode::WGFX_CREATE_SAMPLERSTATE_FAIL;

	D3D11_BLEND_DESC blendStateDesc;
	ZeroMemory(&blendStateDesc, sizeof(D3D11_BLEND_DESC));
	blendStateDesc.RenderTarget[0].BlendEnable			= true;
	blendStateDesc.RenderTarget[0].SrcBlend				= D3D11_BLEND_SRC_ALPHA;
	blendStateDesc.RenderTarget[0].DestBlend			= D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDesc.RenderTarget[0].BlendOp				= D3D11_BLEND_OP_ADD;
	blendStateDesc.RenderTarget[0].SrcBlendAlpha		= D3D11_BLEND_ONE;
	blendStateDesc.RenderTarget[0].DestBlendAlpha		= D3D11_BLEND_ZERO;
	blendStateDesc.RenderTarget[0].BlendOpAlpha			= D3D11_BLEND_OP_ADD;
	blendStateDesc.RenderTarget[0].RenderTargetWriteMask= 0x0f;

	result = _device->CreateBlendState(&blendStateDesc, &blendState);

	if( FAILED(result) )
		return ErrorCode::WGFX_CREATE_BLENDSTATE_FAIL;

	return ErrorCode::WGFX_INITSHADER_OK;
}

void TestShader::shutDown()
{

}

ErrorCode TestShader::draw(ID3D11DeviceContext* _deviceContext, ID3D11Buffer* _vbuffer, ID3D11Buffer* _ibuffer, ID3D11ShaderResourceView* _srView, int _indexCount)
{
	ErrorCode result;
	result = setShaderParamaters(_deviceContext, _vbuffer, _ibuffer, _srView);
	if(result != ErrorCode::WGFX_SET_SHADER_PARAMETERS_OK)
		return ErrorCode::WGFX_SET_SHADER_PARAMETERS_FAIL;

	drawShader(_deviceContext, _indexCount);

	return ErrorCode::WGFX_DRAW_OK;
}

ErrorCode TestShader::setShaderParamaters(ID3D11DeviceContext* _deviceContext, ID3D11Buffer* _vbuffer, ID3D11Buffer* _ibuffer, ID3D11ShaderResourceView* _srView)
{
	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset.
	stride = sizeof(SpriteVertex); 
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	_deviceContext->IASetVertexBuffers(0, 1, &_vbuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	//_deviceContext->IASetIndexBuffer(_ibuffer, DXGI_FORMAT_R32_UINT, 0);
	
	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	// Set the vertex input layout.
	_deviceContext->IASetInputLayout(layout);

	// Set the vertex and pixel shaders that will be used to render this triangle.
	_deviceContext->VSSetShader(vertexShader,	NULL, 0);
	_deviceContext->GSSetShader(geometryShader,	NULL, 0);
	_deviceContext->PSSetShader(pixelShader,	NULL, 0);
	_deviceContext->PSSetShaderResources(0, 1, &_srView);
	_deviceContext->PSSetSamplers(0, 1, &sampleState);

	float blendFactor[4];
	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;
	_deviceContext->OMSetBlendState(blendState, blendFactor, 0xffffffff);

	return ErrorCode::WGFX_SET_SHADER_PARAMETERS_OK;
}


void TestShader::drawShader(ID3D11DeviceContext* _deviceContext, int indexCount)
{
	//_deviceContext->DrawIndexed(indexCount,0,0);
	_deviceContext->Draw(indexCount, 0);
}