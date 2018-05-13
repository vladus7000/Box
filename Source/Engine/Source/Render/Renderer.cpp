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
#include <algorithm>
#undef max
#undef min

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

		memset(&m_backBufferSurfaceDesc, 0, sizeof(m_backBufferSurfaceDesc));

		DXUTSetIsInGammaCorrectMode(false);
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

		{
			Resource r("desc/post/tonemap.shader");
			auto handle = ResourceManager::Instance().getHandle(r);
			m_tonemapShader = handle->getExtraTyped<Shader>();
		}
		{
			Resource r("desc/post/postprocess.shader");
			auto handle = ResourceManager::Instance().getHandle(r);
			m_postProcessShader = handle->getExtraTyped<Shader>();
		}
		{
			Resource r("desc/skybox.shader");
			auto handle = ResourceManager::Instance().getHandle(r);
			m_skyboxShader = handle->getExtraTyped<Shader>();
		}
		{
			Resource r("cubemaps/galileo_cross.dds");
			auto handle = ResourceManager::Instance().getHandle(r);
			m_skyboxTexture = handle->getExtraTyped<Texture>();
		}
		return result;
	}

	void Renderer::deinit()
	{
		DXUT::Deinit();

		if (m_skyboxBuffer)
		{
			m_skyboxBuffer->Release();
		}
	}

	void Renderer::renderScene(F32 delta)
	{
		if (!m_scene)
		{
			return;
		}

		m_frameGlobals.updateEnvironmentSettings(m_scene->getEnvironmentSettings());
		m_frameGlobals.update(m_context, delta);
		m_frameGlobals.bind(m_context);

		ID3D11ShaderResourceView* srvs[8] = { m_skyboxTexture->getSRV_Raw() };
		m_context->PSSetShaderResources(0, 1, srvs);

		ID3D11SamplerState* samplers[1] = { m_skyboxTexture->getSamplerState_Raw() };
		m_context->PSSetSamplers(0, 1, samplers);

		lightingPass(delta);
		skyboxPass();
		tonemapPass();
		renderPostEffects();
	}

	void Renderer::lightingPass(F32 delta)
	{
		float color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		clear(ClearFlags::CF_CLEAR_COLOR, m_hdrOffscreenBuffer.get(), color);
		Texture* arr[1] = { m_hdrOffscreenBuffer.get() };
		setRenderTargets(1, arr, m_backBuffer.get());

		genericRenderGraphicsNodes(m_renderList.m_dynamicObjects, delta);
		genericRenderGraphicsNodes(m_renderList.m_editorObjects, delta);
	}

	void Renderer::tonemapPass()
	{
		float color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		clear(ClearFlags::CF_CLEAR_COLOR, m_ldrOffscreenBuffer.get(), color);

		/////////// avg lum
		Texture* renderTargets[1] = { m_lumChain[0].get() };
		setRenderTargets(1, renderTargets);

		//input
		ID3D11ShaderResourceView* srvs[8] = { m_hdrOffscreenBuffer->getSRV_Raw() };
		m_context->PSSetShaderResources(0, 1, srvs);
		ID3D11SamplerState* samplers[1] = { m_hdrOffscreenBuffer->getSamplerState_Raw() };
		m_context->PSSetSamplers(0, 1, samplers);

		//shader
		m_tonemapShader->setActiveTechnique(0);
		auto& technique = m_tonemapShader->getActiveTechnique();
		technique.apply(m_context);

		drawQuad(m_BBwidth, m_BBHeight);

		/////////// downsample
		for (U32 i = 1; i < m_lumChain.size(); i++)
		{
			auto rt = m_lumChain[i].get();
			auto srv = m_lumChain[i - 1].get();

			//rt
			renderTargets[0] = { rt };
			setRenderTargets(1, renderTargets);

			//input
			ID3D11ShaderResourceView* srvs[8] = { srv->getSRV_Raw() };
			m_context->PSSetShaderResources(0, 1, srvs);
			ID3D11SamplerState* samplers[1] = { srv->getSamplerState_Raw() };
			m_context->PSSetSamplers(0, 1, samplers);

			//shader
			m_tonemapShader->setActiveTechnique(1);
			auto& technique = m_tonemapShader->getActiveTechnique();
			technique.apply(m_context);

			drawQuad(srv->getWidth() / 2, srv->getHeight() / 2);

			srvs[0] = nullptr;
			m_context->PSSetShaderResources(0, 1, srvs);
		}

		////////// tonemap
		//input
		{
			Texture* renderTargets[1] = { m_ldrOffscreenBuffer.get() };
			setRenderTargets(1, renderTargets);

			auto lum = m_lumChain.back().get();
			ID3D11ShaderResourceView* srvs[8] = { m_hdrOffscreenBuffer->getSRV_Raw(), lum->getSRV_Raw() };
			m_context->PSSetShaderResources(0, 2, srvs);
			ID3D11SamplerState* samplers[2] = { m_hdrOffscreenBuffer->getSamplerState_Raw(), lum->getSamplerState_Raw() };
			m_context->PSSetSamplers(0, 2, samplers);

			//shader
			m_tonemapShader->setActiveTechnique(2);
			auto& technique = m_tonemapShader->getActiveTechnique();
			technique.apply(m_context);

			drawQuad(m_BBwidth, m_BBHeight);
			srvs[0] = nullptr;
			srvs[1] = nullptr;
			m_context->PSSetShaderResources(0, 2, srvs);
		}
	}

	void Renderer::renderPostEffects()
	{
		Texture* renderTargets[1] = { m_backBuffer.get() };
		setRenderTargets(1, renderTargets, m_backBuffer.get());

		ID3D11ShaderResourceView* srvs[8] = { m_ldrOffscreenBuffer->getSRV_Raw() };
		m_context->PSSetShaderResources(0, 1, srvs);
		
		ID3D11SamplerState* samplers[1] = { m_ldrOffscreenBuffer->getSamplerState_Raw() };
		m_context->PSSetSamplers(0, 1, samplers);

		m_postProcessShader->setActiveTechnique(0);
		auto& technique = m_postProcessShader->getActiveTechnique();
		technique.apply(m_context);

		drawQuad(m_BBwidth, m_BBHeight);

		srvs[0] = nullptr;
		m_context->PSSetShaderResources(0, 1, srvs);
	}

	void Renderer::skyboxPass()
	{
		struct SKYBOX_VERTEX
		{
			D3DXVECTOR4 pos;
		};

		m_skyboxShader->setActiveTechnique(0);
		auto& technique = m_skyboxShader->getActiveTechnique();
		technique.apply(m_context);
		HRESULT hr;

		UINT uStrides = sizeof(SKYBOX_VERTEX);
		UINT uOffsets = 0;
		ID3D11Buffer* pBuffers[1] = { m_skyboxBuffer };
		m_context->IASetVertexBuffers(0, 1, pBuffers, &uStrides, &uOffsets);
		m_context->IASetIndexBuffer(NULL, DXGI_FORMAT_R32_UINT, 0);
		m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		m_context->Draw(4, 0);
	}

	void Renderer::cullObjects()
	{
		cleanRenderLists();
		m_scene->cullGraphicsObjects(m_camera->getFrustum(), m_renderList);
	}

	void Renderer::clear(U32 flags, Texture* rtv, F32 color[4], Texture* dsv, F32 depth, U8 stencil)
	{
		const bool clearColor = (flags & ClearFlags::CF_CLEAR_COLOR) != 0;
		const bool clearDepth = (flags & ClearFlags::CF_CLEAR_DEPTH) != 0;
		const bool clearStencil = (flags & ClearFlags::CF_CLEAR_STENCIL) != 0;

		if (clearColor)
		{
			ASSERT(rtv);
			ASSERT(rtv->isCube() == false);
			m_context->ClearRenderTargetView(rtv->getRTV_Raw(), color);
		}
		if (clearDepth && clearStencil)
		{
			ASSERT(dsv);
			ASSERT(dsv->isCube() == false);
			ID3D11DepthStencilView* pDSV = DXUTGetD3D11DepthStencilView();

			m_context->ClearDepthStencilView(dsv->getDSV_Raw(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, depth, stencil);
		}
		else if (clearDepth || clearStencil)
		{
			ASSERT(dsv);
			ASSERT(dsv->isCube() == false);
			if (clearDepth)
			{
				m_context->ClearDepthStencilView(dsv->getDSV_Raw(), D3D11_CLEAR_DEPTH, depth, stencil);
			}
			else
			{
				m_context->ClearDepthStencilView(dsv->getDSV_Raw(), D3D11_CLEAR_STENCIL, depth, stencil);
			}
		}
	}

	void Renderer::setRenderTargets(U32 count, Texture** rtv, Texture* dsv)
	{
		const U32 maxRTV = 8;
		ID3D11RenderTargetView* rtvs[maxRTV] = { nullptr };
		count = std::min(count, maxRTV);
		for (U32 i = 0; i < count; i++)
		{
			rtvs[i] = rtv[i]->getRTV_Raw();
		}
		m_context->OMSetRenderTargets(count, rtvs, dsv ? dsv->getDSV_Raw() : nullptr);
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
	void Renderer::drawQuad(U32 w, U32 h)
	{
		D3D11_VIEWPORT vpOld[D3D11_VIEWPORT_AND_SCISSORRECT_MAX_INDEX];
		UINT nViewPorts = 1;
		m_context->RSGetViewports(&nViewPorts, vpOld);

		D3D11_VIEWPORT vp;
		vp.Width = (float)w;
		vp.Height = (float)h;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		m_context->RSSetViewports(1, &vp);

		m_context->IASetInputLayout(nullptr);
		m_context->IASetVertexBuffers(0, 0, nullptr, 0, 0);
		m_context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		m_context->IASetIndexBuffer(nullptr, DXGI_FORMAT_R16_UINT, 0);
		m_context->Draw(4, 0);

		m_context->RSSetViewports(nViewPorts, vpOld);
	}

	U32 getPrevPow2(U32 v)
	{
		v--;
		v |= v >> 1;
		v |= v >> 2;
		v |= v >> 4;
		v |= v >> 8;
		v |= v >> 16;
		v++;
		v /= 2;
		return v;
	}

	void Renderer::recreateRenderTargets(U32 w, U32 h)
	{
		{
			m_hdrOffscreenBuffer = std::make_shared<Texture>();
			m_hdrOffscreenBuffer->setName("hdrOffscreenBuffer");
			const bool created = m_hdrOffscreenBuffer->createSurface(m_device, m_BBwidth, m_BBHeight, 1, 1, 1, DXGI_FORMAT_R32G32B32A32_TYPELESS, DXGI_FORMAT_R32G32B32A32_FLOAT, DXGI_FORMAT_R32G32B32A32_FLOAT);
			m_hdrOffscreenBuffer->initDefaultSampler(m_device);
			ASSERT(created);
		}

		{
			m_ldrOffscreenBuffer = std::make_shared<Texture>();
			m_ldrOffscreenBuffer->setName("ldrOffscreenBuffer");
			const bool created = m_ldrOffscreenBuffer->createSurface(m_device, m_BBwidth, m_BBHeight, 1, 1, 1, DXGI_FORMAT_R8G8B8A8_TYPELESS, DXGI_FORMAT_R8G8B8A8_SNORM, DXGI_FORMAT_R8G8B8A8_SNORM);
			m_ldrOffscreenBuffer->initDefaultSampler(m_device);
			ASSERT(created);
		}

		m_lumChain.clear();

		U32 size = getPrevPow2(std::min(w, h));

		while (true)
		{
			m_lumChain.push_back(std::make_shared<Texture>());
			auto& tex = m_lumChain.back();
			const bool created = tex->createSurface(m_device, size, size, 1, 1, 1, DXGI_FORMAT_R32_TYPELESS, DXGI_FORMAT_R32_FLOAT, DXGI_FORMAT_R32_FLOAT);
			tex->initDefaultSampler(m_device);
			ASSERT(created);

			if (size == 1)
			{
				break;
			}
			size /= 2;
		}

		recreateSkybox(w, h);
	}

	void Renderer::recreateSkybox(U32 w, U32 h)
	{
		/// for skybox
		if (m_skyboxBuffer)
		{
			m_skyboxBuffer->Release();
		}
		struct SKYBOX_VERTEX
		{
			D3DXVECTOR4 pos;
		};

		SKYBOX_VERTEX pVertex[4];
		// Map texels to pixels 
		float fHighW = -1.0f - (1.0f / (float)w);
		float fHighH = -1.0f - (1.0f / (float)h);
		float fLowW = 1.0f + (1.0f / (float)w);
		float fLowH = 1.0f + (1.0f / (float)h);

		pVertex[0].pos = D3DXVECTOR4(fLowW, fLowH, 1.0f, 1.0f);
		pVertex[1].pos = D3DXVECTOR4(fLowW, fHighH, 1.0f, 1.0f);
		pVertex[2].pos = D3DXVECTOR4(fHighW, fLowH, 1.0f, 1.0f);
		pVertex[3].pos = D3DXVECTOR4(fHighW, fHighH, 1.0f, 1.0f);

		UINT uiVertBufSize = 4 * sizeof(SKYBOX_VERTEX);
		//Vertex Buffer
		D3D11_BUFFER_DESC vbdesc;
		vbdesc.ByteWidth = uiVertBufSize;
		vbdesc.Usage = D3D11_USAGE_IMMUTABLE;
		vbdesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbdesc.CPUAccessFlags = 0;
		vbdesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = pVertex;
		m_device->CreateBuffer(&vbdesc, &InitData, &m_skyboxBuffer);
	}

	bool operator==(const DXGI_SURFACE_DESC& lhs, const DXGI_SURFACE_DESC& rhs)
	{
		return lhs.Width == rhs.Width
			&& lhs.Height == rhs.Height
			&& lhs.Format == rhs.Format
			&& lhs.SampleDesc.Count == rhs.SampleDesc.Count
			&& lhs.SampleDesc.Quality == rhs.SampleDesc.Quality;
	}

	bool operator!=(const DXGI_SURFACE_DESC& lhs, const DXGI_SURFACE_DESC& rhs)
	{
		return !(lhs == rhs);
	}

	void Renderer::onD3D11ResizedSwapChain(ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc)
	{
		m_swapChain = pSwapChain;
		m_device = pd3dDevice;
		if (box::DXUT::Inited())
		{
			auto rtv = DXUTGetD3D11RenderTargetView();
			auto dsv = DXUTGetD3D11DepthStencilView();
			
			m_backBufferSurfaceDesc = *pBackBufferSurfaceDesc;
			m_BBwidth = m_backBufferSurfaceDesc.Width;
			m_BBHeight = m_backBufferSurfaceDesc.Height;
			m_backBuffer = std::make_shared<Texture>(m_backBufferSurfaceDesc.Width, m_backBufferSurfaceDesc.Height, nullptr, rtv, true, dsv, true);
			m_backBuffer->setName("backBuffer");
			recreateRenderTargets(m_BBwidth, m_BBHeight);

			auto& dialogResourceManager = box::DXUT::GetDialogResourceManager();
			dialogResourceManager.OnD3D11ResizedSwapChain(pd3dDevice, &m_backBufferSurfaceDesc);
		}
	}
}