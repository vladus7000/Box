#pragma once
#include "System\Patterns\Singleton.hpp"

namespace box
{
	class Renderer
	{
		SINGLETON(Renderer);
	public:
		bool init();
		void deinit();
	};
}