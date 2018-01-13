#pragma once
#include "System\Patterns\Singleton.hpp"
#include "System\Threading\Thread.hpp"

#include <list>

namespace box
{
	class ThreadManager
	{
	SINGLETON(ThreadManager);
	public:
		using Threads = std::list<Thread::StrongThreadPtr>;

		bool init();
		void deinit();

		void abortAllThreads(bool immediate);
		Thread::WeakThreadPtr attachThread(Thread::StrongThreadPtr thread);
	private:
		Threads m_threads;
	};
}