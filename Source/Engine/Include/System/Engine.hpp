#pragma once
#include "MachineInfo.hpp"

namespace box
{
	class Engine
	{
	public:
		Engine();
		~Engine();

		bool startup(
#ifdef EDITOR_BUILD
			void* hwnd, // the editor already has created hwnd
#endif
			U32 argc = 0, char** argv = nullptr);
		void shutdown();

	private:
		MachineInfo m_machineInfo;
	};
}