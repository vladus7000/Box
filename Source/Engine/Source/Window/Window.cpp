#include "StdAfx.hpp"
#include "Window\Window.hpp"

namespace box
{
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

	Display& Display::Instance()
	{
		static Display d;
		return d;
	}
}