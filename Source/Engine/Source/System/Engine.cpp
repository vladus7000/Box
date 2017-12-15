#include "StdAfx.hpp"
#include <System/Engine.hpp>
#include <stdio.h>

namespace box
{
	void StartEngine()
	{
#ifdef EDITOR_BUILD
		printf("EDITOR: Engine started\n");
#endif
#ifdef GAME_BUILD
		printf("GAME: Engine started\n");
#endif
	}

	void StopEngine()
	{
#ifdef EDITOR_BUILD
		printf("EDITOR: Engine stopped\n");
#endif
#ifdef GAME_BUILD
		printf("GAME: Engine stopped\n");
#endif
	}

	Engine::Engine()
	{
	}

	Engine::~Engine()
	{
	}

	bool Engine::startup(U32 argc, char** argv)
	{
		bool result = true;
		result &= m_machineInfo.detect();

		return result;
	}

	void Engine::shutdown()
	{

	}
}