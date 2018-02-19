#pragma once

#include "System/ResourceSystem/ResourceExtraData.hpp"
#include "Mesh.hpp"
#include <string>
#include <vector>

namespace box
{
	class Model : public ResourceExtraData
	{
	public:
		using ModelStrongPtr = std::shared_ptr<Model>;
		using ModelWeakPtr = std::weak_ptr<Model>;

	public:
		Model(const std::string& name = "Model");
		~Model() = default;

		virtual const std::string& getName() const { return m_name; }

		void addMesh(Mesh::MeshStrongPtr mesh) { m_meshes.push_back(mesh); }
		U32 getMeshesCount() const { return static_cast<U32>(m_meshes.size()); }
		Mesh::MeshWeakPtr getMeshAt(U32 i) { return m_meshes[i]; }

	private:
		std::string m_name;
		std::vector<Mesh::MeshStrongPtr> m_meshes;
	};
}
