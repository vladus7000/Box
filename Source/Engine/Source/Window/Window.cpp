#include "StdAfx.hpp"
#include "Window\Window.hpp"

namespace box
{
	SINGLETON_ACCESSOR(Display);
	bool Display::init(void* displayHandle)
	{
		bool result = false;
		if (displayHandle)
		{
			m_displayHandle = displayHandle;
			result = true;
		}

		return result;
	}

	void Display::deinit()
	{
	}

#ifdef GAME_BUILD
	SINGLETON_ACCESSOR(Window);
	bool Window::init()
	{
		return false;
	}
	void Window::deinit()
	{
	}
#endif
}