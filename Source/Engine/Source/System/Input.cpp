#include "StdAfx.hpp"
#include "System\Input.hpp"
#include <stdio.h>

namespace box
{
	SINGLETON_ACCESSOR(Input);

	bool Input::init()
	{
		return false;
	}

	void Input::deinit()
	{
	}

	void Input::test_register_key_event(U32 keyKode)
	{
		printf("Registered key %d \n", keyKode);
	}
}