#pragma once
#include "System\Patterns\Singleton.hpp"
#include <windows.h>

namespace box
{
	class Display
	{
		SINGLETON(Display);
	public:
		bool init(void* displayHandle);
		void deinit();

		void* getDisplayHandle() const { return m_displayHandle; }

	private:
		void* m_displayHandle;
	};

#ifdef GAME_BUILD
	class Window
	{
		SINGLETON(Window);
	public:
		bool init();
		void deinit();

		void pollEvents();

		void* getWindowHandle() const { return m_windowHandle; }

		bool windowIsClosed() const { return m_windowIsClosed; } // hack
		void closeWindow() { m_windowIsClosed = true; }

	private:
		void* m_windowHandle;
		bool m_windowIsClosed; // hack

	};
#endif
}