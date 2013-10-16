#pragma once

#include <boost/filesystem.hpp>
#include <glm/glm.hpp>

struct FT_LibraryRec_;
struct FT_FaceRec_;
struct FT_GlyphSlotRec_;

class Font
{
public:
	enum class ErrorCode
	{
		OK,
		INIT_LIBRARY_FAILED,
		FONT_LOAD_FAILED,
		SIZE_FAILED,
		GLYPH_FAILED,
	};

	struct Glyph
	{
		std::vector<unsigned char> bitBuffer;
		size_t width;
		size_t height;
		glm::vec2 origin;
		glm::vec2 advance;
	};

private:
	static FT_LibraryRec_* library;
	static ErrorCode initLibrary();

	boost::filesystem::path fontPath;
	unsigned int size;
	FT_FaceRec_* face;

public:
	Font();
	~Font();

	ErrorCode init(const boost::filesystem::path& _fontPath, unsigned int _size);

	ErrorCode getGlyph(Glyph& _glyphOut, char32_t _character);

private: // Delete copy functions
	Font(Font&);
	Font& operator=(Font&);
};
