#pragma once
#include "System\Patterns\Singleton.hpp"

namespace box
{
	class CheatManager
	{
		SINGLETON(CheatManager);
	public:
		bool init();
		void deinit();
	};
}