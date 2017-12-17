#pragma once
#include "System\Patterns\Singleton.hpp"

namespace box
{
	class Input
	{
		SINGLETON(Input);

	public:
		bool init();
		void deinit();

		//test
		void test_register_key_event(U32 keyKode);

	private:
	};
}