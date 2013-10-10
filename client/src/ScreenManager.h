#ifndef __SCREENMANAGER_H
#define __SCREENMANAGER_H

#include "FunctionHandler.h"
#include "GUIActionHandler.h"
#include "Input/IInput.h"
#include "ScreenState.h"

#include <map>
#include <vector>

class ScreenManager : public GUIActionHandler
{
public:
	ScreenManager(const boost::filesystem::path& _rootDir, FunctionHandler* _funcHandler);
	//ErrorCode
	~ScreenManager();

	bool initialize(std::shared_ptr<ICoreSystem> _iCoreSystem);
	void update(float _dt, std::shared_ptr<IGraphics> _graphics);
	bool onInput(const std::vector<IInput::Event>& _events);
	bool removeScreen(ScreenState* _screen);
	bool openScreen(const std::string& _stateId);

	std::string getText(const std::string& _elemId) const;

private:
	std::vector<ScreenState::ptr> screens;
	std::map<std::string, ScreenState::ptr> registeredStates;

	FunctionHandler* funcHandler;

	bool registerScreenState(const std::string& _stateId, ScreenState::ptr _state);
	bool goBack();

	bool readScreens(const boost::filesystem::path& _screenFile);
	void buttonPressed(const std::string& _func) override;

	std::shared_ptr<ICoreSystem>	iCoreSystem;
};

#endif