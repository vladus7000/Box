#pragma once

#include "SceneNode.hpp"
#include "Render/RenderList.hpp"
#include "Math/Frustum.hpp"
#include "EnvironmentSettings.hpp"
#include "Render/Camera.hpp"

namespace box
{
	class Scene
	{
	public:
		using SceneStrongPtr = std::shared_ptr<Scene>;
		using SceneWeakPtr = std::weak_ptr<Scene>;

	public:
		Scene()
		{
			m_root = std::make_shared<SceneNode>(SceneNode::Type::Invalid);
		}
		~Scene() = default;

		inline virtual void render(F32 delta)
		{
			m_root->preRender();
			m_root->render(delta);
			m_root->postRender();
		}
		inline void update(F32 delta)
		{
			m_root->update(delta);
		}
		inline void restore()
		{
			m_root->restore();
		}
		inline void deviceLost()
		{
			m_root->deviceLost();
		}
		inline SceneNode::SceneNodeWeakPtr getRoot()
		{
			return m_root;
		}

		void cullGraphicsObjects(const Frustum& frustum, RenderObjects& out)
		{
			m_root->gatherObjects(frustum, out);
		}

		void updateEnvironmentSettings(tinyxml2::XMLNode* node)
		{
			m_environment.updateEnvironmentSettings(node);
		}

		void setCamera(Camera::CameraStrongPtr camera) { m_camera = camera; }
		Camera::CameraStrongPtr getCamara() const { return m_camera; }

		const EnvironmentSettings& getEnvironmentSettings() const { return m_environment; }

		int getSizeForXML() const
		{
			return m_root->getSizeForXML();
		}

		tinyxml2::XMLNode* serializeToXML(tinyxml2::XMLNode* node, tinyxml2::XMLDocument& doc) const
		{
			tinyxml2::XMLNode* scene = doc.NewElement("Scene");

			m_environment.serializeToXML(scene, doc);
			if (m_camera)
			{
				m_camera->serializeToXML(scene, doc);
			}

			//tinyxml2::XMLNode* newNode = m_root->serializeToXML(scene, doc);

			if (node)
			{
				node->InsertEndChild(scene);
			}

			return scene;
		}

		bool loadFromXML(tinyxml2::XMLNode* node, tinyxml2::XMLDocument& doc)
		{
			bool ok = false;
			if (auto root = node->ToElement())
			{
				if (strcmp(root->Name(), "Scene") == 0)
				{
					ok = true;
					if (auto envSettings = root->FirstChildElement("EnvironmentSettings"))
					{
						ok &= m_environment.loadFromXML(envSettings);
					}
					if (auto element = root->FirstChildElement("Camera"))
					{
						if (!m_camera)
						{
							m_camera = std::make_shared<Camera>();
						}
						ok &= m_camera->loadFromXML(element);
					}
					//if (auto nodeElement = root->FirstChildElement("SceneNode"))
					//{
					//	ok &= m_root->loadFromXML(nodeElement);
					//}
				}
			}

			return ok;
		}

	private:
		inline virtual void preRender()
		{
			m_root->preRender();
		}
		inline virtual void postRender()
		{
			m_root->postRender();
		}
	private:
		
		SceneNode::SceneNodeStrongPtr m_root;
		EnvironmentSettings m_environment;
		Camera::CameraStrongPtr m_camera;
	};
}
