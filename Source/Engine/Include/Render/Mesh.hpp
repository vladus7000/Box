#pragma once

#include <memory>
#include "Material.hpp"
#include "Math/Matrix4D.hpp"
#include <tinyxml2/tinyxml2.h>

#include <d3d11.h>

namespace box
{
	class Mesh
	{
	public:
		using MeshStrongPtr = std::shared_ptr<Mesh>;
		using MeshWeakPtr = std::weak_ptr<Mesh>;

		struct SimpleVertexFormat
		{
			float pos[3];
			float tcoord[2];
			float norm[3];
		};

	public:
		Mesh(ID3D11Buffer* vertex, ID3D11Buffer* index, U32 count);
		~Mesh();

		void setMaterial(Material::MaterialStrongPtr material) { m_material = material; } // TODO: check input vertex format
		Material::MaterialWeakPtr getMaterial() const { return m_material; }

		void setName(const std::string& name) { m_name = name; }
		const std::string& getName() { return m_name; }

		void setWorldMatrix(const Matrix4D& world) { m_worldMatrix = world; }
		const Matrix4D& getWorldMatrix() const { return m_worldMatrix; }

		void render(F32 delta);
		void preRender();
		void postRender();
		void restore();
		void deviceLost();

		void setRawVertexBuffer(std::vector<Mesh::SimpleVertexFormat>&& buf);
		void setRawIndexBuffer(std::vector<U16>&& buf);

		int getSizeForXML() const;
		tinyxml2::XMLNode* serializeToXML(tinyxml2::XMLNode* node, tinyxml2::XMLDocument& doc) const;
		bool loadFromXML(tinyxml2::XMLNode* node, tinyxml2::XMLDocument& doc);

	private:
		Material::MaterialStrongPtr m_material;
		Matrix4D m_worldMatrix;
		ID3D11Buffer* m_vertexBuffer;
		ID3D11Buffer* m_indexBuffer;
		U32 m_indexCount;

		std::vector<Mesh::SimpleVertexFormat> m_rawVertexBuffer;
		std::vector<U16> m_rawIndexBuffer;
		std::string m_name;
	};
}
