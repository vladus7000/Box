#include "StdAfx.hpp"
#include "System/Process/ProcessManager.hpp"

namespace box
{
	SINGLETON_ACCESSOR(ProcessManager);

	void ProcessManager::update(F32 delta)
	{
		//adding to queue
		{
			for (auto& it : m_pendingToAdd)
			{
				m_processes.push_back(it);
			}
			m_pendingToAdd.clear();
		}

		auto it = m_processes.begin();
		while (it != m_processes.end())
		{
			Process::StrongProcessPtr process = (*it);
			auto thisIt = it;
			it++;

			if (process->getState() == Process::State::Uninitialized)
			{
				process->onInit();
			}

			if (process->getState() == Process::State::Running)
			{
				process->onUpdate(delta);
			}

			if (process->isDead())
			{
				switch (process->getState())
				{
					case Process::State::Succeeded:
					{
						process->onSucceed();
						if (Process::StrongProcessPtr child = process->removeChild())
						{
							attachProcess(child);
						}

					}
					break;
					case Process::State::Failed:
					{
						process->onFail();
					}
					break;
					case Process::State::Aborted:
					{
						process->onAbort();
					}
					break;
				}

				m_processes.erase(thisIt);
			}
		}
	}

	void ProcessManager::abortAllProcesses(bool immediate)
	{
		for (auto& it : m_processes)
		{
			it->onAbort();
		}
		m_processes.clear();
		m_pendingToAdd.clear();
	}

	Process::WeakProcessPtr ProcessManager::attachProcess(Process::StrongProcessPtr process)
	{
		m_pendingToAdd.push_back(process);
		Process::WeakProcessPtr ret = process;
		return ret;
	}
}