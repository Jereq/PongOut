#include "GraphicsWindows.h"
#include "DXAssetInstancing.h"

GraphicsWindows::GraphicsWindows(HWND _hWnd)
	: d3d(0), hWnd(_hWnd), IGraphics()
{
}

GraphicsWindows::~GraphicsWindows()
{

}

void GraphicsWindows::destroy()
{

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

	sprite			= new DXSprite();
	vBufferAsset	= new VertexBuffer();
	iBufferAsset	= new IndexBuffer();

	ID3D11Buffer* vBuffer = NULL;
	ID3D11Buffer* iBuffer = NULL;
	

	result = DXCREATE::createBuffers(vBuffer, iBuffer, vertexCount, indexCount, d3d->device);

	vBufferAsset->initialize(vBuffer, "TestVertex");
	iBufferAsset->initialize(iBuffer, "TestIndex");
	sprite->initialize("testSprite", vBufferAsset, iBufferAsset, NULL);

	return result;
}

void GraphicsWindows::addRectangle(glm::vec3 _center, glm::vec2 _size, float _rotation, std::string id)
{

}

void GraphicsWindows::drawFrame()
{
	d3d->beginScene(0.f, 0.f, 0.f, 1.f);

	// testshader draw with testsprite
	testShader->draw(d3d->deviceContext, sprite);

	d3d->endScene();
}
