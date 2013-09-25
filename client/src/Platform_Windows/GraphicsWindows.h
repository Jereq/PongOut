#ifndef __GRAPHICSWINDOWS_H
#define __GRAPHICSWINDOWS_H

#include "../IGraphics.h"
#include "D3D.h"
#include "TestShader.h"
#include <map>


typedef ID3D11ShaderResourceView* SRV;

struct sprite;
//{
//	SRV bufferTexture;
//	std::vector<SpriteVertex> vertices;
//};

class GraphicsWindows : public IGraphics
{
public:
	GraphicsWindows(HWND _hWnd);
	~GraphicsWindows();
	
protected:

private:
	D3D*	d3d;
	HWND	hWnd;

	TestShader*	testShader;

	virtual bool init();
	virtual void destroy();

	virtual bool loadResources(const boost::filesystem::path& _resourceDir);
	virtual void addRectangle(glm::vec3 _center, glm::vec2 _size, float _rotation, std::string id);

	virtual void drawFrame();

	unsigned int	vertexCount;
	unsigned int	indexCount;

	std::vector<sprite>			frameSprites;
	std::map<std::string, SRV>	textures;


	ID3D11Buffer* vBuf;
	ID3D11Buffer* iBuf;

	bool createBuffers(ID3D11Buffer*& _vBuffer, ID3D11Buffer*& _iBuffer, ID3D11Device* _device, int _index);

};

#endif