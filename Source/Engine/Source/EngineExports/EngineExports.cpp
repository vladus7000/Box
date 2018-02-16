#include "StdAfx.hpp"
#include "System/Engine.hpp"
#include "System/Input.hpp"
#include "Render/Renderer.hpp"
#include "Gameplay\GameView.hpp"
#include "Scene\Scene.hpp"
#include "System\ResourceSystem\ResourceEvents.hpp"
#include "System\EventSystem\EventSystem.hpp"

#include <DXUT11\Core\DXUT.h>
#include "Render\DXUTHelper.hpp"
#include "DXUT11\Optional\SDKmisc.h"

using namespace box;
namespace
{
	class EditorView : public box::GameView
	{
	public:
		virtual ~EditorView() = default;

		virtual bool restore() override
		{
			m_renderer = &Renderer::Instance();
			m_scene = std::make_shared<Scene>();
			m_camera = std::make_shared<Camera>();
			m_modelsLoaded = 0;

			m_renderer->setScene(m_scene);
			m_renderer->setCamera(m_camera);

			m_delegate = fastdelegate::MakeDelegate(this, &EditorView::resourceLoaded);
			EventSystem::Instance().add(m_delegate, Event_ResourceLoaded::Type);

			return true;
		}

		virtual void deinit() override
		{
			EventSystem::Instance().remove(m_delegate, Event_ResourceLoaded::Type);
		}

		virtual void deviceLost() override
		{
		}

		virtual S32 render(F64 time, F32 delta) override
		{
			ID3D11DeviceContext* context = DXUTGetD3D11DeviceContext();
			ID3D11Device* device = DXUTGetD3D11Device();

			float ClearColor[4] = { 0.0f, 0.25f, 0.25f, 0.55f };
			ID3D11RenderTargetView* pRTV = DXUTGetD3D11RenderTargetView();
			context->ClearRenderTargetView(pRTV, ClearColor);

			ID3D11DepthStencilView* pDSV = DXUTGetD3D11DepthStencilView();
			context->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH, 1.0, 0);

			m_renderer->renderScene(delta);

			auto& txtHelper = DXUT::GetTextHelper();
			txtHelper.Begin();
			txtHelper.SetInsertionPos(2, 0);
			txtHelper.SetForegroundColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
			txtHelper.DrawTextLine(DXUTGetFrameStats(DXUTIsVsyncEnabled()));
			txtHelper.DrawTextLine(DXUTGetDeviceStats());
			txtHelper.SetForegroundColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			wchar_t buf[50];
			wsprintf(buf, L"Objects loaded: %d", m_modelsLoaded);
			txtHelper.DrawTextLine(buf);
			txtHelper.End();
			return 0;
		}

		virtual void update(F64 fTime, F32 fElapsedTime) override
		{

		}

		virtual AppMsg::Status msgProc(const AppMsg& msg) override
		{
			return AppMsg::Status::DefaultAction;
		}
	private:
		void resourceLoaded(EventSystem::StrongEventDataPtr resource)
		{
			std::shared_ptr<Event_ResourceLoaded> loadedResource(std::static_pointer_cast<Event_ResourceLoaded>(resource));
			if (loadedResource->getType() == Event_ResourceLoaded::ResourceType::Model)
			{
				m_modelsLoaded++;
			}
		}
	private:
		Renderer* m_renderer;
		Scene::SceneStrongPtr m_scene;
		Camera::CameraStrongPtr m_camera;
		EventSystem::DelegateType m_delegate;
		U32 m_modelsLoaded;
	};
}
namespace Exports
{
	using namespace box;
	Engine* g_engineInstance = nullptr;

namespace System
{
	int StartEngine(void* hwnd, int argc, char** argv)
	{
		if (g_engineInstance)
		{
			return -1;
		}

		std::shared_ptr<EditorView> editor(new EditorView());

		g_engineInstance = new box::Engine();
		g_engineInstance->startup(hwnd, argc, argv);
		g_engineInstance->attachGameView(editor);
		return 0;
	}

	int StopEngine()
	{
		if (!g_engineInstance)
		{
			return -1;
		}

		g_engineInstance->shutdown();
		delete g_engineInstance;
		g_engineInstance = nullptr;

		return 0;
	}

	int WndProc(void* hwnd, int msg, int wParam, int lParam)
	{
		if (!g_engineInstance)
		{
			return -1;
		}

		DXUTStaticWndProc(static_cast<HWND>(hwnd), msg, wParam, lParam);
		return 0;
	}

	int Update(box::F32 delta)
	{
		if (!g_engineInstance)
		{
			return -1;
		}
	}

	int RenderFrame(box::F32 delta)
	{
		if (!g_engineInstance)
		{
			return -1;
		}

		DXUTRender3DEnvironment();
		return 0;
	}
}//System

namespace Input
{
	int OnKeyDown(U32 key)
	{
		if (g_engineInstance)
		{
			box::Input::Instance().onKeyDown(key);
			return 0;
		}

		return -1;
	}

	int OnKeyUp(U32 key)
	{
		if (g_engineInstance)
		{
			box::Input::Instance().onKeyUp(key);
			return 0;
		}

		return -1;
	}

	int OnMouseMove(U32 x, U32 y)
	{
		if (g_engineInstance)
		{
			box::Input::Instance().onMouseMove(x, y);
			return 0;
		}

		return -1;
	}

	int OnMouseButtonDown(U32 x, U32 y, U32 key)
	{
		if (g_engineInstance)
		{
			box::Input::Instance().onMouseButtonDown(x, y, key);
			return 0;
		}

		return -1;
	}

	int OnMousebuttonUp(U32 x, U32 y, U32 key)
	{
		if (g_engineInstance)
		{
			box::Input::Instance().onMousebuttonUp(x, y, key);
			return 0;
		}

		return -1;
	}
} // Input
}