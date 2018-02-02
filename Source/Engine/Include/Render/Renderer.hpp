#pragma once
#include "System\Patterns\Singleton.hpp"
#include "Scene\Scene.hpp"

namespace box
{
	struct RenderObjects
	{
		std::vector<SceneNode> m_staticObjects;
		std::vector<SceneNode> m_dynamicObjects;
		std::vector<SceneNode> m_terrainObjects;
		std::vector<SceneNode> m_alphaObjects;
		std::vector<SceneNode> m_postObjects;
	};

	class Renderer
	{
		SINGLETON(Renderer);
	public:
		bool init();
		void deinit();

		void setScene(Scene::SceneStrongPtr scene) { m_scene = scene; };

		void renderScene(F32 delta);
		void cullObjects();

	private:
		Scene::SceneStrongPtr m_scene;
		RenderObjects m_renderList;
	};
}