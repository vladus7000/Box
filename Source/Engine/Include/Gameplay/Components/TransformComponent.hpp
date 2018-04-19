#pragma once

#include "Gameplay/Component.hpp"
#include "Math/Matrix4D.hpp"

namespace box
{
	struct TransformComponent : public Component
	{
		static const U64 ComponentID = 0x30bc59df;
		TransformComponent()
		{
			m_componentId = ComponentID;
			m_transformMatrix.setIdentity();
		}

		virtual tinyxml2::XMLNode* serializeToXML(tinyxml2::XMLNode* node, tinyxml2::XMLDocument& doc) const
		{
			auto rootElement = doc.NewElement("Transform");

			m_transformMatrix.serializeToXMLElement(rootElement);

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
				if (strcmp(rootElement->Name(), "Transform") == 0)
				{
					m_transformMatrix.loadFromXMLElement(rootElement);
					return true;
				}
			}
			return false;
		}

		Matrix4D m_transformMatrix;
	};
}