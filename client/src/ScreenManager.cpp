#include "ScreenManager.h"

#include "MenuState.h"

ScreenManager::ScreenManager()
{
	registerScreenState("login", ScreenState::ptr(new MenuState("login")));
}

ScreenManager::~ScreenManager()
{
}

void ScreenManager::update(float _dt, std::shared_ptr<IGraphics> _graphics)
{
	//relevant screens for this frame
	std::vector<ScreenState::ptr> screensToUpdate;

	for(auto screen : screens)
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
		ScreenState::ptr currentScreen = screensToUpdate[i-1];

		//several screens can be active at once
		if(currentScreen->isActive())
		{
			currentScreen->update(dt);
		}
	}

	for(i=numScreens; i>0; i--)
	{
		ScreenState::ptr currentScreen = screensToUpdate[i-1];
		currentScreen->draw(_graphics);
	}
}

bool ScreenManager::onInput(const std::vector<IInput::Event>& _events)
{
	//for (auto it = screens.rbegin(); it !=
	screens.back()->onInput(_events);
	return true;
}

bool ScreenManager::registerScreenState(const std::string& _stateId, ScreenState::ptr _state)
{
	if (registeredStates.count(_stateId) != 0)
	{
		return false;
	}

	registeredStates[_stateId] = _state;

	return true;
}

bool ScreenManager::openScreen(const std::string& _stateId)
{
	if (registeredStates.count(_stateId) == 0)
	{
		return false;
	}

	ScreenState::ptr state = registeredStates.at(_stateId);

	if (!state->isInitialized())
	{
		if (!state->initialize())
		{
			return false;
		}
	}

	screens.push_back(state);
	screens.back()->onEntry();

	return true;
}

bool ScreenManager::goBack()
{
	if (screens.size() < 2)
	{
		return false;
	}

	screens.back()->onExit();
	screens.pop_back();
	screens.back()->onEntry();

	return true;
}