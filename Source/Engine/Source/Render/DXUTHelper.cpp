#include "StdAfx.hpp"
#include <windows.h>
#include "Render\DXUTHelper.hpp"

#include <DXUT11\Core\DXUT.h>
#include "DXUT11\Optional\SDKmisc.h"
#include "DXUT11\Optional\DXUTgui.h"

HRESULT WINAPI DXTraceW(__in_z const char* strFile, __in DWORD dwLine, __in HRESULT hr, __in_z_opt const WCHAR* strMsg, __in BOOL bPopMsgBox)
{
	return 0;
}

namespace box
{
namespace DXUT
{


	CDXUTDialogResourceManager* g_DialogResourceManager;
	CDXUTTextHelper* g_TextHelper = nullptr;

	void Restore(ID3D11Device* device, ID3D11DeviceContext* context)
	{
		Deinit();
		g_DialogResourceManager = new CDXUTDialogResourceManager;
		g_DialogResourceManager->OnD3D11DestroyDevice();
		g_DialogResourceManager->AddTexture(MAKEINTRESOURCE(0xFFFF), (HMODULE)0xFFFF);
		g_DialogResourceManager->OnD3D11CreateDevice(device, context);
		g_TextHelper = new CDXUTTextHelper(device, context, g_DialogResourceManager, 15);
	}

	void Deinit()
	{
		if (g_DialogResourceManager)
		{
			g_DialogResourceManager->OnD3D11DestroyDevice();
			delete g_DialogResourceManager;
		}
		if (g_TextHelper)
		{
			delete g_TextHelper;
		}
	}

	AppMsg::Status MsgProc(const AppMsg& msg)
	{
		if (g_DialogResourceManager->MsgProc(msg.hwnd, msg.uMsg, msg.wParam, msg.lParam))
		{
			return AppMsg::Status::Processed;
		}
		return AppMsg::Status::DefaultAction;
	}

	CDXUTDialogResourceManager& GetDialogResourceManager()
	{
		return *g_DialogResourceManager;
	}

	CDXUTTextHelper& GetTextHelper()
	{
		return *g_TextHelper;
	}

}
}
