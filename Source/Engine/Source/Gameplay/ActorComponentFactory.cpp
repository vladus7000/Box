#pragma once
#include "StdAfx.hpp"

#include "Gameplay/ActorComponentFactory.hpp"
#include "Gameplay/Component.hpp"
#include "Gameplay/Actor.hpp"


namespace box
{
	SINGLETON_ACCESSOR(ActorComponentFactory);

	Actor::StrongActorPtr ActorComponentFactory::createActor(const std::string& name)
	{
		//TODO: lock
		Actor::StrongActorPtr actor;
		auto foundIt = m_actorCreators.find(name);
		if (foundIt != m_actorCreators.end())
		{
			ActorCreatorFunc f = foundIt->second;
			actor = f(getNextId());

			if (actor && !actor->init())
			{
				actor.reset();
			}

			if (actor)
			{
				actor->postInit();
			}
		}
		return actor;
	}

	Component::StrongComponentPtr ActorComponentFactory::createComponent(const std::string& name)
	{
		//TODO: lock
		Component::StrongComponentPtr component;
		auto foundIt = m_componentCreators.find(name);
		if (foundIt != m_componentCreators.end())
		{
			ComponentCreatorFunc f = foundIt->second;
			component = f(getNextId());

			if (component && !component->init())
			{
				component.reset();
			}
		}
		return component;
	}

	U64 ActorComponentFactory::getNextId()
	{
		static U64 id = 0;
		return id++;
	}
}