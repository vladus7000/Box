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

	void GraphicsNode::gatherCurrentNodeGraphicsObjects(RenderObjects& out)
	{
		auto count = m_model->getMeshesCount();
		for (U32 i = 0; i < count; i++)
		{
			auto meshWeak = m_model->getMeshAt(i);
			if (auto mesh = meshWeak.lock())
			{
				out.m_dynamicObjects.push_back(mesh);
			}
		}
	}

	int GraphicsNode::getSizeForXML() const
	{
		int ret = SceneNode::getSizeForXML();
		return ret + m_model->getSizeForXML();
	}

	tinyxml2::XMLNode* GraphicsNode::serializeToXML(tinyxml2::XMLNode* node, tinyxml2::XMLDocument& doc) const
	{
		auto newNode = SceneNode::serializeToXML(nullptr, doc);

		tinyxml2::XMLElement* graphicsNode = doc.NewElement("GraphicsNode");
		m_model->serializeToXML(graphicsNode, doc);
		graphicsNode->InsertEndChild(newNode);
		if (node)
		{
			node->InsertEndChild(graphicsNode);
		}

		return graphicsNode;
	}

	bool GraphicsNode::loadFromXML(tinyxml2::XMLNode* node)
	{
		if (auto element = node->ToElement())
		{
			if (strcmp(element->Name(), "GraphicsNode") == 0)
			{
				if (auto modelElement = element->FirstChildElement("Model"))
				{
					m_model = std::make_shared<Model>();
					m_model->loadFromXML(modelElement);
				}
				if (auto sceneNodeElement = element->FirstChildElement("SceneNode"))
				{
					SceneNode::loadFromXML(sceneNodeElement);
				}
				return true;
			}
		}
		return false;
	}

}
