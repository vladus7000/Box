#pragma once

#include "Gameplay/Component.hpp"
#include "Math/Matrix4D.hpp"
#include "Render/GraphicsNode.hpp"

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

		virtual tinyxml2::XMLNode* serializeToXML(tinyxml2::XMLNode* node, tinyxml2::XMLDocument& doc) const
		{
			auto rootElement = doc.NewElement("GraphicsComponent");
				m_graphicsNode->serializeToXML(rootElement, doc);
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