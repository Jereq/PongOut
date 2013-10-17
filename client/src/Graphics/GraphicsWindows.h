#ifndef __GRAPHICSWINDOWS_H
#define __GRAPHICSWINDOWS_H

#include "D3D.h"
#include "ErrorCodeList.h"
#include "Font.h"
#include "IGraphics.h"
#include "TestShader.h"
#include <map>


typedef ID3D11ShaderResourceView* SRV;

struct sprite;
struct SpriteVertex;
//{
//	SRV bufferTexture;
//	std::vector<SpriteVertex> vertices;
//};

class GraphicsWindows : public IGraphics
{
public:
	typedef std::shared_ptr<GraphicsWindows> ptr;

	GraphicsWindows(WNDPROC _wndProc);
	~GraphicsWindows();

	HWND getHWND();
	
protected:

private:
	WNDPROC wndProc;
	HWND hWnd;

	D3D*	d3d;

	TestShader*	testShader;

	virtual bool init();
	virtual void destroy();

	virtual bool loadResources(const boost::filesystem::path& _resourceDir);
	virtual void addRectangle(glm::vec3 _center, glm::vec2 _size, float _rotation, std::string id);
	virtual ErrorCode addText(const std::string& _fontId, glm::vec3 _startPos, glm::vec2 _letterSize, const std::string& _text) override;

	virtual void drawFrame();

	unsigned int	vertexCount;
	unsigned int	indexCount;

	std::vector<sprite>			frameSprites;
	std::map<SRV, std::vector<SpriteVertex>> frameChars;
	std::map<std::string, SRV>	textures;
	
	struct LoadedChar
	{
		SRV texture;
		glm::vec2 origin;
		glm::vec2 advance;
		glm::vec2 size;
	};

	std::map<char32_t, LoadedChar> loadedChars;
	std::map<std::string, Font> loadedFonts;

	ID3D11Buffer* vBuf;
	ID3D11Buffer* iBuf;

	//ErrorCode createBuffers(ID3D11Buffer*& _vBuffer, ID3D11Buffer*& _iBuffer, ID3D11Device* _device, int _index);
	ErrorCode createBuffers(ID3D11Buffer*& _vBuffer, ID3D11Device* _device, std::vector<SpriteVertex>& _sv);

	ErrorCode loadChar(LoadedChar& _charOut, Font& _font, char32_t _character);
	ErrorCode getChar(LoadedChar& _charOut, Font& _font, char32_t _character);
};

#endif