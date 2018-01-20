#pragma once
#include "MachineInfo.hpp"

#ifdef GAME_BUILD
#include <memory>
namespace box
{
	class GameView;
}
#endif

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

#ifdef GAME_BUILD
		void attachGameView(std::shared_ptr<GameView> view);
		void enterMainLoop();
#endif

	private:
		MachineInfo m_machineInfo;
	};
}