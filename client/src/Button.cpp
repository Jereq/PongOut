#include "Button.h"

Button::Button()
{

}

Button::~Button()
{

}

bool Button::initialize(const std::string _textureName, const std::string _textName, const glm::vec2 _size, const glm::vec3 _center)
{
	textureName = _textureName;
	textName	= _textName;
	size		= _size;
	center		= _center;

	return true;
}

const std::string Button::getTextureName()
{
	return textureName;
}

const std::string Button::getTextName()
{
	return textName;
}

const bool Button::isMarked()
{
	return marked;
}

const glm::vec2 Button::getSize()
{
	return size;
}

const glm::vec3 Button::getCenter()
{
	return center;
}

void Button::update(float _dt)
{
	//switch for interpolation later
	if(marked)
		size = glm::vec2(0.25,0.25);
	else
		size = glm::vec2(0.125,0.125);
}