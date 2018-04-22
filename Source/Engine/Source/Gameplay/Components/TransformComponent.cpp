#include "StdAfx.hpp"
#include "Gameplay/Components/TransformComponent.hpp"
#include "Gameplay/Components/GraphicsComponent.hpp"
#include "Gameplay/Actor.hpp"

namespace box
{
	void TransformComponent::threadSafeUpdate()
	{
		D3DXMatrixTranslation(&m_transformMatrix, m_position.x, m_position.y, m_position.z);
		if (auto owner = m_owner.lock())
		{
			if (auto graphicsComponent = owner->getComponent<GraphicsComponent>(GraphicsComponent::ComponentID).lock())
			{
				graphicsComponent->m_graphicsNode->setPosition(m_position);
			}
		}
	}
}
