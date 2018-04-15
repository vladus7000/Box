#pragma once

#include "GraphicsNode.hpp"

namespace box
{
	class GridNode final : public GraphicsNode
	{
	public:
		GridNode();
		virtual ~GridNode() {};

		virtual void render(F32 delta) override;
		virtual void gatherCurrentNodeGraphicsObjects(RenderObjects& out) override;

		int getSizeForXML() const override { return 0; };
		tinyxml2::XMLNode* serializeToXML(tinyxml2::XMLNode* node, tinyxml2::XMLDocument& doc) const override { return node; };
		bool loadFromXML(tinyxml2::XMLNode* node) override { return true; };
	private:
		Mesh::MeshStrongPtr m_gridMesh;
	};
}