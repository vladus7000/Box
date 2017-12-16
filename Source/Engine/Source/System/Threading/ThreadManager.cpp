#include "StdAfx.hpp"
#include "System\Threading\ThreadManager.hpp"

namespace box
{
	SINGLETON_ACCESSOR(ThreadManager);

	bool ThreadManager::init()
	{
		return false;
	}

	void ThreadManager::deinit()
	{
	}
}