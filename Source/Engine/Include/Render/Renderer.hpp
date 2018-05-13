#pragma once
#include "System\Patterns\Singleton.hpp"
#include "Scene\Scene.hpp"
#include "Camera.hpp"
#include "FrameGlobals.hpp"
#include "RenderList.hpp"
#include "Texture.hpp"
#include "Shader.hpp"

struct DXUTDeviceSettings;

namespace box
{
	class Renderer
	{
		SINGLETON(Renderer);
	public:

		enum ClearFlags
		{
			CF_CLEAR_COLOR = 1,
			CF_CLEAR_DEPTH = 2,
			CF_CLEAR_STENCIL = 4
		};

		bool init();
		void deinit();

		void setScene(Scene::SceneStrongPtr scene) { m_scene = scene; };
		Scene::SceneWeakPtr getScene() const { return m_scene; }

		void setCamera(Camera::CameraStrongPtr camera) { m_camera = camera; }
		Camera::CameraWeakPtr getCamera() const { return m_camera; }

		void renderScene(F32 delta);
		void cullObjects();

		//////////Render specific
		void clear(U32 flags, Texture* rtv = nullptr, F32 color[4] = nullptr, Texture* dsv = nullptr, F32 depth = 1.0f, U8 stencil = 0);
		void setRenderTargets(U32 count = 0, Texture** rtv = nullptr, Texture* dsv = nullptr);

		Texture* getBackBuffer() { return m_backBuffer.get(); }

		ID3D11DeviceContext* getDirectXContext() const { return m_context; }
		ID3D11Device* getDirectXDevice() const { return m_device; }

	private:
		void cleanRenderLists();
		void genericRenderGraphicsNodes(const std::vector<GraphicsNode*>& nodes, F32 delta);
		void drawQuad(U32 w, U32 h);

		void renderPostEffects();
		void tonemapPass();
		void lightingPass(F32 delta);
		void skyboxPass();
		void recreateRenderTargets(U32 w, U32 h);
		void recreateSkybox(U32 w, U32 h);
	public:
		void onModifyDeviceSettings(DXUTDeviceSettings* pDeviceSettings) { m_deviceSettings = pDeviceSettings; }
		void onD3D11CreateDevice(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc) { m_device = pd3dDevice; }
		void onD3D11ResizedSwapChain(ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc);

	private:
		Scene::SceneStrongPtr m_scene;
		Camera::CameraStrongPtr m_camera;
		RenderObjects m_renderList;
		ID3D11DeviceContext* m_context;
		ID3D11Device* m_device;
		IDXGISwapChain* m_swapChain;
		FrameGlobals m_frameGlobals;
		DXUTDeviceSettings* m_deviceSettings;
		DXGI_SURFACE_DESC m_backBufferSurfaceDesc;
		Texture::TextureStrongPtr m_backBuffer;
		Texture::TextureStrongPtr m_hdrOffscreenBuffer;
		Texture::TextureStrongPtr m_ldrOffscreenBuffer;
		Shader::ShaderStrongPtr m_tonemapShader;
		Shader::ShaderStrongPtr m_postProcessShader;
		std::vector<Texture::TextureStrongPtr> m_lumChain;
		U32 m_BBwidth;
		U32 m_BBHeight;
		///// test for skybox
		Shader::ShaderStrongPtr m_skyboxShader;
		ID3D11Buffer* m_skyboxBuffer = nullptr;
		Texture::TextureStrongPtr m_skyboxTexture;
	};
}