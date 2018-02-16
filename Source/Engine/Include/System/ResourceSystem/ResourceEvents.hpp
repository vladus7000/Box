#pragma once

#include "System/EventSystem/Event.hpp"
#include "System/ResourceSystem/ResourceHandle.hpp"

namespace box
{
	class Event_ResourceLoaded : public EventData
	{
	public:
		static const EventType Type;
		enum class ResourceType
		{
			Model,
			Invalid
		};
	public:
		Event_ResourceLoaded()
			: m_type(ResourceType::Invalid)
		{}
		Event_ResourceLoaded(ResourceType type, ResourceHandle::StrongResourceHandlePtr handle)
			: m_type(type)
			, m_handle(handle)
		{}

		virtual ~Event_ResourceLoaded() {};
		virtual const EventType& getType() const override { return Type; }
		virtual const char* getName() { return "Event_ResourceLoaded"; }

		ResourceType getType() { return m_type; }
		ResourceHandle::StrongResourceHandlePtr getHandle() { return m_handle; }
	private:
		ResourceType m_type;
		ResourceHandle::StrongResourceHandlePtr m_handle;
	};
}
