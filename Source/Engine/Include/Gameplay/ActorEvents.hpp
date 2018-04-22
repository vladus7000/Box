#pragma once

#include "System/EventSystem/Event.hpp"
#include "Actor.hpp"

namespace box
{
	class Event_ActorAdded : public EventData
	{
	public:
		static const EventType Type;
	public:
		Event_ActorAdded(Actor::StrongActorPtr actor)
			: m_actor(actor)
		{}

		virtual ~Event_ActorAdded() {};
		virtual const EventType& getType() const override { return Type; }
		virtual const char* getName() { return "Event_ActorAdded"; }

		Actor::StrongActorPtr getActor() { return m_actor; }
	private:
		Actor::StrongActorPtr m_actor;
	};
}
