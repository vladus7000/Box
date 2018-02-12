#include "StdAfx.hpp"
#include "System/ResourceSystem/ResourceSystemExports.hpp"
#include "System/Engine.hpp"

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

		return 0;
	}
}
}