#pragma once

#include <memory>

#include "System\Patterns\Singleton.hpp"
#include "Resource.hpp"
#include "ResourceFile.hpp"
#include "ResourceHandle.hpp"
#include "ResourceCache.hpp"

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

		void importModelFromFile(const std::string& fileName);

	private:
		ResourceCache* m_cache;
	};
}