#pragma once
#include "System\Patterns\Singleton.hpp"

namespace box
{
	class NetworkManager
	{
		SINGLETON(NetworkManager);
	public:
		bool init();
		void deinit();
	};
}