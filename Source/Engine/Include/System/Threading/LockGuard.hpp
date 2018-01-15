#pragma once

namespace box
{
	template <class T>
	class LockGuard
	{
	public:
		LockGuard(T& _lock) : lock(_lock) { lock.Lock(); }
		~LockGuard() { lock.Unlock(); }
	private:
		LockGuard(const LockGuard&);
		LockGuard(LockGuard&&);
		LockGuard& operator=(const LockGuard& o);
		LockGuard&operator=(LockGuard&& o);

		T& lock;
	};
}
