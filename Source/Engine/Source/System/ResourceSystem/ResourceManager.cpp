#include "StdAfx.hpp"
#include "System\ResourceSystem\ResourceManager.hpp"

namespace box
{
	SINGLETON_ACCESSOR(ResourceManager);

	bool ResourceManager::init()
	{
		return false;
	}

	void ResourceManager::deinit()
	{
	}

	void ResourceManager::preload(char* wildcard, bool threaded)
	{

	}
}