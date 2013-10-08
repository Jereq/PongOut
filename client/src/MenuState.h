#ifndef __MENUSTATE_H
#define __MENUSTATE_H

#include "ScreenState.h"
#include "Button.h"
#include <vector>
#include "Image.h"

class MenuState : public ScreenState
{
public:
	MenuState(const std::string _screenName = "UNDEFINED");
	~MenuState();

	typedef std::shared_ptr<MenuState> ptr;

	void addButtons(std::vector<Button> _buttons);
	void addImages(std::vector<Image> _images);
	void setBackground(const std::string _backgroundName);
private:
	std::vector<Button> buttons;
	std::vector<Image> images;
	virtual bool initialize();
	virtual void onInput(const std::vector<IInput::Event> _events);
	virtual void update(const float _dt);
	virtual void draw(std::shared_ptr<IGraphics> _graphics);
	
};

#endif
