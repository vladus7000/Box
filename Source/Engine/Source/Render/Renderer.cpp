#include "StdAfx.hpp"
#include "Render\Renderer.hpp"
#include "System\ResourceSystem\ResourceManager.hpp"
#include "Render\ResourceLoaders\DDSTextureResourceLoader.hpp"
#include "Render\ResourceLoaders\ShaderResourceLoader.hpp"
#include <Render/ResourceLoaders/SdkmeshResourceLoader.hpp>
#include <Render/ResourceLoaders/ModelResourceLoader.hpp>
#include <Render/ResourceLoaders/MaterialResourceLoader.hpp>
#include "Window\Window.hpp"
#include "Render\Mesh.hpp"

#include <DXUT11\Core\DXUT.h>
#include "Render\DXUTHelper.hpp"
#include "DXUT11\Optional\DXUTgui.h"
#include "Render/GraphicsNode.hpp"

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
		reinterpret_cast<Renderer*>(pUserContext)->onD3D11CreateDevice(pd3dDevice, pBackBufferSurfaceDesc);
		return S_OK;
	}

	void CALLBACK OnD3D11ReleasingSwapChain(void* pUserContext)
	{
	}

	void CALLBACK OnD3D11DestroyDevice(void* pUserContext)
	{
	}

	bool CALLBACK ModifyDeviceSettings(DXUTDeviceSettings* pDeviceSettings, void* pUserContext)
	{
		reinterpret_cast<Renderer*>(pUserContext)->onModifyDeviceSettings(pDeviceSettings);
		return true;
	}

	HRESULT CALLBACK OnD3D11ResizedSwapChain(ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain,
		const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext)
	{
		reinterpret_cast<Renderer*>(pUserContext)->onD3D11ResizedSwapChain(pd3dDevice, pSwapChain, pBackBufferSurfaceDesc);
		return S_OK;
	}

	bool Renderer::init()
	{
		bool result = true;

		DXUTSetCallbackDeviceChanging(ModifyDeviceSettings, this);
		DXUTSetCallbackD3D11DeviceAcceptable(IsD3D11DeviceAcceptable, this);
		DXUTSetCallbackD3D11DeviceCreated(OnD3D11CreateDevice, this);

		DXUTSetCallbackD3D11SwapChainReleasing(OnD3D11ReleasingSwapChain, this);
		DXUTSetCallbackD3D11DeviceDestroyed(OnD3D11DestroyDevice, this);
		DXUTSetCallbackD3D11SwapChainResized(OnD3D11ResizedSwapChain, this);
		DXUTCreateDevice(D3D_FEATURE_LEVEL_11_0, true, Window::Instance().getWidth(), Window::Instance().getHeight());

		m_context = DXUTGetD3D11DeviceContext();
		m_device = DXUTGetD3D11Device();

		ResourceManager::Instance().registerLoader(std::make_shared<DDSTextureResourceLoader>());
		ResourceManager::Instance().registerLoader(std::make_shared<ShaderResourceLoader>());
		ResourceManager::Instance().registerLoader(std::make_shared<SdkmeshResourceLoader>());
		ResourceManager::Instance().registerLoader(std::make_shared<ModelResourceLoader>());
		ResourceManager::Instance().registerLoader(std::make_shared<MaterialResourceLoader>());

		DXUT::Restore(m_device, m_context);

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
			m_frameGlobals.updateEnvironmentSettings(m_scene->getEnvironmentSettings());
			m_frameGlobals.update(m_context, delta);
			m_frameGlobals.bind(m_context);

			genericRenderGraphicsNodes(m_renderList.m_dynamicObjects, delta);
			genericRenderGraphicsNodes(m_renderList.m_editorObjects, delta);
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

	void Renderer::genericRenderGraphicsNodes(const std::vector<GraphicsNode*>& nodes, F32 delta)
	{
		for (auto& it : nodes)
		{
			for (int i = 0; i < it->getMeshesCount(); i++)
			{
				auto mesh = it->getMeshAt(i).lock();
				if (auto material = mesh->getMaterial().lock())
				{
					auto shader = material->getShader();
					auto provider = shader->getEnvProvider();

					provider->prepareShader(m_context, *shader, *material, *it);
					material->apply(m_context);
					mesh->render(delta);
				}
			}
		}
	}

	void Renderer::onD3D11ResizedSwapChain(ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc)
	{
		m_swapChain = pSwapChain;
		m_backBufferSurfaceDesc = *pBackBufferSurfaceDesc;
		if (box::DXUT::Inited())
		{
			auto& dialogResourceManager = box::DXUT::GetDialogResourceManager();
			dialogResourceManager.OnD3D11ResizedSwapChain(pd3dDevice, &m_backBufferSurfaceDesc);
		}
	}

}