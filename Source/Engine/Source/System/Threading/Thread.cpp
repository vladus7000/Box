#include "StdAfx.hpp"
#include "System\Threading\Thread.hpp"

#include <Windows.h>

namespace box
{

	typedef struct tagTHREADNAME_INFO
	{

		DWORD dwType; // must be 0x1000
		LPCSTR szName; // pointer to name (in user addr space)
		DWORD dwThreadID; // thread ID (-1=caller thread)
		DWORD dwFlags; // reserved for future use, must be zero

	} THREADNAME_INFO;

	void SetThreadName(DWORD dwThreadID, LPCSTR szThreadName)
	{
		THREADNAME_INFO info;
		info.dwType = 0x1000;
		info.szName = szThreadName;
		info.dwThreadID = dwThreadID;
		info.dwFlags = 0;
		__try
		{
			RaiseException(0x406D1388, 0, sizeof(info) / sizeof(DWORD), (ULONG_PTR*)&info);
		}
		__except (EXCEPTION_CONTINUE_EXECUTION)
		{
		}
	}

	struct Shadow
	{
		HANDLE event;
		HANDLE thread;
		DWORD id;
		S32 res;
	};

	int PriorityMapper(Thread::Priority p)
	{
		int result = 0;
		switch (p)
		{
		case Thread::Priority::TimeCritical:
			result = 15;
			break;
		case Thread::Priority::Higest:
			result = 2;
			break;
		case Thread::Priority::AboveNormal:
			result = 1;
			break;
		case Thread::Priority::Normal:
			result = 0;
			break;
		case Thread::Priority::BelowNormal:
			result = -1;
			break;
		case Thread::Priority::Lowest:
			result = -2;
			break;
		case Thread::Priority::Idle:
			result = -15;
			break;
		default:
			result = 0;
			break;
		}
		return result;
	}

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
			const auto& name = This->getName();
			U64 mask = This->getAffinityMask();
			SetThreadName(-1, name.c_str());
			SetThreadAffinityMask(GetCurrentThread(), mask);
			s->res = This->run();
		}
		This->stop();
		return 0;
	}

	Thread::Thread(const std::string& name, U32 stackSize, U64 mask, Priority priority)
		: m_name(name)
		, m_stackSize(stackSize)
		, m_affinityMask(mask)
		, m_state(State::Uninited)
		, m_priority(priority)
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
		if (m_state == State::Uninited)
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
				m_stackSize,
				ThreadFunction,
				this,
				0, // use default creation flags 
				&id);
			SetThreadPriority(thread, PriorityMapper(m_priority));
			s->thread = thread;
			s->id = id;
			m_state = State::Inited;
		}
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
