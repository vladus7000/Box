#pragma once

#include <memory>
#include <map>
#include <string>
#include <functional>

#include "System/Patterns/Singleton.hpp"
#include "Component.hpp"
#include "Actor.hpp"

namespace box
{
	class ActorComponentFactory
	{
		SINGLETON(ActorComponentFactory);
	public:
		using ComponentCreatorFunc = std::function<Component*(void)>;

		void registerComponentCreator(const std::string& name, ComponentCreatorFunc& f) { m_componentCreators[name] = f; }

		Actor::StrongActorPtr createActor();
		Component::StrongComponentPtr createComponent(const std::string& name);

	private:
		U64 getNextId();
	private:
		std::map<std::string, ComponentCreatorFunc> m_componentCreators;
	};
}