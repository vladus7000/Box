#pragma once

#include <map>
#include <memory>
#include <string>

#include "Component.hpp"

namespace box
{
	class Actor
	{
		friend class ActorComponentFactory;
	public:
		using StrongActorPtr = std::shared_ptr<Actor>;
		using WeakActorPtr = std::weak_ptr<Actor>;
		using Components = std::map<Component::ComponentId, Component::StrongComponentPtr>;
		using ActorId = U64;

	public:
		explicit Actor(ActorId id) : m_id(id) {}
		virtual ~Actor() {}

		virtual bool init() { return false; }
		virtual void postInit() {}
		virtual void deinit() {}

		virtual void update(F32 delta) { for (auto& it : m_components) it.second->update(delta); }

		template <class ComponetClass>
		std::weak_ptr<ComponetClass> getComponent(Component::ComponentId id)
		{
			//TODO: lock
			const auto foundIt = m_components.find(id);
			if (foundIt != m_components.end())
			{
				Component::StrongComponentPtr base(foundIt->second);
				std::shared_ptr<ComponetClass> component(std::static_pointer_cast<ComponetClass>(base));
				std::weak_ptr<ComponetClass> ret(component);
				return ret;
			}
			else
			{
				return std::weak_ptr<ComponetClass>();
			}
		}

		ActorId getId() const { return m_id; }

		void setName(const std::string& name) { m_name = name; }
		const std::string& getName() const { return m_name; }

	private:
		void addComponent(Component::StrongComponentPtr component) { m_components[component->getId()] = component; }

	private:
		Components m_components;
		ActorId m_id;
		std::string m_name;
	};
}
