#pragma once

namespace box
{
	class CriticalSection
	{
	public:
		CriticalSection();
		~CriticalSection();

		void Lock();
		void Unlock();
		bool TryLock();

	private:
		CriticalSection(const CriticalSection&);
		CriticalSection(CriticalSection&&);
		CriticalSection& operator=(const CriticalSection& o);
		CriticalSection&operator=(CriticalSection&& o);

		U8 m_shadow[64];
	};
}
