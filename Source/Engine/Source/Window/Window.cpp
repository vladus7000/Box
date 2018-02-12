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

	SINGLETON_ACCESSOR(Window);

	void CALLBACK OnKeyboard(UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext)
	{
	}

	bool Window::init(void* windowHandle)
	{
#ifdef EDITOR_BUILD
		DXUTSetWindow(static_cast<HWND>(windowHandle), static_cast<HWND>(windowHandle), static_cast<HWND>(windowHandle), false);
#else
		DXUTSetCallbackKeyboard(OnKeyboard);

		DXUTSetCursorSettings(true, true);
		DXUTCreateWindow(L"Window");
#endif
		RECT rcClient;
		m_windowHandle = DXUTGetHWND();
		m_windowHeight = m_windowWidth = 0;
		GetClientRect(DXUTGetHWND(), &rcClient);
		if (!IsIconic(DXUTGetHWND()))
		{
			GetClientRect(DXUTGetHWND(), &rcClient);

			m_windowWidth = (UINT)rcClient.right;
			m_windowHeight = (UINT)rcClient.bottom;
		}
		return true;
	}

	void Window::deinit()
	{
	}

	void Window::onResize()
	{
		DXUTResizeDXGIBuffers(0, 0, 0);
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
}