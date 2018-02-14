#include "StdAfx.hpp"
#include "System/ResourceSystem/ResourceSystemExports.hpp"
#include "System/Engine.hpp"

#include "System/ResourceSystem/ResourceManager.hpp"

namespace Exports
{
	extern box::Engine* g_engineInstance;

namespace Resources
{
	int ImportModel(const char* fileName)
	{
		if (!g_engineInstance)
		{
			return -1;
		}

		box::ResourceManager::Instance().importModelFromFile(fileName);

		return 0;
	}
}
}