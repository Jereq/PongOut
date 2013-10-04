#ifndef __BUTTON_H
#define __BUTTON_H

#include <string>
#include <glm/glm.hpp>

class Button
{
public:
	Button();
	~Button();

	bool				initialize(const std::string _textureName, const std::string _textName, 
					const glm::vec2 _size, const glm::vec3 _center, const std::string _func);

	virtual void		update(float _dt);
	const std::string	getTextureName();
	const std::string	getTextName();
	const bool			isMarked();
	const glm::vec2		getSize();
	const glm::vec3		getCenter();
public://private
	std::string textureName;
	std::string textName;
	std::string func;
	bool		marked;
	glm::vec2	size;
	glm::vec3	center;
};

#endif