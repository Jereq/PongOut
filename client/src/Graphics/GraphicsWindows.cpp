#include "GraphicsWindows.h"
#include "DXAssetInstancing.h"

#include <ResourceLoader/ResourceLoader.h>

struct sprite
{
	SRV bufferTexture;
	std::vector<SpriteVertex> vertices;
	std::string id;
};

GraphicsWindows::GraphicsWindows(WNDPROC _wndProc)
	: d3d(0), wndProc(_wndProc), hWnd(nullptr)
{
}

GraphicsWindows::~GraphicsWindows()
{

}

HWND GraphicsWindows::getHWND()
{
	return hWnd;
}

void GraphicsWindows::destroy()
{
	hWnd = nullptr;
}


bool GraphicsWindows::loadResources(const boost::filesystem::path& _resourceDir)
{
	std::vector<ResourceLoader::Resource> textureResources;
	ResourceLoader::ErrorCode err = ResourceLoader::getResources(textureResources, _resourceDir, "texture");
	if (err == ResourceLoader::ErrorCode::INVALID_FORMAT)
	{
		std::cout << "Warning: " << _resourceDir / "resources.txt" << " contains invalid formatting." << std::endl;
	}
	else if (err != ResourceLoader::ErrorCode::SUCCESS)
	{
		std::cout << "Error: Could not load resources from" << _resourceDir << std::endl;
		return false;
	}

	for (auto texRes : textureResources)
	{
		SRV tex = NULL;
		ErrorCode result;
		result = DXCREATE::createTexture(texRes.path.string(), tex, d3d->device);

		if (result != ErrorCode::G_OK)
		{
			return false;
		}

		textures.insert(std::make_pair(texRes.name, tex));
	}

	return true;
}

bool GraphicsWindows::init()
{
	ErrorCode result;

	HMODULE hInstance = GetModuleHandle(nullptr);
	
	WNDCLASSEXW  wndClass;

	LPCWSTR szAppName = L"PongOut";

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

	RegisterClassExW(&wndClass);

	hWnd = CreateWindowExW(WS_EX_LEFT, szAppName, L"PongOut",
				WS_OVERLAPPEDWINDOW,
				CW_USEDEFAULT, CW_USEDEFAULT,
				1280,1024,
				NULL, NULL, hInstance, NULL);

	ShowWindow(hWnd, SW_SHOWNORMAL);
	UpdateWindow(hWnd);
	
	d3d = new D3D();

	if(d3d != NULL)
		result = d3d->initialize(hWnd);

	if(result != ErrorCode::WGFX_OK)
	{
		MessageBox(hWnd, "Failed to initialize Direct3D", "Error", MB_OK);
		return false;
	}


	testShader = new TestShader();
	result = testShader->initialize(d3d->device, hWnd);


	return true;
}


void GraphicsWindows::addRectangle(glm::vec3 _center, glm::vec2 _size, float _rotation, std::string _id)
{
	if (textures.count(_id) == 0)
	{
		_id = "textureNotFound";
		//ErrorCode result;
		//if(result != ErrorCode::WGFX_OK)
		//	return;
	}
	
	SpriteVertex sv = { (float)_center.x, (float)_center.y, (float)_center.z,	//center
						(float)_size.x, (float)_size.y };						//dimensions

	int index = -1;
	for(size_t i=0; i<frameSprites.size();i++)
	{
		if(_id == frameSprites[i].id)
		{
			index = i;
			break;
		}
	}
	if(index < 0)	// we need to make a new entry in framesprites
	{
		sprite s;
		s.id = _id;
		s.bufferTexture = textures.at(_id);
		s.vertices.push_back(sv);
		frameSprites.push_back(s);
	
	}
	else	// just put the appropriate data in already existing entry
	{
		frameSprites[index].vertices.push_back(sv);
	}

	return;
}

ErrorCode GraphicsWindows::createBuffers(ID3D11Buffer*& _vBuffer, ID3D11Buffer*& _iBuffer, ID3D11Device* _device, int _index)
{
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
		
	vertexCount				= frameSprites[_index].vertices.size();
	indexCount				= vertexCount;
	unsigned long* indices	= new unsigned long[indexCount];
	SpriteVertex*sv			= new SpriteVertex[vertexCount];

	memset(sv, 0, sizeof(SpriteVertex) * vertexCount);
	memcpy(sv, &frameSprites[_index].vertices[0], sizeof(SpriteVertex) * vertexCount);//we need to do a conversion from vector to * because of pSysMem

	unsigned int i;
	for(i=0; i<indexCount; i++)
		indices[i] = i;

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage				= D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth			= sizeof(SpriteVertex) * vertexCount;
	vertexBufferDesc.BindFlags			= D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags			= 0;
	vertexBufferDesc.StructureByteStride= 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem			= sv;
	vertexData.SysMemPitch		= 0;
	vertexData.SysMemSlicePitch	= 0;

	// create the vertex buffer.
	result = _device->CreateBuffer(&vertexBufferDesc, &vertexData, &_vBuffer);
	if(FAILED(result))
		return ErrorCode::WGFX_BUFFER_INIT_FAIL;

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage				= D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth			= sizeof(unsigned long) * indexCount;
	indexBufferDesc.BindFlags			= D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags		= 0;
	indexBufferDesc.MiscFlags			= 0;
	indexBufferDesc.StructureByteStride	= 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem			= indices;
	indexData.SysMemPitch		= 0;
	indexData.SysMemSlicePitch	= 0;

	// Create the index buffer.
	result = _device->CreateBuffer(&indexBufferDesc, &indexData, &_iBuffer);
	if(FAILED(result))
		return ErrorCode::WGFX_BUFFER_INIT_FAIL;

	delete [] indices;
	indices = 0;

	return ErrorCode::WGFX_BUFFER_INIT_OK;
}

void GraphicsWindows::drawFrame()
{
	d3d->beginScene(0.f, 0.f, 0.f, 1.f);

	
	int index = 0;
	for (sprite s : frameSprites)
	{
		indexCount = s.vertices.size();
		SRV tex = s.bufferTexture;
		ErrorCode result = createBuffers(vBuf, iBuf, d3d->device, index);

		if(result == ErrorCode::WGFX_OK)
			testShader->draw(d3d->deviceContext, vBuf, iBuf, tex, indexCount);

		index++;
		
		if( vBuf != NULL )
		{
			vBuf->Release();
			vBuf = NULL;
		}
		if( iBuf != NULL )
		{
			iBuf->Release();
			iBuf = NULL;
		}
	}

	for (sprite s : frameSprites)
		s.vertices.clear();

	frameSprites.clear();


	d3d->endScene();
}
