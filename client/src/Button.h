#ifndef __BUTTON_H
#define __BUTTON_H

#include "GUIActionHandler.h"
#include "Input/IInput.h"
#include "IGraphics.h"

#include <string>
#include <glm/glm.hpp>

class Button
{
public:
	Button(GUIActionHandler* _actionHandler = nullptr);
	~Button();

	bool				initialize(const std::string& _backgroundNormal, const std::string& _backgroundHovered, std::string& _textName, 
						const glm::vec2 _size, const glm::vec3 _center, const std::string _func);


	void				onInput(IInput::Event _event);
	virtual void		update(float _dt);
	void				draw(IGraphics::ptr _graphics);

	const bool			isMarked();
	const glm::vec2		getSize();
	const glm::vec3		getCenter();
	std::string			getFunc() const;

private:
	enum class State
	{
		Normal,
		Hovered,
		Pressed,
		Disabled,
	};

	State		state;
	std::string backgroundNormal;
	std::string backgroundHovered;
	std::string textName;

	std::string func;
	bool		marked;
	glm::vec2	size;
	glm::vec3	center;
	GUIActionHandler*	actionHandler;



};

#endif
