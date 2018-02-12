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

	class Window
	{
		SINGLETON(Window);
	public:
		bool init(void* windowHandle = nullptr);
		void deinit();

		void pollEvents();

		void* getWindowHandle() const { return m_windowHandle; }
		U32 getHeight() const { return m_windowHeight; }
		U32 getWidth() const { return m_windowWidth; }

		void onResize();

	private:
		void* m_windowHandle;
		U32 m_windowHeight;
		U32 m_windowWidth;
	};
}