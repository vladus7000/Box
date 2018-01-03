#pragma once
#include "System\Patterns\Singleton.hpp"

namespace box
{
	class KeyboardHandler
	{
	public:
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

#ifdef GAME_BUILD
		void poll(F32 delta);
#endif
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