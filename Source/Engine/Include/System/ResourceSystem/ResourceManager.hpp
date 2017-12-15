#pragma once
#include "System\Patterns\Singleton.hpp"

namespace box
{
	class ResourceManager
	{
		SINGLETON(ResourceManager);
	public:
		bool init();
		void deinit();
	};
}