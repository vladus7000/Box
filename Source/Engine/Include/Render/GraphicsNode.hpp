#pragma once

#include "Scene\SceneNode.hpp"
#include "Model.hpp"

namespace box
{
	class GraphicsNode : public SceneNode
	{
	public:
		GraphicsNode()
			: SceneNode(SceneNode::Type::Graphics)
		{}
		virtual ~GraphicsNode() {};

		void setModel(std::shared_ptr<Model> model) { m_model = model; }
		std::weak_ptr<Model> getModel(std::shared_ptr<Model> model) const { return m_model; }

	private:
		std::shared_ptr<Model> m_model;
	};
}