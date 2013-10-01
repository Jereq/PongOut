#ifndef __DXASSETINSTANCING_H
#define __DXASSETINSTANCING_H

#include "D3DX11.h"
#include "D3DX10.h"
#include "ErrorCodeList.h"

struct SpriteVertex
{
	float center[3];
	float dimensions[2];
};

inline float convertPixelsToClipSpace( const int pixelDimension, const int pixels )
{
	float ret = (float)pixels/pixelDimension*2 -1;
    return ret;
}
 
inline float convertPixelsToClipSpaceDistance( const int pixelDimension, const int pixels )
{
	float ret = (float)pixels/pixelDimension*2;
    return ret;
}

struct PositionBufferType
{
	D3DXVECTOR2 position;
};



namespace DXCREATE
{
	static HRESULT createSampleState(ID3D11SamplerState*& _sampleState, ID3D11Device* _device)
	{
		HRESULT result;
		D3D11_SAMPLER_DESC samplerDesc;

		samplerDesc.Filter			= D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU		= D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV		= D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW		= D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.MipLODBias		= 0.0f;
		samplerDesc.MaxAnisotropy	= 1;
		samplerDesc.ComparisonFunc	= D3D11_COMPARISON_ALWAYS;
		samplerDesc.BorderColor[0]	= 0;
		samplerDesc.BorderColor[1]	= 0;
		samplerDesc.BorderColor[2]	= 0;
		samplerDesc.BorderColor[3]	= 0;
		samplerDesc.MinLOD			= 0;
		samplerDesc.MaxLOD			= D3D11_FLOAT32_MAX;

		result = _device->CreateSamplerState(&samplerDesc, &_sampleState);

		return result;
	}

	static ErrorCode createTexture(std::string _file, ID3D11ShaderResourceView*& _srView, ID3D11Device* _device)
	{
		HRESULT result;
		result = D3DX11CreateShaderResourceViewFromFile(_device, _file.c_str(), 0, 0, &_srView, 0);
		if( FAILED(result) )
			return ErrorCode::WGFX_TEXTURE_INIT_FAIL;

		return ErrorCode::WGFX_TEXTURE_INIT_OK;
	}

	static HRESULT createConstantBuffer(ID3D11Buffer*& _cBuffer, ID3D11Device* _device)
	{
		HRESULT result;
		//ID3D10Blob* errorMessage;
		D3D11_BUFFER_DESC cBufferDesc;

		cBufferDesc.Usage				= D3D11_USAGE_DYNAMIC;
		cBufferDesc.ByteWidth			= sizeof(PositionBufferType) * 8;
		cBufferDesc.BindFlags			= D3D11_BIND_CONSTANT_BUFFER;
		cBufferDesc.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
		cBufferDesc.MiscFlags			= 0;
		cBufferDesc.StructureByteStride	= 0;

		result = _device->CreateBuffer(&cBufferDesc, NULL, &_cBuffer);
		return result;
	}

};

#endif