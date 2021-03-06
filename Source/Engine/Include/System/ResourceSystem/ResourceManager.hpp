#pragma once

#include <memory>

#include "System\Patterns\Singleton.hpp"
#include "Resource.hpp"
#include "ResourceFile.hpp"
#include "ResourceHandle.hpp"
#include "ResourceCache.hpp"

#include "Render/Model.hpp"

namespace box
{
	class ResourceManager
	{
		SINGLETON(ResourceManager);
	public:
		bool init();
		void deinit();

		void preload(char* wildcard, bool threaded = false);

		std::shared_ptr<ResourceHandle> getHandle(Resource& r, bool async = false);
		void registerLoader(std::shared_ptr<ResourceLoader> loader);

		int importStaticModel(const std::string& fileName, const std::string& modelName);
		int ImportDynamicModel(const std::string& fileName);

		void loadModel(const std::string& fileName, Model& model);

		int ImportShader(const std::string& fileName);
		int ImportDDSTexture(const std::string& fileName);

		int CompileShader(const std::string& fileName);

		int getResourceCollectionSizeForXml();
		void serializeResourceCollectionToXml(char* out);

		void resyncResourceFolders();

	private:
		void loadStaticModelFromFile(const std::string& fileName, std::vector<box::Mesh::MeshStrongPtr>& outMeshes, bool readMaterials = true,  bool saveNewMaterials = true);
		ResourceCache* m_cache;
	};
}