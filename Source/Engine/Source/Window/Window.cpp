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

	LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_DESTROY:
			Window::Instance().closeWindow();
			break;
		}
		return DefWindowProc(hwnd, message, wParam, lParam);
	}

	SINGLETON_ACCESSOR(Window);

	bool Window::init()
	{
		TCHAR szAppName[] = TEXT("System");
		WNDCLASS wndclass;

		HINSTANCE instance = GetModuleHandle(NULL);

		wndclass.style = CS_HREDRAW | CS_VREDRAW;
		wndclass.lpfnWndProc = WndProc;
		wndclass.cbClsExtra = 0;
		wndclass.cbWndExtra = 0;
		wndclass.hInstance = instance;
		wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wndclass.lpszMenuName = NULL;
		wndclass.lpszClassName = szAppName;

		RegisterClass(&wndclass);

		U32 w = 800;
		U32 h = 600;
		HWND hwnd = CreateWindow( szAppName, "Game", WS_OVERLAPPEDWINDOW, 0, 0, w, h, NULL, NULL, instance, NULL);
		
		ShowWindow(hwnd, SW_SHOW);
		UpdateWindow(hwnd);

		m_windowHandle = static_cast<void*>(hwnd);
		m_windowIsClosed = false;

		return hwnd != nullptr;
	}

	void Window::deinit()
	{
	}

	void Window::pollEvents()
	{
		MSG msg;
		while (PeekMessage(&msg, static_cast<HWND>(m_windowHandle), NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

#endif
}