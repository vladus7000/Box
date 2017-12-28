#pragma once
#include "StdAfx.hpp"

#include "Gameplay/ActorComponentFactory.hpp"

namespace box
{
	SINGLETON_ACCESSOR(ActorComponentFactory);

	Actor::StrongActorPtr ActorComponentFactory::createActor()
	{
		//TODO: lock
		Actor::StrongActorPtr actor(new Actor(getNextId()));
		if (actor->init())
		{
			auto component = createComponent("TEST");

			if (component)
			{
				component->setOwner(actor);
				actor->addComponent(component);
			}
			else
			{
				actor.reset();
			}
		}
		else
		{
			actor.reset();
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
			component.reset(f());

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