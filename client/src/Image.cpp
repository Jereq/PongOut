#include "Image.h"


bool Image::initialize(const std::string& _id, const std::string& _imageName, glm::vec3 _center, glm::vec2 _size)
{
	if(_id.empty() || _imageName.empty())
		return false;

	id			= _id;
	imageName	= _imageName;
	center		= _center;
	size		= _size;

	return true;
}

void Image::draw(IGraphics::ptr _graphics)
{
	_graphics->addRectangle(center, size, 0, imageName);
}