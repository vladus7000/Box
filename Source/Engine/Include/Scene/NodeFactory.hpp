#pragma once

#include <memory>

namespace box
{
	class SceneNode;

	namespace NodeFactory
	{
		std::shared_ptr<SceneNode> CreateNode(const char* name);
	}
}