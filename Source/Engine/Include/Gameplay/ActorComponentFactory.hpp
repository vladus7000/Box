#pragma once

#include <memory>
#include <map>
#include <string>
#include <functional>

#include "System/Patterns/Singleton.hpp"

namespace box
{
	class Actor;
	class Component;

	class ActorComponentFactory
	{
		SINGLETON(ActorComponentFactory);
	public:

		void init();

		using ComponentCreatorFunc = std::function<std::shared_ptr<Component>(U64)>;
		using ActorCreatorFunc = std::function<std::shared_ptr<Actor>(U64)>;

		void registerComponentCreator(const std::string& name, const ComponentCreatorFunc& f) { m_componentCreators[name] = f; }
		void registerActorCreator(const std::string& name, const ActorCreatorFunc& f) { m_actorCreators[name] = f; }

		std::shared_ptr<Actor> createActor(const std::string& name);
		std::shared_ptr<Component> createComponent(const std::string& name);

	private:
		U64 getNextId();
	private:
		std::map<std::string, ComponentCreatorFunc> m_componentCreators;
		std::map<std::string, ActorCreatorFunc> m_actorCreators;
	};
}