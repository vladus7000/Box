#pragma once

#include <vector>
#include <memory>

namespace box
{
	class GraphicsNode;
}

namespace box
{
	struct RenderObjects
	{
		std::vector<GraphicsNode*> m_staticObjects;
		std::vector<GraphicsNode*> m_dynamicObjects;
		std::vector<GraphicsNode*> m_terrainObjects;
		std::vector<GraphicsNode*> m_alphaObjects;
		std::vector<GraphicsNode*> m_postObjects;
		std::vector<GraphicsNode*> m_editorObjects;

		void clear()
		{
			m_staticObjects.clear();
			m_dynamicObjects.clear();
			m_terrainObjects.clear();
			m_alphaObjects.clear();
			m_postObjects.clear();
			m_editorObjects.clear();
		}
	};
}