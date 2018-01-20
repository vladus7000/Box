#pragma once

namespace box
{
	struct AppMsg
	{
		HWND hwnd;
		UINT uMsg;
		WPARAM wParam;
		LPARAM lParam;
	};
}