#pragma once
#include "StdAfx.hpp"

#include "Gameplay/ActorComponentFactory.hpp"
#include "Gameplay/Component.hpp"
#include "Gameplay/Actor.hpp"
#include "Gameplay/Components/GraphicsComponent.hpp"
#include "Gameplay/Components/TransformComponent.hpp"
#include "System/EventSystem/EventSystem.hpp"
#include "Gameplay/ActorEvents.hpp"

namespace box
{
	SINGLETON_ACCESSOR(ActorComponentFactory);

	std::shared_ptr<Component> makeTransform(U64 id)
	{
		return std::make_shared<TransformComponent>();
	}

	std::shared_ptr<Component> makeGraphics(U64 id)
	{
		return std::make_shared<GraphicsComponent>();
	}

	std::shared_ptr<Actor> makeBaseActor(U64 id)
	{
		auto actor = std::make_shared<Actor>(id);

		std::shared_ptr<Event_ActorAdded> event = std::make_shared<Event_ActorAdded>(actor);
		EventSystem::Instance().raiseEvent(event);

		return actor;
	}

	void ActorComponentFactory::init()
	{
		registerComponentCreator("GraphicsComponent", makeGraphics);
		registerComponentCreator("Transform", makeTransform);

		registerActorCreator("ActorBase", makeBaseActor);
	}

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
		static U64 id = 1;
		return id++;
	}
}