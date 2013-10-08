#pragma once

#include <glm/glm.hpp>
#include <string>
#include "Input/IInput.h"
#include "IGraphics.h"

class InputField
{
private:
	enum class State
	{
		Normal,
		Hovered,
		Pressed,
		Selected,
		Disabled,
	};

	State		state;
	std::string id;
	std::string	text;
	std::string textureName;
	glm::vec3	center;
	glm::vec2	size;
	bool		marked;
public:
	InputField();
	bool initialize(const std::string& _id, std::string& _textureName, glm::vec3 _center, glm::vec2 _size);
	void onInput(IInput::Event _event);
	void draw(IGraphics::ptr _graphics);

	std::string getId() const;
	std::string getText() const;
};
