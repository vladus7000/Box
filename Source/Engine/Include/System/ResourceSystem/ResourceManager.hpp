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

		void preload(char* wildcard, bool threaded = false);

	private:
	};
}