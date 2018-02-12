#pragma once

#include "System/Types.hpp"

namespace Exports
{
	namespace System
	{
		int StartEngine(void* hwnd, int argc, char** argv);
		int StopEngine();

		int Update(box::F32 delta);
		int RenderFrame(box::F32 delta);
	}

	namespace Input
	{
		int OnKeyDown(box::U32 key);
		int OnKeyUp(box::U32 key);
		int OnMouseMove(box::U32 x, box::U32 y);
		int OnMouseButtonDown(box::U32 x, box::U32 y, box::U32 key);
		int OnMousebuttonUp(box::U32 x, box::U32 y, box::U32 key);
	}
}
