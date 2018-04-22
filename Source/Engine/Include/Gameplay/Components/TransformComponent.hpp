#pragma once

#include "Gameplay/Component.hpp"
#include "Math/Matrix4D.hpp"
#include "Math/Vector3D.hpp"

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

		void setPosition(const Vector3D& position) { m_position = position; }
		const Vector3D& getPosition() const { return m_position; }

		virtual void threadSafeUpdate() override;

		virtual tinyxml2::XMLNode* serializeToXML(tinyxml2::XMLNode* node, tinyxml2::XMLDocument& doc, bool runTimeInfo) const override
		{
			auto rootElement = doc.NewElement("Transform");

			rootElement->SetAttribute("ID", (int64_t)m_componentId);

			m_position.serializeToXMLElement(rootElement);

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
					m_position.loadFromXMLElement(rootElement);
					return true;
				}
			}
			return false;
		}
		virtual bool updateFromXml(tinyxml2::XMLNode* node)
		{
			return loadFromXML(node);
		}

		Matrix4D m_transformMatrix;
		Vector3D m_position;
	};
}