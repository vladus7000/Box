#include "StdAfx.hpp"
#include "Scene/NodeFactory.hpp"

#include "Render/GraphicsNode.hpp"

namespace box
{
	namespace NodeFactory
	{
		std::shared_ptr<box::SceneNode> CreateNode(const char* name)
		{
			if (strcmp(name, "GraphicsNode") == 0)
			{
				return std::make_shared<box::GraphicsNode>();
			}
			else if (strcmp(name, "SceneNode") == 0)
			{
				return std::make_shared<box::SceneNode>(SceneNode::Type::Invalid);
			}

			return std::shared_ptr<box::SceneNode>();
		}
	}
}