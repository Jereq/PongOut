#ifndef __MENUSTATE_H
#define __MENUSTATE_H

#include "ScreenState.h"
#include "Button.h"
#include "InputField.h"
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
	void addInputFields(std::vector<InputField> _inputFields);
	void setBackground(const std::string _backgroundName);
	void setMusic(const std::string _music);

	std::string getText(const std::string& _elemId) const override;

private:
	std::vector<Button> buttons;
	std::vector<Image> images;
	std::vector<InputField> inputFields;
	std::string music;
	virtual bool initialize(std::shared_ptr<ICoreSystem> _iCoreSystem);
	virtual void onInput(const std::vector<IInput::Event> _events);
	virtual bool onEntry();
	virtual void update(const float _dt);
	virtual void draw(std::shared_ptr<IGraphics> _graphics);
	virtual bool onEntry();
};

#endif
