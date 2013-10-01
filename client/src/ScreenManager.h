#ifndef __SCREENMANAGER_H
#define __SCREENMANAGER_H

#include "ScreenState.h"

#include <vector>

class ScreenManager
{
public:
	ScreenManager();
	~ScreenManager();

	void update(float _dt, std::shared_ptr<IGraphics> _graphics);
	bool addScreen(ScreenState* _screen);
	bool removeScreen(ScreenState* _screen);
private:
	std::vector<ScreenState*> screens;
};

#endif