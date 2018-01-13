#include "StdAfx.hpp"
#include "System\Threading\ThreadManager.hpp"

namespace box
{
	SINGLETON_ACCESSOR(ThreadManager);

	bool ThreadManager::init()
	{
		return true;
	}

	void ThreadManager::deinit()
	{
		m_threads.clear();
	}

	void ThreadManager::abortAllThreads(bool immediate)
	{
		for (auto& it : m_threads)
		{
			it->stop();
		}
		m_threads.clear();
	}

	Thread::WeakThreadPtr ThreadManager::attachThread(Thread::StrongThreadPtr thread)
	{
		m_threads.push_back(thread);
		Thread::WeakThreadPtr ret = thread;
		return ret;
	}
}