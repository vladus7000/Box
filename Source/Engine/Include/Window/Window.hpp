#pragma once
#include "System\Patterns\Singleton.hpp"

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

		void* getWindowHandle() const { return m_windowHandle; }
	private:
		void* m_windowHandle;

	};
#endif
}