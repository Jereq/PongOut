#include "GraphicsWindows.h"


GraphicsWindows::GraphicsWindows(HWND _hWnd)
	: d3d(0), hWnd(_hWnd)
{
	// this should be called outside this class
	initialize();
}

GraphicsWindows::~GraphicsWindows()
{

}

bool GraphicsWindows::initialize()
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

void GraphicsWindows::drawFrame()
{

}