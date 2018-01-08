#pragma once

#include "System\Patterns\Singleton.hpp"
#include "System\EventSystem\Event.hpp"
#include "FastDelegate\FastDelegate.h"
#include <memory>

namespace box
{
	class EventSystem
	{
	public:
		using StrongEventDataPtr = std::shared_ptr<EventData>;
		using DelegateType = fastdelegate::FastDelegate1<StrongEventDataPtr>;
	public:
		static EventSystem& Instance();
		
		bool init();
		void deinit();

		void add(const DelegateType& delegate, const EventData::EventType& eventType);
		void remove(const DelegateType& delegate, const EventData::EventType& eventType);
		void raiseEvent(StrongEventDataPtr event);
		void queueEvent(StrongEventDataPtr event);
		void update(F32 delta);

	private:
		EventSystem();
		~EventSystem();
		EventSystem(const EventSystem& rhs) = delete;
		EventSystem(EventSystem&& rhs) = delete;
		EventSystem& operator=(EventSystem&& rhs) = delete;
		EventSystem& operator=(EventSystem& rhs) = delete;

	private:
		struct impl;
		std::unique_ptr<impl> m_impl;
	};
}