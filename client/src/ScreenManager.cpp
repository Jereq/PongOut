#include "ScreenManager.h"

ScreenManager::ScreenManager()
{

}

ScreenManager::~ScreenManager()
{
}

bool ScreenManager::addScreen(ScreenState* _screen)
{
	bool isDuplicate = false;
	unsigned int i;
	for(i=0; i < screens.size(); i++)
	{
		// screen already exists!
		if(_screen->getScreenName() == screens[i]->getScreenName())
		{
			isDuplicate = true;
			break;
		}
	}

	//new instance, push on stack
	if(!isDuplicate)
	{
		_screen->initialize();
		screens.push_back(_screen);
	}
	else //instance already exists, delete imposter
	{
		delete _screen;
		return false;
	}

	return true;
}

void ScreenManager::update(float _dt, std::shared_ptr<IGraphics> _graphics)
{
	//relevant screens for this frame
	std::vector<ScreenState*> screensToUpdate;

	for(ScreenState* screen : screens)
	{
		if(screen->isActive())
			screensToUpdate.push_back(screen);
	}

	int numScreens = screensToUpdate.size();
	int i;
	unsigned int topScreen = numScreens;
	float dt = 42.0f;

	for(i=numScreens; i>0; i--)
	{
		ScreenState* currentScreen = screensToUpdate[i-1];

		//only topscreen should receive I/O
		if(i == topScreen)
		{
			currentScreen->onInput();
		}

		//several screens can be active at once
		if(currentScreen->isActive())
		{
			currentScreen->update(dt);
		}
	}

	for(i=numScreens; i>0; i--)
	{
		ScreenState* currentScreen = screensToUpdate[i-1];
		currentScreen->draw(_graphics);
	}
}