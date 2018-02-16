#pragma once
#include "System\Patterns\Singleton.hpp"
#include "Scene\Scene.hpp"
#include "Camera.hpp"

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
		Scene::SceneWeakPtr getScene() const { return m_scene; }

		void setCamera(Camera::CameraStrongPtr camera) { m_camera = camera; }
		Camera::CameraWeakPtr getCamera() const { return m_camera; }

		void renderScene(F32 delta);
		void cullObjects();

	private:
		Scene::SceneStrongPtr m_scene;
		Camera::CameraStrongPtr m_camera;
		RenderObjects m_renderList;
	};
}