#pragma once

#include "System/Types.hpp"

#include "System/ResourceSystem/ResourceSystemExports.hpp"

namespace Exports
{
	namespace System
	{
		int StartEngine(void* hwnd, int argc, char** argv);
		int StopEngine();

		int WndProc(void* hwnd, int msg, int wParam, int lParam);

		int Update(box::F32 delta);
		int RenderFrame(box::F32 delta);

		int Resize(int width, int height);
	}

	namespace Input
	{
		int OnKeyDown(box::U32 key);
		int OnKeyUp(box::U32 key);
		int OnMouseMove(box::U32 x, box::U32 y);
		int OnMouseButtonDown(box::U32 x, box::U32 y, box::U32 key);
		int OnMousebuttonUp(box::U32 x, box::U32 y, box::U32 key);
	}

	namespace Editor
	{
		int SetRenderPanelActive(int active);

		int SetCameraFov(float fov);
		int SetCameraZNear(float zNear);
		int SetCameraZFar(float zFar);
		int SetCameraMovementSpeed(float speed);
	}

}
