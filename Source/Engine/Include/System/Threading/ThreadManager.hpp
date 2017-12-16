#pragma once
#include "System\Patterns\Singleton.hpp"

namespace box
{
	class ThreadManager
	{
	SINGLETON(ThreadManager);
	public:
		bool init();
		void deinit();
	};
}