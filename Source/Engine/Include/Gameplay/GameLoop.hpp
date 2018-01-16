#pragma once

class ID3D11Device;
class IDXGISwapChain;
struct DXGI_SURFACE_DESC;

namespace box
{
	class GameLoop
	{
	public:
		virtual ~GameLoop() = default;
		virtual void logicTick(F64 fTime, F32 fElapsedTime) = 0;
		virtual void leaveGameLoop() = 0;
		virtual LRESULT MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing, void* pUserContext) = 0;
		virtual HRESULT OnD3D11ResizedSwapChain(ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext) = 0;
	};
}