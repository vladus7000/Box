#pragma once

#include "Event.hpp"

namespace box
{
	class Event_EngineStasted : public EventData
	{
	public:
		static const EventType Type;
		virtual ~Event_EngineStasted() {};
		virtual const EventType& getType() const override { return Type; }
		virtual const char* getName() { return "Event_EngineStasted"; }
	};

	class Event_ResourceCacheStarted : public EventData
	{
	public:
		static const EventType Type;
		virtual ~Event_ResourceCacheStarted() {};
		virtual const EventType& getType() const override { return Type; }
		virtual const char* getName() { return "Event_ResourceCacheStarted"; }
	};
}