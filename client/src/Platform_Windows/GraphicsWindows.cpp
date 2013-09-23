#include "GraphicsWindows.h"


GraphicsWindows::GraphicsWindows(HWND _hWnd)
	: d3d(0), hWnd(_hWnd), IGraphics()
{
	// this should be called outside this class
	//init();
}

GraphicsWindows::~GraphicsWindows()
{

}

void GraphicsWindows::destroy()
{

}

bool GraphicsWindows::loadImage(const boost::filesystem::path& _imagePath)
{

	return true;
}

bool GraphicsWindows::loadResources(const boost::filesystem::path& _resourceDir)
{

	return true;
}

bool GraphicsWindows::init()
{
	bool result = false;
	
	d3d = new D3D();

	if(d3d != NULL)
		result = d3d->initialize(hWnd);

	if(!result)
	{
		MessageBox(hWnd, "Failed to initialize Direct3D", "Error", MB_OK);
		return false;
	}


	testShader = new TestShader();
	result = testShader->initialize(d3d->device, hWnd);

	return result;
}

struct P
{
	float center[2];
	float dim[2];
};

void GraphicsWindows::addRectangle(glm::vec2 _center, glm::vec2 _size, float _rotation, std::string id)
{

}

void GraphicsWindows::drawFrame()
{
	d3d->beginScene(0.f, 0.f, 0.f, 1.f);

	d3d->endScene();
}