#include "StdAfx.hpp"

#include <stdio.h>

#include <System/Engine.hpp>
#include "System/Memory/Allocator.hpp"
#include "System/RunEnvironment.hpp"
#include "System/ResourceSystem/ResourceManager.hpp"
#include "Window\Window.hpp"
#include "System\Threading\ThreadManager.hpp"
#include "System\Input.hpp"
#include "Scripting\ScriptingManager.hpp"
#include "Render\Renderer.hpp"
#include "Sound\SoundSystem.hpp"
#include "System\RandomGenerator.hpp"
#include "Cheat\CheatManager.hpp"
#include "Network\NetworkManager.hpp"
#include "System\EventSystem\EventSystem.hpp"
#include "System\Process\ProcessManager.hpp"
#include "System\EventSystem\EngineEvents.hpp"
#include "Gameplay\GameView.hpp"
#include <DXUT11\Core\DXUT.h>

#include <vector>
#include "Render\DXUTHelper.hpp"

namespace box
{
	std::vector<std::shared_ptr<GameView>> g_gameViews;

	void CALLBACK OnFrameMove(double fTime, float fElapsedTime, void* pUserContext)
	{
		for (auto& it : g_gameViews)
		{
			it->update(fTime, fElapsedTime);
		}
	}

	LRESULT CALLBACK MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing,
		void* pUserContext)
	{
		AppMsg msg;
		msg.hwnd = hWnd;
		msg.uMsg = uMsg;
		msg.lParam = lParam;
		msg.wParam = wParam;

		if (DXUT::MsgProc(msg) == AppMsg::Status::Processed)
		{
			*pbNoFurtherProcessing = true;
			return S_OK;
		}

		// TODO: sort gameViews;
		for (auto& it : g_gameViews)
		{
			if (it->msgProc(msg) == AppMsg::Status::Processed)
			{
				*pbNoFurtherProcessing = true;
				return S_OK;
			}
		}
		return S_OK;
	}

	Engine::Engine()
	{
	}

	Engine::~Engine()
	{
	}

	bool Engine::startup(
#ifdef EDITOR_BUILD
		void* hwnd,
#endif
		U32 argc, char** argv)
	{
		bool result = true;
		result &= m_machineInfo.detect();

		{// warming up singletons
			DXUTInit(true, true, NULL);
			DXUTSetCallbackFrameMove(OnFrameMove);
			DXUTSetCallbackMsgProc(MsgProc);

			Allocator::Instance();
			RunEnvironment::Instance();
			ResourceManager::Instance();

#if defined(DEBUG) | defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
			Window::Instance();
			ProcessManager::Instance();
			ThreadManager::Instance();
			Input::Instance();
			ScriptingManager::Instance();
			Renderer::Instance();
			SoundSystem::Instance();
			NetworkManager::Instance();
			CheatManager::Instance();
			EventSystem::Instance();
		}
		result &= Allocator::Instance().init();
		result &= EventSystem::Instance().init();
		result &= RunEnvironment::Instance().init(argc, argv);
		result &= ResourceManager::Instance().init();
		
#ifdef GAME_BUILD
		result &= Window::Instance().init();
#endif
#ifdef EDITOR_BUILD
		result &= Window::Instance().init(hwnd);
#endif
		result &= ThreadManager::Instance().init();
		result &= Input::Instance().init();
		result &= ScriptingManager::Instance().init();
		result &= Renderer::Instance().init();
		result &= SoundSystem::Instance().init();
		result &= NetworkManager::Instance().init();
		result &= RandomGenerator::init();
		result &= CheatManager::Instance().init();

		{
			std::shared_ptr<Event_EngineStasted> event(new Event_EngineStasted);
			EventSystem::Instance().raiseEvent(event);
		}
		return result;
	}

	void Engine::shutdown()
	{
		CheatManager::Instance().deinit();
		RandomGenerator::deinit();
		NetworkManager::Instance().deinit();
		SoundSystem::Instance().deinit();
		Renderer::Instance().deinit();
		ScriptingManager::Instance().deinit();
		Input::Instance().deinit();
		ThreadManager::Instance().abortAllThreads(true);
		ThreadManager::Instance().deinit();
#ifdef GAME_BUILD
		Window::Instance().deinit();
		ProcessManager::Instance().abortAllProcesses(true);
#endif
		ResourceManager::Instance().deinit();
		RunEnvironment::Instance().deinit();
		EventSystem::Instance().deinit();
		Allocator::Instance().deinit();
		DXUTShutdown();
	}

	void Engine::attachGameView(std::shared_ptr<GameView> view)
	{
		if (view)
		{
			if (view->restore())
			{
				g_gameViews.push_back(view);
			}
		}
	}

	void Engine::enterMainLoop()
	{
		if (g_gameViews.size() > 0)
		{
			DXUTMainLoop();
			for (auto& it : g_gameViews)
			{
				it->deinit();
			}
		}
		g_gameViews.clear();
	}
}