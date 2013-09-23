#ifndef __DXASSETS_H
#define __DXASSETS_H

#include "AssetContainer.h"
#include <d3d11.h>

class VertexBuffer : public Asset<ID3D11Buffer>
{
	public:
	VertexBuffer() : Asset()
	{}
	~VertexBuffer()
	{}

private:
	virtual bool clear()
	{ 
		if(getRefCount() > 0)
			return false;

		data->Release();
		data = NULL;
		name = "EMPTY";

		return true;
	}
};

class IndexBuffer : public Asset<ID3D11Buffer>
{
	public:
	IndexBuffer() : Asset()
	{}
	~IndexBuffer()
	{}

private:
	virtual bool clear()
	{ 
		if(getRefCount() > 0)
			return false;

		data->Release();
		data = NULL;
		name = "EMPTY";

		return true;
	}
};

class Texture : public Asset<ID3D11ShaderResourceView>
{
public:
	Texture(){}
	~Texture(){}

private:
	virtual bool clear()
	{ 
		if(getRefCount() > 0)
			return false;

		data->Release();
		data = NULL;
		name = "EMPTY";

		return true;
	}
};

class DXSprite : public Sprite<ID3D11Buffer, ID3D11Buffer, ID3D11ShaderResourceView>
{
public:
	DXSprite() : Sprite()
	{}

	~DXSprite()
	{}


private:
	virtual void clear()
	{

	}
};

#endif