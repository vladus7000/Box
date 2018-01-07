#pragma once

namespace box
{
	class EventData
	{
	public:
		using EventType = U64;
		virtual ~EventData() {};
		virtual const EventType& getType() const = 0;
		virtual const char* getName() = 0;
	};
}