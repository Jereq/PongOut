#include "Font.h"

#include <ft2build.h>
#include FT_FREETYPE_H

FT_Library Font::library = nullptr;

Font::ErrorCode Font::initLibrary()
{
	FT_Error error = FT_Init_FreeType(&library);
	if (error)
	{
		return ErrorCode::INIT_LIBRARY_FAILED;
	}

	return ErrorCode::OK;
}

Font::Font()
	: face(nullptr),
	  flipOrigin(false)
{
}

Font::~Font()
{
}

Font::ErrorCode Font::init(const boost::filesystem::path& _fontPath, unsigned int _size, bool _flipOrigin)
{
	ErrorCode err;

	fontPath = _fontPath;
	size = _size;
	flipOrigin = _flipOrigin;

	if (!library)
	{
		err = initLibrary();
		if (err != ErrorCode::OK)
		{
			return err;
		}
	}

	FT_Error error = FT_New_Face(Font::library, fontPath.string().c_str(), 0, &face);
	if (error)
	{
		return ErrorCode::FONT_LOAD_FAILED;
	}

	error = FT_Set_Pixel_Sizes(face, 0, _size);
	if (error)
	{
		return ErrorCode::SIZE_FAILED;
	}

	return ErrorCode::OK;
}

Font::ErrorCode Font::getGlyph(Glyph& _glyphOut, char32_t _character)
{
	FT_Error error = FT_Load_Char(face, _character, FT_LOAD_RENDER);
	if (error)
	{
		return ErrorCode::GLYPH_FAILED;
	}

	FT_GlyphSlot glyph = face->glyph;

	const unsigned int buffSize = glyph->bitmap.width * glyph->bitmap.rows;

	_glyphOut.bitBuffer.resize(buffSize);

	if (flipOrigin)
	{
		auto it(_glyphOut.bitBuffer.end());

		unsigned char* bitBuff = glyph->bitmap.buffer;

		for (int i = 0; i < glyph->bitmap.rows; i++)
		{
			it -= glyph->bitmap.width;
			for (int j = 0; j < glyph->bitmap.width; j++)
			{
				*it = bitBuff[j];
				++it;
			}
			it -= glyph->bitmap.width;

			bitBuff += glyph->bitmap.pitch;
		}
	}
	else
	{
		auto it(_glyphOut.bitBuffer.begin());

		unsigned char* bitBuff = glyph->bitmap.buffer;

		for (int i = 0; i < glyph->bitmap.rows; i++)
		{
			for (int j = 0; j < glyph->bitmap.width; j++)
			{
				*it = bitBuff[j];
				++it;
			}

			bitBuff += glyph->bitmap.pitch;
		}
	}

	_glyphOut.width = glyph->bitmap.width;
	_glyphOut.height = glyph->bitmap.rows;
	_glyphOut.origin = glm::vec2(glyph->bitmap_left, glyph->bitmap_top);
	_glyphOut.advance = glm::vec2(glyph->advance.x / 64.f, glyph->advance.y / 64.f);

	return ErrorCode::OK;
}

unsigned int Font::getSize() const
{
	return size;
}
