#include "IInput.h"

std::vector<IInput::Event> IInput::getEvents(bool clearEvents)
{
	if (clearEvents)
	{
		return std::move(events);
	}
	else
	{
		return events;
	}
}