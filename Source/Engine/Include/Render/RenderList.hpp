#pragma once

#include <vector>
#include <memory>

namespace box
{
	class Mesh;
}

namespace box
{
	struct RenderObjects
	{
		std::vector<std::shared_ptr<Mesh>> m_staticObjects;
		std::vector<std::shared_ptr<Mesh>> m_dynamicObjects;
		std::vector<std::shared_ptr<Mesh>> m_terrainObjects;
		std::vector<std::shared_ptr<Mesh>> m_alphaObjects;
		std::vector<std::shared_ptr<Mesh>> m_postObjects;

		void clear()
		{
			m_staticObjects.clear();
			m_dynamicObjects.clear();
			m_terrainObjects.clear();
			m_alphaObjects.clear();
			m_postObjects.clear();
		}
	};
}