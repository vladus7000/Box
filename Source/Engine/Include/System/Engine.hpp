#pragma once
#include "MachineInfo.hpp"

#include <memory>
namespace box
{
	class GameView;
}

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

		void attachGameView(std::shared_ptr<GameView> view);
		void enterMainLoop();

	private:
		MachineInfo m_machineInfo;
	};
}