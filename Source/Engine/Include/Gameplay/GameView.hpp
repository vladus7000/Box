#pragma once

class ID3D11Device;
class IDXGISwapChain;
struct DXGI_SURFACE_DESC;

#include <System/AppMsg.hpp>

namespace box
{
	class GameView
	{
	public:
		virtual ~GameView() = default;

		virtual bool restore() = 0;
		virtual void deinit() = 0;
		virtual void deviceLost() = 0;

		virtual S32 render(F64 time, F32 delta) = 0;
		virtual void update(F64 fTime, F32 fElapsedTime) = 0;

		//virtual LRESULT MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext) = 0;
		virtual S32 msgProc(const AppMsg& msg) = 0;
		virtual HRESULT OnD3D11ResizedSwapChain(ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext) = 0;
	};
}