#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>
#include <tinyxml2/tinyxml2.h>

#include "ActorComponentFactory.hpp"
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

		virtual void update(F32 delta)
		{
			for (auto& it : m_components)
			{
				it.second->update(delta);
			}

			for (auto& actor : m_children)
			{
				actor->update(delta);
			}
		}

		void addChild(StrongActorPtr actor)
		{
			auto& foundIt = std::find(m_children.begin(), m_children.end(), actor);

			if (foundIt == m_children.end())
			{
				m_children.push_back(actor);
			}
		}

		void removeChild(StrongActorPtr actor)
		{
			auto& foundIt = std::find(m_children.begin(), m_children.end(), actor);

			if (foundIt != m_children.end())
			{
				m_children.erase(foundIt);
			}
		}

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

		virtual tinyxml2::XMLNode* serializeToXML(tinyxml2::XMLNode* node, tinyxml2::XMLDocument& doc) const
		{
			tinyxml2::XMLElement* rootElement = doc.NewElement("ActorBase");
			
			tinyxml2::XMLElement* components = doc.NewElement("Components");

			for (auto& it : m_components)
			{
				it.second->serializeToXML(components, doc);
			}

			rootElement->InsertEndChild(components);

			tinyxml2::XMLElement* childs = doc.NewElement("Children");
			for (const auto& it : m_children)
			{
				it->serializeToXML(childs, doc);
			}
			rootElement->InsertEndChild(childs);

			if (node)
			{
				node->InsertEndChild(rootElement);
			}

			return rootElement;
		}

		virtual bool loadFromXML(tinyxml2::XMLNode* node)
		{
			bool ok = true;
			if (auto rootElement = node->ToElement())
			{
				if (strcmp(rootElement->Name(), "ActorBase") == 0)
				{
					if (auto componentsElement = rootElement->FirstChildElement("Components"))
					{
						for (tinyxml2::XMLNode* child = componentsElement->FirstChild(); child; child = child->NextSibling())
						{
							auto component = ActorComponentFactory::Instance().createComponent(child->ToElement()->Name());
							ok &= component->loadFromXML(child);

							m_components[component->getId()] = component;
						}
					}

					if (auto childsElement = rootElement->FirstChildElement("Children"))
					{
						for (tinyxml2::XMLNode* child = childsElement->FirstChild(); child; child = child->NextSibling())
						{
							if (auto element = child->ToElement())
							{
								auto node = ActorComponentFactory::Instance().createActor(element->Name());
								ok &= node->loadFromXML(child);

								m_children.push_back(node);
							}
						}
					}
				}
				return ok;
			}
			return false;
		}

		void addComponent(Component::StrongComponentPtr component) { m_components[component->getId()] = component; }

	private:
		Components m_components;
		ActorId m_id;
		std::vector<Actor::StrongActorPtr> m_children;
		std::string m_name;
	};
}
