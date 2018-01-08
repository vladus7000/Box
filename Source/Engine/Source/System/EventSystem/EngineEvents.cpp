#include "StdAfx.hpp"
#include "System\EventSystem\EngineEvents.hpp"

namespace box
{
// MSVS:Tools -> Create GUID
	const EventData::EventType Event_EngineStasted::Type(0x59f88704);
	const EventData::EventType Event_ResourceCacheStarted::Type(0x5b997e39);
	const EventData::EventType Event_ResourceLoaded::Type(0x69394f0c);
}