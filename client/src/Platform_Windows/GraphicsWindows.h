#ifndef __GRAPHICSWINDOWS_H
#define __GRAPHICSWINDOWS_H

#include "../IGraphics.h"
#include "D3D.h"
#include "TestShader.h"
#include "DXAssets.h"

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
	virtual bool loadImage(const boost::filesystem::path& _imagePath);

	virtual void addRectangle(glm::vec2 _center, glm::vec2 _size, float _rotation, std::string id);

	virtual void drawFrame();
};

#endif