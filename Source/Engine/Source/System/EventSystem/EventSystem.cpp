#include "StdAfx.hpp"
#include "System\EventSystem\EventSystem.hpp"

#include <map>
#include <list>

namespace box
{
	struct EventSystem::impl
	{
		using Delegates = std::list<DelegateType>;
		using EventsMap = std::map<EventData::EventType, Delegates>;
		using EventQueue = std::list<StrongEventDataPtr>;

		EventsMap eventsMap;
		EventQueue incomingEvents;

		void add(const DelegateType& delegate, const EventData::EventType& eventType)
		{
			auto& delegates = eventsMap[eventType];
			for (auto it : delegates)
			{
				if (it == delegate)
				{
					return;
				}
			}
			delegates.push_back(delegate);
		}

		void remove(const DelegateType& delegate, const EventData::EventType& eventType)
		{
			auto foundIt = eventsMap.find(eventType);
			if (foundIt != eventsMap.end())
			{
				for (auto it = foundIt->second.begin(); it != foundIt->second.end(); ++it)
				{
					if ((*it) == delegate)
					{
						foundIt->second.erase(it);
						return;
					}
				}
			}
		}

		void raiseEvent(StrongEventDataPtr event)
		{
			if (event)
			{
				auto& type = event->getType();
				auto foundIt = eventsMap.find(type);
				{
					if (foundIt != eventsMap.end())
					{
						auto& delegates = foundIt->second;
						for (auto delegate : delegates)
						{
							delegate(event);
						}
					}
				}
			}
		}

		void queueEvent(StrongEventDataPtr event)
		{
			incomingEvents.push_back(event);
		}

		void update(F32 delta)
		{
			for (auto ev : incomingEvents)
			{
				raiseEvent(ev);
			}
			incomingEvents.clear();
		}
	};

	SINGLETON_ACCESSOR(EventSystem);
	EventSystem::EventSystem()
	{
		m_impl.reset(new impl);
	}

	EventSystem::~EventSystem()
	{
	}

	void EventSystem::add(const DelegateType& delegate, const EventData::EventType& eventType)
	{
		m_impl->add(delegate, eventType);
	}

	void EventSystem::remove(const DelegateType& delegate, const EventData::EventType& eventType)
	{
		m_impl->remove(delegate, eventType);
	}

	void EventSystem::raiseEvent(StrongEventDataPtr event)
	{
		m_impl->raiseEvent(event);
	}

	void EventSystem::queueEvent(StrongEventDataPtr event)
	{
		m_impl->queueEvent(event);
	}

	void EventSystem::update(F32 delta)
	{
		m_impl->update(delta);
	}

}
