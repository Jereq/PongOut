#include "GraphicsWindows.h"

#include "DXAssetInstancing.h"
#include <ResourceLoader/ResourceLoader.h>

#include <iostream>
#include <utf8.h>

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
	ResourceLoader::ErrorCode err = ResourceLoader::getResources(textureResources, _resourceDir);
	if (err == ResourceLoader::ErrorCode::INVALID_FORMAT)
	{
		std::cout << "Warning: " << _resourceDir / "resources.txt" << " contains invalid formatting." << std::endl;
	}
	else if (err != ResourceLoader::ErrorCode::SUCCESS)
	{
		std::cout << "Error: Could not load resources from" << _resourceDir << std::endl;
		return false;
	}

	for (auto res : textureResources)
	{
		if (res.type == "texture")
		{
			SRV tex = NULL;
			ErrorCode result;
			result = DXCREATE::createTexture(res.path.string(), tex, d3d->device);

			if (result != ErrorCode::G_OK)
			{
				return false;
			}

			textures.insert(std::make_pair(res.name, tex));
		}
		else if (res.type == "font")
		{
			Font f;
			Font::ErrorCode fErr = f.init(res.path, 64, false);
			if (fErr != Font::ErrorCode::OK)
			{
				return false;
			}

			loadedFonts.insert(std::make_pair(res.name, f));
		}
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
				1280,720,
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

ErrorCode GraphicsWindows::addText(const std::string& _fontId, glm::vec3 _startPos, glm::vec2 _letterSize, const std::string& _text)
{
	if (loadedFonts.count(_fontId) == 0)
	{
		return ErrorCode::G_INVALID_ARGUMENT;
	}

	Font& font = loadedFonts.at(_fontId);

	auto utf8It = _text.begin();
	auto itEnd = _text.end();

	if (!utf8::is_valid(utf8It, itEnd))
	{
		return ErrorCode::G_INVALID_ARGUMENT;
	}

	glm::vec3 penPos = _startPos;

	while (utf8It != itEnd)
	{
		char32_t character32 = utf8::next(utf8It, itEnd);

		LoadedChar c;
		ErrorCode err = getChar(c, font, character32);
		if (err != ErrorCode::G_OK)
		{
			return err;
		}

		float scale = 1.f / font.getSize();
		glm::vec3 posScale(_letterSize.x * scale, _letterSize.y * scale, 1.f);
		glm::vec2 size = _letterSize * c.size * scale;

		glm::vec3 pos = penPos + glm::vec3(c.origin.x, -c.size.y + c.origin.y, 0.f) * posScale + glm::vec3(size, 0.f) * 0.5f;

		SpriteVertex sv = {
			pos.x, pos.y, pos.z,
			size.x, size.y
		};

		frameChars[c.texture].push_back(sv);

		penPos += glm::vec3(c.advance.x, c.advance.y, 0.f) * posScale;
	}

	return ErrorCode::G_OK;
}

ErrorCode GraphicsWindows::createBuffers(ID3D11Buffer*& _vBuffer, ID3D11Device* _device, std::vector<SpriteVertex>& _sv)
{
	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
		
	vertexCount				= _sv.size();
	indexCount				= vertexCount;
	unsigned long* indices	= new unsigned long[indexCount];

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
	vertexData.pSysMem			= &_sv[0];//sv;
	vertexData.SysMemPitch		= 0;
	vertexData.SysMemSlicePitch	= 0;

	// create the vertex buffer.
	result = _device->CreateBuffer(&vertexBufferDesc, &vertexData, &_vBuffer);
	if(FAILED(result))
		return ErrorCode::WGFX_BUFFER_INIT_FAIL;

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
		ErrorCode result = createBuffers(vBuf, d3d->device, s.vertices);

		if(result == ErrorCode::WGFX_OK)
			testShader->draw(d3d->deviceContext, vBuf, NULL, tex, indexCount);

		index++;
		
		if( vBuf != NULL )
		{
			vBuf->Release();
			vBuf = NULL;
		}

	}

	for (sprite s : frameSprites)
		s.vertices.clear();

	frameSprites.clear();


	if (!frameChars.empty())
	{


		for (auto& c : frameChars)
		{
			if (c.second.empty())
			{
				continue;
			}

			auto& spriteVs = c.second;

			int icount = spriteVs.size();
			SRV tex = c.first;	
			ErrorCode result = createBuffers(vBuf, d3d->device, spriteVs);

			if(result == ErrorCode::WGFX_OK)
				testShader->draw(d3d->deviceContext, vBuf, NULL, tex, indexCount);

			spriteVs.clear();
			if( vBuf != NULL )
			{
				vBuf->Release();
				vBuf = NULL;
			}
		}
	}

	d3d->endScene();
}

ErrorCode GraphicsWindows::loadChar(LoadedChar& _charOut, Font& _font, char32_t _character)
{
	Font::Glyph glyph;
	Font::ErrorCode err = _font.getGlyph(glyph, _character);
	if (err != Font::ErrorCode::OK)
	{
		return ErrorCode::G_GLYPH_COULD_NOT_BE_LOADED;
	}

	//_charOut.texture

	HRESULT hr;
	ID3D11ShaderResourceView* rsv = NULL;
	D3D11_TEXTURE2D_DESC desc;
	desc.Width	= glyph.width;
	desc.Height	= glyph.height;
	desc.MipLevels	= 1;
	desc.ArraySize	= 1;
	desc.Format		= DXGI_FORMAT_A8_UNORM;
	desc.SampleDesc.Count	= 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage			= D3D11_USAGE_DEFAULT;
	desc.BindFlags		= D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags		= 0;

	D3D11_SUBRESOURCE_DATA sub;
	sub.pSysMem = glyph.bitBuffer.data();
	sub.SysMemPitch = glyph.width;
	sub.SysMemSlicePitch = 0;
	ID3D11Texture2D* tex = NULL;
	
	hr = d3d->device->CreateTexture2D(&desc, &sub, &tex);

	ID3D11ShaderResourceView* srv = NULL;
	hr = d3d->device->CreateShaderResourceView(tex,NULL, &srv);
	
	_charOut.origin = glyph.origin;
	_charOut.advance = glyph.advance;
	_charOut.size = glm::vec2(glyph.width, glyph.height);
	_charOut.texture = srv;
	return ErrorCode::G_OK;
}

ErrorCode GraphicsWindows::getChar(LoadedChar& _charOut, Font& _font, char32_t _character)
{
	auto it = loadedChars.find(_character);
	if (it == loadedChars.end())
	{
		LoadedChar c;
		ErrorCode err = loadChar(c, _font, _character);
		if (err != ErrorCode::G_OK)
		{
			return err;
		}

		auto resPair = loadedChars.insert(std::make_pair(_character, c));
		it = resPair.first;
	}

	_charOut = it->second;

	return ErrorCode::G_OK;
}
