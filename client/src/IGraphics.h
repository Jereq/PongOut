#pragma once

#include <boost/filesystem.hpp>
#include <glm/glm.hpp>
#include <string>

class IGraphics
{
public:
	virtual ~IGraphics() {};

	virtual bool init() =0;
	virtual void destroy() =0;

	virtual bool loadResources(const boost::filesystem::path& _resourceDir) =0;
	virtual bool loadImage(const boost::filesystem::path& _imagePath) =0;

	virtual void addRectangle(glm::vec2 _center, glm::vec2 _size, float _rotation, std::string id) =0;

	virtual void drawFrame() =0;
};