#pragma once
#include "System\Patterns\Singleton.hpp"

namespace box
{
	enum class ButtonCodes
	{
		//keyboards
		Key_W = 0x57,
		Key_A = 0x41,
		Key_S = 0x53,
		Key_D = 0x44,
		Key_Space = 0x20,
		Key_Shift = 0xA0, // L shift


	};
	struct KeyState
	{
		enum class State : U8
		{
			up,
			firstPress,
			pressed,
			firstUp,
		};
		State m_state;
		KeyState()
			: m_state(State::up)
		{
		}
		inline bool isPressed() { return (m_state == State::firstPress) || (m_state == State::pressed); }
	};

	class KeyboardHandler
	{
	public:
		virtual void keyState(KeyState state[256]) = 0;
		virtual void onKeyDown(U32 key) = 0;
		virtual void onKeyUp(U32 key) = 0;
	};

	class MouseHandler
	{
	public:
		enum MouseButtons
		{
			LEFT_BUTTON = 1,
			RIGHT_BUTTON = 2,
			MIDDLE_BUTTON = 4,
		};
		virtual void onMouseMove(U32 x, U32 y) = 0;
		virtual void onMouseButtonDown(U32 x, U32 y, U32 key) = 0;
		virtual void onMousebuttonUp(U32 x, U32 y, U32 key) = 0;
	};

	class Input
	{
		SINGLETON(Input);

	public:
		bool init();
		void deinit();

		void registerMouseHandler(MouseHandler* handler);
		void unregisterMouseHandler(MouseHandler* handler);

		void registerKeyboardHandler(KeyboardHandler* handler);
		void unregisterKeyboardHandler(KeyboardHandler* handler);

		void poll(F32 delta);

#ifdef GAME_BUILD
	private:
#endif
		void onKeyDown(U32 key);
		void onKeyUp(U32 key);
		void onMouseMove(U32 x, U32 y);
		void onMouseButtonDown(U32 x, U32 y, U32 key);
		void onMousebuttonUp(U32 x, U32 y, U32 key);

	private:
	};
}