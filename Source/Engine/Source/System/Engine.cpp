#include "StdAfx.hpp"
#include <System/Engine.hpp>
#include "System/Memory/Allocator.hpp"
#include "System/RunEnvironment.hpp"
#include "System/ResourceSystem/ResourceManager.hpp"
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
		result &= Allocator::Instance().init();
		result &= RunEnvironment::Instance().init(argc, argv);
		result &= ResourceManager::Instance().init();
		//create window
		//init scripts
		//init render
		//init sound
		//init network
		//init random
		//init cheats

		return result;
	}

	void Engine::shutdown()
	{
		//deinit cheats
		//deinit random
		//deinit network
		//deinit sound
		//deinit render
		//deinit scripts
		//close window
		ResourceManager::Instance().deinit();
		RunEnvironment::Instance().deinit();
		Allocator::Instance().deinit();
	}
}