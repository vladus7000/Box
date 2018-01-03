#include <StdAfx.hpp>
#include <stdio.h>
#include <System/Engine.hpp>
#include <Window/Window.hpp>
#include "System/Process/ProcessManager.hpp"
#include "System\Input.hpp"

using namespace box;

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
		printf("Boom!!!()\n");
	}

};

class Game
{
public:
	void run()
	{
		auto& window = Window::Instance();
		{
			Process::StrongProcessPtr delay(new DelayedProcess(2.0f));
			Process::StrongProcessPtr boom(new Boom());
			delay->attachChild(boom);
			ProcessManager::Instance().attachProcess(delay);
		}

		while (!window.windowIsClosed())
		{
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