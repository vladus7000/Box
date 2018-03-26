#include "StdAfx.hpp"
#include "Render\Renderer.hpp"
#include "System\ResourceSystem\ResourceManager.hpp"
#include "Render\ResourceLoaders\DDSTextureResourceLoader.hpp"
#include "Render\ResourceLoaders\ShaderResourceLoader.hpp"
#include <Render/ResourceLoaders/SdkmeshResourceLoader.hpp>
#include "Window\Window.hpp"
#include "Render\Mesh.hpp"

#include <DXUT11\Core\DXUT.h>
#include "Render\DXUTHelper.hpp"
#include "DXUT11\Optional\DXUTgui.h"

namespace box
{
	SINGLETON_ACCESSOR(Renderer);

	bool CALLBACK IsD3D11DeviceAcceptable(const CD3D11EnumAdapterInfo *AdapterInfo, UINT Output, const CD3D11EnumDeviceInfo *DeviceInfo,
		DXGI_FORMAT BackBufferFormat, bool bWindowed, void* pUserContext)
	{
		return true;
	}

	HRESULT CALLBACK OnD3D11CreateDevice(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc,
		void* pUserContext)
	{
		HRESULT hr;
		auto context = DXUTGetD3D11DeviceContext();
		DXUT::Restore(pd3dDevice, context);
		return hr;
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

	HRESULT CALLBACK OnD3D11ResizedSwapChain(ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain,
		const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext)
	{
		auto& dialogResourceManager = box::DXUT::GetDialogResourceManager();
		dialogResourceManager.OnD3D11ResizedSwapChain(pd3dDevice, pBackBufferSurfaceDesc);
		return S_OK;
	}

	bool Renderer::init()
	{
		bool result = true;
		ResourceManager::Instance().registerLoader(std::make_shared<DDSTextureResourceLoader>());
		ResourceManager::Instance().registerLoader(std::make_shared<ShaderResourceLoader>());
		ResourceManager::Instance().registerLoader(std::make_shared<SdkmeshResourceLoader>());

		DXUTSetCallbackDeviceChanging(ModifyDeviceSettings);
		DXUTSetCallbackD3D11DeviceAcceptable(IsD3D11DeviceAcceptable);
		DXUTSetCallbackD3D11DeviceCreated(OnD3D11CreateDevice);

		DXUTSetCallbackD3D11SwapChainReleasing(OnD3D11ReleasingSwapChain);
		DXUTSetCallbackD3D11DeviceDestroyed(OnD3D11DestroyDevice);
		DXUTSetCallbackD3D11SwapChainResized(OnD3D11ResizedSwapChain);
		DXUTCreateDevice(D3D_FEATURE_LEVEL_11_0, true, Window::Instance().getWidth(), Window::Instance().getHeight());

		m_context = DXUTGetD3D11DeviceContext();
		m_device = DXUTGetD3D11Device();

		result &= (m_context && m_device);

		result &= m_frameGlobals.restore(m_device);

		return result;
	}

	void Renderer::deinit()
	{
		DXUT::Deinit();
	}

	void Renderer::renderScene(F32 delta)
	{
		if (m_scene)
		{
			m_frameGlobals.update(m_context, delta);
			m_frameGlobals.bind(m_context);

			for (auto& it : m_renderList.m_dynamicObjects)
			{
				it->render(delta);
			}
		}
	}

	void Renderer::cullObjects()
	{
		cleanRenderLists();
		m_scene->cullGraphicsObjects(m_camera->getFrustum(), m_renderList);
	}

	void Renderer::cleanRenderLists()
	{
		m_renderList.clear();
	}

}