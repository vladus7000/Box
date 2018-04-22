#pragma once

#include "Gameplay/Component.hpp"
#include "Gameplay/Components/TransformComponent.hpp"
#include "Math/Matrix4D.hpp"
#include "Render/GraphicsNode.hpp"
#include "Scene/Scene.hpp"
#include "Gameplay/Actor.hpp"

namespace box
{
	struct GraphicsComponent : public Component
	{
		static const U64 ComponentID = 0xaf249c44;
		GraphicsComponent()
		{
			m_componentId = ComponentID;
			m_graphicsNode = std::make_shared<GraphicsNode>();
		}

		virtual tinyxml2::XMLNode* serializeToXML(tinyxml2::XMLNode* node, tinyxml2::XMLDocument& doc, bool runTimeInfo) const override
		{
			auto rootElement = doc.NewElement("GraphicsComponent");
			m_graphicsNode->serializeToXML(rootElement, doc);

			rootElement->SetAttribute("ID", (long long int)m_componentId);

			if (node)
			{
				node->InsertEndChild(rootElement);
			}
			return rootElement;
		}

		virtual bool loadFromXML(tinyxml2::XMLNode* node)
		{
			if (auto rootElement = node->ToElement())
			{
				if (strcmp(rootElement->Name(), "GraphicsComponent") == 0)
				{
					if (auto element = rootElement->FirstChildElement("GraphicsNode"))
					{
						auto scene = m_owner.lock()->getScene().lock()->getRoot().lock();
						if (auto component = m_owner.lock()->getComponent<TransformComponent>(TransformComponent::ComponentID).lock())
						{
							m_graphicsNode->setTransform(component.get());
						}
						scene->addChild(m_graphicsNode);
						m_graphicsNode->loadFromXML(element);
						return true;
					}
				}
			}
			return false;
		}

		std::shared_ptr<GraphicsNode> m_graphicsNode;
	};
}