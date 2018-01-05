#include "StdAfx.hpp"
#include "System\ResourceSystem\ResourceManager.hpp"
#include "System\ResourceSystem\ZipFile.hpp"
#include "System/Process/ProcessManager.hpp"
#include "System/Process/Processes/PreloadProcess.hpp"
#include "System/Process/Processes/LoadResourceProcess.hpp"

namespace
{
	const box::U32 DefaultCacheSizeMb = 100;
}

namespace box
{
	SINGLETON_ACCESSOR(ResourceManager);

	bool ResourceManager::init()
	{
		m_cache = new ResourceCache(DefaultCacheSizeMb);
		ResourceFile* resFile(nullptr);

		{
			resFile = new ResourceZipFile("configs.zip");
			m_cache->addResourceFile(resFile);
		}

		{
			resFile = new ResourceZipFile("sounds.zip");
			m_cache->addResourceFile(resFile);
		}

		{
			resFile = new ResourceZipFile("textures.zip");
			m_cache->addResourceFile(resFile);
		}

		{
			resFile = new ResourceZipFile("models.zip");
			m_cache->addResourceFile(resFile);
		}

		{
			resFile = new ResourceZipFile("animations.zip");
			m_cache->addResourceFile(resFile);
		}

		return m_cache->init();
	}

	void ResourceManager::deinit()
	{
		delete m_cache;
	}

	void ResourceManager::preload(char* wildcard, bool threaded)
	{
		if (!threaded)
		{
			m_cache->preload(wildcard);
		}
		else
		{
			Process::StrongProcessPtr load(new PreloadProcess(wildcard, m_cache));
			ProcessManager::Instance().attachProcess(load);
		}
	}

	std::shared_ptr<ResourceHandle> ResourceManager::getHandle(Resource& r, bool async)
	{
		if (!async)
		{
			return m_cache->getHandle(r);
		}
		else
		{
			std::shared_ptr<ResourceHandle> handle = m_cache->find(r);
			if (!handle)
			{
				handle = std::make_shared<ResourceHandle>(r, nullptr, 0, m_cache);
				m_cache->insertToLoadQueue(handle);
				Process::StrongProcessPtr load(new LoadResourceProcess(handle));
				ProcessManager::Instance().attachProcess(load);
			}
			return handle;
		}
	}
}