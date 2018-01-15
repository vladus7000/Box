#include "StdAfx.hpp"
#include "System\Threading\Semaphore.hpp"

#include <Windows.h>

namespace box
{
	struct Shadow
	{
		HANDLE semaphore;
	};

	Semaphore::Semaphore(U32 initCount, U32 maxCount)
	{
		Shadow* s = reinterpret_cast<Shadow*>(m_shadow);
		s->semaphore = NULL;
		s->semaphore = CreateSemaphoreEx(0, initCount, maxCount, 0, 0, SEMAPHORE_MODIFY_STATE | DELETE | SYNCHRONIZE);
	}

	Semaphore::~Semaphore()
	{
		Shadow* s = reinterpret_cast<Shadow*>(m_shadow);
		CloseHandle(s->semaphore);
	}

	void Semaphore::signal()
	{
		Shadow* s = reinterpret_cast<Shadow*>(m_shadow);
		ReleaseSemaphore(s->semaphore, 1, NULL);
	}

	void Semaphore::wait()
	{
		Shadow* s = reinterpret_cast<Shadow*>(m_shadow);
		WaitForSingleObject(s->semaphore, INFINITE);
	}

	bool Semaphore::tryWait()
	{
		Shadow* s = reinterpret_cast<Shadow*>(m_shadow);
		return WaitForSingleObject(s->semaphore, 0) == WAIT_OBJECT_0;
	}

}
