#pragma once

#include <memory>
#include <list>
#include <map>
#include <string>
#include <vector>

#include "System\ResourceSystem\ResourceHandle.hpp"

namespace box
{
	class Resource;
	class ResourceLoader;
	class ResourceFile;
}

namespace box
{
	class ResourceCache
	{
	public:
		ResourceCache(size_t sizeInMb, ResourceFile* resFile = nullptr);
		~ResourceCache();

		void addResourceFile(ResourceFile*);

		bool init();
		void registerLoader(std::shared_ptr<ResourceLoader> loader);
		std::shared_ptr<ResourceHandle> getHandle(Resource& r);
		std::shared_ptr<ResourceHandle> find(const Resource& r);

		void preload(const std::string& pattern);
		void flush();
		void memoryHasBeenFreed(size_t size);

		void insertToSystem(std::shared_ptr<ResourceHandle> handle);
		void insertToLoadQueue(std::shared_ptr<ResourceHandle> handle);
		void clearFromLoadQueue(std::shared_ptr<ResourceHandle> handle);

	private:
		using ResourceHandleList = std::list<std::shared_ptr<ResourceHandle>>;
		using ResourceHandleMap = std::map<std::string, std::shared_ptr<ResourceHandle>>;
		using ResourceLoaders = std::list<std::shared_ptr<ResourceLoader>>;
		using ResourceFiles = std::vector<ResourceFile*>;

		const U8* update(std::shared_ptr<ResourceHandle> handle);
		std::shared_ptr<ResourceHandle> load(const Resource& r);
		void load(std::shared_ptr<ResourceHandle> handle);
		void free(std::shared_ptr<ResourceHandle> gonner);

		bool makeRoom(size_t size);
		U8* allocate(size_t size);
		void freeOneResource();

	private:
		friend class PreloadProcess;
		friend class LoadResourceProcess;

		ResourceHandleList m_lru;
		ResourceHandleMap m_resources;
		ResourceHandleMap m_waitingForLoading;
		ResourceLoaders m_loaders;
		ResourceFiles m_resourceFiles;
		size_t m_cacheSize;
		size_t m_allocated;
		const bool m_editorMode;
	};
}