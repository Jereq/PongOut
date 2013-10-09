#ifndef __GRAPHICSCOMPONENT_H
#define __GRAPHICSCOMPONENT_H

#include "IGraphics.h"

class GameObject;

class GraphicsComponent
{
private:
	std::shared_ptr<IGraphics> graphics;

public:

	typedef std::shared_ptr<GraphicsComponent> ptr;

	bool initialize(std::shared_ptr<IGraphics> _graphics);
	void addSpriteToFrame(const std::string& _textureId, glm::vec3 _center, glm::vec2 _size, float _rotation);
};

#endif