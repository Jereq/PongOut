#ifndef __SCREENSTATE_H
#define __SCREENSTATE_H

#include "IGraphics.h"
#include <string>

class ScreenState
{
public:
	ScreenState(const std::string _screenName);
	~ScreenState();

	virtual void onInput();
	virtual void update(const float _dt);
	virtual void draw(std::shared_ptr<IGraphics> _graphics);
	virtual bool initialize();

	const bool isActive();
	const bool isPopup();
	const std::string getScreenName();

protected:
	const std::string getBackgroundName();
	bool		active;
	bool		popup;

private:
	std::string screenName;
	std::string backgroundName;

};

#endif