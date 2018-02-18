#pragma once

#include <vector>
#include <memory>

#include "Math/Vector3D.hpp"

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
		inline void setRadius(F32 radius) { m_radius = radius; }

		inline const Vector3D& getPosition() const { return m_position; }
		inline void setPosition(const Vector3D& position) { m_position = position; }

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

	private:
		Type m_type;
		std::vector<SceneNodeStrongPtr> m_children;
		SceneNode* m_parent;
		F32 m_radius;
		Vector3D m_position;
	};
}
