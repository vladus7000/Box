#include "StdAfx.hpp"
#include "Window\Window.hpp"

#include <DXUT11\Core\DXUT.h>

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

	void CALLBACK OnKeyboard(UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext)
	{
	}

	bool Window::init()
	{
		DXUTSetCallbackKeyboard(OnKeyboard);
	
		DXUTSetCursorSettings(true, true);
		DXUTCreateWindow(L"Window");
		
		return true;
	}

	void Window::deinit()
	{
	}

	void Window::pollEvents()
	{
		/*MSG msg;
		while (PeekMessage(&msg, static_cast<HWND>(m_windowHandle), NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}*/
	}
#endif
}