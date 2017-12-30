#pragma once

#include <memory>
#include <list>
#include <map>
#include <string>

namespace box
{
	class Resource;
	class ResourceLoader;
	class ResourceFile;
	class ResourceHandle;
}

namespace box
{
	class ResourceCache
	{
	public:
		ResourceCache(size_t sizeInMb, ResourceFile* resFile);
		~ResourceCache();

		bool init();
		void registerLoader(std::shared_ptr<ResourceLoader> loader);
		std::shared_ptr<ResourceHandle> getHandle(Resource& r);

		void preload(const std::string& pattern);
		void flush();
		void memoryHasBeenFreed(size_t size);

	private:
		using ResourceHandleList = std::list<std::shared_ptr<ResourceHandle>>;
		using ResourceHandleMap = std::map<std::string, std::shared_ptr<ResourceHandle>>;
		using ResourceLoaders = std::list<std::shared_ptr<ResourceLoader>>;

		std::shared_ptr<ResourceHandle> find(const Resource& r);
		const U8* update(std::shared_ptr<ResourceHandle> handle);
		std::shared_ptr<ResourceHandle> load(const Resource& r);
		void free(std::shared_ptr<ResourceHandle> gonner);

		bool makeRoom(size_t size);
		U8* allocate(size_t size);
		void freeOneResource();

	private:
		ResourceHandleList m_lru;
		ResourceHandleMap m_resources;
		ResourceLoaders m_loaders;
		ResourceFile* m_resourceFile;
		size_t m_cacheSize;
		size_t m_allocated;
	};
}