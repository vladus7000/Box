#include <Engine.hpp>
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
		StartEngine();
	}
	Engine::~Engine()
	{
		StopEngine();
	}
}