#pragma once
#include "MachineInfo.hpp"

namespace box
{
	class Engine
	{
	public:
		Engine();
		~Engine();

		bool startup(U32 argc = 0, char** argv = nullptr);
		void shutdown();

	private:
		MachineInfo m_machineInfo;
	};
}