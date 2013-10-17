#include "ScreenState.h"

ScreenState::ScreenState(const std::string _screenName)
	: screenName(_screenName), initialized(false), popup(false)
{

}

ScreenState::~ScreenState()
{

}

void ScreenState::onInput(const std::vector<IInput::Event> _events)
{

}

bool ScreenState::initialize(std::shared_ptr<ICoreSystem> _iCoreSystem, Server::ptr _server)
{
	iCoreSystem = _iCoreSystem;
	initialized = true;
	server = _server;
	return true;
}

bool ScreenState::onExit()
{
	active = false;

	return true;
}

bool ScreenState::onEntry()
{
	active = true;

	return true;
}

void ScreenState::update(const float _dt)
{
}

void ScreenState::draw(std::shared_ptr<IGraphics> _graphics)
{
	
}

bool ScreenState::isInitialized() const
{
	return initialized;
}

bool ScreenState::isActive() const
{
	return active;
}

bool ScreenState::isPopup() const
{
	return popup;
}

std::string ScreenState::getBackgroundName() const
{
	return backgroundName;
}

std::string ScreenState::getScreenName() const
{
	return screenName;
}