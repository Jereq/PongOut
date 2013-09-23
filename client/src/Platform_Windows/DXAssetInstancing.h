#ifndef __DXASSETINSTANCING_H
#define __DXASSETINSTANCING_H

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

struct SpriteVertex
{
	float center[2];
	float dimensions[2];
};

namespace DXCREATE
{
	static bool createBuffers(ID3D11Buffer*& _vBuffer, ID3D11Buffer*& _iBuffer, unsigned int& _vertexCount, unsigned int& _indexCount, ID3D11Device* _device)
	{
		SpriteVertex* vertices;
		unsigned long* indices;
		D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
		D3D11_SUBRESOURCE_DATA vertexData, indexData;
		HRESULT result;
		unsigned int i;


		// Set the number of vertices in the vertex array.
		_vertexCount = 1;

		// Set the number of indices in the index array.
		_indexCount = _vertexCount;

		// Create the vertex array.
		vertices = new SpriteVertex[_vertexCount];
		if(!vertices)
		{
			return false;
		}

		// Create the index array.
		indices = new unsigned long[_indexCount];
		if(!indices)
		{
			return false;
		}

		// Initialize vertex array to zeros at first.
		memset(vertices, 0, (sizeof(SpriteVertex) * _vertexCount));

		vertices[0].center[0] = 0.5f;//convertPixelsToClipSpace(1280,0);
		vertices[0].center[1] = 0.5f;//-convertPixelsToClipSpace(1024,0);
		vertices[0].dimensions[0] = 0.2f;//convertPixelsToClipSpaceDistance(1280,500);
		vertices[0].dimensions[1] = 0.2f;//convertPixelsToClipSpaceDistance(1024,402);

		//vertices[1].center[0] = convertPixelsToClipSpace(1280,0);
		//vertices[1].center[1] = -convertPixelsToClipSpace(1024,0);
		//vertices[1].dimensions[0] = convertPixelsToClipSpaceDistance(1280,10);
		//vertices[1].dimensions[1] = convertPixelsToClipSpaceDistance(1024,10);

	 //
		//vertices[2].center[0] = convertPixelsToClipSpace(1280,10);
		//vertices[2].center[1] = -convertPixelsToClipSpace(1024,10);
		//vertices[2].dimensions[0] = convertPixelsToClipSpaceDistance(1280,10);
		//vertices[2].dimensions[1] = convertPixelsToClipSpaceDistance(1024,10);

		// Load the index array with data.
		for(i=0; i<_indexCount; i++)
		{
			indices[i] = i;
		}


		// Set up the description of the static vertex buffer.
		vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		vertexBufferDesc.ByteWidth = sizeof(SpriteVertex) * _vertexCount;
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		vertexBufferDesc.MiscFlags = 0;
		vertexBufferDesc.StructureByteStride = 0;

		// Give the subresource structure a pointer to the vertex data.
		vertexData.pSysMem = vertices;
		vertexData.SysMemPitch = 0;
		vertexData.SysMemSlicePitch = 0;

		// Now create the vertex buffer.
		result = _device->CreateBuffer(&vertexBufferDesc, &vertexData, &_vBuffer);
		if(FAILED(result))
		{
			return false;
		}

		// Set up the description of the static index buffer.
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = sizeof(unsigned long) * _indexCount;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;
		indexBufferDesc.StructureByteStride = 0;

		// Give the subresource structure a pointer to the index data.
		indexData.pSysMem = indices;
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;

		// Create the index buffer.
		result = _device->CreateBuffer(&indexBufferDesc, &indexData, &_iBuffer);
		if(FAILED(result))
		{
			return false;
		}

		// Release the arrays now that the vertex and index buffers have been created and loaded.
		delete [] vertices;
		vertices = 0;

		delete [] indices;
		indices = 0;

		return true;
	}
};

#endif