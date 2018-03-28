#include "StdAfx.hpp"
#include "System\Input.hpp"
#include <stdio.h>
#include <list>
#include <windows.h>


namespace
{
	std::list<box::KeyboardHandler*> KeyboardHandlers;
	std::list<box::MouseHandler*> MousedHandlers;
}

namespace box
{
	SINGLETON_ACCESSOR(Input);

	class EditorInputController : public KeyboardHandler, public MouseHandler
	{
	public:
		virtual void keyState(KeyState state[256]) override
		{

		}

		virtual void onKeyDown(U32 key) override
		{
			printf("onKeyDown key %d \n", key);
		}
		virtual void onKeyUp(U32 key) override
		{
			printf("onKeyUp key %d \n", key);
		}

		virtual void onMouseMove(U32 x, U32 y) override
		{
			printf("mouse onMouseMove\n");
		}
		virtual void onMouseButtonDown(U32 x, U32 y, U32 key) override
		{
			printf("mouse click\n");
		}
		virtual void onMousebuttonUp(U32 x, U32 y, U32 key) override
		{
			printf("mouse up\n");
		}
	}g_editorInputController;

	bool Input::init()
	{
		registerKeyboardHandler(&g_editorInputController);
		registerMouseHandler(&g_editorInputController);
		return true;
	}

	void Input::deinit()
	{
		unregisterKeyboardHandler(&g_editorInputController);
		unregisterMouseHandler(&g_editorInputController);
		MousedHandlers.clear();
		KeyboardHandlers.clear();
	}

	void Input::registerMouseHandler(MouseHandler* handler)
	{
		auto fountIt = std::find(MousedHandlers.begin(), MousedHandlers.end(), handler);

		if (fountIt == MousedHandlers.end())
		{
			MousedHandlers.push_back(handler);
		}
	}

	void Input::unregisterMouseHandler(MouseHandler* handler)
	{
		auto fountIt = std::find(MousedHandlers.begin(), MousedHandlers.end(), handler);

		if (fountIt != MousedHandlers.end())
		{
			MousedHandlers.erase(fountIt);
		}
	}

	void Input::registerKeyboardHandler(KeyboardHandler* handler)
	{
		auto fountIt = std::find(KeyboardHandlers.begin(), KeyboardHandlers.end(), handler);

		if (fountIt == KeyboardHandlers.end())
		{
			KeyboardHandlers.push_back(handler);
		}
	}

	void Input::unregisterKeyboardHandler(KeyboardHandler* handler)
	{
		auto fountIt = std::find(KeyboardHandlers.begin(), KeyboardHandlers.end(), handler);

		if (fountIt != KeyboardHandlers.end())
		{
			KeyboardHandlers.erase(fountIt);
		}
	}

	void Input::poll(F32 delta)
	{
		static KeyState keyState[256];
		for (U32 key = 7; key < 256; key++)
		{
			KeyState& currentKey = keyState[key];
			const U16 currentState = GetAsyncKeyState(key);
			const bool pressed = (currentState & 0x8000) != 0;
			switch (currentKey.m_state)
			{
			case KeyState::State::up:
				if (pressed) currentKey.m_state = KeyState::State::firstPress;
				break;
			case KeyState::State::firstPress:
				if (pressed) currentKey.m_state = KeyState::State::pressed;
				break;
			case KeyState::State::pressed:
				if (!pressed) currentKey.m_state = KeyState::State::firstUp;
				break;
			default:
				break;
			}

			if (currentKey.m_state == KeyState::State::firstPress)
			{
				onKeyDown(key);
			}
			if (currentKey.m_state == KeyState::State::firstUp)
			{
				currentKey.m_state = KeyState::State::up;
				onKeyUp(key);
			}
		}
		for (auto it : KeyboardHandlers)
		{
			it->keyState(keyState);
		}
	}

	void Input::onKeyDown(U32 key)
	{
		for (auto it : KeyboardHandlers)
		{
			it->onKeyDown(key);
		}
	}

	void Input::onKeyUp(U32 key)
	{
		for (auto it : KeyboardHandlers)
		{
			it->onKeyUp(key);
		}
	}

	void Input::onMouseMove(U32 x, U32 y)
	{
		for (auto it : MousedHandlers)
		{
			it->onMouseMove(x, y);
		}
	}

	void Input::onMouseButtonDown(U32 x, U32 y, U32 key)
	{
		for (auto it : MousedHandlers)
		{
			it->onMouseButtonDown(x, y, key);
		}
	}

	void Input::onMousebuttonUp(U32 x, U32 y, U32 key)
	{
		for (auto it : MousedHandlers)
		{
			it->onMousebuttonUp(x, y, key);
		}
	}
}