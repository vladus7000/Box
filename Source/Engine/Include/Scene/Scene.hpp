#pragma once

#include "SceneNode.hpp"
#include "Render/RenderList.hpp"
#include "Math/Frustum.hpp"
#include "EnvironmentSettings.hpp"

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

		const EnvironmentSettings& getEnvironmentSettings() const { return m_environment; }

		int getSizeForXML() const
		{
			return m_root->getSizeForXML();
		}

		tinyxml2::XMLNode* serializeToXML(tinyxml2::XMLNode* node, tinyxml2::XMLDocument& doc) const
		{
			tinyxml2::XMLNode* scene = doc.NewElement("Scene");
			tinyxml2::XMLNode* newNode = m_root->serializeToXML(scene, doc);

			if (node)
			{
				node->InsertEndChild(scene);
			}

			return scene;
		}

		bool loadFromXML(tinyxml2::XMLNode* node, tinyxml2::XMLDocument& doc)
		{
			if (auto element = node->ToElement())
			{
				if (strcmp(element->Name(), "Scene") == 0)
				{
					m_root->loadFromXML(element->FirstChild(), doc);
				}
			}

			return false;
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
	};
}
