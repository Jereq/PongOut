#ifndef __ASSETCONTAINER_H
#define __ASSETCONTAINER_H

#include "Asset.h"

template<typename VB, typename IB, typename TEX>
class Sprite
{
public:
	Sprite(std::string _name = "EMPTY")
		: name(_name)
	{}

	virtual ~Sprite()
	{}

	bool initialize(std::string _name, Asset<VB>* _vBuffer, Asset<IB>* _iBuffer, Asset<TEX>* _texture)
	{
		if( name != "EMPTY")
			return false;

		name		= _name;
		vBuffer	= _vBuffer->getData();
		iBuffer	= _iBuffer->getData();
		//texture	= _texture->getData();

		return true;
	}

	ID3D11ShaderResourceView* getTexture()
	{
		return texture;
	}

	ID3D11Buffer* getVBuffer()
	{
		return vBuffer;
	}

	ID3D11Buffer* getIBuffer()
	{
		return iBuffer;
	}
	virtual void clear() = 0;

protected:
	std::string	name;
	VB*		vBuffer;
	IB*		iBuffer;
	TEX*	texture;
};

#endif