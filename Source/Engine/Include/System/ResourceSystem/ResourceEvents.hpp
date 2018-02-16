#pragma once

#include "System/EventSystem/Event.hpp"

namespace box
{
	class Event_ResourceLoaded : public EventData
	{
	public:
		static const EventType Type;
		virtual ~Event_ResourceLoaded() {};
		virtual const EventType& getType() const override { return Type; }
		virtual const char* getName() { return "Event_ResourceLoaded"; }
	};
}
