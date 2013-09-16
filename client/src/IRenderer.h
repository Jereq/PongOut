#pragma once

#include <boost/filesystem.hpp>

class IRenderer
{
public:
	virtual ~IRenderer() {};

	virtual bool init() =0;
	virtual void destroy() =0;

	virtual bool loadImage(const boost::filesystem::path& _imagePath) =0;
	virtual void draw() =0;
};
