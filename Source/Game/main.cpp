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

class Game
{
public:
	void run()
	{
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
		Allocator::MemoryStats oldstat;
		oldstat.allocatedMemory = 0;
		oldstat.allocatedMemoryWithOverhead = 0;
		while (!window.windowIsClosed())
		{
			auto stat = Allocator::Instance().getStats();
			if (stat.allocatedMemory != oldstat.allocatedMemory)
			{
				oldstat = stat;
				printf("aloc %d, aloc+ %d \n", stat.allocatedMemory, stat.allocatedMemoryWithOverhead);
			}
			window.pollEvents();
			Input::Instance().poll(1.0f / 60.0f);
			ProcessManager::Instance().update(1.0f / 60.0f);
			Sleep(16);
		}
	}
};

int main(int argc, char** argv)
{
	{
		box::Engine engine;
		engine.startup(argc, argv);
		{
			Game game;
			game.run();
		}
		engine.shutdown();
	}
	
}