#include "StdAfx.hpp"
#include "System\ResourceSystem\ResourceManager.hpp"
#include "System\ResourceSystem\ZipFile.hpp"
#include "System/ResourceSystem/XMLResourceLoader.hpp"
#include "System/Process/ProcessManager.hpp"
#include "System/Process/Processes/PreloadProcess.hpp"
#include "System/Process/Processes/LoadResourceProcess.hpp"
#include "System\EventSystem\EventSystem.hpp"
#include "System\EventSystem\EngineEvents.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

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
			resFile = new ResourceZipFile("Configs.zip");
			m_cache->addResourceFile(resFile);
		}

		{
			resFile = new ResourceZipFile("Sounds.zip");
			m_cache->addResourceFile(resFile);
		}

		{
			resFile = new ResourceZipFile("Textures.zip");
			m_cache->addResourceFile(resFile);
		}

		{
			resFile = new ResourceZipFile("Models.zip");
			m_cache->addResourceFile(resFile);
		}

		{
			resFile = new ResourceZipFile("Animations.zip");
			m_cache->addResourceFile(resFile);
		}

		{
			resFile = new ResourceZipFile("Shaders.zip");
			m_cache->addResourceFile(resFile);
		}


		bool res = m_cache->init();
		if (res)
		{
			m_cache->registerLoader(std::shared_ptr<ResourceLoader>(new XMLResourceLoader()));

			std::shared_ptr<Event_ResourceCacheStarted> event(new Event_ResourceCacheStarted);
			EventSystem::Instance().raiseEvent(event);
		}
		return res;
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

	void ResourceManager::registerLoader(std::shared_ptr<ResourceLoader> loader)
	{
		m_cache->registerLoader(loader);
	}

	void ResourceManager::importModelFromFile(const std::string& fileName)
	{
		Assimp::Importer importer;
		const aiScene* assimpScene = importer.ReadFile(fileName, aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Quality);
		if (assimpScene)
		{
			//TODO: do something clever here.
		}
	}

}