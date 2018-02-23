#pragma once

#include <memory>
#include "Material.hpp"
#include "Math/Matrix4D.hpp"

#include <d3d11.h>

namespace box
{
	class Mesh
	{
	public:
		using MeshStrongPtr = std::shared_ptr<Mesh>;
		using MeshWeakPtr = std::weak_ptr<Mesh>;

	public:
		Mesh(ID3D11Buffer* vertex, ID3D11Buffer* index, U32 count);
		~Mesh();

		void setMaterial(Material::MaterialStrongPtr material) { m_material = material; } // TODO: check input vertex format
		Material::MaterialWeakPtr getMaterial() const { return m_material; }

		void setWorldMatrix(const Matrix4D& world) { m_worldMatrix = world; }
		const Matrix4D& getWorldMatrix() const { return m_worldMatrix; }

		void render(F32 delta);
		void preRender();
		void postRender();
		void restore();
		void deviceLost();

	private:
		Material::MaterialStrongPtr m_material;
		Matrix4D m_worldMatrix;
		ID3D11Buffer* m_vertexBuffer;
		ID3D11Buffer* m_indexBuffer;
		U32 m_indexCount;
	};
}
