#include "ScreenState.h"

ScreenState::ScreenState(const std::string _screenName)
	: screenName(_screenName)
{

}

ScreenState::~ScreenState()
{

}

void ScreenState::onInput()
{

}

bool ScreenState::initialize()
{
	return true;
}

void ScreenState::update(const float _dt)
{
}

void ScreenState::draw(std::shared_ptr<IGraphics> _graphics)
{
	
}

const bool ScreenState::isActive()
{
	return active;
}

const bool ScreenState::isPopup()
{
	return popup;
}

const std::string ScreenState::getBackgroundName()
{
	return backgroundName;
}

const std::string ScreenState::getScreenName()
{
	return screenName;
}