#pragma once

#include <memory>
#include "Material.hpp"

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

		inline void render(F32 delta);
		inline void preRender();
		inline void postRender();
		inline void restore();
		inline void deviceLost();

	private:
		Material::MaterialStrongPtr m_material;
		ID3D11Buffer* m_vertexBuffer;
		ID3D11Buffer* m_indexBuffer;
		U32 m_indexCount;
	};
}
