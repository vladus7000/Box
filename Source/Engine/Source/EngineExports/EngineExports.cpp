#include "StdAfx.hpp"
#ifdef EDITOR_BUILD
#include "EngineExports/EngineExports.hpp"
#include "System/Engine.hpp"
#include "System/Input.hpp"
#include "Render/Renderer.hpp"
#include "Render/Camera.hpp"
#include "Gameplay\GameView.hpp"
#include "Gameplay/ActorEvents.hpp"
#include "Scene\Scene.hpp"
#include "System\ResourceSystem\ResourceEvents.hpp"
#include "System\ResourceSystem\ResourceManager.hpp"
#include "System\EventSystem\EventSystem.hpp"
#include "Window\Window.hpp"

#include <DXUT11\Core\DXUT.h>
#include "Render\DXUTHelper.hpp"
#include "Render\GraphicsNode.hpp"
#include "Render\GridNode.hpp"
#include "DXUT11\Optional\SDKmisc.h"
#include "Gameplay/Actor.hpp"
#include "Gameplay/Components/GraphicsComponent.hpp"
#include "Gameplay/Components/TransformComponent.hpp"
#include "Gameplay/ActorComponentFactory.hpp"

#include <stdio.h>

using namespace box;
namespace
{
	template <class T, class U >
	std::shared_ptr<T> castTo(std::shared_ptr<U> in)
	{
		return std::static_pointer_cast<T>(in);
	}
	class EditorView;

	EditorView* g_editor = nullptr;

	class EditorView : public box::GameView , public box::KeyboardHandler, public box::MouseHandler
	{
	public:
		enum class ViewMode
		{
			Level,
			Preview
		};
		virtual ~EditorView() = default;

		void setCameraFov(float fov) { m_camera->setFov(fov); }
		void setCameraZNear(float zNear) { m_camera->setZNear(zNear); }
		void setCameraZFar(float zFar) { m_camera->setZFar(zFar); }
		void setCameraMovementSpeed(float speed) { m_cameraMovementSpeed = speed; }
		void setRenderViewActive(bool active) { m_renderViewActive = active; }
		void setViewMode(ViewMode mode) { m_activeViewMode = mode; }

		void clearPreviewModel() { m_clearPreviewModel = true; }

		void SaveLevelToXMLFile(const char* filename)
		{
			tinyxml2::XMLDocument xmlDoc;
			tinyxml2::XMLNode* actors = m_scene->serializeToXML(nullptr, xmlDoc);
			tinyxml2::XMLNode* scene = m_rootActor->serializeToXML(nullptr, xmlDoc);
			xmlDoc.InsertFirstChild(scene);
			xmlDoc.InsertEndChild(actors);

			FILE* pFile;
			pFile = fopen(filename, "w");

			tinyxml2::XMLPrinter printer(pFile, false);

			xmlDoc.Print(&printer);
			
			fclose(pFile);
		}

		void loadLevelFromXMLFile(const char* filename)
		{
			tinyxml2::XMLDocument xmlDoc;
			xmlDoc.LoadFile(filename);
			tinyxml2::XMLNode* scene = xmlDoc.FirstChildElement("Scene");
			tinyxml2::XMLNode* rootActor = xmlDoc.FirstChildElement("ActorBase");
			m_rootActor = ActorComponentFactory::Instance().createActor("ActorBase");
			m_actorsMap.clear();
			m_scene = std::make_shared<Scene>();
			m_scene->loadFromXML(scene, xmlDoc);
			m_scene->getRoot().lock()->addChild(m_grid);
			m_rootActor->setScene(m_scene);
			m_rootActor->loadFromXML(rootActor);
		}

		void addPreviewModelToCollection(const char* modelName, const char* descrFileName, const char* srcFileName)
		{
			FILE* pFile;
			tinyxml2::XMLDocument xmlDoc2;
			pFile = fopen(descrFileName, "w");
			m_previewModel->setSourceFile(srcFileName);
			m_previewModel->setName(modelName);
			tinyxml2::XMLNode* pRoot = m_previewModel->serializeToXML(nullptr, xmlDoc2);
			xmlDoc2.InsertFirstChild(pRoot);
			tinyxml2::XMLPrinter printer2(pFile, false);
			xmlDoc2.Print(&printer2);
			fclose(pFile);
		}

		void addModelToScene(const char* desc)
		{
			Resource r(desc);

			auto handle = box::ResourceManager::Instance().getHandle(r);
			if (!handle || !handle->getExtra())
			{
				return;
			}
			m_previewModel = handle->getExtraTyped<Model>();

			auto newActor = ActorComponentFactory::Instance().createActor("ActorBase");

			auto transformComponent = std::make_shared<TransformComponent>();
			transformComponent->setOwner(newActor);
			newActor->addComponent(transformComponent);

			{
				auto graphicsComponent = std::make_shared<GraphicsComponent>();
				graphicsComponent->setOwner(newActor);

				newActor->addComponent(graphicsComponent);
				m_scene->getRoot().lock()->addChild(graphicsComponent->m_graphicsNode);
				graphicsComponent->m_graphicsNode->setModel(m_previewModel);
				graphicsComponent->m_graphicsNode->setTransform(transformComponent.get());
				//m_previewModel->setTransform(transformComponent.get());
				m_rootActor->addChild(newActor);
			}

			/*if (auto graphicsComponent = m_rootActor->getComponent<GraphicsComponent>(GraphicsComponent::ComponentID).lock())
			{
				auto transform = m_rootActor->getComponent<TransformComponent>(TransformComponent::ComponentID).lock();
				graphicsComponent->m_graphicsNode->setModel(m_previewModel);
				m_previewModel->setTransform(transform.get());
			}*/
		}

		void updateEnvironmentSettings(const char* xml)
		{
			m_envSettings.Clear();
			size_t size = strlen(xml);
			tinyxml2::XMLError result = m_envSettings.Parse(xml, size);
			if (result == tinyxml2::XMLError::XML_SUCCESS)
			{
				tinyxml2::XMLElement* root = m_envSettings.RootElement();
				m_scene->updateEnvironmentSettings(root);
				m_previewScene->updateEnvironmentSettings(root);
			}
		}

		U32 getActorsXml(char* out, U32 buffMaxCapacity)
		{
			tinyxml2::XMLDocument xmlDoc;
			tinyxml2::XMLNode* pRoot = m_rootActor->serializeToXML(nullptr, xmlDoc, true);
			xmlDoc.InsertFirstChild(pRoot);

			tinyxml2::XMLPrinter printer(nullptr, true);

			xmlDoc.Print(&printer);

			if (buffMaxCapacity >= printer.CStrSize())
			{
				memcpy(out, printer.CStr(), printer.CStrSize());
				return 0;
			}
			return printer.CStrSize() - buffMaxCapacity;
		}

		int getActorInfo(unsigned int actorID, char* out, unsigned int buffMaxCapacity)
		{
			auto foundIt = m_actorsMap.find(actorID);
			if (foundIt != m_actorsMap.end())
			{
				tinyxml2::XMLDocument xmlDoc;

				if (auto actor = foundIt->second.lock())
				{
					auto elem = actor->serializeToXML(nullptr, xmlDoc, true);
					xmlDoc.InsertFirstChild(elem);
				}

				tinyxml2::XMLPrinter printer(nullptr);

				xmlDoc.Print(&printer);

				if (buffMaxCapacity >= printer.CStrSize())
				{
					memcpy(out, printer.CStr(), printer.CStrSize());
					return 0;
				}
				return printer.CStrSize() - buffMaxCapacity;
			}

			return -1;
		}

		int updateActor(unsigned int actorID, const char* xml)
		{
			auto foundIt = m_actorsMap.find(actorID);
			if (foundIt != m_actorsMap.end())
			{
				const size_t size = strlen(xml);
				tinyxml2::XMLDocument doc;
				tinyxml2::XMLError result = doc.Parse(xml, size);
				if (result == tinyxml2::XMLError::XML_SUCCESS)
				{
					tinyxml2::XMLElement* root = doc.RootElement();

					if (auto actor = foundIt->second.lock())
					{
						actor->updateFromXml(root);
					}
				}
			}
			return -1;
		}

		virtual void keyState(const KeyState state[256]) override
		{
			if (!m_renderViewActive)
			{
				return;
			}
			if (state[static_cast<U32>(box::ButtonCodes::Key_W)].isPressed())
			{
				m_camera->moveForward(m_cameraMovementSpeed);
			}

			if (state[static_cast<U32>(box::ButtonCodes::Key_S)].isPressed())
			{
				m_camera->moveBackward(m_cameraMovementSpeed);
			}

			if (state[static_cast<U32>(box::ButtonCodes::Key_A)].isPressed())
			{
				m_camera->moveLeft(m_cameraMovementSpeed);
			}

			if (state[static_cast<U32>(box::ButtonCodes::Key_D)].isPressed())
			{
				m_camera->moveRight(m_cameraMovementSpeed);
			}
		}

		virtual void onKeyDown(U32 key) override
		{
		}
		virtual void onKeyUp(U32 key) override
		{
		}

		virtual void onMouseMove(U32 x, U32 y) override
		{
			const S32 dx = m_prevMouseX - x;
			const S32 dy = m_prevMouseY - y;

			if (m_mouseButtonPressed && (dx != 0 || dy != 0))
			{
				m_camera->yaw(dx * m_cameraMouseSense);
				m_camera->pitch(dy * m_cameraMouseSense);
			}
			m_prevMouseX = x;
			m_prevMouseY = y;
		}
		virtual void onMouseButtonDown(U32 x, U32 y, U32 key) override
		{
			m_mouseButtonPressed = true;
		}
		virtual void onMousebuttonUp(U32 x, U32 y, U32 key) override
		{
			m_mouseButtonPressed = false;
		}

		virtual bool restore() override
		{
			m_cameraMovementSpeed = 0.03f;
			m_cameraMouseSense = 0.01f;
			m_renderViewActive = true;
			m_mouseButtonPressed = false;
			m_clearPreviewModel = false;
			m_prevMouseX = 0;
			m_prevMouseY = 0;
			g_editor = this;
			m_activeViewMode = ViewMode::Level;
			m_renderer = &Renderer::Instance();
			m_scene = std::make_shared<Scene>();
			m_previewScene = std::make_shared<Scene>();
			m_camera = std::make_shared<Camera>();

			const F32 aspect = (F32)Window::Instance().getWidth() / (F32)Window::Instance().getHeight();
			const F32 fov = 1.0472f;
			m_camera->initialize(0.5f, 500.0f, aspect, fov, Vector3D(0.0f, 1.0f, -50.0f), Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 1.0f, 0.0f));

			m_renderer->setScene(m_scene);
			m_renderer->setCamera(m_camera);

			m_delegate = fastdelegate::MakeDelegate(this, &EditorView::resourceLoaded);
			m_actorAddedDelegate = fastdelegate::MakeDelegate(this, &EditorView::actorAdded);
			EventSystem::Instance().add(m_delegate, Event_ResourceLoaded::Type);
			EventSystem::Instance().add(m_actorAddedDelegate, Event_ActorAdded::Type);
			Input::Instance().registerKeyboardHandler(this);
			Input::Instance().registerMouseHandler(this);

			m_grid = std::make_shared<GridNode>();
			m_scene->getRoot().lock()->addChild(m_grid);

			m_rootActor = ActorComponentFactory::Instance().createActor("ActorBase");
			m_rootActor->setScene(m_scene);

			auto transformComponent = std::make_shared<TransformComponent>();
			transformComponent->setOwner(m_rootActor);
			m_rootActor->addComponent(transformComponent);

			return true;
		}

		virtual void deinit() override
		{
			Input::Instance().unregisterMouseHandler(this);
			Input::Instance().unregisterKeyboardHandler(this);
			EventSystem::Instance().remove(m_delegate, Event_ResourceLoaded::Type);
			EventSystem::Instance().remove(m_actorAddedDelegate, Event_ActorAdded::Type);
			g_editor = nullptr;
			m_envSettings.Clear();
		}

		virtual void deviceLost() override
		{
		}

		virtual S32 render(F64 time, F32 delta) override
		{
			ID3D11DeviceContext* context = DXUTGetD3D11DeviceContext();
			ID3D11Device* device = DXUTGetD3D11Device();

			float ClearColor[4] = { 0.0f, 0.25f, 0.25f, 0.55f };
			if (m_activeViewMode == ViewMode::Preview)
			{
				ClearColor[0] = 0.15f;
			}

			auto backBuffer = m_renderer->getBackBuffer();
			m_renderer->clear(Renderer::ClearFlags::CF_CLEAR_COLOR | Renderer::ClearFlags::CF_CLEAR_DEPTH, backBuffer, ClearColor, backBuffer, 1.0f, 0);

			m_camera->update(delta);

			m_renderer->renderScene(delta);

			auto& txtHelper = DXUT::GetTextHelper();
			wchar_t buf[50];
			if (m_activeViewMode == ViewMode::Level)
			{
				wsprintf(buf, L"ViewMode: Level");
			}
			else
			{
				wsprintf(buf, L"ViewMode: Preview");
			}
			txtHelper.Begin();
			txtHelper.SetInsertionPos(2, 0);
			txtHelper.SetForegroundColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			txtHelper.DrawTextLine(buf);
			txtHelper.SetForegroundColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
			txtHelper.DrawTextLine(DXUTGetFrameStats(DXUTIsVsyncEnabled()));
			txtHelper.DrawTextLine(DXUTGetDeviceStats());
			txtHelper.End();
			return 0;
		}

		virtual void update(F64 fTime, F32 fElapsedTime) override
		{
			m_rootActor->threadSafeUpdate();
			m_rootActor->update(fElapsedTime);

			if (m_clearPreviewModel)
			{
				m_clearPreviewModel = false;
				m_previewScene = std::make_shared<Scene>();
				if (tinyxml2::XMLElement* root = m_envSettings.RootElement())
				{
					m_previewScene->updateEnvironmentSettings(root);
				}
			}
			Scene::SceneStrongPtr scene = m_activeViewMode == ViewMode::Level ? m_scene : m_previewScene;
			m_renderer->setScene(scene);

			if (m_nodeToAdd)
			{
				std::shared_ptr<box::SceneNode> root;
				root = scene->getRoot().lock();
				root->addChild(m_nodeToAdd);
				m_nodeToAdd.reset();
			}
			m_renderer->cullObjects();
		}

		virtual AppMsg::Status msgProc(const AppMsg& msg) override
		{
			return AppMsg::Status::DefaultAction;
		}
	private:
		void resourceLoaded(EventSystem::StrongEventDataPtr in)
		{
			auto resource = castTo<Event_ResourceLoaded>(in);
			if (resource->getType() == Event_ResourceLoaded::ResourceType::Model)
			{
				auto handle = resource->getHandle();
				if (!handle || !handle->getExtra())
				{
					return;
				}

				m_previewModel = handle->getExtraTyped<Model>();

				m_nodeToAdd = std::make_shared<GraphicsNode>();
				m_nodeToAdd->setModel(m_previewModel);
			}
		}
		void actorAdded(EventSystem::StrongEventDataPtr event)
		{
			auto actorAddedEvent = castTo<Event_ActorAdded>(event);
			auto actor = actorAddedEvent->getActor();
			m_actorsMap[actor->getId()] = actor;
		}
	private:
		Renderer* m_renderer;
		Scene::SceneStrongPtr m_scene;
		Scene::SceneStrongPtr m_previewScene;
		Model::ModelStrongPtr m_previewModel;
		std::shared_ptr<GraphicsNode> m_nodeToAdd;
		Camera::CameraStrongPtr m_camera;
		EventSystem::DelegateType m_delegate;
		EventSystem::DelegateType m_actorAddedDelegate;
		F32 m_cameraMovementSpeed;
		bool m_renderViewActive;
		U32 m_prevMouseX;
		U32 m_prevMouseY;
		F32 m_cameraMouseSense;
		bool m_mouseButtonPressed;
		bool m_clearPreviewModel;
		ViewMode m_activeViewMode;
		tinyxml2::XMLDocument m_envSettings;
		std::shared_ptr<GridNode> m_grid;
		Actor::StrongActorPtr m_rootActor;
		std::map<Actor::ActorId, Actor::WeakActorPtr> m_actorsMap;
	};
}

namespace Exports
{
	using namespace box;
	Engine* g_engineInstance = nullptr;

#define CHECK_ENGINE() if (!g_engineInstance) { return -1; }
#define CHECK_EDITOR() if (!g_editor) { return -1; }

namespace System
{
	int StartEngine(void* hwnd, int argc, char** argv)
	{
		if (g_engineInstance)
		{
			return -1;
		}
		std::shared_ptr<EditorView> editor = std::make_shared<EditorView>();

		g_engineInstance = new box::Engine();
		g_engineInstance->startup(hwnd, argc, argv);
		g_engineInstance->attachGameView(editor);
		return 0;
	}

	int StopEngine()
	{
		CHECK_ENGINE();

		g_engineInstance->shutdown();
		delete g_engineInstance;
		g_engineInstance = nullptr;

		return 0;
	}

	int WndProc(void* hwnd, int msg, int wParam, int lParam)
	{
		CHECK_ENGINE();
		box::Input::Instance().poll(1.0f / 60.0f);
		DXUTStaticWndProc(static_cast<HWND>(hwnd), msg, wParam, lParam);
		return 0;
	}

	int Update(box::F32 delta)
	{
		CHECK_ENGINE();
		return 0;
	}

	int RenderFrame(box::F32 delta)
	{
		CHECK_ENGINE();

		DXUTRender3DEnvironment();
		return 0;
	}

	int Resize(int width, int height)
	{
		CHECK_ENGINE();

		Window::Instance().resize(width, height);
		return 0;
	}
}//System

namespace Input
{
	int OnKeyDown(U32 key)
	{
		CHECK_ENGINE();

	//	box::Input::Instance().onKeyDown(key);
		return 0;
	}

	int OnKeyUp(U32 key)
	{
		if (g_engineInstance)
		{
		//	box::Input::Instance().onKeyUp(key);
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

namespace Editor
{

	int SetEnabledCameraInput(int active)
	{
		CHECK_ENGINE();
		CHECK_EDITOR();

		g_editor->setRenderViewActive((active == 1) ? true : false);

		return 0;
	}

	int SetViewMode(int mode)
	{
		CHECK_ENGINE();
		CHECK_EDITOR();

		g_editor->setViewMode(mode == 0 ? EditorView::ViewMode::Level : EditorView::ViewMode::Preview);

		return 0;
	}

	int ClearPreviewModel()
	{
		CHECK_ENGINE();
		CHECK_EDITOR();

		g_editor->clearPreviewModel();

		return 0;
	}

	int SetCameraFov(float fov)
	{
		CHECK_ENGINE();
		CHECK_EDITOR();

		g_editor->setCameraFov(fov);

		return 0;
	}

	int SetCameraZNear(float zNear)
	{
		CHECK_ENGINE();
		CHECK_EDITOR();

		g_editor->setCameraZNear(zNear);

		return 0;
	}

	int SetCameraZFar(float zFar)
	{
		CHECK_ENGINE();
		CHECK_EDITOR();

		g_editor->setCameraZFar(zFar);

		return 0;
	}

	int SetCameraMovementSpeed(float speed)
	{
		CHECK_ENGINE();
		CHECK_EDITOR();

		g_editor->setCameraMovementSpeed(speed);

		return 0;
	}

	int AddPreviewModelToCollection(const char* modelName, const char* descrFileName, const char* srcFileName)
	{
		CHECK_ENGINE();
		CHECK_EDITOR();

		g_editor->addPreviewModelToCollection(modelName, descrFileName, srcFileName);

		return 0;
	}

	int AddModelToScene(const char* desc)
	{
		CHECK_ENGINE();
		CHECK_EDITOR();
		g_editor->addModelToScene(desc);
		return 0;
	}

	int LoadLevelFromXMLFile(const char* fileName)
	{
		CHECK_ENGINE();
		CHECK_EDITOR();
		g_editor->loadLevelFromXMLFile(fileName);
		return 0;
	}

	int LoadLevelFromXMLBuffer(const char* buffer)
	{
		CHECK_ENGINE();
		return 0;
	}

	int GetLevelSizeForXml()
	{
		CHECK_ENGINE();
		return 0;
	}

	int SaveLevelToXMLFile(const char* fileName)
	{
		CHECK_ENGINE();
		CHECK_EDITOR();
		g_editor->SaveLevelToXMLFile(fileName);
		return 0;
	}

	int SerializeLevelToXML(char* out)
	{
		CHECK_ENGINE();
		return 0;
	}

	int GetLevelInfoSizeForXml()
	{
		CHECK_ENGINE();
		return 0;

	}

	int SerializeLevelInfoToXml(char* out)
	{
		CHECK_ENGINE();
		return 0;
	}

	int UpdateEnvironmentSettings(const char* xml)
	{
		CHECK_ENGINE();
		CHECK_EDITOR();
		g_editor->updateEnvironmentSettings(xml);
		return 0;
	}

	int GetActorsXml(char* out, unsigned int buffMaxCapacity)
	{
		CHECK_ENGINE();
		CHECK_EDITOR();
		return g_editor->getActorsXml(out, buffMaxCapacity);
	}

	int GetActorInfo(unsigned int actorID, char* out, unsigned int buffMaxCapacity)
	{
		CHECK_ENGINE();
		CHECK_EDITOR();
		return g_editor->getActorInfo(actorID, out, buffMaxCapacity);
	}

	int UpdateActor(unsigned int actorID, const char* xml)
	{
		CHECK_ENGINE();
		CHECK_EDITOR();
		return g_editor->updateActor(actorID, xml);
	}

} // Editor

}
#endif