#include "StdAfx.hpp"

#include <algorithm>
#include <stdio.h>

#include "System\ResourceSystem\ResourceCache.hpp"
#include "System\ResourceSystem\ResourceFile.hpp"
#include "System\ResourceSystem\ResourceLoader.hpp"
#include "System\ResourceSystem\Resource.hpp"
#include "System\ResourceSystem\ResourceHandle.hpp"

#define LOG(args) printf(args)

namespace box
{
	class DefaultResourceLoader : public ResourceLoader
	{
	public:
		virtual const std::string getPattern() const override { return "*"; }
		virtual bool useRawFile() const override { return true; }
		virtual size_t getLoadedResourceSize(U8* buffer, size_t size) override { return size; }
		virtual bool loadResource(U8* buffer, size_t size, std::shared_ptr<ResourceHandle> handle) override { return true; };
	};

	ResourceCache::ResourceCache(size_t sizeInMb, ResourceFile* resFile)
		: m_cacheSize(sizeInMb * 1024 * 1024)
		, m_allocated(0)
		, m_resourceFile(resFile)
	{
		LOG("ResCache constructed\n");
	}

	bool wildcardMatch(const std::string& pattern, const std::string& test)
	{
		return true;
	}

	ResourceCache::~ResourceCache()
	{
		LOG("ResCache desctructed\n");
		while (!m_lru.empty())
		{
			freeOneResource();
		}

		if (m_resourceFile)
		{
			delete m_resourceFile;
			m_resourceFile = nullptr;
		}
	}

	bool ResourceCache::init()
	{
		LOG("ResCache inited\n");
		bool res = false;

		if (m_resourceFile->open())
		{
			registerLoader(std::shared_ptr<ResourceLoader>(new DefaultResourceLoader()));
			res = true;
		}

		return res;
	}

	void ResourceCache::registerLoader(std::shared_ptr<ResourceLoader> loader)
	{
		LOG("ResCache loader registered\n");
		m_loaders.push_back(loader);
	}

	std::shared_ptr<ResourceHandle> ResourceCache::getHandle(Resource& r)
	{
		LOG("ResCache get handle\n");
		std::shared_ptr<ResourceHandle> handle(find(r));
		if (handle)
		{
			LOG("ResCache: in cache\n");
			update(handle);
		}
		else
		{
			LOG("ResCache: cache miss\n");
			handle = load(r);
		}
		return handle;
	}

	void ResourceCache::preload(const std::string& pattern)
	{
	}

	void ResourceCache::flush()
	{
	}

	void ResourceCache::memoryHasBeenFreed(size_t size)
	{
		LOG("ResCache memoryHasBeenFreed\n");
		m_allocated -= size;
	}

	std::shared_ptr<ResourceHandle> ResourceCache::find(const Resource& r)
	{
		LOG("ResCache find\n");
		std::shared_ptr<ResourceHandle> handle;
		auto foundIt = m_resources.find(r.m_name);
		if (foundIt != m_resources.end())
		{
			handle = foundIt->second;
		}
		return handle;
	}

	const U8* ResourceCache::update(std::shared_ptr<ResourceHandle> handle)
	{
		LOG("ResCache update\n");
		auto foundIt = std::find_if(m_lru.begin(), m_lru.end(), [&](std::shared_ptr<ResourceHandle> element)
		{
			if (element->m_resource.m_name == handle->m_resource.m_name)
			{
				return true;
			}
			return false;
		});

		if (foundIt != m_lru.end())
		{
			std::shared_ptr<ResourceHandle> promote = *foundIt;
			m_lru.erase(foundIt);
			m_lru.push_front(promote);
		}

		return nullptr;
	}

	std::shared_ptr<ResourceHandle> ResourceCache::load(const Resource& r)
	{
		LOG("ResCache load\n");
		std::shared_ptr<ResourceHandle> handle;
		std::shared_ptr<ResourceLoader> loader;

		for (auto it : m_loaders)
		{
			if (wildcardMatch(it->getPattern(), r.m_name))
			{
				loader = it;
				break;
			}
		}
		if (!loader)
		{
			return handle;
		}

		size_t rawSize = m_resourceFile->getRawResourceSize(r);

		if (rawSize == 0)
		{
			return handle;
		}
		U8* rawBuffer = loader->useRawFile() ? allocate(rawSize) : new U8[rawSize];

		if (!rawBuffer)
		{
			return handle;
		}

		m_resourceFile->getRawResource(r, rawBuffer);
		U8* buffer(nullptr);

		if (loader->useRawFile())
		{
			buffer = rawBuffer;
			handle = std::make_shared<ResourceHandle>(r, buffer, rawSize, this);
		}
		else
		{
			size_t size = loader->getLoadedResourceSize(rawBuffer, rawSize);
			buffer = allocate(size);

			if (!buffer)
			{
				return handle;
			}

			handle = std::make_shared<ResourceHandle>(r, buffer, size, this);

			bool success = loader->loadResource(rawBuffer, rawSize, handle);
			delete[] rawBuffer;

			if (!success)
			{
				return std::shared_ptr<ResourceHandle>();
			}
		}

		if (handle)
		{
			m_lru.push_front(handle);
			m_resources[r.m_name] = handle;
		}

		return handle;
	}

	void ResourceCache::free(std::shared_ptr<ResourceHandle> gonner)
	{
		auto foundIt = std::find_if(m_lru.begin(), m_lru.end(), [&](std::shared_ptr<ResourceHandle> element)
		{
			if (element->m_resource.m_name == gonner->m_resource.m_name)
			{
				return true;
			}
			return false;
		});

		if (foundIt != m_lru.end())
		{
			m_lru.erase(foundIt);
			m_resources.erase(gonner->m_resource.m_name);
		}
	}

	bool ResourceCache::makeRoom(size_t size)
	{
		LOG("ResCache make room\n");
		if (size > m_cacheSize)
		{
			return false;
		}

		while (size > (m_cacheSize - m_allocated))
		{
			if (m_lru.empty())
			{
				return false;
			}

			freeOneResource();
		}

		return true;
	}

	U8* ResourceCache::allocate(size_t size)
	{
		LOG("ResCache allocate\n");
		if (!makeRoom(size))
		{
			return nullptr;
		}

		U8* mem = new U8[size];
		m_allocated += size;

		return mem;
	}

	void ResourceCache::freeOneResource()
	{
		LOG("ResCache freeOneResource\n");
		auto gonner = m_lru.end();
		gonner--;

		auto handle = *gonner;

		m_lru.pop_back();
		m_resources.erase(handle->m_resource.m_name);
	}
}