#ifndef __ASSETCONTAINER_H
#define __ASSETCONTAINER_H

#include "Asset.h"

template<typename VB, typename IB, typename TEX>
class Sprite
{
public:
	Sprite(std::string _name = "EMPTY")
		: m_name(_name)
	{}

	virtual ~Sprite()
	{}

	bool initialize(std::string _name, Asset<VB>* _vBuffer, Asset<IB>* _iBuffer, Asset<TEX>* _texture)
	{
		if( m_name != "EMPTY")
			return false;

		m_name		= _name;
		m_vBuffer	= _vBuffer->getData();
		m_iBuffer	= _iBuffer->getData();
		m_texture	= _texture->getData();

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
	std::string	m_name;
	VB*		vBuffer;
	IB*		iBuffer;
	TEX*	texture;
};

#endif