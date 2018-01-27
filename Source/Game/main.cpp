#include <StdAfx.hpp>
#include <stdio.h>
#include <System/Engine.hpp>
#include <Window/Window.hpp>
#include "System/Process/ProcessManager.hpp"
#include "System\Input.hpp"
#include "System\ResourceSystem\ResourceManager.hpp"
#include "System\EventSystem\EventSystem.hpp"
#include "System\EventSystem\EngineEvents.hpp"
#include "System\Memory\Allocator.hpp"
#include "System\Threading\LockGuard.hpp"
#include "System\Threading\CriticalSection.hpp"

#include "System\Threading\Thread.hpp"
#include "System\Threading\Semaphore.hpp"
#include "Gameplay\GameView.hpp"

#include <DXUT11\Core\DXUT.h>
#include "DXUT11\Optional\DXUTgui.h"
#include "DXUT11\Optional\SDKmisc.h"
#include "Render\DXUTHelper.hpp"
#define RELEASE(x) if (x) { x->Release(); x = nullptr; }
#include "Render\Shader.hpp"
#include "UI/Dialog.hpp"

using namespace box;

class BoomEvent : public EventData
{
public:
	virtual const EventType& getType() const override
	{
		return 1;
	}

	virtual const char* getName() override
	{
		return "BoomEvent";
	}
};

class DelayedProcess : public Process
{
public:
	DelayedProcess(F32 delay)
		: m_delay(delay)
		, m_currentTime(0.0f)
	{}
	~DelayedProcess()
	{
		printf("~DelayedProcess()\n");
	}

protected:
	virtual void onUpdate(F32 delta) override
	{
		m_currentTime += delta;
		if (m_currentTime > m_delay)
		{
			succeeded();
		}
	}

private:
	F32 m_delay;
	F32 m_currentTime;
};

class Boom : public Process
{
public:
	~Boom()
	{
		printf("~Boom()\n");
	}
protected:
	virtual void onUpdate(F32 delta) override
	{
		succeeded();
		
		std::shared_ptr<BoomEvent> boom(new BoomEvent);
		EventSystem::Instance().raiseEvent(boom);
	}

};

void printBoom(std::shared_ptr<EventData> event)
{
	printf(" printBoom Boom!!!()\n");
	Process::StrongProcessPtr delay(new DelayedProcess(10.0f));
	Process::StrongProcessPtr boom(new Boom());
	delay->attachChild(boom);
	ProcessManager::Instance().attachProcess(delay);
}

void resLoaded(std::shared_ptr<EventData> event)
{
	printf(" Resourse loaded!!!()\n");
}

void CALLBACK OnGUIEvent(UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext)
{
}

class Game : public box::GameView
{
public:
	virtual void deviceLost() override {}
	virtual S32 render(F64 time, F32 delta) override { return 0; }

	virtual void update(F64 fTime, F32 fElapsedTime) override
	{
		auto stat = Allocator::Instance().getStats();
		if (stat.allocatedMemory != oldstat.allocatedMemory)
		{
			oldstat = stat;
			printf("aloc %d, aloc+ %d \n", stat.allocatedMemory, stat.allocatedMemoryWithOverhead);
		}
		
		Input::Instance().poll(1.0f / 60.0f);
		ProcessManager::Instance().update(1.0f / 60.0f);
		Sleep(16);

		float ClearColor[4] = { 0.0f, 0.25f, 0.25f, 0.55f };
		ID3D11RenderTargetView* pRTV = DXUTGetD3D11RenderTargetView();
		context->ClearRenderTargetView(pRTV, ClearColor);

		ID3D11DepthStencilView* pDSV = DXUTGetD3D11DepthStencilView();
		context->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH, 1.0, 0);



		if (m_yesNoDialog.isVisible())
		{
			m_yesNoDialog.render(fTime, fElapsedTime);
			if (m_yesNoDialog.getAnswer() == Dialog::Answer::Yes)
			{
				exit(0);
			}
			else if (m_yesNoDialog.getAnswer() == Dialog::Answer::No)
			{
				m_yesNoDialog.setVisible(false);
				m_yesNoDialog.resetAnswer();
			}
			return;
		}

		HUD.OnRender(fElapsedTime);
		auto& txtHelper = box::DXUT::GetTextHelper();
		txtHelper.Begin();
		txtHelper.SetInsertionPos(2, 0);
		txtHelper.SetForegroundColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
		txtHelper.DrawTextLine(DXUTGetFrameStats(DXUTIsVsyncEnabled()));
		txtHelper.DrawTextLine(DXUTGetDeviceStats());
		txtHelper.SetForegroundColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		txtHelper.DrawTextLine(L"Press F1 for help");
		txtHelper.End();
	}

	virtual void deinit() override
	{
	}

	virtual AppMsg::Status msgProc(const AppMsg& msg) override
	{
		switch (msg.uMsg)
		{
		case WM_CLOSE:
			m_yesNoDialog.setVisible(true);
			return AppMsg::Status::Processed;
			break;
		}
		
		if (HUD.MsgProc(msg.hwnd, msg.uMsg, msg.wParam, msg.lParam))
		{
			return AppMsg::Status::Processed;
		}

		if (m_yesNoDialog.msgProc(msg) == AppMsg::Status::Processed)
		{
			return AppMsg::Status::Processed;
		}

		return AppMsg::Status::DefaultAction;
	}

	virtual bool restore() override
	{
		context = DXUTGetD3D11DeviceContext();
		device = DXUTGetD3D11Device();

		m_yesNoDialog.restore();
		m_yesNoDialog.setVisible(false);
		auto& dialogResourceManager = box::DXUT::GetDialogResourceManager();
		HUD.Init(&dialogResourceManager);
		HUD.SetCallback(OnGUIEvent); int iY = 50;
		HUD.AddButton(1, L"Toggle full screen", 50, iY, 170, 23);
		HUD.SetLocation(20, 0);
		HUD.SetSize(170, 170);

		typedef fastdelegate::FastDelegate1<std::shared_ptr<EventData>> MyDelegateT;
		MyDelegateT MyDelegate(&printBoom);
		EventSystem::Instance().add(MyDelegate, 1);

		MyDelegateT MyDelegate2(&resLoaded);
		EventSystem::Instance().add(MyDelegate2, Event_ResourceLoaded::Type);

		ResourceManager::Instance().preload("*.sb");
		ResourceManager::Instance().preload("*.txt");
		ResourceManager::Instance().preload("*.fbx");
		ResourceManager::Instance().preload("*.bnk");
		ResourceManager::Instance().preload("*.png");

		{
			Resource r("mpvehicles.sb");
			auto handle = ResourceManager::Instance().getHandle(r);
		}

		{
			Resource r("Readme.txt");
			auto handle = ResourceManager::Instance().getHandle(r);
		}

		{
			Resource r("untitled.fbx");
			auto handle = ResourceManager::Instance().getHandle(r);
		}

		{
			Resource r("Generic.bnk");
			auto handle = ResourceManager::Instance().getHandle(r);
		}
		
		{
			Resource r("Syberia3_Wallpaper4.png");
			auto handle = ResourceManager::Instance().getHandle(r, true);
		}
		{
			{
				Resource r("Syberia3_Wallpaper4.png");
				auto handle = ResourceManager::Instance().getHandle(r);
			}

			{
				Shader s("desc/lighting/phong.shader");
				s.restore();
			}
		}
		auto& window = Window::Instance();
		{
			Process::StrongProcessPtr delay(new DelayedProcess(10.0f));
			Process::StrongProcessPtr boom(new Boom());
			delay->attachChild(boom);
			ProcessManager::Instance().attachProcess(delay);
		}
		Resource r("Syberia3_Wallpaper4.png");
		auto handle = ResourceManager::Instance().getHandle(r);
		oldstat.allocatedMemory = 0;
		oldstat.allocatedMemoryWithOverhead = 0;

		return true;
	}

	Allocator::MemoryStats oldstat;
	ID3D11DeviceContext* context;
	ID3D11Device* device;
	CDXUTDialog HUD;
	Dialog m_yesNoDialog;
};

int g_c = 0;
bool stop1 = false;
box::CriticalSection g_s;

class RenderThread : public box::Thread
{
public:
	RenderThread(box::Semaphore& startTick, box::Semaphore& endTick)
		: m_startTick(startTick)
		, m_endTick(endTick)
		,Thread("RenderThread")
	{
		printf("RenderThread created\n");
	}
	~RenderThread()
	{
		printf("RenderThread destroyed\n");
	}
	virtual int run() override
	{
		while (!stop1)
		{
			printf("RenderThread Wait!!!!!!!!!!\n");
			m_startTick.wait();
			if (stop1) break;
			printf("RenderThread do work!!!!!!!!!!\n");
			Sleep(100);
			m_endTick.signal();
		}
		return 0;
	}
	box::Semaphore& m_startTick;
	box::Semaphore& m_endTick;
};

class PhysicsTread : public box::Thread
{
public:
	PhysicsTread(box::Semaphore& startTick, box::Semaphore& endTick)
		: m_startTick(startTick)
		, m_endTick(endTick)
		, Thread("PhysicsTread")
	{
		printf("PhysicsTread created\n");
	}
	~PhysicsTread()
	{
		printf("PhysicsTread destroyed\n");
	}
	virtual int run() override
	{
		while (!stop1)
		{
			printf("PhysicsTread Wait!!!!!!!!!!\n");
			m_startTick.wait();
			if (stop1) break;
			printf("PhysicsTread do work!!!!!!!!!!\n");
			Sleep(200);
			m_endTick.signal();
		}
		return 0;
	}
	box::Semaphore& m_startTick;
	box::Semaphore& m_endTick;
};

int main(int argc, char** argv)
{
	{
		box::CriticalSection s;
		//{
		//	box::LockGuard<box::CriticalSection> g(s);
		//}
		box::Engine engine;
		/*{
			box::Semaphore startTick;
			box::Semaphore renderSema;
			box::Semaphore physSema;
			Thread::StrongThreadPtr t(new RenderThread(startTick, renderSema));
			t->init();
			t->start();

			Thread::StrongThreadPtr t2(new PhysicsTread(startTick, physSema));
			t2->init();
			t2->start();


			Sleep(10);
			for (int i = 0; i < 10; i++)
			{
				printf("Save!!!!!!!!!!\n");
				startTick.signal();
				startTick.signal();
				printf("main thread do work!!!!!!!!!!\n");
				Sleep(300);
				renderSema.wait();
				physSema.wait();

			}
			stop1 = true;
			startTick.signal();
			startTick.signal();
		}*/
		engine.startup(argc, argv);
		{
			std::shared_ptr<Game> game(new Game);
			engine.attachGameView(game);
			engine.enterMainLoop();
		}
		engine.shutdown();
	}
	
}