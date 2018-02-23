#include "StdAfx.hpp"

#include "Render/GraphicsNode.hpp"

namespace box
{
	void GraphicsNode::render(F32 delta)
	{
		SceneNode::render(delta);

		auto count = m_model->getMeshesCount();
		for (U32 i = 0; i < count; i++)
		{
			auto meshWeak = m_model->getMeshAt(i);
			if (auto mesh = meshWeak.lock())
			{
				mesh->render(delta);
			}
		}
	}
}
