#include "StdAfx.hpp"
#include "System\Input.hpp"
#include <stdio.h>

namespace box
{
	SINGLETON_ACCESSOR(Input);

	void Keyboard::keyPress(U32 keyKode)
	{
		printf("Registered key %d \n", keyKode);
	}

	void Mouse::click(U32 mouseButtons)
	{
		printf("mouse click\n");
	}

	void Mouse::move(U32 x, U32 y)
	{
		printf("mouse move\n");
	}

	void Mouse::scroll(S32 value)
	{
		printf("mouse scroll\n");
	}

	void Mouse::down(U32 mouseButtons)
	{
		printf("mouse down\n");
	}

	void Mouse::up(U32 mouseButtons)
	{
		printf("mouse up\n");
	}

	bool Input::init()
	{
		return false;
	}

	void Input::deinit()
	{
	}
}