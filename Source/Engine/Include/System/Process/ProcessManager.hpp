#pragma once

#include <list>

#include "System/Patterns/Singleton.hpp"
#include "Process.hpp"

namespace box
{
	class ProcessManager
	{
		SINGLETON(ProcessManager);
	public:
		using Processes = std::list<Process::StrongProcessPtr>;

	public:
		void update(F32 delta);
		void abortAllProcesses(bool immediate);

		Process::WeakProcessPtr attachProcess(Process::StrongProcessPtr process);

		U32 getProcessCount() const { return  static_cast<U32>(m_processes.size()); }

	private:
		Processes m_processes;
		Processes m_pendingToAdd;
	};
}