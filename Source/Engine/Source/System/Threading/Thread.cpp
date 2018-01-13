#include "StdAfx.hpp"
#include "System\Threading\Thread.hpp"

#include <Windows.h>

namespace box
{
	struct Shadow
	{
		HANDLE event;
		HANDLE thread;
		DWORD id;
		S32 res;
	};

	DWORD WINAPI ThreadFunction(LPVOID lpParam)
	{
		Thread* This = reinterpret_cast<Thread*>(lpParam);
		if (!This)
		{
			return 0;
		}

		Shadow* s = reinterpret_cast<Shadow*>(This->getShadow());
		HANDLE e = s->event;
		WaitForSingleObject(e, INFINITE);
		if (This->getState() != Thread::State::Uninited && This->getState() != Thread::State::Stopped)
		{
			ResetEvent(e);
			s->res = This->run();
		}
		This->stop();
		return 0;
	}

	Thread::Thread(const std::string& name, U32 stackSize, U32 mask)
		: m_name(name)
		, m_stackSize(stackSize)
		, m_affinityMask(mask)
		, m_state(State::Uninited)
	{
	}

	Thread::~Thread()
	{
		stop();
	}

	S32 Thread::getResult()
	{
		Shadow* s = reinterpret_cast<Shadow*>(m_shadow);
		return s->res;
	}

	void Thread::init()
	{
		static U32 n = 0;
		n++;
		std::string eventName("__ThreadEvent");
		eventName += n;
		std::wstring nameW(eventName.begin(), eventName.end());
		HANDLE event;
		event = CreateEvent(NULL, TRUE, FALSE, nameW.c_str());

		Shadow* s = reinterpret_cast<Shadow*>(m_shadow);
		s->res = 0;
		s->event = event;

		DWORD id;
		HANDLE thread = CreateThread(NULL, // default security attributes
			0,             // use default stack size  
			ThreadFunction,// thread function name
			this,          // argument to thread function 
			0,             // use default creation flags 
			&id);

		s->thread = thread;
		s->id = id;
		m_state = State::Inited;
	}

	void Thread::start()
	{
		if (m_state != State::Running && m_state == State::Inited)
		{
			m_state = State::Running;
			Shadow* s = reinterpret_cast<Shadow*>(m_shadow);
			HANDLE e = s->event;
			SetEvent(e);
		}
	}

	void Thread::stop()
	{
		if (m_state != State::Stopped && (m_state == State::Running || m_state == State::Paused))
		{
			m_state = State::Stopped;

			Shadow* s = reinterpret_cast<Shadow*>(m_shadow);
			HANDLE h = s->thread;
			HANDLE e = s->event;
			WaitForMultipleObjects(1, &h, TRUE, INFINITE);
			CloseHandle(e);
			CloseHandle(h);
		}
	}

	void Thread::pause()
	{
		if (m_state == State::Running)
		{
			m_state = State::Paused;
		}
	}

	void Thread::unPause()
	{
		if (m_state == State::Paused)
		{
			m_state = State::Running;
		}
	}
}