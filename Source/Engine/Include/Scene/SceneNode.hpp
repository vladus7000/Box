#pragma once

#include <vector>
#include <memory>

#include "Math/Vector3D.hpp"
#include "Math/Frustum.hpp"
#include "Render/RenderList.hpp"
#include <tinyxml2/tinyxml2.h>

#include "NodeFactory.hpp"

namespace box
{
	class SceneNode
	{
	public:
		using SceneNodeStrongPtr = std::shared_ptr<SceneNode>;
		using SceneNodeWeakPtr = std::weak_ptr<SceneNode>;
	public:

		enum class Type
		{
			Graphics,
			Invalid
		};

		SceneNode(Type type)
			: m_radius(1.0f)
			, m_type(type)
		{}
		virtual ~SceneNode() = default;

		Type getType() const { return m_type; }

		inline virtual void render(F32 delta)
		{
			for (auto& it : m_children)
			{
				it->render(delta);
			}
		}
		inline virtual void preRender()
		{
			for (auto& it : m_children)
			{
				it->preRender();
			}
		}
		inline virtual void postRender()
		{
			for (auto& it : m_children)
			{
				it->postRender();
			}
		}
		inline virtual void update(F32 delta)
		{
			for (auto& it : m_children)
			{
				it->update(delta);
			}
		}
		inline void restore()
		{
			for (auto& it : m_children)
			{
				it->restore();
			}
		}
		inline void deviceLost()
		{
			for (auto& it : m_children)
			{
				it->deviceLost();
			}
		}

		inline F32 getRadius() const { return m_radius; }
		inline void setRadius(F32 radius) { m_radius = radius; } //Update radius ?

		inline const Vector3D& getPosition() const { return m_position; }
		inline void setPosition(const Vector3D& position) { m_position = position; } //Update radius ?

		inline void updateRadius(const SceneNode& child)
		{
			Vector3D dir = child.getPosition() - m_position;
			F32 newRadius = dir.getLenght() + child.getRadius();
			if (newRadius > m_radius)
			{
				m_radius = newRadius;
				if (m_parent)
				{
					m_parent->updateRadius(*this);
				}
			}
		}

		inline void setParent(SceneNode* parent) { m_parent = parent; }
		inline SceneNode* getParent(SceneNodeStrongPtr parent) const { return m_parent; }

		inline void addChild(SceneNodeStrongPtr child)
		{
			if (child)
			{
				child->setParent(this);
				m_children.push_back(child);
				updateRadius(*child);
			}
		}

		inline void removeChild(SceneNodeStrongPtr child)
		{
			auto foundIt = std::find(m_children.begin(), m_children.end(), child);
			if (foundIt != m_children.end())
			{
				m_children.erase(foundIt);
			}
		}

		inline bool isHasChild(SceneNodeStrongPtr child)
		{
			auto foundIt = std::find(m_children.begin(), m_children.end(), child);
			return (foundIt != m_children.end());
		}

		Frustum::TestResult cullingTest(const Frustum& frustum)
		{
			U32 mask = 0;
			Frustum::TestResult testResult = frustum.testSphere(m_position, m_radius, mask);
			return testResult;
		}

		Frustum::TestResult gatherObjects(const Frustum& frustum, RenderObjects& out)
		{
			Frustum::TestResult testResult = cullingTest(frustum);

			if (testResult == Frustum::TestResult::inside)
			{
				gatherAllGraphicsObjects(out);
			}
			else if (testResult == Frustum::TestResult::intersect)
			{
				gatherCurrentNodeGraphicsObjects(out);
				for (auto& it : m_children)
				{
					Frustum::TestResult testResult = it->cullingTest(frustum);
					if (testResult == Frustum::TestResult::inside || testResult == Frustum::TestResult::intersect)
					{
						it->gatherObjects(frustum, out);
					}
				}
			}
			return testResult;
		}

		virtual void gatherAllGraphicsObjects(RenderObjects& out)
		{
			gatherCurrentNodeGraphicsObjects(out);
			for (auto& it : m_children)
			{
				if (it->getType() == Type::Graphics)
				{
					it->gatherAllGraphicsObjects(out);
				}
			}
		}

		virtual void gatherCurrentNodeGraphicsObjects(RenderObjects& out) {}

		virtual int getSizeForXML() const
		{
			int ret = 0;
			for (auto& it : m_children)
			{
				ret += it->getSizeForXML();
			}
			return ret;
		}
		virtual tinyxml2::XMLNode* serializeToXML(tinyxml2::XMLNode* node, tinyxml2::XMLDocument& doc) const
		{
			tinyxml2::XMLElement* sceneNodeElement = doc.NewElement("SceneNode");
			sceneNodeElement->SetAttribute("radius", m_radius);
			m_position.serializeToXMLElement(sceneNodeElement);
			for (const auto& it : m_children)
			{
				it->serializeToXML(sceneNodeElement, doc);
			}

			if (node)
			{
				node->InsertEndChild(sceneNodeElement);
			}

			return sceneNodeElement;
		}

		virtual bool loadFromXML(tinyxml2::XMLNode* node)
		{
			if (auto element = node->ToElement())
			{
				if (strcmp(element->Name(), "SceneNode") == 0)
				{
					element->QueryFloatAttribute("radius", &m_radius);
					m_position.loadFromXMLElement(element);
				}

				bool ok = true;

				for (tinyxml2::XMLNode* child = element->FirstChild(); child; child = child->NextSibling())
				{
					if (auto element = child->ToElement())
					{
						auto node = NodeFactory::CreateNode(element->Name());
						ok &= node->loadFromXML(child);

						m_children.push_back(node);
					}
				}

				return ok;
			}
			return false;
		}

	private:
		Type m_type;
		std::vector<SceneNodeStrongPtr> m_children;
		SceneNode* m_parent;
		F32 m_radius;
		Vector3D m_position;
	};
}
