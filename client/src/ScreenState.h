#ifndef __SCREENSTATE_H
#define __SCREENSTATE_H

#include <Graphics/IGraphics.h>
#include <Input/IInput.h>
#include <string>

class ScreenState
{
public:
	typedef std::shared_ptr<ScreenState> ptr;

	ScreenState(const std::string _screenName);
	~ScreenState();

	virtual void onInput(const std::vector<IInput::Event> _events);
	virtual void update(const float _dt);
	virtual void draw(std::shared_ptr<IGraphics> _graphics);
	virtual bool initialize();
	virtual bool onExit();
	virtual bool onEntry();

	virtual std::string getText(const std::string& _elemId) const =0;

	bool isInitialized() const;
	bool isActive() const;
	bool isPopup() const;
	std::string getScreenName() const;

protected:
	std::string getBackgroundName() const;
	bool		initialized;
	bool		active;
	bool		popup;

//private:
	std::string screenName;
	std::string backgroundName;

};

#endif
