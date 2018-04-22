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
			, m_transform(nullptr)
		{}
		virtual ~GraphicsNode() {};

		void setModel(std::shared_ptr<Model> model) { m_model = model; }
		std::weak_ptr<Model> getModel() const { return m_model; }

		virtual U32 getMeshesCount() const { return m_model->getMeshesCount(); }
		virtual Mesh::MeshWeakPtr getMeshAt(U32 i) { return m_model->getMeshAt(i); }

		void setTransform(TransformComponent* transform) { m_transform = transform; }
		TransformComponent* getTransform() const { return m_transform; }

		virtual void render(F32 delta) override;
		virtual void gatherCurrentNodeGraphicsObjects(RenderObjects& out) override;

		int getSizeForXML() const override;
		tinyxml2::XMLNode* serializeToXML(tinyxml2::XMLNode* node, tinyxml2::XMLDocument& doc) const override;
		bool loadFromXML(tinyxml2::XMLNode* node) override;

	protected:
		std::shared_ptr<Model> m_model;
		TransformComponent* m_transform;
	};
}