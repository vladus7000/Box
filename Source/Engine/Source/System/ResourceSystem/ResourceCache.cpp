#include "StdAfx.hpp"

#include <algorithm>
#include <stdio.h>
#include <regex>

#include "System\ResourceSystem\ResourceCache.hpp"
#include "System\ResourceSystem\ResourceFile.hpp"
#include "System\ResourceSystem\ResourceLoader.hpp"
#include "System\ResourceSystem\Resource.hpp"

#define LOG(args) printf(args)

namespace box
{
	class DefaultResourceLoader : public ResourceLoader
	{
	public:
		virtual const std::string getPattern() const override { return "*"; }
		virtual bool useRawFile() const override { return true; }
		virtual size_t getLoadedResourceSize(U8* buffer, size_t size) override { return size; }
		virtual bool loadResource(U8* buffer, size_t size, std::shared_ptr<ResourceHandle> handle) override { return true; }
		virtual bool discardRawBufferAfterLoad() const override { return true; }
	};

	ResourceCache::ResourceCache(size_t sizeInMb, ResourceFile* resFile)
		: m_cacheSize(sizeInMb * 1024 * 1024)
		, m_allocated(0)
	{
		if (resFile)
		{
			m_resourceFiles.push_back(resFile);
		}
	}

	bool match(char const *needle, char const *haystack) {
		for (; *needle != '\0'; ++needle) {
			switch (*needle) {
			case '?':
				if (*haystack == '\0')
					return false;
				++haystack;
				break;
			case '*': {
				if (needle[1] == '\0')
					return true;
				size_t max = strlen(haystack);
				for (size_t i = 0; i < max; i++)
					if (match(needle + 1, haystack + i))
						return true;
				return false;
			}
			default:
				if (*haystack != *needle)
					return false;
				++haystack;
			}
		}
		return *haystack == '\0';
	}

	bool wildcardMatch(const char* pattern, const char* test)
	{
		//std::regex re(pattern);
		//std::smatch res;
		return match(pattern, test);//std::regex_match(test, re);
	}

	ResourceCache::~ResourceCache()
	{
		while (!m_lru.empty())
		{
			freeOneResource();
		}

		for (auto resFile : m_resourceFiles)
		{
			delete resFile;
		}
	}

	void ResourceCache::addResourceFile(ResourceFile* resourceFile)
	{
		if (resourceFile && resourceFile->open())
		{
			m_resourceFiles.push_back(resourceFile);
		}
	}

	bool ResourceCache::init()
	{
		bool res = true;

		for (auto resFile : m_resourceFiles)
		{
			res &= resFile->open();
		}

		registerLoader(std::shared_ptr<ResourceLoader>(new DefaultResourceLoader()));

		return res;
	}

	void ResourceCache::registerLoader(std::shared_ptr<ResourceLoader> loader)
	{
		m_loaders.push_back(loader);
	}

	std::shared_ptr<ResourceHandle> ResourceCache::getHandle(Resource& r)
	{
		std::shared_ptr<ResourceHandle> handle(find(r));
		if (handle)
		{
			update(handle);
		}
		else
		{
			handle = load(r);
		}
		return handle;
	}

	void ResourceCache::preload(const std::string& pattern)
	{
		for (auto resFile : m_resourceFiles)
		{
			auto count = resFile->getResourcesCount();
			for (size_t i = 0; i < count; i++)
			{
				std::string name = resFile->getResourceName(i);
				if (wildcardMatch(pattern.c_str(), name.c_str()))
				{
					Resource r(name);
					auto handle = find(r);
					if (!handle)
					{
						load(r);
					}
				}
			}
		}
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

	void ResourceCache::insertToSystem(std::shared_ptr<ResourceHandle> handle)
	{
		m_lru.push_front(handle);
		m_resources[handle->m_resource.m_name] = handle;
	}

	std::shared_ptr<ResourceHandle> ResourceCache::load(const Resource& r)
	{
		LOG("ResCache load\n");
		std::shared_ptr<ResourceHandle> handle;
		std::shared_ptr<ResourceLoader> loader;

		for (auto it : m_loaders)
		{
			if (wildcardMatch(it->getPattern().c_str(), r.m_name.c_str()))
			{
				loader = it;
				break;
			}
		}
		if (!loader)
		{
			return handle;
		}

		ResourceFile* foundResFile(nullptr);
		size_t rawSize(0);
		for (auto resFile : m_resourceFiles)
		{
			rawSize = resFile->getRawResourceSize(r);
			if (rawSize > 0)
			{
				foundResFile = resFile;
				break;
			}
		}

		if (rawSize <= 0 || foundResFile == nullptr)
		{
			return handle;
		}
		U8* rawBuffer = loader->useRawFile() ? allocate(rawSize) : new U8[rawSize];

		if (!rawBuffer)
		{
			return handle;
		}

		foundResFile->getRawResource(r, rawBuffer);
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
			handle->setDataReady();
			insertToSystem(handle);
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