#include "InputState.h"

void InputState::setFrameEvents(std::vector<IInput::Event> _events)
{
	if(events.size() > 0)
		events.clear();

	events = _events;
}

std::vector<IInput::Event>& InputState::getEvents()
{
	return events;
}