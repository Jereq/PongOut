#pragma once

#include <boost/filesystem.hpp>

class IGraphics
{
public:
	virtual ~IGraphics() {};

	virtual bool init() =0;
	virtual void destroy() =0;

	virtual bool loadResources(...) =0;
	virtual bool loadImage(const boost::filesystem::path& _imagePath) =0;

	virtual void addRectangle(pos _center, size _size, float _rotation, idtype id) =0;

	virtual void drawFrame() =0;
};
