#ifndef __GRAPHICSWINDOWS_H
#define __GRAPHICSWINDOWS_H

#include "../IGraphics.h"
#include "D3D.h"
#include "TestShader.h"
#include "DXAssets.h"

class GraphicsWindows : public IGraphics
{
public:
	GraphicsWindows(WNDPROC _wndProc);
	~GraphicsWindows();
	
protected:

private:
	WNDPROC wndProc;

	D3D*	d3d;

	TestShader*	testShader;

	virtual bool init();
	virtual void destroy();

	virtual bool loadResources(const boost::filesystem::path& _resourceDir);


	virtual void addRectangle(glm::vec3 _center, glm::vec2 _size, float _rotation, std::string id);

	virtual void drawFrame();

	VertexBuffer*	vBufferAsset;
	IndexBuffer*	iBufferAsset;
	DXSprite*		sprite;
	unsigned int	vertexCount;
	unsigned int	indexCount;
};

#endif