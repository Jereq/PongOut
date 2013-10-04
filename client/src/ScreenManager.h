#ifndef __SCREENMANAGER_H
#define __SCREENMANAGER_H

#include "Input/IInput.h"
#include "ScreenState.h"

#include <map>
#include <vector>

class ScreenManager
{
public:
	ScreenManager();
	//ErrorCode
	~ScreenManager();

	void update(float _dt, std::shared_ptr<IGraphics> _graphics);
	bool onInput(const std::vector<IInput::Event>& _events);
	bool removeScreen(ScreenState* _screen);
	bool openScreen(const std::string& _stateId);

private:
	std::vector<ScreenState::ptr> screens;
	std::map<std::string, ScreenState::ptr> registeredStates;

	bool registerScreenState(const std::string& _stateId, ScreenState::ptr _state);
	bool goBack();
};

#endif