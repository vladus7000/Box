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
		SceneNode::serializeToXML(node, doc);
		tinyxml2::XMLElement* graphicsNode = doc.NewElement("GraphicsNode");
		m_model->serializeToXML(graphicsNode, doc);
		if (node)
		{
			node->InsertEndChild(graphicsNode);
		}
		return graphicsNode;
	}

	bool GraphicsNode::loadFromXML(tinyxml2::XMLNode* node, tinyxml2::XMLDocument& doc)
	{
		SceneNode::loadFromXML(node, doc);
		// Todo read xml and make m_model!!!
		m_model->loadFromXML(node, doc);
		return false;
	}

}
