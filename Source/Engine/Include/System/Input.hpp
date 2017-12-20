#pragma once
#include "System\Patterns\Singleton.hpp"

namespace box
{
	class Keyboard
	{
	public:
		void keyPress(U32 keyKode);
	};

	class Mouse
	{
		enum MouseButtons
		{ 
			LEFT_BUTTON = 1,
			RIGHT_BUTTON = 2,
			MIDDLE_BUTTON = 4,
		};
	public:
		void click(U32 mouseButtons);
		void move(U32 x, U32 y);
		void scroll(S32 value);
		void down(U32 mouseButtons);
		void up(U32 mouseButtons);

	};

	class Input
	{
		SINGLETON(Input);

	public:
		bool init();
		void deinit();

		Keyboard& keyboard() { return m_keyboard; }
		Mouse& mouse() { return m_mouse; }

	private:
		Keyboard m_keyboard;
		Mouse m_mouse;
	};
}