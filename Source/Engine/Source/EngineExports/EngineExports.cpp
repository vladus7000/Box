#include "StdAfx.hpp"
#include "System/Engine.hpp"
#include "System/Input.hpp"

namespace Exports
{
	using namespace box;
	Engine* g_engineInstance = nullptr;

namespace System
{
	int StartEngine(void* hwnd, int argc, char** argv)
	{
		if (g_engineInstance)
		{
			return -1;
		}

		g_engineInstance = new box::Engine();
		g_engineInstance->startup(hwnd);
		return 0;
	}

	int StopEngine()
	{
		if (!g_engineInstance)
		{
			return -1;
		}

		g_engineInstance->shutdown();
		delete g_engineInstance;
		g_engineInstance = nullptr;

		return 0;
	}

	int Update(box::F32 delta)
	{
		if (!g_engineInstance)
		{
			return -1;
		}
	}

	int RenderFrame(box::F32 delta)
	{
		if (!g_engineInstance)
		{
			return -1;
		}
	}
}//System

namespace Input
{
	int OnKeyDown(U32 key)
	{
		if (g_engineInstance)
		{
			box::Input::Instance().onKeyDown(key);
			return 0;
		}

		return -1;
	}

	int OnKeyUp(U32 key)
	{
		if (g_engineInstance)
		{
			box::Input::Instance().onKeyUp(key);
			return 0;
		}

		return -1;
	}

	int OnMouseMove(U32 x, U32 y)
	{
		if (g_engineInstance)
		{
			box::Input::Instance().onMouseMove(x, y);
			return 0;
		}

		return -1;
	}

	int OnMouseButtonDown(U32 x, U32 y, U32 key)
	{
		if (g_engineInstance)
		{
			box::Input::Instance().onMouseButtonDown(x, y, key);
			return 0;
		}

		return -1;
	}

	int OnMousebuttonUp(U32 x, U32 y, U32 key)
	{
		if (g_engineInstance)
		{
			box::Input::Instance().onMousebuttonUp(x, y, key);
			return 0;
		}

		return -1;
	}
} // Input
}