#include "StdAfx.hpp"
#include "System\Threading\CriticalSection.hpp"

#include <Windows.h>

namespace box
{
	struct Shadow
	{
		CRITICAL_SECTION criticalSection;
	};

	CriticalSection::CriticalSection()
	{
		Shadow* s = reinterpret_cast<Shadow*>(m_shadow);
		InitializeCriticalSection(&s->criticalSection);
	}

	CriticalSection::~CriticalSection()
	{
		Shadow* s = reinterpret_cast<Shadow*>(m_shadow);
		DeleteCriticalSection(&s->criticalSection);
	}

	void CriticalSection::Lock()
	{
		Shadow* s = reinterpret_cast<Shadow*>(m_shadow);
		EnterCriticalSection(&s->criticalSection);
	}

	void CriticalSection::Unlock()
	{
		Shadow* s = reinterpret_cast<Shadow*>(m_shadow);
		LeaveCriticalSection(&s->criticalSection);
	}

	bool CriticalSection::TryLock()
	{
		Shadow* s = reinterpret_cast<Shadow*>(m_shadow);
		return static_cast<bool>(TryEnterCriticalSection(&s->criticalSection));
	}
}
