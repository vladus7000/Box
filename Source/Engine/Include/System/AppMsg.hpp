#pragma once

#include <windows.h>

namespace box
{
	struct AppMsg
	{
		enum class Status
		{
			Processed,
			DefaultAction
		};

		HWND hwnd;
		UINT uMsg;
		WPARAM wParam;
		LPARAM lParam;
	};
}