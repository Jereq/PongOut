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
	bool flipOrigin;

public:
	Font();
	~Font();

	/**
	 *  Initialize font by loading the specified face.
	 *
	 *  @param _fontPath the path to the font file.
	 *  @param _size the pixel resolution needed. The result depends on the font.
	 *  @param _flipOrigin The origin is normally set to the top left. Set to true to flip the origin
	 *		to the lower left, otherwise set to false.
	 *  @return ErrorCode::OK if the font was loaded successfully. FONT_LOAD_FAILED if the font file could not be loaded.
	 *		SIZE_FAILED if the requested size could not be set.
	 */
	ErrorCode init(const boost::filesystem::path& _fontPath, unsigned int _size, bool _flipOrigin);

	ErrorCode getGlyph(Glyph& _glyphOut, char32_t _character);
	unsigned int getSize() const;
};
