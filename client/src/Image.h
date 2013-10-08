#ifndef __IMAGE_H
#define __IMAGE_H

#include <glm/glm.hpp>
#include <string>
#include "IGraphics.h"

class Image
{
public:
	bool initialize(const std::string& _id, const std::string& _imageName, glm::vec3 _center, glm::vec2 _size);
	void draw(IGraphics::ptr _graphics);

private:
	std::string id;
	std::string imageName;
	glm::vec3	center;
	glm::vec2	size;
};

#endif