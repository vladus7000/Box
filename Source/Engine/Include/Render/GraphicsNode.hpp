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

		virtual void render(F32 delta) override;
		virtual void gatherCurrentNodeGraphicsObjects(RenderObjects& out) override;

		int getSizeForXML() const override;
		tinyxml2::XMLNode* serializeToXML(tinyxml2::XMLNode* node, tinyxml2::XMLDocument& doc) const override;
		bool loadFromXML(tinyxml2::XMLNode* node, tinyxml2::XMLDocument& doc) override;

	private:
		std::shared_ptr<Model> m_model;
	};
}