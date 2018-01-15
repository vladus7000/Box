#include "StdAfx.hpp"
#include "Window\Window.hpp"

#include <DXUT11\Core\DXUT.h>


namespace box
{
	SINGLETON_ACCESSOR(Display);
	bool Display::init(void* displayHandle)
	{
		bool result = false;
		if (displayHandle)
		{
			m_displayHandle = displayHandle;
			result = true;
		}

		return result;
	}

	void Display::deinit()
	{
	}

#ifdef GAME_BUILD

	SINGLETON_ACCESSOR(Window);

	bool CALLBACK IsD3D11DeviceAcceptable(const CD3D11EnumAdapterInfo *AdapterInfo, UINT Output, const CD3D11EnumDeviceInfo *DeviceInfo,
		DXGI_FORMAT BackBufferFormat, bool bWindowed, void* pUserContext)
	{
		return true;
	}
	HRESULT CALLBACK OnD3D11CreateDevice(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc,
		void* pUserContext)
	{
		HRESULT hr;
		return hr;
	}
	HRESULT CALLBACK OnD3D11ResizedSwapChain(ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain,
		const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext)
	{
		HRESULT hr;
		return hr;
	}
	void CALLBACK OnD3D11FrameRender(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext, double fTime,
		float fElapsedTime, void* pUserContext)
	{
		return;
	}
	void CALLBACK OnD3D11ReleasingSwapChain(void* pUserContext)
	{
	
	}
	void CALLBACK OnD3D11DestroyDevice(void* pUserContext)
	{
	}
	bool CALLBACK ModifyDeviceSettings(DXUTDeviceSettings* pDeviceSettings, void* pUserContext)
	{
		return true;
	}
	LRESULT CALLBACK MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing,
		void* pUserContext)
	{
		return 0;
	}
	void CALLBACK OnKeyboard(UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext)
	{
	}

	bool Window::init()
	{

#if defined(DEBUG) | defined(_DEBUG)
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
		DXUTSetCallbackDeviceChanging(ModifyDeviceSettings);
		DXUTSetCallbackMsgProc(MsgProc);
		DXUTSetCallbackKeyboard(OnKeyboard);
	
		DXUTSetCallbackD3D11DeviceAcceptable(IsD3D11DeviceAcceptable);
		DXUTSetCallbackD3D11DeviceCreated(OnD3D11CreateDevice);
		DXUTSetCallbackD3D11SwapChainResized(OnD3D11ResizedSwapChain);
		DXUTSetCallbackD3D11FrameRender(OnD3D11FrameRender);
		DXUTSetCallbackD3D11SwapChainReleasing(OnD3D11ReleasingSwapChain);
		DXUTSetCallbackD3D11DeviceDestroyed(OnD3D11DestroyDevice);
	
		DXUTInit(true, true, NULL);
		DXUTSetCursorSettings(true, true);
		DXUTCreateWindow(L"Window");
		DXUTCreateDevice(D3D_FEATURE_LEVEL_11_0, true, 800, 600);
		return true;
	}

	void Window::deinit()
	{
	}

	void Window::pollEvents()
	{
		/*MSG msg;
		while (PeekMessage(&msg, static_cast<HWND>(m_windowHandle), NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}*/
	}
#endif
}