#include "GraphicsWindows.h"
#include "DXAssetInstancing.h"

struct sprite
{
	SRV bufferTexture;
	std::vector<SpriteVertex> vertices;
	std::string id;
};

GraphicsWindows::GraphicsWindows(HWND _hWnd)
	: d3d(0), hWnd(_hWnd), IGraphics(), vBuf(0), iBuf(0)
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

	SRV tex = NULL;
	result = DXCREATE::createTexture("../resources/textures/box_dark_magenta_01.png", tex, d3d->device);
	textures.insert( std::pair<std::string, SRV>("blocks/dark_magenta_01", tex));
	result = DXCREATE::createTexture("../resources/textures/box_pink_01.png", tex, d3d->device);
	textures.insert( std::pair<std::string, SRV>("blocks/pink_01", tex));
	result = DXCREATE::createTexture("../resources/textures/box_orange_01.png", tex, d3d->device);
	textures.insert( std::pair<std::string, SRV>("blocks/orange_01", tex));
	result = DXCREATE::createTexture("../resources/textures/ball_prototype.png", tex, d3d->device);
	textures.insert( std::pair<std::string, SRV>("player/ball_prototype", tex));
	return result;
}


void GraphicsWindows::addRectangle(glm::vec3 _center, glm::vec2 _size, float _rotation, std::string id)
{
	std::map<std::string, SRV>::iterator it = textures.find(id);
	if(it == textures.end())	// dynamic loading
	{
		int clean = id.find_first_of('/');
		std::string cleanType = id.substr(0, clean+1);
		std::string path;
		clean = id.find_last_of('/');
		std::string cleanId = id.substr(clean+1, id.size());
		path = "../resources/textures/" + cleanId + ".png";
		SRV tex = NULL;
		bool result;
		result = DXCREATE::createTexture(path, tex, d3d->device);

		if(!result)
			return;

		textures.insert( std::pair<std::string, SRV>(cleanType + cleanId,tex));
		it = textures.find(id);
	}
	
	
	SpriteVertex sv = { (float)_center.x, (float)_center.y, (float)_center.z,	//center
						(float)_size.x, (float)_size.y };						//dimensions

	int index = -1;
	for(int i=0; i<frameSprites.size();i++)
	{
		if(id == frameSprites[i].id)
		{
			index = i;
			break;
		}
	}
	if(index < 0)	// we need to make a new entry in framesprites
	{
		sprite s;
		s.id = id;
		s.bufferTexture = it->second;
		s.vertices.push_back(sv);
		frameSprites.push_back(s);
	
	}
	else	// just put the appropriate data in already existing entry
	{
		frameSprites[index].vertices.push_back(sv);
	}

	return;
}

bool GraphicsWindows::createBuffers(ID3D11Buffer*& _vBuffer, ID3D11Buffer*& _iBuffer, ID3D11Device* _device, int _index)
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
		return false;

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
		return false;

	delete [] indices;
	indices = 0;

	return true;
}

void GraphicsWindows::drawFrame()
{
	d3d->beginScene(0.f, 0.f, 0.f, 1.f);

	
	int index = 0;
	for (sprite s : frameSprites)
	{
		indexCount = s.vertices.size();
		SRV tex = s.bufferTexture;
		bool result = createBuffers(vBuf, iBuf, d3d->device, index);

		if(result)
			testShader->draw(d3d->deviceContext, vBuf, iBuf, tex, indexCount);

		index++;
	}

	for (sprite s : frameSprites)
		s.vertices.clear();

	frameSprites.clear();

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
	d3d->endScene();
}
