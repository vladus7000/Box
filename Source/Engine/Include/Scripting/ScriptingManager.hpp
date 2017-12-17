#pragma once
#include "System\Patterns\Singleton.hpp"

namespace box
{
	class ScriptingManager
	{
		SINGLETON(ScriptingManager);
		public:
			bool init();
			void deinit();
	};
}