#ifndef __MENUSTATE_H
#define __MENUSTATE_H

#include "ScreenState.h"
#include "Button.h"
#include <vector>

class MenuState : public ScreenState
{
public:
	MenuState(const std::string _screenName = "UNDEFINED");
	~MenuState();

private:
	std::vector<Button*> buttons;

	virtual bool initialize();
	virtual void onInput(const std::vector<IInput::Event> _events);
	virtual void update(const float _dt);
	virtual void draw(std::shared_ptr<IGraphics> _graphics);
};

#endif
