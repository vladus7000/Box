#pragma once
#include "System\Patterns\Singleton.hpp"

namespace box
{
	class SoundSystem
	{
		SINGLETON(SoundSystem);
	public:
		bool init();
		void deinit();
	};
}