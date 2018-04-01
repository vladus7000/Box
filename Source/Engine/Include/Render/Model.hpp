#pragma once

#include "System/ResourceSystem/ResourceExtraData.hpp"
#include "Mesh.hpp"
#include <string>
#include <vector>
#include <tinyxml2/tinyxml2.h>

namespace box
{
	class Model : public ResourceExtraData
	{
	public:
		using ModelStrongPtr = std::shared_ptr<Model>;
		using ModelWeakPtr = std::weak_ptr<Model>;

	public:
		Model(const std::string& name, const std::string& sourceFile);
		~Model() = default;

		void setName(const std::string& name) { m_name = name; }
		virtual const std::string& getName() const { return m_name; }

		void setSourceFile(const std::string& name) { m_sourceFile = name; }
		const std::string& getSourceFile() const { return m_sourceFile; }

		void addMesh(Mesh::MeshStrongPtr mesh) { m_meshes.push_back(mesh); }
		U32 getMeshesCount() const { return static_cast<U32>(m_meshes.size()); }
		Mesh::MeshWeakPtr getMeshAt(U32 i) { return m_meshes[i]; }
		
		int getSizeForXML() const;
		tinyxml2::XMLNode* serializeToXML(tinyxml2::XMLNode* node, tinyxml2::XMLDocument& doc) const;
		bool loadFromXML(tinyxml2::XMLNode* node, tinyxml2::XMLDocument& doc);

	private:
		std::string m_name;
		std::string m_sourceFile;
		std::vector<Mesh::MeshStrongPtr> m_meshes;
	};
}
