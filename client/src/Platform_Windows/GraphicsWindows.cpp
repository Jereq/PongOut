#include "GraphicsWindows.h"
#include "DXAssetInstancing.h"

GraphicsWindows::GraphicsWindows(WNDPROC _wndProc)
	: d3d(0), wndProc(_wndProc)
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

	HMODULE hInstance = GetModuleHandle(nullptr);
	
	WNDCLASSEX  wndClass;

	const static char szAppName[] = "PongOut";

	wndClass.cbSize			= sizeof(wndClass);
	wndClass.style			= CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc	= wndProc;
	wndClass.cbClsExtra		= 0;
	wndClass.cbWndExtra		= 0;
	wndClass.hInstance		= hInstance;
	wndClass.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hIconSm		= LoadIcon(NULL, IDI_APPLICATION);
	wndClass.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground	= (HBRUSH) GetStockObject(WHITE_BRUSH);
	wndClass.lpszClassName	= szAppName;
	wndClass.lpszMenuName	= NULL;

	RegisterClassEx(&wndClass);

	HWND hWnd = CreateWindow(szAppName, "PongOut",
				WS_OVERLAPPEDWINDOW,
				CW_USEDEFAULT, CW_USEDEFAULT,
				CW_USEDEFAULT, CW_USEDEFAULT,
				NULL, NULL, hInstance, NULL);

	ShowWindow(hWnd, SW_SHOWNORMAL);
	UpdateWindow(hWnd);
	
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
